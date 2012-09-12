/*
-------------------------------------------------------------------------
OBJECT NAME:	nc2iwg1.cc

FULL NAME:	netCDF to IWG1

DESCRIPTION:	Produce IWG1 packets from a netCDF file.  Either output
		to stdout or UDP 7071 with a 1 sec delay.

COPYRIGHT:	University Corporation for Atmospheric Research, 2011
-------------------------------------------------------------------------
*/

#include <QApplication>
#include <QUdpSocket>

#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>
#include <vector>

#include <netcdfcpp.h>

using namespace std;

struct _var
{
  string name;
  NcVar *var;
  float *data;
};

static const int IWG1_PORT = 7071;

static const char *varList[] = {
	"Time", "GGLAT", "GGLON", "GGALT", "NAVAIL", "PALTF", "HGM232",
	"GSF", "TASX", "IAS", "MACH_A", "VSPD", "THDG", "TKAT", "DRFTA",
	"PITCH", "ROLL", "SSLIP", "ATTACK", "ATX", "DPXC", "TTX", "PSXC",
	"QCXC", "PCAB", "WSC", "WDC", "WIC", "SOLZE", "Solar_El_AC",
	"SOLAZ", "Sun_Az_AC", NULL };

static bool udpOut = false;
static useconds_t udpSendDelay = 1000000;	// for udp output only.

void processArgs(char **argv);
string formatIWG1(vector <struct _var> &vars, size_t index);

/* -------------------------------------------------------------------- */
void
usage()
{
  cerr << "netCDF to IWG1.\n  Usage: nc2iwg1 [-u [-d usec]] file.nc\n";
  cerr << "    -u: output udp broadcast packets on port 7071.\n";
  cerr << "    -d: delay in microseconds between sending udp packets.\n\n";
  cerr << "    Read a netCDF file [file.nc] and output IWG1 strings to stdout.\n";
  cerr << "    Optionally the IWG1 packets can be broadcast.\n";
  exit(1);
}

int
main(int argc, char *argv[])
{
  QUdpSocket *sock = 0;

  QApplication app(argc, argv, false);

  if (argc < 2)
    usage();

  processArgs(argv);

  NcFile inFile(argv[argc-1]);

  if (!inFile.is_valid())
  {
    cerr << "nc2iwg1: Failed to open input file, exiting.\n";
    return 1;
  }

  putenv((char *)"TZ=UTC");     // Perform all time calculations at UTC.
  NcError * ncErr = new NcError(NcError::silent_nonfatal);

  struct _var tvar;
  vector<struct _var> iwg1Vars;
  for (int i = 0; varList[i]; ++i)
  {
    tvar.name = varList[i];
    tvar.var = inFile.get_var(varList[i]);

    if (tvar.var && tvar.var->is_valid())
    {
      cerr << "Reading " << tvar.name;
      tvar.data = new float[tvar.var->num_vals()];
      tvar.var->get(tvar.data, tvar.var->edges());
      cerr << endl;
    }
    else
      cerr << tvar.name << " not found.\n";

    iwg1Vars.push_back(tvar);
  }

  if (udpOut)
    sock = new QUdpSocket();

  for (int i = 0; i < iwg1Vars[0].var->num_vals(); ++i)
  {
    if (udpOut)
    {
      string iwg1 = formatIWG1(iwg1Vars, i);
      sock->writeDatagram(	iwg1.c_str(), iwg1.length(),
				QHostAddress::Broadcast, IWG1_PORT);
      iwg1.resize(20);
      cout << iwg1 << '\r' << flush;
      usleep(udpSendDelay);
    }
    else
      cout << formatIWG1(iwg1Vars, i) << endl;
  }
}


time_t
startTime(NcVar *var)
{
  static time_t st = 0;

  if (st == 0)
  {
    // Get start time from Time units attribute.
    struct tm ft;
    NcAtt *time_units = var->get_att("units");
    strptime(time_units->as_string(0), "seconds since %F %T %z", &ft);
    st = mktime(&ft);
  }

  return st;
}


string
formatTime(struct _var tVar, size_t index)
{
  char buffer[128];
  // Get start time, add Time data offset, and generate an ascii timestamp.
  time_t recordTime = startTime(tVar.var) + (time_t)tVar.data[index];
  struct tm *rt = gmtime(&recordTime);
  strftime(buffer, 128, "%Y%m%dT%H%M%S", rt);
  return buffer;
}


string
formatIWG1(vector <struct _var> &vars, size_t index)
{
  std::stringstream temp;

  temp << "IWG1," << formatTime(vars[0], index);

  for (size_t i = 1; i < vars.size(); ++i)
  {
    temp << ",";
    if (vars[i].var && vars[i].var->is_valid())
      temp << vars[i].data[index];
  }

  return temp.str();
}


void
processArgs(char **argv)
{
  while (*++argv)
    if ((*argv)[0] == '-')
      switch ((*argv)[1])
        {
        case 'h':
          usage();
          break;

        case 'u':
          udpOut = true;
          break;

        case 'd':
          udpSendDelay = atoi(*++argv);
          break;
        }
}

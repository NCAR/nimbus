
#include "sync_reader.hh"
#include "timeseg.h" // for BEG_OF_TAPE, END_OF_TAPE

#include <nidas/core/Socket.h>
#include <nidas/util/Logger.h>
#include <nidas/dynld/raf/SyncServer.h>
#include <nidas/dynld/raf/SyncRecordSource.h>
#include <nidas/core/DSMSensor.h>
#include <nidas/core/SampleTag.h>
#include <nidas/core/VariableConverter.h>
#include <nidas/core/XMLParser.h>
#include <nidas/core/Project.h>
#include <nidas/util/Process.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

#include <vector>
#include <algorithm>
#include <stdexcept>

using std::vector;
using nidas::core::Polynomial;
using nidas::core::Project;
using nidas::core::Variable;

#include "nimbus.h"

namespace 
{
  char sync_server_pipe[80] = "";
  pid_t syncPID = -1;
  nidas::dynld::raf::SyncRecordReader* syncRecReader = 0;
  nidas::dynld::raf::SyncServer* syncServer = 0;
  std::string sync_xml_path;
}


void
SetSyncXMLPath(const std::string& path)
{
  sync_xml_path = path;
}


/* -------------------------------------------------------------------- */
/* Check if sync_server has exited or died, report status */
void
CheckSyncServer()
{
  if (syncPID > 0) {
    int status;
    int res = waitpid(syncPID,&status,WNOHANG);
    if (res < 0) WLOG(("error waiting on sync_server, pid=%d: %m",syncPID));
    else if (res > 0) {
      if (WIFEXITED(status))
        ILOG(("sync_server finished, status=%d",WEXITSTATUS(status)));
      else if (WIFSIGNALED(status)) {
        int sig = WTERMSIG(status);
        PLOG(("sync_server terminated on signal %s(%d)",strsignal(sig),sig));
      }
      syncPID = -1;
    }
  }
}

void
ShutdownSyncServer()
{
  CheckSyncServer();
  // send SIGTERM to sync_server. If it doesn't shutdown send SIGKILL.
  for (int i = 0; syncPID > 0 && i < 5; i++) {
    kill(syncPID, SIGTERM);
    sleep(1);
    CheckSyncServer();
  }
  if (syncPID > 0) {
    kill(syncPID, SIGKILL);
    sleep(1);
    CheckSyncServer();
  }

  if (strlen(sync_server_pipe))
    unlink(sync_server_pipe);

  if (syncServer)
  {
    syncServer->interrupt();
    syncServer->join();
  }
}


void
CloseSyncReader()
{
  ShutdownSyncServer();
  delete syncRecReader;
  syncRecReader = 0;
  delete syncServer;
  syncServer = 0;
  // Destroy the project created and initialized by the SyncServer.
  Project::destroyInstance();
  nidas::core::XMLImplementation::terminate();
}


nidas::dynld::raf::SyncRecordReader* 
GetSyncReader()
{
  return syncRecReader;
}


void
SyncReaderSetTimeWindow(time_t begin, time_t end)
{
  if (syncServer)
  {
    nidas::util::UTime ustart, uend;

    syncServer->getTimeWindow(&ustart, &uend);
    // Expand the time window at both ends to account for samples and
    // variables which may end up being shifted in time, and to allow the
    // nimbus buffers to fill up.
    if (begin != BEG_OF_TAPE)
    {
      begin -= 5*60;
      ustart = nidas::util::UTime(begin);
    }
    if (end != END_OF_TAPE)
    {
      // I'm not sure this is really needed, unless a sync record EOF is
      // handled differently in nimbus processing than hitting the end
      // time.
      end += 2*60;
      uend = nidas::util::UTime(end);
    }
    // For some as yet unknown reason, using SyncServer::setTimeWindow()
    // causes the difference tests to fail for the F2DIO variables in the
    // WINTER project (and maybe others).  So for now, disable this
    // feature.
    //
    // syncServer->setTimeWindow(ustart, uend);
  }    
}

// When post-processing, this starts the SyncServer thread which will read
// samples into the processing chain and feed them to the SyncRecordReader.
// Starting requires a separate call to account for interactive use, where
// the user can change calibrations or the time window after loading the
// project configuration.  In that case the start happens in in
// FindFirstLogicalADS3().
nidas::dynld::raf::SyncRecordReader* 
StartReadingSyncRecords()
{
  // Make sure reader is running if not started yet.
  if (syncRecReader)
  {
    if (syncServer && !syncServer->isRunning())
    {
      ILOG(("Starting syncServer thread to begin reading sync records..."));
      syncServer->start();
    }
  }
  return syncRecReader;
}


void
StartSyncServerProcess(const std::set<std::string>& headerfiles,
		       const std::string& xml_path)
{
  ILOG(("StartSyncServerProcess: header_file=%s", (*headerfiles.begin()).c_str()));

  strcpy(sync_server_pipe, "/tmp/sync_server_XXXXXX");
  mktemp(sync_server_pipe);

  std::vector<std::string> args;
  if (getenv("NIMBUS_SYNC_SERVER_VALGRIND"))
  {
    const char* vg[] = { "valgrind" };
    std::copy(vg, vg+sizeof(vg)/sizeof(vg[0]), std::back_inserter(args));
  }
  if (getenv("NIMBUS_SYNC_SERVER_GDB"))
  {
    const char* gdb[] = { "xterm", "-e", "gdb", "-ex", "run", "--args" };
    std::copy(gdb, gdb+sizeof(gdb)/sizeof(gdb[0]), std::back_inserter(args));
  }
  args.push_back("sync_server");
  if (getenv("NIMBUS_SYNC_SERVER_LOG_DEBUG"))
  {
    args.push_back("--loglevel");
    args.push_back("debug");
  }
#ifdef notdef
  args.push_back("-l");
  args.push_back("60");	// 60 second time-sorter.
#endif
  args.push_back("-p");
  args.push_back(sync_server_pipe);
  if (xml_path.length() > 0)
  {
    args.push_back("-x");
    args.push_back(xml_path);
  }
  std::copy(headerfiles.begin(), headerfiles.end(),
	    std::back_inserter(args));

  std::ostringstream oss;
  std::vector<char *> files;
  for (unsigned int i = 0; i < args.size(); ++i)
  {
    oss << args[i] << " ";
    files.push_back((char*)(args[i].c_str()));
  }
  files.push_back(0);
  std::cerr << "Exec: " << oss.str() << std::endl;
  ILOG(("Exec: ") << oss.str());

  syncPID = fork();
  if (syncPID == 0)
  {
    execvp(files[0], &(files[0]));
    std::cerr << "nimbus: failed to exec sync_server" << std::endl;
    _exit(1);
  }

  int nchecks = 0;
  bool ready = false;
  while (!ready && ++nchecks <= 10)
  {
    struct stat buf;
    ready = stat(sync_server_pipe, &buf) == 0 && S_ISSOCK(buf.st_mode);
    // If the socket exists, the extra sleep makes sure it's ready.
    sleep(1);
  }
  if (!ready)
  {
    std::cerr << "sync_server socket not created within 10 seconds!\n";
    exit(1);
  }
  else
  {
    std::cerr << "sync_server socket exists!" << std::endl;
  }
#ifdef notdef
  nidas::util::UnixSocketAddress usa(sync_server_pipe);
  sock = new nidas::util::Socket(usa);
  nidas::core::IOChannel * iochan = new nidas::core::Socket(sock);
  syncRecReader = new nidas::dynld::raf::SyncRecordReader(iochan);
#endif
}


nidas::dynld::raf::SyncRecordReader* 
StartSyncReader(const std::set<std::string>& headerfiles, bool postprocess)
{
  DLOG(("entering StartSyncReader() with ") << headerfiles.size()
       << " header files and postprocess=" << postprocess);

  // bool postprocess = (! headerfiles.empty());
  nidas::util::Socket * sock;

  if (postprocess)
  {
    // *** Post-processing ***
    ILOG(("StartSyncReader: header_file=%s", (*headerfiles.begin()).c_str()));

    // Create our own SyncServer instance to run here in this process,
    // and then link it to a SyncRecordReader.

    syncServer = new nidas::dynld::raf::SyncServer();
    syncServer->setSorterLengthSeconds(60);

    if (sync_xml_path.length() > 0)
    {
      syncServer->setXMLFileName(sync_xml_path);
    }

    std::list<std::string> dataFileNames;
    std::copy(headerfiles.begin(), headerfiles.end(),
	      std::back_inserter(dataFileNames));
    syncServer->setDataFileNames(dataFileNames);

    // The reader attaches itself as a client of SyncServer and does other
    // setup like requesting the sync header.
    syncRecReader = new nidas::dynld::raf::SyncRecordReader(syncServer);

  }
  else
  {
    // *** Real-time ***
    ILOG(("StartSyncReader: connecting to dsm_server on localhost:30001"));

    // establish socket to dsm_server.  If sync_server_pipe has been set,
    // then resort to the testing scenario where a separate sync_server
    // process is simulating dsm_server on the pipe file.
    if (strlen(sync_server_pipe) == 0)
    {
      sock = new nidas::util::Socket("localhost", 30001);
    }
    else
    {
      nidas::util::UnixSocketAddress usa(sync_server_pipe);
      sock = new nidas::util::Socket(usa);
    }
    nidas::core::IOChannel * iochan = new nidas::core::Socket(sock);
    syncRecReader = new nidas::dynld::raf::SyncRecordReader(iochan);

    // We still need to load the project configuration for all the settings
    // which cannot be pulled from the sync header.  The xml path comes
    // from the input stream if not already specified.
    if (sync_xml_path.length() == 0)
    {
      sync_xml_path = syncRecReader->getConfigName();
    }
    if (sync_xml_path.length() > 0)
    {
      ILOG(("Loading Project from config file: ") << sync_xml_path);
      std::string expName = nidas::util::Process::expandEnvVars(sync_xml_path);
      Project *project = Project::getInstance();
      project->parseXMLConfigFile(expName);
      project->setConfigName(sync_xml_path);
    }
    else
    {
      std::ostringstream msg;
      msg << "The XML config file is required, but it is not specified and "
	  << "is not set in input stream!  "
	  << "Set it on the command-line with -x.";
      throw std::runtime_error(msg.str());
    }
  }

  // the SyncRecordReader::getVariables() throws Exception if
  // something is wrong with the header.
  std::list<const nidas::dynld::raf::SyncRecordVariable*> vars;
  try {
      // Loop over all variables from sync_server/nidas.
      vars = syncRecReader->getVariables();
  }
  catch(const nidas::util::Exception& e) {
    PLOG(("Error reading sync record:") << e.what());
    fprintf(stderr, "Fatal error: syncRecReader->getVariables() failed.\n");
    fprintf(stderr,
	    "  Check XML file, space in project name?  "
	    "Check units for odd characters.\n");
    exit(1);
  }

  return syncRecReader;
}



void
LoadCalibration(nidas::core::Variable* var, time_t startTime,
		std::vector<double>& coefs)
{
  nidas::core::VariableConverter* converter =
    const_cast<nidas::core::VariableConverter*>(var->getConverter());

  if (! converter)
    return;

  nidas::core::Polynomial* poly =
    dynamic_cast<nidas::core::Polynomial*>(converter);
  nidas::core::Linear* linear = 
    dynamic_cast<nidas::core::Linear*>(converter);
  if (linear)
  {
    //linear->readCalFile(startTime);
    coefs.clear();
    coefs.push_back(linear->getIntercept());
    coefs.push_back(linear->getSlope());
  }
  else if (poly)
  {
    //poly->readCalFile(startTime);
    coefs.clear();
    const std::vector<float>& dcoefs = poly->getCoefficients();
    for (size_t i = 0; i < dcoefs.size(); ++i)
      coefs.push_back(dcoefs[i]);
  } 

}

std::list<const nidas::core::Variable*>
selectVariablesFromProject(nidas::core::Project* project)
{
  std::list<const nidas::core::Variable*> vlist;

  nidas::dynld::raf::SyncRecordSource::
    selectVariablesFromProject(project, vlist);
  return vlist;
}



std::string
getSerialNumber(const nidas::core::Variable* variable)
{
  std::string sn;
  const nidas::core::SampleTag* tag = variable->getSampleTag();
  const nidas::core::DSMSensor* sensor = 0;
  const nidas::core::Parameter* parm = 0;
  if (tag)
    sensor = tag->getDSMSensor();
  if (sensor)
    parm = sensor->getParameter("SerialNumber");
  if (parm)
    sn = parm->getStringValue(0);
  return sn;
}


void
SetCalibration(RAWTBL* rp)
{
  Project* project = Project::getInstance();
  Variable* variable = lookup_variable(selectVariablesFromProject(project), 
				       rp->name);
  if (variable)
  {
    Polynomial* pc = new Polynomial();
    std::vector<float> fcofs;
    std::copy(rp->cof.begin(), rp->cof.end(), back_inserter(fcofs));
    pc->setCoefficients(fcofs);
    variable->setConverter(pc);
    std::ostringstream msg;
    msg << "replaced cal coefficients on " << rp->name << ": ";
    for (unsigned int i = 0; i < rp->cof.size(); ++i)
    {
      msg << rp->cof[i] << ", ";
    }
    ILOG(("") << msg.str());
  }
  else
  {
    std::ostringstream msg;
    msg << "In SetCalibration(): No such variable '" << rp->name << "'";
    throw std::runtime_error(msg.str());
  }

}


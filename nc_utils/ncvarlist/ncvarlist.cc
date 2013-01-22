/*
-------------------------------------------------------------------------
OBJECT NAME:	ncvarlist.cc

FULL NAME:	netCDF Variable List

DESCRIPTION:	Output list of variables, names, units, and titles, from a netCDF file.

COPYRIGHT:	University Corporation for Atmospheric Research, 2011
-------------------------------------------------------------------------
*/

#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <vector>

#include <netcdfcpp.h>

using namespace std;

static bool csv = false;

void processArgs(char **argv);

/* -------------------------------------------------------------------- */
void
usage()
{
  cerr << "netCDF variable list.\n  Usage: ncvavrlist [-c] file.nc\n";
  cerr << "    -c: output comma separated values (CSV).\n";
  cerr << "    Read a netCDF file [file.nc] and output variable names, units, and titles.\n";
  exit(1);
}

int
main(int argc, char *argv[])
{
  if (argc < 2)
    usage();

  processArgs(argv);

  NcFile inFile(argv[argc-1]);

  if (!inFile.is_valid())
  {
    cerr << "ncvarlist: Failed to open input file, exiting.\n";
    return 1;
  }

  NcError * ncErr = new NcError(NcError::silent_nonfatal);

  NcAtt *att;
  cout << argv[argc-1] << endl;
  
  // Output some basic info about the file.
  att = inFile.get_att("project");
  if (att) cout << att->as_string(0);
  att = inFile.get_att("FlightNumber");
  if (att) cout << ", " << att->as_string(0);
  att = inFile.get_att("FlightDate");
  if (att) cout << ", " << att->as_string(0);
  cout << endl;

  size_t longestName = 0, longestUnits = 0;

  // Go through var list and determine longest name and units for pretty print.
  if (!csv)
  {
    for (int i = 0; i < inFile.num_vars(); ++i)
    {
      NcVar *var = inFile.get_var(i);

      if (i > 0 && var)
      {
        longestName = max(longestName, strlen(var->name()));
        att = var->get_att("units");
        if (att) longestUnits = max(longestUnits, strlen(att->as_string(0)));
      }
    }
  }


  // Output.
  for (int i = 0; i < inFile.num_vars(); ++i)
  {
    NcVar *var = inFile.get_var(i);
    NcAtt *units = var->get_att("units");
    NcAtt *long_name = var->get_att("long_name");

    if (var)
      if (csv)
      {
        cout << "\"" << var->name() << "\",\"";
        if (units) cout << units->as_string(0);
        cout << "\",\"";
        if (long_name) cout << long_name->as_string(0);
        cout << "\"" << endl;
      }
      else
      {
        cout << left << setw (longestName+3) << var->name();
        if (units) cout << left << setw (longestUnits+3) << units->as_string(0);
        if (long_name) cout << long_name->as_string(0);
        cout << endl;
      }
  }
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

        case 'c':
          csv = true;
          break;
        }
}

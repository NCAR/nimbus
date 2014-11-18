#include <cstdlib>
#include <cstring>

#include <netcdfcpp.h>

using namespace std;

// Command line options.
bool verbose = false;
float upper_alt = 8800.0;	// Default.
float lower_alt = 4500.0;	// Default.

NcValues *
getData(NcFile * file, const char * name)
{
  NcVar * var = file->get_var(name);

  if (var == 0 || var->is_valid() == false)
  {
    cerr << "No variable " << name << "?" << endl;
    return 0;
  }

  NcValues * data = var->values();

  if (data == 0)
  {
    cerr << "No data for variable " << name << "?" << endl;
    return 0;
  }

  return data;
}


void
usage()
{
  cerr << "Print out number of dips in a flight, take-off and landing will be a dip.\n";
  cerr << "Usage: nc_dips [-v] [-l lower_alt_limit] [-u upper_alt_limit] netcdf_file\n";
  exit(1);
}


void
processArgs(char **argv)
{
  while (*++argv)
    if ((*argv)[0] == '-')
      switch ((*argv)[1])
        {
        case 'v':
          verbose = true;
          break;

        case 'l':
          lower_alt = atof(*++argv);
          break;

        case 'u':
          upper_alt = atof(*++argv);
          break;

        case 'h':
          usage();
          break;

        }
}


int
main(int argc, char *argv[])
{
  int indx = 1;


  if (argc < 2)
    usage();

  processArgs(argv);

  if (strcmp(argv[indx], "-v") == 0)
  {
    verbose = true;
  }

  if (strcmp(argv[indx], "-l") == 0)
  {
    ++indx;
    lower_alt = atof(argv[indx++]);
  }

  if (strcmp(argv[indx], "-u") == 0)
  {
    ++indx;
    upper_alt = atof(argv[indx++]);
  }

  // keep program from exiting, if netCDF API doesn't find something.
  NcError * ncErr = new NcError(NcError::silent_nonfatal);


  // Open NetCDF file
  NcFile * ncFile = new NcFile(argv[indx], NcFile::ReadOnly);
  if (ncFile->is_valid() == false)
  {
    cerr << "Could not open NetCDF file '" << argv[indx] << "' for reading.\n";
    return 1;
  }
  
  if (verbose)
  {
    NcAtt * project = ncFile->get_att("project");
    NcAtt * flight = ncFile->get_att("FlightNumber");
    cout	<< argv[indx] << ":"
		<< project->as_string(0) << ":"
		<< flight->as_string(0) << ":\n";
  }

  // Try GPS alt first, otherwise pressure alt.
  NcValues * alt_data = getData(ncFile, "GGALT");
  if (alt_data == 0)
    alt_data = getData(ncFile, "PALT");

  if (alt_data == 0)
    return 1;

  // Get flight start time. = FileStartTime + first Time[Offset] value.
  NcVar * time_var = ncFile->get_var("Time");

  // Locate Start of Flight
  long i, ndips = 0;
  bool high_alt = false;
  for (i = 0; i < time_var->num_vals(); ++i)
  {
    float alt = alt_data->as_float(i);

    if (alt < 0)
      continue;

    if (high_alt == false && alt > upper_alt)
    {
      high_alt = true;
    }
    else
    if (high_alt == true && alt < lower_alt)
    {
      high_alt = false;
      ++ndips;
    }
  }


  if (verbose) cout << "Number of dips = ";
  cout << ndips << endl;

  delete alt_data;
  delete ncFile;
}

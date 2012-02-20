#include <cstdlib>
#include <ctime>
#include <cstring>

#include <netcdfcpp.h>

using namespace std;


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


time_t
getStartTime(NcVar * time_var)
{
  NcAtt * unit_att = time_var->get_att("units");
  NcAtt * frmt_att = time_var->get_att("strptime_format");

  if (unit_att == 0 || unit_att->is_valid() == false)
  {
    cerr << "Units not present for time variable.\n";
    return 0;
  }

  char format[1000];
  if (frmt_att == 0 || frmt_att->is_valid() == false)
    strcpy(format, "seconds since %F %T %z");
  else
    strcpy(format, frmt_att->as_string(0));

  struct tm StartFlight;
  strptime(unit_att->as_string(0), format, &StartFlight);

  time_t start_t = mktime(&StartFlight);
  if (start_t <= 0)
    cerr << "Problem decoding units from Time variable, _start = " << start_t << endl;

  return start_t;
}


int
main(int argc, char *argv[])
{
  int indx = 1;
  float tas_cutoff = 25.0;	// Default.


  // Check arguments / usage.
  if (argc < 2)
  {
    cerr << "Print out take-off and landing times from a netCDF file.\nUses true airspeed above 25 m/s (use -t to change).\n\n";
    cerr << "Usage: flt_time [-t value] netcdf_file\n";
    exit(1);
  }

  if (strcmp(argv[indx], "-t") == 0)
  {
    ++indx;
    tas_cutoff = atof(argv[indx++]);
  }

  // keep program from exiting, if netCDF API doesn't find something.
  NcError * ncErr = new NcError(NcError::silent_nonfatal);

  putenv((char *)"TZ=UTC");     // Force all time routines to work in UTC.

  // Open NetCDF file
  NcFile * ncFile = new NcFile(argv[indx], NcFile::ReadOnly);
  if (ncFile->is_valid() == false)
  {
    cerr << "Could not open NetCDF file '" << argv[indx] << "' for reading.\n";
    return 1;
  }
  
  NcAtt * flight = ncFile->get_att("FlightNumber");
  cout << argv[indx] << ":" << flight->as_string(0) << ":\n";

  NcValues * time_data = getData(ncFile, "Time");
  NcValues * tas_data = getData(ncFile, "TASX");

  if (time_data == 0 || tas_data == 0)
    return 1;

  // Get flight start time.
  NcVar * time_var = ncFile->get_var("Time");
  time_t start_t = getStartTime(time_var);

  if (start_t <= 0)
  {
    cerr << "Invalid start time, fatal.\n";
    return 1;
  }


  // Locate Start of Flight
  long i;
  for (i = 0; i < time_var->num_vals(); ++i)
    if (tas_data->as_float(i) > tas_cutoff)
    {
      time_t x = start_t + i;
      cout << "   " << ctime(&x);
//cout << "take-off indx = " << x << " " << i << endl;
      break;
    }

  // Locate End of Flight
  for (; i < time_var->num_vals(); ++i)
    if (tas_data->as_float(i) < tas_cutoff && tas_data->as_float(i) != -32767.0)
    {
      time_t x = start_t + i;

      cout << "   " << ctime(&x);
//cout << "landing indx = " << x << " " << i << endl;
      break;
    }

  delete time_data;
  delete tas_data;
  delete ncFile;
}

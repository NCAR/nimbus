#include "config.h"
#include "netcdf.h"

#include <cstring>
#include <cstdlib>

netCDF::netCDF(Config & cfg) : _file(0), _mode(NcFile::Write)
{
  // No file to pre-open or file does not exist.  Bail out.
  if (cfg.outputFile.size() == 0 || access(cfg.outputFile.c_str(), F_OK))
    return;

  // Check for write permission.
  if (access(cfg.outputFile.c_str(), W_OK))
  {
    std::cerr << "Write permission failure on output file "
	<< cfg.outputFile << std::endl;
    exit(1);
  }

  // Attempt to open for writing.
  _file = new NcFile(cfg.outputFile.c_str(), _mode);

  // Failed to open netCDF file for writing....
  if (!_file->is_valid())
  {
    std::cerr << "Failed to open netCDF output file "
	<< cfg.outputFile << std::endl;
    exit(1);
  }

  readStartEndTime(cfg);
}

void netCDF::CreateNetCDFfile(Config & cfg)
{
  if (_file && _file->is_valid())	// Was successfully opened in ctor, leave.
    return;

  /* If user did not specify output file, then create base + .nc of input file.
   * And destroy an existing file if it exists.
   */
  if (cfg.outputFile.size() == 0) {
    std::string ncfilename;
    size_t pos = cfg.inputFile.find_last_of("/\\");
    ncfilename = cfg.inputFile.substr(pos+1);
    pos = ncfilename.find_last_of(".");
    cfg.outputFile = ncfilename.substr(0,pos) + ".nc";
    _mode = NcFile::Replace;
  }
  else {
    // User specified an output filname on command line, see if file exists.
    NcFile test(cfg.outputFile.c_str(), NcFile::Write);
    if (!test.is_valid())
      _mode = NcFile::New;
  }

  _file = new NcFile(cfg.outputFile.c_str(), _mode);

  /* If we are creating a file from scratch, perform the following.
   */
  if (_mode != NcFile::Write)
  {
    struct tm st, et;
    gmtime_r(&cfg.starttime, &st);
    gmtime_r(&cfg.stoptime, &et);

    _file->add_att("institution", "NCAR Research Aviation Facility");
    _file->add_att("Source", "NCAR/RAF Fast-2DC Processing Software");
    _file->add_att("Conventions", "NCAR-RAF/nimbus");
    _file->add_att("ConventionsURL", "http://www.eol.ucar.edu/raf/Software/netCDF.html");
    _file->add_att("ProjectName", cfg.project.c_str());
    _file->add_att("FlightNumber", cfg.flightNumber.c_str());
    _file->add_att("FlightDate", cfg.flightDate.c_str());
    char tmp[128];
    sprintf(tmp, "%02d:%02d:%02d-%02d:%02d:%02d",
	st.tm_hour, st.tm_min, st.tm_sec, et.tm_hour, et.tm_min, et.tm_sec);
    _file->add_att("TimeInterval", tmp);
    _file->add_att("Reconstruction", cfg.recon);
    _file->add_att("SizeMethod", cfg.smethod);
    _file->add_att("Raw_2D_Data_File", cfg.inputFile.c_str());
  }
}

void netCDF::readStartEndTime(Config & cfg)
{
  NcVar *var;

  if ((var = _file->get_var("Time")) == 0)
    var = _file->get_var("time");

  /* If we found a Time variable, acquire the start/end time from the netCDF
   * file.  This is the time frame we want to process against.
   */
  if (var)
  {
    if (var->num_vals() == 0)
      return;

    NcValues *v = var->values();
    NcAtt *units, *frmt_att;
    char *frmt;

    units = var->get_att("units");
    frmt_att = var->get_att("strptime_format");

    if (units == 0)
    {
      std::cerr << "No units for Time variable, fatal." << std::endl;
      exit(1);
    }

    if (frmt_att == 0)
    {
      frmt = (char *)"seconds since %F %T %z";
    }
    else
      frmt = frmt_att->as_string(0);

    struct tm tm;
    time_t st, et;
    strptime(units->as_string(0), frmt, &tm);
    st = mktime(&tm) + v->as_int(0);
    et = st + v->as_int(var->num_vals()-1);
    std::cout << "NetCDF start time : " << ctime(&st);
    std::cout << "         end time : " << ctime(&et);

    cfg.starttime = st;
    cfg.stoptime = et;

    delete v;
  }
}

NcDim *netCDF::addDimension(const char name[], int size)
{
  NcDim *dim = _file->get_dim(name);

  if (dim) {
    std::cout << "Found dimension " << name << std::endl;
  }
  else {
    dim = _file->add_dim(name, size);
    if (dim)
      std::cout << "Created dimension " << name << std::endl;
    else
      std::cout << "Failed to create dimension " << name << std::endl;
  }

  if (strcmp(name, "Time") == 0) // Capture time dimension.
    _timedim = dim;

  return dim;
}

NcVar *netCDF::addTimeVariable(Config & cfg, int size)
{
  _timevar = _file->get_var("Time");

  if (_timevar)
    return _timevar;

  char timeunits[70];
  int year, month, day;
  struct tm st;
  gmtime_r(&cfg.starttime, &st);

  sscanf(cfg.flightDate.c_str(), "%d/%d/%d", &month, &day, &year);
  sprintf(timeunits,"seconds since %04d-%02d-%02d %02d:%02d:%02d +0000", year,month,day,st.tm_hour,st.tm_min,st.tm_sec);
  _timevar = _file->add_var("Time", ncInt, _timedim);

  if (_timevar == 0)
  {
    std::cout << "Failed to create Time variable.\n";
    return _timevar;
  }
  _timevar->add_att("long_name", "time of measurement");
  _timevar->add_att("standard_name", "time");
  _timevar->add_att("units", timeunits);
  _timevar->add_att("strptime_format", "seconds since %F %T %z");

  int time[size];
  for (int i = 0; i < size; i++) time[i] = i;
  if (!_timevar->put(time, size))
    std::cout << "Failed to write Time data to file.\n";

  return _timevar;
}

#include "config.h"
#include "netcdf.h"

#include <cstring>

netCDF::netCDF(Config & cfg)
{
  NcFile::FileMode mode = NcFile::Write;
  new NcError(NcError::verbose_nonfatal);

  // If user did not specify output file, then create base + .nc of input file.
  if (cfg.outputFile.size() == 0) {
    std::string ncfilename;
    size_t pos = cfg.inputFile.find_last_of("/\\");
    ncfilename = cfg.inputFile.substr(pos+1);
    pos = ncfilename.find_last_of(".");
    cfg.outputFile = ncfilename.substr(0,pos) + ".nc";
    mode = NcFile::Replace;
  }
  else {
    // See if file exists.
    NcFile test(cfg.outputFile.c_str(), NcFile::Write);
    if (!test.is_valid())
      mode = NcFile::New;

  }

  _file = new NcFile(cfg.outputFile.c_str(), mode);

  if (mode != NcFile::Write)
  {
    int h1=cfg.starttime/10000;
    int m1=(cfg.starttime-h1*10000l)/100;
    int s1=(long(cfg.starttime)-h1*10000l-m1*100l);
    int h2=cfg.stoptime/10000;
    int m2=(cfg.stoptime-h2*10000l)/100;
    int s2=(long(cfg.stoptime)-h2*10000l-m2*100l);

    _file->add_att("institution", "NCAR Research Aviation Facility");
    _file->add_att("Source", "NCAR/RAF Fast-2DC Processing Software");
    _file->add_att("Conventions", "NCAR-RAF/nimbus");
    _file->add_att("ConventionsURL", "http://www.eol.ucar.edu/raf/Software/netCDF.html");
    _file->add_att("ProjectName", cfg.project.c_str());
    _file->add_att("FlightNumber", cfg.flightNumber.c_str());
    _file->add_att("FlightDate", cfg.flightDate.c_str());
    char tmp[128];
    sprintf(tmp, "%02d:%02d:%02d-%02d:%02d:%02d", h1,m1,s1,h2,m2,s2);
    _file->add_att("TimeInterval", tmp);
    _file->add_att("Reconstruction", cfg.recon);
    _file->add_att("SizeMethod", cfg.smethod);
    _file->add_att("Raw_2D_Data_File", cfg.inputFile.c_str());
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
  int h1=cfg.starttime/10000;
  int m1=(cfg.starttime-h1*10000l)/100;
  int s1=(long(cfg.starttime)-h1*10000l-m1*100l);

  sscanf(cfg.flightDate.c_str(), "%d/%d/%d", &month, &day, &year);
  sprintf(timeunits,"seconds since %04d-%02d-%02d %02d:%02d:%02d +0000", year,month,day,h1,m1,s1);
  _timevar = _file->add_var("Time", ncInt, _timedim);

  if (_timevar == 0)
  {
    std::cout << "Failed to create Time variable.\n";
    return _timevar;
  }
  _timevar->add_att("long_name", "time of measurement");
  _timevar->add_att("standard_name", "time");
  _timevar->add_att("units", timeunits);

  int time[size];
  for (int i = 0; i < size; i++) time[i] = i;
  if (!_timevar->put(time, size))
    std::cout << "Failed to write Time data to file.\n";

  return _timevar;
}

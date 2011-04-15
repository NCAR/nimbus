#include "config.h"
#include "netcdf.h"
#include "probe.h"
#include "ProbeData.h"

#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>

using namespace std;

netCDF::netCDF(Config & cfg) : _file(0), _mode(NcFile::Write), _tas(0)
{
  // No file to pre-open or file does not exist.  Bail out.
  if (cfg.outputFile.size() == 0 || access(cfg.outputFile.c_str(), F_OK))
    return;

  // Check for write permission.
  if (access(cfg.outputFile.c_str(), W_OK))
  {
    cerr << "Write permission failure on output file "
	<< cfg.outputFile << endl;
    exit(1);
  }

  // Attempt to open for writing.
  _file = new NcFile(cfg.outputFile.c_str(), _mode);

  // Failed to open netCDF file for writing....
  if (!_file->is_valid())
  {
    cerr << "Failed to open netCDF output file "
	<< cfg.outputFile << endl;
    exit(1);
  }

  readStartEndTime(cfg);

  // Check for existence of TASX variable.
  _tas = _file->get_var("TASX");

  if (_tas)
    cout << "TASX variable found, will use instead of tas in 2D records.\n";
}

netCDF::~netCDF()
{
  _file->close();
}

void netCDF::CreateNetCDFfile(Config & cfg)
{
  if (_file && _file->is_valid())	// Was successfully opened in ctor, leave.
    return;

  /* If user did not specify output file, then create base + .nc of input file.
   * And destroy an existing file if it exists.
   */
  if (cfg.outputFile.size() == 0) {
    string ncfilename;
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

void netCDF::readTrueAirspeed(float tas[], size_t n)
{
  assert ((int)n == _tas->num_vals());

  NcValues *data = _tas->values();
  for (size_t i = 0; i < n; ++i)
    tas[i] = data->as_float(i);

  delete data;
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
      cerr << "No units for Time variable, fatal." << endl;
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
    cout << "NetCDF start time : " << ctime(&st);
    cout << "         end time : " << ctime(&et);

    cfg.starttime = st;
    cfg.stoptime = et;

    delete v;
  }
}

NcDim *netCDF::addDimension(const char name[], int size)
{
  NcDim *dim = _file->get_dim(name);

  if (dim) {
    cout << "Found dimension " << name << endl;
  }
  else {
    dim = _file->add_dim(name, size);
    if (dim)
      cout << "Created dimension " << name << endl;
    else
      cout << "Failed to create dimension " << name << endl;
  }

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
    cout << "Failed to create Time variable.\n";
    return _timevar;
  }
  _timevar->add_att("long_name", "time of measurement");
  _timevar->add_att("standard_name", "time");
  _timevar->add_att("units", timeunits);
  _timevar->add_att("strptime_format", "seconds since %F %T %z");

  int time[size];
  for (int i = 0; i < size; i++) time[i] = i;
  if (!_timevar->put(time, size))
    cout << "Failed to write Time data to file.\n";

  return _timevar;
}

void netCDF::CreateDimensions(int numtimes, ProbeInfo &probe, Config &cfg)
{
  char tmp[1024];

  // Define the dimensions.
  _timedim = addDimension("Time", numtimes);
  _spsdim = addDimension("sps1", 1);
  sprintf(tmp, "Vector%d", probe.numBins);
  _bindim = addDimension(tmp, probe.numBins);
  sprintf(tmp, "Vector%d", probe.numBins+1);
  _bindim_plusone = addDimension(tmp, probe.numBins+1);
  _intbindim = addDimension("interarrival_endpoints", cfg.nInterarrivalBins+1);

}


int netCDF::WriteData(ProbeInfo & probe, ProbeData & data)
{
  NcVar *var;
  string varname;

  //Total counts and LWC
  varname="CONC2DCA"+probe.suffix;
  if ((var = _file->get_var(varname.c_str())) == 0) {
    if (!(var = _file->add_var(varname.c_str(), ncFloat, _timedim))) return netCDF::NC_ERR;
    if (!var->add_att("_FillValue", (float)(-32767.0))) return netCDF::NC_ERR;
    if (!var->add_att("units", "#/L")) return netCDF::NC_ERR;
    if (!var->add_att("long_name", "Total Fast 2DC Concentration, All Particles")) return netCDF::NC_ERR;
    if (!var->add_att("Category", "PMS Probe")) return netCDF::NC_ERR;
    if (!var->add_att("SerialNumber", probe.serialNumber.c_str())) return netCDF::NC_ERR;
    if (!var->add_att("DataQuality", "Good")) return netCDF::NC_ERR;
  }
  if (!var->put(&data.all.total_conc[0], data.size())) return netCDF::NC_ERR;

  varname="CONC2DCR"+probe.suffix;
  if ((var = _file->get_var(varname.c_str())) == 0) {
    if (!(var = _file->add_var(varname.c_str(), ncFloat, _timedim))) return netCDF::NC_ERR;
    if (!var->add_att("_FillValue", (float)(-32767.0))) return netCDF::NC_ERR;
    if (!var->add_att("units", "#/L")) return netCDF::NC_ERR;
    if (!var->add_att("long_name", "Total Fast 2DC Concentration, Round Particles")) return netCDF::NC_ERR;
    if (!var->add_att("Category", "PMS Probe")) return netCDF::NC_ERR;
    if (!var->add_att("SerialNumber", probe.serialNumber.c_str())) return netCDF::NC_ERR;
    if (!var->add_att("DataQuality", "Good")) return netCDF::NC_ERR;
  }
  if (!var->put(&data.round.total_conc[0], data.size())) return netCDF::NC_ERR;

  varname="PLWC2DCR"+probe.suffix;
  if ((var = _file->get_var(varname.c_str())) == 0) {
    if (!(var = _file->add_var(varname.c_str(), ncFloat, _timedim))) return netCDF::NC_ERR;
    if (!var->add_att("_FillValue", (float)(-32767.0))) return netCDF::NC_ERR;
    if (!var->add_att("units", "g/m3")) return netCDF::NC_ERR;
    if (!var->add_att("long_name", "Fast 2DC Liquid Water Content, Round Particles")) return netCDF::NC_ERR;
    if (!var->add_att("Category", "PMS Probe")) return netCDF::NC_ERR;
    if (!var->add_att("SerialNumber", probe.serialNumber.c_str())) return netCDF::NC_ERR;
    if (!var->add_att("DataQuality", "Good")) return netCDF::NC_ERR;
  }
  if (!var->put(&data.round.lwc[0], data.size())) return netCDF::NC_ERR;

  varname="PLWC2DCA"+probe.suffix;
  if ((var = _file->get_var(varname.c_str())) == 0) {
    if (!(var = _file->add_var(varname.c_str(), ncFloat, _timedim))) return netCDF::NC_ERR;
    if (!var->add_att("_FillValue", (float)(-32767.0))) return netCDF::NC_ERR;
    if (!var->add_att("units", "g/m3")) return netCDF::NC_ERR;
    if (!var->add_att("long_name", "Fast 2DC Liquid Water Content, All Particles")) return netCDF::NC_ERR;
    if (!var->add_att("Category", "PMS Probe")) return netCDF::NC_ERR;
    if (!var->add_att("SerialNumber", probe.serialNumber.c_str())) return netCDF::NC_ERR;
    if (!var->add_att("DataQuality", "Good")) return netCDF::NC_ERR;
  }
  if (!var->put(&data.all.lwc[0], data.size())) return netCDF::NC_ERR;

  varname="DBAR2DCR"+probe.suffix;
  if ((var = _file->get_var(varname.c_str())) == 0) {
    if (!(var = _file->add_var(varname.c_str(), ncFloat, _timedim))) return netCDF::NC_ERR;
    if (!var->add_att("_FillValue", (float)(-32767.0))) return netCDF::NC_ERR;
    if (!var->add_att("units", "um")) return netCDF::NC_ERR;
    if (!var->add_att("long_name", "Fast 2DC Mean Particle Diameter, Round Particles")) return netCDF::NC_ERR;
    if (!var->add_att("Category", "PMS Probe")) return netCDF::NC_ERR;
    if (!var->add_att("SerialNumber", probe.serialNumber.c_str())) return netCDF::NC_ERR;
    if (!var->add_att("DataQuality", "Good")) return netCDF::NC_ERR;
  }
  if (!var->put(&data.round.dbar[0], data.size())) return netCDF::NC_ERR;

  varname="DBAR2DCA"+probe.suffix;
  if ((var = _file->get_var(varname.c_str())) == 0) {
    if (!(var = _file->add_var(varname.c_str(), ncFloat, _timedim))) return netCDF::NC_ERR;
    if (!var->add_att("_FillValue", (float)(-32767.0))) return netCDF::NC_ERR;
    if (!var->add_att("units", "um")) return netCDF::NC_ERR;
    if (!var->add_att("long_name", "Fast 2DC Mean Particle Diameter, All Particles")) return netCDF::NC_ERR;
    if (!var->add_att("Category", "PMS Probe")) return netCDF::NC_ERR;
    if (!var->add_att("SerialNumber", probe.serialNumber.c_str())) return netCDF::NC_ERR;
    if (!var->add_att("DataQuality", "Good")) return netCDF::NC_ERR;
  }
  if (!var->put(&data.all.dbar[0], data.size())) return netCDF::NC_ERR;

  varname="DISP2DCR"+probe.suffix;
  if ((var = _file->get_var(varname.c_str())) == 0) {
    if (!(var = _file->add_var(varname.c_str(), ncFloat, _timedim))) return netCDF::NC_ERR;
    if (!var->add_att("_FillValue", (float)(-32767.0))) return netCDF::NC_ERR;
    if (!var->add_att("units", "none")) return netCDF::NC_ERR;
    if (!var->add_att("long_name", "Fast 2DC Dispersion, Round Particles")) return netCDF::NC_ERR;
    if (!var->add_att("Category", "PMS Probe")) return netCDF::NC_ERR;
    if (!var->add_att("SerialNumber", probe.serialNumber.c_str())) return netCDF::NC_ERR;
    if (!var->add_att("DataQuality", "Good")) return netCDF::NC_ERR;
  }
  if (!var->put(&data.round.disp[0], data.size())) return netCDF::NC_ERR;

  varname="DISP2DCA"+probe.suffix;
  if ((var = _file->get_var(varname.c_str())) == 0) {
    if (!(var = _file->add_var(varname.c_str(), ncFloat, _timedim))) return netCDF::NC_ERR;
    if (!var->add_att("_FillValue", (float)(-32767.0))) return netCDF::NC_ERR;
    if (!var->add_att("units", "none")) return netCDF::NC_ERR;
    if (!var->add_att("long_name", "Fast 2DC Dispersion, All Particles")) return netCDF::NC_ERR;
    if (!var->add_att("Category", "PMS Probe")) return netCDF::NC_ERR;
    if (!var->add_att("SerialNumber", probe.serialNumber.c_str())) return netCDF::NC_ERR;
    if (!var->add_att("DataQuality", "Good")) return netCDF::NC_ERR;
  }
  if (!var->put(&data.all.disp[0], data.size())) return netCDF::NC_ERR;

  varname="DBZ2DCR"+probe.suffix;
  if ((var = _file->get_var(varname.c_str())) == 0) {
    if (!(var = _file->add_var(varname.c_str(), ncFloat, _timedim))) return netCDF::NC_ERR;
    if (!var->add_att("_FillValue", (float)(-32767.0))) return netCDF::NC_ERR;
    if (!var->add_att("units", "dBZ")) return netCDF::NC_ERR;
    if (!var->add_att("long_name", "Fast 2DC Calculated Reflectivity, Round Particles")) return netCDF::NC_ERR;
    if (!var->add_att("Category", "PMS Probe")) return netCDF::NC_ERR;
    if (!var->add_att("SerialNumber", probe.serialNumber.c_str())) return netCDF::NC_ERR;
    if (!var->add_att("DataQuality", "Good")) return netCDF::NC_ERR;
  }
  if (!var->put(&data.round.dbz[0], data.size())) return netCDF::NC_ERR;

  varname="DBZ2DCA"+probe.suffix;
  if ((var = _file->get_var(varname.c_str())) == 0) {
    if (!(var = _file->add_var(varname.c_str(), ncFloat, _timedim))) return netCDF::NC_ERR;
    if (!var->add_att("_FillValue", (float)(-32767.0))) return netCDF::NC_ERR;
    if (!var->add_att("units", "dBZ")) return netCDF::NC_ERR;
    if (!var->add_att("long_name", "Fast 2DC Calculated Reflectivity, All Particles")) return netCDF::NC_ERR;
    if (!var->add_att("Category", "PMS Probe")) return netCDF::NC_ERR;
    if (!var->add_att("SerialNumber", probe.serialNumber.c_str())) return netCDF::NC_ERR;
    if (!var->add_att("DataQuality", "Good")) return netCDF::NC_ERR;
  }
  if (!var->put(&data.all.dbz[0], data.size())) return netCDF::NC_ERR;

  varname="REFF2DCR"+probe.suffix;
  if ((var = _file->get_var(varname.c_str())) == 0) {
    if (!(var = _file->add_var(varname.c_str(), ncFloat, _timedim))) return netCDF::NC_ERR;
    if (!var->add_att("_FillValue", (float)(-32767.0))) return netCDF::NC_ERR;
    if (!var->add_att("units", "um")) return netCDF::NC_ERR;
    if (!var->add_att("long_name", "Fast 2DC Effective Radius, Round Particles")) return netCDF::NC_ERR;
    if (!var->add_att("Category", "PMS Probe")) return netCDF::NC_ERR;
    if (!var->add_att("SerialNumber", probe.serialNumber.c_str())) return netCDF::NC_ERR;
    if (!var->add_att("DataQuality", "Good")) return netCDF::NC_ERR;
  }
  if (!var->put(&data.round.eff_rad[0], data.size())) return netCDF::NC_ERR;

  varname="REFF2DCA"+probe.suffix;
  if ((var = _file->get_var(varname.c_str())) == 0) {
    if (!(var = _file->add_var(varname.c_str(), ncFloat, _timedim))) return netCDF::NC_ERR;
    if (!var->add_att("_FillValue", (float)(-32767.0))) return netCDF::NC_ERR;
    if (!var->add_att("units", "um")) return netCDF::NC_ERR;
    if (!var->add_att("long_name", "Fast 2DC Effective Radius, All Particles")) return netCDF::NC_ERR;
    if (!var->add_att("Category", "PMS Probe")) return netCDF::NC_ERR;
    if (!var->add_att("SerialNumber", probe.serialNumber.c_str())) return netCDF::NC_ERR;
    if (!var->add_att("DataQuality", "Good")) return netCDF::NC_ERR;
  }
  if (!var->put(&data.all.eff_rad[0], data.size())) return netCDF::NC_ERR;

  varname="NACCEPT2DCR"+probe.suffix;
  if ((var = _file->get_var(varname.c_str())) == 0) {
    if (!(var = _file->add_var(varname.c_str(), ncFloat, _timedim))) return netCDF::NC_ERR;
    if (!var->add_att("units", "count")) return netCDF::NC_ERR;
    if (!var->add_att("long_name", "Number of Particles Accepted, Round Particles")) return netCDF::NC_ERR;
    if (!var->add_att("Category", "PMS Probe")) return netCDF::NC_ERR;
    if (!var->add_att("SerialNumber", probe.serialNumber.c_str())) return netCDF::NC_ERR;
    if (!var->add_att("DataQuality", "Good")) return netCDF::NC_ERR;
  }
  if (!var->put(&data.round.accepted[0], data.size())) return netCDF::NC_ERR;

  varname="NACCEPT2DCA"+probe.suffix;
  if ((var = _file->get_var(varname.c_str())) == 0) {
    if (!(var = _file->add_var(varname.c_str(), ncFloat, _timedim))) return netCDF::NC_ERR;
    if (!var->add_att("units", "count")) return netCDF::NC_ERR;
    if (!var->add_att("long_name", "Number of Particles Accepted, All Particles")) return netCDF::NC_ERR;
    if (!var->add_att("Category", "PMS Probe")) return netCDF::NC_ERR;
    if (!var->add_att("SerialNumber", probe.serialNumber.c_str())) return netCDF::NC_ERR;
    if (!var->add_att("DataQuality", "Good")) return netCDF::NC_ERR;
  }
  if (!var->put(&data.all.accepted[0], data.size())) return netCDF::NC_ERR;

  varname="NREJECT2DCR"+probe.suffix;
  if ((var = _file->get_var(varname.c_str())) == 0) {
    if (!(var = _file->add_var(varname.c_str(), ncFloat, _timedim))) return netCDF::NC_ERR;
    if (!var->add_att("units", "count")) return netCDF::NC_ERR;
    if (!var->add_att("long_name", "Number of Particles Rejected, Round Particles")) return netCDF::NC_ERR;
    if (!var->add_att("Category", "PMS Probe")) return netCDF::NC_ERR;
    if (!var->add_att("SerialNumber", probe.serialNumber.c_str())) return netCDF::NC_ERR;
    if (!var->add_att("DataQuality", "Good")) return netCDF::NC_ERR;
  }
  if (!var->put(&data.round.rejected[0], data.size())) return netCDF::NC_ERR;

  varname="NREJECT2DCA"+probe.suffix;
  if ((var = _file->get_var(varname.c_str())) == 0) {
    if (!(var = _file->add_var(varname.c_str(), ncFloat, _timedim))) return netCDF::NC_ERR;
    if (!var->add_att("units", "count")) return netCDF::NC_ERR;
    if (!var->add_att("long_name", "Number of Particles Rejected, All Particles")) return netCDF::NC_ERR;
    if (!var->add_att("Category", "PMS Probe")) return netCDF::NC_ERR;
    if (!var->add_att("SerialNumber", probe.serialNumber.c_str())) return netCDF::NC_ERR;
    if (!var->add_att("DataQuality", "Good")) return netCDF::NC_ERR;
  }
  if (!var->put(&data.all.rejected[0], data.size())) return netCDF::NC_ERR;
  
  
  
  //Misc
  if (_tas == 0)
  {
    varname="poisson_coeff1"+probe.suffix;
    if ((var = _file->get_var(varname.c_str())) == 0) {
      if (!(var = _file->add_var(varname.c_str(), ncFloat, _timedim))) return netCDF::NC_ERR;
      if (!var->add_att("units", "unitless")) return netCDF::NC_ERR;
      if (!var->add_att("long_name", "Interarrival Time Fit Coefficient 1")) return netCDF::NC_ERR;
    }
    if (!var->put(&data.cpoisson1[0], data.size())) return netCDF::NC_ERR;

    varname="poisson_coeff2"+probe.suffix;
    if ((var = _file->get_var(varname.c_str())) == 0) {
      if (!(var = _file->add_var(varname.c_str(), ncFloat, _timedim))) return netCDF::NC_ERR;
      if (!var->add_att("units", "1/seconds")) return netCDF::NC_ERR;
      if (!var->add_att("long_name", "Interarrival Time Fit Coefficient 2")) return netCDF::NC_ERR;
    }
    if (!var->put(&data.cpoisson2[0], data.size())) return netCDF::NC_ERR;

    varname="poisson_coeff3"+probe.suffix;
    if ((var = _file->get_var(varname.c_str())) == 0) {
      if (!(var = _file->add_var(varname.c_str(), ncFloat, _timedim))) return netCDF::NC_ERR;
      if (!var->add_att("units", "1/seconds")) return netCDF::NC_ERR;
      if (!var->add_att("long_name", "Interarrival Time Fit Coefficient 3")) return netCDF::NC_ERR;
    }
    if (!var->put(&data.cpoisson3[0], data.size())) return netCDF::NC_ERR;

    varname="poisson_cutoff"+probe.suffix;
    if ((var = _file->get_var(varname.c_str())) == 0) {
      if (!(var = _file->add_var(varname.c_str(), ncFloat, _timedim))) return netCDF::NC_ERR;
      if (!var->add_att("units", "seconds")) return netCDF::NC_ERR;
      if (!var->add_att("long_name", "Interarrival Time Lower Limit")) return netCDF::NC_ERR;
    }
    if (!var->put(&data.pcutoff[0], data.size())) return netCDF::NC_ERR;

    varname="poisson_correction"+probe.suffix;
    if ((var = _file->get_var(varname.c_str())) == 0) {
      if (!(var = _file->add_var(varname.c_str(), ncFloat, _timedim))) return netCDF::NC_ERR;
      if (!var->add_att("units", "unitless")) return netCDF::NC_ERR;
      if (!var->add_att("long_name", "Count/Concentration Correction Factor for Interarrival Rejection")) return netCDF::NC_ERR;
    }
    if (!var->put(&data.corrfac[0], data.size())) return netCDF::NC_ERR;

    varname="TAS"+probe.suffix;
    if ((var = _file->get_var(varname.c_str())) == 0) {
      if (!(var = _file->add_var(varname.c_str(), ncFloat, _timedim))) return netCDF::NC_ERR;
      if (!var->add_att("units", "m/s")) return netCDF::NC_ERR;
      if (!var->add_att("long_name", "True Air Speed")) return netCDF::NC_ERR;
    }
    if (!var->put(&data.tas[0], data.size())) return netCDF::NC_ERR;
  
    varname="SA"+probe.suffix;
    if ((var = _file->get_var(varname.c_str())) == 0) {
      if (!(var = _file->add_var(varname.c_str(), ncFloat, _bindim))) return netCDF::NC_ERR;
      if (!var->add_att("units", "m2")) return netCDF::NC_ERR;
      if (!var->add_att("long_name", "Sample area per channel")) return netCDF::NC_ERR;
    }
    if (!var->put(&probe.samplearea[0], probe.numBins)) return netCDF::NC_ERR;
  
    //Bins  
    varname="bin_endpoints"+probe.suffix;
    if ((var = _file->get_var(varname.c_str())) == 0) {
      if (!(var = _file->add_var(varname.c_str(), ncFloat, _bindim_plusone))) return netCDF::NC_ERR;
      if (!var->add_att("units", "microns")) return netCDF::NC_ERR;
    }
    if (!var->put(&probe.bin_endpoints[0], probe.numBins+1)) return netCDF::NC_ERR; 

    varname="bin_midpoints"+probe.suffix;
    if ((var = _file->get_var(varname.c_str())) == 0) {
      if (!(var = _file->add_var(varname.c_str(), ncFloat, _bindim))) return netCDF::NC_ERR;
      if (!var->add_att("units", "microns")) return netCDF::NC_ERR;
      if (!var->add_att("long_name", "Size Channel Midpoints")) return netCDF::NC_ERR;
    }
    if (!var->put(&probe.bin_midpoints[0], probe.numBins)) return netCDF::NC_ERR;
  }

  return 0;
}

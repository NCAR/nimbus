#include <netcdfcpp.h>

class Config;
class ProbeInfo;
class ProbeData;

/**
 * Class to handle opening/creating the netCDF file and writing
 * variable data.
 */
class netCDF
{
public:
  netCDF(Config & cfg);
  ~netCDF();

  NcFile *ncid() const { return _file; }

  void CreateNetCDFfile(Config & cfg);

  void CreateDimensions(int numtimes, ProbeInfo &probe, Config &cfg);

  /**
   * Add dimension to netCDF file.  If dimension already exists,
   * then a pointer to that dimension is returned.
   */
  NcDim *addDimension(const char name[], int size);

  NcVar *addTimeVariable(Config & cfg, int size);

  bool hasTASX()
  { return _tas ? true : false; }

  /**
   * Check for the existence of TASX.  Read it into provided space.
   */
  void readTrueAirspeed(float tas[], size_t n);

  int WriteData(ProbeInfo & probe, ProbeData & data);


  NcDim *timedim() const { return _timedim; }
  NcDim *spsdim() const { return _spsdim; }
  NcDim *bindim() const { return _bindim; }
  NcDim *bindim_plusone() const { return _bindim_plusone; }
  NcDim *intbindim() const { return _intbindim; }


  static const int NC_ERR = 2;

private:
  void readStartEndTime(Config & cfg);

  NcFile *_file;
  NcFile::FileMode _mode;

  NcDim *_timedim, *_spsdim, *_bindim, *_bindim_plusone, *_intbindim;
  NcVar *_timevar;
  NcVar *_tas;
};

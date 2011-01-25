#include <netcdfcpp.h>

class Config;

/**
 * Class to handle opening/creating the netCDF file and writing
 * variable data.
 */
class netCDF
{
public:
  netCDF(Config & cfg);

  NcFile *ncid() const { return _file; }

  void CreateNetCDFfile(Config & cfg);

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


  static const int NC_ERR = 2;

private:
  void readStartEndTime(Config & cfg);

  NcFile *_file;
  NcFile::FileMode _mode;

  NcDim *_timedim;
  NcVar *_timevar;
  NcVar *_tas;
};

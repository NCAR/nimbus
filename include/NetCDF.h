/*
-------------------------------------------------------------------------
OBJECT NAME:	NetCDF.h

FULL NAME:	NetCDF output.

DESCRIPTION:	Produce RAF NetCDF flight file.

COPYRIGHT:	University Corporation for Atmospheric Research, 2004-18
-------------------------------------------------------------------------
*/

#ifndef _NetCDF_h_
#define	_NetCDF_h_

#include <map>
#include <queue>
#include "nimbus.h"

/* -------------------------------------------------------------------- */
class NetCDF
{
public:
  NetCDF();
  ~NetCDF();

  void	CreateFile(const char fileName[], size_t nRecords);
  void	SwitchToDataMode();
  void	WriteCoordinateVariableData();
  void	WriteNetCDF();
  void	WriteNetCDFfromMemory();
  void	ProcessFlightDate();
  void	QueueMissingData(int h, int m, int s, int nRecords);
  void	Sync();
  void	Close();


protected:

  void	writeTimeUnits(), writeMinMax();
  void	addCommonVariableAttributes(const var_base *var), addLandmarks();
  void	createSizeDistributionCoordinateDimVars(var_base *var);
  void  addVariableMetadata(const var_base *var);

  long	UTSeconds(double *record);
  void	SetBaseTime(double *record);
  void	WriteMissingRecords();

  void	putGlobalAttribute(const char attrName[], const float *value);
  void	putGlobalAttribute(const char attrName[], const char *value);
  void	putGlobalAttribute(const char attrName[], const std::string value);

  bool	variableIs_a_SizeDistribution(var_base *vp) const;

  int _ncid;

  bool _realTimeMode;

  struct tm _startFlight;
  long	_recordNumber;
  long	_timeVar;
  int	_timeOffsetID, _timeVarID;
  int   _timeLength; // length of time var - used when saving entire file in memory

  // To be deprecated.  Used in old ADS2 netCDF format.
  int	_baseTimeID;
  float	_timeOffset;

  // Rate, DimID
  std::map<int, int> _rateDimIDs;
  // VectorLen, DimID
  // SerialNumber, DimID
  std::map<std::string, int> _vectorDimIDs;
  int _bnds_dimid;

  // Count of netcCDF write errors.
  size_t _errCnt;

  // vector to hold entire Time variable in memory
  std::vector<int> _TimeSamples;
  // ADS2
  std::vector<int> _TimeOffsets;

struct missDat  /* (Time gap) / (missing data) information */
  {
  int           hour;
  int           minute;
  int           second;
  size_t        nRecords;
  } ;

  std::queue<struct missDat *> _missingRecords;

  static const std::string Program, Institution, Source, Address, Phone,
	Publisher_Name, Publisher_URL, Publisher_EMail,
	Creator_Name, Creator_URL, Creator_EMail,
	Conventions, ConventionsURL, ProcessorURL, NETCDF_FORMAT_VERSION;

  static const char *ISO8601_Z;

  static const int DEFAULT_TI_LENGTH;

};	// END NETCDF.H

#endif

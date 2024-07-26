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
  void	WriteNetCDF();
  void	ProcessFlightDate();
  void	QueueMissingData(int h, int m, int s, int nRecords);
  void	Sync();
  void	Close();


protected:

  int	writeBlank(int varid, size_t start[], size_t count[], int OutputRate);
  void	markDependedByList(const char target[]), writeTimeUnits();
  void	clearDependedByList(), printDependedByList(), writeMinMax();
  void	addCommonVariableAttributes(const var_base *var), addLandmarks();
  void	createSizeDistributionCoordinateDimVars(var_base *var);
  void  addVariableMetadata(const var_base *var);

  long	UTSeconds(double *record);
  void	SetBaseTime(double *record);
  void	WriteMissingRecords();
  void	BlankOutBadData();

  void	putGlobalAttribute(const char attrName[], const float *value);
  void	putGlobalAttribute(const char attrName[], const char *value);
  void	putGlobalAttribute(const char attrName[], const std::string value);


  int _ncid;

  bool _realTimeMode;

  struct tm _startFlight;
  long	_recordNumber;
  long	_timeVar;
  int	_timeOffsetID, _timeVarID;

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

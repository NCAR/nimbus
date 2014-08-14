/*
-------------------------------------------------------------------------
OBJECT NAME:	netcdf.c

FULL NAME:	NetCDF IO

ENTRY POINTS:	CreateNetCDF()
		WriteNetCDF()
		SyncNetCDF()
		CloseNetCDF()
		SetBaseTime()
		QueueMissingData()
		WriteMissingRecords()
		BlankOutBadData()
		readLandmarks()

STATIC FNS:	writeBlank()
		writeTimeUnits()
		clearDependedByList()
		markDependedByList()
		printDependedByList()
		addCommonVariableAttributes()
		addLandmarks()

DESCRIPTION:	This file has the routines necessary to Create and write
		data for distribution of NCAR/RAF aircraft data in netCDF
		format.

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2012
-------------------------------------------------------------------------
*/

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <map>
#include <sstream>

#include "nimbus.h"
#include "decode.h"
#include "gui.h"
#include <raf/ctape.h>
#include <netcdf.h>
#include <raf/raf_queue.h>
#include <raf/vardb.hh>
#include "svnInfo.h"

static const std::string Source = "NCAR Research Aviation Facility";
static const std::string Address = "P.O. Box 3000, Boulder, CO 80307-3000";
static const std::string Phone = "(303) 497-1030";
static const std::string URL = "http://www.eol.ucar.edu";
static const std::string EMail = "codiac at ucar.edu";
static const std::string Conventions = "NCAR-RAF/nimbus";
static const std::string ConventionsURL = "http://www.eol.ucar.edu/raf/Software/netCDF.html";
static const std::string NETCDF_FORMAT_VERSION = "1.3";

static const char *ISO8601_Z = "%FT%T %z";

static const int DEFAULT_TI_LENGTH = 17;

struct missDat	/* (Time gap) / (missing data) information */
  {
  int		hour;
  int		minute;
  int		second;
  size_t	nRecords;
  } ;

static int	fd = -1;
static struct tm StartFlight;
static long	recordNumber = 0;
static long	TimeVar = 0;

// To be deprecated.
static int	baseTimeID;
static float	TimeOffset = 0.0;


static Queue	*missingRecords;
static void	WriteMissingRecords();

extern NR_TYPE	*SampledData, *AveragedData, *HighRateData;
extern FILE	*LogFile;
extern VDBFile  *vardb;

int	FlightDate[3];		// HACK: for amlib
char	dateProcessed[64];	// For export to psql.cc


static int	writeBlank(int varid, size_t start[], size_t count[], int OutputRate);
static void	markDependedByList(const char target[]), writeTimeUnits();
static void	clearDependedByList(), printDependedByList(), writeMinMax();
static void	addCommonVariableAttributes(const var_base *var), addLandmarks();

void	AddPMS1dAttrs(int ncid, const var_base * rp), ReadMetaData(int fd),
	CheckAndAddAttrs(int fd, int varid, char name[]);

//      Rate, DimID
std::map<int, int> _rateDimIDs;
//   VectorLen, DimID
std::map<int, int> _vectorDimIDs;

/* -------------------------------------------------------------------- */
long UTSeconds(NR_TYPE *record)	// Seconds since midnight
{
  static int prev_day = 0;
  static int offset = 0;

  if (prev_day > 0 && (int)record[timeIndex[5]] != prev_day)
    offset += 86400;

  prev_day = (int)record[timeIndex[5]];

  return  (int)record[timeIndex[0]] * 3600
	+ (int)record[timeIndex[1]] * 60
	+ (int)record[timeIndex[2]]
	+ offset;
}

/* -------------------------------------------------------------------- */
void SetBaseTime(NR_TYPE *record)
{
  StartFlight.tm_isdst	= -1;

  StartFlight.tm_hour = (int)record[timeIndex[0]],
  StartFlight.tm_min = (int)record[timeIndex[1]],
  StartFlight.tm_sec = (int)record[timeIndex[2]];

  TimeVar = StartFlight.tm_hour * 3600 + StartFlight.tm_min * 60 + StartFlight.tm_sec;

  if (cfg.isADS3())	// We don't support BaseTime anymore.
    return;

  time_t BaseTime = timegm(&StartFlight);
  nc_put_var1_long(fd, baseTimeID, NULL, &BaseTime);

  if (BaseTime <= 0)
  {
    sprintf(buffer, "\nWARNING:  >>>>> base_time = %ld <<<<<\n\n", BaseTime);
    LogMessage(buffer);
  }

}	/* END SETBASETIME */

static int	timeOffsetID, timeVarID;


/* -------------------------------------------------------------------- */
static void putGlobalAttribute(const char attrName[], const float *value)
{
  nc_put_att_float(fd, NC_GLOBAL, attrName, NC_FLOAT, 1, value);
}

/* -------------------------------------------------------------------- */
static void putGlobalAttribute(const char attrName[], const char *value)
{
  nc_put_att_text(fd, NC_GLOBAL, attrName, strlen(value)+1, value);
}

/* -------------------------------------------------------------------- */
static void putGlobalAttribute(const char attrName[], const std::string value)
{
  putGlobalAttribute(attrName, value.c_str());
}

/* -------------------------------------------------------------------- */
void CreateNetCDF(const char fileName[])
{
  ILOG(("CreatingNetCDF, file = %s", fileName));

  ncopts = NC_VERBOSE;

  if (nc_create(fileName, NC_CLOBBER, &fd) != NC_NOERR)
  {
    HandleError("netcdf.c: Failed to create or open output file.");
  }

  if (cfg.ProductionRun())
    chmod(fileName, 0644);
  else
    chmod(fileName, 0666);


  /* Dimensions.
   */
  int TimeDim;
  nc_def_dim(fd, "Time", NC_UNLIMITED, &TimeDim);
  nc_def_dim(fd, "sps1", 1, &_rateDimIDs[1]);

  /* Global Attributes.
   */
  putGlobalAttribute("institution", Source);
  putGlobalAttribute("Address", Address);
  putGlobalAttribute("Phone", Phone);
  putGlobalAttribute("creator_url", URL);
  putGlobalAttribute("creator_email", EMail);
  putGlobalAttribute("Conventions", Conventions);
  putGlobalAttribute("ConventionsURL", ConventionsURL);
  putGlobalAttribute("Metadata_Conventions", "Unidata Dataset Discovery v1.0"); 
  putGlobalAttribute("ConventionsVersion", NETCDF_FORMAT_VERSION.c_str());
  putGlobalAttribute("standard_name_vocabulary", "CF-1.0");
  putGlobalAttribute("ProcessorRevision", &SVNREVISION[10]);
  putGlobalAttribute("NIDASrevision", cfg.NIDASrevision().c_str());

  if (strstr(SVNURL, "http"))
    strcpy(buffer, strstr(SVNURL, "http"));
  else
    strcpy(buffer, SVNURL);

  putGlobalAttribute("ProcessorURL", buffer);

  if (!cfg.ProductionRun())
    putGlobalAttribute("WARNING",
	"This file contains PRELIMINARY DATA that are NOT to be used for critical analysis.");


  {
  time_t	t;
  struct tm	tm;

  t = time(0);
  tm = *localtime(&t);
  strftime(dateProcessed, 64, ISO8601_Z, &tm);
  putGlobalAttribute("date_created", dateProcessed);

  if (LogFile)
    fprintf(LogFile, "Processed on: %s\n", dateProcessed);
  }

  putGlobalAttribute("ProjectName", cfg.ProjectName());	// Deprecate this sometime after 2011.  cjw
  putGlobalAttribute("project", cfg.ProjectName());
  putGlobalAttribute("Platform", cfg.TailNumber());
  if (cfg.isADS2()) putGlobalAttribute("ProjectNumber", cfg.ProjectNumber());
  putGlobalAttribute("FlightNumber", cfg.FlightNumber());

  if (cfg.ProcessingMode() == Config::RealTime)
    sprintf(buffer, "%02d/%02d/%04d", StartFlight.tm_mon+1, StartFlight.tm_mday, StartFlight.tm_year+1900);
  else
    strcpy(buffer, cfg.FlightDate().c_str());

  putGlobalAttribute("FlightDate", buffer);

  if (LogFile)
    fprintf(LogFile, "Flight Date: %s\n", buffer);

  /* Will be updated later.
   */
  memset(buffer, ' ', DEFAULT_TI_LENGTH);
  buffer[DEFAULT_TI_LENGTH] = '\0';
  putGlobalAttribute("TimeInterval", buffer);

  if (cfg.InterpolationType() == Config::Linear)
    putGlobalAttribute(InterpKey.c_str(), Interp_Linear);
  else
  if (cfg.InterpolationType() == Config::CubicSpline)
    putGlobalAttribute(InterpKey.c_str(), Interp_Cubic);
  else
  if (cfg.InterpolationType() == Config::AkimaSpline)
    putGlobalAttribute(InterpKey.c_str(), Interp_Akima);

  putGlobalAttribute("latitude_coordinate", cfg.CoordinateLatitude());
  putGlobalAttribute("longitude_coordinate", cfg.CoordinateLongitude());
  putGlobalAttribute("zaxis_coordinate", cfg.CoordinateAltitude());
  putGlobalAttribute("time_coordinate", cfg.CoordinateTime());

  float x = -32767.0;
  putGlobalAttribute("geospatial_lat_min", &x);
  putGlobalAttribute("geospatial_lat_max", &x);
  putGlobalAttribute("geospatial_lon_min", &x);
  putGlobalAttribute("geospatial_lon_max", &x);
  putGlobalAttribute("geospatial_vertical_min", &x);
  putGlobalAttribute("geospatial_vertical_max", &x);
  putGlobalAttribute("geospatial_vertical_positive", "up");
  putGlobalAttribute("geospatial_vertical_units", "m");
  putGlobalAttribute("wind_field", cfg.WindFieldVariables());

  addLandmarks();


  /* Write out Categories.
   */
  {
  VDBDictionary *cat_dict;
  cat_dict = vardb->get_dictionary("categories");

  buffer[0] = '\0';

  strcat(buffer, cat_dict->get_entry(0).c_str());
  for (size_t i = 1; i < cat_dict->num_entries(); ++i)
    {
    strcat(buffer, ",");
    strcat(buffer, cat_dict->get_entry(i).c_str());
    }

  putGlobalAttribute("Categories", buffer);
  }



  /* First dimension is time dimension.
   * Second is Rate in Hz.
   * Third is Vector Length.
   */
  int ndims, dims[3];
  dims[0] = TimeDim;


  /* Time Variables.
   */
  if (cfg.isADS2())  // When you remove these 2, look TimeOffset and make sure everything Jives.
  {
    nc_def_var(fd, "base_time", NC_LONG, 0, 0, &baseTimeID);
    strcpy(buffer, "seconds since 1970-01-01 00:00:00 +0000");
    nc_put_att_text(fd, baseTimeID, "units", strlen(buffer)+1, buffer);
    strcpy(buffer, "Start time of data recording.");
    nc_put_att_text(fd, baseTimeID, "long_name", strlen(buffer)+1, buffer);
  }

  nc_def_var(fd, "Time", NC_LONG, 1, dims, &timeVarID);
  strcpy(buffer, "time of measurement");
  nc_put_att_text(fd, timeVarID, "long_name", strlen(buffer)+1, buffer);
  strcpy(buffer, "time");
  nc_put_att_text(fd, timeVarID, "standard_name", strlen(buffer)+1, buffer);

  if (cfg.isADS2())
  {
    nc_def_var(fd, "time_offset", NC_FLOAT, 1, dims, &timeOffsetID);
    strcpy(buffer, "Seconds since base_time.");
    nc_put_att_text(fd, timeOffsetID, "long_name", strlen(buffer)+1, buffer);
  }


  // Write units for both Time & time_offset.
  writeTimeUnits();


  /* SDI variables.
   */

  /* For each variable:
   *	- Set dimensions
   *	- define variable
   *	- Set attributes
   *	- Set data_pointer
   */

  /* Raw/"block probe" variables.
   */
  for (size_t i = 0; i < raw.size(); ++i)
  {
    RAWTBL *rp;
    if ((rp = raw[i])->Output == false)
      continue;

    if (rp->OutputRate == 0)
    {
      char msg[256];

      sprintf(msg, "%s: Assertion `OutputRate > 0' failed.", rp->name);
      LogMessage(msg);
      quit();
    }

    // Check to see if dimension exists.  If not, create it.
    if (_rateDimIDs.find(rp->OutputRate) == _rateDimIDs.end())
    {
      char tmp[32];
      sprintf(tmp, "sps%zu", rp->OutputRate);
      nc_def_dim(fd, tmp, rp->OutputRate, &_rateDimIDs[rp->OutputRate]);
    }

    dims[1] = _rateDimIDs[rp->OutputRate];
    if (rp->OutputRate == 1)
      ndims = 1;
    else
      ndims = 2;

    if (rp->Length > 1)
    {
      // Check to see if dimension exists.  If not, create it.
      if (_vectorDimIDs.find(rp->Length) == _vectorDimIDs.end())
      {
        char tmp[32];
        sprintf(tmp, "Vector%zu", rp->Length);
        nc_def_dim(fd, tmp, rp->Length, &_vectorDimIDs[rp->Length]);
      }

      ndims = 3;
      dims[2] = _vectorDimIDs[rp->Length];
    }

//printf("RAW: %s\n", rp->name);
    nc_def_var(fd, rp->name, NC_FLOAT, ndims, dims, &rp->varid);

    addCommonVariableAttributes(rp);

    nc_put_att_long(fd, rp->varid, "SampledRate", NC_LONG, 1, (long *)&rp->SampleRate);

    if (cfg.TimeShifting() && rp->StaticLag != 0)
    {
      nc_put_att_int(fd, rp->varid, "TimeLag", NC_INT, 1, &rp->StaticLag);
      nc_put_att_text(fd, rp->varid, "TimeLagUnits", 13, "milliseconds");
    }

    if (cfg.Despiking() && rp->SpikeSlope != 0.0)
      nc_put_att_float(fd, rp->varid, "DespikeSlope", NC_FLOAT, 1, &rp->SpikeSlope);

    nc_put_att_text(fd, rp->varid, "DataQuality", strlen(rp->DataQuality)+1,
		rp->DataQuality);

    if (rp->cof.size() > 0)
    {
      std::vector<float> cof;
      for (size_t j = 0; j < rp->cof.size(); ++j)
        cof.push_back((float)rp->cof[j]);

      nc_put_att_float(fd, rp->varid, "CalibrationCoefficients", NC_FLOAT,
               cof.size(), &cof[0]);
    }

    if (rp->Modulo)
    {
      float mod[2];

      mod[0] = (float)rp->Modulo->value[0];
      mod[1] = (float)rp->Modulo->value[1];
      ncattput(fd, rp->varid, "modulus_range", NC_FLOAT, 2, mod);
    }


    if (rp->Length > 3 &&
	(rp->ProbeType & PROBE_PMS2D || rp->ProbeType & PROBE_PMS1D ||
	 rp->ProbeType & PROBE_RDMA || rp->ProbeType & PROBE_CLMT))
      AddPMS1dAttrs(fd, rp);

    CheckAndAddAttrs(fd, rp->varid, rp->name);
  }



  /* Derived variables.
   */
  for (size_t i = 0; i < derived.size(); ++i)
  {
    DERTBL *dp;
    if ((dp = derived[i])->Output == false)
      continue;

    if (dp->OutputRate == 0)
    {
      char msg[256];

      sprintf(msg, "%s: Assertion `OutputRate > 0' failed.", dp->name);
      LogMessage(msg);
      quit();
    }

    // Check to see if dimension exists.  If not, create it.
    if (_rateDimIDs.find(dp->OutputRate) == _rateDimIDs.end())
    {
      char tmp[32];
      sprintf(tmp, "sps%zu", dp->OutputRate);
      nc_def_dim(fd, tmp, dp->OutputRate, &_rateDimIDs[dp->OutputRate]);
    }

    dims[1] = _rateDimIDs[dp->OutputRate];
    if (dp->OutputRate == 1)
      ndims = 1;
    else
      ndims = 2;

    if (dp->Length > 1)
    {
      // Check to see if dimension exists.  If not, create it.
      if (_vectorDimIDs.find(dp->Length) == _vectorDimIDs.end())
        {
        char tmp[32];
        sprintf(tmp, "Vector%zu", dp->Length);
        nc_def_dim(fd, tmp, dp->Length, &_vectorDimIDs[dp->Length]);
        }

      ndims = 3;
      dims[2] = _vectorDimIDs[dp->Length];
    }

//printf("DER: %s\n", dp->name); fflush(stdout);
    nc_def_var(fd, dp->name, NC_FLOAT, ndims, dims, &dp->varid);

    addCommonVariableAttributes(dp);

    nc_put_att_text(fd, dp->varid, "DataQuality", strlen(dp->DataQuality)+1,
		dp->DataQuality);

    sprintf(buffer, "%zu", dp->ndep);
    for (size_t j = 0; j < dp->ndep; ++j)
    {
      strcat(buffer, " ");
      strcat(buffer, dp->depend[j]);
    }

    nc_put_att_text(fd, dp->varid, "Dependencies", strlen(buffer)+1,buffer);

    if (dp->Modulo)
    {
      float mod[2];

      mod[0] = (float)dp->Modulo->value[0];
      mod[1] = (float)dp->Modulo->value[1];
      nc_put_att_float(fd, dp->varid, "modulus_range", NC_FLOAT, 2, mod);
    }

    CheckAndAddAttrs(fd, dp->varid, dp->name);

    if (dp->Length > 3 &&
	(dp->ProbeType & PROBE_PMS2D || dp->ProbeType & PROBE_PMS1D ||
	 dp->ProbeType & PROBE_RDMA || dp->ProbeType & PROBE_CLMT))
      AddPMS1dAttrs(fd, dp);
  }

  ReadMetaData(fd);

int old_fill_mode;
nc_set_fill(fd, NC_NOFILL, &old_fill_mode); /* set nofill */

}	/* END CREATENETCDF */

/* -------------------------------------------------------------------- */
void SwitchNetCDFtoDataMode()
{
  nc_enddef(fd);
  nc_sync(fd);

}	/* END SWITCHNETCDFTODATAMODE */

/* -------------------------------------------------------------------- */
void WriteNetCDF()
{
  float *data;
  int status;

  struct missDat	*dp;
  static int		errCnt = 0;
  static bool		firstWrite = true;

  if (firstWrite)
  {
    SetBaseTime(SampledData);
    firstWrite = false;
  }

  size_t start[3], count[3];
  start[0] = recordNumber; start[1] = start[2] = 0;
  count[0] = 1;

  // Output Time variable as seconds since midnight (UTSeconds).
  long ut_seconds = UTSeconds(SampledData);
  nc_put_var1_long(fd, timeVarID, start, &ut_seconds);
  if (cfg.isADS2())
    nc_put_var1_float(fd, timeOffsetID, start, &TimeOffset);

  for (size_t i = 0; i < raw.size(); ++i)
  {
    RAWTBL *rp;
    if ((rp = raw[i])->Output == false)
      continue;

    size_t N = rp->Length * rp->OutputRate;
    count[1] = rp->OutputRate;
    count[2] = rp->Length;

    data = new float[N];

    if (rp->OutputRate == Config::LowRate)
    {
      for (size_t j = 0; j < N; ++j)
        data[j] = (float)AveragedData[rp->LRstart + j];
    }
    else
    {
      if (rp->OutputRate == rp->SampleRate && rp->OutputRate != (size_t)cfg.ProcessingRate())
      {
        for (size_t j = 0; j < N; ++j)
          data[j] = (float)SampledData[rp->SRstart + j];
      }
      else
      {
        for (size_t j = 0; j < N; ++j)
          data[j] = (float)HighRateData[rp->HRstart + j];
      }
    }

    for (size_t j = 0; j < N; ++j)
      if (isnan(data[j]))
        data[j] = (float)MISSING_VALUE;

    status = nc_put_vara_float(fd, rp->varid, start, count, data);
    if (status != NC_NOERR)
    {
      fprintf(stderr,
            "WriteNetCDF: write failure, variable %s, RecordNumber = %ld, status = %d\n",
            rp->name, recordNumber, status);
      fprintf(stderr, "%s\n", nc_strerror(status));
      ++errCnt;
    }
    delete [] data;
  }


  for (size_t i = 0; i < derived.size(); ++i)
  {
    DERTBL *dp;
    if ((dp = derived[i])->Output == false)
      continue;

    size_t N = dp->Length * dp->OutputRate;
    count[1] = dp->OutputRate;
    count[2] = dp->Length;

    data = new float[N];

    if (dp->OutputRate == Config::LowRate)
    {
      for (size_t j = 0; j < N; ++j)
        data[j] = (float)AveragedData[dp->LRstart + j];
    }
    else
    {
      for (size_t j = 0; j < N; ++j)
        data[j] = (float)HighRateData[dp->HRstart + j];
    }

    for (size_t j = 0; j < N; ++j)
      if (isnan(data[j]))
        data[j] = (float)MISSING_VALUE;

    status = nc_put_vara_float(fd, dp->varid, start, count, data);
    if (status != NC_NOERR)
    {
      fprintf(stderr,
            "WriteNetCDF: write failure, variable %s, RecordNumber = %ld, status = %d\n",
            dp->name, recordNumber, status);
      fprintf(stderr, "%s\n", nc_strerror(status));
      ++errCnt;
    }
    delete [] data;
  }

  if (errCnt > 10)
  {
    fprintf(stderr, "Too many write errors, closing file and exiting...\n");
    quit();
  }

  TimeOffset += 1.0;
  ++TimeVar;
  ++recordNumber;

  if ( (dp = (struct missDat *)FrontQueue(missingRecords)) )
  {
    int hour, min, sec;

    hour = (int)SampledData[raw[SearchTable(raw, "HOUR")]->SRstart];
    min = (int)SampledData[raw[SearchTable(raw, "MINUTE")]->SRstart];
    sec = (int)SampledData[raw[SearchTable(raw, "SECOND")]->SRstart];

    if (hour == dp->hour && min == dp->minute && sec == dp->second)
      WriteMissingRecords();
  }
}	/* END WRITENETCDF */

/* -------------------------------------------------------------------- */
void QueueMissingData(int h, int m, int s, int nRecords)
{
  struct missDat	*dp;
  static int		firstTime = true;

  if (firstTime)
  {
    missingRecords = CreateQueue();
    firstTime = false;
  }

  dp = new struct missDat;

  dp->hour = h;
  dp->minute = m;
  dp->second = s;
  dp->nRecords = nRecords;

  EnQueue(missingRecords, (void *)dp);

  if (cfg.ProcessingMode() == Config::RealTime)
    WriteMissingRecords();

}	/* END QUEUEMISSINGDATA */

/* -------------------------------------------------------------------- */
static void WriteMissingRecords()
{
  size_t	i;
  float		*d, hour, minute, second;
  void		*ldp[MAX_VARIABLES];
  struct missDat	*dp;

  dp = (struct missDat *)FrontQueue(missingRecords);
  d = new float[5000];
  /* 5000 is fastest sampling rate */

  for (i = 0; i < 5000; ++i)
    d[i] = (float)MISSING_VALUE;

  int indx = 0;
  ldp[indx++] = (void *)&TimeVar;
  if (cfg.isADS2())
    ldp[indx++] = (void *)&TimeOffset;

  for (i = 0; i < raw.size(); ++i)
    {
    if (raw[i]->Output == false)
      continue;

    if (strcmp("HOUR", raw[i]->name) == 0)
      {
      ldp[indx++] = &hour;
      hour = dp->hour;
      }
    else
    if (strcmp("MINUTE", raw[i]->name) == 0)
      {
      ldp[indx++] = &minute;
      minute = dp->minute;
      }
    else
    if (strcmp("SECOND", raw[i]->name) == 0)
      {
      ldp[indx++] = &second;
      second = dp->second;
      }
    else
      ldp[indx++] = (void *)d;
    }

  for (i = 0; i < derived.size(); ++i)
    if (derived[i]->Output)
      ldp[indx++] = (void *)d;

  for (i = 0; i < dp->nRecords; ++i)
    {
    if (++second > 59.0)
      {
      second = 0.0;
      if (++minute > 59.0)
        {
        minute = 0.0;
        if (++hour > 23.0)
          hour = 0.0;
        }
      }

    ncrecput(fd, recordNumber, ldp);

    TimeOffset += 1.0;
    ++TimeVar;
    ++recordNumber;
    }

  DeQueue(missingRecords);
  delete [] d;

}	/* END WRITEMISSINGRECORDS */

/* -------------------------------------------------------------------- */
void SyncNetCDF()
{
  FormatTimeSegmentsForOutputFile(buffer);

  nc_redef(fd);
  putGlobalAttribute("TimeInterval", buffer);
  nc_enddef(fd);

  nc_sync(fd);

}	/* END SYNCNETCDF */

/* -------------------------------------------------------------------- */
void CloseNetCDF()
{
  if (fd == ERR)
    return;

  nc_redef(fd);
  writeMinMax();
  writeTimeUnits();
  nc_enddef(fd);

  SyncNetCDF();

  nc_close(fd);
  fd = ERR;

  LogMessage("Time interval(s) completed : ");
  LogMessage(buffer);
  LogMessage("\n");

}	/* END CLOSENETCDF */

/* -------------------------------------------------------------------- */
void BlankOutBadData()
{
  char  *blanks[512];
  int	sTime[4], eTime[4];	// Requested Start/End Time.
  size_t start[3], count[3];
  int	fsTime[4], feTime[4];	// File Start/End Time.


  /* Come through as a second pass after all processing has been done, and
   * replace "bad" segments with MISSING_VALUE.
   */
  sprintf(buffer, "%s.%s", BLANKVARS.c_str(), cfg.FlightNumber().c_str());
  if (AccessProjectFile(buffer, "r") == FALSE)
    return;

  ReadTextFile(buffer, blanks);

  /* Acquire file start & end times. */
  FormatTimeSegmentsForOutputFile(buffer);
  sscanf(buffer, "%d:%d:%d-%d:%d:%d",
	&fsTime[0], &fsTime[1], &fsTime[2], &feTime[0], &feTime[1], &feTime[2]);

  fsTime[3] = SecondsSinceMidnite(fsTime);
  feTime[3] = SecondsSinceMidnite(feTime);

  /* Check for time wrap within output data file
   */
  if (feTime[3] < fsTime[3]) feTime[3] += 86400;


  /* First do all variables where the DataQuality flag is bad:  whole flight.
   */
  start[0] = start[1] = start[2] = 0;
  count[0] = feTime[3] - fsTime[3] + 1;

  for (size_t i = 0; i < derived.size(); ++i)
    {
    if (strcmp(derived[i]->DataQuality, dataQuality[BAD]) == 0)
      {
      sprintf(buffer, "Blanking %s from %02d:%02d:%02d to %02d:%02d:%02d.\n",
       derived[i]->name, fsTime[0], fsTime[1], fsTime[2], feTime[0],
       feTime[1], feTime[2]);
      LogMessage(buffer);

      clearDependedByList();

      if (derived[i]->DependedUpon)
        {
        markDependedByList(derived[i]->name);
        printDependedByList();
        }
      if (writeBlank(derived[i]->varid, start, count, derived[i]->OutputRate) != NC_NOERR)
        {
        sprintf(buffer, "Failure writing BadData for variable %s.\n",
                derived[i]->name);
        LogMessage(buffer);
        }
      }
    }


  /* Loop through each line item in BLANKVARS file.
   */
  for (int i = 0; blanks[i]; ++i)
    {
    int	index;
    char target[NAMELEN];

    sscanf(blanks[i], "%s %d:%d:%d %d:%d:%d", target,
	&sTime[0], &sTime[1], &sTime[2], &eTime[0], &eTime[1], &eTime[2]);

    /* Turn time period into record numbers. */
    sTime[3] = SecondsSinceMidnite(sTime);
    eTime[3] = SecondsSinceMidnite(eTime);

    if (eTime[3] == sTime[3])
      eTime[3] = sTime[3] + 1;

    if (eTime[3] < sTime[3])
      eTime[3] += 86400;

    /* Sync blanking times to file times  (RLR  13 July 2001)
     */
    if (sTime[3] < fsTime[3]) sTime[3] += 86400;
    if (eTime[3] < sTime[3]) eTime[3] += 86400;

    /*  Test for invalid time period (ignore and warn)  (RLR  13 July 2001)
     */
    if (sTime[3] > feTime[3] || eTime[3] < fsTime[3])
      {
      sprintf(buffer, "Blanking times (%02d:%02d:%02d to %02d:%02d:%02d) not valid for %s.\n", sTime[0], sTime[1], sTime[2], eTime[0], eTime[1], eTime[2], target);
      LogMessage(buffer);
      continue;
      }

    if (sTime[3] < fsTime[3])
      memcpy((void *)sTime, (void *)fsTime, sizeof(fsTime));

    if (eTime[3] > feTime[3])
      memcpy((void *)eTime, (void *)feTime, sizeof(feTime));

    sprintf(buffer, "Blanking %s from %02d:%02d:%02d to %02d:%02d:%02d.\n",
    target, sTime[0], sTime[1], sTime[2], eTime[0], eTime[1], eTime[2]);
    LogMessage(buffer);

    start[0] = sTime[3] - fsTime[3];
    start[1] = start[2] = 0;
    count[0] = eTime[3] - sTime[3] + 1;

    if (start[0] < 0)
      {
      sprintf(buffer, "Start time precedes file start time for %s.\n", target);
      LogMessage(buffer);
      continue;
      }

    clearDependedByList();

    if ((index = SearchTable(raw, target)) != ERR &&
        raw[index]->Output)
      {		// Do nothing, but avoid error message below if it was a raw variable.
      }
    else
    if ((index = SearchTable(derived, target)) != ERR &&
	derived[index]->Output)
      {
/*  See if measurement has already been blanked for whole flight  */
      if (strcmp(derived[index]->DataQuality, dataQuality[BAD]) == 0)
        {
        sprintf(buffer, "%s has already been blanked because the DataQuality flag is Bad.\n",derived[index]->name);
        LogMessage(buffer);
        }
      else
        {
        if (derived[index]->DependedUpon)
          {
          markDependedByList(derived[index]->name);
          printDependedByList();
          }
        if (writeBlank(derived[index]->varid, start, count, derived[index]->OutputRate) != NC_NOERR)
          {
          sprintf(buffer, "Failure writing BadData for variable %s.\n",
		  derived[index]->name);
          LogMessage(buffer);
          }
        }
      }
    else
      {
      sprintf(buffer, "  WARNING: %s NOT found!  Continuing.\n", target);
      LogMessage(buffer);
      }
    }
}       /* END BLANKOUTBADDATA */

/* -------------------------------------------------------------------- */
static int writeBlank(int varid, size_t start[], size_t count[], int OutputRate)
{
  long	nValues;
  float	*p;

  count[1] = OutputRate;
  count[2] = 1;

  nValues = count[0] * count[1] * count[2];
  p = new float[nValues];

  for (int i = 0; i < nValues; ++i)
    p[i] = (float)MISSING_VALUE;

  return(nc_put_vara_float(fd, varid, start, count, p));
}

/* -------------------------------------------------------------------- */
static void writeMinMax()
{
  float range[2];

  for (size_t i = 0; i < raw.size(); ++i)
  {
    RAWTBL * rp = raw[i];
    if (!rp->Output)
      continue;

    range[0] = rp->min;
    range[1] = rp->max;
    nc_put_att_float(fd, rp->varid, "actual_range", NC_FLOAT, 2, range);
    if (cfg.CoordinateLatitude().compare(rp->name) == 0) {
      putGlobalAttribute("geospatial_lat_min", &rp->min);
      putGlobalAttribute("geospatial_lat_max", &rp->max);
    }
    if (cfg.CoordinateLongitude().compare(rp->name) == 0) {
      putGlobalAttribute("geospatial_lon_min", &rp->min);
      putGlobalAttribute("geospatial_lon_max", &rp->max);
    }
    if (cfg.CoordinateAltitude().compare(rp->name) == 0) {
      putGlobalAttribute("geospatial_vertical_min", &rp->min);
      putGlobalAttribute("geospatial_vertical_max", &rp->max);
    }
  }
  for (size_t i = 0; i < derived.size(); ++i)
  {
    DERTBL * dp = derived[i];
    if (!dp->Output)
      continue;

    range[0] = dp->min;
    range[1] = dp->max;
    nc_put_att_float(fd, dp->varid, "actual_range", NC_FLOAT, 2, range);
    if (cfg.CoordinateLatitude().compare(dp->name) == 0) {
      putGlobalAttribute("geospatial_lat_min", &dp->min);
      putGlobalAttribute("geospatial_lat_max", &dp->max);
    }
    if (cfg.CoordinateLongitude().compare(dp->name) == 0) {
      putGlobalAttribute("geospatial_lon_min", &dp->min);
      putGlobalAttribute("geospatial_lon_max", &dp->max);
    }
    if (cfg.CoordinateAltitude().compare(dp->name) == 0) {
      putGlobalAttribute("geospatial_vertical_min", &dp->min);
      putGlobalAttribute("geospatial_vertical_max", &dp->max);
    }
  }
}

/* -------------------------------------------------------------------- */
static void writeTimeUnits()
{
  const char *format = "seconds since %F %T %z";
  struct tm tmp;

  StartFlight.tm_isdst = 0;
  tmp = StartFlight;
  tmp.tm_hour = tmp.tm_min = tmp.tm_sec = 0;
  strftime(buffer, 256, format, &tmp);
  nc_put_att_text(fd, timeVarID, "units", strlen(buffer)+1, buffer);
  nc_put_att_text(fd, timeVarID, "strptime_format", strlen(format)+1, format);
  if (cfg.isADS2())
    nc_put_att_text(fd, timeOffsetID, "units", strlen(buffer)+1, buffer);


  strftime(buffer, 256, ISO8601_Z, &StartFlight);
  putGlobalAttribute("time_coverage_start", buffer);

  time_t endTime = timegm(&tmp);
  endTime += (TimeVar - 1);
  struct tm EndFlight;
  gmtime_r(&endTime, &EndFlight);
  EndFlight.tm_isdst = 0;
  strftime(buffer, 256, ISO8601_Z, &EndFlight);
  putGlobalAttribute("time_coverage_end", buffer);
}

/* -------------------------------------------------------------------- */
static void
clearDependedByList()
{
  for (size_t i = 0; i < derived.size(); ++i)
    if (derived[i]->DependedUpon & 0xf0)
      derived[i]->DependedUpon &= 0x0f;
}

static void
markDependedByList(const char target[])
{
  for (size_t i = 0; i < derived.size(); ++i)
  {
    DERTBL *dp = derived[i];

    for (size_t j = 0; j < dp->ndep; ++j)
      if (strcmp(target, dp->depend[j]) == 0)
      {
        dp->DependedUpon |= 0xf0;
        markDependedByList(dp->name);
      }
  }
}

static void
printDependedByList()
{
  LogMessage(" The following variables depend upon this variable:\n ");

  for (size_t i = 0; i < derived.size(); ++i)
    if (derived[i]->DependedUpon & 0xf0)
    {
      sprintf(buffer, " %s", derived[i]->name);
      LogMessage(buffer);
    }

  LogMessage("\n");
}

/* -------------------------------------------------------------------- */
static void addCommonVariableAttributes(const var_base *var)
{
  VDBVar *vdb_var = vardb->get_var(var->name);

  float miss_val = (float)MISSING_VALUE;
  nc_put_att_float(fd, var->varid, "_FillValue", NC_FLOAT, 1, &miss_val);

/* Once we support individual _FillValue in Q missing data routine, then use this line.
  float fv = VarDB_GetFillValue(var->name);
  ncattput(fd, var->varid, "_FillValue", NC_FLOAT, 1, &fv);
*/

  strcpy(buffer, var->Units.c_str());
  nc_put_att_text(fd, var->varid, "units", strlen(buffer)+1, buffer);

  strcpy(buffer, var->LongName.c_str());
  nc_put_att_text(fd, var->varid, "long_name", strlen(buffer)+1, buffer);

  if (vdb_var)
  {
    std::string std_name = vdb_var->get_attribute("standard_name");
    if (std_name.size() > 0)
      nc_put_att_text(fd, var->varid, "standard_name", std_name.size()+1, std_name.c_str());

    std::string min = vdb_var->get_attribute("min_limit");
    std::string max = vdb_var->get_attribute("max_limit");
    if (min.size() > 0 && max.size() > 0)
    {
      float   range[2];

      range[0] = atof(min.c_str());
      range[1] = atof(max.c_str());
      nc_put_att_float(fd, var->varid, "valid_range", NC_FLOAT, 2, range);
    }

    delete vdb_var;
  }

  float zero[2] = { 0.0, 0.0 };
  nc_put_att_float(fd, var->varid, "actual_range", NC_FLOAT, 2, zero);

  if (var->CategoryList.size() > 0)
  {
    char temp[32];
    strcpy(temp, var->CategoryList[0].c_str());
    if ( strcmp(temp, "None") )
      nc_put_att_text(fd, var->varid, "Category", strlen(temp)+1, temp);
  }

  if (var->SerialNumber.length() > 0)
    nc_put_att_text(fd, var->varid, "SerialNumber",
	var->SerialNumber.length()+1, var->SerialNumber.c_str());

}	/* END ADDCOMMONVARIABLEATTRIBUTES */

/* -------------------------------------------------------------------- */
void ProcessFlightDate()
{
  if (cfg.ProcessingMode() == Config::RealTime)
  {
    time_t      x = time(NULL);
    gmtime_r(&x, &StartFlight);
    StartFlight.tm_mon += 1;
    StartFlight.tm_year += 1900;  /* will be subtracted off later       */
  }
  else
  {
    strcpy(buffer, cfg.FlightDate().c_str());
    if (cfg.FlightDate().size() > 0)
      sscanf(buffer, "%d/%d/%d", &StartFlight.tm_mon, &StartFlight.tm_mday, &StartFlight.tm_year);
  }

  FlightDate[0] = StartFlight.tm_mon;  /* HACK: for amlib/xlate/time.c */
  FlightDate[1] = StartFlight.tm_mday;
  FlightDate[2] = StartFlight.tm_year;

  StartFlight.tm_year -= 1900;
  StartFlight.tm_mon -= 1;
}

/* -------------------------------------------------------------------- */
std::string readLandmarks()
{
  char *marks[200], label[128];
  float lat, lon;
  std::stringstream landMarks;
  landMarks.str("");

  ReadTextFile(LANDMARKS, marks);

  for (int i = 0; marks[i] != NULL; ++i)
    {
    sscanf(marks[i], "%f %f %s\n", &lat, &lon, label);

    if (i > 0)
      landMarks << ",";
    landMarks << lat << " " << lon << " " << label;
    }

  FreeTextFile(marks);
  return landMarks.str();
}

/* -------------------------------------------------------------------- */
static void addLandmarks()
{
  putGlobalAttribute("landmarks", readLandmarks());
}

/* END NETCDF.C */

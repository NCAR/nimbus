/*
-------------------------------------------------------------------------
OBJECT NAME:	netcdf.c

FULL NAME:	NetCDF IO

ENTRY POINTS:	CreateNetCDF()
		WriteNetCDF()
		WriteNetCDF_MRF()
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

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2005
-------------------------------------------------------------------------
*/

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <ctime>
#include <map>
#include <sstream>

#include "nimbus.h"
#include "decode.h"
#include "gui.h"
#include "ctape.h"
#include "netcdf.h"
#include "raf_queue.h"
#include "vardb.h"
#include "svnInfo.h"

static const std::string Source = "NCAR Research Aviation Facility";
static const std::string Address = "P.O. Box 3000, Boulder, CO 80307-3000";
static const std::string Phone = "(303) 497-1030";
static const std::string Conventions = "NCAR-RAF/nimbus";
static const std::string ConventionsURL = "http://www.eol.ucar.edu/raf/Software/netCDF.html";
static const std::string NETCDF_FORMAT_VERSION = "1.3";


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
static void	*data_p[MAX_VARIABLES];	// Data pointers to pass into ncrecput()
static long	recordNumber = 0;
static long	TimeVar = 0;
static int	startVarIndx = 0;

// To be depracated.
static int	baseTimeID;
static float	TimeOffset = 0.0;


static Queue	*missingRecords;
static void	WriteMissingRecords();

extern NR_TYPE	*SampledData, *AveragedData, *HighRateData;
extern FILE	*LogFile;

int	FlightDate[3];		// HACK: for amlib
char	dateProcessed[64];	// For export to psql.cc


static int	writeBlank(int varid, long start[], long count[], int OutputRate);
static void	markDependedByList(char target[]), writeTimeUnits();
static void	clearDependedByList(), printDependedByList();
static void	addCommonVariableAttributes(var_base *var), addLandmarks();

void	AddPMS1dAttrs(int ncid, var_base * rp), ReadMetaData(int fd),
	CheckAndAddAttrs(int fd, int varid, char name[]);

//      Rate, DimID
std::map<int, int> _rateDimIDs;
//   VectorLen, DimID
std::map<int, int> _vectorDimIDs;

/* -------------------------------------------------------------------- */
void SetBaseTime(NR_TYPE *record)
{
  time_t BaseTime;

  StartFlight.tm_isdst	= -1;

  StartFlight.tm_hour = (int)record[timeIndex[0]],
  StartFlight.tm_min = (int)record[timeIndex[1]],
  StartFlight.tm_sec = (int)record[timeIndex[2]];

  BaseTime = timegm(&StartFlight);

  if (cfg.isADS3())	// We don't support BaseTime anymore.
    return;

  BaseTime = timegm(&StartFlight);
  ncvarput1(fd, baseTimeID, NULL, (void *)&BaseTime);

  if (BaseTime <= 0)
  {
    sprintf(buffer, "\nWARNING:  >>>>> base_time = %ld <<<<<\n\n", BaseTime);
    LogMessage(buffer);
  }

}	/* END SETBASETIME */

static int	timeOffsetID, timeVarID;


/* -------------------------------------------------------------------- */
static void putGlobalAttribute(const char attrName[], const char *value)
{
  ncattput(fd, NC_GLOBAL, attrName, NC_CHAR, strlen(value)+1, (void *)value);
}

/* -------------------------------------------------------------------- */
static void putGlobalAttribute(const char attrName[], const std::string value)
{
  putGlobalAttribute(attrName, value.c_str());
}

/* -------------------------------------------------------------------- */
void CreateNetCDF(const char fileName[])
{
  fd = nccreate(fileName, NC_CLOBBER);

  if (cfg.ProductionRun())
    chmod(fileName, 0644);
  else
    chmod(fileName, 0666);


  /* Dimensions.
   */
  int TimeDim = ncdimdef(fd, "Time", NC_UNLIMITED);
  _rateDimIDs[1] = ncdimdef(fd, "sps1", 1);

  /* Global Attributes.
   */
  putGlobalAttribute("Source", Source);
  putGlobalAttribute("Address", Address);
  putGlobalAttribute("Phone", Phone);
  putGlobalAttribute("Conventions", Conventions);
  putGlobalAttribute("ConventionsURL", ConventionsURL);
  putGlobalAttribute("ConventionsVersion", NETCDF_FORMAT_VERSION.c_str());
  putGlobalAttribute("ProcessorRevision", &SVNREVISION[10]);

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
  strftime(dateProcessed, 64, "%F %T %z", &tm);
  putGlobalAttribute("DateProcessed", dateProcessed);

  if (LogFile)
    fprintf(LogFile, "Processed on: %s\n", dateProcessed);
  }

  putGlobalAttribute("ProjectName", cfg.ProjectName());
  putGlobalAttribute("Aircraft", cfg.TailNumber());
  putGlobalAttribute("ProjectNumber", cfg.ProjectNumber());
  putGlobalAttribute("FlightNumber", cfg.FlightNumber());

  if (cfg.ProcessingMode() == Config::RealTime)
    sprintf(buffer, "%02d/%02d/%04d", StartFlight.tm_mon+1, StartFlight.tm_mday, StartFlight.tm_year+1900);
  else
    strcpy(buffer, cfg.FlightDate().c_str());

  putGlobalAttribute("FlightDate", buffer);

  if (LogFile)
    fprintf(LogFile, "Flight Date: %s\n", buffer);

  if (cfg.InterpolationType() == Config::Linear)
    putGlobalAttribute(InterpKey.c_str(), Interp_Linear);
  else
  if (cfg.InterpolationType() == Config::CubicSpline)
    putGlobalAttribute(InterpKey.c_str(), Interp_Cubic);
  else
  if (cfg.InterpolationType() == Config::AkimaSpline)
    putGlobalAttribute(InterpKey.c_str(), Interp_Akima);

  putGlobalAttribute("coordinates", cfg.CoordinateVariables());
  putGlobalAttribute("wind_field", cfg.WindFieldVariables());
  addLandmarks();

  /* Will be updated later.
   */
  memset(buffer, ' ', DEFAULT_TI_LENGTH);
  buffer[DEFAULT_TI_LENGTH] = '\0';
  putGlobalAttribute("TimeInterval", buffer);


  /* Write out Categories.
   */
  {
  char *list[128];

  int catIdx = VarDB_GetCategoryList(list);

  buffer[0] = '\0';

  for (int i = 1; i < catIdx; ++i)	/* Skip category "None"	*/
    {
    strcat(buffer, list[i]);
    if (i != catIdx - 1)
      strcat(buffer, ",");
    }

  putGlobalAttribute("Categories", buffer);
  }



  /* First dimension is time dimension.
   * Second is Rate in Hz.
   * Third is Vector Length.
   */
  int ndims, dims[3];
  dims[0] = TimeDim;
  int indx = 0;	// Index for data_p


  /* Time Variables.
   */
  if (cfg.isADS2())  // When you remove these 2, look TimeOffset and make sure everything Jives.
  {
    baseTimeID = ncvardef(fd, "base_time", NC_LONG, 0, 0);
    strcpy(buffer, "seconds since 1970-01-01 00:00:00 +0000");
    ncattput(fd, baseTimeID, "units", NC_CHAR, strlen(buffer)+1, buffer);
    strcpy(buffer, "Start time of data recording.");
    ncattput(fd, baseTimeID, "long_name", NC_CHAR, strlen(buffer)+1, buffer);
  }

  timeVarID = ncvardef(fd, "Time", NC_LONG, 1, dims);
  strcpy(buffer, "time of measurement");
  ncattput(fd, timeVarID, "long_name", NC_CHAR, strlen(buffer)+1, buffer);
  strcpy(buffer, "time");
  ncattput(fd, timeVarID, "standard_name", NC_CHAR, strlen(buffer)+1, buffer);
  data_p[indx++] = (void *)&TimeVar;

  if (cfg.isADS2())
  {
    timeOffsetID = ncvardef(fd, "time_offset", NC_FLOAT, 1, dims);
    strcpy(buffer, "Seconds since base_time.");
    ncattput(fd, timeOffsetID, "long_name", NC_CHAR, strlen(buffer)+1, buffer);
    data_p[indx++] = (void *)&TimeOffset;
  }

  startVarIndx = indx;

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
      sprintf(tmp, "sps%d", rp->OutputRate);
      _rateDimIDs[rp->OutputRate] = ncdimdef(fd, tmp, rp->OutputRate);
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
        sprintf(tmp, "Vector%d", rp->Length);
        _vectorDimIDs[rp->Length] = ncdimdef(fd, tmp, rp->Length);
      }

      ndims = 3;
      dims[2] = _vectorDimIDs[rp->Length];
    }

//printf("RAW: %s\n", rp->name);
    rp->varid = ncvardef(fd, rp->name, NC_FLOAT, ndims, dims);

    addCommonVariableAttributes(rp);

    ncattput(fd, rp->varid, "SampledRate", NC_LONG, 1, &rp->SampleRate);

    if (cfg.TimeShifting() && rp->StaticLag != 0)
    {
      ncattput(fd, rp->varid, "TimeLag", NC_LONG, 1, &rp->StaticLag);
      ncattput(fd, rp->varid, "TimeLagUnits", NC_CHAR, 13, "milliseconds");
    }

    if (cfg.Despiking() && rp->SpikeSlope != 0.0)
      ncattput(fd, rp->varid, "DespikeSlope", NC_FLOAT, 1, &rp->SpikeSlope);

    ncattput(fd, rp->varid, "DataQuality", NC_CHAR, strlen(rp->DataQuality)+1,
		rp->DataQuality);

    if (rp->cof.size() > 0)
      ncattput(fd, rp->varid, "CalibrationCoefficients", NC_FLOAT,
               rp->cof.size(), &rp->cof[0]);

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

    if (rp->OutputRate == Config::LowRate)
      data_p[indx++] = (void *)&AveragedData[rp->LRstart];
    else
    if (rp->OutputRate == rp->SampleRate && rp->OutputRate != (size_t)cfg.ProcessingRate())
      data_p[indx++] = (void *)&SampledData[rp->SRstart];
    else
      data_p[indx++] = (void *)&HighRateData[rp->HRstart];
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
      sprintf(tmp, "sps%d", dp->OutputRate);
      _rateDimIDs[dp->OutputRate] = ncdimdef(fd, tmp, dp->OutputRate);
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
        sprintf(tmp, "Vector%d", dp->Length);
        _vectorDimIDs[dp->Length] = ncdimdef(fd, tmp, dp->Length);
        }

      ndims = 3;
      dims[2] = _vectorDimIDs[dp->Length];
    }

//printf("DER: %s\n", dp->name); fflush(stdout);
    dp->varid = ncvardef(fd, dp->name, NC_FLOAT, ndims, dims);

    addCommonVariableAttributes(dp);

    ncattput(fd, dp->varid, "DataQuality", NC_CHAR, strlen(dp->DataQuality)+1,
		dp->DataQuality);

    sprintf(buffer, "%d", dp->ndep);
    for (size_t j = 0; j < dp->ndep; ++j)
    {
      strcat(buffer, " ");
      strcat(buffer, dp->depend[j]);
    }

    ncattput(fd, dp->varid, "Dependencies",NC_CHAR,strlen(buffer)+1,buffer);

    if (dp->Modulo)
    {
      float mod[2];

      mod[0] = (float)dp->Modulo->value[0];
      mod[1] = (float)dp->Modulo->value[1];
      ncattput(fd, dp->varid, "modulus_range", NC_FLOAT, 2, mod);
    }

    CheckAndAddAttrs(fd, dp->varid, dp->name);

    if (dp->Length > 3 &&
	(dp->ProbeType & PROBE_PMS2D || dp->ProbeType & PROBE_PMS1D ||
	 dp->ProbeType & PROBE_RDMA || dp->ProbeType & PROBE_CLMT))
      AddPMS1dAttrs(fd, dp);

    if (dp->OutputRate == Config::LowRate)
      data_p[indx++] = (void *)&AveragedData[dp->LRstart];
    else
      data_p[indx++] = (void *)&HighRateData[dp->HRstart];
  }

  ReadMetaData(fd);

}	/* END CREATENETCDF */

/* -------------------------------------------------------------------- */
void SwitchNetCDFtoDataMode()
{
  ncendef(fd);

}	/* END SWITCHNETCDFTODATAMODE */

/* -------------------------------------------------------------------- */
void replaceNANwithMissingValue()
{
  for (size_t i = 0; i < nLRfloats; ++i)
    if (isnan(AveragedData[i]))
      AveragedData[i] = MISSING_VALUE;

  for (size_t i = 0; i < nSRfloats; ++i)
    if (isnan(SampledData[i]))
      SampledData[i] = MISSING_VALUE;

  if (HighRateData)
    for (size_t i = 0; i < nHRfloats; ++i)
      if (isnan(HighRateData[i]))
        HighRateData[i] = MISSING_VALUE;
}

/* -------------------------------------------------------------------- */
void WriteNetCDF()
{
  struct missDat	*dp;
  static int		errCnt = 0;
  static bool		firstWrite = true;

  if (firstWrite)
  {
    SetBaseTime(SampledData);
    firstWrite = false;
  }

  replaceNANwithMissingValue();

  if (ncrecput(fd, recordNumber, data_p) == ERR)
  {
    if (errno == ENOSPC)
    {
      LogMessage("Disk full, closing file.\n");
      CloseNetCDF();
      PauseFlag = true;
      PauseWhatToDo = P_QUIT;

      return;
    }

    fprintf(stderr,
            "WriteNetCDF: ncrecput failure, RecordNumber = %ld, errno = %d\n",
            recordNumber, errno);

    if (++errCnt > 8)
    {
      fprintf(stderr,
              "Too many write errors, closing file and exiting...\n");
      quit();
    }
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
void WriteNetCDF_MRF()
{
  int		indx = startVarIndx;
  RAWTBL	*rp;

  /* We need to reset SampleRate indices, because for HighRate, SampleData
   * is from a circular buffer.
   */
  for (size_t i = 0; i < raw.size(); ++i)
    if ((rp = raw[i])->Output)
    {
      // The 2nd part of this if test if for PMS1D, where highrate is still not
      // 25Hz, but rather it's own 10hz.
      if (rp->OutputRate == rp->SampleRate && rp->OutputRate != (size_t)cfg.ProcessingRate())
        data_p[indx] = (void *)&SampledData[rp->SRstart];

      ++indx;
    }

  WriteNetCDF();

}	/* END WRITENETCDF_MRF */

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
  NR_TYPE	*d, hour, minute, second;
  void		*ldp[MAX_VARIABLES];
  struct missDat	*dp;

  dp = (struct missDat *)FrontQueue(missingRecords);
  d = new float[5000];
  /* 5000 is fastest sampling rate */

  for (i = 0; i < 5000; ++i)
    d[i] = MISSING_VALUE;

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

  ncredef(fd);
  putGlobalAttribute("TimeInterval", buffer);
  ncendef(fd);

  ncsync(fd);

}	/* END SYNCNETCDF */

/* -------------------------------------------------------------------- */
void CloseNetCDF()
{
  if (fd == ERR)
    return;

  ncredef(fd);
  writeTimeUnits();
  ncendef(fd);

  SyncNetCDF();

  ncclose(fd);
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
  long	start[3], count[3];
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
    if (strcmp(derived[i]->DataQuality, "Bad") == 0)
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
      if (writeBlank(derived[i]->varid, start, count, derived[i]->OutputRate) == ERR)
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

    if ((index = SearchTable(derived, target)) != ERR &&
	derived[index]->Output)
      {
/*  See if measurement has already been blanked for whole flight  */
      if (strcmp(derived[index]->DataQuality, "Bad") == 0)
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
        if (writeBlank(derived[index]->varid, start, count, derived[index]->OutputRate) == ERR)
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
static int writeBlank(int varid, long start[], long count[], int OutputRate)
{
  long		nValues;
  NR_TYPE	*p;

  count[1] = OutputRate;
  count[2] = 1;

  nValues = count[0] * count[1] * count[2];
  p = new NR_TYPE[nValues];

  for (int i = 0; i < nValues; ++i)
    p[i] = MISSING_VALUE;

  return(ncvarput(fd, varid, start, count, (void *)p));
}

/* -------------------------------------------------------------------- */
static void writeTimeUnits()
{
  const char *format = "seconds since %F %T %z";

  strftime(buffer, 256, format, &StartFlight);
  ncattput(fd, timeVarID, "units", NC_CHAR, strlen(buffer)+1, buffer);
  ncattput(fd, timeVarID, "strptime_format", NC_CHAR, strlen(format)+1, format);
  if (cfg.isADS2())
    ncattput(fd, timeOffsetID, "units", NC_CHAR, strlen(buffer)+1, buffer);
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
markDependedByList(char target[])
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
static void addCommonVariableAttributes(var_base *var)
{
  const char *p;

  ncattput(fd, var->varid, "_FillValue", NC_FLOAT, 1, &MISSING_VALUE);
/* Once we support individual _FillValue in Q missing data routine, then use this line.
  float fv = VarDB_GetFillValue(var->name);
  ncattput(fd, var->varid, "_FillValue", NC_FLOAT, 1, &fv);
*/

  strcpy(buffer, var->Units.c_str());
  ncattput(fd, var->varid, "units", NC_CHAR, strlen(buffer)+1, buffer);

  strcpy(buffer, var->LongName.c_str());
  ncattput(fd, var->varid, "long_name", NC_CHAR, strlen(buffer)+1, buffer);

  if (fabs(VarDB_GetMinLimit(var->name)) + fabs(VarDB_GetMaxLimit(var->name)) > 0.0001)
  {
    NR_TYPE   range[2];

    range[0] = VarDB_GetMinLimit(var->name);
    range[1] = VarDB_GetMaxLimit(var->name);
    ncattput(fd, var->varid, "valid_range", NC_FLOAT, 2, range);
  }

  if (var->CategoryList.size() > 0)
  {
    char temp[32];
    strcpy(temp, var->CategoryList[0].c_str());
    if ( strcmp(temp, "None") )
      ncattput(fd, var->varid, "Category", NC_CHAR, strlen(temp)+1, temp);
  }

  p = VarDB_GetStandardNameName(var->name);
  if (p && strcmp(p, "None") != 0)
    ncattput(fd, var->varid, "standard_name", NC_CHAR, strlen(p)+1, p);

  if (var->SerialNumber.length() > 0)
    ncattput(fd, var->varid, "SerialNumber", NC_CHAR,
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

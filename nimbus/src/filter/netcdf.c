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

STATIC FNS:	writeBlank()
		writeTimeUnits()
		clearDependedByList()
		markDependedByList()
		printDependedByList()
		addCommonVariableAttributes()

DESCRIPTION:	This file has the routines necessary to Create and write
		data for distribution of NCAR/RAF aircraft data in netCDF
		format.

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2005
-------------------------------------------------------------------------
*/

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

#include "nimbus.h"
#include "decode.h"
#include "gui.h"
#include "ctape.h"
#include "netcdf.h"
#include "raf_queue.h"
#include "vardb.h"
#include "svnInfo.h"

#include <cmath>
#include <map>

static const std::string NETCDF_FORMAT_VERSION = "1.3";

#define DEFAULT_TI_LENGTH	(19 * MAX_TIME_SLICES)

struct missDat	/* (Time gap) / (missing data) information */
	{
	int		hour;
	int		minute;
	int		second;
	size_t		nRecords;
	} ;

static int		fd = -1;
static int		baseTimeID;
static struct tm	StartFlight;
static void		*data_p[MAX_VARIABLES];
static long		recordNumber = 0;
static long		TimeVar = 0;
static float		TimeOffset = 0.0;
static const int	startVarIndx = 2;

static Queue	*missingRecords;
static void	WriteMissingRecords();

extern NR_TYPE	*SampledData, *AveragedData, *HighRateData;
extern FILE	*LogFile;

int	FlightDate[3];		// HACK: for amlib
char	dateProcessed[64];	// For export to psql.c


static int	writeBlank(int varid, long start[], long count[], int OutputRate);
static void	markDependedByList(char target[]), writeTimeUnits();
static void	clearDependedByList(), printDependedByList();
static void	addCommonVariableAttributes(char name[], int varid);

void	AddPMS1dAttrs(int ncid, RAWTBL *rp),
	CheckAndAddAttrs(int fd, int varid, char name[]);

//      Rate, DimID
std::map<int, int> _rateDimIDs;
//   VectorLen, DimID
std::map<int, int> _vectorDimIDs;

/* -------------------------------------------------------------------- */
void SetBaseTime(struct Hdr_blk *hdr)
{
  time_t	BaseTime;

  StartFlight.tm_hour	= ntohs(hdr->hour);
  StartFlight.tm_min	= ntohs(hdr->minute);
  StartFlight.tm_sec	= ntohs(hdr->second);
  StartFlight.tm_isdst	= -1;

  /* Account for circular buffer spin up in [lr|hr]loop.c
   */
  if (cfg.ProcessingMode() != Config::RealTime)
    if (cfg.ProcessingRate() == Config::LowRate)
      StartFlight.tm_sec += 2;
    else
      StartFlight.tm_sec += 14;

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
void CreateNetCDF(const char fileName[])
{
  size_t	i, j;
  int		indx, catIdx;
  SDITBL	*sp;
  RAWTBL	*rp;
  DERTBL	*dp;

  int	ndims, dims[3], TimeDim;

  char	*p;


  fd = nccreate(fileName, NC_CLOBBER);

  if (cfg.ProductionRun())
    chmod(fileName, 0644);
  else
    chmod(fileName, 0666);


  /* Dimensions.
   */
  TimeDim = ncdimdef(fd, "Time", NC_UNLIMITED);
  _rateDimIDs[1] = ncdimdef(fd, "sps1", 1);


  /* Global Attributes.
   */
  strcpy(buffer, "NCAR Research Aviation Facility");
  ncattput(fd,NC_GLOBAL, "Source", NC_CHAR, strlen(buffer)+1, (void *)buffer);

  strcpy(buffer, "P.O. Box 3000, Boulder, CO 80307-3000");
  ncattput(fd,NC_GLOBAL, "Address", NC_CHAR, strlen(buffer)+1, (void *)buffer);

  strcpy(buffer, "(303) 497-1030");
  ncattput(fd,NC_GLOBAL, "Phone", NC_CHAR, strlen(buffer)+1, (void *)buffer);

  strcpy(buffer, "NCAR-RAF/nimbus");
  ncattput(fd, NC_GLOBAL, "Conventions", NC_CHAR,
           strlen(buffer)+1, (void *)buffer);

  strcpy(buffer, "http://www.eol.ucar.edu/raf/Software/netCDF.html");
  ncattput(fd, NC_GLOBAL, "ConventionsURL", NC_CHAR,
           strlen(buffer)+1, (void *)buffer);

  strcpy(buffer, NETCDF_FORMAT_VERSION.c_str());
  ncattput(fd, NC_GLOBAL, "Version", NC_CHAR, strlen(buffer)+1, (void *)buffer);

  strcpy(buffer, &SVNREVISION[10]);
  ncattput(fd, NC_GLOBAL, "ProcessorRevision", NC_CHAR, strlen(buffer)+1, (void *)buffer);

  if (strstr(SVNURL, "http"))
    strcpy(buffer, strstr(SVNURL, "http"));
  else
    strcpy(buffer, SVNURL);

  ncattput(fd, NC_GLOBAL, "ProcessorURL", NC_CHAR, strlen(buffer)+1, (void *)buffer);


  if (!cfg.ProductionRun())
    {
    strcpy(buffer, "This file contains PRELIMINARY DATA that are NOT to be used for critical analysis.");

    ncattput(fd, NC_GLOBAL, "WARNING", NC_CHAR,
		strlen(buffer)+1, (void *)buffer);
    }


  {
  time_t	t;
  struct tm	tm;

  t = time(0);
  tm = *localtime(&t);
  strftime(dateProcessed, 64, "%F %T %z", &tm);
  ncattput(fd, NC_GLOBAL, "DateProcessed", NC_CHAR,
	  strlen(dateProcessed)+1, (void *)dateProcessed);

  if (LogFile)
    fprintf(LogFile, "Processed on: %s\n", dateProcessed);
  }

  if (ProjectName)
    {
    ncattput(fd, NC_GLOBAL, "ProjectName", NC_CHAR,
    strlen(ProjectName)+1, (void *)ProjectName);
    }

  if (GetAircraft(&p) != ERR)
    ncattput(fd, NC_GLOBAL, "Aircraft", NC_CHAR, strlen(p)+1, (void *)p);

  if (GetProjectNumber(&p) != ERR)
    ncattput(fd, NC_GLOBAL, "ProjectNumber", NC_CHAR, strlen(p)+1, (void *)p);

  if (GetFlightNumber(&p) != ERR)
    ncattput(fd, NC_GLOBAL, "FlightNumber", NC_CHAR, strlen(p)+1, (void *)p);

  if (cfg.ProcessingMode() == Config::RealTime)
  {
    time_t	x = time(NULL);
    gmtime_r(&x, &StartFlight);
    StartFlight.tm_year += 1900;  /* will be subtracted off later	*/
    p = "UTC";
  }
  else
  {
    GetHeaderDate(&p);

    sscanf(p, "%d/%d/%d", &StartFlight.tm_mon, &StartFlight.tm_mday, &StartFlight.tm_year);
    strcpy(buffer, p);

    GetTimeZone(&p);
  }

  FlightDate[0] = StartFlight.tm_mon;  /* HACK: for amlib/xlate/time.c */
  FlightDate[1] = StartFlight.tm_mday;
  FlightDate[2] = StartFlight.tm_year;

  StartFlight.tm_year -= 1900;
  StartFlight.tm_mon -= 1;

  ncattput(fd, NC_GLOBAL, "FlightDate", NC_CHAR, strlen(buffer)+1, (void *)buffer);

  if (LogFile)
    fprintf(LogFile, "Flight Date: %s\n", buffer);


  strcpy(buffer, "LONC LATC GGALT Time");
  ncattput(fd, NC_GLOBAL, "coordinates", NC_CHAR, strlen(buffer)+1, (void *)buffer);

  /* Will be updated later.
   */
  memset(buffer, ' ', DEFAULT_TI_LENGTH);
  ncattput(fd, NC_GLOBAL, "TimeInterval", NC_CHAR, DEFAULT_TI_LENGTH, (void *)buffer);


  /* Write out Categories.
   */
  {
  char *list[128];

  catIdx = VarDB_GetCategoryList(list);

  buffer[0] = '\0';

  for (i = 1; i < (size_t)catIdx; ++i)	/* Skip category "None"	*/
    {
    strcat(buffer, list[i]);
    if (i != (size_t)catIdx - 1)
      strcat(buffer, ",");
    }

  ncattput(fd, NC_GLOBAL, "Categories", NC_CHAR, strlen(buffer)+1, (void *)buffer);
  }



  /* First dimension is time dimension.
   * Second is Rate in Hz.
   * Third is Vector Length.
   */
  dims[0] = TimeDim;


  /* Time Variables.
   */
  baseTimeID = ncvardef(fd, "base_time", NC_LONG, 0, 0);
  strcpy(buffer, "seconds since 1970-01-01 00:00:00 +0000");
  ncattput(fd, baseTimeID, "units", NC_CHAR, strlen(buffer)+1, buffer);
  strcpy(buffer, "Start time of data recording.");
  ncattput(fd, baseTimeID, "long_name", NC_CHAR, strlen(buffer)+1, buffer);

  timeVarID = ncvardef(fd, "Time", NC_LONG, 1, dims);
  strcpy(buffer, "time of measurement");
  ncattput(fd, timeVarID, "long_name", NC_CHAR, strlen(buffer)+1, buffer);
  strcpy(buffer, "time");
  ncattput(fd, timeVarID, "standard_name", NC_CHAR, strlen(buffer)+1, buffer);

  timeOffsetID = ncvardef(fd, "time_offset", NC_FLOAT, 1, dims);
  strcpy(buffer, "Seconds since base_time.");
  ncattput(fd, timeOffsetID, "long_name", NC_CHAR, strlen(buffer)+1, buffer);

  // Write units for both Time & time_offset.
  writeTimeUnits();


  /* SDI variables.
   */
  indx = startVarIndx;	/* Index for data_p	*/

  /* For each variable:
   *	- Set dimensions
   *	- define variable
   *	- Set attributes
   *	- Set data_pointer
   */
  for (i = 0; i < sdi.size(); ++i)
    {
    if ((sp = sdi[i])->Output == false)
      continue;

    // Check to see if dimension exists.  If not, create it.
    if (_rateDimIDs.find(sp->OutputRate) == _rateDimIDs.end())
      {
      char tmp[32];
      sprintf(tmp, "sps%d", sp->OutputRate);
      _rateDimIDs[sp->OutputRate] = ncdimdef(fd, tmp, sp->OutputRate);
      }

    if (sp->OutputRate == 1)
      ndims = 1;
    else
      {
      ndims = 2;
      dims[1] = _rateDimIDs[sp->OutputRate];
      }


    sp->varid = ncvardef(fd, sp->name, NC_FLOAT, ndims, dims);

    addCommonVariableAttributes(sp->name, sp->varid);

    ncattput(fd, sp->varid, "SampledRate", NC_LONG, 1, &sp->SampleRate);

    if (cfg.TimeShifting() && sp->StaticLag != 0)
      {
      ncattput(fd, sp->varid, "TimeLag", NC_LONG, 1, &sp->StaticLag);
      ncattput(fd, sp->varid, "TimeLagUnits", NC_CHAR, 12, "milliseconds");
      }

    if (cfg.Despiking() && sp->SpikeSlope != 0.0)
      ncattput(fd, sp->varid, "DespikeSlope", NC_FLOAT, 1, &sp->SpikeSlope);

    ncattput(fd, sp->varid, "DataQuality", NC_CHAR, strlen(sp->DataQuality)+1,
		sp->DataQuality);
    ncattput(fd, sp->varid, "CalibrationCoefficients", NC_FLOAT,
		sp->cof.size(), &sp->cof[0]);

    if (sp->Modulo)
      {
      float mod[2];

      mod[0] = (float)sp->Modulo->value[0];
      mod[1] = (float)sp->Modulo->value[1];
      ncattput(fd, sp->varid, "modulus_range", NC_FLOAT, 2, mod);
      }

    if (sp->OutputRate == Config::LowRate)
      data_p[indx++] = (void *)&AveragedData[sp->LRstart];
    else
    if (sp->OutputRate == sp->SampleRate)
      data_p[indx++] = (void *)&SampledData[sp->SRstart];
    else
      data_p[indx++] = (void *)&HighRateData[sp->HRstart];
    }



  /* Raw/"block probe" variables.
   */
  for (i = 0; i < raw.size(); ++i)
    {
    if ((rp = raw[i])->Output == false)
      continue;

    // Check to see if dimension exists.  If not, create it.
    if (_rateDimIDs.find(rp->OutputRate) == _rateDimIDs.end())
      {
      char tmp[32];
      sprintf(tmp, "sps%d", rp->OutputRate);
      _rateDimIDs[rp->OutputRate] = ncdimdef(fd, tmp, rp->OutputRate);
      }

    if (rp->OutputRate == 1)
      ndims = 1;
    else
      {
      ndims = 2;
      dims[1] = _rateDimIDs[rp->OutputRate];
      }


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


    rp->varid = ncvardef(fd, rp->name, NC_FLOAT, ndims, dims);

    addCommonVariableAttributes(rp->name, rp->varid);

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


    if (rp->Length > 3)
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
  for (i = 0; i < derived.size(); ++i)
    {
    if ((dp = derived[i])->Output == false)
      continue;

    // Check to see if dimension exists.  If not, create it.
    if (_rateDimIDs.find(dp->OutputRate) == _rateDimIDs.end())
      {
      char tmp[32];
      sprintf(tmp, "sps%d", dp->OutputRate);
      _rateDimIDs[dp->OutputRate] = ncdimdef(fd, tmp, dp->OutputRate);
      }

    if (dp->OutputRate == 1)
      ndims = 1;
    else
      {
      ndims = 2;
      dims[1] = _rateDimIDs[dp->OutputRate];
      }


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

//printf("DER:%s\n", dp->name); fflush(stdout);
    dp->varid = ncvardef(fd, dp->name, NC_FLOAT, ndims, dims);

    addCommonVariableAttributes(dp->name, dp->varid);

    ncattput(fd, dp->varid, "DataQuality", NC_CHAR, strlen(dp->DataQuality)+1,
		dp->DataQuality);

    sprintf(buffer, "%d", dp->ndep);
    for (j = 0; j < dp->ndep; ++j)
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

    if (dp->OutputRate == Config::LowRate)
      data_p[indx++] = (void *)&AveragedData[dp->LRstart];
    else
      data_p[indx++] = (void *)&HighRateData[dp->HRstart];
    }

  data_p[0] = (void *)&TimeVar;
  data_p[1] = (void *)&TimeOffset;

  ncendef(fd);

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

  if (cfg.ProcessingRate() == Config::HighRate)
    for (size_t i = 0; i < nHRfloats; ++i)
      if (isnan(HighRateData[i]))
        HighRateData[i] = MISSING_VALUE;

}

/* -------------------------------------------------------------------- */
void WriteNetCDF()
{
  struct missDat	*dp;
  static int		errCnt = 0;

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
      CloseNetCDF();
      exit(1);
      }
    }

  TimeOffset += 1.0;
  ++TimeVar;
  ++recordNumber;

  if ( (dp = (struct missDat *)FrontQueue(missingRecords)) )
    {
    int		hour, min, sec;

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
  SDITBL	*sp;
  RAWTBL	*rp;


  /* We need to reset SampleRate indices, because for HighRate, SampleData
   * is from a circular buffer.
   */
  for (size_t i = 0; i < sdi.size(); ++i)
    if ((sp = sdi[i])->Output)
      {
      if (sp->OutputRate == sp->SampleRate)
        data_p[indx] = (void *)&SampledData[sp->SRstart];

      ++indx;
      }

  for (size_t i = 0; i < raw.size(); ++i)
    if ((rp = raw[i])->Output)
      {
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
  int		indx = startVarIndx;
  NR_TYPE	*d, hour, minute, second;
  void		*ldp[MAX_VARIABLES];
  struct missDat	*dp;

  dp = (struct missDat *)FrontQueue(missingRecords);
  d = new float[2500];
  /* 1000 is fastest sampling rate */

  for (i = 0; i < 2500; ++i)
    d[i] = MISSING_VALUE;

  ldp[0] = (void *)&TimeVar;
  ldp[1] = (void *)&TimeOffset;

  for (i = 0; i < sdi.size(); ++i)
    if (sdi[i]->Output)
      ldp[indx++] = (void *)d;

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

}	/* WRITEMISSINGRECORDS */

/* -------------------------------------------------------------------- */
void SyncNetCDF()
{
// Can't write attribute in data mode.
//  FormatTimeSegmentsForOutputFile(buffer);
//  ncattput(fd, NC_GLOBAL, "TimeInterval", NC_CHAR, strlen(buffer), (void *)buffer);

  ncsync(fd);

}	/* END SYNCNETCDF */

/* -------------------------------------------------------------------- */
void CloseNetCDF()
{
  if (fd == ERR)
    return;

  int	len;

  writeTimeUnits();

  FormatTimeSegmentsForOutputFile(buffer);

  if ((len = strlen(buffer) + 1) > DEFAULT_TI_LENGTH)
    ncredef(fd);

  ncattput(fd, NC_GLOBAL, "TimeInterval", NC_CHAR, len, (void *)buffer);
  ncclose(fd);
  fd = ERR;

  LogMessage("Time interval(s) completed : ");
  LogMessage(buffer);
  LogMessage("\n");

}	/* END CLOSENETCDF */

/* -------------------------------------------------------------------- */
void BlankOutBadData()
{
  char  *blanks[512], target[NAMELEN];
  int	sTime[4], eTime[4], index;
  long	start[3], count[3];
  int	fsTime[4], feTime[4];


  /* Come through as a second pass after all processing has been done, and
   * replace "bad" segments with MISSING_VALUE.
   */
  sprintf(buffer, "%s.%s", BLANKVARS, FlightNumber);
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
  count[0] = feTime[3] - fsTime[3];

  for (size_t i = 0; i < sdi.size(); ++i)
    {
    if (strcmp(sdi[i]->DataQuality, "Bad") == 0)
      {
      sprintf(buffer, "Blanking %s from %02d:%02d:%02d to %02d:%02d:%02d.\n",
       sdi[i]->name, fsTime[0], fsTime[1], fsTime[2], feTime[0],
       feTime[1], feTime[2]);
      LogMessage(buffer);

      clearDependedByList();

      if (sdi[i]->DependedUpon)
        {
        markDependedByList(sdi[i]->name);
        printDependedByList();
        }
      if (writeBlank(sdi[i]->varid, start, count, sdi[i]->OutputRate) == ERR)
        {
        sprintf(buffer, "Failure writing BadData for variable %s.\n",
                sdi[i]->name);
        LogMessage(buffer);
        }
      }
    }

  for (size_t i = 0; i < raw.size(); ++i)
    {
    if (strcmp(raw[i]->DataQuality, "Bad") == 0)
      {
      sprintf(buffer, "Blanking %s from %02d:%02d:%02d to %02d:%02d:%02d.\n",
       raw[i]->name, fsTime[0], fsTime[1], fsTime[2], feTime[0],
       feTime[1], feTime[2]);
      LogMessage(buffer);

      if (raw[i]->DependedUpon)
        {
        markDependedByList(raw[i]->name);
        printDependedByList();
        }

      if (writeBlank(raw[i]->varid, start, count, raw[i]->OutputRate) == ERR)
        {
        sprintf(buffer, "Failure writing BadData for variable %s.\n",
                raw[i]->name);
        LogMessage(buffer);
        }
      }
    }

  for (size_t i = 0; i < derived.size(); ++i)
    {
    if (strcmp(derived[i]->DataQuality, "Bad") == 0)
      {
      sprintf(buffer, "Blanking %s from %02d:%02d:%02d to %02d:%02d:%02d.\n",
       derived[i]->name, fsTime[0], fsTime[1], fsTime[2], feTime[0],
       feTime[1], feTime[2]);
      LogMessage(buffer);

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

    if ((index = SearchTable(sdi, target)) != ERR &&
	sdi[index]->Output)
      {

/*  See if measurement has already been blanked for whole flight  */
      if (strcmp(sdi[index]->DataQuality, "Bad") == 0)
        {
        sprintf(buffer, "%s has already been blanked because the DataQuality flag is Bad.\n",sdi[index]->name);
        LogMessage(buffer);
        }
      else
        {
        if (sdi[index]->DependedUpon)
          {
          markDependedByList(sdi[index]->name);
          printDependedByList();
          }
        if (writeBlank(sdi[index]->varid, start, count, sdi[index]->OutputRate) == ERR)
          {
          sprintf(buffer, "Failure writing BadData for variable %s.\n",
		  sdi[index]->name);
          LogMessage(buffer);
          }
        }
      }
    else
    if ((index = SearchTableSansLocation(raw, target)) != ERR &&
	raw[index]->Output)
      {

/*  See if measurement has already been blanked for whole flight  */
      if (strcmp(raw[index]->DataQuality, "Bad") == 0)
        {
        sprintf(buffer, "%s has already been blanked because the DataQuality flag is Bad.\n",raw[index]->name);
        LogMessage(buffer);
        }
      else
        {
        if (raw[index]->DependedUpon)
          {
          markDependedByList(raw[index]->name);
          printDependedByList();
          }
        if (writeBlank(raw[index]->varid, start, count, raw[index]->OutputRate) == ERR)
          {
          sprintf(buffer, "Failure writing BadData for variable %s.\n",
		  raw[index]->name);
          LogMessage(buffer);
          }
        }
      }
    else
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
  strftime(buffer, 256, "seconds since %F %T +0000", &StartFlight);
  ncattput(fd, timeVarID, "units", NC_CHAR, strlen(buffer)+1, buffer);
  ncattput(fd, timeOffsetID, "units", NC_CHAR, strlen(buffer)+1, buffer);
}

/* -------------------------------------------------------------------- */
static void clearDependedByList()
{
  for (size_t i = 0; i < derived.size(); ++i)
    if (derived[i]->DependedUpon & 0xf0)
      derived[i]->DependedUpon &= 0x0f;
}

/* -------------------------------------------------------------------- */
static void markDependedByList(char target[])
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
}       /* END DOUBLECHECK */

/* -------------------------------------------------------------------- */
static void printDependedByList()
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
static void addCommonVariableAttributes(char name[], int varid)
{
  char *p;

  ncattput(fd, varid, "_FillValue", NC_FLOAT, 1, &MISSING_VALUE);
  p = VarDB_GetUnits(name);
  ncattput(fd, varid, "units", NC_CHAR, strlen(p)+1, p);
  p = VarDB_GetTitle(name);
  ncattput(fd, varid, "long_name", NC_CHAR, strlen(p)+1, p);

  if (fabs(VarDB_GetMinLimit(name)) + fabs(VarDB_GetMaxLimit(name)) > 0.0001)
  {
    NR_TYPE   range[2];

    range[0] = VarDB_GetMinLimit(name);
    range[1] = VarDB_GetMaxLimit(name);
    ncattput(fd, varid, "valid_range", NC_FLOAT, 2, range);
  }

  p = VarDB_GetCategoryName(name);
  ncattput(fd, varid, "Category", NC_CHAR, strlen(p)+1, p);

  p = VarDB_GetStandardNameName(name);
  if (p && strcmp(p, "None") != 0)
    ncattput(fd, varid, "standard_name", NC_CHAR, strlen(p)+1, p);

  ncattput(fd, varid, "missing_value", NC_FLOAT, 1, &MISSING_VALUE);

}	/* END ADDCOMMONVARIABLEATTRIBUTES */

/* END NETCDF.C */

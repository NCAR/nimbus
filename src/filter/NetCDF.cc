/*
-------------------------------------------------------------------------
OBJECT NAME:	netcdf.c

DESCRIPTION:	This file has the routines necessary to Create and write
		data for distribution of NCAR/RAF aircraft data in netCDF
		format.

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2022
-------------------------------------------------------------------------
*/

#include "NetCDF.h"

#include <sys/stat.h>

#include "decode.h"
#include "gui.h"
#include "src/hdr_api/ctape.h"
#include <netcdf.h>
#include <raf/vardb.hh>
#include "gitInfo.h"

#include "trace_variables.h"

const std::string NetCDF::Program = "NSF NCAR";
const std::string NetCDF::Institution = "National Center for Atmospheric Research";
const std::string NetCDF::Source = "airborne observations";
const std::string NetCDF::Address = "P.O. Box 3000, Boulder, CO 80307-3000";
const std::string NetCDF::Phone = "(303) 497-1030";
const std::string NetCDF::Creator_Name = "NCAR EOL - Research Aviation Facility";
const std::string NetCDF::Creator_URL = "https://www.eol.ucar.edu/who-we-are/eol-organization/research-aviation-facility-raf";
const std::string NetCDF::Creator_EMail = "raf-pm at ucar.edu";
const std::string NetCDF::Publisher_Name = "UCAR NCAR - Earth Observing Laboratory";
const std::string NetCDF::Publisher_URL = "https://www.eol.ucar.edu/data-software/eol-field-data-archive";
const std::string NetCDF::Publisher_EMail = "datahelp at eol.ucar.edu";
const std::string NetCDF::ProcessorURL = "https://github.com/NCAR/nimbus";
const std::string NetCDF::Conventions = "NCAR-RAF/nimbus-2.0,ACDD-1.3";
const std::string NetCDF::ConventionsURL = "https://www.eol.ucar.edu/raf/Software/netCDF.html";
//const std::string NetCDF::NETCDF_FORMAT_VERSION = "2.0";

const char *NetCDF::ISO8601_Z = "%FT%T %z";

const int NetCDF::DEFAULT_TI_LENGTH = 17;

extern NR_TYPE	*SampledData, *AveragedData, *HighRateData;
extern FILE	*LogFile;
extern VDBFile  *vardb;

int	FlightDate[3];		// HACK: for amlib
char	dateProcessed[64];	// For export to psql.cc


void	AddPMS1dAttrs(int ncid, const var_base * rp), ReadMetaData(int fd),
	ReadDOI(int fd);

extern "C" {
void sRefer(DERTBL *), sReferAttack(DERTBL *);
}

/* -------------------------------------------------------------------- */
NetCDF::NetCDF() :
  _ncid(0), _realTimeMode(false), _recordNumber(0), _timeVar(0),
  _timeOffset(0.0), _bnds_dimid(-1), _errCnt(0)
{
  memset(&_startFlight, 0, sizeof(_startFlight));

}

NetCDF::~NetCDF()
{
}

/* -------------------------------------------------------------------- */
long NetCDF::UTSeconds(double *record)	// Seconds since midnight
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
void NetCDF::SetBaseTime(double *record)
{
  _startFlight.tm_isdst	= -1;

  _startFlight.tm_hour = (int)record[timeIndex[0]],
  _startFlight.tm_min = (int)record[timeIndex[1]],
  _startFlight.tm_sec = (int)record[timeIndex[2]];

  _timeVar = _startFlight.tm_hour * 3600 + _startFlight.tm_min * 60 + _startFlight.tm_sec;

  if (cfg.isADS3())	// We don't support BaseTime anymore.
    return;

  time_t BaseTime = timegm(&_startFlight);
  nc_put_var1_long(_ncid, _baseTimeID, NULL, &BaseTime);

  if (BaseTime <= 0)
  {
    snprintf(buffer, 8192, "\nWARNING:  >>>>> base_time = %ld <<<<<\n\n", BaseTime);
    LogMessage(buffer);
  }

}	/* END SETBASETIME */


/* -------------------------------------------------------------------- */
void NetCDF::CreateFile(const char fileName[], size_t nRecords)
{
  int status;

  ILOG(("CreatingNetCDF, file = %s", fileName));

  // We may not use this, but I suspect we can still support real-time.nc
  if (nRecords == 0)
    _realTimeMode = true;

  ncopts = NC_VERBOSE;

  if (nc_create(fileName, NC_CLOBBER | NC_NETCDF4, &_ncid) != NC_NOERR)
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
  nc_def_dim(_ncid, "Time", NC_UNLIMITED, &TimeDim);
  nc_def_dim(_ncid, "sps1", 1, &_rateDimIDs[1]);

  /* Global Attributes.
   */
  putGlobalAttribute("program", Program);
  putGlobalAttribute("institution", Institution);
  putGlobalAttribute("Address", Address);
  putGlobalAttribute("Phone", Phone);

  putGlobalAttribute("source", Source);
  putGlobalAttribute("platform", cfg.TailNumber());
  putGlobalAttribute("platform_type", "aircraft");
  putGlobalAttribute("project", cfg.ProjectName());

  putGlobalAttribute("creator_name", Creator_Name);
  putGlobalAttribute("creator_email", Creator_EMail);
  putGlobalAttribute("creator_url", Creator_URL);
  putGlobalAttribute("creator_type", "group");
  snprintf(buffer, 128, "%s %s Team", Program.c_str(), cfg.AircraftString().c_str());
  putGlobalAttribute("creator_group", buffer);

  putGlobalAttribute("publisher_name", Publisher_Name);
  putGlobalAttribute("publisher_url", Publisher_URL);
  putGlobalAttribute("publisher_email", Publisher_EMail);
  putGlobalAttribute("publisher_type", "group");

  ReadDOI(_ncid);
  putGlobalAttribute("Conventions", Conventions);
  putGlobalAttribute("ConventionsURL", ConventionsURL);
  putGlobalAttribute("standard_name_vocabulary", "CF-1.11");

  putGlobalAttribute("ProcessorRepositoryURL", ProcessorURL);
  putGlobalAttribute("ProcessorRepositoryBranch", REPO_BRANCH);
  putGlobalAttribute("ProcessorRepositoryRevision", REPO_REVISION);
  putGlobalAttribute("ProcessorRepositoryDate", REPO_DATE);
  putGlobalAttribute("ProcessorRepositoryHASH", REPO_HASH);
  if (strlen(REPO_DIRTY) > 0)
    putGlobalAttribute("ProcessorRepositoryDirt", REPO_DIRTY);
  putGlobalAttribute("ProjectDirectoryRepoURL", cfg.ProjectRepoURL());
  putGlobalAttribute("ProjectDirectoryRevision", cfg.ProjectDirectoryRevision());
  if (cfg.ProjectDirectoryStatus().size() > 0)
    putGlobalAttribute("ProjectDirectoryDirt", cfg.ProjectDirectoryStatus());
  putGlobalAttribute("NIDASrevision", cfg.NIDASrevision());

  if (cfg.ZeroBinOffset())
    putGlobalAttribute("SizeDistributionLegacyZeroBin", "yes");
  else
    putGlobalAttribute("SizeDistributionLegacyZeroBin", "no");


  {
  time_t	t;
  struct tm	tm;

  memset(&tm, 0, sizeof(struct tm));
  t = time(0);
  tm = *localtime(&t);
  strftime(dateProcessed, 64, ISO8601_Z, &tm);
  putGlobalAttribute("date_created", dateProcessed);

  if (LogFile)
    fprintf(LogFile, "Processed on: %s\n", dateProcessed);
  }

  if (cfg.isADS2()) putGlobalAttribute("ProjectNumber", cfg.ProjectNumber());
  putGlobalAttribute("FlightNumber", cfg.FlightNumber());

  if (cfg.ProcessingMode() == Config::RealTime)
    snprintf(buffer, 8192, "%02d/%02d/%04d", _startFlight.tm_mon+1, _startFlight.tm_mday, _startFlight.tm_year+1900);
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

  if (!cfg.ProductionRun())
    putGlobalAttribute("WARNING",
	"This file contains PRELIMINARY DATA that are NOT to be used for critical analysis.");

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
    VDBFile::categories_type categories = vardb->get_categories();

    buffer[0] = '\0';
    if (categories.size() > 0)
      strcat(buffer, categories[0].c_str());
    for (size_t i = 1; i < categories.size(); ++i)
    {
      strcat(buffer, ",");
      strcat(buffer, categories[i].c_str());
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
    nc_def_var(_ncid, "base_time", NC_LONG, 0, 0, &_baseTimeID);
    strcpy(buffer, "seconds since 1970-01-01 00:00:00 +0000");
    nc_put_att_text(_ncid, _baseTimeID, "units", strlen(buffer)+1, buffer);
    strcpy(buffer, "Start time of data recording.");
    nc_put_att_text(_ncid, _baseTimeID, "long_name", strlen(buffer)+1, buffer);
  }

  nc_def_var(_ncid, "Time", NC_LONG, 1, dims, &_timeVarID);
  strcpy(buffer, "time of measurement");
  nc_put_att_text(_ncid, _timeVarID, "long_name", strlen(buffer), buffer);
  strcpy(buffer, "time");
  nc_put_att_text(_ncid, _timeVarID, "standard_name", strlen(buffer), buffer);

  if (cfg.isADS2())
  {
    nc_def_var(_ncid, "time_offset", NC_FLOAT, 1, dims, &_timeOffsetID);
    strcpy(buffer, "Seconds since base_time.");
    nc_put_att_text(_ncid, _timeOffsetID, "long_name", strlen(buffer), buffer);
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

      snprintf(msg, 128, "%s: Assertion `OutputRate > 0' failed.", rp->name);
      LogMessage(msg);
      quit();
    }

    // Check to see if dimension exists.  If not, create it.
    if (_rateDimIDs.find(rp->OutputRate) == _rateDimIDs.end())
    {
      char tmp[32];
      snprintf(tmp, 32, "sps%zu", rp->OutputRate);
      nc_def_dim(_ncid, tmp, rp->OutputRate, &_rateDimIDs[rp->OutputRate]);
    }

    dims[1] = _rateDimIDs[rp->OutputRate];
    if (rp->OutputRate == 1)
      ndims = 1;
    else
      ndims = 2;

    if (rp->Length > 1)
    {
      if (rp->SerialNumber.size() > 0)	// you should only ever come here once per serial number
      {
        createSizeDistributionCoordinateDimVars(rp);
        dims[2] = _vectorDimIDs[rp->SerialNumber];
      }
      else
      {
        char tmp[32];
        int dimID;
        snprintf(tmp, 32, "histogram%zu", rp->Length);
        nc_def_dim(_ncid, tmp, rp->Length, &dimID);
        dims[2] = dimID;
      }

      ndims = 3;
    }

//printf("RAW: %s\n", rp->name);
    nc_def_var(_ncid, rp->name, NC_FLOAT, ndims, dims, &rp->varid);

    addCommonVariableAttributes(rp);

    nc_put_att_long(_ncid, rp->varid, "SampledRate", NC_LONG, 1, (long *)&rp->SampleRate);

    int lag = rp->nidasLag;
    if (cfg.TimeShifting() && rp->StaticLag != 0)
      lag += rp->StaticLag;

    if (lag != 0)
    {
      const char *cstr = "milliseconds";
      nc_put_att_int(_ncid, rp->varid, "TimeLag", NC_INT, 1, &lag);
      nc_put_att_text(_ncid, rp->varid, "TimeLagUnits", strlen(cstr), cstr);
    }

    if (cfg.Despiking() && rp->SpikeSlope != 0.0)
      nc_put_att_float(_ncid, rp->varid, "DespikeSlope", NC_FLOAT, 1, &rp->SpikeSlope);

    nc_put_att_text(_ncid, rp->varid, "DataQuality", strlen(rp->DataQuality),
		rp->DataQuality);

    if (rp->cof.size() > 0)
    {
      std::vector<float> cof;
      for (size_t j = 0; j < rp->cof.size(); ++j)
        cof.push_back((float)rp->cof[j]);

      nc_put_att_float(_ncid, rp->varid, "CalibrationCoefficients", NC_FLOAT,
               cof.size(), &cof[0]);
    }

    if (rp->Modulo)
    {
      float mod[2];

      mod[0] = (float)rp->Modulo->value[0];
      mod[1] = (float)rp->Modulo->value[1];
      nc_put_att_float(_ncid, rp->varid, "modulus_range", NC_FLOAT, 2, mod);
    }


    if (rp->Length > 3 &&
	(rp->ProbeType & PROBE_PMS2D || rp->ProbeType & PROBE_PMS1D ||
	 rp->ProbeType & PROBE_RDMA || rp->ProbeType & PROBE_CLMT))
      AddPMS1dAttrs(_ncid, rp);
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

      snprintf(msg, 128, "%s: Assertion `OutputRate > 0' failed.", dp->name);
      LogMessage(msg);
      quit();
    }

    // Check to see if dimension exists.  If not, create it.
    if (_rateDimIDs.find(dp->OutputRate) == _rateDimIDs.end())
    {
      char tmp[32];
      snprintf(tmp, 32, "sps%zu", dp->OutputRate);
      nc_def_dim(_ncid, tmp, dp->OutputRate, &_rateDimIDs[dp->OutputRate]);
    }

    dims[1] = _rateDimIDs[dp->OutputRate];
    if (dp->OutputRate == 1)
      ndims = 1;
    else
      ndims = 2;

    if (dp->Length > 1)
    {
      // Check to see if dimension exists.  If not, create it.
      if (_vectorDimIDs.count(dp->SerialNumber) == 0)
      {
        int coord_dims[1] = { (int)dp->Length };
        nc_def_dim(_ncid, dp->SerialNumber.c_str(), dp->Length, &_vectorDimIDs[dp->SerialNumber]);
        nc_def_var(_ncid, dp->SerialNumber.c_str(), NC_FLOAT, 1, coord_dims, &dp->coord_varid);
      }

      ndims = 3;
      dims[2] = _vectorDimIDs[dp->SerialNumber];
    }

    status = nc_def_var(_ncid, dp->name, NC_FLOAT, ndims, dims, &dp->varid);
    if (status != NC_NOERR)
    {
      fprintf(stderr,"CreateNetCDF: error, derived variable %s status = %d\n", dp->name,status);
      fprintf(stderr, "%s\n", nc_strerror(status));
    }


    if (dp->compute == (void(*)(void*))sRefer ||
        dp->compute == (void(*)(void*))sReferAttack)
    {
      dp->metadata = dp->depends[0]->metadata;
    }

    addCommonVariableAttributes(dp);

    nc_put_att_text(_ncid, dp->varid, "DataQuality", strlen(dp->DataQuality),
		dp->DataQuality);

    snprintf(buffer, 8192, "%zu", dp->nDependencies);
    for (size_t j = 0; j < dp->nDependencies; ++j)
    {
      strcat(buffer, " ");
      strcat(buffer, dp->depend[j]);
    }

    nc_put_att_text(_ncid, dp->varid, "Dependencies", strlen(buffer),buffer);

    if (dp->Modulo)
    {
      float mod[2];

      mod[0] = (float)dp->Modulo->value[0];
      mod[1] = (float)dp->Modulo->value[1];
      nc_put_att_float(_ncid, dp->varid, "modulus_range", NC_FLOAT, 2, mod);
    }

    if (dp->Length > 3 &&
	(dp->ProbeType & PROBE_PMS2D || dp->ProbeType & PROBE_PMS1D ||
	 dp->ProbeType & PROBE_RDMA || dp->ProbeType & PROBE_CLMT))
      AddPMS1dAttrs(_ncid, dp);
  }

  ReadMetaData(_ncid);

int old_fill_mode;
nc_set_fill(_ncid, NC_NOFILL, &old_fill_mode); /* set nofill */

}	/* END CREATENETCDF */

/* -------------------------------------------------------------------- */
void NetCDF::createSizeDistributionCoordinateDimVars(var_base *vp)
{
  char dname[128], text[128];
  int coord_dims[2], status;

  // exit if we've already created these (2DSH & 2DSV would cause two entries into here)
  if (_vectorDimIDs.count(vp->SerialNumber))
    return;

  if (_bnds_dimid == -1)	// Create this only once.
    nc_def_dim(_ncid, "bnds", 2, &_bnds_dimid);

  strcpy(dname, vp->SerialNumber.c_str());

  // Create dimension
  nc_def_dim(_ncid, dname, vp->Length, &coord_dims[0]);
  _vectorDimIDs[vp->SerialNumber] = coord_dims[0];
  coord_dims[1] = _bnds_dimid;

  // Create coordinate variable
  status = nc_def_var(_ncid, dname, NC_FLOAT, 1, coord_dims, &vp->coord_varid);
  strcat(dname, "_bnds");
  if (status != NC_NOERR)
  {
    fprintf(stderr, "CreateNetCDF: error, coord variable %s %s status = %d\n", vp->name, vp->SerialNumber.c_str(), status);
    fprintf(stderr, "%s\n", nc_strerror(status));
  }
  else
  {
    nc_put_att_text(_ncid, vp->coord_varid, "units", 2, "um");
    snprintf(text, 128, "%s arithmetic midpoint bin size in diameter", &vp->name[1]);
    nc_put_att_text(_ncid, vp->coord_varid, "long_name", strlen(text), text);
    nc_put_att_text(_ncid, vp->coord_varid, "bounds", strlen(dname), dname);
  }

  // Create bounds variable
  status = nc_def_var(_ncid, dname, NC_FLOAT, 2, coord_dims, &vp->bounds_varid);
  if (status != NC_NOERR)
  {
    fprintf(stderr, "CreateNetCDF: error, coord variable %s %s status = %d\n", vp->name, vp->SerialNumber.c_str(), status);
    fprintf(stderr, "%s\n", nc_strerror(status));
  }
  else
  {
    nc_put_att_text(_ncid, vp->bounds_varid, "units", 2, "um");
    snprintf(text, 128, "lower and upper bounds for %s", & vp->name[1]);
    nc_put_att_text(_ncid, vp->bounds_varid, "long_name", strlen(text), text);
  }
}

/* -------------------------------------------------------------------- */
void NetCDF::SwitchToDataMode()
{
  nc_enddef(_ncid);
  nc_sync(_ncid);

}	/* END SWITCHNETCDFTODATAMODE */

/* -------------------------------------------------------------------- */
void NetCDF::WriteCoordinateVariableData()
{
  float cell_size[1024], mid_points[1024], bounds[1024][2];

  nc_enddef(_ncid);

  for (size_t i = 0; i < derived.size(); ++i)
  {
    DERTBL *dp;
    if ((dp = derived[i])->Output == false)
      continue;

    memset(cell_size, 0, sizeof(cell_size));
    memset(mid_points, 0, sizeof(mid_points));
    memset(bounds, 0, sizeof(bounds));
    if (nc_get_att_float(_ncid, dp->varid, "CellSizes", cell_size) == NC_NOERR)
    {
      size_t nCell, lend;

      nc_inq_attlen(_ncid, dp->varid, "CellSizes", &nCell);
      assert(nCell <= 1024);

      for (int j = 0; j < dp->Length; ++j)
      {
        mid_points[j] = (cell_size[j]+cell_size[j+1]) / 2;
        bounds[j][0] = cell_size[j];
        bounds[j][1] = cell_size[j+1];
      }
      nc_put_var_float(_ncid, dp->depends[0]->coord_varid, mid_points);
      nc_put_var_float(_ncid, dp->depends[0]->bounds_varid, (float *)bounds);
    }
  }

  nc_sync(_ncid);

}	/* END WRITECOORDINATEVARIABLEDATA */

/* -------------------------------------------------------------------- */
void NetCDF::WriteNetCDF()
{
  float *data;
  int status;

  static bool		firstWrite = true;

  if (firstWrite)
  {
    SetBaseTime(SampledData);
    firstWrite = false;
  }

  if ( !_missingRecords.empty() )
  {
    struct missDat *dp = _missingRecords.front();
    int hour, min, sec;

    hour = (int)SampledData[raw[SearchTable(raw, "HOUR")]->SRstart];
    min = (int)SampledData[raw[SearchTable(raw, "MINUTE")]->SRstart];
    sec = (int)SampledData[raw[SearchTable(raw, "SECOND")]->SRstart];

    if (hour == dp->hour && min == dp->minute && sec == dp->second)
      WriteMissingRecords();
  }

  size_t start[3], count[3];
  start[0] = _recordNumber; start[1] = start[2] = 0;
  count[0] = 1;

  time_t stime = SampledDataTimeToSeconds(SampledData);

  // Output Time variable as seconds since midnight (UTSeconds).
  long ut_seconds = UTSeconds(SampledData);
  nc_put_var1_long(_ncid, _timeVarID, start, &ut_seconds);
  if (cfg.isADS2())
    nc_put_var1_float(_ncid, _timeOffsetID, start, &_timeOffset);

  static TraceVariables tv;

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
      if (tv.active())
      {
        tv.trace_variable("write netcdf lowrate", rp->name, stime,
                          &(AveragedData[rp->LRstart]), N);
      }
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
      if (std::isnan(data[j]))
        data[j] = (float)MISSING_VALUE;

    status = nc_put_vara_float(_ncid, rp->varid, start, count, data);
    if (status != NC_NOERR)
    {
      fprintf(stderr,
            "WriteNetCDF: write failure, variable %s, RecordNumber = %ld, status = %d\n",
            rp->name, _recordNumber, status);
      fprintf(stderr, "%s\n", nc_strerror(status));
      _errCnt++;
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
      if (std::isnan(data[j]))
        data[j] = (float)MISSING_VALUE;

    status = nc_put_vara_float(_ncid, dp->varid, start, count, data);
    if (status != NC_NOERR)
    {
      fprintf(stderr,
            "WriteNetCDF: write failure, variable %s, RecordNumber = %ld, status = %d\n",
            dp->name, _recordNumber, status);
      fprintf(stderr, "%s\n", nc_strerror(status));
      _errCnt++;
    }
    delete [] data;
  }

  if (_errCnt > 10)
  {
    fprintf(stderr, "Too many write errors, closing file and exiting...\n");
    quit();
  }

  _timeOffset += 1.0;
  ++_timeVar;
  ++_recordNumber;

}	/* END WRITENETCDF */

/* -------------------------------------------------------------------- */
void NetCDF::QueueMissingData(int h, int m, int s, int nRecords)
{
  struct missDat *dp;

  dp = new struct missDat;

  dp->hour = h;
  dp->minute = m;
  dp->second = s;
  dp->nRecords = nRecords;

  _missingRecords.push(dp);

  if (cfg.ProcessingMode() == Config::RealTime)
    WriteMissingRecords();

}	/* END QUEUEMISSINGDATA */

/* -------------------------------------------------------------------- */
void NetCDF::WriteMissingRecords()
{
  size_t	i;
  float		hour, minute, second;
  // 5000 is fastest sampling rate
  std::vector<float> d(5000, MISSING_VALUE);
  void		*ldp[MAX_VARIABLES];
  struct missDat	*dp;

  dp = _missingRecords.front();

  int indx = 0;
  ldp[indx++] = (void *)&_timeVar;
  if (cfg.isADS2())
    ldp[indx++] = (void *)&_timeOffset;

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
      ldp[indx++] = (void *)&d[0];
    }

  if (second < 0)
  {
    ELOG(("Somehow second for missing record not set. "
          "Time from queue is %02d:%02d:%02d, initialized time "
          "is %02d:%02d:%02d.  Raw table size=%d, derived size=%d, "
          "num missing records=%d",
          int(dp->hour), int(dp->minute), int(dp->second),
          int(hour), int(minute), int(second),
          int(raw.size()), int(derived.size()), int(dp->nRecords)));
    second = dp->second;
  }

  for (i = 0; i < derived.size(); ++i)
    if (derived[i]->Output)
      ldp[indx++] = (void *)&d[0];

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

    ncrecput(_ncid, _recordNumber, ldp);

    _timeOffset += 1.0;
    ++_timeVar;
    ++_recordNumber;
    }

  _missingRecords.pop();
  delete dp;

}	/* END WRITEMISSINGRECORDS */

/* -------------------------------------------------------------------- */
void NetCDF::Sync()
{
  FormatTimeSegmentsForOutputFile(buffer);

  nc_redef(_ncid);
  putGlobalAttribute("TimeInterval", buffer);
  nc_enddef(_ncid);

  nc_sync(_ncid);

}	/* END SYNCNETCDF */

/* -------------------------------------------------------------------- */
void NetCDF::Close()
{
  if (_ncid == ERR)
    return;

  nc_redef(_ncid);
  writeMinMax();
  writeTimeUnits();
  nc_enddef(_ncid);

  Sync();

  nc_close(_ncid);
  _ncid = ERR;

  LogMessage("Time interval(s) completed : ");
  LogMessage(buffer);
  LogMessage("\n");

}	/* END CLOSENETCDF */

/* -------------------------------------------------------------------- */
void NetCDF::writeMinMax()
{
  float range[2];

  for (size_t i = 0; i < raw.size(); ++i)
  {
    RAWTBL * rp = raw[i];
    if (!rp->Output)
      continue;

    range[0] = rp->min;
    range[1] = rp->max;
    nc_put_att_float(_ncid, rp->varid, "actual_range", NC_FLOAT, 2, range);
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
    nc_put_att_float(_ncid, dp->varid, "actual_range", NC_FLOAT, 2, range);
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
void NetCDF::writeTimeUnits()
{
  const char *format = "seconds since %F %T %z";
  struct tm tmp, EndFlight;

  memset(&tmp, 0, sizeof(struct tm));
  memset(&EndFlight, 0, sizeof(struct tm));

  _startFlight.tm_isdst = 0;
  tmp = _startFlight;
printf("%s\n", asctime(&_startFlight));
  tmp.tm_hour = tmp.tm_min = tmp.tm_sec = 0;
  strftime(buffer, 256, format, &tmp);
  nc_put_att_text(_ncid, _timeVarID, "units", strlen(buffer), buffer);
  nc_put_att_text(_ncid, _timeVarID, "strptime_format", strlen(format), format);
  if (cfg.isADS2())
    nc_put_att_text(_ncid, _timeOffsetID, "units", strlen(buffer), buffer);


  strftime(buffer, 256, ISO8601_Z, &_startFlight);
  putGlobalAttribute("time_coverage_start", buffer);

  time_t endTime = timegm(&tmp);
  endTime += (_timeVar - 1);
  gmtime_r(&endTime, &EndFlight);
  EndFlight.tm_isdst = 0;
  strftime(buffer, 256, ISO8601_Z, &EndFlight);
  putGlobalAttribute("time_coverage_end", buffer);
}

/* -------------------------------------------------------------------- */
void NetCDF::addCommonVariableAttributes(const var_base *var)
{
  VDBVar *vdb_var = vardb->search_var(var->name);

  addVariableMetadata(var);

  if (vdb_var)
  {
    std::string std_name = vdb_var->get_attribute(VDBVar::STANDARD_NAME);
    if (std_name.size() > 0)
      nc_put_att_text(_ncid, var->varid, "standard_name", std_name.size(), std_name.c_str());

    if (vdb_var->has_attribute(VDBVar::MIN_LIMIT) &&
	vdb_var->has_attribute(VDBVar::MAX_LIMIT))
    {
      float   range[2];
      range[0] = vdb_var->get_attribute_value<float>(VDBVar::MIN_LIMIT);
      range[1] = vdb_var->get_attribute_value<float>(VDBVar::MAX_LIMIT);
      nc_put_att_float(_ncid, var->varid, "valid_range", NC_FLOAT, 2, range);
    }
  }

  float zero[2] = { 0.0, 0.0 };
  nc_put_att_float(_ncid, var->varid, "actual_range", NC_FLOAT, 2, zero);

  if (var->CategoryList.size() > 0)
  {
    char temp[32];
    strcpy(temp, var->CategoryList[0].c_str());
    if ( strcmp(temp, "None") )
      nc_put_att_text(_ncid, var->varid, "Category", strlen(temp), temp);
  }

  if (var->SerialNumber.length() > 0)
    nc_put_att_text(_ncid, var->varid, "SerialNumber",
	var->SerialNumber.length(), var->SerialNumber.c_str());

}	/* END ADDCOMMONVARIABLEATTRIBUTES */

/* -------------------------------------------------------------------- */
void NetCDF::addVariableMetadata(const var_base *var)
{
  for (size_t i = 0; i < var->metadata.size(); ++i)
  {
    const Metadata *mdp = &var->metadata[i];
    if (mdp->isFloat())
      nc_put_att_float(_ncid, var->varid, mdp->_attr_name.c_str(), NC_FLOAT,
                mdp->_attr_flt.size(), &mdp->_attr_flt[0]);
    else
    if (mdp->isInt())
      nc_put_att_int(_ncid, var->varid, mdp->_attr_name.c_str(), NC_INT,
                mdp->_attr_int.size(), &mdp->_attr_int[0]);
    else
      nc_put_att_text(_ncid, var->varid, mdp->_attr_name.c_str(),
                mdp->_attr_str.length(), mdp->_attr_str.c_str());
  }
}       /* END ADDVARIABLEMETADATA */

/* -------------------------------------------------------------------- */
void NetCDF::ProcessFlightDate()
{
  if (cfg.ProcessingMode() == Config::RealTime)
  {
    time_t      x = time(NULL);
    gmtime_r(&x, &_startFlight);
    _startFlight.tm_mon += 1;
    _startFlight.tm_year += 1900;  /* will be subtracted off later       */
  }
  else
  {
    strcpy(buffer, cfg.FlightDate().c_str());
    if (cfg.FlightDate().size() > 0)
      sscanf(buffer, "%d/%d/%d", &_startFlight.tm_mon, &_startFlight.tm_mday, &_startFlight.tm_year);
  }

  FlightDate[0] = _startFlight.tm_mon;  /* HACK: for amlib/xlate/time.c */
  FlightDate[1] = _startFlight.tm_mday;
  FlightDate[2] = _startFlight.tm_year;

  _startFlight.tm_year -= 1900;
  _startFlight.tm_mon -= 1;
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
void NetCDF::addLandmarks()
{
  putGlobalAttribute("landmarks", readLandmarks());
}

/* -------------------------------------------------------------------- */
void NetCDF::putGlobalAttribute(const char attrName[], const float *value)
{
  nc_put_att_float(_ncid, NC_GLOBAL, attrName, NC_FLOAT, 1, value);
}

void NetCDF::putGlobalAttribute(const char attrName[], const char *value)
{
  nc_put_att_text(_ncid, NC_GLOBAL, attrName, strlen(value), value);
}

void NetCDF::putGlobalAttribute(const char attrName[], const std::string value)
{
  putGlobalAttribute(attrName, value.c_str());
}

/* END NETCDF.C */

/*
-------------------------------------------------------------------------
OBJECT NAME:	pms1d.c

FULL NAME:	PMS-1D related stuff

ENTRY POINTS:	AddPMS1dAttrs()
		AddToPMS1DprobeList()
		GetPMS1DAttrsForSQL()

STATIC FNS:	getCellSizes()

DESCRIPTION:	AddPMS1dAttrs() adds PMSspecs probe parameters to the
		netCDF file variable attributes.  The other function is
		stub for post-processing.

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-2006
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include <raf/pms.h>
#include <netcdf.h>

static int getCellSizes(const var_base * rp, float cellSizes[]);


/* -------------------------------------------------------------------- */
static void setSerialNumberAndProbeType(const char * name, const char * serialNum, int probeType)
{
  int raw_indx, der_indx;
  char tmp[64], * location;

  strcpy(tmp, name); tmp[0] = 'C';
  if (strchr(name, '_'))	// If this is a specific probe & location.
  {
    raw_indx = SearchTable(raw, name);
    der_indx = SearchTable(derived, tmp);
  }
  else
  {
    raw_indx = SearchTableSansLocation(raw, name);
    der_indx = SearchTableSansLocation(derived, tmp);
  }

  if (raw_indx == ERR)
    return;

  raw[raw_indx]->SerialNumber	= serialNum;
  raw[raw_indx]->ProbeType	= probeType;

  if ((location = strstr(raw[raw_indx]->name, "_")) == 0)
    printf("No location found for %s\n", raw[raw_indx]->name);

  for (size_t i = 0; i < raw.size(); ++i)
    if (strstr(raw[i]->name, location))
    {
      raw[i]->SerialNumber	= raw[raw_indx]->SerialNumber;
      raw[i]->ProbeType	= raw[raw_indx]->ProbeType;
    }
  for (size_t i = 0; i < derived.size(); ++i)
    if (strstr(derived[i]->name, location))
    {
      derived[i]->SerialNumber	= raw[raw_indx]->SerialNumber;
      derived[i]->ProbeType	= raw[raw_indx]->ProbeType;
      derived[i]->Default_HR_OR	= raw[raw_indx]->SampleRate;
	// This is needed by amlib initializers, since they are called
	// via derived variables, not raw.
      derived[i]->SampleRate	= raw[raw_indx]->SampleRate;
    }

  if (der_indx != ERR)
  {
    derived[der_indx]->Length	= raw[raw_indx]->Length;
  }
  else
    printf("Debug: No concentration %s found for %s.\n", tmp, name);


  if (strstr(name, "1D") || strstr(name, "2D"))	// 2D data, we need probeCount setup.
  {
    char target[12];
    strcpy(target, &name[1]);

    for (size_t i = 0; i < raw.size(); ++i)
      if (strstr(raw[i]->name, target))
      {
        raw[i]->SerialNumber = serialNum;
        raw[i]->ProbeType = probeType;
      }

    for (size_t i = 0; i < derived.size(); ++i)
      if (strstr(derived[i]->name, target))
      {
        derived[i]->SerialNumber = serialNum;
        derived[i]->ProbeType = probeType;
      }
  }
}


// Temporary hack, until I finish consolidating suport files into VarDB.ncml.
void PMS1D_SetupForADS3()
{

  setSerialNumberAndProbeType("AS100", "FSSP109", PROBE_PMS1D | PROBE_FSSP);
  setSerialNumberAndProbeType("A260X", "260X06", PROBE_PMS1D | PROBE_260X);
  setSerialNumberAndProbeType("ACDP", "CDP016", PROBE_PMS1D | PROBE_CDP);
  setSerialNumberAndProbeType("AS200", "PCAS108", PROBE_PMS1D | PROBE_PCASP);
  setSerialNumberAndProbeType("AS300", "FSSP305", PROBE_PMS1D | PROBE_F300);
  setSerialNumberAndProbeType("AUHSAS", "UHSAS001", PROBE_PMS1D | PROBE_PCASP);

  if (cfg.ProjectName().compare("PASE") == 0)
    setSerialNumberAndProbeType("AS100", "FSSP122", PROBE_PMS1D | PROBE_FSSP);
  else
  if (cfg.ProjectName().compare("VOCALS") == 0)
  {
    setSerialNumberAndProbeType("A1DC_RPC", "F2DC001", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC_RPC", "F2DC001", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A1DC_RPI", "F2DC002", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC_RPI", "F2DC002", PROBE_PMS2D | PROBE_2DC);
    if (cfg.FlightNumber().compare("rf11") >= 0)
      setSerialNumberAndProbeType("AS100", "FSSP122", PROBE_PMS1D | PROBE_FSSP);
//    setSerialNumberAndProbeType("AUHSAS", "UHSAS002", PROBE_PMS1D | PROBE_PCASP);
  }
  else
  if (cfg.ProjectName().compare("ADELE/SPRITES") == 0)
  {
    setSerialNumberAndProbeType("A1DC_RIO", "F2DC001", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC_RIO", "F2DC001", PROBE_PMS2D | PROBE_2DC);
  }
  else
  if (cfg.ProjectName().compare("HIPPO-2") == 0)
  {
    setSerialNumberAndProbeType("A1DC", "F2DC003", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC", "F2DC003", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("ACDP", "CDP058", PROBE_PMS1D | PROBE_CDP);
  }
  else
  if (cfg.ProjectName().compare("PLOWS") == 0)
  {
    setSerialNumberAndProbeType("A1DC_RPC", "F2DC001", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC_RPC", "F2DC001", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A1DC_LPC", "F2DC002", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC_LPC", "F2DC002", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("AS100_LWI", "FSSP109", PROBE_PMS1D | PROBE_FSSP);
    setSerialNumberAndProbeType("AS100_LWO", "FSSP122", PROBE_PMS1D | PROBE_FSSP);
  }
  else
  if (cfg.ProjectName().compare("HIPPO-3") == 0)
  {
    setSerialNumberAndProbeType("A1DC", "F2DC003", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC", "F2DC003", PROBE_PMS2D | PROBE_2DC);
  }
  else
  if (cfg.ProjectName().compare("PREDICT") == 0)
  {
    setSerialNumberAndProbeType("A1DC_LMI", "F2DC003", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC_LMI", "F2DC003", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("ACDP", "CDP058", PROBE_PMS1D | PROBE_CDP);
    setSerialNumberAndProbeType("AS200", "OPC001", PROBE_PMS1D | PROBE_PCASP);
    setSerialNumberAndProbeType("AUHSAS_LMO", "UHSAS001", PROBE_PMS1D | PROBE_PCASP);
    setSerialNumberAndProbeType("AUHSAS_CVI", "UHSAS002", PROBE_PMS1D | PROBE_PCASP);
  }
  else
  if (cfg.ProjectName().compare("WAMO") == 0)
  {
    setSerialNumberAndProbeType("ACDP", "CDP058", PROBE_PMS1D | PROBE_CDP);
  }
  else
  {
    setSerialNumberAndProbeType("A1DC", "F2DC001", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC", "F2DC001", PROBE_PMS2D | PROBE_2DC);
  }
  setSerialNumberAndProbeType("A1DP", "2DP10", PROBE_PMS2D | PROBE_2DP);
  setSerialNumberAndProbeType("A2DP", "2DP10", PROBE_PMS2D | PROBE_2DP);
}

/* -------------------------------------------------------------------- */
void GetPMS1DAttrsForSQL(RAWTBL *rp, char sql_buff[])
/* Add PMSspecs to netCDF attributes for probe, they go with accumulations */
{
  int	i, fb, lb, nBins;
  char	*p, temp[32];
  float	cellSize[300];

  /* If not PMS1D or PMS2D, then bail out. */
  if ((rp->ProbeType & 0xff000000) != 0x80000000 &&
      (rp->ProbeType & 0xff000000) != 0x10000000)
  {
    fprintf(stderr, "%s not a PMS1D probe?\n", rp->name);
    strcpy(sql_buff, ", 0, 0, \'{}\'");
    return;
  }

  MakeProjectFileName(buffer, PMS_SPEC_FILE);
  InitPMSspecs(buffer);

  fb = 0;
  lb = rp->Length-1;

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "FIRST_BIN")) )
  {
    fb = atoi(p);

    /* We are dropping the unused 0th bin for SQL database.
     * See 12 lines down and also psql.cc PostgreSQL::addVectorToAllStreams().
     * Remove when we FIRST_BIN/LAST_BIN no longer compensate for the legacy 0th bin.
     */
    --fb;
  }

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "LAST_BIN")) )
  {
    lb = atoi(p) - 1;

    if (strstr(rp->name, "2D"))	/* 2D's use 63 bins, instead of 1DC */
      lb += (rp->Length >> 1);

    /* We are dropping the unused 0th bin for SQL database.
     * See also psql.cc PostgreSQL::addVectorToAllStreams().
     * Remove when we FIRST_BIN/LAST_BIN no longer compensate for the legacy 0th bin.
     */
    --lb;
  }

  nBins = getCellSizes(rp, cellSize);
  sprintf(sql_buff, ", %d, %d, '{", fb, lb);

  sprintf(temp, "%g", cellSize[0]);
  strcat(sql_buff, temp);

  for (i = 1; i < nBins; ++i)
  {
    sprintf(temp, ",%g", cellSize[i]);
    strcat(sql_buff, temp);
  }

  strcat(sql_buff, "}'");

  ReleasePMSspecs();

}	/* GETPMS1DATTRSFORSQL */

/* -------------------------------------------------------------------- */
/* Add PMSspecs data and other meta-data to the netCDF file, attributes
 * for probe.
 */
void AddPMS1dAttrs(int ncid, const var_base * varp)
{
  int	nBins, cvarid;
  char	*p;
  float	cellSize[300];
  bool	warnMidPoints = false;

  /* If not PMS1D, then bail out. */
  if ((varp->ProbeType & 0xff000000) != 0x80000000 && (varp->ProbeType & 0xff000000) != 0x10000000)
    return;

  cvarid = varp->varid;

  MakeProjectFileName(buffer, PMS_SPEC_FILE);
  InitPMSspecs(buffer);

  // Really wish I could up dymamic_cast.  What I really want to check:
  // if (varp == <RAWTBL*>)
  if (varp->name[0] == 'A')
  {
    if (varp->ProbeType & PROBE_260X || varp->ProbeType & PROBE_200X ||
        varp->ProbeType & PROBE_200Y || varp->ProbeType & PROBE_PMS2D)
    {
      if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "RANGE_STEP")) ) {
        int value = atoi(p);
        ncattput(ncid, cvarid, "Resolution", NC_INT, 1, &value);
      }

      if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "NDIODES")) ) {
        int value = atoi(p);
        ncattput(ncid, cvarid, "nDiodes", NC_INT, 1, &value);
      }

      if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "MAG")) ) {
        float value = atof(p);
        ncattput(ncid, cvarid, "Magnification", NC_FLOAT, 1, &value);
      }

      if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "RESPONSE_TIME")) ) {
        float value = atof(p);
        ncattput(ncid, cvarid, "ResponseTime", NC_FLOAT, 1, &value);
      }

      if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "ARM_DISTANCE")) ) {
        float value = atof(p);
        ncattput(ncid, cvarid, "ArmDistance", NC_FLOAT, 1, &value);
      }

    ncattput(ncid, cvarid, "HistogramNote", NC_CHAR, 48, "Zeroth data bin is an unused legacy placeholder.");
    }

    if (varp->ProbeType & PROBE_PMS2D)
    {
      if (varp->Length < 64)
      {
        strcpy(buffer, "2D buffers with more than 8 seconds elapsed time or fewer than 20 particles.");
        ncattput(ncid, cvarid, "Rejected", NC_CHAR, strlen(buffer), buffer);
      }

      if (strstr(varp->name, "1D"))
      {
        const char * s = "Entire In";
        ncattput(ncid, cvarid, "ParticleAcceptMethod", NC_CHAR, strlen(s), s);
      }

      if (strstr(varp->name, "2D"))
      {
        const char * s = "Error";
        float f;

        if (cfg.TwoDProcessingMethod() == Config::Center_In)
          s = "Center In";
        if (cfg.TwoDProcessingMethod() == Config::Reconstruction)
          s = "Reconstruction";
        ncattput(ncid, cvarid, "ParticleAcceptMethod", NC_CHAR, strlen(s), s);

        f = cfg.TwoDAreaRejectRatio();
        ncattput(ncid, cvarid, "ParticleAreaRejectionRatio", NC_FLOAT, 1, &f);
      }
    }
  }
  else
  {
    if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "FIRST_BIN")) ) {
      int	value = atoi(p);
      ncattput(ncid, cvarid, "FirstBin", NC_INT, 1, &value);
    }

    if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "LAST_BIN")) ) {
      int	value = atoi(p) - 1;	/* Go from exclusive to inclusive */

      if (strstr(varp->name, "2D"))	/* 2D's use 63 bins, instead of 1DC */
        value += (varp->Length >> 1);

      ncattput(ncid, cvarid, "LastBin", NC_INT, 1, &value);
    }

    nBins = getCellSizes(varp, cellSize);
    ncattput(ncid, cvarid, "CellSizes", NC_FLOAT, nBins, cellSize);
    ncattput(ncid, cvarid, "CellSizeUnits", NC_CHAR, 11, "micrometers");
    ncattput(ncid, cvarid, "CellSizeNote", NC_CHAR, 25, "CellSizes are end points.");
    ncattput(ncid, cvarid, "HistogramNote", NC_CHAR, 48, "Zeroth data bin is an unused legacy placeholder.");

  if (cellSize[0] == 0.0)
    warnMidPoints = true;

    if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "DOF")) ) {
      float value = atof(p);
      ncattput(ncid, cvarid, "DepthOfField", NC_FLOAT, 1, &value);
    }

    if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "BEAM_DIAM")) ) {
      float value = atof(p);
      ncattput(ncid, cvarid, "BeamDiameter", NC_FLOAT, 1, &value);
    }

    if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "DENS")) ) {
      float value = atof(p);
      ncattput(ncid, cvarid, "Density", NC_FLOAT, 1, &value);
    }

    if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "PLWFAC")) ) {
      float value = atof(p);
      ncattput(ncid, cvarid, "PLWfactor", NC_FLOAT, 1, &value);
    }

    if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "DBZFAC")) ) {
      float value = atof(p);
      ncattput(ncid, cvarid, "DBZfactor", NC_FLOAT, 1, &value);
    }

    if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "SAMPLE_AREA")) ) {
      float value = atof(p);
      ncattput(ncid, cvarid, "SampleArea", NC_FLOAT, 1, &value);
    }
  }


  ReleasePMSspecs();


  /* Older projects have a PMSspecs file with mid-points instead of end-points.
   * Warn the user about this, as it will produce incorrect PMS1D results.
   */
  if (warnMidPoints)
  {
    fprintf(stderr, "PMSspecs file contains mid-point, not end-point cell diameters for %s %s.\n",
	varp->name, varp->SerialNumber.c_str());
    fprintf(stderr, "Nimbus was modified on 9/5/98 to use end-points.  Please fix.\n");
    quit();
  }
}

/* -------------------------------------------------------------------- */
static int getCellSizes(const var_base * rp, float cellSize[])
{
  int	i, nBins;
  char	*p;

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "CELL_SIZE")) == NULL)
  {
    /* ADS2 SPP probes mimiced old PMS1D interface and padded a useless
     * 0th bin (so 31 bins instead of 30).  ADS3 will not do this.  PMSspecs
     * files should now have FirstBin of 0 instead of 1.  Re: -1 vs. -0 below.
     */
// Note: ADS3 is propogating the problem until netCDF file refactor.
// Then we should remove all evidence of this 0th bin from ADS2 & ADS3.
    sprintf(buffer, "CELL_SIZE_%d", rp->Length - 1);
    p = GetPMSparameter(rp->SerialNumber.c_str(), buffer);
  }

  if (p)
  {
    strcpy(buffer, p);
    p = strtok(buffer, " \t,");
 
    for (i = 0; p && i < 164; ++i)
    {
      cellSize[i] = atof(p);
      p = strtok(NULL, " \t,");
      if (rp->SerialNumber.compare(0, 4, "RDMA") == 0)
        cellSize[i] /= 1000.0;
    }

    return(i);
  }


  if (rp->ProbeType & PROBE_260X || strstr(rp->name, "2D"))
    nBins = 64;
  else
  if (rp->ProbeType & PROBE_HVPS)
    nBins = 256;
  else
  if (rp->ProbeType & PROBE_PMS2D)
    nBins = 32;
  else
    nBins = 16;

  // Double the nBins for the Fast2DC.
  if (rp->SerialNumber.find("F2D", 0) != std::string::npos)
    nBins <<= 1;

  float	min = 0.0, max = 0.0, step = 0.0;

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "MIN_RANGE")) )
    min = atof(p);

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "MAX_RANGE")) )
    max = atof(p);

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "RANGE_STEP")) )
    step = atof(p);

  for (i = 0; i < nBins; min += step)
    cellSize[i++] = min;

  return(nBins);

}	/* GETCELLSIZES */

/* -------------------------------------------------------------------- */
void setProbeCount(const char * loc, int probeNum)
{
  for (size_t i = 0; i < raw.size(); ++i)
    if (strstr(raw[i]->name, loc))
      raw[i]->ProbeCount = probeNum;

  for (size_t i = 0; i < derived.size(); ++i)
    if (strstr(derived[i]->name, loc))
      derived[i]->ProbeCount = probeNum;
}

/* -------------------------------------------------------------------- */
void AddToPMS1DprobeList(
	const char    probe[],
	const char    location[],
	const char    serial_num[],
	int     type)
{

/* This stub is called by nimbus.  winput/winds uses the version in pms1d_rt.c
 */


}   /* END ADDTOPMS1DPROBELIST */

/* PMS1D.C */

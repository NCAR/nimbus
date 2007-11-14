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

static int getCellSizes(var_base * rp, float cellSizes[]);
static int setProbeCount(const char * loc, int probeNum);

// Temporary hack, until I finish consolidating suport files into VarDB.ncml.
void PMS1D_SetupForADS3()
{
  int raw_indx, der_indx;
  char * location;

  if ((raw_indx = SearchTableSansLocation(raw, "AS100")) != ERR)
  {
    location = strrchr(raw[raw_indx]->name, '_');

    if (cfg.ProjectName() == "PASE")
      raw[raw_indx]->SerialNumber	= "FSSP122";
    else
      raw[raw_indx]->SerialNumber	= "FSSP109";
//    raw[raw_indx]->Average	= (void (*) (...))SumVector;
    raw[raw_indx]->ProbeType	= PROBE_PMS1D | PROBE_FSSP;
  }
  if ((der_indx = SearchTableSansLocation(derived, "CS100")) != ERR)
  {
    derived[der_indx]->SerialNumber	= raw[raw_indx]->SerialNumber;
    derived[der_indx]->Length		= raw[raw_indx]->Length;
    derived[der_indx]->ProbeType	= raw[raw_indx]->ProbeType;
  }
  if (raw_indx != ERR && der_indx == ERR)
    printf("Debug: No CS100 found.\n");

  if ((raw_indx = SearchTableSansLocation(raw, "A260X")) != ERR)
  {
    location = strrchr(raw[raw_indx]->name, '_');

    raw[raw_indx]->SerialNumber	= "260X06";
//    raw[raw_indx]->Average	= (void (*) (...))SumVector;
    raw[raw_indx]->ProbeType	= PROBE_PMS1D | PROBE_260X;
  }
  if ((der_indx = SearchTableSansLocation(derived, "C260X")) != ERR)
  {
    derived[der_indx]->SerialNumber	= raw[raw_indx]->SerialNumber;
    derived[der_indx]->Length		= raw[raw_indx]->Length;
    derived[der_indx]->ProbeType	= raw[raw_indx]->ProbeType;
  }
  if (raw_indx != ERR && der_indx == ERR)
    printf("Debug: No C260X found.\n");


  if ((raw_indx = SearchTableSansLocation(raw, "ACDP")) != ERR)
  {
    location = strrchr(raw[raw_indx]->name, '_');

    raw[raw_indx]->SerialNumber	= "CDP001";
//    raw[raw_indx]->Average	= (void (*) (...))SumVector;
    raw[raw_indx]->ProbeType	= PROBE_PMS1D | PROBE_CDP;
  }
  if ((der_indx = SearchTableSansLocation(derived, "CCDP")) != ERR)
  {
    derived[der_indx]->SerialNumber	= raw[raw_indx]->SerialNumber;
    derived[der_indx]->Length		= raw[raw_indx]->Length;
    derived[der_indx]->ProbeType	= raw[raw_indx]->ProbeType;
  }
  if (raw_indx != ERR && der_indx == ERR)
    printf("Debug: No CCDP found.\n");

  if ((raw_indx = SearchTableSansLocation(raw, "AS200")) != ERR)
  {
    location = strrchr(raw[raw_indx]->name, '_');

    raw[raw_indx]->SerialNumber	= "PCAS108";
//    raw[raw_indx]->Average	= (void (*) (...))SumVector;
    raw[raw_indx]->ProbeType	= PROBE_PMS1D | PROBE_PCASP;
  }
  if ((der_indx = SearchTableSansLocation(derived, "CS200")) != ERR)
  {
    derived[der_indx]->SerialNumber	= raw[raw_indx]->SerialNumber;
    derived[der_indx]->Length		= raw[raw_indx]->Length;
    derived[der_indx]->ProbeType	= raw[raw_indx]->ProbeType;
  }
  if (raw_indx != ERR && der_indx == ERR)
    printf("Debug: No CS200 found.\n");


  if ((raw_indx = SearchTableSansLocation(raw, "AS300")) != ERR)
  {
    location = strrchr(raw[raw_indx]->name, '_');

    raw[raw_indx]->SerialNumber	= "FSSP305";
//    raw[raw_indx]->Average	= (void (*) (...))SumVector;
    raw[raw_indx]->ProbeType	= PROBE_PMS1D | PROBE_F300;
  }
  if ((der_indx = SearchTableSansLocation(derived, "CS300")) != ERR)
  {
    derived[der_indx]->SerialNumber	= raw[raw_indx]->SerialNumber;
    derived[der_indx]->Length		= raw[raw_indx]->Length;
    derived[der_indx]->ProbeType	= raw[raw_indx]->ProbeType;
  }
  if (raw_indx != ERR && der_indx == ERR)
    printf("Debug: No CS300 found.\n");


  if ((raw_indx = SearchTableSansLocation(raw, "AUHSAS")) != ERR)
  {
    location = strrchr(raw[raw_indx]->name, '_');

    raw[raw_indx]->SerialNumber	= "UHSAS001";
//    raw[raw_indx]->Average	= (void (*) (...))SumVector;
    raw[raw_indx]->ProbeType	= PROBE_PMS1D | PROBE_PCASP;
  }
  if ((der_indx = SearchTableSansLocation(derived, "CUHSAS")) != ERR)
  {
    derived[der_indx]->SerialNumber	= raw[raw_indx]->SerialNumber;
    derived[der_indx]->Length		= raw[raw_indx]->Length;
    derived[der_indx]->ProbeType	= raw[raw_indx]->ProbeType;
  }
  if (raw_indx != ERR && der_indx == ERR)
    printf("Debug: No CUHSAS found.\n");

}

/* -------------------------------------------------------------------- */
void GetPMS1DAttrsForSQL(RAWTBL *rp, char sql_buff[])
/* Add PMSspecs to netCDF attributes for probe, they go with accumulations */
{
  int	i, fb, lb, nBins;
  char	*p, temp[32];
  float	cellSize[300];

  /* If not PMS1D, then bail out. */
  if (rp->ProbeType & 0xff000000 != 0x80000000)
    return;

  MakeProjectFileName(buffer, PMS_SPEC_FILE);
  InitPMSspecs(buffer);

  fb = 0;
  lb = rp->Length-1;

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "FIRST_BIN")) )
  {
    fb = atoi(p);

    /* We are dropping the unused 0th bin for SQL database.
     * See 12 lines down and also PostgreSQL::addVectorToAllStreams().
     */
    if (cfg.isADS2()) --fb; // We are dropping the unused 0th bin for SQL database.
  }

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "LAST_BIN")) )
  {
    lb = atoi(p) - 1;

    if (strstr(rp->name, "2D"))	/* 2D's use 63 bins, instead of 1DC */
      lb += 32;

    /* We are dropping the unused 0th bin for SQL database.
     * See also PostgreSQL::addVectorToAllStreams().
     */
    if (cfg.isADS2()) --lb;
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
void AddPMS1dAttrs(int ncid, var_base * rp)
{
  int	nBins, cvarid;
  char	*p;
  float	cellSize[300];
  bool	warnMidPoints = false;

  /* If not PMS1D, then bail out. */
  if (rp->ProbeType & 0xff000000 != 0x80000000)
    return;

  cvarid = rp->varid;

  MakeProjectFileName(buffer, PMS_SPEC_FILE);
  InitPMSspecs(buffer);

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "FIRST_BIN")) ) {
    int	value = atoi(p);
    ncattput(ncid, cvarid, "FirstBin", NC_INT, 1, &value);
  }

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "LAST_BIN")) ) {
    int	value = atoi(p) - 1;	/* Go from exclusive to inclusive */

    if (strstr(rp->name, "2D"))	/* 2D's use 63 bins, instead of 1DC */
      value += 32;

    ncattput(ncid, cvarid, "LastBin", NC_INT, 1, &value);
  }


  nBins = getCellSizes(rp, cellSize);
  ncattput(ncid, cvarid, "CellSizes", NC_FLOAT, nBins, cellSize);

  if (cellSize[0] == 0.0)
    warnMidPoints = true;


  if (rp->ProbeType & PROBE_260X || rp->ProbeType & PROBE_200X ||
      rp->ProbeType & PROBE_200Y || rp->ProbeType & PROBE_PMS2D)
  {
    if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "NDIODES")) ) {
      int value = atoi(p);
      ncattput(ncid, cvarid, "nDiodes", NC_INT, 1, &value);
    }

    if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "MAG")) ) {
      float value = atof(p);
      ncattput(ncid, cvarid, "Magnification", NC_FLOAT, 1, &value);
    }

    if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "RESPONSE_TIME")) ) {
      float value = atof(p);
      ncattput(ncid, cvarid, "ResponseTime", NC_FLOAT, 1, &value);
    }

    if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "ARM_DISTANCE")) ) {
      float value = atof(p);
      ncattput(ncid, cvarid, "ArmDistance", NC_FLOAT, 1, &value);
    }
  }


  ncattput(ncid, cvarid, "CellSizeUnits", NC_CHAR, 12, "micrometers");


  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "DOF")) ) {
    float value = atof(p);
    ncattput(ncid, cvarid, "DepthOfField", NC_FLOAT, 1, &value);
  }

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "BEAM_DIAM")) ) {
    float value = atof(p);
    ncattput(ncid, cvarid, "BeamDiameter", NC_FLOAT, 1, &value);
  }

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "DENS")) ) {
    float value = atof(p);
    ncattput(ncid, cvarid, "Density", NC_FLOAT, 1, &value);
  }

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "PLWFAC")) ) {
    float value = atof(p);
    ncattput(ncid, cvarid, "PLWfactor", NC_FLOAT, 1, &value);
  }

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "DBZFAC")) ) {
    float value = atof(p);
    ncattput(ncid, cvarid, "DBZfactor", NC_FLOAT, 1, &value);
  }

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "SAMPLE_AREA")) ) {
    float value = atof(p);
    ncattput(ncid, cvarid, "SampleArea", NC_FLOAT, 1, &value);
  }


  ReleasePMSspecs();


  if (rp->ProbeType & PROBE_PMS2D)
  {
    strcpy(buffer, "2D buffers with more than 8 seconds elapsed time or fewer than 20 particles.");
    ncattput(ncid, cvarid, "Rejected", NC_CHAR, strlen(buffer), buffer);

    if (strstr(rp->name, "1D"))
    {
      char * s = "Entire In";
      ncattput(ncid, cvarid, "ParticleAcceptMethod", NC_CHAR, strlen(s), s);
    }

    if (strstr(rp->name, "2D"))
    {
      char * s = "Error";
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

  /* Older projects have a PMSspecs file with mid-points instead of end-points.
   * Warn the user about this, as it will produce incorrect PMS1D results.
   */
  if (warnMidPoints)
  {
    fprintf(stderr, "PMSspecs file contains mid-point, not end-point cell diameters for %s %s.\n",
	rp->name, rp->SerialNumber.c_str());
    fprintf(stderr, "Nimbus was modified on 9/5/98 to use end-points.  Please fix.\n");
    quit();
  }
}

/* -------------------------------------------------------------------- */
static int getCellSizes(var_base * rp, float cellSize[])
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
static int setProbeCount(const char * loc, int probeNum)
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

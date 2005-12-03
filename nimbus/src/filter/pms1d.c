/*
-------------------------------------------------------------------------
OBJECT NAME:	pms1d.c

FULL NAME:	PMS-1d related stuff

ENTRY POINTS:	AddPMS1dAttrs()
		AddToPMS1DprobeList()
		GetPMS1DAttrsForSQL()

STATIC FNS:	getCellSizes()

DESCRIPTION:	AddPMS1dAttrs() adds PMSspecs probe parameters to the
		netCDF file variable attributes.  The other function is
		stub for post-processing.

REFERENCES:	none

REFERENCED BY:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-2000
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "pms.h"
#include "netcdf.h"

static int getCellSizes(RAWTBL *rp, float cellSizes[]);

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

  sprintf(buffer, PMS_SPEC_FILE, ProjectDirectory, cfg.ProjectNumber().c_str());
  InitPMSspecs(buffer);

  fb = 0;
  lb = rp->Length-1;

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "FIRST_BIN")) )
    fb = atoi(p);

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "LAST_BIN")) ) {
    lb = atoi(p) - 1;

    if (strstr(rp->name, "2D"))	/* 2D's use 63 bins, instead of 1DC */
      lb += 32;
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
void AddPMS1dAttrs(int ncid, RAWTBL *rp)
/* Add PMSspecs to netCDF attributes for probe, they go with accumulations */
{
  int	nBins;
  char	*p;
  float	cellSize[300];
  bool	warnMidPoints = false;

  /* If not PMS1D, then bail out. */
  if (rp->ProbeType & 0xff000000 != 0x80000000)
    return;

  sprintf(buffer, PMS_SPEC_FILE, ProjectDirectory, cfg.ProjectNumber().c_str());
  InitPMSspecs(buffer);

  ncattput(ncid, rp->varid, "SerialNumber", NC_CHAR,
           rp->SerialNumber.length()+1, rp->SerialNumber.c_str());

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "FIRST_BIN")) ) {
    int	value = atoi(p);
    ncattput(ncid, rp->varid, "FirstBin", NC_INT, 1, &value);
    }

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "LAST_BIN")) ) {
    int	value = atoi(p) - 1;	/* Go from exclusive to inclusive */

    if (strstr(rp->name, "2D"))	/* 2D's use 63 bins, instead of 1DC */
      value += 32;

    ncattput(ncid, rp->varid, "LastBin", NC_INT, 1, &value);
    }


  nBins = getCellSizes(rp, cellSize);
  ncattput(ncid, rp->varid, "CellSizes", NC_FLOAT, nBins, cellSize);

  if (cellSize[0] == 0.0)
    warnMidPoints = true;


  if (rp->ProbeType & PROBE_260X || rp->ProbeType & PROBE_200X ||
      rp->ProbeType & PROBE_200Y || rp->ProbeType & PROBE_PMS2D)
    {
    if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "NDIODES")) ) {
      int	value = atoi(p);
      ncattput(ncid, rp->varid, "nDiodes", NC_INT, 1, &value);
      }

    if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "MAG")) ) {
      float	value = atof(p);
      ncattput(ncid, rp->varid, "Magnification", NC_FLOAT, 1, &value);
      }

    if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "RESPONSE_TIME")) ) {
      float	value = atof(p);
      ncattput(ncid, rp->varid, "ResponseTime", NC_FLOAT, 1, &value);
      }

    if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "ARM_DISTANCE")) ) {
      float	value = atof(p);
      ncattput(ncid, rp->varid, "ArmDistance", NC_FLOAT, 1, &value);
      }
    }


  ncattput(ncid, rp->varid, "CellSizeUnits", NC_CHAR, 12, "micrometers");


  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "DOF")) ) {
    float	value = atof(p);
    ncattput(ncid, rp->varid, "DepthOfField", NC_FLOAT, 1, &value);
    }

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "BEAM_DIAM")) ) {
    float	value = atof(p);
    ncattput(ncid, rp->varid, "BeamDiameter", NC_FLOAT, 1, &value);
    }

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "DENS")) ) {
    float	value = atof(p);
    ncattput(ncid, rp->varid, "Density", NC_FLOAT, 1, &value);
    }

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "PLWFAC")) ) {
    float	value = atof(p);
    ncattput(ncid, rp->varid, "PLWfactor", NC_FLOAT, 1, &value);
    }

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "DBZFAC")) ) {
    float	value = atof(p);
    ncattput(ncid, rp->varid, "DBZfactor", NC_FLOAT, 1, &value);
    }

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "SAMPLE_AREA")) ) {
    float	value = atof(p);
    ncattput(ncid, rp->varid, "SampleArea", NC_FLOAT, 1, &value);
    }


  ReleasePMSspecs();


  if (rp->ProbeType & PROBE_PMS2D)
    {
    strcpy(buffer, "2D buffers with more than 8 seconds elapsed time or fewer than 20 particles.");
    ncattput(ncid, rp->varid, "Rejected", NC_CHAR, strlen(buffer), buffer);
    }

  /* Older projects have a PMSspecs file with mid-points instead of end-points.
   * Warn the user about this, as it will produce incorrect PMS1D results.
   */
  if (warnMidPoints)
    {
    fprintf(stderr, "PMSspecs file contains mid-point, not end-point cell diameters for %s %s.\n", rp->name, rp->SerialNumber.c_str());
    fprintf(stderr, "Nimbus was modified on 9/5/98 to use end-points.  Please fix.\n");
    exit(1);
    }

}

/* -------------------------------------------------------------------- */
static int getCellSizes(RAWTBL *rp, float cellSize[])
{
  int	i, nBins;
  char	*p;

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "CELL_SIZE")) == NULL) {
    sprintf(buffer, "CELL_SIZE_%d", rp->Length-1);
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
void AddToPMS1DprobeList(
	char    probe[],
	char    location[],
	char    serial_num[],
	int     type)
{

/* This stub is called by nimbus.  winput/winds uses the version in pms1d_rt.c
 */


}   /* END ADDTOPMS1DPROBELIST */

/* PMS1D.C */

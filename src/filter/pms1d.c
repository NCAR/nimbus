/*
-------------------------------------------------------------------------
OBJECT NAME:	pms1d.c

FULL NAME:	PMS-1D related stuff

ENTRY POINTS:	AddToPMS1DprobeList()	# hdr_decode.c (ADS2 only so far)
		PMS1D_SetupForADS3()	# hdr_decode.c
		AddPMS1dAttrs()		# NetCDF.cc
		GetPMS1DAttrsForSQL()	# psql.cc
		setProbeCount()		# src/amlib/std/probe_name.c

STATIC FNS:	setSerialNumberAndProbeType()
		setProbeType()
		getCellSizes()

DESCRIPTION:	AddPMS1dAttrs() adds PMSspecs probe parameters to the
		netCDF file variable attributes.  The other function is
		stub for post-processing.

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-2018
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include <raf/pms.h>
#include <netcdf.h>
#include <unistd.h>

#include "sync_reader.hh"
#include <nidas/core/Project.h>

static int getCellSizes(const var_base * rp, float cellSizes[]);


/* -------------------------------------------------------------------- */
void ReadPMSspecs(const char name[])
{
  char tmp[1024];

  /* Try to open a flight specific PMSspecs.rf## file first, if that does
   * not exist, fall back on the plain old PMSspecs.
   */
  strcpy(tmp, name);
  strcat(tmp, ".");
  strcat(tmp, cfg.FlightNumber().c_str());

  if (access(tmp, R_OK) == F_OK)
    InitPMSspecs(tmp);
  else
    InitPMSspecs(name);

}


/* -------------------------------------------------------------------- */
bool thisIs2Dnot1D(const char * name)
{
  char basename[64], *p;

  strcpy(basename, name);
  if ( (p = strchr(basename, '_')) )
    *p = '\0';

  return( (strstr(basename, "2D")) );
}

/* -------------------------------------------------------------------- */
static void setSerialNumberAndProbeType(const char * name, int probeType)
{
  int raw_indx, der_indx;
  char cname[64], *location;

  DLOG(("probe for variable '") << std::string(name) << "'");

  strcpy(cname, name);

  // A variables are raw Accumulation histograms.  C variables are the
  // derived Concentrations.  Particle probes also have raw and derived
  // scalars, which are matched to this serial number by location.  So for
  // a particle probe variable with location _LWOO, all variables with the
  // same location will get the same serial number.  It's possible this
  // could collide with other DSM variables when the DSM and probe have the
  // same location suffix, such as LWO.
  cname[0] = 'C';

  if (strrchr(name, '_'))	// If this is a specific probe & location.
  {
    raw_indx = SearchTable(raw, name);
    der_indx = SearchTable(derived, cname);
  }
  else
  {
    raw_indx = SearchTableSansLocation(raw, name);
    der_indx = SearchTableSansLocation(derived, cname);
  }

  if (raw_indx == ERR)
    return;

  raw[raw_indx]->ProbeType	= probeType;

  if ((location = strstr(raw[raw_indx]->name, "_")) == 0)
    printf("No location found for %s\n", raw[raw_indx]->name);

  for (size_t i = 0; i < raw.size(); ++i)
    if (strstr(raw[i]->name, location))
    {
      raw[i]->ProbeType		= raw[raw_indx]->ProbeType;
    }
  for (size_t i = 0; i < derived.size(); ++i)
    if (strstr(derived[i]->name, location))
    {
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
    printf("Debug: No concentration %s found for %s.\n", cname, name);


#ifdef of_questionable_use
  // This may be related to older work that was not finished, and may be
  // unnecessary for now.
  if (strstr(name, "1D") || strstr(name, "2D"))	// 2D data, we need probeCount setup.
  {
    char target[12];
    strcpy(target, &name[1]);

    for (size_t i = 0; i < raw.size(); ++i)
      if (strstr(raw[i]->name, target))
      {
        raw[i]->ProbeType = probeType;
      }

    for (size_t i = 0; i < derived.size(); ++i)
      if (strstr(derived[i]->name, target))
      {
        derived[i]->ProbeType = probeType;
      }
  }
#endif
}


static void
setProbeType(const std::string& aname, int probeType)
{
  // Look for all instances of this accumulation variable name, and use it
  // to set the probeType for all variables associated with this probe.
  nidas::core::Project* project = nidas::core::Project::getInstance();

  varlist_t variables;
  variables = selectVariablesFromProject(project);

  for (varlist_t::iterator it = variables.begin(); it != variables.end(); ++it)
  {
    const nidas::core::Variable* var = *it;
    std::string vname = var->getName();

    if (vname == aname || vname.find(aname + "_") == 0)
    {
      DLOG(("matched ") << aname << " with " << vname);
      setSerialNumberAndProbeType(vname.c_str(), probeType);
    }
  }
}


/* -------------------------------------------------------------------- */
void PMS1D_SetupForADS3()
{
  // Grab the Project configuration and traverse it looking for particle
  // probes.  A particle probe is anything whose catalog name is known in
  // the probe-model-to-probe-type map.  Then retrieve the serial number
  // parameter from the Sensor and propagate it to all the variables
  // related to that sensor.

  // Step through all the known particle probe accumulation variables,
  // looking them up, and setting the probe type.
  setProbeType("AS100", PROBE_PMS1D | PROBE_FSSP);
  setProbeType("AS200", PROBE_PMS1D | PROBE_PCASP);
  setProbeType("AS300", PROBE_PMS1D | PROBE_F300);
  setProbeType("A260X", PROBE_PMS1D | PROBE_260X);
  setProbeType("ACDP", PROBE_PMS1D | PROBE_CDP);
  setProbeType("AUHSAS", PROBE_PMS1D | PROBE_PCASP);
  setProbeType("A1DC", PROBE_PMS2D | PROBE_2DC);	// 2DC & 2DP
  setProbeType("A2DC", PROBE_PMS2D | PROBE_2DC);
  setProbeType("A1DP", PROBE_PMS2D | PROBE_2DP);
  setProbeType("A2DP", PROBE_PMS2D | PROBE_2DP);
  setProbeType("A1DSH", PROBE_PMS2D | PROBE_2DS);	// SPEC 2DS
  setProbeType("A2DSH", PROBE_PMS2D | PROBE_2DS);
  setProbeType("A1DSV", PROBE_PMS2D | PROBE_2DS);
  setProbeType("A2DSV", PROBE_PMS2D | PROBE_2DS);
  setProbeType("A1DH", PROBE_PMS2D | PROBE_HVPS);	// SPEC HVPS
  setProbeType("A2DH", PROBE_PMS2D | PROBE_HVPS);
  setProbeType("APIP", PROBE_PMS2D | PROBE_2DP);	// DMT PIP
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
  ReadPMSspecs(buffer);

  fb = 0;
  lb = rp->Length-1;

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "FIRST_BIN")) )
  {
    fb = atoi(p);
  }

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "LAST_BIN")) )
  {
    lb = atoi(p) - 1;

    if (thisIs2Dnot1D(rp->name))	/* 2D's use 63 bins, instead of 1DC */
      lb += (rp->Length >> 1);
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
  ReadPMSspecs(buffer);

  // Really wish I could up dymamic_cast.  What I really want to check:
  // if (varp == <RAWTBL*>)
  if (varp->name[0] == 'A')
  {
    if (varp->ProbeType & PROBE_260X || varp->ProbeType & PROBE_200X ||
        varp->ProbeType & PROBE_200Y || varp->ProbeType & PROBE_PMS2D)
    {
      if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "RANGE_STEP")) ) {
        int value = atoi(p);
        nc_put_att_int(ncid, cvarid, "Resolution", NC_INT, 1, &value);
      }

      if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "NDIODES")) ) {
        int value = atoi(p);
        nc_put_att_int(ncid, cvarid, "nDiodes", NC_INT, 1, &value);
      }

      if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "MAG")) ) {
        float value = atof(p);
        nc_put_att_float(ncid, cvarid, "Magnification", NC_FLOAT, 1, &value);
      }

      if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "RESPONSE_TIME")) ) {
        float value = atof(p);
        nc_put_att_float(ncid, cvarid, "ResponseTime", NC_FLOAT, 1, &value);
      }

      if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "ARM_DISTANCE")) ) {
        float value = atof(p);
        nc_put_att_float(ncid, cvarid, "ArmDistance", NC_FLOAT, 1, &value);
      }
    }

    if ((varp->ProbeType & PROBE_PMS2D))
    {
      if (varp->Length < 64)
      {
        strcpy(buffer, "2D buffers with more than 8 seconds elapsed time or fewer than 20 particles.");
        nc_put_att_text(ncid, cvarid, "Rejected", strlen(buffer), buffer);
      }

      if (strstr(varp->name, "1D"))
      {
        const char * s = "Entire In";
        nc_put_att_text(ncid, cvarid, "ParticleAcceptMethod", strlen(s), s);
      }

      if (thisIs2Dnot1D(varp->name))
      {
        const char * s = "Error";
        float f;

        if (cfg.TwoDProcessingMethod() == Config::Center_In)
          s = "Center In";
        if (cfg.TwoDProcessingMethod() == Config::Reconstruction)
          s = "Reconstruction";
        nc_put_att_text(ncid, cvarid, "ParticleAcceptMethod", strlen(s), s);

        f = cfg.TwoDAreaRejectRatio();
        nc_put_att_float(ncid, cvarid, "ParticleAreaRejectionRatio", NC_FLOAT, 1, &f);
      }
    }
  }
  else
  {
    if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "FIRST_BIN")) ) {
      int	value = atoi(p);
      nc_put_att_int(ncid, cvarid, "FirstBin", NC_INT, 1, &value);
    }

    if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "LAST_BIN")) ) {
      int	value = atoi(p) - 1;	/* Go from exclusive to inclusive */

      if (thisIs2Dnot1D(varp->name))	/* 2D's use 63 bins, instead of 1DC */
        value += (varp->Length >> 1);

      nc_put_att_int(ncid, cvarid, "LastBin", NC_INT, 1, &value);
    }

    nBins = getCellSizes(varp, cellSize);
    nc_put_att_float(ncid, cvarid, "CellSizes", NC_FLOAT, nBins, cellSize);
    nc_put_att_text(ncid, cvarid, "CellSizeUnits", 11, "micrometers");
    nc_put_att_text(ncid, cvarid, "CellSizeNote", 43, "CellSizes are lower bin limits as particle size.");

    if (cellSize[0] == 0.0)
      warnMidPoints = true;

    if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "DOF")) ) {
      float value = atof(p);
      nc_put_att_float(ncid, cvarid, "DepthOfField", NC_FLOAT, 1, &value);
    }

    if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "BEAM_DIAM")) ) {
      float value = atof(p);
      nc_put_att_float(ncid, cvarid, "BeamDiameter", NC_FLOAT, 1, &value);
    }

    if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "DENS")) ) {
      float value = atof(p);
      nc_put_att_float(ncid, cvarid, "Density", NC_FLOAT, 1, &value);
    }

    if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "PLWFAC")) ) {
      float value = atof(p);
      nc_put_att_float(ncid, cvarid, "PLWfactor", NC_FLOAT, 1, &value);
    }

    if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "DBZFAC")) ) {
      float value = atof(p);
      nc_put_att_float(ncid, cvarid, "DBZfactor", NC_FLOAT, 1, &value);
    }

    if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "SAMPLE_AREA")) ) {
      float value = atof(p);
      nc_put_att_float(ncid, cvarid, "SampleArea", NC_FLOAT, 1, &value);
    }

    if ((p = GetPMSparameter(varp->SerialNumber.c_str(), "PULSE_WIDTH")) ) {
      float value = atof(p);
      nc_put_att_float(ncid, cvarid, "PulseWidth", NC_FLOAT, 1, &value);
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
    sprintf(buffer, "CELL_SIZE_%lu", rp->Length);
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


  if (rp->ProbeType & PROBE_260X || thisIs2Dnot1D(rp->name) || strstr(rp->name, "PIP"))
    nBins = 64;
  else
  if (rp->ProbeType == PROBE_2DS)
    nBins = 128;
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

  float	min = 0.0, step = 0.0;

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "MIN_RANGE")) )
    min = atof(p);

#ifdef notdef
  float max = 0.0;
  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "MAX_RANGE")) )
    max = atof(p);
#endif

  if ((p = GetPMSparameter(rp->SerialNumber.c_str(), "RANGE_STEP")) )
    step = atof(p);

  nBins += 1;	// One more than actual bins
  for (i = 0; i < nBins; min += step)
    cellSize[i++] = min;

  return(nBins);

}	/* GETCELLSIZES */

/* -------------------------------------------------------------------- */
void setProbeCount(const char *loc, int probeNum)
{
  // This is an ADS3 operation only, ADS2 is correctly setup in hdr_decode.c
  if (cfg.isADS2())
    return;

  const char *underscore = strchr(loc, '_');
  char target1[64];	// location
  char target2[64];	// optional, used for 1DC vs 2DC in same location.

  strcpy(target1, loc);
  memset(target2, 0, 64);
  if (underscore)
    strcpy(target1, underscore);	// grab just location

  if (underscore && loc[0] != '_')	// we have more than just location (1DC/2DC)
  {
    strcpy(target2, loc);
    *(strchr(target2, '_')) = 0;
  }

  for (size_t i = 0; i < raw.size(); ++i)
    if (strstr(raw[i]->name, target1))
    {
      if (strlen(target2) == 0 || (strlen(target2) && strstr(raw[i]->name, target2)))
        raw[i]->ProbeCount = probeNum;
    }

  for (size_t i = 0; i < derived.size(); ++i)
    if (strstr(derived[i]->name, target1))
    {
      if (strlen(target2) == 0 || (strlen(target2) && strstr(derived[i]->name, target2)))
        derived[i]->ProbeCount = probeNum;
    }
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

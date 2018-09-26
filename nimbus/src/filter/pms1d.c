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

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-2011
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include <raf/pms.h>
#include <netcdf.h>

#include "sync_reader.hh"
#include <nidas/core/Project.h>

static int getCellSizes(const var_base * rp, float cellSizes[]);


/* -------------------------------------------------------------------- */
static void setSerialNumberAndProbeType(const char * name, const char * serialNum, int probeType)
{
  int raw_indx, der_indx;
  char cname[64], *location;

  DLOG(("probe for variable '") << std::string(name)
       << "' getting serial number '" << std::string(serialNum) << "'");

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

  raw[raw_indx]->SerialNumber	= serialNum;
  raw[raw_indx]->ProbeType	= probeType;

  if ((location = strstr(raw[raw_indx]->name, "_")) == 0)
    printf("No location found for %s\n", raw[raw_indx]->name);

  for (size_t i = 0; i < raw.size(); ++i)
    if (strstr(raw[i]->name, location))
    {
      raw[i]->SerialNumber	= raw[raw_indx]->SerialNumber;
      raw[i]->ProbeType		= raw[raw_indx]->ProbeType;
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
#endif
}


static void
setVariableProbeType(const std::string& aname, int probeType)
{
  // Look for all instances of this accumulation variable name, and use it
  // to pull the serial number for it's particle probe from the XML.
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
      //const nidas::core::SampleTag* tag = var->getSampleTag();
      //const nidas::core::DSMSensor* sensor = project->findSensor(tag);
      std::string snumber = getSerialNumber(var);
      setSerialNumberAndProbeType(vname.c_str(), snumber.c_str(), probeType);
    }
  }
}



// Temporary hack, until I finish consolidating suport files into VarDB.ncml.
void PMS1D_SetupForADS3()
{
  // Grab the Project configuration and traverse it looking for particle
  // probes.  A particle probe is anything whose catalog name is known in
  // the probe-model-to-probe-type map.  Then retrieve the serial number
  // parameter from the Sensor and propagate it to all the variables
  // related to that sensor.

  // Really this should be extended to all variables, so that all sensor
  // serial numbers are associated with their variables, at least their
  // "raw" variables.


  // Step through all the known particle probe accumulation variables,
  // looking them up, finding the serial number and propagating it.
  setVariableProbeType("AS100", PROBE_PMS1D | PROBE_FSSP);
  setVariableProbeType("AS200", PROBE_PMS1D | PROBE_PCASP);
  setVariableProbeType("AS300", PROBE_PMS1D | PROBE_F300);
  setVariableProbeType("A260X", PROBE_PMS1D | PROBE_260X);
  setVariableProbeType("ACDP", PROBE_PMS1D | PROBE_CDP);
  setVariableProbeType("AUHSAS", PROBE_PMS1D | PROBE_PCASP);
  setVariableProbeType("A1DC", PROBE_PMS2D | PROBE_2DC);
  setVariableProbeType("A2DC", PROBE_PMS2D | PROBE_2DC);
  setVariableProbeType("A1DP", PROBE_PMS2D | PROBE_2DP);
  setVariableProbeType("A2DP", PROBE_PMS2D | PROBE_2DP);

#ifdef notdef
  // Remove this section once the sync-server branch is merged and the
  // above is confirmed to work.

  // Start with these serial numbers as defaults.
  setSerialNumberAndProbeType("AS100", "FSSP109", PROBE_PMS1D | PROBE_FSSP);
  setSerialNumberAndProbeType("AS200", "PCAS108", PROBE_PMS1D | PROBE_PCASP);
  setSerialNumberAndProbeType("AS300", "FSSP305", PROBE_PMS1D | PROBE_F300);
  setSerialNumberAndProbeType("A260X", "260X06", PROBE_PMS1D | PROBE_260X);
  setSerialNumberAndProbeType("ACDP", "CDP016", PROBE_PMS1D | PROBE_CDP);
  setSerialNumberAndProbeType("AUHSAS", "UHSAS001", PROBE_PMS1D | PROBE_PCASP);
  setSerialNumberAndProbeType("A1DC", "F2DC001", PROBE_PMS2D | PROBE_2DC);
  setSerialNumberAndProbeType("A2DC", "F2DC001", PROBE_PMS2D | PROBE_2DC);
  setSerialNumberAndProbeType("A1DP", "F2DP001", PROBE_PMS2D | PROBE_2DP);
  setSerialNumberAndProbeType("A2DP", "F2DP001", PROBE_PMS2D | PROBE_2DP);

  if (cfg.ProjectName().compare("SAANGRIA-TEST") == 0)
  {
    setSerialNumberAndProbeType("A1DC", "F2DC002", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC", "F2DC002", PROBE_PMS2D | PROBE_2DC);
  }
  else
  if (cfg.ProjectName().compare("TEST-C130") == 0)
  {
    setSerialNumberAndProbeType("AS100_old", "FSSP122", PROBE_PMS1D | PROBE_FSSP);
    setSerialNumberAndProbeType("AS100_new", "FSSP109", PROBE_PMS1D | PROBE_FSSP);
    setSerialNumberAndProbeType("ACDP_16RWO", "CDP016", PROBE_PMS1D | PROBE_CDP);
    setSerialNumberAndProbeType("ACDP_58RWI", "CDP058", PROBE_PMS1D | PROBE_CDP);
    setSerialNumberAndProbeType("A1DC_RPI", "F2DC002", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC_RPI", "F2DC002", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A1DC_LPI", "F2DC003", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC_LPI", "F2DC003", PROBE_PMS2D | PROBE_2DC);
  }
  else
  if (cfg.ProjectName().compare("PASE") == 0)
  {
    setSerialNumberAndProbeType("ACDP", "CDP058", PROBE_PMS1D | PROBE_CDP);
    setSerialNumberAndProbeType("A1DC", "F2DC001", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC", "F2DC001", PROBE_PMS2D | PROBE_2DC);
  }
  else
  if (cfg.ProjectName().compare("PASE") == 0)
    setSerialNumberAndProbeType("AS100", "FSSP122", PROBE_PMS1D | PROBE_FSSP);
  else
  if (cfg.ProjectName().compare("DC3") == 0)
  {
    setSerialNumberAndProbeType("ACDP_LWII", "CDP058", PROBE_PMS1D | PROBE_CDP);
    setSerialNumberAndProbeType("A1DC_LWIO", "F2DC001", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC_LWIO", "F2DC001", PROBE_PMS2D | PROBE_2DC);
  }
  else
  if (cfg.ProjectName().compare("DC3-TEST") == 0)
  {
    setSerialNumberAndProbeType("A1DC_LWOO", "F2DC001", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC_LWOO", "F2DC001", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A1DC_RWOO", "F2DC003", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC_RWOO", "F2DC003", PROBE_PMS2D | PROBE_2DC);
  }
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
    setSerialNumberAndProbeType("A1DC", "F2DC001", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC", "F2DC001", PROBE_PMS2D | PROBE_2DC);
  }
  else
  if (cfg.ProjectName().compare("PLOWS") == 0)
  {
    setSerialNumberAndProbeType("A1DC_RPC", "F2DC001", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC_RPC", "F2DC001", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A1DC_LPC", "F2DC002", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC_LPC", "F2DC002", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A1DP", "2DP10", PROBE_PMS2D | PROBE_2DP);
    setSerialNumberAndProbeType("A2DP", "2DP10", PROBE_PMS2D | PROBE_2DP);
    setSerialNumberAndProbeType("AS100_LWI", "FSSP109", PROBE_PMS1D | PROBE_FSSP);
    setSerialNumberAndProbeType("AS100_LWO", "FSSP122", PROBE_PMS1D | PROBE_FSSP);
  }
  else
  if (cfg.ProjectName().compare("HIPPO-2") == 0)
  {
    setSerialNumberAndProbeType("A1DC", "F2DC003", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC", "F2DC003", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("ACDP", "CDP058", PROBE_PMS1D | PROBE_CDP);
  }
  else
  if (cfg.ProjectName().compare("HIPPO-3") == 0 ||
      cfg.ProjectName().compare("HIPPO-4") == 0 ||
      cfg.ProjectName().compare("HIPPO-5") == 0)
  {
    setSerialNumberAndProbeType("A1DC", "F2DC003", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC", "F2DC003", PROBE_PMS2D | PROBE_2DC);
  }
  else
  if (cfg.ProjectName().compare("PREDICT") == 0)
  {
    setSerialNumberAndProbeType("A1DC", "F2DC003", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC", "F2DC003", PROBE_PMS2D | PROBE_2DC);
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
  if (cfg.ProjectName().compare("IDEAS-4") == 0 && cfg.Aircraft() == Config::C130)
  {
    setSerialNumberAndProbeType("A1DC_LPC", "F2DC001", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC_LPC", "F2DC001", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A1DC_LPI", "F2DC003", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC_LPI", "F2DC003", PROBE_PMS2D | PROBE_2DC);
  }
  else
  if (cfg.ProjectName().compare("IDEAS-4") == 0 && cfg.Aircraft() == Config::HIAPER)
  {
    setSerialNumberAndProbeType("AS100", "FSSP122", PROBE_PMS1D | PROBE_FSSP);
    setSerialNumberAndProbeType("AUHSAS", "UHSAS002", PROBE_PMS1D | PROBE_PCASP);

    if (cfg.FlightNumber().compare("rf08") <= 0) 
    {
      setSerialNumberAndProbeType("A1DC_LWIO", "F2DC001", PROBE_PMS2D | PROBE_2DC);
      setSerialNumberAndProbeType("A2DC_LWIO", "F2DC001", PROBE_PMS2D | PROBE_2DC);
    }

    if (cfg.FlightNumber().compare("rf03") >= 0 && cfg.FlightNumber().compare("rf05") <= 0) 
    {
      setSerialNumberAndProbeType("A1DC_RWOI", "F2DC003", PROBE_PMS2D | PROBE_2DC);
      setSerialNumberAndProbeType("A2DC_RWOI", "F2DC003", PROBE_PMS2D | PROBE_2DC);
    }
    else
    {
      setSerialNumberAndProbeType("A1DC_RWOI", "F2DC002", PROBE_PMS2D | PROBE_2DC);
      setSerialNumberAndProbeType("A2DC_RWOI", "F2DC002", PROBE_PMS2D | PROBE_2DC);
    }

    if (cfg.FlightNumber().compare("rf05") >= 0) 
      setSerialNumberAndProbeType("ACDP", "CDP058", PROBE_PMS1D | PROBE_CDP);

    if (cfg.FlightDate().compare("10/18/2013") >= 0) 
    {
      setSerialNumberAndProbeType("ACDP_RWOI", "CDP016", PROBE_PMS1D | PROBE_CDP);
      setSerialNumberAndProbeType("ACDP_LWIO", "CDP058", PROBE_PMS1D | PROBE_CDP);
    }
  }
  else
  if (cfg.ProjectName().compare("ICE-T") == 0)
  {
    if (cfg.FlightNumber().compare("ff01") >= 0) 
      setSerialNumberAndProbeType("ACDP", "CDP058", PROBE_PMS1D | PROBE_CDP);
    if (cfg.FlightNumber().compare("ff03") >= 0) 
      setSerialNumberAndProbeType("ACDP", "CDP016", PROBE_PMS1D | PROBE_CDP);
    if (cfg.FlightNumber().compare("rf01") >= 0) 
      setSerialNumberAndProbeType("ACDP", "CDP058", PROBE_PMS1D | PROBE_CDP);
    if (cfg.FlightNumber().compare("rf06") >= 0) 
      setSerialNumberAndProbeType("ACDP", "CDP016", PROBE_PMS1D | PROBE_CDP);
    if (cfg.FlightNumber().compare("tf01") >= 0) 
      setSerialNumberAndProbeType("ACDP", "CDP058", PROBE_PMS1D | PROBE_CDP);
    setSerialNumberAndProbeType("AS100", "FSSP122", PROBE_PMS1D | PROBE_FSSP);
    setSerialNumberAndProbeType("AS200", "PCAS108", PROBE_PMS1D | PROBE_PCASP);
    setSerialNumberAndProbeType("AS300", "FSSP305", PROBE_PMS1D | PROBE_F300);
    setSerialNumberAndProbeType("A1DC", "F2DC001", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC", "F2DC001", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A1DP", "F2DP001", PROBE_PMS2D | PROBE_2DP);
    setSerialNumberAndProbeType("A2DP", "F2DP001", PROBE_PMS2D | PROBE_2DP);
    setSerialNumberAndProbeType("AUHSAS_CVI", "UHSAS002", PROBE_PMS1D | PROBE_PCASP);
    setSerialNumberAndProbeType("AUHSAS_GH", "UHSAS003", PROBE_PMS1D | PROBE_PCASP);
  }
  else
  if (cfg.ProjectName().compare("NOMADSS") == 0)
  {
    setSerialNumberAndProbeType("AS100", "FSSP122", PROBE_PMS1D | PROBE_FSSP);
  }
  else
  if (cfg.ProjectName().compare("CONTRAST") == 0)
  {
    setSerialNumberAndProbeType("ACDP", "CDP058", PROBE_PMS1D | PROBE_CDP);
  }
  else
  if (cfg.ProjectName().compare("HCRTEST") == 0)
  {
    setSerialNumberAndProbeType("A1DC_RWIO", "F2DC002", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A1DC_RWOI", "F2DC003", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("ACDP", "CDP016", PROBE_PMS1D | PROBE_CDP);
  }
  else
  if (cfg.ProjectName().compare("WINTER") == 0)
  {
    setSerialNumberAndProbeType("AS100", "FSSP122", PROBE_PMS1D | PROBE_CDP);
    setSerialNumberAndProbeType("A1DC", "F2DC003", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC", "F2DC003", PROBE_PMS2D | PROBE_2DC);
  }
  else
  if (cfg.ProjectName().compare("ARISTO2015") == 0)
  {
    setSerialNumberAndProbeType("AS100", "FSSP122", PROBE_PMS1D | PROBE_CDP);
    setSerialNumberAndProbeType("A1DC_LPB", "F2DC001", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A1DC_RPB", "F2DC002", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("ACDP", "CDP016", PROBE_PMS1D | PROBE_CDP);
  }
  else
  if (cfg.ProjectName().compare("ORCAS") == 0)
  {
    setSerialNumberAndProbeType("ACDP", "CDP058", PROBE_PMS1D | PROBE_CDP);
    if (cfg.FlightNumber().compare(0, 4, "ff01") == 0 ||
        cfg.FlightNumber().compare(0, 4, "ff02") == 0 ||
        cfg.FlightNumber().compare(0, 2, "tf") == 0)
      setSerialNumberAndProbeType("A1DC", "F2DC002", PROBE_PMS2D | PROBE_2DC);
    else
      setSerialNumberAndProbeType("A1DC", "F2DC001", PROBE_PMS2D | PROBE_2DC);
  }
  else
  if (cfg.ProjectName().compare("ARISTO2016") == 0)
  {
    setSerialNumberAndProbeType("AUHSAS_LWI", "UHSAS001", PROBE_PMS1D | PROBE_PCASP);
    setSerialNumberAndProbeType("AUHSAS_CAB", "UHSAS011", PROBE_PMS1D | PROBE_PCASP);	// Univ Wyoming
    setSerialNumberAndProbeType("ACDP", "CDP058", PROBE_PMS1D | PROBE_CDP);
    setSerialNumberAndProbeType("A1DC", "F2DC002", PROBE_PMS2D | PROBE_2DC);
  }
  else
  if (cfg.ProjectName().compare("ARISTO2017") == 0)
  {
    setSerialNumberAndProbeType("ACDP_RWIO", "CDP016", PROBE_PMS1D | PROBE_CDP);
    setSerialNumberAndProbeType("ACDP_RWII", "CDP058", PROBE_PMS1D | PROBE_CDP);
    setSerialNumberAndProbeType("A1DC", "F2DC002", PROBE_PMS2D | PROBE_2DC);
  }
  else
  if (cfg.ProjectName().compare("SOCRATES") == 0)
  {
    setSerialNumberAndProbeType("A1DC_RWOI", "F2DC003", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A2DC_RWOI", "F2DC003", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("AUHSAS_LWII", "UHSAS001", PROBE_PMS1D | PROBE_PCASP);
    setSerialNumberAndProbeType("AUHSAS_CVIU", "UHSAS015", PROBE_PMS1D | PROBE_PCASP);
    setSerialNumberAndProbeType("APIP_RWII", "PIP011", PROBE_PMS2D | PROBE_2DP);
  }
  else
  if (cfg.ProjectName().compare("WECAN") == 0)
  {
    setSerialNumberAndProbeType("A1DC_LPO", "F2DC003", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("A1DC_LPC", "F2DC002", PROBE_PMS2D | PROBE_2DC);
    setSerialNumberAndProbeType("AUHSAS_RPO", "UHSAS001", PROBE_PMS1D | PROBE_PCASP);
    setSerialNumberAndProbeType("AUHSAS_CVIU", "UHSAS015", PROBE_PMS1D | PROBE_PCASP);
  }
#endif
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

    if ((varp->ProbeType & PROBE_PMS2D))
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
    ncattput(ncid, cvarid, "CellSizeNote", NC_CHAR, 43, "CellSizes are upper bin limits as diameter.");
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
    sprintf(buffer, "CELL_SIZE_%lu", rp->Length - 1);
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


  if (rp->ProbeType & PROBE_260X || strstr(rp->name, "2D") || strstr(rp->name, "PIP"))
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

  for (i = 0; i < nBins; min += step)
    cellSize[i++] = min;

  return(nBins);

}	/* GETCELLSIZES */

/* -------------------------------------------------------------------- */
void setProbeCount(const char *loc, int probeNum)
{
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

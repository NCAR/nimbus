/*
-------------------------------------------------------------------------
OBJECT NAME:	conc2d.c

FULL NAME:	PMS 2d concentrations

ENTRY POINTS:	sconc2dc(), sconc2dp()

STATIC FNS:	none

DESCRIPTION:	Concentrations from 2d probes with NEW housekeeping block.

INPUT:		True airspeed, shadow counts

OUTPUT:		Concentration

COPYRIGHT:	University Corporation for Atmospheric Research, 1997-2010
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"
#include <raf/pms.h>

#include <map>

static std::map<std::string, float> SampleArea;

/* -------------------------------------------------------------------- */
void conc2Init(var_base *varp)
{ 
  if (varp->SerialNumber.length() == 0) {
    if (cfg.isADS3()) {
      fprintf(stderr, "conc2d.c: %s has no serial number, fatal.\n", varp->name);
      exit(1);
    }

    /* The code used to have fixed EAW and arm distance, so there was no serial number.
     * My last refactor of this code broke it for ADS2 files.  Values are the same for
     * all probes, fake it here.
     */
    if (strstr(varp->name, "2C")) varp->SerialNumber = "2DC18";
    if (strstr(varp->name, "2P")) varp->SerialNumber = "2DP10";
  }

  const char *serialNumber = varp->SerialNumber.c_str();

  MakeProjectFileName(buffer, PMS_SPEC_FILE);
  InitPMSspecs(buffer);

  char *eaw, *arm_dist;
  if ((eaw = GetPMSparameter(serialNumber, "EAW")) == NULL) {
    fprintf(stderr, "conc2d.c: serial number = [%s]: EAW not found.\n",
		serialNumber); exit(1);
  }
  if ((arm_dist = GetPMSparameter(serialNumber, "ARM_DISTANCE")) == NULL) {
    fprintf(stderr, "conc2d.c: serial number = [%s]: ARM_DISTANCE not found.\n",
		serialNumber); exit(1);
  }

  SampleArea[varp->SerialNumber] = atof(eaw) * atof(arm_dist);
}

/* -------------------------------------------------------------------- */
void sconc2d(DERTBL *varp)
{
  NR_TYPE	shador, tasx;

  shador = GetSample(varp, 0);
  tasx = GetSample(varp, 1);

  if (isnan(tasx))	/* Lab setting, for spinning disk */
    tasx = 34.0;

  PutSample(varp, shador / (tasx * SampleArea[varp->SerialNumber]) * 1000.0);

}	/* END SCONC2D */

/* END CONC2D.C */

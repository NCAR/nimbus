/*
-------------------------------------------------------------------------
OBJECT NAME:	uvhyg.c

FULL NAME:	

ENTRY POINTS:	suvmr()

STATIC FNS:	

DESCRIPTION:	Stuarts UV Hygrometer

COPYRIGHT:	University Corporation for Atmospheric Research, 2007
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static NR_TYPE sigma = 1.158e-17;
static NR_TYPE offset = 0.21;
static NR_TYPE gain = 0.9;
static NR_TYPE pathlength = 0.4318;


/* -------------------------------------------------------------------- */
/**
 * UV Hygrometer pre-2013.
 */
void suvmr(DERTBL * varp)
{
  NR_TYPE	xuvi, psx, atx;

  xuvi = GetSample(varp, 0);
  psx = GetSample(varp, 1);
  atx = GetSample(varp, 2);

  if (cfg.isADS2())	// Old UV Hygrometer
    PutSample(varp, (log((xuvi + 0.125) / 1.33) + 0.35 * psx / atx) / -0.1);
  else	// VOCALS and later.
    PutSample(varp, -37.95 * ((atx + Kelvin) / psx) * (log((xuvi - 0.076) / 12.25) + 0.00045 * psx));
}


/* -------------------------------------------------------------------- */
void uvhInit(var_base *varp)
{
  float *tmp;


  if (cfg.ProjectName().compare("NOMADSS") == 0)	// NOMADSS only.
  {
    LogMessage("UVHygrometer: NOMADSS specific multiplier being applied to XSIGV.");
    ((DERTBL*)varp)->depends[1]->addToMetadata(
	"NOTE", "NOMADSS specific: XSIGV is converted to g/cm3, multiplied by 3.3427e16 to convert to #/cm3");
  }



  if ((tmp = GetDefaultsValue("UV_SIGMA", varp->name)) == NULL)
  {
    sprintf(buffer, "UV_SIGMA set to %f in AMLIB function uvhyg.\n", sigma);
    LogMessage(buffer);
  }
  else
    sigma = tmp[0];

  if ((tmp = GetDefaultsValue("UV_OFFSET", varp->name)) == NULL)
  {
    sprintf(buffer, "UV_OFFSET set to %f in AMLIB function uvhyg.\n", offset);
    LogMessage(buffer);
  }
  else
    offset = tmp[0];

  if ((tmp = GetDefaultsValue("UV_GAIN", varp->name)) == NULL)
  {
    sprintf(buffer, "UV_GAIN set to %f in AMLIB function uvhyg.\n", gain);
    LogMessage(buffer);
  }
  else
    gain = tmp[0];

  if ((tmp = GetDefaultsValue("UV_PATHLEN", varp->name)) == NULL)
  {
    sprintf(buffer, "UV_PATHLEN set to %f in AMLIB function uvhyg.\n", pathlength);
    LogMessage(buffer);
  }
  else
    pathlength = tmp[0];
}

/* -------------------------------------------------------------------- */
void sconch(DERTBL *varp)
{
  NR_TYPE       xsigv, conc;

  xsigv = GetSample(varp, 0);

  /* New UVHYG code note fully ready for NOMADSS.  nidas cal files for
   * XSIGV convert it to g/cm3.  CONCH will just convert this from g/cm3
   * to #/cm3 with below convertion.
   */
  if (cfg.ProjectName().compare("NOMADSS") == 0)	// NOMADSS only.
    conc = xsigv * 3.3427E+16;	// convert from g/cm3 to #/cm3.
  else
    conc = log((xsigv - offset) / gain) / (-sigma * pathlength);

  if (conc <= 0.0)
    conc = floatNAN;

  PutSample(varp, conc);
}


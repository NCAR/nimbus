/*
-------------------------------------------------------------------------
OBJECT NAME:	deni.c

FULL NAME:	Distance East & Distance North.

ENTRY POINTS:	deiInit()
		sdei()
		sdni()

STATIC FNS:	none

DESCRIPTION:	Compute distance north and east of the initial latitude
		and longitude in km.

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2004
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

/*  Values for BJC (Jeffco Airport, Broomfield, Colorado)
 *    based on Hangar Apron Reference Point Summary, January 1994   RLR */
static NR_TYPE	startLat = 39.913333, startLon = -105.118333;

/* -------------------------------------------------------------------- */
void deiInit(var_base *varp)
{
  float  *tmp;

  if ((tmp = GetDefaultsValue("DENI_START_LAT", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function deiInit.\n", startLat);
    LogMessage(buffer);
  }
  else
    startLat = tmp[0];

  if ((tmp = GetDefaultsValue("DENI_START_LON", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function deiInit.\n", startLon);
    LogMessage(buffer);
  }
  else
    startLon = tmp[0];

  std::vector<float> lat, lon;
  lat.push_back(startLat);
  varp->addToMetadata("StartingLatitude", lat);
  lon.push_back(startLon);
  varp->addToMetadata("StartingLongitude", lon);

}  /* END DEIINIT */

/* -------------------------------------------------------------------- */
void sdei(DERTBL *varp)
{
  NR_TYPE    lat, lon;

  lat = GetSample(varp, 0);
  lon = GetSample(varp, 1);

  PutSample(varp, (lon - startLon) * 111.17 * cos((double)(DEG_RAD*lat)));

}  /* END SDEI */

/* -------------------------------------------------------------------- */
void sdni(DERTBL *varp)
{
  NR_TYPE  lat;

  lat = GetSample(varp, 0);

  PutSample(varp, (lat - startLat) * 111.17);

}  /* END SDNI */

/* -------------------------------------------------------------------- */
void sdri(DERTBL *varp)
{
  NR_TYPE    dei, dni;

  dei = GetSample(varp, 0);
  dni = GetSample(varp, 1);

  PutSample(varp, (NR_TYPE)sqrt(dei*dei+dni*dni));
}

/* -------------------------------------------------------------------- */
void sdai(DERTBL *varp)
{
  NR_TYPE    dei, dni;

  dei = GetSample(varp, 0);
  dni = GetSample(varp, 1);

  PutSample(varp, (NR_TYPE)atan2(dni, dei) * RAD_DEG);
}

/* END DENI.C */

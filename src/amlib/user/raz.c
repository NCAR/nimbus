/*
-------------------------------------------------------------------------
OBJECT NAME:	raz.c

FULL NAME:	

ENTRY POINTS:	srange(), sazimuth()

STATIC FNS:	none

DESCRIPTION:	Functions for Radius and Distance from a fixed object.
		Used initially for SCMS project, fixed point was CP-2 radar 7/95.

COPYRIGHT:	University Corporation for Atmospheric Research, 1995
-------------------------------------------------------------------------
*/

#include <math.h>

#include "nimbus.h"
#include "amlib.h"

#define RAD	0.01745329

static NR_TYPE	r;

/*  Values from /home/local/proj/defaults/Defaults on 30 April 1998  RLR  */
static NR_TYPE	startLat = 28.5,  /*  fixed point was CP-2 radar 7/95  */
                startLon = -80.5;

/* -------------------------------------------------------------------- */
void razInit(var_base *varp)
{
  NR_TYPE  *tmp;

  if ((tmp = GetDefaultsValue("RAZ_START_LAT", varp->name)) == NULL)
    {
    sprintf(buffer, "Value set to %f in AMLIB function razInit.\n", startLat);
    LogMessage(buffer);
    }
  else
    startLat = tmp[0];

  if ((tmp = GetDefaultsValue("RAZ_START_LON", varp->name)) == NULL)
    {
    sprintf(buffer, "Value set to %f in AMLIB function razInit.\n", startLon);
    LogMessage(buffer);
    }
  else
    startLon = tmp[0];

}	/* END INITRAZ */

/* -------------------------------------------------------------------- */
void srange(DERTBL *varp)
{
  NR_TYPE	lat, lon, x, y, d;

  lat = GetSample(varp, 0);
  lon = GetSample(varp, 1);

  x = (lon - startLon) * cos((double)(RAD*lat)) * 60.0;

  y = (lat - startLat) * 60.0;

  d = sqrt(x*x + y*y);
  r = atan(x / y) * 57.29577952;

  if (x < 0.0 && y >= 0.0)
    r += 360.0;
  else
  if (x < 0.0 && y < 0.0)
    r += 180.0;
  else
  if (x >= 0.0 && y < 0.0)
    r += 180.0;

  PutSample(varp, d);

}	/* END SRANGE */

/* -------------------------------------------------------------------- */
void sazimuth(DERTBL *varp)
{
  PutSample(varp, r);

}	/* EMD SAZIMUTH */

/* END RAZ.C */

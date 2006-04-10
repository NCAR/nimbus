/*
-------------------------------------------------------------------------
OBJECT NAME:	boomln.c

FULL NAME:	Retrieve Boom Length for given Aircraft

ENTRY POINTS:	GetBoomLength()

STATIC FNS:	none

DESCRIPTION:	Boom Length is defined as the distance from the Inertial
		system to the tip of the Radome.

INPUT:		none

OUTPUT:		float Boom_Length

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2006
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "ac.h"


/* -------------------------------------------------------------------- */
NR_TYPE GetBoomLength()
{
  NR_TYPE boomln;
  char aircraft[16];

  switch (cfg.Aircraft())
    {
    case Config::HIAPER:
      sprintf(aircraft, "N677F");
      break;

    case Config::NRL_P3:
      sprintf(aircraft, "NRL-P3");
      break;

    case Config::C130:
    case Config::B57:
      sprintf(aircraft, "N%03dAR", cfg.Aircraft());
      break;

    default:
      sprintf(aircraft, "N%03dD", cfg.Aircraft());
    }

  InitAircraftSpecs(AIRCRAFT.c_str());
  boomln = atof(GetAircraftParameter(aircraft, "BOOM_LEN"));
  ReleaseAircraftSpecs();

  return(boomln);

}	/* END GETBOOMLENGTH */

/* END BOOMLN.C */

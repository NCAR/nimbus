/*
-------------------------------------------------------------------------
OBJECT NAME:	boomln.c

FULL NAME:	Retrieve Boom Length for given Aircraft

ENTRY POINTS:	GetBoomLength()

STATIC FNS:	none

DESCRIPTION:	Boom Length is defined as the distance from the Inertial
		system to the tip of the Radome.

NOTE:		Since corrected winds use the GPS or GPS blended data, they
		need the boom length from the GPS antenna, which may be in
		a different locale than the INS.

		e.g on the GV, INS is 4.4m from nose tip, Novatel antenna
		is 8.7m from nose tip.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2016
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include <raf/ac.h>


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

    case Config::TADS:
      strcpy(aircraft, "GONADS");
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

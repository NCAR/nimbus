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

REFERENCES:	libaircraft.a

REFERENCED BY:	irs.c, ins.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-96
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "ac.h"
#include "raf.h"

extern int Aircraft;

/* -------------------------------------------------------------------- */
NR_TYPE GetBoomLength()
{
  NR_TYPE	boomln;
  char    aircraft[8];

  switch (Aircraft)
    {
    case NRL_P3:
      sprintf(aircraft, "NRL-P3");
      break;

    case C130:
    case B57:
      sprintf(aircraft, "N%03dAR", Aircraft);
      break;

    default:
      sprintf(aircraft, "N%03dD", Aircraft);
    }

  InitAircraftSpecs(AIRCRAFT);
  boomln = atof(GetAircraftParameter(aircraft, "BOOM_LEN"));
  ReleaseAircraftSpecs();

  return(boomln);

}	/* END GETBOOMLENGTH */

/* END BOOMLN.C */

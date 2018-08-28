/*
-------------------------------------------------------------------------
OBJECT NAME:	ggps.c

FULL NAME:

ENTRY POINTS:	sgsvew()
		sgsvns()

STATIC FNS:	none

DESCRIPTION:	Compute east and north components of ground speed from
		track angle and ground speed

INPUT:

OUTPUT:

REFERENCES:

REFERENCED BY:  compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 2003
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

/* -------------------------------------------------------------------- */
void sggvew(DERTBL *varp)
{ 
  NR_TYPE    gspd, trk;

  gspd = GetSample(varp, 0);
  trk = GetSample(varp, 1);
  
  PutSample(varp, (gspd * sin((double)(DEG_RAD*trk))));

}  /* END SGGVEW */

/* -------------------------------------------------------------------- */
void sggvns(DERTBL *varp)
{
  NR_TYPE  gspd, trk;

  gspd = GetSample(varp, 0);
  trk = GetSample(varp, 1);

  PutSample(varp, (gspd * cos((double)(DEG_RAD*trk))));

}  /* END SGGVNS */

/*  END GGPS.C */

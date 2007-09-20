/*
-------------------------------------------------------------------------
OBJECT NAME:	prcltrk1.c

FULL NAME:	Parcel Tracking

ENTRY POINTS:	sptdrdst(), sptdrtim(), sptdrrad()

STATIC FNS:	none

DESCRIPTION:	Parcel Tracking, Dead Reckoning.

COPYRIGHT:	University Corporation for Atmospheric Research, 1995
-------------------------------------------------------------------------
*/

#include <math.h>
#include "nimbus.h"
#include "amlib.h"

#define MAX_PTDR	5
#define CRADEG		0.017453  /* convert to radians from degrees */

static NR_TYPE	x[MAX_PTDR], y[MAX_PTDR];
static NR_TYPE	bearing[MAX_PTDR];
static NR_TYPE	timeToReturn[MAX_PTDR];

/*  Value from Jeffco Tower (303)466-3546 as of 30 April 1998  RLR  */
static NR_TYPE	decl = -10.6;  /*  (10.9 deg E as of Jan 1995)  */
/*                                 Annual rate of change 0.1 W  */

void ResetCircle(int num), ResetDeadReckoning(int), ResetLagrangian(int);

/* -------------------------------------------------------------------- */
void parcel_reset(int num)
{
  if (num >= 3)
    ResetCircle(num - 3);
  else
    ResetLagrangian(num);

}	/* END PARCEL_RESET */

/* -------------------------------------------------------------------- */
void deadReckInit(var_base *varp)
{
  NR_TYPE  *tmp;
  if ((tmp = GetDefaultsValue("DECLINATION", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function deadReckInit.\n", decl);
    LogMessage(buffer);
  }
  else
    decl = tmp[0];
/*  decl = (GetDefaultsValue("DECLINATION", varp->name))[0];  <-- original code */

}	/* END DEADRECKINIT */

/* -------------------------------------------------------------------- */
void ResetDeadReckoning(int num)
{
	x[num] = y[num] = 0.0;

}	/* END RESETDEADRECKONING */

/* -------------------------------------------------------------------- */
void sptdrdst(DERTBL *varp)
{
	int		indx;
	NR_TYPE		tasx, thdg, sslip, d;

	tasx = GetSample(varp, 0);
	thdg = GetSample(varp, 1) * CRADEG;
	sslip = GetSample(varp, 2) * CRADEG;

	indx = varp->ProbeCount;

	x[indx] += tasx * 0.00054 * sin(thdg - sslip);
	y[indx] += tasx * 0.00054 * cos(thdg - sslip);

	d = sqrt(x[indx]*x[indx] + y[indx]*y[indx]);
	bearing[indx] = atan2(x[indx], y[indx]) * 180.0 / M_PI + 180.0 - decl;
	timeToReturn[indx] = (d / 0.00054) / (60.0 * tasx);


	if (bearing[indx] < 0.0)
		bearing[indx] += 360.0;
	else
	if (bearing[indx] > 360.0)
		bearing[indx] -= 360.0;
		
	PutSample(varp, d);

}	/* END SPTDRDST */

/* -------------------------------------------------------------------- */
void sptdrrad(DERTBL *varp)
{
	PutSample(varp, bearing[varp->ProbeCount]);

}	/* END SPTDRRAD */

/* -------------------------------------------------------------------- */
void sptdrtim(DERTBL *varp)
{
	PutSample(varp, timeToReturn[varp->ProbeCount]);

}	/* END SPTDRRAD */

/* END PRCLTRK1.C */

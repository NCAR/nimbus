/*
-------------------------------------------------------------------------
OBJECT NAME:	ACEASIA.c

FULL NAME:	ACE-Asis

ENTRY POINTS:	sfunc()

STATIC FNS:  

DESCRIPTION:	Template for AMLIB std & user directories.

INPUT:    

OUTPUT:    

REFERENCES:  

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 2001
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static NR_TYPE pof_cals[4][4] =	{ 27.540, 0.0390, 4.0e-5, 5.62e-8, 
				 6.0, 0.00850, 8.74e-6, 1.21e-8,
				 1.358, 0.00191, 1.98e-6, 2.78e-9,
				 0.385, 0.000545, 5.6e-7, 7.91e-10 };

/* -------------------------------------------------------------------- */
void initACEAsia()
{

}

/* -------------------------------------------------------------------- */
void sLynn(DERTBL *varp)
{
  int		idx = varp->name[7] - '1';
  NR_TYPE  pres, tmp, out, pres2;

  pres = GetSample(varp, 0);
  tmp = GetSample(varp, 1);

  if (idx > 2)
    idx = 3;

  pres2 = pres * pres;

  out = pof_cals[idx][0] * (pres / tmp) + pof_cals[idx][1] * pres -
	pof_cals[idx][2] * pres2/tmp - pof_cals[idx][3]* pres2;

  PutSample(varp, out);

}  /* END SFUNC */

/* END ACEASIA.C */

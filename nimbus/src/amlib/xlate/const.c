/*
-------------------------------------------------------------------------
OBJECT NAME:	const.c

FULL NAME:	Constants

ENTRY POINTS:	none

STATIC FNS:	none

DESCRIPTION:	Globals file containing misc constants used by multiple
		routines.

COPYRIGHT:	University Corporation for Atmospheric Research, 1996
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

/* -------------------------------------------------------------------- */
NR_TYPE FirstPoly(NR_TYPE x, NR_TYPE c[])
{
  return(c[0] + (x * c[1]));

}

/* -------------------------------------------------------------------- */
NR_TYPE SecondPoly(NR_TYPE x, NR_TYPE c[])
{
  return(c[0] + x * (c[1] + x * c[2]));

}

/* END CONST.C */

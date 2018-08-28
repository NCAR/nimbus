/*
-------------------------------------------------------------------------
OBJECT NAME:	template.c

FULL NAME:	

ENTRY POINTS:	xletorq()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1995
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void xlNOzone(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = ((Ozone_blk *)input)->ozone;

}	/* END XLETORQ */

/* -------------------------------------------------------------------- */
void xlJplTdl(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = ((Jpltdl_blk *)input)->vmr;

}	/* END XLETORQ */

/* -------------------------------------------------------------------- */
void xllah2o(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = ((Noaa_blk *)input)->lah2o;

}	/* END XLETORQ */

/* -------------------------------------------------------------------- */
void xlfph0(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = ((Noaa_blk *)input)->fph0;

}	/* END XLETORQ */

/* -------------------------------------------------------------------- */
void xlfph1(RAWTBL *varp, void *input, NR_TYPE *output)
{
  *output = ((Noaa_blk *)input)->fph1;

}	/* END XLETORQ */

/* END ETORQ.C */

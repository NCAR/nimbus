/*
-------------------------------------------------------------------------
OBJECT NAME:	gpsPps.c

FULL NAME:	Convert Collins PPS GPS block data

ENTRY POINTS:	xlgptime(), xlgplat(), xlgplon(), xlgpalt(), xlgpvns(),
		xlgpvew(), xlgpvspd()

DESCRIPTION:	Collins GPS data is stored in DEC floating point.

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1996
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void xlgptime(RAWTBL *varp, void *p, NR_TYPE *np)
{
  *np = ((Ppsgps_blk *)p)->utc_time;

}	/* END XLGPTIME */

/* -------------------------------------------------------------------- */
void xlgplat(RAWTBL *varp, void *p, NR_TYPE *np)
{
	*np = ((Ppsgps_blk *)p)->lat / 4.0 * RAD_DEG;

}	/* END XLGPLAT */

/* -------------------------------------------------------------------- */
void xlgplon(RAWTBL *varp, void *p, NR_TYPE *np)
{
	*np = ((Ppsgps_blk *)p)->lon / 4.0 * RAD_DEG;

}	/* END XLGPLON */

/* -------------------------------------------------------------------- */
void xlgpalt(RAWTBL *varp, void *p, NR_TYPE *np)
{
	*np = ((Ppsgps_blk *)p)->alt_msl / 4.0;

}	/* END XLGPALT */

/* -------------------------------------------------------------------- */
void xlgpvns(RAWTBL *varp, void *p, NR_TYPE *np)
{
	*np = ((Ppsgps_blk *)p)->vel_north / 4.0;

}	/* END XLGPVNS */

/* -------------------------------------------------------------------- */
void xlgpvew(RAWTBL *varp, void *p, NR_TYPE *np)
{
	*np = ((Ppsgps_blk *)p)->vel_east / 4.0;

}	/* END XLGPVEW */

/* -------------------------------------------------------------------- */
void xlgpvspd(RAWTBL *varp, void *p, NR_TYPE *np)
{
	*np = ((Ppsgps_blk *)p)->vel_up / 4.0;

}	/* END XLGPVSPD */

/* END GPSPPS.C */

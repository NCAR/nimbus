/*
-------------------------------------------------------------------------
OBJECT NAME:	gpsm.c

FULL NAME:	Translate Magnavox GPS block

ENTRY POINTS:	xlgmtime()

DESCRIPTION:	

INPUT:		GPS block pointer

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void xlgmtime(RAWTBL *varp, void *input, NR_TYPE *output)
{
	char	*gpstime = (char *)input;

	*output = (10000 * (gpstime[0] * 10 + gpstime[1])) +
		  (100 * (gpstime[2] * 10 + gpstime[3])) +
		  (gpstime[4] * 10 + gpstime[5]);
}

/* -------------------------------------------------------------------- */
void xlgmlat(RAWTBL *varp, void *input, NR_TYPE *output)
{
        *output = (NR_TYPE)((GpsMag_blk *)input)->glat;
}

/* -------------------------------------------------------------------- */
void xlgmlon(RAWTBL *varp, void *input, NR_TYPE *output)
{
        *output = (NR_TYPE)((GpsMag_blk *)input)->glon;
}

/* -------------------------------------------------------------------- */
void xlgmalt(RAWTBL *varp, void *input, NR_TYPE *output)
{
        *output = (NR_TYPE)((GpsMag_blk *)input)->galt;
}

/* -------------------------------------------------------------------- */
void xlgmgeoh(RAWTBL *varp, void *input, NR_TYPE *output)
{
        *output = (NR_TYPE)((GpsMag_blk *)input)->geo_height;
}

/* -------------------------------------------------------------------- */
void xlgmvew(RAWTBL *varp, void *input, NR_TYPE *output)
{
        *output = (NR_TYPE)((GpsMag_blk *)input)->veleast;
}

/* -------------------------------------------------------------------- */
void xlgmvns(RAWTBL *varp, void *input, NR_TYPE *output)
{
        *output = (NR_TYPE)((GpsMag_blk *)input)->velnrth;
}

/* -------------------------------------------------------------------- */
void xlgmmode(RAWTBL *varp, void *input, NR_TYPE *output)
{
        *output = (NR_TYPE)((GpsMag_blk *)input)->gpsmode;
}

/* END GPSM.C */

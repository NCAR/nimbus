/*
-------------------------------------------------------------------------
OBJECT NAME:	fssp.c

FULL NAME:	Translate PMS1D to new record format (including Version 2)

ENTRY POINTS:	xlfreset(), xlfrange(), xlfstrob()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void xlfreset(RAWTBL *varp, void *p, NR_TYPE *np)
{
  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (NR_TYPE)ntohs(((P16_blk *)p)[i].aux[3]);

}	/* END XLFRESET */

/* -------------------------------------------------------------------- */
void xlfrange(RAWTBL *varp, void *p, NR_TYPE *np)
{
  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    np[i] = (NR_TYPE)ntohs(((P16_blk *)p)[i].rng_flg);

    if (np[i] < 0.0 || np[i] > 3.0)
      np[i] = 0.0;
    }

}	/* END XLFRANGE */

/* -------------------------------------------------------------------- */
void xlfrange2(RAWTBL *varp, void *p, NR_TYPE *np)
{
  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    np[i] = (NR_TYPE)ntohs(((P16v2_blk *)p)[i].rng_flg);

    if (np[i] < 0.0 || np[i] > 3.0)
      np[i] = 0.0;
    }

}	/* END XLFRANGE2 */

/* -------------------------------------------------------------------- */
void xlfrange3(RAWTBL *varp, void *p, NR_TYPE *np)
{
  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    np[i] = (NR_TYPE)ntohs(((DMT100_blk *)p)[i].range);

    if (np[i] < 0.0 || np[i] > 3.0)
      np[i] = 0.0;
    }

}	/* END XLFRANGE3 */

/* -------------------------------------------------------------------- */
void xlfstrob(RAWTBL *varp, void *p, NR_TYPE *np)
{
  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (NR_TYPE)ntohs(((P16_blk *)p)[i].aux[2]);

}	/* END XLFSTROB */

/* END FSSP.C */

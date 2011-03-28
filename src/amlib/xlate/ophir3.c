/*
-------------------------------------------------------------------------
OBJECT NAME:	ophir3.c

FULL NAME:	Translate Ophir III data block

ENTRY POINTS:	xlogain(), xlobbcod(), xlodetdc(), xlotsnt(), xlotbnch(),
		xlotdet(), xlotbbc(), xlotbbf(), xlosmotr(), xlodetsg(),
		xlotbox()

STATIC FNS:	none

DESCRIPTION:	
	Valid black body codes are as follows:

        "b" = 98        normal operation
        "z" = 122       detector zero
        "c" = 99        black body full cool cycle
        "h" = 104       black body ful heat cycle

  OGAIN          electronic gain, 1, 2, or 3
  OBBCOD         blackbody code, see above
  ODETDC         detector dc bias
  OTSNT          optical snout temperature
  OTBNCH         optical bench temperature
  OTDET          HgCdTe detector temperature
  OTBBC          cooled blackbody temperature
  OTBBF          floating blackbody temperature
  OSMOTR         motor speed
  OTBOX          detector signal
  ODETSG         electronics box temperature


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
void xlogain(RAWTBL *varp, void *p, NR_TYPE *np)
{
  if (varp->cof.size() == 3)
    *np = (NR_TYPE)SecondPoly((NR_TYPE)ntohs(((Oph3_blk *)p)->gain), varp->cof);
  else
    *np = (NR_TYPE)FirstPoly((NR_TYPE)ntohs(((Oph3_blk *)p)->gain), varp->cof);

}	/* END XLOGAIN */

/* -------------------------------------------------------------------- */
void xlobbcod(RAWTBL *varp, void *p, NR_TYPE *np)
{
  *np = (NR_TYPE)((Oph3_blk *)p)->bb_pos[0];

}	/* END XLOBBCOD */

/* -------------------------------------------------------------------- */
void xlodetdc(RAWTBL *varp, void *p, NR_TYPE *np)
{
  if (varp->cof.size() == 3)
    *np = (NR_TYPE)SecondPoly((NR_TYPE)ntohs(((Oph3_blk *)p)->det_bias), varp->cof);
  else
    *np = (NR_TYPE)FirstPoly((NR_TYPE)ntohs(((Oph3_blk *)p)->det_bias), varp->cof);

}	/* END XLODETDC */

/* -------------------------------------------------------------------- */
void xlotsnt(RAWTBL *varp, void *p, NR_TYPE *np)
{
  if (varp->cof.size() == 3)
    *np = (NR_TYPE)SecondPoly((NR_TYPE)ntohs(((Oph3_blk *)p)->snout_tmp), varp->cof);
  else
    *np = (NR_TYPE)FirstPoly((NR_TYPE)ntohs(((Oph3_blk *)p)->snout_tmp), varp->cof);

}	/* END XLOTSNT */

/* -------------------------------------------------------------------- */
void xlotbnch(RAWTBL *varp, void *p, NR_TYPE *np)
{
  if (varp->cof.size() == 3)
    *np = (NR_TYPE)SecondPoly((NR_TYPE)ntohs(((Oph3_blk *)p)->bench_tmp), varp->cof);
  else
    *np = (NR_TYPE)FirstPoly((NR_TYPE)ntohs(((Oph3_blk *)p)->bench_tmp), varp->cof);

}	/* END XLOTBNCH */

/* -------------------------------------------------------------------- */
void xlotdet(RAWTBL *varp, void *p, NR_TYPE *np)
{
  if (varp->cof.size() == 3)
    *np = (NR_TYPE)SecondPoly((NR_TYPE)ntohs(((Oph3_blk *)p)->det_tmp), varp->cof);
  else
    *np = (NR_TYPE)FirstPoly((NR_TYPE)ntohs(((Oph3_blk *)p)->det_tmp), varp->cof);

}	/* END XLOTDET */

/* -------------------------------------------------------------------- */
void xlotbbc(RAWTBL *varp, void *p, NR_TYPE *np)
{
  if (varp->cof.size() == 3)
    *np = (NR_TYPE)SecondPoly((NR_TYPE)ntohs(((Oph3_blk *)p)->cool_tmp), varp->cof);
  else
    *np = (NR_TYPE)FirstPoly((NR_TYPE)ntohs(((Oph3_blk *)p)->cool_tmp), varp->cof);

}	/* END XLOTBBC */

/* -------------------------------------------------------------------- */
void xlotbbf(RAWTBL *varp, void *p, NR_TYPE *np)
{
  if (varp->cof.size() == 3)
    *np = (NR_TYPE)SecondPoly((NR_TYPE)ntohs(((Oph3_blk *)p)->float_tmp), varp->cof);
  else
    *np = (NR_TYPE)FirstPoly((NR_TYPE)ntohs(((Oph3_blk *)p)->float_tmp), varp->cof);

}	/* END XLOTBBF */

/* -------------------------------------------------------------------- */
void xlosmotr(RAWTBL *varp, void *p, NR_TYPE *np)
{
  if (varp->cof.size() == 3)
    *np = (NR_TYPE)SecondPoly((NR_TYPE)ntohs(((Oph3_blk *)p)->motor_spd), varp->cof);
  else
    *np = (NR_TYPE)FirstPoly((NR_TYPE)ntohs(((Oph3_blk *)p)->motor_spd), varp->cof);

}	/* END XLOSMOTR */

/* -------------------------------------------------------------------- */
void xlodetsg(RAWTBL *varp, void *p, NR_TYPE *np)
{
  if (varp->cof.size() == 3)
    *np = (NR_TYPE)SecondPoly((NR_TYPE)ntohs(((Oph3_blk *)p)->det_sig), varp->cof);
  else
    *np = (NR_TYPE)FirstPoly((NR_TYPE)ntohs(((Oph3_blk *)p)->det_sig), varp->cof);

}	/* END XLDETSG */

/* -------------------------------------------------------------------- */
void xlotbox(RAWTBL *varp, void *p, NR_TYPE *np)
{
  if (varp->cof.size() == 3)
    *np = (NR_TYPE)SecondPoly((NR_TYPE)ntohs(((Oph3_blk *)p)->box_tmp), varp->cof);
  else
    *np = (NR_TYPE)FirstPoly((NR_TYPE)ntohs(((Oph3_blk *)p)->box_tmp), varp->cof);

}	/* END XLOTBOX */

/* END OPHIR3.C */

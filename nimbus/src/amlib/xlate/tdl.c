/*
-------------------------------------------------------------------------
OBJECT NAME:	float.c

FULL NAME:	Generic float array decoding

ENTRY POINTS:	xlfloat*()

STATIC FNS:	none

DESCRIPTION:	JPLTDL & LHTDL (Bruce Gandrud)

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1999-2000
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void xllh_ppmv(RAWTBL *varp, void *input, NR_TYPE *output)
{
  LhTdl_blk	*p = (LhTdl_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    output[i] = ntohf(p->tdllh[i].ppmv);

}	/* END XLFLOAT0 */

/* -------------------------------------------------------------------- */
void xllh_vmr(RAWTBL *varp, void *input, NR_TYPE *output)
{
  LhTdl_blk	*p = (LhTdl_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    output[i] = ntohf(p->tdllh[i].vmr);

}	/* END XLFLOAT1 */

/* -------------------------------------------------------------------- */
void xllh_relhum(RAWTBL *varp, void *input, NR_TYPE *output)
{
  LhTdl_blk	*p = (LhTdl_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    output[i] = ntohf(p->tdllh[i].relhum);

}	/* END XLFLOAT2 */

/* -------------------------------------------------------------------- */
void xllh_press(RAWTBL *varp, void *input, NR_TYPE *output)
{
  LhTdl_blk	*p = (LhTdl_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    output[i] = ntohf(p->tdllh[i].press);

}	/* END XLFLOAT3 */

/* -------------------------------------------------------------------- */
void xllh_temp(RAWTBL *varp, void *input, NR_TYPE *output)
{
  LhTdl_blk	*p = (LhTdl_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    output[i] = ntohf(p->tdllh[i].temp);

}	/* END XLFLOAT4 */

/* -------------------------------------------------------------------- */
void xllh_pp2f(RAWTBL *varp, void *input, NR_TYPE *output)
{
  LhTdl_blk	*p = (LhTdl_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    output[i] = ntohf(p->tdllh[i].pp2f);

}	/* END XLFLOAT5 */

/* -------------------------------------------------------------------- */
void xllh_Pv0(RAWTBL *varp, void *input, NR_TYPE *output)
{
  LhTdl_blk	*p = (LhTdl_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    output[i] = ntohf(p->tdllh[i].Pv0);

}	/* END XLFLOAT6 */

/* -------------------------------------------------------------------- */
void xllh_indexmax(RAWTBL *varp, void *input, NR_TYPE *output)
{
  LhTdl_blk	*p = (LhTdl_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    output[i] = ntohf(p->tdllh[i].Indexmax);

}	/* END XLFLOAT7 */

/* -------------------------------------------------------------------- */
void xllh_yzero(RAWTBL *varp, void *input, NR_TYPE *output)
{
  LhTdl_blk	*p = (LhTdl_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    output[i] = ntohf(p->tdllh[i].Yzero);

}	/* END XLFLOAT8 */

/* END TDL.C */

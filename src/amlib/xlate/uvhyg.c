/*
-------------------------------------------------------------------------
OBJECT NAME:	uvhyg.c

FULL NAME:	Translate UV Hygrometer data block

ENTRY POINTS:	xluvhsmp(), xluvhref(), xluvkrsp(), xluvkrrf(), xluvchsp(),
		xluvchrf(), xluvpvol(), xluvtvol(), xlmruv

DESCRIPTION:	
		UVHSMP         hydrogen sample
		UVHREF         hydrogen reference
		UVKRSP         krypton sample
		UVKRRF         krypton reference
		UVCHSP         chopper sample
		UVCHRF         chopper reference
		UVPVOL         sample volume pressure
		UVTVOL         sample volume temperature
		MRUV           calculated humidity

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void xluvhsmp(RAWTBL *varp, const void *p, NR_TYPE *np)
{
  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = ntohf(((Uv_blk *)p)->uvsmp[i].hydr_smp);
}

/* -------------------------------------------------------------------- */
void xluvhref(RAWTBL *varp, const void *p, NR_TYPE *np)
{
  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = ntohf(((Uv_blk *)p)->uvsmp[i].hydr_ref);
}

/* -------------------------------------------------------------------- */
void xluvkrsp(RAWTBL *varp, const void *p, NR_TYPE *np)
{
  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = ntohf(((Uv_blk *)p)->uvsmp[i].kryp_smp);
}

/* -------------------------------------------------------------------- */
void xluvkrrf(RAWTBL *varp, const void *p, NR_TYPE *np)
{
  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = ntohf(((Uv_blk *)p)->uvsmp[i].kryp_ref);
}

/* -------------------------------------------------------------------- */
void xluvchsp(RAWTBL *varp, const void *p, NR_TYPE *np)
{
  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = ntohf(((Uv_blk *)p)->uvsmp[i].chop_smp);
}

/* -------------------------------------------------------------------- */
void xluvchrf(RAWTBL *varp, const void *p, NR_TYPE *np)
{
  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = ntohf(((Uv_blk *)p)->uvsmp[i].chop_ref);
}

/* -------------------------------------------------------------------- */
void xluvpvol(RAWTBL *varp, const void *p, NR_TYPE *np)
{
  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = ntohf(((Uv_blk *)p)->uvsmp[i].pressure);
}

/* -------------------------------------------------------------------- */
void xluvtvol(RAWTBL *varp, const void *p, NR_TYPE *np)
{
  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = ntohf(((Uv_blk *)p)->uvsmp[i].temp);
}

/* -------------------------------------------------------------------- */
void xlmruv(RAWTBL *varp, const void *p, NR_TYPE *np)
{
  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = ntohf(((Uv_blk *)p)->uvsmp[i].humidity);
}

/* END UVHYG.C */

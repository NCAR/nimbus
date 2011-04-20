/*
-------------------------------------------------------------------------
OBJECT NAME:  flux.c

FULL NAME:  Two minute running fluxes

ENTRY POINTS:  sflux()

STATIC FNS:  

DESCRIPTION:  Flux computations.

COPYRIGHT:  University Corporation for Atmospheric Research, 1994
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

#define MAX_FLUXES  8


/*  Value from /home/local/proj/Defaults on 23 April 1998           RLR */
static int  nSeconds = 120;

static NR_TYPE  *wi_buffer, *x_buffer[MAX_FLUXES];
static NR_TYPE  wi_sum, wi2_sum, x_sum[MAX_FLUXES], x2_sum[MAX_FLUXES],
		wix_sum[MAX_FLUXES], wx[MAX_FLUXES], wi_bar, prev_wi;

/* -------------------------------------------------------------------- */
void initFlux(DERTBL *varp)
{
  int    i;
  static bool  firstTime = TRUE;
  static int  pCnt = 0;

  if (firstTime)
  {
    float *tmp;

    if ((tmp = GetDefaultsValue("FLUX_RUN", varp->name)) == NULL)
    {
      sprintf(buffer,"Value set to %d in AMLIB function initFlux.\n", nSeconds);
      LogMessage(buffer);
    }
    else
      nSeconds = (int)tmp[0];

    wi_buffer = new NR_TYPE[nSeconds];
    wi_sum = wi2_sum = 0.0;

    for (i = 0; i < nSeconds; ++i)
      wi_buffer[i] = 0.0;

    for (i = 0; i < MAX_FLUXES; ++i)
      x_sum[i] = x2_sum[i] = wix_sum[i] = 0.0;

    firstTime = FALSE;
  }

  if (pCnt >= MAX_FLUXES)
  {
    fprintf(stderr, "flux.c: MAX_FLUXES exceeded, fatal.\n");
    exit(1);
  }

  x_buffer[pCnt] = new NR_TYPE[nSeconds];

  for (i = 0; i < nSeconds; ++i)
    x_buffer[pCnt][i] = 0.0;

  ++pCnt;

}  /* END INITFLUX */

/* -------------------------------------------------------------------- */
void sflux_xx(DERTBL *varp)
{
  int    probeCount;
  NR_TYPE  xx, second, wi, x, prev_x;
  NR_TYPE  *x_buff, x_bar, x2_bar;

  static int  buffIndex = 0;
  static NR_TYPE  prev_second = -1;

  second= GetSample(varp, 0);
  wi  = GetSample(varp, 1);
  x  = GetSample(varp, 2);


  if (second != prev_second)
  {
    if (++buffIndex >= nSeconds)
      buffIndex = 0;

    prev_second = second;
  }

  probeCount = varp->ProbeCount;

  x_buff = x_buffer[probeCount];

  prev_x = x_buff[buffIndex];
  x_buff[buffIndex] = x;

  x_sum[probeCount] -= prev_x;
  x_sum[probeCount] += x;

  x_bar = x_sum[probeCount] / nSeconds;

  x2_sum[probeCount] -= prev_x * prev_x;
  x2_sum[probeCount] += x * x;

  x2_bar = x2_sum[probeCount] / nSeconds;
  xx = x2_bar - (x_bar * x_bar);

  wix_sum[probeCount] -= prev_wi * prev_x;
  wix_sum[probeCount] += wi * x;

  wx[probeCount] = (wix_sum[probeCount] / nSeconds) - (wi_bar * x_bar);

  PutSample(varp, xx);

}  /* END SFLUX */

/* -------------------------------------------------------------------- */
void sflux_ww(DERTBL *varp)
{
  NR_TYPE  wi, ww;

  static int  buffIndex = 0;
  static NR_TYPE  wi2_bar;

  wi = GetSample(varp, 0);

  if (++buffIndex >= nSeconds)
    buffIndex = 0;

  prev_wi = wi_buffer[buffIndex];
  wi_buffer[buffIndex] = wi;
  wi_sum -= prev_wi;
  wi_sum += wi;

  wi2_sum -= prev_wi * prev_wi;
  wi2_sum += wi * wi;

  wi_bar = wi_sum / nSeconds;
  wi2_bar = wi2_sum / nSeconds;

  ww = wi2_bar - (wi_bar * wi_bar);

  PutSample(varp, ww);

}  /* END FLX-WW */

/* -------------------------------------------------------------------- */
void sflux_wx(DERTBL *varp)
{
  PutSample(varp, wx[varp->ProbeCount]);

}  /* END SFLUX_WX */

/* END FLUX.C */

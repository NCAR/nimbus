/*
-------------------------------------------------------------------------
OBJECT NAME:	rhouv.c

FULL NAME:	Absolute humidity (RAF fast) (g/m3)

ENTRY POINTS:	srhouv()

STATIC FNS:	none

DESCRIPTION:  

COPYRIGHT:	University Corporation for Atmospheric Research, 2007
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static NR_TYPE cx_1[4] = { 0.5, 0.0, 0.0, 0.0 };
static NR_TYPE  *CX_1;

/* -------------------------------------------------------------------- */
void rhouvInit(var_base *varp)
{
  NR_TYPE	*tmp;

  if ((tmp = GetDefaultsValue("RHOUV_CAL", varp->name)) == NULL)
  {
    CX_1 = cx_1;
    sprintf(buffer, "Values returned = %f, %f, %f in AMLIB function RHOUV_CAL.\n", CX_1[0], CX_1[1], CX_1[2]);
    LogMessage(buffer);
  }
  else
    CX_1 = tmp;
}

/* -------------------------------------------------------------------- */
void srhouv(DERTBL *varp)
{
  NR_TYPE xuvi = GetSample(varp, 0);
  NR_TYPE rhodt = GetSample(varp, 1);
  NR_TYPE dpxc = GetSample(varp, 2);
  NR_TYPE atx = GetSample(varp, 3);
  NR_TYPE rhouv, prev_uv, diff;

  static const int nSeconds = 10;
  static int buffIndex = 0;
  static NR_TYPE uv_buffer[nSeconds], uv_sum;

  rhouv = xuvi;
  diff = (rhodt - xuvi);
  if (diff < -0.5)
    diff = -0.5;
  if (diff > 0.5)
    diff = 0.25;

  if (dpxc < atx)
  {
    if (++buffIndex >= nSeconds)
      buffIndex = 0;

    prev_uv = uv_buffer[buffIndex];
    uv_buffer[buffIndex] = diff;
    uv_sum -= prev_uv;
    uv_sum += diff;
  }

  rhouv = rhouv +  CX_1[0] * (uv_sum / nSeconds);

  if (rhouv < 0.01)
    rhouv = 0.01;

  PutSample(varp, rhouv);
}

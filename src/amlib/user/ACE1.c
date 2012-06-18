/*
-------------------------------------------------------------------------
OBJECT NAME:  func.c

FULL NAME:  

DESCRIPTION:  Template for AMLIB std & user directories.

COPYRIGHT:  University Corporation for Atmospheric Research, 1995, 1998
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

/*  Values from /home/local/proj/130/Defaults on 29 April 1998     RLR  */
static NR_TYPE cndiv_l[3] = {1027.0, 30.0, 1562.0};  /*  LRT processing */
static NR_TYPE cndiv_h[3] = {41.08, 1.20, 62.48};    /*  HRT processing */
static NR_TYPE *CNDIV;

/*  Note:  It may be prudent to change the code to automatically handle
           both low and high-rate data without having different values
           for CNDIV.  (The LRT version is 25x the HRT.)
           RLR  19980430  */

/* -------------------------------------------------------------------- */
void initCNs()
{
  float *tmp;
  if ((tmp = GetDefaultsValue("CNDIV")) == NULL)
  {
    if (ProcessingRate == LOW_RATE)
	  CNDIV = cndiv_l;
    else
	  CNDIV = cndiv_h;
    sprintf(buffer, "Value set to %f in AMLIB function initCNs.\n", CNDIV);
    LogMessage(buffer);
  }
  else
    CNDIV = tmp;
/*  CNDIV = GetDefaultsValue("CNDIV"); <--  original code */

}

/* -------------------------------------------------------------------- */
void sdiv1(DERTBL *varp)
{
  NR_TYPE  x;

  x = GetSample(varp, 0);

  PutSample(varp, x / CNDIV[0] * 60);

}  /* END SFUNC */

/* -------------------------------------------------------------------- */
void sdiv2(DERTBL *varp)
{
  NR_TYPE  x;

  x = GetSample(varp, 0);

  PutSample(varp, x / CNDIV[1] * 60);

}  /* END SFUNC */

/* -------------------------------------------------------------------- */
void sdiv3(DERTBL *varp)
{
  NR_TYPE  x;

  x = GetSample(varp, 0);

  PutSample(varp, x / CNDIV[2] * 60);

}  /* END SFUNC */

/* -------------------------------------------------------------------- */
void sxfcn(DERTBL *varp)
{
  NR_TYPE  x;

  x = GetSample(varp, 0);

  PutSample(varp, 0.001312 * x + 0.034);

}  /* END SFUNC */

/* -------------------------------------------------------------------- */
void sfudge(DERTBL *varp)
{
  NR_TYPE  o3fp, o3ff, o3fv, fudge;

  o3fp = GetSample(varp, 0);
  o3ff = GetSample(varp, 1);

  fudge = floatNAN;

  if (o3fp > 1.0)
    o3fv = o3ff * 1013.25 / o3fp;
  else
    o3fv = floatNAN;  /* Missing Value  */

  if (o3fv > 0.0)
    fudge = 1.269e+18 * ((1.422e-15 / o3fv ) + (o3fv / 1.211e+20));

  PutSample(varp, fudge);

}

/* END ACE1.C */

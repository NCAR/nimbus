/*
-------------------------------------------------------------------------
OBJECT NAME:	prcltrk2.c

FULL NAME:	Parcel Tracking

ENTRY POINTS:	sptlgdst(), sptlgalt(), sptlgrad()

STATIC FNS:	none

DESCRIPTION:	Parcel Tracking, Lagrangian method.

COPYRIGHT:	University Corporation for Atmospheric Research, 1995
-------------------------------------------------------------------------
*/

#include <math.h>
#include "nimbus.h"
#include "amlib.h"

#define NPARCEL 5    /* Number of parcels handled here  */

#define CD2NMI  60.0    /* Degrees Latitude --> nmi conversion  */
#define CM2FT  3.2808398  /* Meters --> feet conversion    */
#define CM2NMI  5.396E-4  /* Meters --> nmi conversion    */
#define DEG  57.29577952  /* Radians --> degrees conversion  */
#define RAD  0.01745329


static int  reset[NPARCEL];  /* reset flags        */
static int  intvl_fill[NPARCEL];  /* one half of the INTRVL  */
static NR_TYPE  pdst[NPARCEL];  /* distance to parcel (nmi. from a/c)  */
static NR_TYPE  prad[NPARCEL];  /* angle to parcel (deg from a/c)  */
static NR_TYPE  pheight[NPARCEL];  /* altitude to parcel (ft above a/c) */

static int  ptime[NPARCEL];  /* number of seconds tracking parcel  */
static NR_TYPE  pu[NPARCEL];  /* e-w wind values at start of track  */
static NR_TYPE  pv[NPARCEL];  /* n-s wind values at start of track  */
static NR_TYPE  pw[NPARCEL];  /* vert wind values at start of track  */
static NR_TYPE  plat[NPARCEL];  /* latitude values at start of track  */
static NR_TYPE  plon[NPARCEL];  /* longitude values at start of track  */
static NR_TYPE  palt[NPARCEL];  /* altitude values at start of track  */

static NR_TYPE  *u;  /* current e-w wind values  */
static NR_TYPE  *v;  /* current n-s wind values  */
static NR_TYPE  *w;  /* current vertical wind values  */

/*  Value from /home/local/proj/defaults/Defaults on 29 April 1998  RLR  */
static int  INTRVL = 10;    /* Number of samples in mean winds  */

/* -------------------------------------------------------------------- */
void parcelInit(var_base *varp)
{
  NR_TYPE  *tmp;

  if ((tmp = GetDefaultsValue("PRCLTRK_INTRVL", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %d in AMLIB function parcelInit.\n", INTRVL);
    LogMessage(buffer);
  }
  else
    INTRVL = (int)tmp[0];

  u = new NR_TYPE[INTRVL];
  v = new NR_TYPE[INTRVL];
  w = new NR_TYPE[INTRVL];

}  /* END PARCELINIT */

/* -------------------------------------------------------------------- */
void ResetLagrangian(int num)
{
  reset[num] = 1;    /* 1 = reset, 0 = no reset  */

  /* number of points to advance before a mean wind can be computed
   * at the start of the track
   */
  intvl_fill[num] = (INTRVL+1) / 2;

}  /* PARCEL_RESET */

/* -------------------------------------------------------------------- */
void sptlgdst(DERTBL *varp)
{
  NR_TYPE  north, east;
  int  i, j, indx;

  static int  count = 0;  /* count times routine called  */

  i = varp->ProbeCount;
  indx = count % INTRVL;      /* mod of the count */

  u[indx] = GetSample(varp, 3);
  v[indx] = GetSample(varp, 4);
  w[indx] = GetSample(varp, 5);

  ptime[i]++;    /* incr tracking time */

  if (intvl_fill[i] > 0)    /* save initial posn,winds */
    {
    pu[i] = pv[i] = pw[i] = 0;

    for (j = 0; j < INTRVL; j++)
      {
      pu[i] += u[j];
      pv[i] += v[j];
      pw[i] += w[j];
      }

    pu[i] = pu[i] / INTRVL;
    pv[i] = pv[i] / INTRVL;
    pw[i] = pw[i] / INTRVL;

    intvl_fill[i]--;
    }

  if (reset[i] > 0)
    {
    plat[i] = GetSample(varp, 0);
    plon[i] = GetSample(varp, 1);
    palt[i] = GetSample(varp, 2);

    reset[i] = 0;
    ptime[i] = 0;
    }

  /* Distance, radial and elevation to parcel */
  north = (plat[i] - GetSample(varp, 0)) *
    CD2NMI + ptime[i] * pv[i] * CM2NMI;

  east = cos((double)(RAD * 0.5 * (plat[i] + GetSample(varp, 0)))) *
    (plon[i] - GetSample(varp, 1)) *
    CD2NMI + ptime[i] * pu[i] * CM2NMI;

  pdst[i] = sqrt((double)(north*north + east*east));

  if (north != 0.0 || east != 0.0)
    prad[i] = atan2((double)-east, (double)-north) * DEG + 180.0;
  else
    prad[i] = 0.0;

  pheight[i] = (palt[i] - GetSample(varp, 2)) *
      CM2FT + ptime[i] * pw[i] * CM2FT;

  count++;

  PutSample(varp, pdst[0]);

}  /* END SPTDST1 */

/* -------------------------------------------------------------------- */
void sptlgrad(DERTBL *varp)
{
  PutSample(varp, prad[0]);
}

/* -------------------------------------------------------------------- */
void sptlgalt(DERTBL *varp)
{
  PutSample(varp, pheight[0]);
}

/* END PRCLTRK2.C */

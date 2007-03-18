/*
:set ts=4
-------------------------------------------------------------------------
OBJECT NAME:    radc.c

FULL NAME:      Radiation Data Corrections

ENTRY POINTS:   radcInit(), sradcf(), sradc()

STATIC FNS:    

DESCRIPTION:    Calculate aircraft attitude corrections for hemispheric
				radiation data

INPUT:          PITCH ROLL THF SOLAZ SOLZE

OUTPUT:         RADCF RADC 

REFERENCES:    

REFERENCED BY:  compute.c

COPYRIGHT:      University Corporation for Atmospheric Research, 1997, 1998

ORIG. AUTHOR:   Ron Ruth
-------------------------------------------------------------------------
 */

#include <stdio.h>
#include <math.h>
#include "nimbus.h"
#include "amlib.h"
#include "decode.h"

static NR_TYPE  PMAX =  6.0;  /* radiation corr. pitch max threshold (deg) */
static NR_TYPE  PMIN = -6.0;  /* radiation corr. pitch min threshold (deg) */
static NR_TYPE  RMAX =  6.0;  /* radiation corr. roll max threshold (deg) */
static NR_TYPE  RMIN = -6.0;  /* radiation corr. roll min threshold (deg) */
static NR_TYPE  SWDIFF = 0.34;  /* Short-wave diffuse rad. fraction (none) */
static NR_TYPE  SWDIRF = 0.66;  /* Short-wave direct rad. fraction (none) */
/* const double  DEG_RAD = M_PI/180.0;  * in file "const.c" */

static NR_TYPE  radcf;  /* radiation correction factor (none) */

/* -------------------------------------------------------------------- */
void radcInit(var_base *varp)
{
  NR_TYPE  *tmp;
  if ((tmp = GetDefaultsValue("RAD_PITCH_MAX", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function radcInit.\n", PMAX);
    LogMessage(buffer);
  }
  else
    PMAX = tmp[0];

  if ((tmp = GetDefaultsValue("RAD_PITCH_MIN", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function radcInit.\n", PMIN);
    LogMessage(buffer);
  }
  else
    PMIN = tmp[0];

  if ((tmp = GetDefaultsValue("RAD_ROLL_MAX", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function radcInit.\n", RMAX);
    LogMessage(buffer);
  }
  else
    RMAX = tmp[0];

  if ((tmp = GetDefaultsValue("RAD_ROLL_MIN", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function radcInit.\n", RMIN);
    LogMessage(buffer);
  }
  else
    RMIN = tmp[0];

  if ((tmp = GetDefaultsValue("SWDIFF", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function radcInit.\n", SWDIFF);
    LogMessage(buffer);
  }
  else
    SWDIFF = tmp[0];

  if ((tmp = GetDefaultsValue("SWDIRF", varp->name)) == NULL)
  {
    sprintf(buffer, "Value set to %f in AMLIB function radcInit.\n", SWDIRF);
    LogMessage(buffer);
  }
  else
    SWDIRF = tmp[0];

/*  Temporary diagnostic output 
  printf (" default values used by radc.c:\n");
  printf ("   RAD_PITCH_MAX = %f\n",PMAX);
  printf ("   RAD_PITCH_MIN = %f\n",PMIN);
  printf ("    RAD_ROLL_MAX = %f\n",RMAX);
  printf ("    RAD_ROLL_MIN = %f\n",RMIN);
  printf ("          SWDIFF = %f\n",SWDIFF);
  printf ("          SWDIRF = %f\n",SWDIRF);
 */
 
}  /* END RADCINIT  */

/* -------------------------------------------------------------------- */
void sradcf(DERTBL *varp)
/*  Radiation correction factor */
{

  double  pitch, roll, thdg, solze, solaz;
  double  tr, tp, tz, amt, cf1, cf2;

  pitch = (double)GetSample(varp, 0);      /* Aircraft pitch (deg) */
  roll  = (double)GetSample(varp, 1);      /* Aircraft roll (deg) */
  thdg  = (double)GetSample(varp, 2)*DEG_RAD;  /* Aircraft true heading (rad)*/
  solze = (double)GetSample(varp, 3);      /* Solar zenith angle (rad) */
  solaz = (double)GetSample(varp, 4);      /* Solar azimuth angle (rad) */

/* Check pitch and roll thresholds */
  if ( pitch > (double)PMAX || roll > (double)RMAX ||
     pitch < (double)PMIN || roll < (double)RMIN )
  {
    radcf = floatNAN;
  }
  else
  {
/* Aircraft attitude correction factor */
      tr  = tan((roll)*DEG_RAD);
      tp  = tan((pitch)*DEG_RAD);
    tz  = tan(solze);
/*  Transform solar azimuth into aircraft coordinates */
      amt = M_PI - solaz - thdg;
    cf1 = sqrt(1.0 + tr*tr + tp*tp);
    cf2 =  1.0 + tr*tz*sin(amt) - tp*tz*cos(amt);
    if (cf2 != 0.0)
      radcf = (NR_TYPE)(cf1/cf2);
    else
      radcf = floatNAN;
  }
  PutSample(varp, radcf);
/*  printf (" PutSample radiation correction factor = %f\n",radcf); */

}   /* END RADCF  */

/* -------------------------------------------------------------------- */
void sradc(DERTBL *varp)
{

  NR_TYPE rad;  /* Uncorrected radiation value */

/* Output a corrected value only of radcf is valid. */
  if (!isnan(radcf))
  {
    rad = GetSample(varp, 0);
    PutSample(varp, (rad*SWDIFF + rad*SWDIRF*radcf));
  }
  else
  {
    PutSample(varp, floatNAN);
  }

}  /* END SRADC */

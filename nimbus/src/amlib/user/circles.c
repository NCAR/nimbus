/*
-------------------------------------------------------------------------
OBJECT NAME:  circles.c

FULL NAME:  Flying circles

ENTRY POINTS:  sptdrdst(), sptdrtim(), sptdrrad()

STATIC FNS:  none

DESCRIPTION:  How to fly a large circle.

INPUT:    

OUTPUT:    

REFERENCES:  none

REFERENCED BY:  compute.c

COPYRIGHT:  University Corporation for Atmospheric Research, 1995,1998
-------------------------------------------------------------------------
*/

#include <math.h>
#include "nimbus.h"
#include "amlib.h"

#define MAX_CIRCLE  3

static int  reset = TRUE;

static NR_TYPE  heading,      /* Current heading shoud be    */
                neg_heading,  /* Opposite direction      */
                distance,     /* Distance of circumference covered  */
                thdgAccum,     /* Distance of circumference covered  */
                Circles[MAX_CIRCLE], circleSize;
/*  Values from /home/local/proj/defaults/Defaults on 29 April 1998  RLR  */
static NR_TYPE  Mins[MAX_CIRCLE] = {40, 50, 60};

/* -------------------------------------------------------------------- */
void initCircles(DERTBL *varp)
{
  int  i;
  NR_TYPE  *mins;
  NR_TYPE  *tmp;

  if ((tmp = GetDefaultsValue("CIRCLES", varp->name)) == NULL)
  {
    mins = Mins;
    sprintf(buffer, "Values set to %f, %f, %f in AMLIB function initCircles.\n", mins[0], mins[1], mins[2]);
    LogMessage(buffer);
  }
  else
    mins = tmp;
/*  mins = GetDefaultsValue("CIRCLES", varp->name);  <-- original code */

  for (i = 0; i < MAX_CIRCLE; ++i)
    Circles[i] = 360.0 / (mins[i] * 60.0);

}  /* END INITCIRCLES */

/* -------------------------------------------------------------------- */
void ResetCircle(int num)
{
  reset = TRUE;

/* For DYCOMS II only, remove later.
 */
{
char tbuf[128];
int i1, i2, i3;
FILE *fp = fopen("/home/local/proj/circle", "r");

fscanf(fp, "%d %d %d", &i1, &i2, &i3);
Circles[0] = 360.0 / (i1 * 60);
Circles[1] = 360.0 / (i2 * 60);
Circles[2] = 360.0 / (i3 * 60);

fclose(fp);
}

  circleSize = Circles[num];

}  /* END RESETCIRCLE */

/* -------------------------------------------------------------------- */
void scircP(DERTBL *varp)
{
  NR_TYPE  thdg;

  thdg = GetSample(varp, 0);

  if (reset)  /* It's been reset  */
    {
    heading = neg_heading = thdg;
    distance = 0.0;
    thdgAccum = 0.0;
    reset = FALSE;
    }
  else
    {
    heading = heading + circleSize;
    neg_heading = neg_heading - circleSize;
    }


  if (neg_heading < 0.0)
    neg_heading += 360.0;
  else
  if (neg_heading >= 360.0)
    neg_heading -= 360.0;
    
  if (heading < 0.0)
    heading += 360.0;
  else
  if (heading >= 360.0)
    heading -= 360.0;
    
  PutSample(varp, heading);

}  /* END SPTDRDST */

/* -------------------------------------------------------------------- */
void scircN(DERTBL *varp)
{
  PutSample(varp, neg_heading);

}  /* END SPTDRRAD */

/* -------------------------------------------------------------------- */
void scircR(DERTBL *varp)
{
  NR_TYPE  radius, tasx, thdg, delta;
  static NR_TYPE prevTHDG;

  tasx = GetSample(varp, 0);
  thdg = GetSample(varp, 1);

  distance += tasx;
  delta = thdg - prevTHDG;

  if (delta >= 180.0)
    delta -= 360.0;

  if (delta <= -180.0)
    delta += 360.0;

  thdgAccum += delta;
  prevTHDG = thdg;

  if (thdgAccum != 0.0)
    radius = fabs(distance / (thdgAccum * M_PI / 180.0) / 1000);
  else
    radius = 0.0;

  PutSample(varp, radius);

}  /* END SPTDRRAD */

/* END PRCLTRK1.C */

/*
-------------------------------------------------------------------------
OBJECT NAME:	interp.c

FULL NAME:	Interpolation algorythms

ENTRY POINTS:	PolyInterp()
		LinearInterpolate()

DESCRIPTION:	These interpolation algorythms are for Phase Shifting.

INPUT:		

OUTPUT:		float	new_y

REFERENCES:	none

REFERENCED BY:	phase_shift.c

NOTE:		variable yaxis should contain 4 points with the desired
		value lying between yaxis[1] and yaxis[2].

BIBLIOGRAPHY:	PolyInterp() algorythm is LaGrange Polynomial.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include <math.h>
#include "nimbus.h"
#include "decode.h"

#define N	4
#define DEGREE	(N-1)


/* -------------------------------------------------------------------- */
NR_TYPE PolyInterp(NR_TYPE yaxis[], int ms_gap, int ms_want)
{
  int		i, m;
  NR_TYPE	x[N], c[N], numerator, denominator;
  NR_TYPE	y;

  y		= 0.0;
  numerator	= 1.0;

  /* Create X axis.  c array is to reduce computation later
   */
  for (i = 0; i < N; ++i)
    numerator *= (c[i] = ms_want - (x[i] = ms_gap * i));

  for (m = 0; m < N; ++m)
    {
    denominator = 1.0;

    for (i = 0; i < N; ++i)
      if (m != i)
        denominator *= (x[m] - x[i]);

    y += ((numerator / c[m]) / denominator) * yaxis[m];
    }

/*
  if (y != 0.0) {
    printf("----------------------\n");
    printf("y[0]=%f, y[1]=%f,y[2]=%f,y[3]=%f, ",yaxis[0],yaxis[1],yaxis[2],yaxis[3]);
    printf("y=%f\n", y);
    }
*/
  return(y);

}	/* END POLYINTERP */

/* -------------------------------------------------------------------- */
NR_TYPE LinearInterpolate(NR_TYPE p1, NR_TYPE p2, int ms_gap, int ms_want)
{
  NR_TYPE	slope;

  slope = (p2 - p1) / ms_gap;

  return(p1 + (slope * ms_want));

}	/* END LINEARINTERPOLATE */

/* END INTERP.C */

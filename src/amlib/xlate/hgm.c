/*
-------------------------------------------------------------------------
OBJECT NAME:	hgm.c

FULL NAME:	Translate HGM SDI variables to new record format

ENTRY POINTS:	xlhgm()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		

OUTPUT:		Averaged value

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "raf.h"
#include "nimbus.h"
#include "amlib.h"
#include "gust.h"

extern int	Aircraft;

/* -------------------------------------------------------------------- */
void xlhgm(RAWTBL *varp, void *p, NR_TYPE *output)
{
  int		i;
  short		*input = (short *)p;
  NR_TYPE	hgm, hgmv;

  if (Aircraft == KINGAIR)
    for (i = 0; i < varp->SampleRate; ++i)
      {
      hgmv = (ntohs(input[i * varp->ADSoffset]) - varp->convertOffset) *
							varp->convertFactor;

      hgm  = 480.0 - 50.0 * hgmv;

      if (hgmv < -0.4)
        hgm = 366.6667 - 333.3333 * hgmv;

      if (atoi(ProjectNumber) == 201)
        if (hgmv < -7.3)
          hgm = -948.0 - 196.0 * hgmv;
        else
          hgm = 264.0 - 29.98 * hgmv;

      output[i] = hgm * 0.3048;
      }
  else
    /* This is hard wired for 2 or 3 cal coe's */
    for (i = 0; i < varp->SampleRate; ++i)
      {
      hgm = (ntohs(input[i * varp->ADSoffset]) - varp->convertOffset) *
							varp->convertFactor;

      if (varp->order == 2)
        output[i] = FirstPoly(hgm, varp->cof);
      else
        output[i] = SecondPoly(hgm, varp->cof);
      }

}	/* END XLHGM */

/* END HGM.C */

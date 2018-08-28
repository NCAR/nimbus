/*
-------------------------------------------------------------------------
OBJECT NAME:	hgm.c

FULL NAME:	Translate HGM SDI variables to new record format

ENTRY POINTS:	xlhgm()

STATIC FNS:	none

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-05
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void xlhgm(RAWTBL *varp, void *p, NR_TYPE *output)
{
  short		*input = (short *)p;
  NR_TYPE	hgm, hgmv;

  if (cfg.Aircraft() == Config::KINGAIR)
    for (size_t i = 0; i < varp->SampleRate; ++i)
      {
      hgmv = (ntohs(input[i * varp->ADSoffset]) - varp->convertOffset) *
						varp->convertFactor;

      hgm  = 480.0 - 50.0 * hgmv;

      if (hgmv < -0.4)
        hgm = 366.6667 - 333.3333 * hgmv;

      if (atoi(cfg.ProjectNumber().c_str()) == 201)
        {
        if (hgmv < -7.3)
          hgm = -948.0 - 196.0 * hgmv;
        else
          hgm = 264.0 - 29.98 * hgmv;
        }

      output[i] = hgm * 0.3048;
      }
  else
    /* This is hard wired for 2 or 3 cal coe's */
    for (size_t i = 0; i < varp->SampleRate; ++i)
      {
      hgm = (ntohs(input[i * varp->ADSoffset]) - varp->convertOffset) *
							varp->convertFactor;

      if (varp->cof.size() == 2)
        output[i] = FirstPoly(hgm, varp->cof);
      else
        output[i] = SecondPoly(hgm, varp->cof);
      }

}	/* END XLHGM */

/* END HGM.C */

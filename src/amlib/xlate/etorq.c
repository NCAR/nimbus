/*
-------------------------------------------------------------------------
OBJECT NAME:	etorq.c

FULL NAME:	

ENTRY POINTS:	xletorq()

STATIC FNS:	none

DESCRIPTION:	According to Ed Brown the SDI variable ETORQ is off by
		57.? degrees.  So here is the correction function. 3/16/93

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

/* These names mean absolutly nothing */
static NR_TYPE	lez[] =	{ 1847.9928, 0.17078673, -2.2302340e-08 };
static NR_TYPE	otf[] =	{ 1847.3864, 0.17202371, -2.7902932e-07 };
static NR_TYPE	fte[] =	{ -925.09894, 0.16334911, 4.9590147e-07 };
		

/* -------------------------------------------------------------------- */
void xletorq(RAWTBL *varp, void *input, NR_TYPE *output)
{
  short		etorq;
  NR_TYPE	*cc;
  std::vector<NR_TYPE> ccv;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    etorq = ntohs(((short *)input)[i * varp->ADSoffset]);

    if (etorq <= 0)
      cc = lez;
    else
    if (etorq >= 1 && etorq <= 5568)
      cc = otf;
    else
    if (etorq >= 5569 && etorq <= 8197)
      cc = fte;
    else
      {
      output[i] = 0.0;
      continue;
      }

    // Copy into a vector.
    for (size_t j = 0; j < 3; ++j)
      ccv.push_back(cc[i]);

    output[i] = SecondPoly((NR_TYPE)etorq, ccv);
    }

}	/* END XLETORQ */

/* END ETORQ.C */

/*
-------------------------------------------------------------------------
OBJECT NAME:	grey_hskp.c

FULL NAME:	Translate PMS2D houskeeping variables to Nimbus record
		format

ENTRY POINTS:	xl2d_hskp0(), xl2d_hskp1(), xl2d_hskp2(), xl2d_hskp3(),
		xl2d_hskp4(), xl2d_hskp5(), xl2d_hskp6(), xl2d_hskp7()

STATIC FNS:	none

DESCRIPTION:	

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1998
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void xlGrey_hskp0(RAWTBL *varp, void *input, NR_TYPE *np)
{
  Grey_house	*p = (Grey_house *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (NR_TYPE)SecondPoly((NR_TYPE)ntohs(p[i].hdata[0]), varp->cof);

}	/* END XLGREY_HSKP0 */

/* -------------------------------------------------------------------- */
void xlGrey_hskp1(RAWTBL *varp, void *input, NR_TYPE *np)
{
  Grey_house	*p = (Grey_house *)input;

  np[0] = 0.0;
  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (NR_TYPE)SecondPoly((NR_TYPE)ntohs(p[i].hdata[1]), varp->cof);

}	/* END XLGREY_HSKP1 */

/* END GREY_HSKP.C */

/*
-------------------------------------------------------------------------
OBJECT NAME:	APN159.c

FULL NAME:	Translate APN159 analog/synchro variables.

ENTRY POINTS:	xlhgme(), xlchgme()

STATIC FNS:	none

DESCRIPTION:	

REFERENCES:	angcr()

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

static NR_TYPE	chgme[250];	/* 250 = Max Rate	*/

NR_TYPE angcr(NR_TYPE, NR_TYPE, float, float);


/* -------------------------------------------------------------------- */
void xlhgme(RAWTBL *varp, void *p, NR_TYPE *output)
{
  ushort	*input = (ushort *)p;
  NR_TYPE	hgme;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    if (HDRversion < 3.1)
      {
      hgme = (NR_TYPE)ntohs(~input[i * varp->ADSoffset]) * RESOLV14BIT * 0.04;
      hgme = angcr(hgme, chgme[i], 14.4, 0.0);
      output[i] = hgme * 84.666667;
      }
    else
      {
      hgme = (NR_TYPE)ntohs(input[i * varp->ADSoffset]) * RESOLV16BIT * 0.04;
      hgme = angcr(hgme, chgme[i], 14.4, 0.0);
      output[i] = hgme * 84.666667;
      }

}	/* END XLHGME */

/* -------------------------------------------------------------------- */
void xlchgme(RAWTBL *varp, void *p, NR_TYPE *output)
{
  ushort	*input = (ushort *)p;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    if (HDRversion < 3.1)
      output[i] = chgme[i] =
                  (NR_TYPE)ntohs(~input[i * varp->ADSoffset]) * RESOLV14BIT;
    else
      output[i] = chgme[i] =
                  (NR_TYPE)ntohs(input[i * varp->ADSoffset]) * RESOLV16BIT;

}	/* END XLCHGME */

/* END APN159.C */

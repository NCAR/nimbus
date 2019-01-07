/*
-------------------------------------------------------------------------
OBJECT NAME:	psff.c

FULL NAME:	Copy PSF into PSFF

ENTRY POINTS:	xlpsff()

DESCRIPTION:	This is an ADS3 function.  We have need to duplicate a
		variable; PSF into PSFF.

COPYRIGHT:	University Corporation for Atmospheric Research, 2018
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void xlpsff(RAWTBL *varp, void *p, NR_TYPE *output)
{
  static RAWTBL *psf = 0;

  if (psf == 0)
  {
    psf = raw[SearchTable(raw, "PSF")];
  }

  for (size_t i = 0; i < varp->SampleRate; ++i)
  {
    output[i] = ((NR_TYPE *)p)[psf->SRstart+i];
  }

}	/* END XLPSFF */

/* -------------------------------------------------------------------- */
void xladiff(RAWTBL *varp, void *p, NR_TYPE *output)
{
  static RAWTBL *adifr = 0;

  if (adifr == 0)
  {
    adifr = raw[SearchTable(raw, "ADIFR")];
  }

  for (size_t i = 0; i < varp->SampleRate; ++i)
  {
    output[i] = ((NR_TYPE *)p)[adifr->SRstart+i];
  }

}	/* END XLPSFF */

/* END PSFF.C */

/*
-------------------------------------------------------------------------
OBJECT NAME:	APN232.c

FULL NAME:	

ENTRY POINTS:	xlhgm232()
		xlhgm232s()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	rec_decode.c, compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1996
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"


/* -------------------------------------------------------------------- */
void xlhgm232(RAWTBL *varp, void *p, NR_TYPE *output)
{
  ushort	status, sample;
  ushort	*input = (ushort *)p;
  NR_TYPE	out;
  static NR_TYPE	lastGood = 0.0;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    status = ntohs(input[i * varp->ADSoffset]) & 0x0001;
    sample = ntohs(input[i * varp->ADSoffset]) & 0xFFFE;
//printf("%x\n", input[i * varp->ADSoffset]);
//    sample = (sample - varp->convertOffset) * varp->convertFactor * FTMTR;
    out = sample * FTMTR;

    if (status)
      output[i] = lastGood;
    else
      output[i] = lastGood = out;
    }

}	/* END XLHGM232 */

/* -------------------------------------------------------------------- */
void xlhgm232s(RAWTBL *varp, void *input, NR_TYPE *output)
{
  for (size_t i = 0; i < varp->SampleRate; ++i)
    output[i] = ntohs(((short *)input)[i * varp->ADSoffset]) & 0x0001;

}	/* END XLHGM232S */

/* END APN232.C */

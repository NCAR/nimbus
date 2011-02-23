/*
-------------------------------------------------------------------------
OBJECT NAME:	APN232.c

FULL NAME:	APN 232 Radar Altimeter

ENTRY POINTS:	xlhgm232()
		xlhgm232s()

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-2011
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

#include <ctype.h>


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
    out = sample; 

    if (toupper(varp->Units.c_str()[0]) == 'M') 
      out *= Feet2Meters;

    if (status)
      output[i] = lastGood;
    else
      output[i] = lastGood = out;
    }
}

/* -------------------------------------------------------------------- */
void xlhgm232s(RAWTBL *varp, void *input, NR_TYPE *output)
{
  for (size_t i = 0; i < varp->SampleRate; ++i)
    output[i] = ntohs(((short *)input)[i * varp->ADSoffset]) & 0x0001;
}

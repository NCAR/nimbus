/*
-------------------------------------------------------------------------
OBJECT NAME:	rec_decode.c

FULL NAME:	Decode ADS data Record

ENTRY POINTS:	DecodeRecord()

DESCRIPTION:	Translates ADS data records into the internal format.
		The internal format is all one data type for ease of
		processing.  The 2 functions are essentially identical,
		the difference is that the one ForRealTime also puts the
		data into the arrays bits[] and volts[], for display
		purposes.

INPUT:		ADS logical record

OUTPUT:		New logical record

REFERENCES:	libxlate.a

REFERENCED BY:	lrloop.c hrloop.c winputops.c

NOTE:		If you chnage one, make sure the other does/doesn't need
		the same change.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-9
-------------------------------------------------------------------------
*/

#include "define.h"
#include "decode.h"

extern NR_TYPE *SRTvolts;

/* -------------------------------------------------------------------- */
void decodeADS2analog(RAWTBL *sp, void *input, NR_TYPE *output)
{
  int		pos;
  ushort	*lrp;		/* ADS Logical Record Pointer	*/

  lrp	= &((ushort *)ADSrecord)[sp->ADSstart];
  pos	= sp->SRstart;

  if (sp->type[0] == 'C')
    {
    if (strcmp(sp->type, "C24") == 0)
      {
      for (size_t j = 0; j < sp->SampleRate; ++j, ++pos)
        {
        SampledData[pos] = bits[pos] =
	ntohl(*((unsigned long *)&lrp[j * sp->ADSoffset]));
        SRTvolts[pos] = (bits[pos] - sp->convertOffset) * sp->convertFactor;
        }
      }
    else
      {
     for (size_t j = 0; j < sp->SampleRate; ++j, ++pos)
        {
        SampledData[pos] = bits[pos] = ntohs(lrp[j * sp->ADSoffset]);
        SRTvolts[pos] = (bits[pos] - sp->convertOffset) * sp->convertFactor;
        }
      }
    }
  else
    {
    if (strcmp(sp->type, "D20") == 0)
      {
      for (size_t j = 0; j < sp->SampleRate; ++j, ++pos)
        {
        SampledData[pos] = bits[pos] =
		(long)ntohl(*((long *)&lrp[j * sp->ADSoffset]));
        SRTvolts[pos] = (bits[pos] - sp->convertOffset) * sp->convertFactor;
        }
      }
    else
      {
      for (size_t j = 0; j < sp->SampleRate; ++j, ++pos)
        {
        SampledData[pos] = bits[pos] =
			(short)ntohs((ushort)lrp[j * sp->ADSoffset]);
        SRTvolts[pos] = (bits[pos] - sp->convertOffset) * sp->convertFactor;
        }
      }
    }
}

/* -------------------------------------------------------------------- */
void DecodeRecord()
{
  RAWTBL *rp;

  /* Extract block variables into new record
   */
  for (size_t i = 0; (rp = raw[i]); ++i)
    (*rp->xlate)(	rp,
			&((ushort *)ADSrecord)[rp->ADSstart],
			&SampledData[rp->SRstart]);

}	/* END DECODEADSRECORD */

/* END REC_DECODE.C */

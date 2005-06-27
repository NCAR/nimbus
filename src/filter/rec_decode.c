/*
-------------------------------------------------------------------------
OBJECT NAME:	rec_decode.c

FULL NAME:	Decode ADS data Record

ENTRY POINTS:	DecodeADSrecord()
		DecodeADSrecordForRealTime()

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

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2005
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"

#include <netinet/in.h>

extern ushort	*bits;

/* -------------------------------------------------------------------- */
void DecodeADSrecord(
	short	lr[],	/* ADS Logical Record	*/
	NR_TYPE	nlr[])	/* New Logical Record	*/
{
  /* Cast SDI variables into new record
   */
  for (size_t i = 0; i < sdi.size(); ++i)
  {
    SDITBL	*sp = sdi[i];
    int		pos;
    short	*lrp;		/* ADS Logical Record Pointer	*/

    lrp	= &lr[sp->ADSstart];
    pos	= sp->SRstart;

    // Raw analog/digital bits/counts for WINDS display.
    if (cfg.ProcessingMode() == Config::RealTime)
      bits[sp->LRstart] = ntohs(lrp[0]);

    if (sp->type[0] == 'C')
    {
      if (strcmp(sp->type, "C24") == 0)
      {
        for (size_t j = 0; j < sp->SampleRate; ++j, ++pos)
          nlr[pos] = (NR_TYPE)ntohl(*((unsigned long *)&lrp[j * sp->ADSoffset]));
      }
      else
      {
        for (size_t j = 0; j < sp->SampleRate; ++j, ++pos)
          nlr[pos] = (NR_TYPE)ntohs(lrp[j * sp->ADSoffset]);
      }
    }
    else
    {
      if (strcmp(sp->type, "D20") == 0)
      {
        for (size_t j = 0; j < sp->SampleRate; ++j, ++pos)
          nlr[pos] = (NR_TYPE)ntohl(*((long *)&lrp[j * sp->ADSoffset]));
      }
      else
      {
        for (size_t j = 0; j < sp->SampleRate; ++j, ++pos)
          nlr[pos] = (short)ntohs((ushort)lrp[j * sp->ADSoffset]);
      }
    }
  }


  /* Extract block variables into new record
   */
  for (size_t i = 0; i < raw.size(); ++i)
  {
    if (raw[i])
      (*raw[i]->xlate)(raw[i], &lr[raw[i]->ADSstart], &nlr[raw[i]->SRstart]);
  }

}	/* END DECODEADSRECORD */

/* END REC_DECODE.C */

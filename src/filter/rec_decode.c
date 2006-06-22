/*
-------------------------------------------------------------------------
OBJECT NAME:	rec_decode.c

FULL NAME:	Decode ADS data Record

ENTRY POINTS:	DecodeADSrecord()

DESCRIPTION:	Translates ADS data records into the internal format.
		The internal format is all one data type for ease of
		processing.

NOTE:		If you chnage one, make sure the other does/doesn't need
		the same change.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2006
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"

#include <netinet/in.h>

extern ushort	*bits;

static bool blankOutThisValue(RAWTBL * var, short thisTime[]);

/* -------------------------------------------------------------------- */
void DecodeADSrecord(
	short	lr[],	/* ADS Logical Record	*/
	NR_TYPE	nlr[])	/* New Logical Record	*/
{
  if (cfg.isADS3())
  {
    // Forcing ADS3 into the ADS2 architecture.  FindNextRecord will
    // put it into ADSrecord, copy it out here.
    memcpy((void *)nlr, (void *)lr, nSRfloats * sizeof(NR_TYPE));

    // Set dynamic lags.
    float *rec_p = (float *)lr;
    for (size_t i = 0; i < raw.size(); ++i)
    {
      raw[i]->DynamicLag = 0;  // reset.

      if (raw[i]->LAGstart > 0)
      {
        float lag = rec_p[raw[i]->LAGstart];
        if (!isnan(lag))
          raw[i]->DynamicLag = (int)(lag / 1000.0);
      }
    }

    return;
  }

  /* Extract block variables into new record
   */
  for (size_t i = 0; i < raw.size(); ++i)
  {
    if (blankOutThisValue(raw[i], lr) == true)
      for (size_t j = 0; j < raw[i]->SampleRate * raw[i]->Length; ++j)
        nlr[raw[i]->SRstart+j] = floatNAN;
    else
      (*raw[i]->xlate)(raw[i], &lr[raw[i]->ADSstart], &nlr[raw[i]->SRstart]);
  }
}	/* END DECODEADSRECORD */

/* -------------------------------------------------------------------- */
static bool blankOutThisValue(RAWTBL * var, short r[])
{
  // If this var has no xlate() fn or DataQuality has been marked bad, blank.
  if (var->xlate == 0 || strcmp(var->DataQuality, "Bad") == 0)
    return true;

  // Compute time for this record.
  static int prevTime = -1;
  static bool crossedMidNight = false;
  int thisTime = ntohs(r[1]) * 3600 + ntohs(r[2]) * 60 + ntohs(r[3]);

  if (thisTime < prevTime)
    crossedMidNight = true;

  prevTime = thisTime;

  if (crossedMidNight)
    thisTime += 86400;

  /* Check to see if the time of this record falls into any of the blank
   * out times for this variable.
   */
  for (size_t i = 0; i < var->blank_out.size(); ++i)
  {
    if (thisTime >= var->blank_out[i].first &&
	thisTime <= var->blank_out[i].second)
      return true;
  }

  return false;
}

/* -------------------------------------------------------------------- */
void decodeADS2analog(RAWTBL *varp, void *input, NR_TYPE *output)
{
  /* Cast SDI variables into new record
   */
  int pos = 0;
  short *lrp = (short *)input;

  // Raw analog/digital bits/counts for WINDS display.
  if (cfg.ProcessingMode() == Config::RealTime)
    bits[varp->LRstart] = ntohs(lrp[0]);

  if (varp->type[0] == 'C')
  {
    if (strcmp(varp->type, "C24") == 0)
    {
      for (size_t j = 0; j < varp->SampleRate; ++j, ++pos)
        output[pos] = (NR_TYPE)ntohl(*((unsigned long *)&lrp[j * varp->ADSoffset]));
    }
    else
    {
      for (size_t j = 0; j < varp->SampleRate; ++j, ++pos)
        output[pos] = (NR_TYPE)ntohs(lrp[j * varp->ADSoffset]);
    }
  }
  else
  {
    if (strcmp(varp->type, "D20") == 0)
    {
      for (size_t j = 0; j < varp->SampleRate; ++j, ++pos)
        output[pos] = (NR_TYPE)ntohl(*((long *)&lrp[j * varp->ADSoffset]));
    }
    else
    {
      for (size_t j = 0; j < varp->SampleRate; ++j, ++pos)
        output[pos] = (short)ntohs((ushort)lrp[j * varp->ADSoffset]);
    }
  }
}

/* END REC_DECODE.C */

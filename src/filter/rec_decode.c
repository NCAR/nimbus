/*
-------------------------------------------------------------------------
OBJECT NAME:	rec_decode.c

FULL NAME:	Decode ADS data Record

ENTRY POINTS:	DecodeADSrecord()
		decodeADS2analog()

STATIC FNS:	blankOutThisValue()
		setNIDASDynamicLags()

DESCRIPTION:	Translates ADS data records into the internal format.
		The internal format is all one data type for ease of
		processing.

NOTE:		If you change one, make sure the other does/doesn't need
		the same change.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-2006
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"

#include <netinet/in.h>

extern ushort	*bits;

static void setNIDASDynamicLags(short lr[]);
static void BlankOutRawData(NR_TYPE nlr[]);


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
    if (cfg.TimeShifting())
      setNIDASDynamicLags(lr);
  }
  else
  {
    // else ADS2.  Extract raw data into processed float/double.
    for (size_t i = 0; i < raw.size(); ++i)
      if (raw[i]->xlate != 0)
        (*raw[i]->xlate)(raw[i], &lr[raw[i]->ADSstart], &nlr[raw[i]->SRstart]);
  }

  BlankOutRawData(nlr);
}

/* -------------------------------------------------------------------- */
static void setNIDASDynamicLags(short lr[])
{
  NR_TYPE *rec_p = (NR_TYPE *)lr;
  for (size_t i = 0; i < raw.size(); ++i)
  {
    raw[i]->DynamicLag = 0;  // reset.

    if (raw[i]->LAGstart > 0)
    {
      NR_TYPE lag = rec_p[raw[i]->LAGstart];
      if (!isnan(lag))
        raw[i]->DynamicLag = (int)(lag / 1000.0);
      else
        raw[i]->badLagCntr++;
    }
  }
}

/* -------------------------------------------------------------------- */
bool blankOutThisValue(var_base * var, time_t thisTime)
{
  // If DataQuality has been marked bad, blank.
  if (strcmp(var->DataQuality, "Bad") == 0)
    return true;

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
static void BlankOutRawData(NR_TYPE nlr[])
{
  /* Blank out any data from the BlankOuts File.  Do raw data here, then it will
   * ripple through derived and make them all nan also.
   */
  int thisTime = SampledDataTimeToSeconds(nlr);

  for (size_t i = 0; i < raw.size(); ++i)
  {
    RAWTBL *rp = raw[i];

    if (blankOutThisValue(rp, thisTime) == true || (cfg.isADS2() && rp->xlate == 0))
    {
      size_t n = rp->SampleRate * rp->Length;
      for (size_t j = 0; j < n; ++j)
        nlr[rp->SRstart+j] = floatNAN;
    }
  }
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
        output[pos] = (NR_TYPE)ntohl(*((uint32_t *)&lrp[j * varp->ADSoffset]));
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
        output[pos] = (NR_TYPE)ntohl(*((int32_t *)&lrp[j * varp->ADSoffset]));
    }
    else
    {
      for (size_t j = 0; j < varp->SampleRate; ++j, ++pos)
        output[pos] = (short)ntohs((ushort)lrp[j * varp->ADSoffset]);
    }
  }
}

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
#include "timeseg.h"

#include <netinet/in.h>

extern ushort	*bits;
extern std::vector<RAWTBL *> decode;

static void setNIDASDynamicLags(const short lr[]);
static void BlankOutRawData(NR_TYPE nlr[]);


/* -------------------------------------------------------------------- */
void DecodeADSrecord(
	const short	lr[],	/* ADS Logical Record	*/
	NR_TYPE	nlr[])	/* New Logical Record	*/
{
  VLOG(("enter DecodeADSrecord()"));
  if (cfg.isADS3())
  {
    // Forcing ADS3 into the ADS2 architecture.  FindNextRecord will
    // put it into ADSrecord, copy it out here.
    VLOG(("memcpy(") << (void*)nlr << "," << (void*)lr << ","
	 << nSRvalues * sizeof(NR_TYPE) << "(" << nSRvalues << " doubles))");
    memcpy((void *)nlr, (void *)lr, nSRvalues * sizeof(NR_TYPE));

    // Set dynamic lags.  HRT only...
    if (cfg.TimeShifting() && cfg.ProcessingRate() == Config::HighRate)
      setNIDASDynamicLags(lr);

    for (size_t i = 0; i < decode.size(); ++i)
      if (decode[i]->xlate)
        (*decode[i]->xlate)(decode[i], (void *)nlr, &nlr[decode[i]->SRstart]);
  }
  else
  {
    // else ADS2.  Extract raw data into processed float/double.
    for (size_t i = 0; i < decode.size(); ++i)
      (*decode[i]->xlate)(decode[i], &lr[decode[i]->ADSstart], &nlr[decode[i]->SRstart]);
  }

  BlankOutRawData(nlr);
}

/* -------------------------------------------------------------------- */
static void setNIDASDynamicLags(const short lr[])
{
  NR_TYPE *rec_p = (NR_TYPE *)lr;
  for (size_t i = 0; i < raw.size(); ++i)
  {
    raw[i]->DynamicLag = 0;  // reset.

    if (raw[i]->TTindx > 0)
    {
      NR_TYPE lag = rec_p[raw[i]->TTindx];
      if (!std::isnan(lag))
        raw[i]->DynamicLag = (int)(lag / 1000.0);
      else
        raw[i]->badLagCntr++;
    }
  }
}

/* -------------------------------------------------------------------- */
int blankOutThisValue(var_base * var, time_t thisTime)
{
  /* Check to see if the time of this record falls into any of the blank
   * out times for this variable.
   */
  for (size_t i = 0; i < var->set_value.size(); ++i)
  {
    if (thisTime >= var->set_value[i].start &&
      thisTime <= var->set_value[i].end)
    return i;
  }

  return -1;
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
    int index = blankOutThisValue(rp, thisTime);

    if (index >= 0 || (cfg.isADS2() && rp->xlate == 0))
    {
      size_t n = rp->SampleRate * rp->Length;
      for (size_t j = 0; j < n; ++j)
        nlr[rp->SRstart+j] = rp->set_value[index].value;
    }
  }
}

/* -------------------------------------------------------------------- */
void decodeADS2analog(RAWTBL *varp, const void *input, NR_TYPE *output)
{
  /* Cast SDI variables into new record
   */
  int pos = 0;
  const short *lrp = (short *)input;

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

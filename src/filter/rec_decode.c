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

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-97
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"

#include <netinet/in.h>

extern ushort	*bits;
extern NR_TYPE	*volts;


/* -------------------------------------------------------------------- */
void DecodeADSrecord(
	short	lr[],	/* ADS Logical Record	*/
	NR_TYPE	nlr[])	/* New Logical Record	*/
{
  int		j, pos;
  short		*lrp;		/* ADS Logical Record Pointer	*/

static int firstTime = true;
static FILE *fp;

  /* Cast SDI variables into new record
   */
  for (int i = 0; i < sdi.size(); ++i)
    {
    SDITBL *sp = sdi[i];

    lrp	= &lr[sp->ADSstart];
    pos	= sp->SRstart;

    if (Mode == REALTIME)
      {
      bits[sp->LRstart] = ntohs(lrp[0]);
      volts[sp->LRstart] = (NR_TYPE)((short)ntohs(lrp[0]) - sp->convertOffset) * sp->convertFactor;
      }

    if (sp->type[0] == 'C')
      {
      if (strcmp(sp->type, "C24") == 0)
        {
        for (j = 0; j < sp->SampleRate; ++j, ++pos)
          nlr[pos] = (NR_TYPE)ntohl(*((unsigned long *)&lrp[j * sp->ADSoffset]));
        }
      else
        {
        for (j = 0; j < sp->SampleRate; ++j, ++pos)
          nlr[pos] = (NR_TYPE)ntohs(lrp[j * sp->ADSoffset]);
        }
      }
    else
      {
      if (strcmp(sp->type, "D20") == 0)
        {
        for (j = 0; j < sp->SampleRate; ++j, ++pos)
          nlr[pos] = (NR_TYPE)ntohl(*((long *)&lrp[j * sp->ADSoffset]));
        }
      else
        {
        for (j = 0; j < sp->SampleRate; ++j, ++pos)
          nlr[pos] = (short)ntohs((ushort)lrp[j * sp->ADSoffset]);

if (strcmp(sp->name, "XDMSEC") == 0)
  {
  static float prevRamp;
  int prevMsec=-1;

  if (ProductionRun)
    sprintf(buffer, "%s/%s%s.bandy", getenv("PROD_DATA"), ProjectNumber, FlightNumber);
  else
    sprintf(buffer, "%s/%s%s.bandy", getenv("DATA_DIR"), ProjectNumber, FlightNumber);
  if (firstTime)
    {
    if ((fp = fopen(buffer, "w+")) == NULL)
      {
      fprintf(stderr, "rec_decode.c: can't open %s\n", buffer);
      exit(1);
      }
    firstTime = false;
    }

  for (j = 0; j < sp->SampleRate && 65535 != (int)nlr[sp->SRstart+j]; ++j)
//  for (j = 0; j < sp->SampleRate; ++j)
    {
    if (j > 40 && (int)nlr[sp->SRstart+j] < 200)
      fprintf(fp, "%02d:%02d:%02d.%03d %d %.2f\n",
        ntohs(lr[1]), ntohs(lr[2]), ntohs(lr[3]), (int)nlr[sp->SRstart+j]+1,
	(int)nlr[sdi[i-2]->SRstart+j], nlr[sdi[i-1]->SRstart+j]);
    else
      fprintf(fp, "%02d:%02d:%02d.%03d %d %.2f\n",
        ntohs(lr[1]), ntohs(lr[2]), ntohs(lr[3]), (int)nlr[sp->SRstart+j],
	(int)nlr[sdi[i-2]->SRstart+j], nlr[sdi[i-1]->SRstart+j]);

//    if (prevRamp+1 != nlr[sdi[i-2]->SRstart+j])
//      fprintf(stderr, ">>>>> We lost one @ %02d:%02d:%02d.%d\n",
//        ntohs(lr[1]), ntohs(lr[2]), ntohs(lr[3]), (int)nlr[sp->SRstart+j]);

    prevRamp = nlr[sdi[i-2]->SRstart+j];
    prevMsec = (int)nlr[sp->SRstart+j];
    }

  fflush(fp);
  }
        }
      }
    }


  /* Extract block variables into new record
   */
  for (int i = 0; i < raw.size(); ++i)
  {
    RAWTBL *rp = raw[i];

    (*rp->xlate)(rp, &lr[rp->ADSstart], &nlr[rp->SRstart]);
  }

}	/* END DECODEADSRECORD */

/* END REC_DECODE.C */

/*
-------------------------------------------------------------------------
OBJECT NAME:	dmt_hskp.c

FULL NAME:	Translate DMT PMS1Dv3 houskeeping variables to Nimbus record
		format

ENTRY POINTS:	xlpms_hskp0(), xlpms_hskp1(), xlpms_hskp2(), xlpms_hskp3(),
		xlpms_hskp4(), xlpms_hskp5(), xlpms_hskp6(), xlpms_hskp7()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	rec_decode.c

COPYRIGHT:	University Corporation for Atmospheric Research, 2000-2001
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

/* These #defines are not used, they are here for documentation
 */
/* FSSP/S100 housekeeping indices	*/
#define FREF	4
#define FTMP	7

/* PCASP/S200 houskeeping indices	*/
#define PHGB	0	/* High Gain Baseline	*/
#define PMGB	1	/* Mid Gain Baseline	*/
#define PLGB	2	/* Low Gain Baseline	*/
#define PFLW	3	/* Sample Flow		*/
#define PREF	4	/* Laser Ref voltage	*/
#define PFLWS	6	/* Sheath Flow		*/
#define PTMP	7	/* Internal temperature	*/

/* F300/S300 housekeeping indices	*/
#define FREF3	4
#define FTMP3	7

#define AVERAGE_LENGTH  44


/* -------------------------------------------------------------------- */
void xlrejDOF(RAWTBL *varp, void *input, NR_TYPE *np)
{
  DMT100_blk	*p = (DMT100_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (NR_TYPE)ntohl(p[i].rejDOF);

}

/* -------------------------------------------------------------------- */
void xlrejAT(RAWTBL *varp, void *input, NR_TYPE *np)
{
  DMT100_blk	*p = (DMT100_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (NR_TYPE)ntohl(p[i].rejAvgTrans);

}

/* -------------------------------------------------------------------- */
void xloFlow(RAWTBL *varp, void *input, NR_TYPE *np)
{
  DMT100_blk	*p = (DMT100_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = (NR_TYPE)ntohl(p[i].ADCoverflow);

}

/* -------------------------------------------------------------------- */
void xlsumCabin(RAWTBL *varp, void *input, NR_TYPE *np)
{ 
  DMT300_blk    *p = (DMT300_blk *)input;
  
  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    np[i] = 0.0;
    for (size_t j = 0; j < 8; ++j)
      np[i] += (NR_TYPE)ntohl(p[i].cabinChan[j]);
    }

}

/* -------------------------------------------------------------------- */
void xlS100cabin4(RAWTBL *varp, void *input, NR_TYPE *np)
{
  DMT100_blk	*p = (DMT100_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = ((NR_TYPE)ntohs(p[i].cabinChan[4]) - 2048) * 4.882812e-3;

}

/* -------------------------------------------------------------------- */
void xlS100cabin7(RAWTBL *varp, void *input, NR_TYPE *np)
{
  DMT100_blk	*p = (DMT100_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = ((NR_TYPE)ntohs(p[i].cabinChan[7]) - 2328) * 0.9765625;
}

/* -------------------------------------------------------------------- */
void xlS200cabin0(RAWTBL *varp, void *input, NR_TYPE *np)
{
  DMT200_blk	*p = (DMT200_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = ((NR_TYPE)ntohs(p[i].cabinChan[0]) - 2048) * 4.882812e-3;

}

/* -------------------------------------------------------------------- */
void xlS200cabin1(RAWTBL *varp, void *input, NR_TYPE *np)
{
  DMT200_blk	*p = (DMT200_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = ((NR_TYPE)ntohs(p[i].cabinChan[1]) - 2048) * 4.882812e-3;

}

/* -------------------------------------------------------------------- */
void xlS200cabin2(RAWTBL *varp, void *input, NR_TYPE *np)
{
  DMT200_blk	*p = (DMT200_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = ((NR_TYPE)ntohs(p[i].cabinChan[2]) - 2048) * 4.882812e-3;

}

/* -------------------------------------------------------------------- */
void xlS200cabin3(RAWTBL *varp, void *input, NR_TYPE *np)
{
  DMT200_blk	*p = (DMT200_blk *)input;

  static NR_TYPE	flowValues[AVERAGE_LENGTH], flowSum = 0.0;
  static size_t		nValues = 0, index = 0;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    NR_TYPE cts = (NR_TYPE)ntohs(p[i].cabinChan[3]);

    size_t corder  = varp->order - 1;
    NR_TYPE x     = varp->cof[corder];

    for (size_t k = 1; k < varp->order; k++)
      x = varp->cof[corder-k] + cts * x;

//    x = 16.89368 - 0.01583*cts + 3.70579E-6*cts*cts;

    if (nValues == 0)
      memset((char *)flowValues, 0, sizeof(flowValues));
    else
      flowSum -= flowValues[index];

    flowValues[index++] = x;
    flowSum += x;

    if (nValues < AVERAGE_LENGTH)
      ++nValues;

    if (index >= AVERAGE_LENGTH)
      index = 0;

    np[i] = flowSum / nValues;
    }
//    np[i] = ((NR_TYPE)ntohs(p[i].cabinChan[3]) - 2295) * 3.553710e-3;

}

/* -------------------------------------------------------------------- */
void xlS200cabin4(RAWTBL *varp, void *input, NR_TYPE *np)
{
  DMT200_blk	*p = (DMT200_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = ((NR_TYPE)ntohs(p[i].cabinChan[4]) - 2048) * 4.882812e-3;

}

/* -------------------------------------------------------------------- */
void xlS200cabin6(RAWTBL *varp, void *input, NR_TYPE *np)
{
  DMT200_blk	*p = (DMT200_blk *)input;

  static NR_TYPE        flowValues[AVERAGE_LENGTH], flowSum = 0.0;
  static size_t		nValues = 0, index = 0;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    {
    NR_TYPE cts = (NR_TYPE)ntohs(p[i].cabinChan[6]);

    size_t corder  = varp->order - 1;
    NR_TYPE x     = varp->cof[corder];

    for (size_t k = 1; k < varp->order; k++)
      x = varp->cof[corder-k] + cts * x;

//    x = -729.57 + 0.87564*cts - 3.5197e-4*cts*cts + 4.750368e-8*cts*cts*cts;

    if (nValues == 0)
      memset((char *)flowValues, 0, sizeof(flowValues));
    else
      flowSum -= flowValues[index];

    flowValues[index++] = x;
    flowSum += x;

    if (nValues < AVERAGE_LENGTH)
      ++nValues;

    if (index >= AVERAGE_LENGTH)
      index = 0;

    np[i] = flowSum / nValues;
    }
//    np[i] = ((NR_TYPE)ntohs(p[i].cabinChan[6]) - 2750) * 46.556155e-3;

}

/* -------------------------------------------------------------------- */
void xlS200cabin7(RAWTBL *varp, void *input, NR_TYPE *np)
{
  DMT200_blk	*p = (DMT200_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = ((NR_TYPE)ntohs(p[i].cabinChan[7]) - 2328) * 0.9765625;

}

/* -------------------------------------------------------------------- */
void xlS300cabin4(RAWTBL *varp, void *input, NR_TYPE *np)
{
  DMT300_blk	*p = (DMT300_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = ((NR_TYPE)ntohs(p[i].cabinChan[4]) - 2048) * 4.882812e-3;

}

/* -------------------------------------------------------------------- */
void xlS300cabin7(RAWTBL *varp, void *input, NR_TYPE *np)
{
  DMT300_blk	*p = (DMT300_blk *)input;

  for (size_t i = 0; i < varp->SampleRate; ++i)
    np[i] = ((NR_TYPE)ntohs(p[i].cabinChan[7]) - 2328) * 0.9765625;
}

/* END DMT_HSKP.C */

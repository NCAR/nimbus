/*
-------------------------------------------------------------------------
OBJECT NAME:	average.c

FULL NAME:	Average Data

ENTRY POINTS:	AverageSampledData()

STATIC FNS:	AverageSDI()	These are not declared static, because they
		Average()	are assigned to function pointers in
		SumSDI()	hdr_decode.c, however, they are only called
		Sum()		from this file.
		SumVector()

DESCRIPTION:	

INPUT:		unaveraged_data, place_to_put_averaged_data

OUTPUT:		Averaged value

REFERENCES:	none

REFERENCED BY:	LowRateLoop(), Filter(), winputops.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-05
-------------------------------------------------------------------------
*/

#include "nimbus.h"

extern NR_TYPE	*SampledData, *AveragedData;


/* -------------------------------------------------------------------- */
void AverageSampledData()
{
  for (size_t i = 0; i < sdi.size(); ++i)
  {
    SDITBL *sp = sdi[i];

    (*sp->Average)(&SampledData[sp->SRstart], &AveragedData[sp->LRstart], sp);
  }


  for (size_t i = 0; i < raw.size(); ++i)
  {
    RAWTBL *rp = raw[i];

    if (rp->SampleRate <= 1)
      memcpy( (char *)&AveragedData[rp->LRstart],
              (char *)&SampledData[rp->SRstart],
              NR_SIZE * rp->Length);
    else
      (*rp->Average)(	&SampledData[rp->SRstart],
              &AveragedData[rp->LRstart],
              rp->SampleRate,
              rp->Length,
              rp->Modulo);
  }

}	/* END AVERAGESAMPLEDDATA */

/* -------------------------------------------------------------------- */
void AverageSDI(NR_TYPE *in_data, NR_TYPE *out_data, SDITBL *sp)
{
  int		sampleCntr = 0;
  double	sum = 0.0;

  for (size_t i = 0; i < sp->SampleRate; ++i)
    if (!isnan(in_data[i]))
      sum += in_data[sampleCntr++];

  if (sampleCntr == 0)
    out_data[0] = floatNAN;
  else
    out_data[0] = sum / sampleCntr;

}	/* END AVERAGESDI */

/* -------------------------------------------------------------------- */
void Average(
	NR_TYPE	*in_data,
	NR_TYPE	*out_data,
	size_t	n,
	size_t	l,
	MOD	*mp)
{
  int		sampleCntr = 0;
  double	sum = 0.0;
  double	average;

  if (mp)		/* if (ModuloVariable)		*/
    {
    bool	low_value, high_value;

    low_value = high_value = false;

    for (size_t i = 0; i < n; ++i)
      if ((int)in_data[i] < mp->bound[0])
        {
        low_value = true;
        sum += mp->diff;
        }
      else
        if ((int)in_data[i] > mp->bound[1])
          high_value = true;

    if (!(low_value && high_value))
      sum = 0.0;
    }

  for (size_t i = 0; i < n; ++i)
    if (!isnan(in_data[i]))
      sum += in_data[sampleCntr++];

  if (sampleCntr == 0)
    average = floatNAN;
  else
    average = sum / sampleCntr;

  if (mp)
    if (average > mp->value[1])
      average -= mp->diff;
    else
      if (average < mp->value[0])
        average += mp->diff;

  out_data[0] = average;

}	/* END AVERAGE */

/* -------------------------------------------------------------------- */
void SumSDI(NR_TYPE *in_data, NR_TYPE *out_data, SDITBL *sp)
{
  NR_TYPE	sum = 0.0;

  for (size_t i = 0; i < sp->SampleRate; ++i)
    if (!isnan(in_data[i]))
      sum += in_data[i];

  out_data[0] = sum;

}	/* END SUMSDI */

/* -------------------------------------------------------------------- */
void Sum(NR_TYPE *in_data, NR_TYPE *out_data, size_t n)
{
  double	sum = 0.0;

  for (size_t i = 0; i < n; ++i)
    if (!isnan(in_data[i]))
      sum += in_data[i];

  out_data[0] = sum;

}	/* END SUM */

/* -------------------------------------------------------------------- */
void SumVector(
	NR_TYPE	*in_data,
	NR_TYPE	*out_data,
	size_t	n,
	size_t	l)
{
  double	sum;

  for (size_t i = 0; i < l; ++i)
    {
    sum = 0.0;

    for (size_t j = 0; j < n; ++j)
      if (!isnan(in_data[(j*l)+i]))
        sum += in_data[(j * l) + i];

    out_data[i] = sum;
    }
}	/* END SUMVECTOR */

/* END AVERAGE.C */

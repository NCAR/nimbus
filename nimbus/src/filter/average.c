/*
-------------------------------------------------------------------------
OBJECT NAME:	average.c

FULL NAME:	Average Data

ENTRY POINTS:	AverageSampledData()

STATIC FNS:	Average()	These are not declared static, because they
		Sum()		are assigned to function pointers in
		SumVector()	hdr_decode.c, however, they are only called
		AverageVector()	from this file.

DESCRIPTION:	

REFERENCED BY:	lrloop.c, hrloop.c rtloop.c, rtloop3.cc

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2007
-------------------------------------------------------------------------
*/

#include "nimbus.h"

extern NR_TYPE	*SampledData, *AveragedData;


/* -------------------------------------------------------------------- */
void AverageSampledData()
{
  for (size_t i = 0; i < raw.size(); ++i)
  {
    RAWTBL *rp = raw[i];

    if (rp->SampleRate <= 1)
      memcpy( (char *)&AveragedData[rp->LRstart],
              (char *)&SampledData[rp->SRstart],
              sizeof(NR_TYPE) * rp->Length);
    else
      (*rp->Average)(	&SampledData[rp->SRstart],
              &AveragedData[rp->LRstart],
              rp->SampleRate,
              rp->Length,
              rp->Modulo);
  }
}	/* END AVERAGESAMPLEDDATA */

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
    bool low_value, high_value;

    low_value = high_value = false;

    for (size_t i = 0; i < n; ++i)
    {
      if (!isnan(in_data[i]))
      {
        if ((int)in_data[i] < mp->bound[0])
        {
          low_value = true;
          sum += mp->diff;
        }
        else
          if ((int)in_data[i] > mp->bound[1])
            high_value = true;
      }
    }

    if (!(low_value && high_value))
      sum = 0.0;
  }

  for (size_t i = 0; i < n; ++i)
    if (!isnan(in_data[i]))
    {
      sum += in_data[i];
      ++sampleCntr;
    }

  if (sampleCntr == 0)
    average = floatNAN;
  else
    average = sum / sampleCntr;

  if (mp)
  {
    if (average > mp->value[1])
      average -= mp->diff;
    else
      if (average < mp->value[0])
        average += mp->diff;
  }

  out_data[0] = average;

}	/* END AVERAGE */

/* -------------------------------------------------------------------- */
void Sum(NR_TYPE *in_data, NR_TYPE *out_data, size_t n, size_t l, MOD *mp)
{
  double sum = 0.0;

  for (size_t i = 0; i < n; ++i)
    if (!isnan(in_data[i]))
      sum += in_data[i];

  out_data[0] = sum;
}

/* -------------------------------------------------------------------- */
void SumVector(NR_TYPE *in_data, NR_TYPE *out_data, size_t n, size_t l)
{
  for (size_t i = 0; i < l; ++i)
  {
    double sum = 0.0;

    for (size_t j = 0; j < n; ++j)
      if (!isnan(in_data[(j*l)+i]))
        sum += in_data[(j * l) + i];

    out_data[i] = sum;
  }
}

/* -------------------------------------------------------------------- */
void AverageVector(NR_TYPE *in_data, NR_TYPE *out_data, size_t n, size_t l)
{
  for (size_t i = 0; i < l; ++i)
  {
    double sum = 0.0;
    int cntr = 0;

    for (size_t j = 0; j < n; ++j)
      if (!isnan(in_data[(j*l)+i]))
      {
        sum += in_data[(j * l) + i];
        ++cntr;
      }

    out_data[i] = sum / cntr;
  }
}

/* END AVERAGE.C */

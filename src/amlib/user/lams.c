/*
-------------------------------------------------------------------------
OBJECT NAME:	lams.c

FULL NAME:	

COPYRIGHT:	University Corporation for Atmospheric Research, 2009
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

#define MAX_BUFFER 512

static const float MINFQZ = 0.0;
static const float MAXFQZ = 100.0;
static const float MARGIN = 3.0;
static const float MPS_PER_FOLD = 78.1;	// m/s per fold.

/* Compute the mean frequency, and spectrum width.
 */
static void compute_freq(const int nsamples,
                         const NR_TYPE *magnitude,
                         NR_TYPE *mean_freq_mhz,
                         NR_TYPE *spec_width_mhz)
{
  int ii;
  const int kCent = nsamples / 2;
  int kMax;
  int kOffset;
  int kStart, kEnd;
  int count;
  int nTest;

  NR_TYPE maxMag;
  NR_TYPE sumPower;
  NR_TYPE sumK;
  NR_TYPE sumK2;
  NR_TYPE meanK;
  NR_TYPE sdevK;

  NR_TYPE powerCentered[MAX_BUFFER];
  const NR_TYPE *mp, *pw;
  
  NR_TYPE nyq_freq_mhz = 100;
  NR_TYPE noise_mag = 1.0e6;

  NR_TYPE mhz_per_sample = nyq_freq_mhz / nsamples;
  NR_TYPE noise_power = noise_mag * noise_mag;

  // Find max magnitude.
  
  maxMag = 0.0;
  kMax = 0;
  mp = magnitude;
  for (ii = 0; ii < nsamples; ii++, mp++) {
    if (*mp > maxMag) {
      kMax = ii;
      maxMag = *mp;
    }
  }
  if (kMax >= kCent) {
    kMax -= kCent;
  }

  // Center power array on the max value.

  kOffset = kCent - kMax;
  for (ii = 0; ii < nsamples; ii++) {
    int jj = (ii + kOffset) % nsamples;
    NR_TYPE powr = magnitude[ii] * magnitude[ii];
    powerCentered[jj] = powr;
  }

  // If the signal is noisy, we use the entire spectrum.
  // Otherwise we only use the part above the noise.

  kStart = 0;
  kEnd = nsamples - 1;

  // Moving away from the peak, find the points in the spectrum
  // where the signal drops below the noise threshold for at
  // least 3 points.
  
  count = 0;
  nTest = 3;
  kStart = kCent - 1;
  pw = powerCentered + kStart;
  for (ii = kStart; ii >= 0; ii--, pw--) {
    if (*pw < noise_power) {
      count ++;
      if (count >= nTest) {
        break;
      }
    } else {
      count = 0;
    }
    kStart = ii;
  }
  
  count = 0;
  kEnd = kCent + 1;
  pw = powerCentered + kEnd;
  for (ii = kEnd; ii < nsamples; ii++, pw++) {
    if (*pw < noise_power) {
      count ++;
      if (count >= nTest) {
        break;
      }
    } else {
      count = 0;
    }
    kEnd = ii;
  }
  
  // Compute mom1 and mom2, using those points above the noise floor.

  sumPower = 0.0;
  sumK = 0.0;
  sumK2 = 0.0;
  pw = powerCentered + kStart;
  for (ii = kStart; ii <= kEnd; ii++, pw++) {
    NR_TYPE kk = (NR_TYPE) ii;
    NR_TYPE power = *pw;
    sumPower += power;
    sumK += power * kk;
    sumK2 += power * kk * kk;
  }
  meanK = 0.0;
  sdevK = 0.0;
  if (sumPower > 0.0) {
    meanK = sumK / sumPower;
    NR_TYPE diff = (sumK2 / sumPower) - (meanK * meanK);
    if (diff > 0) {
      sdevK = sqrt(diff);
    }
  }

  *mean_freq_mhz = mhz_per_sample * (meanK - kOffset);
  *spec_width_mhz = mhz_per_sample * sdevK;

#ifdef DEBUG
  fprintf(stderr, "=================================================\n");
  fprintf(stderr, "    kMax: %d\n", kMax);
  fprintf(stderr, "    kOffset: %d\n", kOffset);
  fprintf(stderr, "    kStart: %d\n", kStart);
  fprintf(stderr, "    kEnd: %d\n", kEnd);
  fprintf(stderr, "    meanK: %g\n", meanK);
  fprintf(stderr, "    sdevK: %g\n", sdevK);
  fprintf(stderr, "    mhz_per_sample: %g\n", mhz_per_sample);
  fprintf(stderr, "    noise_mag: %g\n", noise_mag);
  fprintf(stderr, "=================================================\n");
#endif
}

/* -------------------------------------------------------------------- */
void slamsfqz(DERTBL *varp)
{
  NR_TYPE  *spec;
  int      len;
  NR_TYPE  fqz, width;

  spec = GetVector(varp, 0);
  len  = MAX_BUFFER;  //varp->depends[0]->Length;

  compute_freq(len, spec, &fqz, &width);

  PutSample(varp, fqz);
}

/* -------------------------------------------------------------------- */
void slamsws(DERTBL *varp)
{
  NR_TYPE tas, fqz, ws;

  fqz = GetSample(varp, 0);
  tas = GetSample(varp, 1);

  int nFolds = (int)(tas / MPS_PER_FOLD);
  NR_TYPE residual = fqz * (MAXFQZ / MPS_PER_FOLD);	// convert to m/s

  // If we are close to the fold, then we have to decide which side.
  if (fqz > MAXFQZ - MARGIN)
  {
    NR_TYPE ws1, ws2;
    ws = ws1 = (nFolds * MPS_PER_FOLD) + residual;
    ws2 = (nFolds * MPS_PER_FOLD) + (MAXFQZ - residual);

    if (tas > ws2 || ws2 - tas < tas - ws1)
      ws = ws2;
  }
  else	// We are well away from the fold, go with straight nFolds calc.
  {
    if (nFolds % 2 == 0)
      ws = (nFolds * MPS_PER_FOLD) + residual;
    else
      ws = (nFolds * MPS_PER_FOLD) + (MAXFQZ - residual);
  }

  PutSample(varp, ws);
}

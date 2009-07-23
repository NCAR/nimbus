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

#define MINFQZ   0.0
#define MAXFQZ 100.0
#define MARGIN   5.0

/* Compute the mean frequency, and spectrum width.
 */
static void compute_freq(int     nsamples,
                         NR_TYPE *magnitude,
                         NR_TYPE *mean_freq_mhz,
                         NR_TYPE *spec_width_mhz)
{
  int ii;
  int kCent = nsamples / 2;
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
    kMax -= nsamples;
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
  static NR_TYPE fqz2 = MISSING_VALUE;
  static NR_TYPE fqz1 = MISSING_VALUE;
  static NR_TYPE fqz0 = MISSING_VALUE;
  static NR_TYPE tas1 = MISSING_VALUE;
  static NR_TYPE tas0 = MISSING_VALUE;
  static enum {FREE, BACK, FRONT} bounce;
  static int nBounce = 0;

  NR_TYPE bfqz, ws;

  // These 'GetSample()'s must match the same order as the variables
  // are listed in the DependTable.
  fqz0 = GetSample(varp, 0);
  tas0 = GetSample(varp, 1);

  // Start tracking changes in frequency and true air speed.
  if (fqz2 == MISSING_VALUE) fqz2 = fqz1;
  if (fqz1 == MISSING_VALUE) fqz1 = fqz0;
  if (tas1 == MISSING_VALUE) tas1 = tas0;

  // Count the number of bounces off of the spectral edges,
  // these occur at every 100 MHz.
  switch (bounce) {
  case FREE:

//  printf("nBounce: %2d |      FREE       | fqz: %5.1f -> %5.1f -> %5.1f", nBounce, fqz2, fqz1, fqz0);
    if ( (fqz0 > MAXFQZ - MARGIN) && (fqz0 > fqz1) && (fqz1 > fqz2) ) {
      bounce = FRONT;
    }
    if ( (fqz0 < MINFQZ + MARGIN) && (fqz0 < fqz1) && (fqz1 < fqz2) ) {
      bounce = BACK;
    }
    break;

  case FRONT:

//  printf("nBounce: %2d |           FRONT | fqz: %5.1f -> %5.1f -> %5.1f", nBounce, fqz2, fqz1, fqz0);
    if ( (fqz0 < fqz1) && (fqz1 > fqz2) ) {
      if (tas0 > tas1) nBounce++;
      if (tas0 < tas1) nBounce--;
      bounce = FREE;
    }
    break;

  case BACK:

//  printf("nBounce: %2d | BACK            | fqz: %5.1f -> %5.1f -> %5.1f", nBounce, fqz2, fqz1, fqz0);
    if ( (fqz0 > fqz1) && (fqz1 < fqz2) ) {
      if (tas0 > tas1) nBounce++;
      if (tas0 < tas1) nBounce--;
      bounce = FREE;
    }
    break;
  }

  // Update tracking changes in frequency and true air speed.
  fqz2 = fqz1;
  fqz1 = fqz0;
  tas1 = tas0;

  // Compute bouncing frequency.
  if (nBounce % 2)  // odd
    bfqz = 2 * nBounce * 100 - fqz0;
  else              // even
    bfqz =     nBounce * 100 + fqz0;

  // Convert frequency to speed.
  ws = bfqz * 0.775;
  PutSample(varp, ws);
}

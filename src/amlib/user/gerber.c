/*
-------------------------------------------------------------------------
OBJECT NAME:	gerber.c

FULL NAME:	Gerber LWC

ENTRY POINTS:	splwcg()

DESCRIPTION:	Template for AMLIB std & user directories.

COPYRIGHT:	University Corporation for Atmospheric Research, 2010-12
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "amlib.h"

struct _running_average
{
    int		nSamples[nFeedBackTypes];
    NR_TYPE	*data_values[nFeedBackTypes];
    int		buffIndex[nFeedBackTypes];
    NR_TYPE	sum[nFeedBackTypes];
    NR_TYPE	average[nFeedBackTypes];
};
 
static const size_t baseline_lwc_seconds = 30;
static const size_t baseline_conc_seconds = 2;

// Cloud Concentration Baseline Threshold
static NR_TYPE  cloud_conc_threshold = 0.01;

// Running averages.
struct _running_average lwcg, concg;


/* -------------------------------------------------------------------- */
/* Allocate structure for running average.  Each plwcc() below needs a
 * running average for concentration and liquid water.
 */
static void allocate(struct _running_average * ra, size_t nSeconds)
{
  ra->nSamples[LOW_RATE_FEEDBACK] = nSeconds;
  ra->nSamples[HIGH_RATE_FEEDBACK] = nSeconds * cfg.HRTRate();

  if (ra->data_values[LOW_RATE_FEEDBACK] == 0)
    ra->data_values[LOW_RATE_FEEDBACK] = new NR_TYPE[ra->nSamples[LOW_RATE_FEEDBACK]];
  if (ra->data_values[HIGH_RATE_FEEDBACK] == 0)
    ra->data_values[HIGH_RATE_FEEDBACK] = new NR_TYPE[ra->nSamples[HIGH_RATE_FEEDBACK]];

  memset(ra->data_values[LOW_RATE_FEEDBACK], 0, ra->nSamples[LOW_RATE_FEEDBACK] * sizeof(NR_TYPE));
  memset(ra->data_values[HIGH_RATE_FEEDBACK], 0, ra->nSamples[HIGH_RATE_FEEDBACK] * sizeof(NR_TYPE));
}

/* Add a sample to a running average.
 */
static void add_sample(struct _running_average * ra, NR_TYPE new_value)
{
  if (++ra->buffIndex[FeedBack] >= ra->nSamples[FeedBack])
    ra->buffIndex[FeedBack] = 0;

  NR_TYPE prev_value = ra->data_values[FeedBack][ra->buffIndex[FeedBack]];
  ra->data_values[FeedBack][ra->buffIndex[FeedBack]] = new_value;
  ra->sum[FeedBack] -= prev_value;
  ra->sum[FeedBack] += new_value;
  ra->average[FeedBack] = ra->sum[FeedBack] / ra->nSamples[FeedBack];
}

/* -------------------------------------------------------------------- */
void plwcgInit(var_base *varp)
{
  memset(&lwcg, 0, sizeof(_running_average));
  memset(&concg, 0, sizeof(_running_average));
  allocate(&lwcg, baseline_conc_seconds);
  allocate(&concg, baseline_conc_seconds);

}	/* END PLWCCGINIT */

/* -------------------------------------------------------------------- */
void splwcg(DERTBL *varp)
{
  NR_TYPE plwc, concf;

  plwc  = GetSample(varp, 0);
  concf = GetSample(varp, 1);
 
  /* Baseline to a another cloud instrument to remove drift.
   */
  if (!isnan(concf))
    add_sample(&concg, concf);

  if (concg.average[FeedBack] < cloud_conc_threshold && !isnan(plwc))
    add_sample(&lwcg, -plwc);

  plwc += lwcg.average[FeedBack];
  
  PutSample(varp, plwc);
}

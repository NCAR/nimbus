/**************************** PLWCC *******************************  PLWCC
 ** Corrected PMS liquid water content (g/m3)
   Input:
     plwc - raw liquid water content
     tasx - derived true air speed
     atx  - derived ambient temperature
     psxc - derived static pressure
   Output:
     plwcc - corrected PMS liquid water content
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

/*  Values from /home/local/proj/Defaults on 23 April 1998, RLR		*/
static NR_TYPE  twire[3] = {155.0, 155.0, 155.0};	/* Wire temperature (C)	*/
static NR_TYPE  tasFac[3] = {1.0, 1.0, 1.0};		/* True airspeed factor	*/
static NR_TYPE  twireDiam[3] = {0.21, 0.21, 0.21};	/* Diameter	*/
static NR_TYPE  cloud_conc_threshold = 0.01;		/* Cloud Concentration Baseline Threshold */

// Four running averages.
struct _running_average lwc1, lwc2, conc1, conc2;


NR_TYPE kinglwcc(NR_TYPE plwc, NR_TYPE tasx, NR_TYPE atx, NR_TYPE psxc, NR_TYPE twire, NR_TYPE diam);

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
void plwccInit(var_base *varp)
{
  NR_TYPE *tmp;

  if ((tmp = GetDefaultsValue("TWIRE_PMS", varp->name)) == NULL)
  {
    sprintf(buffer,"Value set to %f in AMLIB function plwccInit.\n", twire[0]);
    LogMessage(buffer);
  }
  else
    twire[0] = tmp[0];

  if ((tmp = GetDefaultsValue("TWIRE_TASFAC", varp->name)) == NULL)
  {
    sprintf(buffer,"Value set to %f in AMLIB function plwccInit.\n", tasFac[0]);
    LogMessage(buffer);
  }
  else
    tasFac[0] = tmp[0];

  if ((tmp = GetDefaultsValue("TWIRE_DIAM", varp->name)) == NULL)
  {
    sprintf(buffer,"Value set to %f in AMLIB function plwccInit.\n", twireDiam[0]);
    LogMessage(buffer);
  }
  else
    twireDiam[0] = tmp[0];

  if ((tmp = GetDefaultsValue("CLOUD_CONC_THRESHOLD", varp->name)) == NULL)
  {
    sprintf(buffer,"Value set to %f in AMLIB function plwccInit.\n", cloud_conc_threshold);
    LogMessage(buffer);
  }
  else
    cloud_conc_threshold = tmp[0];

  memset(&lwc1, 0, sizeof(_running_average));
  memset(&conc1, 0, sizeof(_running_average));
  allocate(&lwc1, baseline_lwc_seconds);
  allocate(&conc1, baseline_lwc_seconds);

}	/* END PLWCCINIT */

/* -------------------------------------------------------------------- */
void plwcc1Init(var_base *varp)
{
  NR_TYPE *tmp;

  if ((tmp = GetDefaultsValue("TWIRE_PMS1", varp->name)) == NULL)
  {
    sprintf(buffer,"Value set to %f in AMLIB function plwccInit.\n", twire[1]);
    LogMessage(buffer);
  }
  else
    twire[1] = tmp[0];

  if ((tmp = GetDefaultsValue("TWIRE_TASFAC1", varp->name)) == NULL)
  {
    sprintf(buffer,"Value set to %f in AMLIB function plwccInit.\n", tasFac[1]);
    LogMessage(buffer);
  }
  else
    tasFac[1] = tmp[0];

  if ((tmp = GetDefaultsValue("TWIRE_DIAM1", varp->name)) == NULL)
  {
    sprintf(buffer,"Value set to %f in AMLIB function plwccInit.\n", twireDiam[1]);
    LogMessage(buffer);
  }
  else
    twireDiam[1] = tmp[0];

  if ((tmp = GetDefaultsValue("CLOUD_CONC_THRESHOLD", varp->name)) == NULL)
  {
    sprintf(buffer,"Value set to %f in AMLIB function plwccInit.\n", cloud_conc_threshold);
    LogMessage(buffer);
  }
  else
    cloud_conc_threshold = tmp[0];

  memset(&lwc2, 0, sizeof(_running_average));
  memset(&conc2, 0, sizeof(_running_average));
  allocate(&lwc2, baseline_conc_seconds);
  allocate(&conc2, baseline_conc_seconds);

}	/* END PLWCC1INIT */

/* -------------------------------------------------------------------- */
void splwcc(DERTBL *varp)
{
  NR_TYPE plwc, tasx, atx, psxc, concf;
  NR_TYPE plwcc;

  plwc  = GetSample(varp, 0);
  tasx  = GetSample(varp, 1);
  atx   = GetSample(varp, 2);
  psxc  = GetSample(varp, 3);
 
  if (tasx < 30.0)
  {
    PutSample(varp, 0.0);
    return;
  }

  tasx *= tasFac[0];
  plwcc = kinglwcc(plwc, tasx, atx, psxc, twire[0], twireDiam[0]);

  /* Baseline to a another cloud instrument to remove drift.
   */
  if (varp->ndep >= 5)
  {
    concf = GetSample(varp, 4);

    if (!isnan(concf))
      add_sample(&conc1, concf);

    if (conc1.average[FeedBack] < cloud_conc_threshold && !isnan(plwcc))
      add_sample(&lwc1, -plwcc);

    plwcc += lwc1.average[FeedBack];
  }

  PutSample(varp, plwcc);
}

/* -------------------------------------------------------------------- */
void splwcc1(DERTBL *varp)
{
  NR_TYPE plwc, tasx, atx, psxc, concf;
  NR_TYPE plwcc;

  plwc  = GetSample(varp, 0);
  tasx  = GetSample(varp, 1);
  atx   = GetSample(varp, 2);
  psxc  = GetSample(varp, 3);
 
  if (tasx < 30.0)
  {
    PutSample(varp, 0.0);
    return;
  }
  
  tasx *= tasFac[1];
  plwcc = kinglwcc(plwc, tasx, atx, psxc, twire[1], twireDiam[1]);

  /* Baseline to a another cloud instrument to remove drift.
   */
  if (varp->ndep >= 5)
  { 
    concf = GetSample(varp, 4);
    
    if (!isnan(concf))
      add_sample(&conc2, concf);

    if (conc2.average[FeedBack] < cloud_conc_threshold && !isnan(plwcc))
      add_sample(&lwc2, -plwcc);

    plwcc += lwc2.average[FeedBack];
  }
  
  PutSample(varp, plwcc);
}

/*
-------------------------------------------------------------------------
OBJECT NAME:	pms1d.c

FULL NAME:	

ENTRY POINTS:	InitPMS1Dsetup()	startup.c
		adjust_1d_times()	timer.c
		ClearAccumulation()	pms1d_util.c
		PlotPMS1Ddata()		timer.c

STATIC FNS:	scale_y()

DESCRIPTION:	3 major arrays used in PMS1D display:
			PMS1Dprobe[]	Shared memory probe info & data,
					initialized by winput.
			PMS1Dsetup[]	Quadrant info.
			PMS1Daccum[]	Period accumulation data (display data)

INPUT:		

OUTPUT:		

REFERENCES:	

REFERENCED BY:	startup.c, timer.c

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include <math.h>
#include <stdio.h>

#include <xview/xview.h>
#include <xview/cms.h>

#include "pms1d.h"
#include "display.h"
#include "ops.h"
#include "lengths.h"
#include "colors.h"
#include "proto.h"


struct quad_info	PMS1Dsetup[NUM_QUADRANTS];
struct accum_info	PMS1Daccum[NUM_QUADRANTS];

int		accum_period[NUM_DISPLAY_QUADRANTS] = {0,0,0,0};
static int	last_time_seen[NUM_DISPLAY_QUADRANTS] = {0,0,0,0};

static void	scale_y();


/* -------------------------------------------------------------------- */
void InitPMS1Dsetup()
{
  int	i;

  for (i = 0; i < NUM_DISPLAY_QUADRANTS; ++i)
    {
    PMS1Dsetup[i].nprobes	= 0;
    PMS1Dsetup[i].type		= CONCENTRATION;
    PMS1Dsetup[i].volume	= LITERS;
    PMS1Dsetup[i].ymin		= 1.0;
    PMS1Dsetup[i].ymax		= 100.0;
    PMS1Dsetup[i].sum_period	= 30;
    PMS1Dsetup[i].xtype		= LINEAR;
    PMS1Dsetup[i].ytype		= LINEAR;
    }

}	/* END INITPMS1DSETUP */

/* -------------------------------------------------------------------- */
void ClearAccumulation(int quadrant)
{
  accum_period[quadrant] = 0;

  memset((char *)&PMS1Daccum[quadrant], 0, sizeof(struct accum_info));

}	/* END CLEARACCUMULATION */

/* -------------------------------------------------------------------- */
void PlotPMS1Ddata(int quadrant, int time_elapsed)
{
  int	i, j, probenum;
  int	ytop[BINS_64];
  int	xposn, next_xposn, bin;
  int	drawType = GetDrawType();

  /* For each probe in the quadrant, draw each bin.
   */
  for (bin = 0, j = 0; j < PMS1Dsetup[quadrant].nprobes; ++j, ++bin)
    {
    probenum = PMS1Dsetup[quadrant].probenum[j];

    /* Determine size of each bin and place in ytop
     */
    scale_y(quadrant, PMS1Daccum[quadrant].data[j], ytop,
            PMS1Dprobe[probenum].nbins);

    for (i = 1; i < PMS1Dprobe[probenum].nbins; ++i, ++bin)
      {
      xposn	= PMS1Dsetup[quadrant].xposn[bin],
      next_xposn = PMS1Dsetup[quadrant].xposn[bin+1],

      (*FillArea[drawType])(
                    pw_xid[quadrant],
                    xposn,
                    ytop[i],
                    next_xposn - xposn,
                    rheight[quadrant] + rtop[quadrant] - ytop[i],
                    ProbeColor[probenum]);
      }
    }


  accum_period[quadrant] += (time_elapsed - last_time_seen[quadrant]);

  /* this can happen when input process is restarted or time
   * frame changed in backwards direction
   */
  if (accum_period[quadrant] < 0)
    {
    ClearAccumulation(quadrant);
    pms1d_start_time(quadrant, current_time);
    }

  last_time_seen[quadrant] = time_elapsed;

  /* Top of accum period or user has adjusted config, draw grey bins.
   * clear accums for next round.
   */
  if (accum_period[quadrant] >= PMS1Dsetup[quadrant].sum_period)
    {
    (*FillArea[drawType])(
               pw_xid[quadrant],
               rleft[quadrant],
               rtop[quadrant],
               rwidth[quadrant],
               rheight[quadrant]+1,
               0);

    pms1d_start_time(quadrant, current_time);

    if (isQuadrantYlog(quadrant))
      y_log10_background(quadrant);

    if (isQuadrantXlog(quadrant))
      x_log10_background(quadrant);

    if (isQuadrantCounts(quadrant))
      x_counts_background(quadrant);

    for (bin = j = 0; j < PMS1Dsetup[quadrant].nprobes; ++j, ++bin)
      {
      probenum = PMS1Dsetup[quadrant].probenum[j];

      scale_y(quadrant,
              PMS1Daccum[quadrant].data[j],
              ytop,
              PMS1Dprobe[probenum].nbins);

      for (i = 1; i < PMS1Dprobe[probenum].nbins; ++i, ++bin)
        {
        xposn	= PMS1Dsetup[quadrant].xposn[bin],
        next_xposn = PMS1Dsetup[quadrant].xposn[bin+1],

        (*FillArea[drawType])(
                      pw_xid[quadrant],
                      xposn,
                      ytop[i],
                      next_xposn - xposn,
                      rheight[quadrant] + rtop[quadrant] - ytop[i],
                      (*GetColorMap[drawType])() == LTBLU_MAP ? WHT : G5);
        }
      }

    ClearAccumulation(quadrant);	
    }

}	/* END PLOTPMS1DDATA */

/* -------------------------------------------------------------------- */
adjust_1d_times(int quadrant, int curr_time)
{
  /* deal with any pms1d display parameters that should be reset when the
   * time frame is changed
   */
  last_time_seen[quadrant] = curr_time;
}

/* -------------------------------------------------------------------- */
static void scale_y(int quadrant, float in[], int out[], int nbins)
{
  double	ratio, diff, logymax, logdata;
  int		i;

  if (isQuadrantYlog(quadrant))
    {
    logymax = log10((double)PMS1Dsetup[quadrant].ymax);
    ratio = rheight[quadrant] /
            (logymax - log10((double)PMS1Dsetup[quadrant].ymin));

    for (i = 1; i < nbins; i++) 
      {
      diff = in[i] - PMS1Dsetup[quadrant].ymin;
      if (diff <= 0.0)
        diff = 0.0001;

      /* computed for (0,0) at upper left corner, BUT offset
       * by amount needed to compensate for lack of regions
       * in xview
       */
      logdata = log10((double)diff);
      out[i] = rtop[quadrant] + ratio * (logymax - logdata);

      if (out[i] < rtop[quadrant])
        out[i] = rtop[quadrant];

      if (out[i] > rbottom[quadrant])
        out[i] = rbottom[quadrant];
      }
    }
  else
    {
    ratio = rheight[quadrant] /
            (PMS1Dsetup[quadrant].ymax - PMS1Dsetup[quadrant].ymin);

    for (i = 1; i < nbins; i++) 
      {
      diff = in[i] - PMS1Dsetup[quadrant].ymin;

      if (diff < 0.0)
        diff = 0.0;

      out[i] = rtop[quadrant] + rheight[quadrant] - diff * ratio;
      if (out[i] < rtop[quadrant])
        out[i] = rtop[quadrant];
      }
    }

}	/* END SCALE_Y */

/* END PMS1D.C */

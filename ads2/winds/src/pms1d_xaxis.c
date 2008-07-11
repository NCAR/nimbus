/*
-------------------------------------------------------------------------
OBJECT NAME:	pms1d_xaxis.c

FULL NAME:	Compute bin widths for display

ENTRY POINTS:	ReCalcXaxis()

STATIC FNS:	PMS1Dxposn()
		InitPMSdata()

DESCRIPTION:	Finds point placing along the x axis based on binwidths
		of the probes being displayed.  Should be used to recompute
		the x axis whenever a probe is added or deleted or if the
		window is resized or if the FSSP is being displayed and
		its range is changed.

INPUT:		Input is an array of pms1d probes to be plotted, fssp
		range.  rwidth, rleft must be defined before entry.

OUTPUT:		xposn_* modified

REFERENCES:	libpms.a

REFERENCED BY:	pms1d_ops.c, pms1d_util.c

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "raf.h"
#include "pms1d.h"
#include "display.h"
#include "files.h"
#include "pms.h"
#include "proto.h"

/* Exported to pms1d_util.c */
double	minx[NUM_DISPLAY_QUADRANTS], maxx[NUM_DISPLAY_QUADRANTS];

static double	*cellSize[MAX1DPROBES];

static void InitPMSdata(), PMS1Dxposn();


/* -------------------------------------------------------------------- */
void ReCalcXaxis(int quadrant)
{
  int		i;
  static bool	firstTime = TRUE;


  if (PMS1Dsetup[quadrant].nprobes == 0)
    {
    minx[quadrant] = 0.0;
    maxx[quadrant] = 100.0;
    return;
    }


  if (firstTime)		/* Read libpms info.	*/
    {
    InitPMSdata();
    firstTime = FALSE;
    }


  /* Create equally spaced xposn for each bin of each probe.
  */
  if (isQuadrantCounts(quadrant))
    {
    int		numbins, bin, probenum, j;
    float	bin_width;

    numbins = 0;

    for (i = 0; i < PMS1Dsetup[quadrant].nprobes; ++i)
      numbins += PMS1Dprobe[PMS1Dsetup[quadrant].probenum[i]].nbins - 1;

    bin_width = (float)rwidth[quadrant] / numbins;

    for (bin = 0, i = 0; i < PMS1Dsetup[quadrant].nprobes; ++i)
      {
      probenum = PMS1Dsetup[quadrant].probenum[i];

      for (j = 0; j < PMS1Dprobe[probenum].nbins; ++j, ++bin)
        PMS1Dsetup[quadrant].xposn[bin] = rleft[quadrant] +(bin_width *(bin-i));
      }

    minx[quadrant] = 0.0;
    maxx[quadrant] = numbins;
    }
  else
    {
    int		fssp_range;
    int		bin_posn, nbins, pNum;

    minx[quadrant] = 1e6;
    maxx[quadrant] = -1;

    /* find min and max values for x range */
    for (i = 0; i < PMS1Dsetup[quadrant].nprobes; i++)
      {
      pNum = PMS1Dsetup[quadrant].probenum[i];
      nbins = PMS1Dprobe[pNum].nbins;

      switch (PMS1Dprobe[pNum].type)
        {
        case FSSP:
          fssp_range = PMS1Dprobe[pNum].range;

          minx[quadrant] = MIN(minx[quadrant],
            cellSize[pNum][(nbins * fssp_range)]);

          maxx[quadrant] = MAX(maxx[quadrant],
            cellSize[pNum][(nbins * fssp_range) + nbins-1]);

          break;	

        case X260:
          minx[quadrant] = MIN(minx[quadrant], cellSize[pNum][0]);
          maxx[quadrant] = MAX(maxx[quadrant], cellSize[pNum][62]);

          break;

        default:
          minx[quadrant] = MIN(minx[quadrant], cellSize[pNum][0]);
          maxx[quadrant] = MAX(maxx[quadrant], cellSize[pNum][nbins-1]);
        }
      }

    /* Now that we have determined min and max, lets set up xposn
    */
    bin_posn = 0;

    for (i = 0; i < PMS1Dsetup[quadrant].nprobes; ++i)
      {
      pNum = PMS1Dsetup[quadrant].probenum[i];
      nbins = PMS1Dprobe[pNum].nbins;

      if (PMS1Dprobe[pNum].type == FSSP)
        {
        fssp_range = PMS1Dprobe[pNum].range;

        PMS1Dxposn(quadrant,
              &PMS1Dsetup[quadrant].xposn[bin_posn],
              &cellSize[pNum][nbins*fssp_range], nbins);
        }
      else
        {
        PMS1Dxposn(quadrant,
              &PMS1Dsetup[quadrant].xposn[bin_posn],
              cellSize[pNum], nbins);
        }

      bin_posn += nbins;
      }
/*
fprintf(stderr, "-------------------------\n");
for (i = 0; i < bin_posn; ++i)
  fprintf(stderr, "%d ", PMS1Dsetup[quadrant].xposn[i]);
fprintf(stderr, "\n");
*/
    }

}	/* END RECALCXAXIS */

/* -------------------------------------------------------------------- */
/* find pixel range for equal size bins
 * rwidth, rleft must be defined before entry
 */
static void PMS1Dxposn(int quadrant, int xposn[], double *size, int nbins)
{
  int		i, right;
  double	nPixels, logxmax;

  right = rleft[quadrant]+rwidth[quadrant];

  if (isQuadrantXlinear(quadrant))
    {
    nPixels = (float)rwidth[quadrant] / (maxx[quadrant]-minx[quadrant]);

    for (i = 0; i < nbins; ++i)
      {
      xposn[i] = (size[i] - minx[quadrant]) * nPixels + rleft[quadrant];

      if (xposn[i] < rleft[quadrant])
        xposn[i] = rleft[quadrant];

      if (xposn[i] > right)
        xposn[i] = right;
      }
    }
  else
    {
    logxmax = log10(maxx[quadrant]);
    nPixels = rwidth[quadrant]/(logxmax - log10(minx[quadrant]));

    for (i = 0; i < nbins; ++i)
      {
      if (i <= minx[quadrant]) 
        xposn[i] = rleft[quadrant];
      else
        /* position computed from right edge*/
        xposn[i] = rwidth[quadrant] - (logxmax - log10(size[i]))
                    * nPixels + rleft[quadrant];

      if (xposn[i] < rleft[quadrant])
        xposn[i] = rleft[quadrant];

      if (xposn[i] > right)
        xposn[i] = right;
      }
    }

}	/* END PMS1DXPOSN */

/* -------------------------------------------------------------------- */
static void InitPMSdata()
{
  char	*p, buffer[1024];
  int	i, j, nBins;

  sprintf(buffer, PMS_SPEC_FILE, ProjectDirectory, proj_number);
  InitPMSspecs(buffer);

  for (i = 0; i < NumberPMS1Dprobes(); ++i)
    {
    cellSize[i] = (double *)GetMemory(sizeof(double) * PMS1Dprobe[i].nbins);

    if (PMS1Dprobe[i].type == FSSP)
      nBins = PMS1Dprobe[i].nbins * 4;
    else
      nBins = PMS1Dprobe[i].nbins;

    sprintf(buffer, "CELL_SIZE_%d", PMS1Dprobe[i].nbins-1);
    if ( (p = GetPMSparameter(PMS1Dprobe[i].SerialNumber, buffer)) ||
         (p = GetPMSparameter(PMS1Dprobe[i].SerialNumber, "CELL_SIZE")) )
      {
      strcpy(buffer, p);
      p = strtok(buffer, " \t,");

      for (j = 0; j < PMS1Dprobe[i].nbins; ++j)
        {
        cellSize[i][j] = atof(p);

        if (j > 0)
          PMS1Dprobe[i].width[j] = cellSize[i][j] - cellSize[i][j-1];
        else
          PMS1Dprobe[i].width[j] = 1.0;

        if (PMS1Dprobe[i].width[j] == 0.0)
          PMS1Dprobe[i].width[j] = 1.0;

        p = strtok(NULL, " \t,");
        }
      }
    else
      {
      float	min, max, step, cell;

      p = GetPMSparameter(PMS1Dprobe[i].SerialNumber, "MIN_RANGE");

      if (p == NULL)
        {
        fprintf(stderr, "PMSspecs file incomplete for Serial # %s.\n", PMS1Dprobe[i].SerialNumber);
        exit(1);
        }

      min = atof(p);
      p = GetPMSparameter(PMS1Dprobe[i].SerialNumber, "MAX_RANGE");

      if (p == NULL)
        {
        fprintf(stderr, "PMSspecs file incomplete for Serial # %s.\n", PMS1Dprobe[i].SerialNumber);
        exit(1);
        }

      max = atof(p);
      p = GetPMSparameter(PMS1Dprobe[i].SerialNumber, "RANGE_STEP");

      if (p == NULL)
        {
        fprintf(stderr, "PMSspecs file incomplete for Serial # %s.\n", PMS1Dprobe[i].SerialNumber);
        exit(1);
        }

      step = atof(p);

      for (j = 0, cell = min; cell <= max; cell += step, ++j)
        cellSize[i][j] = cell;
      }
    }

  ReleasePMSspecs();

}	/* END INITPMSDATA */

/* END PMS1D_XAXIS.C */

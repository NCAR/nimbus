/*
-------------------------------------------------------------------------
OBJECT NAME:	pms1d_ops.c

FULL NAME:	PMS1D Callbacks

ENTRY POINTS:	TogglePMS1Ddatatype()		op_panels.c
		TogglePMS1Dvolume()
		TogglePMS1DxaxisType()
		TogglePMS1DyaxisType()
		set_period()
		minyrange()
		maxyrange()
		isPMS1DprobeSelected()
		TogglePMS1Dprobe()
		get_1d_args()
		pmszoom()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	

REFERENCED BY:	op_panels.c

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "lengths.h"
#include "colors.h"
#include "errors.h"
#include "ops.h"
#include "plotspex.h"
#include "pms1d.h"
#include "proto.h"


/* -------------------------------------------------------------------- */
void TogglePMS1Ddatatype(int quadrant)
{
  if (PMS1Dsetup[quadrant].type == COUNTS)
    PMS1Dsetup[quadrant].type = CONCENTRATION;
  else
    {
    PMS1Dsetup[quadrant].type = COUNTS;

    /* ensure xaxis is set to linear -- log scale not allowed
     * with counts data
     */
    if (!isQuadrantXlinear(quadrant))
      TogglePMS1DxaxisType(quadrant);
    }

  update_mode[quadrant] = RECONFIG;

}	/* END TOGGLEPMS1DDATATYPE */

/* -------------------------------------------------------------------- */
void TogglePMS1Dvolume(int quadrant)
{
  if (isQuadrantCounts(quadrant))
    return;

  if (isQuadrantYliters(quadrant))
    {
    PMS1Dsetup[quadrant].volume = CCs;
    PMS1Dsetup[quadrant].ymax *= 0.001;
    }
  else
    {
    PMS1Dsetup[quadrant].volume = LITERS;
    PMS1Dsetup[quadrant].ymax *= 1000.0;
    }


  if (PMS1Dsetup[quadrant].ymax <= 1.0)
    PMS1Dsetup[quadrant].ymax = 1.0;

  if (PMS1Dsetup[quadrant].ymax <= PMS1Dsetup[quadrant].ymin)
    {
    if (PMS1Dsetup[quadrant].ymin > 10.0)
      PMS1Dsetup[quadrant].ymin = 1.0;
    else
      PMS1Dsetup[quadrant].ymax = PMS1Dsetup[quadrant].ymin + 1.0;
    }

  update_mode[quadrant] = RECONFIG;

}	/* END TOGGLEPMS1DVOLUME */

/* -------------------------------------------------------------------- */
void TogglePMS1DxaxisType(int quadrant)
{
  if (PMS1Dsetup[quadrant].xtype == LINEAR)
    PMS1Dsetup[quadrant].xtype = LOG;
  else
    PMS1Dsetup[quadrant].xtype = LINEAR;

  update_mode[quadrant] = RECONFIG;
}

/* -------------------------------------------------------------------- */
void TogglePMS1DyaxisType(int quadrant)
{
  if (PMS1Dsetup[quadrant].ytype == LINEAR)
    {
    int		x;
    double	d;

    PMS1Dsetup[quadrant].ytype = LOG;

    x = (int)log10(PMS1Dsetup[quadrant].ymin);
    PMS1Dsetup[quadrant].ymin = pow((double)10.0,(double)x);

    x = d = log10(PMS1Dsetup[quadrant].ymax);
    if (d - x != 0.0) ++x;
      PMS1Dsetup[quadrant].ymax = pow((double)10.0,(double)x);
    }
  else
    PMS1Dsetup[quadrant].ytype = LINEAR;

  update_mode[quadrant] = RECONFIG;

}	/* END TOGGLEPMS1DYAXISTYPE */

/* -------------------------------------------------------------------- */
void set_period(int quadrant, char kybdentry[])
{
  if ((PMS1Dsetup[quadrant].sum_period = abs(atoi(kybdentry))) < 1)
    PMS1Dsetup[quadrant].sum_period = 1;

  update_mode[quadrant] = RECONFIG;
}

/* -------------------------------------------------------------------- */
minyrange(int quadrant, char kybdentry[])
{
  double	miny;

  miny = atof(kybdentry);

  if (isQuadrantYlog(quadrant))
    {
    int	x;

    x = (int)log10(miny);
    miny = pow((double)10.0,(double)x);
    }

  if (miny < 0.0)
    miny = 0.0;

  if (miny >= PMS1Dsetup[quadrant].ymax)
    {
    set_notice_needed(MIN_GT_MAX);
    miny = PMS1Dsetup[quadrant].ymin;
    }

  PMS1Dsetup[quadrant].ymin = miny;
  update_mode[quadrant] = RECONFIG;
}

/* -------------------------------------------------------------------- */
maxyrange(int quadrant, char kybdentry[])
{
  double	maxy;

  maxy = atof(kybdentry);

  if (isQuadrantYlog(quadrant))
    {
    int		x;
    double	d;

    x = d = log10(maxy);
    if (d - x != 0.0) ++x;
      maxy = pow((double)10.0,(double)x);
    }

  if (maxy < 0.0)
    maxy = 0.0;

  if (maxy <= PMS1Dsetup[quadrant].ymin)
    {
    set_notice_needed(MIN_GT_MAX);
    maxy = PMS1Dsetup[quadrant].ymax;
    }

  PMS1Dsetup[quadrant].ymax = maxy;
  update_mode[quadrant] = RECONFIG;
}

/* -------------------------------------------------------------------- */
bool isPMS1DprobeSelected(int quadrant, int probenum)
{
  int	i;

  for (i = 0; i < PMS1Dsetup[quadrant].nprobes; ++i)
    if (PMS1Dsetup[quadrant].probenum[i] == probenum)
      return(TRUE);

  return(FALSE);

}	/* END ISPMS1DPROBESELECTED */

/* -------------------------------------------------------------------- */
void TogglePMS1Dprobe(int quadrant, int probenum)
{
  int	i;
  bool	DeleteProbe = FALSE;

  if (probenum >= NumberPMS1Dprobes())
    return;


  for (i = 0; i < PMS1Dsetup[quadrant].nprobes; ++i)
    if (PMS1Dsetup[quadrant].probenum[i] == probenum)
      {
      DeleteProbe = TRUE;
      break;
      }

  if (DeleteProbe)
    {
    for (; i < PMS1Dsetup[quadrant].nprobes; ++i)
      PMS1Dsetup[quadrant].probenum[i] = PMS1Dsetup[quadrant].probenum[i+1];

    --PMS1Dsetup[quadrant].nprobes;
    }
  else
    {
    PMS1Dsetup[quadrant].probenum[PMS1Dsetup[quadrant].nprobes] =
    probenum;

    ++PMS1Dsetup[quadrant].nprobes;
    }


  update_mode[quadrant] = RECONFIG;

}	/* END TOGGLEPMS1DPROBE */

/* -------------------------------------------------------------------- */
get_1d_args(int quadrant, int entry)
{
  FILE	*infile;
  int	i, j, nProbes;
  char	keyword[10];
  char	dummy[15];

  PMS1Dsetup[quadrant].nprobes		= 0;
  PMS1Dsetup[quadrant].type		= CONCENTRATION;
  PMS1Dsetup[quadrant].volume		= LITERS;
  PMS1Dsetup[quadrant].xtype		= LINEAR;
  PMS1Dsetup[quadrant].ytype		= LINEAR;
  PMS1Dsetup[quadrant].ymin		= 1.0;
  PMS1Dsetup[quadrant].ymax		= 100.0;
  PMS1Dsetup[quadrant].sum_period	= 30;

  if (NewSetup(entry))
    {
    set_geom(quadrant,get_default_geom());
    return(TRUE);
    }

  if ((infile = WINDSfopen((char *)GetPlotspexFilename(), "r")) == NULL)
    {
    fprintf(stderr,"WARNING: no plotspex file created for project %s!\n", proj_number);
    return(TRUE);
    } 


  find_plotspec_entry(&infile, entry);

  while (fscanf(infile, "%s", keyword) == 1)
    {
    if (!strcmp(keyword,"END_ARGS"))
      {
      WINDSfclose(infile);
      return(TRUE);
      }

    if (strcmp(keyword, "NPLPROBES") == 0)
      {
      fscanf(infile, "%d", &nProbes);
      continue;
      }

    if (strcmp(keyword, "PLPROBES") == 0)
      {
      for (i = 0; i < nProbes; ++i)
        {
        fscanf(infile, "%s", dummy);
        strupr(dummy);

        for (j = 0; j < NumberPMS1Dprobes(); j++)
          if (strcmp(PMS1Dprobe[j].name, dummy) == 0)
            {
            PMS1Dsetup[quadrant].probenum[i] = j;
            ++PMS1Dsetup[quadrant].nprobes;
            break;
            }
        }

      continue;
      }

    if (strcmp(keyword, "DATATYPE") == 0)
      {
      fscanf(infile, "%d", &PMS1Dsetup[quadrant].type);
      continue;
      }

    if (strcmp(keyword, "XTYPE") == 0)
      {
      fscanf(infile, "%d", &PMS1Dsetup[quadrant].xtype);
      continue;
      }

    if (strcmp(keyword,"GEOMETRY") == 0) 
      {
      int geometry;
      fscanf(infile, "%d", &geometry);
      set_geom(quadrant,geometry);
      continue;
      }

    if (strcmp(keyword, "YTYPE") == 0)
      {
      fscanf(infile, "%d", &PMS1Dsetup[quadrant].ytype);
      continue;
      }

    if (strcmp(keyword, "YMIN") == 0)
      {
      fscanf(infile, "%lf", &PMS1Dsetup[quadrant].ymin);
      continue;
      }

    if (strcmp(keyword, "YMAX") == 0)
      {
      fscanf(infile, "%lf", &PMS1Dsetup[quadrant].ymax);
      continue;
      }

    if (strcmp(keyword, "SUM_PERIOD") == 0)
      {
      fscanf(infile, "%d", &PMS1Dsetup[quadrant].sum_period);
      continue;
      }

    if (strcmp(keyword, "YSCALE") == 0)
      {
      fscanf(infile, "%d", &PMS1Dsetup[quadrant].volume);
      continue;
      }
    }

  return(TRUE);

}	/* END GET_1D_ARGS */

/* -------------------------------------------------------------------- */
void pmszoom(int quadrant, int klik, int direct)
{
  float	yFactor;


  if (direct == ZOOM)
    yFactor = pow((double)0.1, (double)klik);
  else
    yFactor = pow((double)10.0, (double)klik);


  switch (pending_op[quadrant])
    {
    case PMS1D_PERIOD:
      if (direct == ZOOM)
        PMS1Dsetup[quadrant].sum_period -= 5*klik;
      else
        PMS1Dsetup[quadrant].sum_period += 5*klik;

      if (PMS1Dsetup[quadrant].sum_period < 1)
        PMS1Dsetup[quadrant].sum_period = 1;

      break;

    case PMS1D_MINY:
      PMS1Dsetup[quadrant].ymin *= yFactor;

      if (PMS1Dsetup[quadrant].ymin < 0.0)
        PMS1Dsetup[quadrant].ymin = 0.0;

      if (PMS1Dsetup[quadrant].ymin>PMS1Dsetup[quadrant].ymax)
        {
        set_notice_needed(MIN_GT_MAX);
        PMS1Dsetup[quadrant].ymin /= yFactor;
        }

      break;

    case PMS1D_MAXY:
      PMS1Dsetup[quadrant].ymax *= yFactor;

      if (PMS1Dsetup[quadrant].ymin>PMS1Dsetup[quadrant].ymax)
        {
        set_notice_needed(MIN_GT_MAX);
        PMS1Dsetup[quadrant].ymax /= yFactor;
        }

      break;
    }

  update_mode[quadrant] = RECONFIG;

}	/* END PMSZOOM */

/* END PMS1D_OPS.C */

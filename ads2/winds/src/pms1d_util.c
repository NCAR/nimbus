/*
-------------------------------------------------------------------------
OBJECT NAME:	pms1d_util.c

FULL NAME:	

ENTRY POINTS:	PlotPMS1Dbackground()	timer.c
		pms1d_start_time()	pms1d.c & local
		list_sum()		event_ops.c & pms1d_ops.c
		ymin_ypos()		event_ops.c
		divider_ypos()		event_ops.c & local
		label_1d_ymin()		event_ops.c
		label_1d_ymax()		event_ops.c
		x_counts_background()	pms1d.c, pms1d_ops.c & local
		x_log10_background()	pms1d.c, pms1d_ops.c & local
		y_log10_background()	pms1d.c & local

STATIC FNS:	DrawPMS1Dbox()
		DisplayProbeNames()
		y_labels()
		x_labels()

DESCRIPTION:	Plotting Utilities.

INPUT:		

OUTPUT:		

REFERENCES:	

REFERENCED BY:	See Entry Points

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

/*
following are the PMS1D layout parameters for text positioning management:
*/
#define PMS1D_LABEL_GAP         2 
#define PMS1D_TOP_MARGIN (GetWINDSWindowHeight(quadrant)/21)
#define PMS1D_LEFT_MARGIN (GetWINDSWindowWidth(quadrant)/70)
#define PMS1D_RIGHT_MARGIN (GetWINDSWindowWidth(quadrant)/36)

/*
FONTID: font ID
*/
#define FONTID(q) (GetFontID(Get1dDisplayFont(q)))
/*
FONTWIDTH: width in pixels of text rectangle
*/
#define FONTWIDTH(q) (GetFontWidth(Get1dDisplayFont(q)))
/*
FONTHEIGHT: height in pixels of text rectangle, inclusive of ascent and descent
*/
#define FONTHEIGHT(q) (GetFontHeight(Get1dDisplayFont(q)))
/*
FONTDESCENT: height in pixels of descending portion of text rectangle
*/
#define FONTDESCENT(q) (GetFontDescent(Get1dDisplayFont(q)))
/*
FONTASCENT: height in pixels of ascending portion of text rectangle
*/
#define FONTASCENT(q) (GetFontAscent(Get1dDisplayFont(q)))
/*
BOTTOMMOST_PIXEL: southern-most drawable pixel in drawing window
*/
#define BOTTOMMOST_PIXEL(q) (GetWINDSWindowHeight(q)-1)
/*
YPOS_PROBENAMES: y-position for probe names: leave margin of two nominal
gaps plus pixels from font baseline to bottom of text rectangle (descent)
*/
#define YPOS_PROBENAMES(q) (BOTTOMMOST_PIXEL(q)-2*PMS1D_LABEL_GAP-FONTDESCENT(q))
/*
YPOS_TIME: y-position for "Start time" label: pixels in height of font 
above baseline of probe names
*/
#define YPOS_TIME(q) (YPOS_PROBENAMES(q)-FONTHEIGHT(q))
/*
YPOS_DIVIDER: y-position for horizontal line above "Start time" label: pixels 
in height of font plus one nominal gap above baseline of time label
*/
#define YPOS_DIVIDER(q) (YPOS_TIME(q)-FONTHEIGHT(q)-PMS1D_LABEL_GAP)
/*
YPOS_XLABELS: one nominal gap plus pixels in font descent above divider line
*/
#define YPOS_XLABELS(q) (YPOS_DIVIDER(q)-PMS1D_LABEL_GAP-FONTDESCENT(q))
/*
YPOS_RBOTTOM: y-position of line drawn at bottom of plotting region: pixels
in height of font plus one nominal gap above baseline of X-labels
*/
#define YPOS_RBOTTOM(q) (YPOS_XLABELS(q)-FONTHEIGHT(q)-PMS1D_LABEL_GAP)

#include <stdio.h>
#include <math.h>

#include <xview/xview.h>

#include "pms1d.h"
#include "colors.h"
#include "errors.h"
#include "lengths.h"
#include "proto.h"


/* relate probe num to text color */
int	ProbeColor[] = {RED, GRN, PUR, BLU, VIO, IND, CYN };

static int	drawType;


/****************************************************************************/

Get1dFontWidth(quadrant)
int quadrant;
{
 return (GetWINDSWindowWidth(quadrant)/62);
}

/****************************************************************************/

Get1dFontHeight(quadrant)
int quadrant;
{
 return (GetWINDSWindowHeight(quadrant)/18);
}

/*****************************************************************************/

Get1dAreaTooSmall(quadrant) 
int quadrant; 
{
 return FALSE; 
}

/* -------------------------------------------------------------------- */
/* return the y-coordinate for the imaginary line dividing the plot area 
   into ymax and ymin areas
*/

ymin_ypos(quadrant)
int	quadrant;
{
	return(rbottom[quadrant] / 2);
}

/* -------------------------------------------------------------------- */
/* return the y-coordinate for the horizontal line dividing the plot area 
   from the status area on 1d window
*/

divider_ypos(quadrant)
int	quadrant;
{
 return YPOS_DIVIDER(quadrant);
}

/* -------------------------------------------------------------------- */
static void DrawPMS1Dbox(quadrant)
int	quadrant;
{
	int	rright;
  
	rright = rleft[quadrant]+rwidth[quadrant];

	(*SetContext[drawType])(
		pw_xid[quadrant],
		GXcopy,
		FOREGROUND,
		0,
		LineSolid,
		0);

	/* Draw box
	 */
	(*DrawLine[drawType])(
		rleft[quadrant]-1, rtop[quadrant]-1,
		rleft[quadrant]-1, rbottom[quadrant]+1);

	(*DrawLine[drawType])(
		rleft[quadrant]-1, rbottom[quadrant]+1,
		rright, rbottom[quadrant]+1);

	(*DrawLine[drawType])(
		rright, rbottom[quadrant]+1,
		rright, rtop[quadrant]-1);

	(*DrawLine[drawType])(
		rright, rtop[quadrant]-1,
		rleft[quadrant]-1, rtop[quadrant]-1);

	/* Draw divider line
	 */
	(*DrawLine[drawType])(
		0, 
		divider_ypos(quadrant)+2,
		GetWINDSWindowWidth(quadrant), divider_ypos(quadrant)+2);
}

/* -------------------------------------------------------------------- */
static void DisplayProbeNames(quadrant)
int quadrant;
{
	int	i, probenum;
	int	charstart = 8;

	for (i = 0; i < PMS1Dsetup[quadrant].nprobes; ++i)
		{
		probenum = PMS1Dsetup[quadrant].probenum[i];

		(*SetContext[drawType])(
			pw_xid[quadrant],
			GXcopy,
			ProbeColor[probenum],
			0,
			0,
			FONTID(quadrant)); 

		(*DrawText[drawType])(
			charstart,
			YPOS_PROBENAMES(quadrant),
			PMS1Dprobe[probenum].name);

		charstart += FONTWIDTH(quadrant) * 
                 (strlen(PMS1Dprobe[probenum].name)+1);
		}

}	/* END DISPLAYPROBENAMES */

/* -------------------------------------------------------------------- */
static void x_labels(quadrant)
int	quadrant;
{
	int	i, spacing;
	float	diff, n;
	char	label[80];

	(*SetContext[drawType])(
		pw_xid[quadrant],
		GXcopy,
		FOREGROUND,
		0,
		0,
		FONTID(quadrant));

	diff	= (maxx[quadrant] - minx[quadrant]) / 5;
	spacing	= rwidth[quadrant] / 5;

	for (i = 0; i < 6; ++i)
		{
		n = minx[quadrant] + diff*i;

		if (n >= 10.0)
			sprintf(label, "%.0f  ", minx[quadrant] + diff*i);
		else
			sprintf(label, "%.1f  ", minx[quadrant] + diff*i);

		(*DrawText[drawType])(
		 rleft[quadrant]-10 + i*spacing,
		 YPOS_XLABELS(quadrant),
		 label);
		}

}	/* END X_LABELS */

/* -------------------------------------------------------------------- */
static void y_labels(quadrant)
int	quadrant;
{
	int	k, mid, top;
	int	nytit;
	char	temp[2];
	static char *ytitle[] = {"   Counts   ","Number/liter"," Number/cc  "};


	temp[1]	= '\0';
	nytit	= 12; /* string length of longest of above ytitle[]'s */


	mid = rtop[quadrant] + (rbottom[quadrant] - rtop[quadrant] -
/*
allow for font height of y-axis minimum range label
*/
         FONTHEIGHT(quadrant)) / 2;
	top = mid - (nytit / 2) * (FONTHEIGHT(quadrant));

	(*SetContext[drawType])(
		pw_xid[quadrant],
		GXcopy,
		FOREGROUND,
		0,
		0,
		FONTID(quadrant)); 

	/* Write vertical string one character at a time.
	 */
	for (k = 0; k < nytit; k++)
		{
		if (isQuadrantCounts(quadrant))
			temp[0] = ytitle[0][k];
		else
		if (isQuadrantYliters(quadrant))
			temp[0] = ytitle[1][k];
		else
			temp[0] = ytitle[2][k];

		(*DrawText[drawType])(
			PMS1D_LEFT_MARGIN,
/*
do k+1 instead of just k because you want TOP of topmost font to be at top,
so you need to add its font height to make it so...
*/
			top+(k+1)*(FONTHEIGHT(quadrant)),
			temp);
		}

	if (isQuadrantYlog(quadrant) && PMS1Dsetup[quadrant].ymin <= 0.0)
		{
		set_notice_needed(LOG_LESS_OR_EQUAL_ZERO);
		PMS1Dsetup[quadrant].ymin = 1.0;
		}

	label_1d_ymax(quadrant, FOREGROUND, BKGND);
	label_1d_ymin(quadrant, FOREGROUND, BKGND);

}	/* END Y_LABELS */

/* -------------------------------------------------------------------- */
label_1d_ymax(quadrant, foregnd, bkgnd)
int	quadrant;
int	foregnd;
int	bkgnd;
{
	char	label[VALUELEN];  /* char version of range label*/

	/* draw in new string
	*/
	(*SetContext[drawType])(
		pw_xid[quadrant],
		GXcopy,
		foregnd,
		bkgnd,
		0,
		FONTID(quadrant)); 

	sprintf(label, "%6G", PMS1Dsetup[quadrant].ymax);

	(*DrawText[drawType])(
		0,
		rtop[quadrant]+2,
		label);
}

/* -------------------------------------------------------------------- */
label_1d_ymin(quadrant,foregnd,bkgnd)
int	quadrant;
int	foregnd;
int	bkgnd;
{
	char	label[VALUELEN];  /* char version of range label*/

	/* draw in new string
	 */
	(*SetContext[drawType])(
		pw_xid[quadrant],
		GXcopy,
		foregnd,
		bkgnd,
		0,
		FONTID(quadrant)); 

	sprintf(label, "%6G", PMS1Dsetup[quadrant].ymin);

	(*DrawText[drawType])(
		0,
		rbottom[quadrant],
		label);
}

/* -------------------------------------------------------------------- */
pms1d_start_time(quadrant,curr_time)
int	quadrant;
char	curr_time[];
{
	char	time_label[64];
	int	timex; 

	timex= 50;
	strcpy(time_label, "Start time ");
	strcat(time_label, curr_time);

	(*SetContext[drawType])(
		pw_xid[quadrant],
		GXcopy,
		FOREGROUND,
		0,
		0,
		FONTID(quadrant)); 

	(*DrawText[drawType])(
		timex,
		YPOS_TIME(quadrant),
		time_label);

}	/* END PMS1D_START_TIME */

/* -------------------------------------------------------------------- */
void list_sum(quadrant,foregnd,bkgnd)
int	quadrant,foregnd,bkgnd;
{
	char	sum[35];
	int	xposn;

	xposn = GetWINDSWindowWidth(quadrant) / 2.0;
	sprintf(sum, "accumulation period %d secs  ",
					PMS1Dsetup[quadrant].sum_period);

	(*SetContext[drawType])(
		pw_xid[quadrant],
		GXcopy,
		foregnd,
		bkgnd,
		0,
		FONTID(quadrant)); 

	(*DrawText[drawType])(xposn, YPOS_TIME(quadrant), sum);

}	/* END LIST_SUM */

/* -------------------------------------------------------------------- */
/* xaxis, put log10 background on plot*/
void x_log10_background(quadrant)
int	quadrant;
{
	int	rright;
	int	val;
	char	tic_label[20];
	double	i;
	double	start,ratio;
	double	logxmax,incr; 
  
	logxmax	= log10((double)maxx[quadrant]);
	ratio	= rwidth[quadrant]/(logxmax-log10((double)minx[quadrant]));
	rright	= rleft[quadrant]+rwidth[quadrant];

	/* ranges of minx hardwired here because that axis' range is not
	 * changeable by user -- it is function of probes being displayed
	 */
	if (minx[quadrant] >= 0.1   && minx[quadrant] < 1.0)	start = 0.1;
	if (minx[quadrant] >= 1.0   && minx[quadrant] < 10.0)	start = 1.0;
	if (minx[quadrant] >= 10.0  && minx[quadrant] < 100.0)	start = 10.0;
	if (minx[quadrant] >= 100.0 && minx[quadrant] < 1000.0)	start = 100.0;

	(*SetContext[drawType])(
		pw_xid[quadrant],
		GXcopy,
		FOREGROUND,
		0,
		0,
		FONTID(quadrant));

	sprintf(tic_label, "%.1f", minx[quadrant]);

	(*DrawText[drawType])(
		rleft[quadrant] - 4,
		YPOS_XLABELS(quadrant),
		tic_label);

	while (start < start * 10 && start <= maxx[quadrant])
		{
		incr = start;
#ifdef DEBUG
printf(" minx[quadrant]=%f, incr=%f, start=%f\n", minx[quadrant],incr,start);
#endif

		(*SetContext[drawType])(
			pw_xid[quadrant],
			GXcopy,
			7,
			0,
			LineSolid,
			0);

		for (i = start; i < start*10 && i < maxx[quadrant]; i += incr)
			{
			if (i < minx[quadrant])
				continue;

			val = rright - ratio*(logxmax - log10(i));
#ifdef DEBUG
printf(" i=%f, diff=%f, ratio=%f, val=%d\n",i,logxmax-log10(i),ratio,val);
#endif
			(*DrawLine[drawType])(
				val,
				rbottom[quadrant],
				val,
				rtop[quadrant]);
			}

		start *= 10;

		if (start > maxx[quadrant])
			sprintf(tic_label, "%.1f", maxx[quadrant]);
		else
			sprintf(tic_label, "%d", (int)start);

		(*SetContext[drawType])(
			pw_xid[quadrant],
			GXcopy,
			FOREGROUND,
			0,
			0,
			FONTID(quadrant));

		(*DrawText[drawType])(
			val - 7,
			YPOS_XLABELS(quadrant),
			tic_label);
		}

}	/* X_LOG10_BACKGROUND */

/* -------------------------------------------------------------------- */
/* yaxis, put log10 background on concentration plot*/
void y_log10_background(quadrant)
int	quadrant;
{
	XPoint	ptlist[200];
	int	npts, rright;
	double	i;
	int	j, val;
	double	start, incr;
	double	logymax, ratio, miny;
  
	logymax = log10((double)PMS1Dsetup[quadrant].ymax);
	ratio = rheight[quadrant]/(logymax - log10(PMS1Dsetup[quadrant].ymin));
	miny = PMS1Dsetup[quadrant].ymin;

	start	= miny;
	rright	= rleft[quadrant]+rwidth[quadrant];

	(*SetContext[drawType])(
		pw_xid[quadrant],
		GXcopy,
		7, 0, 0, 0);

	while (start < start * 10 && start <= PMS1Dsetup[quadrant].ymax)
		{
		incr = start;

		for (i = start; i < start*10 && i < PMS1Dsetup[quadrant].ymax; )
			{
			if (i < miny)
				continue;

			val = rtop[quadrant] + ratio*(logymax - log10(i));

			npts=0;
			for (j = rleft[quadrant]+1; j < rright; j += 10)
				{
				ptlist[npts].x = (short)j;
				ptlist[npts].y = (short)val;
				npts++;
				}

			(*DrawPolypoint[drawType])(npts, ptlist);

			i += incr;
			}

		start *= 10;
		}

}	/* Y_LOG10_BACKGROUND */

/* -------------------------------------------------------------------- */
/* xaxis - put grid marks every 5th bin as background on plot*/
void x_counts_background(quadrant)
int	quadrant;
{
	int	i, total_bins = 0;
	int	val;
	char	label[8];
  
	for (i = 0; i < PMS1Dsetup[quadrant].nprobes; ++i)
		total_bins +=
			PMS1Dprobe[PMS1Dsetup[quadrant].probenum[i]].nbins - 1;


	for (i = 5; i < total_bins; i += 5)
		{
		val = rleft[quadrant]
			+ ((float)i / (float)total_bins)
			* rwidth[quadrant];

		(*SetContext[drawType])(
			pw_xid[quadrant],
			GXcopy,
			7,
			0,
			LineSolid,
			0);

		(*DrawLine[drawType])(
			val, rbottom[quadrant],
			val, rtop[quadrant]);

		(*SetContext[drawType])(
			pw_xid[quadrant],
			GXcopy,
			FOREGROUND,
			0,
			0,
			FONTID(quadrant));

		sprintf(label, "%d", i);
		(*DrawText[drawType])(
			val - (rwidth[quadrant] / total_bins)+2,
			YPOS_XLABELS(quadrant),
			label);
		}

}	/* X_COUNTS_BACKGROUND */

/* -------------------------------------------------------------------- */
void PlotPMS1Dbackground(quadrant)
int	quadrant;
{
	drawType = GetDrawType();

	rbottom[quadrant] = YPOS_RBOTTOM(quadrant);
	rtop[quadrant]	= PMS1D_TOP_MARGIN;
	rheight[quadrant] = rbottom[quadrant] - rtop[quadrant];
	rleft[quadrant] = PMS1D_LEFT_MARGIN+FONTWIDTH(quadrant)* 6;
	rwidth[quadrant] = GetWINDSWindowWidth(quadrant) - rleft[quadrant]
				- PMS1D_RIGHT_MARGIN;

	DrawPMS1Dbox(quadrant);
	DisplayProbeNames(quadrant);
	pms1d_start_time(quadrant,current_time);
	list_sum(quadrant,FOREGROUND,BKGND);
	y_labels(quadrant);

	if (PMS1Dsetup[quadrant].nprobes > 0)
		{
		ReCalcXaxis(quadrant);

		if (isQuadrantYlog(quadrant))
			y_log10_background(quadrant);

		if (isQuadrantCounts(quadrant))
			x_counts_background(quadrant);
		else
		if (isQuadrantXlog(quadrant))
			x_log10_background(quadrant);
		else
			x_labels(quadrant);
		}

}	/* END PLOTPMS1DBACKGROUND */

/* END PMS1D_UTIL.C */

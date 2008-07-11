/* 
plots.c -- handle xy-plot plotting utilities.  
*/
#include <xview/xview.h>
#include <xview/canvas.h>
#include <xview/svrimage.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/****************************  WINDS INCLUDES  ******************************/

#include <xy_plt.h>
#include <display.h>
#include <constants.h>
#include <lengths.h>
#include <ops.h>
#include <help.h>
#include <colors.h>
#include <errors.h>
#include <xview.h>
#include <shmem.h>
#include <macros.h>
#include <vectors.h>
#include <proto.h>

/*
layout
*/
#define DATA_LABELLEN 6
/*
FONTID: font ID
*/
#define FONTID(font) (GetFontID(font))
/*
FONTWIDTH: width in pixels of text rectangle
*/
#define FONTWIDTH(font) (GetFontWidth(font))
/*
FONTHEIGHT: height in pixels of text rectangle, inclusive of ascent and descent
*/
#define FONTHEIGHT(font) (GetFontHeight(font))
/*
FONTDESCENT: height in pixels of descending portion of text rectangle
*/
#define FONTDESCENT(font) (GetFontDescent(font))
/*
FONTINFO: FontInfo structure pointer
*/
#define FONTINFO(font)	  (GetFontInfoStruct(font))
/*
FONTASCENT: height in pixels of ascending portion of text rectangle
*/
#define FONTASCENT(font) (GetFontAscent(font))
/*
width and height factors for sizing fonts
*/
#define FONTWIDTHFACTOR		57
#define FONTHEIGHTFACTOR	18
#define LABEL_GAP 2           /* pixels between plot name labels  */
#define TICK_LENGTH(q) (FONTASCENT(GetDefaultRangeLabelFont(q))/2)/*tick lngth*/
#define LABELMARGIN(q) (TICK_LENGTH(q)+3)/* gap between labels and plot border*/
#define PLOTMARGIN(q)  (TICK_LENGTH(q)+1)/* gap between ticks and plot area */
#define XAXISRNGLBL 1      /* make room for independent var range label */
#define ADD_TRACE_LABEL 1     /* make room for "add trace" label */
#define LABEL_LEFT_X 15   /*left margin x-coord, start of rng/trace labels*/
/*
LAYOUT: dimensions used for filling and clearing y-axis range labels
*/
#define TRACELABELHEIGHT(q)  (FONTHEIGHT(GetTraceLabelFont(q))+LABEL_GAP)
#define RANGELABELHEIGHT(q)  (FONTHEIGHT(GetDefaultRangeLabelFont(q))+LABEL_GAP)
#define TIMESTAMPHEIGHT(q)   (FONTHEIGHT(GetTimeStampFont(q))+LABEL_GAP)
/*
hilite/clear data stamps
*/
#define DATASTAMPY(q)  (label_y[q][i]-FONTASCENT(GetTraceLabelFont(q)))
#define DATASTAMPWIDTH(q) (DATA_LABELLEN*FONTWIDTH(GetTraceLabelFont(q)))
#define DATASTAMPHEIGHT(q) (FONTHEIGHT(GetTraceLabelFont(q)))
#define INDEPVARDATAX	(NAMLEN+1+DATA_LABELLEN+4+1)
/*
range label highlighting
*/
#define HILITE_MARGIN 1
/*
we are deducting RANGELABELHEIGHT(q) from XAXIS_HILITE_Y so we can use a
positive number here; we tried using y-coord alone and adding a negative number
here, and it didn't work on *some* displays...so we'll go the inelegant route
instead. The problem must lie in call XFillRectangle, which expects a positive
number for height of rectangle being filled...
*/
#define XAXIS_HILITE_WIDTH(q)  (window_width[q])
#define XAXIS_HILITE_HEIGHT(q,trace) ((plot_type[q]==TIME_SERIES? \
    FONTHEIGHT(GetTimeStampFont(q)):(FONTHEIGHT(GetDefaultRangeLabelFont(q))+\
    (get_isolate_traces(q)&&trace!=INDEPVAR?GetHeightStackedLabel(q): \
    LABEL_GAP))))
/*
x-position for hiliting/clearing x-axis range label
*/
#define XAXIS_HILITE_X (0)
/*
y-position for hiliting/clearing x-axis range label: see function named 
XAXIS_HILITE_Y()
*/
#define YAXIS_HILITE_X(q) (get_isolate_traces(q)? \
  rleft[q]-LABELMARGIN(q)-\
  GetMaxLenYaxisLabel(q)*FONTWIDTH(GetDefaultRangeLabelFont(q))-1: \
  label_x[q][trace==INDEPVAR?0:trace]-1)
#define YAXIS_HILITE_Y(q) (rtop[q]-\
                                FONTASCENT(GetDefaultRangeLabelFont(q))/2-1)
#define YAXIS_HILITE_HEIGHT(q) (rheight[q]+\
                     FONTASCENT(GetDefaultRangeLabelFont(q))+(HILITE_MARGIN*2))
#define YAXIS_HILITE_WIDTH(q)  (trace==INDEPVAR? \
 max_indep_len[q]*FONTWIDTH(GetDefaultRangeLabelFont(q))+(HILITE_MARGIN*2): \
 (get_isolate_traces(q)?GetMaxDepVarLen(q)* \
 FONTWIDTH(GetDefaultRangeLabelFont(q)): \
 max_dep_len[q][trace]*FONTWIDTH(GetDefaultRangeLabelFont(q))) \
 +(HILITE_MARGIN*2))


/****************************  EXPORTABLE  ********************************/

int update_mode[NUM_QUADRANTS]=
{INITIAL,INITIAL,INITIAL,INITIAL,INITIAL}; 
/* 
current y- and x- maximum, minimum, and interval between tick marks 
*/
double dep_min[NUM_QUADRANTS][MAX_TRACES],
       indep_min[NUM_QUADRANTS],
       dep_max[NUM_QUADRANTS][MAX_TRACES],
       indep_max[NUM_QUADRANTS],    
       dep_intrvl[NUM_QUADRANTS][MAX_TRACES],   
       indep_intrvl[NUM_QUADRANTS];   
/* 
define plot frame region: upper left corner, width, heigth,
bottom and right edge 
*/
int rleft[NUM_QUADRANTS],
    rtop[NUM_QUADRANTS],
    rwidth[NUM_QUADRANTS],
    rheight[NUM_QUADRANTS], 
    rbottom[NUM_QUADRANTS],             
    rright[NUM_QUADRANTS];             
/* 
number of traces in plot window, their labels, and number of current trace
*/
int num_of_traces[NUM_QUADRANTS],
    trace_number[NUM_QUADRANTS];        
char trace_label[NUM_QUADRANTS][MAX_TRACES][LABELLEN+2*DATA_LABELLEN];   
char indep_label[NUM_QUADRANTS][MAX_TRACES][LABELLEN+2*DATA_LABELLEN];   
/* 
layout of plot labels: y-coord of xaxis labels, range labels for both
axes, lengths of each range label, and maximum length in each column of
range labels. 
*/
static int xaxis_range_label_y[NUM_QUADRANTS][MAX_TRACES];  
int dep_lbl_len[NUM_QUADRANTS][MAX_TRACES][MAX_X_TIX], 
    indep_lbl_len[NUM_QUADRANTS][MAX_Y_TIX],    
    max_dep_len[NUM_QUADRANTS][MAX_TRACES], 
    max_indep_len[NUM_QUADRANTS];      
char dep_rnglbl[NUM_QUADRANTS][MAX_TRACES][MAX_X_TIX][VALUELEN],
     indep_rnglbl[NUM_QUADRANTS][MAX_Y_TIX][VALUELEN]; 
/* 
whether plot scale is fixed or floating 
*/
int dep_rng_typ[NUM_QUADRANTS][MAX_TRACES],
    indep_rng_typ[NUM_QUADRANTS];
/* 
tolerance that designates need for auto_scale operation  
*/
double clip_tolerance[NUM_QUADRANTS][MAX_TRACES],
       indep_clip_tolerance[NUM_QUADRANTS]; 
static int VarIsIndep[NUM_QUADRANTS]=
/* 
ONLY to be used directly AFTER calling event_in_chg_trace_area(); if TRUE, 
cursor is over left half of change trace area in a non-time-series plot 
*/
{FALSE,FALSE,FALSE,FALSE,FALSE};           
/* 
number of divisions on y-axis, skew-t plot 
*/
int skew_y_tix[NUM_DISPLAY_QUADRANTS]; 
/* 
number of divisions on x-axis, skew-t plot 
*/
int skew_x_tix[NUM_DISPLAY_QUADRANTS]; 
/* 
logarithms of ymin, ymax for skew-t plot 
*/
double skew_ymin[NUM_DISPLAY_QUADRANTS],
       skew_ymax[NUM_DISPLAY_QUADRANTS];  
/* 
color for time stamps  
*/
int time_color[NUM_QUADRANTS];   

/*********************  GLOBAL WITHIN FILE   *******************************/
/*  
x position for time labels 
*/
static int time_tick_x[NUM_QUADRANTS][TLABELS];    
/* 
# seconds between time labels 
*/
static float tlabel_intrvl[NUM_QUADRANTS]; 
/* 
position of data stamps for each dependent variable name label 
*/
static int depvar_data_x[NUM_QUADRANTS][MAX_TRACES];  
/* 
position of data stamps for each independent variable name label 
*/
static int indepvar_data_x[NUM_QUADRANTS][MAX_TRACES];
static int save_indepvar_data_x[NUM_QUADRANTS][MAX_TRACES];
/* 
real coordinate dimensions for skew-t plots 
*/
static double skew_width[NUM_DISPLAY_QUADRANTS],
              skew_height[NUM_DISPLAY_QUADRANTS]; 
/* 
skew-t: y coordinates for isobars, x coordinate of last temp label, pixels 
between isotherm labels 
*/
static int isobar_y[NUM_DISPLAY_QUADRANTS][MAX_Y_TIX];  
static int rightmost_isotherm_x[NUM_DISPLAY_QUADRANTS]; 
static int isotherm_intrvl[NUM_DISPLAY_QUADRANTS];    

static int XaxisTickInterval[NUM_DISPLAY_QUADRANTS],
           YaxisTickInterval[NUM_DISPLAY_QUADRANTS];

/***************************************************************************/

GetVarIsIndep(quadrant)
int quadrant;
{ 
 return VarIsIndep[quadrant];
}

/***************************************************************************/

GetTimeStampFontHeight(quadrant)
int quadrant;
/*
return desired height of time stamp font as function of overall window height
*/
{
 return GetWINDSWindowHeight(quadrant)/FONTHEIGHTFACTOR;
}

/***************************************************************************/

GetTimeStampFontWidth(quadrant)
int quadrant;
/*
return desired width of time stamp font as function of overall window width
*/
{
 return GetWINDSWindowWidth(quadrant)/FONTWIDTHFACTOR;
}

/***************************************************************************/

GetTimeFrameFontHeight(quadrant)
int quadrant;
/*
return desired height of time frame font as function of overall window height
*/
{
 return GetWINDSWindowHeight(quadrant)/FONTHEIGHTFACTOR;
}

/***************************************************************************/

GetTimeFrameFontWidth(quadrant)
int quadrant; 
/*
return desired width of time frame font as function of overall window width
*/
{
 return GetWINDSWindowWidth(quadrant)/FONTWIDTHFACTOR; 
}

/***************************************************************************/

GetRangeLabelFontHeight(quadrant)
int quadrant;
/*
return desired height of range label font as function of overall window height
*/
{
 return GetWINDSWindowHeight(quadrant)/FONTHEIGHTFACTOR;
}

/***************************************************************************/

GetRangeLabelFontWidth(quadrant)
int quadrant; 
/*
return desired width of range label font as function of overall window width
*/
{
 return GetWINDSWindowWidth(quadrant)/FONTWIDTHFACTOR;
}

/***************************************************************************/

GetTraceLabelFontHeight(quadrant)
int quadrant;
/*
return desired height of trace label font as function of overall window height
*/
{
 return GetWINDSWindowHeight(quadrant)/FONTHEIGHTFACTOR;
}

/***************************************************************************/

GetTraceLabelFontWidth(quadrant)
int quadrant; 
/*
return desired width of trace label font as function of overall window width
*/
{
 return GetWINDSWindowWidth(quadrant)/FONTWIDTHFACTOR; 
}

/*****************************************************************************/
 
GetXYAreaTooSmall(quadrant)
int quadrant;
/*
return TRUE if overall WINDS tool size is too small for the XY display type
plots
*/
{
 return FALSE;
}

/**************** SETUP_PLOTS()  ************************************/

setup_plots(quadrant) 
int quadrant;

/* set up a plot for each preselected variable */

{ 
int choice;  /* ordinal number of current plot being set up */ 
int total;

 for (choice=0; choice < MAX_TRACES; choice++) {
  dep_ptr[quadrant][choice]=dep_attr_num[quadrant][choice]=color[quadrant][choice]=ERROR;
 }
 indep_ptr[quadrant]=ERROR;

 update_mode[quadrant]=INITIAL;

 if (quadrant==PLOTS || plot_type[quadrant] != TIME_SERIES ) {
   trace_number[quadrant]=INDEPVAR;
   if (indices(quadrant,indepvar_name[quadrant])==ERROR) {
    set_notice_needed(VARIABLE_NOT_AVAILABLE);
    report(quadrant,"independent variable not available");
    return;
   }
 } else {
  time_color[quadrant]=FOREGROUND;
  indep_rng_typ[quadrant]=FLOATING;   
  set_time_range(quadrant,xaxis_seconds[quadrant]);
 }

 for(trace_number[quadrant]=-1,choice=0,total=num_of_traces[quadrant];
  choice < total;
  choice++) {

  trace_number[quadrant]++;

 /* convert user choices into traces */

  if ( indices(quadrant,usrvar[quadrant][choice]) == ERROR) { 

/* error condition  */
   set_notice_needed(VARIABLE_NOT_AVAILABLE);
   report(quadrant,"variable not available");
   --num_of_traces[quadrant];
   --trace_number[quadrant];
  } 
 } 

 if (!(num_of_traces[quadrant])) {  
/* requested setup had no good traces
*/
  if (!GetNewSetupNeeded(quadrant)) {
   report(quadrant,"bogus request; NO TRACES !!");
   set_notice_needed(NO_TRACES);
   return;
  }
 }
/* 
check for tasks that can only be done after various others
*/
 if (quadrant!=PLOTS && get_need_same_scale(quadrant)) {
  set_same_scale(quadrant,TRUE);
  set_need_same_scale(quadrant,FALSE);
  set_ScaleType(quadrant);
  set_scale_type_button(quadrant);
 }

 return;
}

/****************************************************************************/

ClearWindow(quadrant)
int quadrant;
/*
LAYOUT: clear entire quadrant window
*/
{
  (*FillArea[GetDrawType()])(pw_xid[quadrant],
   0,0,window_width[quadrant],window_height[quadrant],0);
}

/************************  DRAW_BKGND()  ************************************/

draw_bkgnd(quadrant)
int quadrant;

/*  set plot window's scale, erase its background, draw borders and label it 
    as per plot_attr file 

    draw_bkgnd is called once on initial setup of window, and subsequently
    whenever user adds/changes/deletes a trace in the window or changes the
    scale of one of the traces

    draw_bkgnd is also called by chg_scale with update_mode == CHG_Y_SCALE to 
    indicate that only a change of scale is needed, with ranges as indicated 
    in global dep_min, dep_max, indep_min, indep_max (set by chg_scale)
*/

{

 if (!(num_of_traces[quadrant]))  { 
/* 
might happen on bogus alternate request 
*/
  ClearWindow(quadrant);
  return;
 }

/* next calls MUST be done in following order: */

 ClearWindow(quadrant);
 DrawTraceLabels(quadrant);
 LayoutNDrawRangeLabels(quadrant);
 DrawPlotBorders(quadrant);
 if (plot_type[quadrant]==TRACK_PLOT) {
  draw_vector_scale(quadrant);
 }
 setscales(quadrant);
}

/**********************  SET_TIME_RANGE()  ************************************/

set_time_range(quadrant,xrange_seconds)
int quadrant,xrange_seconds;
{
  indep_min[quadrant] = 0.0; 
/* minimum interval between labels of 1 second */
  xaxis_seconds[quadrant]=xrange_seconds<(TLABELS-1)?(TLABELS-1):xrange_seconds;
  xaxis_seconds[quadrant]=xrange_seconds>(TLABELS-1)*time_mults[NM_TIME_MULTS-1]?
                (TLABELS-1)*time_mults[NM_TIME_MULTS-1]:xrange_seconds;
  tlabel_intrvl[quadrant]=(float)(xaxis_seconds[quadrant])/(float)(TLABELS-1); 
  indep_max[quadrant] = (double)xaxis_seconds[quadrant];

/* notify user if time range exceeds maximum # seconds stored in memory */

/***  move this to warn users BEFORE an operation is executed that will result
      in loss of displayed data
  if (xaxis_seconds[quadrant] > get_time_frame_size())
   set_notice_needed(TIME_LONGER_THAN_MAXSEX);
***/
}

/*****************************  GET_OFFSET()  ***************************/

get_offset(quadrant)
{
/* add or subtract offset so border is drawn outside of region, so that 
   shift_plots_left() won't copy the tick marks on each shift and there
   will be margin on other x-y plots, other than skew-t
*/

   if (plot_type[quadrant]==SKEW_T) 
    return(1);
   else
    return(PLOTMARGIN(quadrant));
}

/***************************  DRAW_TICK_MARKS()  ******************************/

draw_tick_marks(quadrant,offset)
int quadrant,offset;
/* draw tick marks of length TICK_LENGTH.  Assumes context is set before
   this is called.
*/
{
static int gap,x,y;

  if (plot_type[quadrant] == SKEW_T) {
   return; /* don't draw skew background until AFTER scales are set */
  } else {

/* draw tick marks on vertical axes */

   gap=rheight[quadrant]/(NUM_TIX-1)/
/* 
divide again if traces are isolated
*/
    (!get_indepvar_yaxis(quadrant) && get_isolate_traces(quadrant)?
      num_of_traces[quadrant]:1);
/*
protect against a user-resized window that leaves no room for a gap
*/
   if (!gap)
    gap=1;

/* leftmost y axis */

   x=rleft[quadrant]-(offset);
   for (y=rtop[quadrant]; y<=rbottom[quadrant]; y+=gap)
    (*DrawLine[GetDrawType()])(x,y,x+TICK_LENGTH(quadrant)-1,y);

/* rightmost y axis */

   x=rright[quadrant]+1;
   for (y=rtop[quadrant]; y<=rbottom[quadrant]; y+=gap)
    (*DrawLine[GetDrawType()])(x,y,x+TICK_LENGTH(quadrant)-1,y);

/* draw tick marks on horizontal axes */

   if (plot_type[quadrant]!=TIME_SERIES)
    gap= rwidth[quadrant]/(NUM_TIX-1)/
/* 
divide again if traces are isolated
*/
    (get_indepvar_yaxis(quadrant) && get_isolate_traces(quadrant)?
     num_of_traces[quadrant]:1);
   else
    gap=rwidth[quadrant]/(TLABELS-1);
/*
protect against a user-resized window that leaves no room for a gap
*/
   if (!gap)
    gap=1;

/* top x axis */

   y=rtop[quadrant]-(offset);
   for (x=rleft[quadrant]; x<=rright[quadrant]; x+=gap)
    (*DrawLine[GetDrawType()])(x,y,x,y+TICK_LENGTH(quadrant)-1);

/* bottom x axis */

   y=rbottom[quadrant]+1;
   for (x=rleft[quadrant]; x<=rright[quadrant]; x+=gap)
    (*DrawLine[GetDrawType()])(x,y,x,y+TICK_LENGTH(quadrant)-1);
  }

}

/*****************************  DrawPlotBorders()  ***************************/

DrawPlotBorders(quadrant)
int quadrant;

/* draw plot borders  

   on entry: rleft, rtop, rbottom, and rwidth must be defined
   on exit: borders are drawn around the region (all of the borders are OUTSIDE
    the region)
*/
{
int offset;
  
   if (!repaint_set[quadrant])
    return;

   offset=get_offset(quadrant);
   (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,FOREGROUND,0,
    LineSolid,0);
/*
left y-axis
*/
   (*DrawLine[GetDrawType()])(rleft[quadrant]-offset,rtop[quadrant],
    rleft[quadrant]-offset,rbottom[quadrant]);
/*
bottom x-axis
*/
   (*DrawLine[GetDrawType()])(rleft[quadrant],rbottom[quadrant]+offset,
    rright[quadrant],rbottom[quadrant]+offset);
/*
right y-axis
*/
   (*DrawLine[GetDrawType()])(rright[quadrant]+offset,rbottom[quadrant],
    rright[quadrant]+offset,rtop[quadrant]);
/*
top x-axis
*/
   (*DrawLine[GetDrawType()])(rright[quadrant],rtop[quadrant]-offset,
    rleft[quadrant],rtop[quadrant]-offset);

   draw_tick_marks(quadrant,offset);
}

/***************************  REMOVE_BORDERS()  ******************************/

remove_borders(quadrant)
{
int offset;

   offset=get_offset(quadrant);
   (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,0,0,0,0);
   (*DrawLine[GetDrawType()])(rright[quadrant]+offset,
    rbottom[quadrant]+offset,rright[quadrant]+offset,rtop[quadrant]-offset);
   (*DrawLine[GetDrawType()])(rright[quadrant]+offset,
    rtop[quadrant]-offset,rleft[quadrant]-offset,rtop[quadrant]-offset);
   draw_tick_marks(quadrant,offset);
   (*FlushDisplay[GetDrawType()])(win_display);
}

/***************************  RESTORE_BORDERS()  ******************************/

restore_borders(quadrant)
{
 DrawPlotBorders(quadrant);
}

/*****************  SETSCALES()  *****************************************/

setscales(quadrant)
int quadrant;
/*  
    set scale factors as function of current size of window and 
    current range selections 

    on entry: rwidth, rheight, dep_min, dep_max, indep_min, and indep_max must 
      be defined. 
    on exit: indep_scale and dep_scale are defined
*/

{
int k;

 switch (plot_type[quadrant]) {
  case SKEW_T:  /* done in get_skewt_info() */
   break;
  default:
   if (get_indepvar_yaxis(quadrant)) {
/*
independent variable is on y-axis
*/
    indep_scale[quadrant] = (double)(rheight[quadrant]-1)/
     (indep_max[quadrant] - indep_min[quadrant]);
    for (k=0; k<num_of_traces[quadrant]; k++) {
     dep_scale[quadrant][k] = ((double)(rwidth[quadrant]-1)/
      (dep_max[quadrant][k] - dep_min[quadrant][k]))/
     (get_isolate_traces(quadrant)?num_of_traces[quadrant]:1);
    }
   } else {
/*
independent variable is on x-axis
*/
    indep_scale[quadrant] = (double)(rwidth[quadrant]-1)/
     (indep_max[quadrant] - indep_min[quadrant]);
    for (k=0; k<num_of_traces[quadrant]; k++) {
     dep_scale[quadrant][k] = ((double)(rheight[quadrant]-1)/
      (dep_max[quadrant][k] - dep_min[quadrant][k]))/
     (get_isolate_traces(quadrant)?num_of_traces[quadrant]:1);
    }
   }
   break;
 }
}

/***************************  ADJUST_POINTS()  *********************************/

adjust_points(quadrant,trace,x1,y_1,x2,y2)
int quadrant,trace;
int *x1,*y_1,*x2,*y2;
/*
assuming this quadrant is in isolate-traces mode, adjust the coords of the 
trace'th trace according to formula. Note that old_coord has already
been scaled according to # of traces in quadrant:

new_coord = offset + fractional offset into plot area * width|height of plot area
==>
new_coord = offset 
            + old_coord/(width or height of plot area)
            *(total width or height of all plot areas)/number_of_traces
==>
new_coord = trace number * (total width or height of all areas)/number of traces 
 	    + old_coord/(total width or height of all areas)/number_of_traces 
            * (total width or height of all areas)/number of traces
==>
...factor (total width or height of all areas)/number_of_traces 
cancels in last multiply to derive below coding.  This effectively positions each 
trace at approximately equal intervals within the plot frame.
*/
{
/***
printf("isolate: trace %d original (%d,%d) ",trace,*x1,*y_1,*x2,*y2);
***/
  if (get_indepvar_yaxis(quadrant)) {
   *x1=(trace*rwidth[quadrant]/num_of_traces[quadrant]+*x1);
   *x2=(trace*rwidth[quadrant]/num_of_traces[quadrant]+*x2);
  } else {
   *y_1=(trace*rheight[quadrant]/num_of_traces[quadrant]+*y_1);
   *y2 =(trace*rheight[quadrant]/num_of_traces[quadrant]+*y2);
  }
/***
printf(" adjusted (%d,%d)\n",*x1,*y_1,*x2,*y2);
***/
}

/***************************************************************************/

SetPlotBorderHeight(quadrant)
int quadrant;
/*
Set parameters controlling plot border height.

On entry, following must be set:

 num_of_traces
 window_height
 font sizes

on exit, following are set:

 rtop
 rbottom
 rheight

*/
{
 int  TraceNRngLblArea,num_traces,
 num_tix,trace_label_rows,range_label_rows,time_stamp_row;

 num_traces=num_of_traces[quadrant];
 time_stamp_row=range_label_rows=0;
 if (get_indepvar_yaxis(quadrant)) {
/* 
provide an add_trace_area only if plot type is not skew-t 
*/
  trace_label_rows=plot_type[quadrant]==SKEW_T?num_traces:
   MIN(MAX_TRACES,num_traces+ADD_TRACE_LABEL);
  if (get_isolate_traces(quadrant) || get_same_scale(quadrant)) {
/*
independent var on y-axis, and x-axis range label takes up only one row
*/
   range_label_rows=XAXISRNGLBL;
  } else {
/*
independent var on y-axis, and x-axis range labels take up as many rows
as number of traces (except for skew-t, one only)
*/
   range_label_rows=(plot_type[quadrant]==SKEW_T?1:num_traces);
  }
 } else {
/*
independent var on x-axis
*/
  if (plot_type[quadrant]==TIME_SERIES)
   time_stamp_row=XAXISRNGLBL;
  else
   range_label_rows=XAXISRNGLBL;
  trace_label_rows=MIN(MAX_TRACES,num_traces+ADD_TRACE_LABEL);
 }
 TraceNRngLblArea=trace_label_rows*TRACELABELHEIGHT(quadrant) +
  (plot_type[quadrant]==TIME_SERIES?time_stamp_row*TIMESTAMPHEIGHT(quadrant):0)+
  range_label_rows*RANGELABELHEIGHT(quadrant) +
  (get_indepvar_yaxis(quadrant)?GetHeightStackedLabel(quadrant):0);
/*
calculate y-position of bottom of plot region (rbottom)
*/
 rbottom[quadrant] = window_height[quadrant]-TraceNRngLblArea-
  LABELMARGIN(quadrant);
/*
minimum value for rtop, make room for xy-plot time frame stamp
*/
 rtop[quadrant]=FONTHEIGHT(GetTimeFrameFont(quadrant))+LABELMARGIN(quadrant);
/*
ensure nice tick marks, etc., by adjusting height components so it is evenly 
divisible by maximum possible # of ticks.
...rbottom and rtop are the bottom and top end pixels of the plot frame and
ARE included in the plotting area
*/
 num_tix=(NUM_TIX-1)*(get_indepvar_yaxis(quadrant)?1:
  (get_isolate_traces(quadrant)?num_of_traces[quadrant]:1));
/*
the adjustment here to ensure evenly spaced tick marks will sometimes result
in a seemingly overdone gap between the time frame string and the top plot
frame border. We could go about re-sizing the font here, I suppose.
*/
 while ((rbottom[quadrant]-rtop[quadrant])%num_tix)
  rtop[quadrant]++;
 rheight[quadrant] = rbottom[quadrant] - rtop[quadrant] + 1;
}

/*****************************  DrawTraceLabels()  *************************/

DrawTraceLabels(quadrant)
int quadrant;
/* 
label plot with function name, units, and dependent axis scale values

on entry: dep_attr_num, window_height and window_width must be defined; 
on exit: rbottom, rheight, and rtop are defined and all name labels are done
*/
{
int k, RngLblArea, num_traces;

 num_traces=num_of_traces[quadrant];
 SetPlotBorderHeight(quadrant);
 if (!repaint_set[quadrant])
  return;
/* 
draw the name labels 
*/
 for (k=0; k<num_traces; k++){
  (void)sprintf(trace_label[quadrant][k],"%s (",trace_name[quadrant][k]);
/* 
x-position for data_stamps() updating 
*/
  depvar_data_x[quadrant][k]=LABEL_LEFT_X+FONTWIDTH(GetTraceLabelFont(quadrant))*
   strlen(trace_label[quadrant][k]);
/* 
add units and independent variable, if not time 
*/
  switch (plot_type[quadrant]) {
  case TIME_SERIES:
   (void)sprintf(&trace_label[quadrant][k][strlen(trace_label[quadrant][k])],
    "       %s)",&units[dep_attr_num[quadrant][k]*UNITSLEN]);
   break;
  case CALIBRATION:
    (void)sprintf(&trace_label[quadrant][k][strlen(trace_label[quadrant][k])],
     "       %s) ",
     &units[dep_attr_num[quadrant][k]*UNITSLEN]);
    (void)sprintf(indep_label[quadrant][k],
     "v. Set Pt. (",(char *)GetPlotAttrName(indep_attr_num[quadrant]));
    save_indepvar_data_x[quadrant][k]=
    indepvar_data_x[quadrant][k]=LABEL_LEFT_X+
     FONTWIDTH(GetTraceLabelFont(quadrant))*
     (INDEPVARDATAX+strlen(indep_label[quadrant][k]));
    (void)sprintf(&indep_label[quadrant][k][strlen(indep_label[quadrant][k])],
     "       %s)",&units[indep_attr_num[quadrant]*UNITSLEN]);
   break;
  default:
    (void)sprintf(&trace_label[quadrant][k][strlen(trace_label[quadrant][k])],
     "       %s) ",
     &units[dep_attr_num[quadrant][k]*UNITSLEN]);
    (void)sprintf(indep_label[quadrant][k],
     "v. %s (",(char *)GetPlotAttrName(indep_attr_num[quadrant]));
    save_indepvar_data_x[quadrant][k]=
    indepvar_data_x[quadrant][k]=LABEL_LEFT_X+
     FONTWIDTH(GetTraceLabelFont(quadrant))*
     (INDEPVARDATAX+strlen(indep_label[quadrant][k]));
    (void)sprintf(&indep_label[quadrant][k][strlen(indep_label[quadrant][k])],
     "       %s)",&units[indep_attr_num[quadrant]*UNITSLEN]);
  } 
  if (get_indepvar_yaxis(quadrant)) {
/*
provide an add_trace_area only if plot type is not skew-t
*/
   if (get_isolate_traces(quadrant) || get_same_scale(quadrant)) {
/*
independent var on y-axis, and x-axis range label takes up only one row
*/
    RngLblArea=RANGELABELHEIGHT(quadrant) + GetHeightStackedLabel(quadrant);
   } else {
/*
independent var on y-axis, and x-axis range labels take up as many rows
as number of traces (except for skew-t, one only)
*/
    RngLblArea=(plot_type[quadrant]==SKEW_T?1:num_traces)
     *RANGELABELHEIGHT(quadrant);
   }
  } else {
/*
independent var on x-axis
*/
   if (plot_type[quadrant]==TIME_SERIES)
    RngLblArea=TIMESTAMPHEIGHT(quadrant);
   else
    RngLblArea=RANGELABELHEIGHT(quadrant);
  }
/*
LAYOUT (Trace Labels): y-position of trace labels
*/
  label_y[quadrant][k]=rbottom[quadrant]+LABELMARGIN(quadrant)+RngLblArea+
   (k+1)*TRACELABELHEIGHT(quadrant)-LABEL_GAP-
   FONTDESCENT(GetTraceLabelFont(quadrant));
/*
draw 'em
*/
  (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,color[quadrant][k],0,0,
   FONTID(GetTraceLabelFont(quadrant)));
/*
LAYOUT: write out (unhilited) dependent var trace label
*/
  (*DrawText[GetDrawType()])(
   LABEL_LEFT_X,
   label_y[quadrant][k],
   trace_label[quadrant][k]);
  if (plot_type[quadrant]!=TIME_SERIES) {
   (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,FOREGROUND,0,0,
    FONTID(GetTraceLabelFont(quadrant)));
/*
LAYOUT: write out (unhilited) independent var trace label
*/
   (*DrawText[GetDrawType()])(
    LABEL_LEFT_X+
     FONTWIDTH(GetTraceLabelFont(quadrant))*INDEPVARDATAX,
    label_y[quadrant][k],
    indep_label[quadrant][k]);
  }
 }
 data_stamps(quadrant,
  (start_point[quadrant]+time_gone_by[quadrant]-1)%get_datablock_size());
 return;
}

/*****************************************************************************/

XAXIS_HILITE_Y(q,trace) 
int q,trace;
/*
return y-coordinate for top of xaxis range label hiliting
*/
{
 if (trace)
  return
   XAXIS_HILITE_Y(q,0)+(get_isolate_traces(q) ||
                        get_same_scale(q) ||
                        trace==INDEPVAR?
                        0:trace)*
   XAXIS_HILITE_HEIGHT(q,trace);
 else
  return
   (xaxis_range_label_y[q][0]-
   (plot_type[q]==TIME_SERIES?FONTASCENT(GetTimeStampFont(q)):
   FONTASCENT(GetDefaultRangeLabelFont(q))+
   (get_indepvar_yaxis(q)?GetAscentStackedLabel(q)/2:0)));
}

/*****************************************************************************/

SetXaxisIndepVarXPosns(quadrant)
/*
set y coords of xaxis labels
*/
{
 xaxis_range_label_y[quadrant][0]=
  label_y[quadrant][0]-FONTASCENT(GetTraceLabelFont(quadrant))-
  FONTDESCENT(GetDefaultRangeLabelFont(quadrant))-LABEL_GAP;
}

/*************************************************************************/

GetXaxisIndepVarYposn(quadrant)
int quadrant;
/*
return y-coord for x-axis independent variable range label
*/
{
 return xaxis_range_label_y[quadrant][0];
}

/*****************************************************************************/

SetXaxisDepVarYPosns(quadrant)
/*
set y coords of xaxis range labels
*/
{
int which,trace,num_traces;

 num_traces=plot_type[quadrant]==SKEW_T?1:(get_same_scale(quadrant)?1:
  num_of_traces[quadrant]);
 for (trace=0; trace<num_of_traces[quadrant]; trace++) {
  which=get_same_scale(quadrant)||get_isolate_traces(quadrant)?0:
   num_traces-(trace+1);
  xaxis_range_label_y[quadrant][trace]=
/*
go to top of uppermost trace label...
*/
   label_y[quadrant][0]-FONTASCENT(GetTraceLabelFont(quadrant))-
/*
...and proceed up the gap between that trace label and the xaxis range label...
*/
   LABEL_GAP-
/*
...provide for text postioning offset into range label font...
*/
   FONTDESCENT(GetDefaultRangeLabelFont(quadrant))-
/*
...provide for extra room needed for stacked labels, if any...
*/
   GetHeightStackedLabel(quadrant)/2-
/*
...and finally adjust if needed for normal scale format...
*/
   (which*RANGELABELHEIGHT(quadrant));
 }
}

/*************************************************************************/

GetXaxisDepVarYPosn(quadrant,trace,tick,color)
int quadrant,trace,tick,color;
/*
return y-coord for dependent var range labels on x-axis
*/
{
int shared_tick_offset;

 if (get_isolate_traces(quadrant)) {
/*
all range labels are in one row; adjust y-coord offset for those labels
that point to shared tick marks
*/
  if (tick==NUM_TIX-1 && trace!=0) {
/*
trace's first label, sharing tick with trace on its left: and
lower y-position UNLESS range label is hilited
*/
   shared_tick_offset=color==REVERSE_VIDEO?0:
    GetStackedLabelOffset(quadrant,FALSE);
  } else if (tick==0 && trace!=num_of_traces[quadrant]-1) {
/*
trace's last label, sharing tick with trace on right: 
raise y-position UNLESS range label is hilited
*/
   shared_tick_offset=color==REVERSE_VIDEO?0:
    GetStackedLabelOffset(quadrant,TRUE);
  } else {
/*
non-shared tick: no offset
*/
   shared_tick_offset=0;
  }
 } else {
/*
normal separated rows: no offsets
*/
  shared_tick_offset=0;
 }
 return xaxis_range_label_y[quadrant][trace]+shared_tick_offset;
}

/*****************************************************************************/

SetYaxisIndepVarXposns(quadrant)
/*
initialize the x-coord for y-axis independent variable range labels
*/
{
 label_x[quadrant][0] = LABEL_LEFT_X;
}

/*************************************************************************/

GetYaxisIndepVarYPosn(bottom,tick,interval)
int bottom,tick,interval;
/*
return the y-coord for y-axis independent variable range labels
*/
{
/* 
y position of y-axis labels: start at bottom, deduct interval between tick marks
*/
  return bottom-(tick*interval);
}

/*************************************************************************/

GetYaxisDepVarYPosn(quadrant,trace,tick,interval,color)
int quadrant,trace,tick,interval,color;
/*
return the y-coord for y-axis dependent variable range labels
*/
{
int shared_tick_offset,top,trace_offset;
/*
LAYOUT: position topmost y-axis range label centered on top tick mark
*/
 shared_tick_offset=trace_offset=0;
 top=rtop[quadrant]+FONTASCENT(GetDefaultRangeLabelFont(quadrant))/2;
 if (get_isolate_traces(quadrant)) {
  trace_offset=trace*rheight[quadrant]/num_of_traces[quadrant];
/*
all range labels are in one column; adjust y-coord offset for the
labels that point to shared tick marks.
*/
  if (tick==0 && trace!=0) {
/*
trace's first label, sharing tick with trace above it: 
lower y-position, only if this is not the first trace and the color is
inverse video
*/
   shared_tick_offset=color==REVERSE_VIDEO?0:
    GetStackedLabelOffset(quadrant,FALSE);
  } else if (tick==NUM_TIX-1 && trace!=num_of_traces[quadrant]-1) {
/*
trace's last label, sharing tick with trace below it: raise y-position
*/
   shared_tick_offset=color==REVERSE_VIDEO?0:
    GetStackedLabelOffset(quadrant,TRUE);
  } else {
/*
non-shared tick: no offsets
*/
   shared_tick_offset=0;
  }   
 } else {
/*
normal separated rows: no offsets
*/
   shared_tick_offset=0;
 }
/* 
y position of y-axis labels: start at top -- add offset as per trace if traces 
are to be isolated -- and increase by tick interval and any offset needed for 
shared tick marks 
*/
 return top+trace_offset+tick*interval+shared_tick_offset;
}

/*************************************************************************/

InitYaxisIndepVarXPosn(quadrant)
int quadrant;
/*
initialize the x-coords for y-axis independent variable range labels
*/
{
 label_x[quadrant][0]=LABEL_LEFT_X;
}

/*************************************************************************/

InitYaxisRngLblXPosns(quadrant)
int quadrant;
/*
initialize the x-coords for all y-axis variable range labels
*/
{
 InitYaxisIndepVarXPosn(quadrant);
 if (get_indepvar_yaxis(quadrant) || get_isolate_traces(quadrant) ||
     get_same_scale(quadrant)) {
  return;
 }
 InitYaxisDepVarXPosn(quadrant);
}

/*************************************************************************/

InitYaxisDepVarXPosn(quadrant)
int quadrant;
/*
initialize the x-coords for y-axis dependent variable range labels
*/
{
int trace;

 label_x[quadrant][0]=LABEL_LEFT_X;
 for (trace=0; trace<num_of_traces[quadrant]; trace++) {
  label_x[quadrant][trace]=
   (trace? 
    label_x[quadrant][trace-1]+
/*
add one for a gap between label columns
*/
    (max_dep_len[quadrant][trace-1]+1)*
    FONTWIDTH(GetDefaultRangeLabelFont(quadrant))
   :LABEL_LEFT_X);
 }
}

/*************************************************************************/

GetYaxisDepVarXposn(quadrant,tick,trace,color)
int quadrant,tick,trace,color;
/* 
LAYOUT: x positions of y-axis labels: leftmost edge of x labels 
plus maximum label length minus length of this label in pixels to right-justify
standard y-axis labels, or left plot frame border minus length of each label to 
right-justify in case of isolate-trace labelling 
*/
{
int x,justify_offset,largewidth,smallwidth;

 if (get_isolate_traces(quadrant)) {
/*
justify_offset assumes the fonts are centered within their widths, which may
not always be true, and halves the difference between larger and smaller fonts
to determine how much to deduct from smaller fonts to keep them right-justified
with larger ones; needed on y-axis labels only.
*/
  largewidth=FONTWIDTH(GetDefaultRangeLabelFont(quadrant));
  smallwidth=FONTWIDTH(GetSmallRangeLabelFont(quadrant));
  if (tick==0 && trace!=0) {
/*
trace's first label, sharing tick with trace above it: 
lower y-position, only if this is not the first trace and the color is
inverse video
*/
   justify_offset=color==REVERSE_VIDEO?0:(largewidth-smallwidth)/2;
  } else if (tick==NUM_TIX-1 && trace!=num_of_traces[quadrant]-1) {
/*
trace's last label, sharing tick with trace below it
*/
   justify_offset=color==REVERSE_VIDEO?0:(largewidth-smallwidth)/2;
  } else {
/*
non-shared tick
*/
   justify_offset=0;
  }   
  x=rleft[quadrant]-LABELMARGIN(quadrant)-
   XTextWidth((XFontStruct *)FONTINFO(GetRangeLabelFont(quadrant)),
   dep_rnglbl[quadrant][trace][tick],
   dep_lbl_len[quadrant][trace][tick])-justify_offset;
 } else if (get_same_scale(quadrant)) {
/*
same scale
*/
  x=label_x[quadrant][trace]+(max_dep_len[quadrant][trace]-
   dep_lbl_len[quadrant][trace][tick])*
   FONTWIDTH(GetDefaultRangeLabelFont(quadrant));

 } else {
/*
normal separated rows
*/
  x=label_x[quadrant][trace]+(max_dep_len[quadrant][trace]-
   dep_lbl_len[quadrant][trace][tick])*
   FONTWIDTH(GetDefaultRangeLabelFont(quadrant));
 }
 return x;
}

/****************************************************************************/

SetPlotBorderWidth(quadrant)
int quadrant;
/*
initializes rleft, then adjusts as per number of y-axis traces and the widths
of each. Also sets right edge of plot border and then calculates plot
area width.
*/
{
int num_tix,num_traces;

 num_traces=plot_type[quadrant]==SKEW_T?1:(get_same_scale(quadrant)?1:
  num_of_traces[quadrant]);
/* 
initialize rleft: allow for first time stamp's 1st 4 chars 
*/
 rleft[quadrant]=LABEL_LEFT_X+LABELMARGIN(quadrant);
/* 
rleft and rright are the left and right end pixels of the plot frame and
ARE included in the plotting area
*/
 if (get_indepvar_yaxis(quadrant)) {
  rleft[quadrant]=rleft[quadrant] + (max_indep_len[quadrant])*FONTWIDTH(
   GetDefaultRangeLabelFont(quadrant));
/*
ensure nice tick marks, etc., by adjusting width components so it is evenly 
divisible by maximum possible # of ticks.
*/
  num_tix=plot_type[quadrant]==TIME_SERIES?TLABELS-1:
   (NUM_TIX-1)*(get_isolate_traces(quadrant)?num_of_traces[quadrant]:1);
 } else {
  if (get_isolate_traces(quadrant)) {
/*
all ranges are to be drawn in one column; use the longest string 
before drawing any of them, to facilitate right justifying the labels
*/
   rleft[quadrant]=rleft[quadrant]+
    GetMaxDepVarLen(quadrant)*FONTWIDTH(GetDefaultRangeLabelFont(quadrant));
  } else {
   rleft[quadrant]=label_x[quadrant][num_traces-1]+LABELMARGIN(quadrant)+
    (max_dep_len[quadrant][num_traces-1])*
    FONTWIDTH(GetDefaultRangeLabelFont(quadrant));
  }
/*
ensure nice tick marks, etc., by adjusting width components so it is evenly
divisible by maximum possible # of ticks.
*/
  num_tix=plot_type[quadrant]==TIME_SERIES?TLABELS-1:NUM_TIX-1;
 }
/* 
calculate ballpark starting point for right edge of plot frame 
*/ 
 rright[quadrant]=window_width[quadrant]-GetRangeLabelRightMargin(quadrant);
/*
*decrease* the right edge of plot frame so that we'll have no less than
the desired right margin.
*/
 while ((rright[quadrant]-rleft[quadrant])%num_tix)
  rright[quadrant]--;
 rwidth[quadrant] = rright[quadrant]-rleft[quadrant]+1;
}

/*****************************************************************************/

SetTickIntervals(quadrant)
int quadrant;
/*
set intervals between tick marks as function of window width and height
*/
{
 if (get_indepvar_yaxis(quadrant)) {
  YaxisTickInterval[quadrant]=rheight[quadrant]/(NUM_TIX-1);
  XaxisTickInterval[quadrant]=rwidth[quadrant]/(NUM_TIX-1)/
/*
divide again if traces, and ergo range labels, are to be isolated
*/
    (get_isolate_traces(quadrant)?num_of_traces[quadrant]:1);
 } else {
  XaxisTickInterval[quadrant]=rwidth[quadrant]/(NUM_TIX-1);
  YaxisTickInterval[quadrant]=rheight[quadrant]/(NUM_TIX-1)/
    (get_isolate_traces(quadrant)?num_of_traces[quadrant]:1);
 }
}

/*****************************************************************************/

SetPlotBorders(quadrant)
int quadrant;
/*
set parameters controlling the plot border width and height
*/
{
 SetPlotBorderWidth(quadrant);
 SetPlotBorderHeight(quadrant);
}

/*****************************************************************************/

SetFontsAndPlotBorders(quadrant,recurse) 
int quadrant,recurse;
/*
adjust font sizes as needed in case of added/deleted variables in given 
window; adjust also the plot borders
*/
{
 SetTickIntervals(quadrant);
 CheckRngLblFontSizes(quadrant,GetMaxLenXaxisLabel(quadrant),
  XaxisTickInterval[quadrant],YaxisTickInterval[quadrant]-1);
 InitYaxisRngLblXPosns(quadrant);
 SetPlotBorders(quadrant);
 SetTickIntervals(quadrant);
/*
yeah I know we already did this, but we unfortunately have a circular
dependency here: font heights depend on rheight, rheight depends on
font heights. If we don't set it again, labels may be drawn off-center
on tick marks; if we do, the font heights chosen may need to be adjusted
again, but I'm not going to do that. You can only imagine why, I'm sure.
And I apologize for the circular dependency. So sue me.
*/
 if (recurse)
  SetFontsAndPlotBorders(quadrant,FALSE);
 else
  return;
/*
now adjust as needed the x-y positions of labels on each axis. Y-axis dep-var
x-position was already init'd above in InitYaxisRngLblXPosns(), and is
determined exactly at draw time in GetYaxisDepVarXposn()
*/
 SetXaxisIndepVarXPosns(quadrant);
 SetXaxisDepVarYPosns(quadrant);
 SetYaxisIndepVarXposns(quadrant);
}

/*****************************************************************************/

GetRangeLabels(quadrant,num_traces)
int quadrant,num_traces;
/*
initialize strings holding range labels
*/
{
int trace;

 get_rnglbls(quadrant,INDEPVAR);
 for (trace=0; trace < num_traces; trace++) {
  get_rnglbls(quadrant,trace);
 }
}

/********************  LayoutNDrawRangeLabels()  *****************************/

LayoutNDrawRangeLabels(quadrant)
int quadrant;
/*
  on entry: dep_min and intrvl must be defined
  on exit: range labels are drawn, and rleft and rwidth are defined
*/
{
int trace,num_traces;
/* 
In the skew-t plots case, the temperature and dewpoint
(dependent vars) have the same scale so only one set of labels is drawn
*/
 num_traces=plot_type[quadrant]==SKEW_T?1:(get_same_scale(quadrant)?1:
  num_of_traces[quadrant]);
/* 
initialize rleft: allow for first time stamp's 1st 4 chars 
*/
 rleft[quadrant]=LABEL_LEFT_X; 
/*
determine fonts will fit OK, adjust plot borders accordingly
*/
 if (plot_type[quadrant]==SKEW_T)
  get_skewt_info(quadrant);
 GetRangeLabels(quadrant,num_traces);
 if (plot_type[quadrant]==SKEW_T) {
/*
must reset plot border as per maximum lengths determined in GetRangeLabels()
*/
  SetPlotBorders(quadrant);
 } else {
/*
only need to re-check font sizes if number of traces may vary in given 
display. This can't happen in skew-t plots.
*/
  SetFontsAndPlotBorders(quadrant,TRUE);
 }
/* 
must draw y-axis labels first to determine remaining width of region
*/
 if (get_indepvar_yaxis(quadrant)) {
/*
YAXIS INDEP
*/
  SetXaxisDepVarYPosns(quadrant);
  SetYaxisIndepVarXposns(quadrant);
  if (repaint_set[quadrant]) {
   DrawRangeLabels(quadrant,FOREGROUND,INDEPVAR);
  }
/* 
get additional info if special plot type, now that all needed vars are
defined (rwidth was the last)
*/
  if (plot_type[quadrant]==SKEW_T)
   get_skewt_info(quadrant);
/* 
XAXIS DEP
*/
  if (repaint_set[quadrant]) {
   for (trace=0; trace < num_traces; trace++) {
    DrawRangeLabels(quadrant,
     get_same_scale(quadrant)?BLACK:color[quadrant][trace],trace);
   }
  }
 } else {               
/* 
YAXIS DEP: label dep't var(s) range on y-axis 
*/
  if (repaint_set) {
   for (trace=0; trace < num_traces; trace++) {
    if (get_isolate_traces(quadrant)) {
     DrawRangeLabels(quadrant,color[quadrant][trace],trace);
    } else {
     DrawRangeLabels(quadrant,
      get_same_scale(quadrant)?BLACK:color[quadrant][trace],trace);
    }
   }
  }
/* 
get additional info if special plot type, now that all needed vars are
defined (rwidth was the last)
*/
  if (plot_type[quadrant]==SKEW_T)
   get_skewt_info(quadrant);
/*
XAXIS INDEP
label indep't var range on x-axis
*/
  SetXaxisIndepVarXPosns(quadrant);
  if (plot_type [quadrant]!= TIME_SERIES) {
   if (repaint_set[quadrant]) {
    DrawRangeLabels(quadrant,FOREGROUND,INDEPVAR);
   }
  } else {
/* 
calculate x-y position for time labels -- define x position dynamically in 
case font changes  
*/
   for (trace=0; trace<TLABELS; trace++) 
    time_tick_x[quadrant][trace]=rleft[quadrant] + 
     (trace*rwidth[quadrant]/(TLABELS-1));
  }
 }
}

/**********************************************************************/

GetRangeLabelRightMargin(quadrant)
int quadrant;
/*
return the margin needed to allow complete drawing of rightmost range label
within given window
*/
{
int length;

 if (plot_type[quadrant]==TRACK_PLOT) {
  return GetVectorLabelMargin(quadrant);
 } else if (plot_type[quadrant]==TIME_SERIES) {
  length=GetFontWidth(GetTimeStampFont(quadrant))*strlen(" xx:xx:xx ");
 } else if (get_indepvar_yaxis(quadrant)) {
/*
check dependent variable range label lengths on x-axis
*/
  if (get_isolate_traces(quadrant)) {
   length=GetFontWidth(GetDefaultRangeLabelFont(quadrant))*
    strlen(dep_rnglbl[quadrant][num_of_traces[quadrant]-1][0]);
  } else if (get_same_scale(quadrant)) {
   length=GetFontWidth(GetDefaultRangeLabelFont(quadrant))*
    strlen(dep_rnglbl[quadrant][0][0]);
  } else {
   length=GetMaxDepVarLen(quadrant)*
    GetFontWidth(GetDefaultRangeLabelFont(quadrant));
  }
 } else {
/*
check independent variable range label lengths on x-axis
*/
  length=GetFontWidth(GetDefaultRangeLabelFont(quadrant))*
   strlen(indep_rnglbl[quadrant][NUM_TIX-1]);
 }
 return length/2+GetFontWidth(GetDefaultRangeLabelFont(quadrant));
}

/*******************  TIME_STAMPS()  **********************************/

time_stamps(quadrant)
int quadrant;
/* starting with leftmost time, label the time axis with TLABELS time stamps,
   or, if non time-series, label time frame stamp.  In time series, the 
   time labels line up with LEFT edge of time slice plotted, except for the 
   rightmost label, which marks the RIGHT edge of last time slice.  This
   reconciles labels with what data points are actually plotted, inclusively.  In
   the non-time-series, again indicate the inclusive start to end time frame,
   which would have start second one higher than end second (11:01 to 12:00,
   for example).  
 */

{
static char time_label[TLABELS][TIME_CHARS]; /* time stamps  */
static float first_hr[NUM_DISPLAY_QUADRANTS],
             first_minute[NUM_DISPLAY_QUADRANTS],
             first_sec[NUM_DISPLAY_QUADRANTS];
static int hr,minute,sec;
static int i;  /* values for time labels  */
static char start_time[TIME_CHARS],
       end_time[TIME_CHARS],
       time_interval[TIME_CHARS*2];   /* start/end time */

/************************  ASSUMPTION  ***************************************/
/* constants used in this function:
   24: number of hours in a day
   60: number of minutes in an hour
/************************  END ASSUMPTION  ***********************************/
/* adjust each field according to standard time notations */

 if (!(start_point[quadrant]+time_gone_by[quadrant])) 
  return;

 if (plot_type[quadrant]==TIME_SERIES) {

/* recompute start time for time series each second. Time series always start 
   at point determined by xaxis_seconds
*/ 
  first_sec[quadrant]=(float)local_sec[quadrant]-xaxis_seconds[quadrant]+1;
  first_hr[quadrant]=(float)local_hr[quadrant];
  first_minute[quadrant]=(float)local_minute[quadrant];

 } else if (!got_start_time[quadrant]) {

/* recompute start time for non-time series plots whenever they get redrawn.  
   Start at the lesser of get_time_frame_size() seconds ago or total time 
   elapsed thus far
*/
  if (frozen[quadrant]) {
   if (ElapsedTime-time_gone_by[quadrant]>get_time_frame_size()) {
     first_sec[quadrant]=(float)local_sec[quadrant];
     first_hr[quadrant]=(float)local_hr[quadrant];
     first_minute[quadrant]=(float)local_minute[quadrant];
   } else {
    if (ElapsedTime > get_time_frame_size()) {
     first_sec[quadrant]=(float)local_sec[quadrant]-
      get_update_interval()*get_time_frame_size()+get_update_interval()
/*
add increment equal to number of seconds elapsed since queue filled up
*/
      +(ElapsedTime-time_gone_by[quadrant])*get_update_interval();
     first_hr[quadrant]=(float)local_hr[quadrant];
     first_minute[quadrant]=(float)local_minute[quadrant];
    } 
   }
  } else {
   if (time_gone_by[quadrant] > get_time_frame_size()) {
    first_sec[quadrant]=(float)local_sec[quadrant]-
     get_update_interval()*get_time_frame_size()+get_update_interval();
    first_hr[quadrant]=(float)local_hr[quadrant];
    first_minute[quadrant]=(float)local_minute[quadrant];
   } else { 
    first_sec[quadrant]=(float)local_sec[quadrant]-
     get_update_interval()*time_gone_by[quadrant]+get_update_interval();
    first_minute[quadrant]=(float)local_minute[quadrant];
    first_hr[quadrant]=(float)local_hr[quadrant];
   }
  }
 }
/* 
seconds exceeds norm; adjust
*/
 while (first_sec [quadrant]>= 60.0) {
  first_minute[quadrant]+=1.0;
  first_sec[quadrant]-=60.0;
 }
 while (first_minute [quadrant]>=60.0) {
  first_hr[quadrant]+=1.0;
  first_minute[quadrant]-=60.0;
 }
 while (first_hr [quadrant]>= 24.0)
  first_hr[quadrant]-=24.0;
/* 
seconds below norm; adjust
*/
 while (first_sec [quadrant]< 0) {
  first_sec[quadrant]+=60.0;
  first_minute [quadrant]-= 1.0;
 }
 while (first_minute [quadrant]< 0) {
  first_minute[quadrant]+=60.0;
  first_hr [quadrant]-= 1.0;
 }
 first_hr[quadrant]=first_hr[quadrant]>=0?first_hr[quadrant]:
  first_hr[quadrant]+24.0;
 if (plot_type[quadrant]==TIME_SERIES) {
/* 
default context: black on background color
*/
  (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,time_color[quadrant],0,0,
   FONTID(GetTimeStampFont(quadrant)));
/* 
LAYOUT: clear out area with previous time stamps  
*/
  (*FillArea[GetDrawType()])(pw_xid[quadrant],
    XAXIS_HILITE_X,
    XAXIS_HILITE_Y(quadrant,0),
    XAXIS_HILITE_WIDTH(quadrant),
    XAXIS_HILITE_HEIGHT(quadrant,0),
    time_color[quadrant]==REVERSE_VIDEO?FOREGROUND:0);
   (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,
    time_color[quadrant]==REVERSE_VIDEO?0:FOREGROUND,0,0,
     FONTID(GetTimeStampFont(quadrant)));
/* 
label time stamps on time series plots from left to right 
*/
  for (i=0;i<TLABELS;i++) {
   if (i==TLABELS-1) {  
/* 
set label to exact current time, avoiding imprecision on floating point addition 
of fractions
*/
    hr=local_hr[quadrant]; 
    minute=local_minute[quadrant]; 
    sec=local_sec[quadrant];
   } else {
    hr=(int)first_hr[quadrant];  
    minute=(int)first_minute[quadrant];  
    sec=(int)first_sec[quadrant];
   }
   (void)sprintf(time_label[i],"%02d:%02d:%02d",hr,minute,sec);
/*
LAYOUT: write out time stamps on x-axis
*/
   (*DrawTtext[GetDrawType()])(
    time_tick_x[quadrant][i]-
     FONTWIDTH(GetTimeStampFont(quadrant))*TIME_STAMP_LEN/2,
    xaxis_range_label_y[quadrant][0],
    time_label[i]);
 
   if (i==TLABELS-1)  
/* 
no need to increment 
*/
    break;
/* 
increment and adjust 
*/
   first_sec[quadrant]+=tlabel_intrvl[quadrant];
   while (first_sec [quadrant]>= 60.0) {
    first_minute[quadrant]+=1.0;
    first_sec[quadrant]-=60.0;
   }
   while (first_minute [quadrant]>=60.0) {
    first_hr[quadrant]+=1.0;
    first_minute[quadrant]-=60.0;
   }
   while (first_hr [quadrant]>= 24.0)
    first_hr[quadrant]-=24.0;
  }
 } else {
  (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,time_color[quadrant],0,0,
   FONTID(GetTimeFrameFont(quadrant)));
/* 
time stamps at upper right corner of non-time series plots
*/
  hr=(int)first_hr[quadrant];  
  minute=(int)first_minute[quadrant];  
  sec=(int)first_sec[quadrant];
  (void)sprintf(start_time,"%02d:%02d:%02d",hr,minute,sec);
  (void)sprintf(end_time,"%02d:%02d:%02d",
   local_hr[quadrant],local_minute[quadrant],local_sec[quadrant]);
  (void)sprintf(time_interval,"%s to %s",start_time,end_time);
/*
LAYOUT: write out time frame string
*/
  (*DrawText[GetDrawType()])(
   window_width[quadrant]-((TIME_STAMP_LEN*2+strlen(" to ")+1)
    *FONTWIDTH(GetTimeFrameFont(quadrant))),
   FONTASCENT(GetTimeFrameFont(quadrant)),
   time_interval);
  got_start_time[quadrant]=TRUE;
 }
}

/*************************  DATA_STAMPS()  ************************************/

data_stamps(quadrant,current_point)
int quadrant,current_point;

{
static char data_label[MAX_TRACES][VALUELEN]; 
static char indep_data_label[MAX_TRACES][VALUELEN]; /* time stamps  */
static int i;

/* update the data values on each name label */

 for (i=0; i<num_of_traces[quadrant]; i++) {
  switch (dep_mode[quadrant][i]) {
   case BITS:
    (void)sprintf(data_label[i],hex?"x%05x":"%06o",
     (bitdata[current_point**len_raw+dep_ptr[quadrant][i]]));
    break;
   case VOLTAGE:
    (void)sprintf(data_label[i],"%f",
     (voltsdata[current_point**len_raw+dep_ptr[quadrant][i]]));
    break;
   case RAW:
    (void)sprintf(data_label[i],"%f",
     (rawdata[current_point**len_raw+dep_ptr[quadrant][i]]));
    break;
   case DERIVED:
    (void)sprintf(data_label[i],"%f",
     (data[current_point**len_derive+dep_ptr[quadrant][i]]));
    break;
  }
  data_label[i][DATA_LABELLEN] = '\0';
  if (plot_type [quadrant]!= TIME_SERIES) {
   switch (indep_mode[quadrant]) {
    case BITS:  /* don't allow BITS or VOLTAGE on independent variable */
     break;
    case VOLTAGE:
     break;
    case RAW:
     if (plot_type[quadrant]==CALIBRATION)  /* use designated set point */
      (void)sprintf(indep_data_label[i],"%f",set_point);
     else
      (void)sprintf(indep_data_label[i],"%f",
       (rawdata[current_point**len_raw+indep_ptr[quadrant]]));
     break;
    case DERIVED:
     (void)sprintf(indep_data_label[i],"%f",
       (data[current_point**len_derive+indep_ptr[quadrant]]));
     break;
   }
   indep_data_label[i][DATA_LABELLEN] = '\0';
  }
  if (plot_type[quadrant]!=CALIBRATION && 
    (i==highlighted_trace[quadrant] ||
/* 
if same-scale in effect, reverse video ALL data stamps
*/
    (get_same_scale(quadrant) && 
    ((get_indepvar_yaxis(quadrant) && pending_op[quadrant]==CHG_X_SCALE) ||
    (!get_indepvar_yaxis(quadrant) && pending_op[quadrant]==CHG_Y_SCALE))))) {
   if (hilite_debug)
    printf("reverse video the data stamp; highlighted trace: %d\n",i);
   if (pending_op[quadrant] != CHG_INDEPVAR) {
/*
LAYOUT: reverse video of dependent variable data stamp area
*/
    (*FillArea[GetDrawType()])(pw_xid[quadrant],
     depvar_data_x[quadrant][i],
     DATASTAMPY(quadrant),
     DATASTAMPWIDTH(quadrant),
     DATASTAMPHEIGHT(quadrant),
     color[quadrant][i]);
    (*SetContext[GetDrawType()])(pw_xid[quadrant],
     GXcopy,REVERSE_VIDEO,color[quadrant][i],0,
     FONTID(GetTraceLabelFont(quadrant)));
/*
LAYOUT: write out dependent variable data stamp
*/
    (*DrawTtext[GetDrawType()])(
     depvar_data_x[quadrant][i],
     label_y[quadrant][i],
     data_label[i]); 
   } else  {
/*
LAYOUT: reverse video of independent variable data stamp area
*/
    (*FillArea[GetDrawType()])(pw_xid[quadrant],
     indepvar_data_x[quadrant][i],
     DATASTAMPY(quadrant),
     DATASTAMPWIDTH(quadrant),
     DATASTAMPHEIGHT(quadrant),
     FOREGROUND);
    (*SetContext[GetDrawType()])(pw_xid[quadrant],
     GXcopy,REVERSE_VIDEO,color[quadrant][i],0,
     FONTID(GetTraceLabelFont(quadrant)));
/*
LAYOUT: write out independent variable data stamp
*/
    (*DrawTtext[GetDrawType()])(
     indepvar_data_x[quadrant][i],
     label_y[quadrant][i],
     indep_data_label[i]); 
   }
  } else {
   (*SetContext[GetDrawType()])(pw_xid[quadrant],
     GXcopy,color[quadrant][i],0,0,
     FONTID(GetTraceLabelFont(quadrant)));
/*
LAYOUT: write out dependent variable data stamp string
*/
   (*DrawText[GetDrawType()])(
    depvar_data_x[quadrant][i],
    label_y[quadrant][i],
    data_label[i]); 
   if (plot_type [quadrant]!= TIME_SERIES) {
    (*SetContext[GetDrawType()])(pw_xid[quadrant],
     GXcopy,FOREGROUND,0,0,FONTID(GetTraceLabelFont(quadrant)));
/*
LAYOUT: write out independent variable data stamp string
*/
    (*DrawText[GetDrawType()])(
     indepvar_data_x[quadrant][i],
     label_y[quadrant][i],
     indep_data_label[i]); 
   }
  }
 }
}

/*************************  SET_DEP_RANGE()  **************************/

set_dep_range(quadrant,type)
int quadrant,type;       /* either DEFAULT or CUSTOM  */
{
 double range_temp,min_temp,get_same_scale_max(),get_same_scale_min(),get_avg();
 int trace;

 trace=trace_number[quadrant];
 if (plot_type[quadrant] == SKEW_T) {
/* 
fix x-axis, assumed same-scaled temp and dewpt types
*/
  dep_rng_typ[quadrant][0]=FIXED;

 } else {

  if (get_same_scale(quadrant)) {

/* get range from current same scale settings
*/
   range_temp=get_same_scale_max(quadrant)-get_same_scale_min(quadrant); 
   if (range_temp<0) {

/* range_temp is overloaded -- a bad choice, but we'll live with it for now.  If
   it is less than zero, that means the range is a floating one; however, if
   same-scale is set AND its range is inverted, it will be less than zero; BUT
   the range MUST be a fixed one for same-scale sake.  Brute force it here,
   and remind ourselves that ALL scales will be inverted while same-scale is in
   effect.  Eventually remove that overloading from the range_temp entry in
   plot_attr file by adding a new field for fixed/floating types!  That way,
   you can default a variable's range to inverted even without same-scale...
*/
    range_temp=ABS(range_temp);
    dep_invert_scale[quadrant][dep_attr_num[quadrant][trace]]=TRUE;
   }

  } else {

/* get range from either local or default version of plot_attr file 
*/

   (void)sscanf(type==CUSTOM? 
    &custom_range[quadrant][dep_attr_num[quadrant][trace]*VALUELEN]:
    &default_range[dep_attr_num[quadrant][trace]*VALUELEN],
    "%lf",&range_temp);
  }

  if (range_temp < 0) {

   dep_rng_typ[quadrant][trace]=FLOATING;
/**************************  ASSUMPTION  ***********************************/
/* range_temp specifies what is default for the ENTIRE range, from bottom
   to top of scale; set_float_range is expecting a plus-or-minus value from
   the center of plot frame; ergo, divide range_temp by 2
*/
   set_float_dep_range(quadrant,trace,get_avg(quadrant,seconds_to_avg[quadrant],
    dep_ptr[quadrant][trace],dep_mode[quadrant][trace]),
    -range_temp/2.0,AUTO_ADJUST,dep_attr_num[quadrant][trace]);
/*************************  END ASSUMPTION  ********************************/

  } else {   /* fixed range  */

   dep_rng_typ[quadrant][trace]=FIXED;
   if (get_same_scale(quadrant)) {
    min_temp=get_same_scale_min(quadrant); 
   } else {
    (void)sscanf(type==CUSTOM? 
     &custom_mins[quadrant][dep_attr_num[quadrant][trace]*VALUELEN]:
     &default_mins[dep_attr_num[quadrant][trace]*VALUELEN],
     "%lf",&min_temp);
   }
   set_fixed_dep_range(quadrant,trace,min_temp,min_temp+
    (dep_invert_scale[quadrant][dep_attr_num[quadrant][trace]]?-1:1)*range_temp,
    dep_attr_num[quadrant][trace]);
  }
 } 
}

/*************************  SET_INDEP_RANGE()  **********************/
 
set_indep_range(quadrant,type)
int quadrant,type;
{
double range_temp,indep_temp,get_avg();

 if (plot_type[quadrant] !=SKEW_T) {

/* get current range from either local or default version of plot_attr file */

  (void)sscanf(type==CUSTOM? &custom_range[quadrant][indep_attr_num[quadrant]*VALUELEN]:
                       &default_range[indep_attr_num[quadrant]*VALUELEN],
                       "%lf",&range_temp);
  if (range_temp < 0) {
   indep_rng_typ[quadrant]=FLOATING;
/**************************  ASSUMPTION  ***********************************/
/* range_temp specifies what is default for the ENTIRE range, from bottom
   to top of scale; set_x_float_range is expecting a plus-or-minus value from
   the center of plot frame; ergo, divide range_temp by 2
*/
   set_float_indep_range(quadrant,get_avg(quadrant,seconds_to_avg[quadrant],
    indep_ptr[quadrant],indep_mode[quadrant]),-range_temp/2.0,AUTO_ADJUST,
    indep_attr_num[quadrant]);
/*************************  END ASSUMPTION  ********************************/
  } else {   /* fixed range  */
   indep_rng_typ[quadrant]=FIXED;
   (void)sscanf(type==CUSTOM? &custom_mins[quadrant][indep_attr_num[quadrant]*VALUELEN]:
                        &default_mins[indep_attr_num[quadrant]*VALUELEN],
                        "%lf",&indep_temp);
   set_fixed_indep_range(quadrant,indep_temp,indep_temp+
    (indep_invert_scale[quadrant][indep_attr_num[quadrant]]?-1:1)*range_temp,
    indep_attr_num[quadrant]);
  }
  if (plot_type[quadrant]==CALIBRATION)
   set_point=(float)indep_min[quadrant];
 } else { /* establish skewed scale for x-axis, assumed temperature */
  indep_rng_typ[quadrant]=FIXED;
  skew_height[quadrant]=log10(MAX_PRESS)-log10(skew_minpress[quadrant]);
  indep_min[quadrant]=skew_minpress[quadrant]; 
  indep_max[quadrant]=MAX_PRESS;
  skew_ymax[quadrant]=log10(indep_max[quadrant]);
  skew_ymin[quadrant]=log10(indep_min[quadrant]);
  indep_intrvl[quadrant]=100.0;
  skew_y_tix[quadrant] = (indep_max[quadrant]-indep_min[quadrant])/indep_intrvl[quadrant]+1;
 }
}

/*******************  SET_FLOAT_DEP_RANGE()  *********************************/

set_float_dep_range(quadrant,trace,midpt,plus_or_minus,mode,attrnum)
int trace,quadrant,attrnum;
double midpt;
double plus_or_minus;
int mode;
{
double acceptable_interval(),acceptable_midpt();

   if (plus_or_minus==0.0)  {
    printf("+- is zero in dep func: %lf\n\n",plus_or_minus);
   }
   dep_intrvl[quadrant][trace]=
    mode==AUTO_ADJUST?
     acceptable_interval(
      plus_or_minus/( (NUM_TIX-1)/2),dep_mode[quadrant][trace]):
     plus_or_minus/( (NUM_TIX-1)/2);
   if (dep_invert_scale[quadrant][dep_attr_num[quadrant][trace]] && 
       dep_intrvl[quadrant][trace]>0)
    dep_intrvl[quadrant][trace]*=-1;
   midpt=acceptable_midpt(midpt,dep_intrvl[quadrant][trace]);
   dep_min[quadrant][trace]=midpt-2*dep_intrvl[quadrant][trace];
   dep_max[quadrant][trace]=midpt+2*dep_intrvl[quadrant][trace];
   clip_tolerance[quadrant][trace]=tolerance_factor[quadrant]*
    ABS(dep_max[quadrant][trace]-dep_min[quadrant][trace]);
   (void)sprintf(&custom_range[quadrant][attrnum*VALUELEN],"%lf",   
     -(ABS(dep_max[quadrant][trace]-dep_min[quadrant][trace])));
   (void)sprintf(&custom_mins[quadrant][attrnum*VALUELEN],"%lf",
    dep_min[quadrant][trace]);
}


/*******************  SET_FLOAT_INDEP_RANGE()  ********************************/

set_float_indep_range(quadrant,midpt,plus_or_minus,mode,attrnum)
double midpt;
double plus_or_minus;
int quadrant,mode,attrnum;
{
double acceptable_interval(),acceptable_midpt();

   if (plus_or_minus==0.0)  {
    printf("+- is zero in indep func: %lf\n\n",plus_or_minus);
   }
   indep_intrvl[quadrant]=
    mode==AUTO_ADJUST?acceptable_interval(plus_or_minus/( (NUM_TIX-1)/2),indep_mode[quadrant]):
                  plus_or_minus/( (NUM_TIX-1)/2);
   if (indep_invert_scale[quadrant][indep_attr_num[quadrant]] && indep_intrvl[quadrant]>0)
    indep_intrvl[quadrant]*=-1;
   midpt=acceptable_midpt(midpt,indep_intrvl[quadrant]);
   indep_min[quadrant]=midpt-2*indep_intrvl[quadrant];
   indep_max[quadrant]=midpt+2*indep_intrvl[quadrant];
   indep_clip_tolerance[quadrant]=tolerance_factor[quadrant]*ABS(indep_max[quadrant]-indep_min[quadrant]);
   if (plot_type [quadrant]!= TIME_SERIES ) {
    (void)sprintf(&custom_range[quadrant][attrnum*VALUELEN],"%lf",   
      -(ABS(indep_max[quadrant]-indep_min[quadrant])));
    (void)sprintf(&custom_mins[quadrant][attrnum*VALUELEN],"%lf",indep_min[quadrant]);
   }
}

/**********************  SET_FIXED_DEP_RANGE()  ****************************/

set_fixed_dep_range(quadrant,trace,bot,top,attrnum)
int trace,quadrant,attrnum;
double top,bot;
{
 
 dep_min[quadrant][trace]=bot; dep_max[quadrant][trace]=top;
 dep_intrvl[quadrant][trace]=(top-bot)/(NUM_TIX-1);
 if (dep_invert_scale[quadrant][dep_attr_num[quadrant][trace]] && dep_intrvl[quadrant][trace]>0)
  dep_intrvl[quadrant][trace]*=-1;
 
/* update local versions */

 if (!get_same_scale(quadrant)) {
/*
don't bother changing customized versions of range and minimum if plot
window is same-scaled: all scales are the same for all vars regardless,
and besides this way we can remember the previously customized versions
when user goes out of same scale
*/
  (void)sprintf(&custom_range[quadrant][attrnum*VALUELEN],"%lf",ABS(bot-top));
  (void)sprintf(&custom_mins[quadrant][attrnum*VALUELEN],"%lf",bot);
 }
 
}

/**********************  SET_FIXED_INDEP_RANGE()  ****************************/

set_fixed_indep_range(quadrant,bot,top,attrnum)
int quadrant,attrnum;
double top,bot;
{
 indep_min[quadrant]=bot; 
 indep_max[quadrant]=top;  
 indep_intrvl[quadrant]=(top-bot)/(NUM_TIX-1);
 if (indep_invert_scale[quadrant][indep_attr_num[quadrant]] && indep_intrvl[quadrant]>0)
  indep_intrvl[quadrant]*=-1;
 if (plot_type [quadrant]!= TIME_SERIES) {
  (void)sprintf(&custom_range[quadrant][attrnum*VALUELEN],"%lf",ABS(bot-top));
  (void)sprintf(&custom_mins[quadrant][attrnum*VALUELEN],"%lf",bot);
 }
}

/****************  EVENT_IN_ADD_TRACE_AREA()  *******************************/

event_in_add_trace_area(y)
int y;
/* is the current canvas event in the area below the last trace_label?
   If so, set trace number to current value of num_of_traces, increment
   num_of_traces, prepare an appropriate temporary trace_label, and 
   return TRUE (unless there are already MAX_TRACES traces displayed)
*/
{


 if (plot_type[current_quadrant]==SKEW_T)
  return(FALSE);

 if (!label_y[current_quadrant][0])
/* KBD_USE event detected before DrawTraceLabels() has seen repaint_set[]
as TRUE; hence, label_y not set yet and this is NOT an add_trace operation --
it's a new window overlaying an old one.  Return here to prevent condition
"y>label_y[...]" becoming TRUE below
*/
  return(FALSE);

 if (!(num_of_traces[current_quadrant])) {
  trace_number[current_quadrant]=0;
  return(TRUE);
 }

 if (y > label_y[current_quadrant][num_of_traces[current_quadrant]-1] 
     && y < window_height[current_quadrant] ) {
   if (num_of_traces[current_quadrant] < MAX_TRACES) {
/* 
there actually is an add_trace area available
*/
    trace_number[current_quadrant]=num_of_traces[current_quadrant];
/*
LAYOUT: y-position of add-trace trace label
*/
    label_y[current_quadrant][trace_number[current_quadrant]]=
     label_y[current_quadrant][trace_number[current_quadrant]-1]
     +TRACELABELHEIGHT(current_quadrant);
    return(TRUE);
   } else
    return(FALSE);
 } else
  return(FALSE);
}


/****************  EVENT_IN_CHG_TRACE_AREA()  *******************************/

event_in_chg_trace_area(y)
int y;
/* is the current canvas event in the area with the trace_label's?
   If so, set trace_number to the value that points to the selected trace
   in the trace_label & label_y arrays, and return TRUE; else, return FALSE
*/
{
 VarIsIndep[current_quadrant]=FALSE;
 if (!label_y[current_quadrant][0])
  return(FALSE);

 if (y < 0)
  return(FALSE);

 if (!(num_of_traces[current_quadrant])) {
  return(FALSE);
 }

/* skew-t plots have no add-trace area; apply similar lower boundary test here
   as would be applied in event_in_add_trace_area() for skew-t plots.
*/
 if ( (plot_type[current_quadrant]==SKEW_T && 
       y > window_height[current_quadrant] ) ||
      (plot_type[current_quadrant]!=SKEW_T && 
       y > label_y[current_quadrant][num_of_traces[current_quadrant]-1]) )
  return (FALSE);


 for (trace_number[current_quadrant]=num_of_traces[current_quadrant]-1; 
      trace_number[current_quadrant] >= 0; 
      trace_number[current_quadrant]--) {
  if (y >= TopOfTraceLabel(current_quadrant,trace_number[current_quadrant])) {
   if (plot_type[current_quadrant]!=TIME_SERIES 
    && GetMouseX() >= window_width[current_quadrant]/2)
    VarIsIndep[current_quadrant]=TRUE;
   return(TRUE);
  }
 }
 return(FALSE);
}

/****************  EVENT_IN_CHG_TIME_SCALE_AREA()  ****************************/

event_in_chg_time_scale_area(y)
int y;
{
 if (plot_type[current_quadrant]!=TIME_SERIES) 
  return(FALSE);
 if (!label_y[current_quadrant][0])
  return(FALSE);
 if (!(num_of_traces[current_quadrant])) {
  return(FALSE);
 }
 if (y>=XAXIS_HILITE_Y(current_quadrant,0) && 
     y<=label_y[current_quadrant][0] - 
     FONTHEIGHT(GetTraceLabelFont(current_quadrant))) {
/***
     y<= XAXIS_HILITE_Y(current_quadrant,0)+XAXIS_HILITE_HEIGHT-1) {
***/
  trace_number[current_quadrant]=INDEPVAR;
  return(TRUE);
 } else
  return(FALSE);
}

/******************************************************************************/

TopOfTraceLabel(quadrant,trace)
int quadrant,trace;
{
 return label_y[quadrant][trace]-FONTHEIGHT(GetTraceLabelFont(quadrant))+1;
}

/****************  EVENT_IN_CHG_X_SCALE_AREA()  *******************************/

event_in_chg_x_scale_area(y)
int y;
/* is the current canvas event in the area below plot region and above the
   first name label?  

   If so,  return TRUE
*/
{
static int TopOfRangeLbl,BottomOfRngLbl;
int num_traces;
int trace;

 if (plot_type[current_quadrant]==TIME_SERIES ||
     plot_type[current_quadrant]==SKEW_T)
  return(FALSE);

 if (!label_y[current_quadrant][0])
  return(FALSE);

 num_traces=get_isolate_traces(current_quadrant) ||
            get_same_scale(current_quadrant)?1:num_of_traces[current_quadrant];
 if (!(num_traces)) {
  return(FALSE);
 }
 if (y<XAXIS_HILITE_Y(current_quadrant,0) ||
/*
it's in plot area
*/
     y>=TopOfTraceLabel(current_quadrant,0))
/*
it's in trace label area
*/
  return FALSE;
/*
by now, we know we are in the right area.  Determine which trace is current.
*/
 if (!get_indepvar_yaxis(current_quadrant)) {
  trace_number[current_quadrant]=INDEPVAR;
  return TRUE;
 }
 for (trace_number[current_quadrant]=0;
      trace_number[current_quadrant] < num_traces;
      trace_number[current_quadrant]++) {
  trace=trace_number[current_quadrant];
/*
LAYOUT: y-coords of top and bottom of each x-axis range label
*/
  TopOfRangeLbl=XAXIS_HILITE_Y(current_quadrant,trace);
  BottomOfRngLbl=TopOfRangeLbl+XAXIS_HILITE_HEIGHT(current_quadrant,trace)-1;
  if ( y >= TopOfRangeLbl && y <= BottomOfRngLbl ) {
   if (get_isolate_traces(current_quadrant)) {
/*
then all labels are in one row. Use x position to determine which trace is
current.
*/
    for (trace=0; trace<num_of_traces[current_quadrant]; trace++){
     if (GetMouseX() >= (trace==0?0:rleft[current_quadrant]+
          trace*rwidth[current_quadrant]/num_of_traces[current_quadrant])
     && GetMouseX()< (trace==num_of_traces[current_quadrant]-1?
          window_width[current_quadrant]:
          rleft[current_quadrant]+
          (trace+1)*rwidth[current_quadrant]/num_of_traces[current_quadrant])
     ) {
      trace_number[current_quadrant]=trace;
      return TRUE; 
     }
    }
   } else
    return(TRUE);
  }
 }
 return(FALSE);
}

/****************  EVENT_IN_CHG_Y_SCALE_AREA()  *******************************/

event_in_chg_y_scale_area(x,y)
int x,y;
/* is the current canvas event in the area to left of region and above the
   region bottom?  

   If so, set trace_number to the value that points to the selected trace
   in the trace_label & label_x arrays, and return TRUE
*/
{
static int x_coord,next_x_coord;
int num_traces;
int trace;

 if (plot_type[current_quadrant]==SKEW_T)
  return(FALSE);
 
 if (!label_x[current_quadrant][0])
  return(FALSE);

 num_traces=get_isolate_traces(current_quadrant) ||
            get_same_scale(current_quadrant)?1:num_of_traces[current_quadrant];
 if (!(num_traces)) {
  return(FALSE);
 }
 if (y < 0 || y >= XAXIS_HILITE_Y(current_quadrant,0) ||
     x >= rleft[current_quadrant]) 
  return(FALSE);

 if (get_indepvar_yaxis(current_quadrant)) {
  trace_number[current_quadrant]=INDEPVAR;
  return TRUE;
 }
 for (trace_number[current_quadrant]=0; 
      trace_number[current_quadrant] < num_traces; 
      trace_number[current_quadrant]++) {
  trace=trace_number[current_quadrant];
  x_coord=(trace==0?
   0:label_x[current_quadrant][trace]);
  next_x_coord=(trace<num_traces-1)?
   label_x[current_quadrant][trace+1]:
   rleft[current_quadrant];
  if ( x >= x_coord && x < next_x_coord ) {
   if (get_isolate_traces(current_quadrant)) {
/*
then all labels are in same column. Use y position to determine which trace is
current.
*/
    for (trace=0; trace<num_of_traces[current_quadrant]; trace++){
     if (y >= (trace==0?0:rtop[current_quadrant]+
          trace*rheight[current_quadrant]/num_of_traces[current_quadrant])
     && y< (trace==num_of_traces[current_quadrant]-1?
/* 
this one can slip between the cracks, so to speak, as cursor moves between
y-axis label area and x-axis label area, due to a not so tight cooperation
between the area definitions for those two areas. Cover it up with this
kluge.
*/
            XAXIS_HILITE_Y(current_quadrant,0):
            rtop[current_quadrant]+(trace+1)*
            rheight[current_quadrant]/num_of_traces[current_quadrant])
     ) {
      trace_number[current_quadrant]=trace;
      return TRUE;
     }
    } 
   } else
    return(TRUE);
  }
 }
 return(FALSE);
}

/*******************  EVENT_IN_PLOT_FRAME_AREA()  ****************************/

event_in_plot_frame_area(x,y)
int x,y;
/* is the current canvas event in the area within the region? 
   if so, return TRUE
*/
{

 if (plot_type[current_quadrant]==SKEW_T)
  return(FALSE);

 if (!(num_of_traces[current_quadrant])) {
  return(FALSE);
 }
 if (plot_type[current_quadrant]==TRACK_PLOT) {
  if (y <= rbottom[current_quadrant] && x >= rleft[current_quadrant] && 
      !event_in_vector_scale_area(x,y))
   return(TRUE);
 } else {
  if (y <= rbottom[current_quadrant] && x >= rleft[current_quadrant]) 
   return(TRUE);
 }
 return(FALSE);
}

/********************  HIGHLIGHT_NAME_LABEL()  ******************************/

highlight_trace_label(quadrant,operation)
int quadrant,operation;
/* reverse video the selected trace_label
*/
{
static int bkgnd_color, /* color to highlight background, as function of 
                           operation  */
    trace,
    shift;       /* how many characters to shift right the variable name as 
                    function of message preceding it  */
static char var_name[LABELLEN];

/* 
highlight background 
*/
 trace=trace_number[quadrant];
 bkgnd_color=(operation==ADD_TRACE)?allocate_color(quadrant):
  (operation==CHG_INDEPVAR?FOREGROUND:color[quadrant][trace]);
 shift=0;
/*
LAYOUT: highlight name label
*/
 (*FillArea[GetDrawType()])(pw_xid[quadrant],
  0,
  label_y[quadrant][trace]-FONTASCENT(GetTraceLabelFont(quadrant)),
  window_width[quadrant],
  FONTHEIGHT(GetTraceLabelFont(quadrant)),
  bkgnd_color);
 (*SetContext[GetDrawType()])(pw_xid[quadrant],
  GXcopy,REVERSE_VIDEO,0,0,
  FONTID(GetTraceLabelFont(quadrant)));
 switch (operation) {
  case CHG_TRACE:
   if (plot_type[quadrant]==SKEW_T) { /* delete is not an option */
/*
LAYOUT: write out hilited "Chg or Delete <trace>" string
*/
    (*DrawTtext[GetDrawType()])(
     LABEL_LEFT_X,
     label_y[quadrant][trace],
     "Chg ");
    shift=strlen("Chg ");
   } else {
    (*DrawTtext[GetDrawType()])(
     LABEL_LEFT_X,
     label_y[quadrant][trace],
     num_of_traces[quadrant]==1?"Chg ":"Chg or Delete ");
    shift=num_of_traces[quadrant]==1?strlen("Chg "):
                                     strlen("Chg or Delete ");
   }
   break;
  case CHG_INDEPVAR:
/*
LAYOUT: write out hilited "Chg Range" string
*/
   (*DrawTtext[GetDrawType()])(
     LABEL_LEFT_X,
     label_y[quadrant][trace],
     "Chg ");
   shift=strlen("Chg ");
   break;
  case CHG_FIXED_SCALE:
   (*DrawTtext[GetDrawType()])(
     LABEL_LEFT_X,
     label_y[quadrant][trace],
     "Chg Fixed Range For ");
   shift=strlen("Chg Fixed Range For ");
   break;
  case CHG_FLOATING_SCALE:
   (*DrawTtext[GetDrawType()])(
     LABEL_LEFT_X,
     label_y[quadrant][trace],
    "Chg Floating Range For ");
   shift=strlen("Chg Floating Range For ");
   break;
  case ADD_TRACE:
   (void)sprintf(var_name,"Add Trace");
   break;
 }

 if (operation!=ADD_TRACE) {
  if (operation != CHG_INDEPVAR) {
   (void)sprintf(var_name,"%s (",trace_name[quadrant][trace]);
   depvar_data_x[quadrant][trace]=LABEL_LEFT_X+
    FONTWIDTH(GetTraceLabelFont(quadrant))*(shift+strlen(var_name));
   (void)sprintf(&var_name[strlen(var_name)],"       %s)",
    &units[dep_attr_num[quadrant][trace]*UNITSLEN]);
  } else {
   (void)sprintf(var_name,"Indep't Variable: %s (",
    (char *)GetPlotAttrName(indep_attr_num[quadrant]));
   indepvar_data_x[quadrant][trace]=LABEL_LEFT_X+
    FONTWIDTH(GetTraceLabelFont(quadrant))*(shift+strlen(var_name));
   (void)sprintf(&var_name[strlen(var_name)],"       %s)",
    &units[indep_attr_num[quadrant]*UNITSLEN]);
  }
 }
/*
LAYOUT: write out hilited variable name 
*/
 (*DrawTtext[GetDrawType()])(
  LABEL_LEFT_X+shift*FONTWIDTH(GetTraceLabelFont(quadrant)),
  label_y[quadrant][trace],
  var_name);
 data_stamps(quadrant,(start_point[quadrant]+time_gone_by[quadrant]-1)%get_datablock_size());
}

/**********************  CLEAR_NAME_LABEL()  ******************************/

clear_trace_label(quadrant,mode,trace_num)
int quadrant,mode,trace_num;
/* 
restore the selected trace_label to normal video
*/
{

 if (trace_num==NONE)
  return;

 if (plot_type[quadrant]==LISTS || plot_type[quadrant]==FIXED_FORMAT) 
  return;

 if (trace_num>num_of_traces[quadrant]) /* sometimes event wants to clear 
                          label from previous config'n that had more traces  
                          than this new one. Ignore. */ 
  return; 

/*
LAYOUT: clear hiliting of trace label
*/
 (*FillArea[GetDrawType()])(pw_xid[quadrant],
  0,
  label_y[quadrant][trace_num]-FONTASCENT(GetTraceLabelFont(quadrant)),
  window_width[quadrant],
  FONTHEIGHT(GetTraceLabelFont(quadrant)),
  0);
 if (mode == REFRESH) {

  depvar_data_x[quadrant][trace_num]=
/* 
add 2 for " (" part of string 
*/
   LABEL_LEFT_X
   +FONTWIDTH(GetTraceLabelFont(quadrant))
   *(strlen(trace_name[quadrant][trace_num])+2); 
  indepvar_data_x[quadrant][trace_num]=save_indepvar_data_x[quadrant][trace_num];
  (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,color[quadrant][trace_num],
   0,0,FONTID(GetTraceLabelFont(quadrant)));
/*
LAYOUT: write out cleared dependent var trace label
*/
  (*DrawText[GetDrawType()])(
   LABEL_LEFT_X,
   label_y[quadrant][trace_num],
   trace_label[quadrant][trace_num]);
  if (plot_type[quadrant]!=TIME_SERIES) {
   (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,FOREGROUND,0,0,FONTID(GetTraceLabelFont(quadrant)));
/*
LAYOUT: write out cleared independent var trace label
*/
   (*DrawText[GetDrawType()])(
   LABEL_LEFT_X+
    FONTWIDTH(GetTraceLabelFont(quadrant))*INDEPVARDATAX,
   label_y[quadrant][trace_num],
   indep_label[quadrant][trace_num]);
  }
  data_stamps(quadrant,
   (start_point[quadrant]+time_gone_by[quadrant]-1)%get_datablock_size());
 }
}

/********************  HIGHLIGHT_RANGE_LABEL()  ******************************/

highlight_range_label(quadrant)
int quadrant;
/* 
reverse video the selected range_label
*/
{
int yaxis,trace;

if (plot_type[quadrant]==SKEW_T)
 return;

if (!label_x[quadrant][0])
 return;

trace=trace_number[quadrant];
yaxis=(get_indepvar_yaxis(quadrant)  && trace==INDEPVAR) ||
      (!get_indepvar_yaxis(quadrant) && trace!=INDEPVAR);

if (yaxis) {
/* 
LAYOUT: hilite y-axis range labels
*/
  (*FillArea[GetDrawType()])(pw_xid[quadrant],
   YAXIS_HILITE_X(quadrant),
   YAXIS_HILITE_Y(quadrant),
   YAXIS_HILITE_WIDTH(quadrant),
   YAXIS_HILITE_HEIGHT(quadrant),
   (trace==INDEPVAR || get_same_scale(quadrant))?BLACK:color[quadrant][trace]);
  DrawRangeLabels(quadrant,REVERSE_VIDEO,trace);
/*
also highlight associated trace labels with special feedback messages, unless
independent var is on y-axis
*/
  if (get_indepvar_yaxis(quadrant))
   return;

  if (dep_rng_typ[quadrant][trace] == FIXED)
   if (get_same_scale(quadrant)) { 
/* 
hilite ALL of them
*/
    for (trace_number[quadrant]=0; 
         trace_number[quadrant]<num_of_traces[quadrant]; 
         trace_number[quadrant]++) 
     highlight_trace_label(quadrant,CHG_FIXED_SCALE); 
    trace_number[quadrant]=0; 
   } else 
/* 
hilite just the associated trace label -- trace_number[] is already set
*/
    highlight_trace_label(quadrant,CHG_FIXED_SCALE);
  else
   highlight_trace_label(quadrant,CHG_FLOATING_SCALE);
 } else { 
/* 
LAYOUT: highlight x-axis range label(s)  
*/
  (*FillArea[GetDrawType()])(pw_xid[quadrant],
   XAXIS_HILITE_X,
   XAXIS_HILITE_Y(quadrant,trace),
   XAXIS_HILITE_WIDTH(quadrant),
   XAXIS_HILITE_HEIGHT(quadrant,trace),
   (trace==INDEPVAR || get_same_scale(quadrant))?BLACK:color[quadrant][trace]);
  if (plot_type [quadrant]== TIME_SERIES) {
/* 
time_color[] can't be sent as argument to time_stamps() since per second 
calls don't know whether this field is highlighted or not 
*/
   time_color[quadrant]=REVERSE_VIDEO; 
   time_stamps(quadrant);
  } else
   DrawRangeLabels(quadrant,REVERSE_VIDEO,trace);
/*
highlight associated trace labels, unless independent variable is on x-axis 
*/
  if (!get_indepvar_yaxis(quadrant))
   return;

  if (dep_rng_typ[quadrant][trace] == FIXED)
   if (get_same_scale(quadrant)) { 
/* 
hilite ALL of them
*/
    for (trace_number[quadrant]=0; 
         trace_number[quadrant]<num_of_traces[quadrant]; 
         trace_number[quadrant]++) 
     highlight_trace_label(quadrant,CHG_FIXED_SCALE); 
    trace_number[quadrant]=0; 
   } else 
/* 
hilite just the associated trace label -- trace_number[] is already set
*/
    highlight_trace_label(quadrant,CHG_FIXED_SCALE);
  else
   highlight_trace_label(quadrant,CHG_FLOATING_SCALE);
 }
}

/**********************  CLEAR_RANGE_LABEL()  ******************************/

clear_range_label(quadrant,trace)
int quadrant,trace;
/* restore the selected range_label to normal video -- locking done by 
   highlighting routine, unlocking done by clear_trace_label()
*/
{
int yaxis;

 if (trace>=num_of_traces[quadrant]) 
/* 
sometimes LOC_MOVE event wants to clear label
from previous config'n that had more traces 
than this new one. Ignore. 
*/
  return;
 if (plot_type[quadrant]==LISTS || plot_type[quadrant]==FIXED_FORMAT) 
  return;
 if (plot_type[quadrant]==SKEW_T)  
/* 
no range changes allowed 
*/
  return;
 if (trace==NONE)
  return;

 yaxis=(get_indepvar_yaxis(quadrant)  && trace==INDEPVAR) ||
       (!get_indepvar_yaxis(quadrant) && trace!=INDEPVAR);
/*
LAYOUT: clear hiliting of y-axis range labels
*/
 if (yaxis) {
  (*FillArea[GetDrawType()])(pw_xid[quadrant],
   YAXIS_HILITE_X(quadrant),
   YAXIS_HILITE_Y(quadrant),
   YAXIS_HILITE_WIDTH(quadrant),
   YAXIS_HILITE_HEIGHT(quadrant),
   0);
  DrawRangeLabels(quadrant,
   (trace==INDEPVAR || get_same_scale(quadrant))?BLACK:
   color[quadrant][trace],trace);
/*
clear associated trace label(s) unless y-range is independent variable
*/
  if (!get_indepvar_yaxis(quadrant)) {
   if (get_same_scale(quadrant)) {
/* 
clear ALL of them unless independent var is on y-axis: none were hilited, for now
*/
    for (trace=0;
         trace<num_of_traces[quadrant];
         trace++) {
     clear_trace_label(quadrant,REFRESH,trace);
    }
   } else {
/* 
clear just the associated trace label
*/ 
    clear_trace_label(quadrant,REFRESH,trace);
   }
  }
 } else {
/* 
LAYOUT: clear hiliting on x-axis range labels
*/
  (*FillArea[GetDrawType()])(pw_xid[quadrant],
   XAXIS_HILITE_X,
   XAXIS_HILITE_Y(quadrant,trace),
   XAXIS_HILITE_WIDTH(quadrant),
   XAXIS_HILITE_HEIGHT(quadrant,trace),
   0);
  if (plot_type [quadrant]== TIME_SERIES) {
   time_color[quadrant]=FOREGROUND;
   time_stamps(quadrant);
  } else
   DrawRangeLabels(quadrant,
    (trace==INDEPVAR || get_same_scale(quadrant))?BLACK:
    color[quadrant][trace],
    trace);
/*
clear associated trace label(s) unless x-range is independent variable
*/
  if (get_indepvar_yaxis(quadrant)) {
   if (get_same_scale(quadrant)) {
/* 
clear ALL of them unless independent var is on y-axis: none were hilited, for now
*/
    for (trace=0;
         trace<num_of_traces[quadrant];
         trace++) {
     clear_trace_label(quadrant,REFRESH,trace);
    }
   } else {
/* 
clear just the associated trace label
*/ 
    clear_trace_label(quadrant,REFRESH,trace);
   }
  }
 }
}

/************************  DRAW_ISOBARS()  **********************************/

draw_skew_bkgnd(xid,quadrant)
Drawable xid;
int quadrant;
/* draw skew-t background lines.  Isobars are drawn horizontally at y-position
   of y-range labels.  Isotherms drawn at 45 degree angle starting from lower
   border at x-range labels.  Dry adiabats are drawn according to equation
   given in "Introduction to Theoretical Meteorology", Seymour L. Hess (1959).

   This routine is called once at startup, and subsequently whenever frame
   is redrawn (change of trace, refresh, etc.).
*/
{
static int npts,calculations_needed[NUM_DISPLAY_QUADRANTS];
static int ColorMap[NUM_DISPLAY_QUADRANTS];
static double previous_skew_width[NUM_DISPLAY_QUADRANTS]=
{0,0,0,0};
static double previous_skew_height[NUM_DISPLAY_QUADRANTS]=
{0,0,0,0};
static int first_time_through= TRUE;
static int j,line_drawn,
           x_coord,y_coord,
           x1,y_1,x2,y2,x1_,y1_,x2_,y2_;
static double log_press,temp,theta,Const,ln_1000;
static Server_image skew_bkgnd[NUM_DISPLAY_QUADRANTS];
static Drawable skew_xid[NUM_DISPLAY_QUADRANTS];
XPoint *ptlist;
static double log_base_e_10;

#define KAPPA .28566      /* R over c sub p (see Hess) */
#define K_TO_C (-273.15)   /* conversion from Kelvin to Centigrade */
#define MB_TO_DYNES 1000.0  /* convert from millibars to dynes per sq. cm. */
/* determine if current setup is same as previous one; if so, calculations are
   not needed to draw background.  Different setup? calculations needed
*/
 if (ColorMap[quadrant]!=(*GetColorMap[GetDrawType()])() ||
     skew_width[quadrant] != previous_skew_width[quadrant] ||
     skew_height[quadrant] != previous_skew_height[quadrant] 
     ) {
  ColorMap[quadrant]=(*GetColorMap[GetDrawType()])();
  (*InitMemDraw[GetDrawType()])
   (&skew_bkgnd[quadrant],rwidth[quadrant],rheight[quadrant],
   BYTE_LEN);
  skew_xid[quadrant]=(Drawable)xv_get(skew_bkgnd[quadrant],XV_XID);
  calculations_needed[quadrant]=TRUE;
/* UNRESOLVED LINT PROBLEM */ 
  ptlist=(XPoint *)(malloc((unsigned)(sizeof(short)*2*rheight[quadrant])));
  if (first_time_through) { /* set constants 1st time thru */
   ln_1000=KAPPA*log(1000.0 * MB_TO_DYNES);  /* log natural of 1000.0 */
   log_base_e_10=log(10.0);   /* log base e of 10 */
   first_time_through=FALSE;
  } 
/*
LAYOUT: clear plot region on skew-t
*/
  (*FillArea[GetDrawType()])(skew_xid[quadrant],
   0,0,rwidth[quadrant],rheight[quadrant],0); 
  previous_skew_width[quadrant]=skew_width[quadrant]; 
  previous_skew_height[quadrant]=skew_height[quadrant]; 
 } 

 if (calculations_needed[quadrant]) {

/* draw brown curved dry adiabats */

  (*SetContext[GetDrawType()])(skew_xid[quadrant],GXcopy,BRN,0,LineSolid,0);
  temp=dep_min[quadrant][0]-K_TO_C;
  do {
/* for each temperature from left to right:  */
   npts=0;
/* determine the constant in its corresponding equation for theta: */
   Const=log(temp)-ln_1000;
/* vary over range of pressure, solve for theta, plot line; don't start
   for loop at 0 since clipping not checked in DrawPolyline() */
   for (y_coord=1; y_coord<rheight[quadrant]; y_coord++ ) {
    log_press=(double)y_coord/indep_scale[quadrant]+skew_ymin[quadrant]; /* log base 10 of pressure */
/* add 3 to logarithm base 10 of pressure to convert to dynes/cm2  */
    theta=exp(KAPPA * (log_press+3) * log_base_e_10 + Const)+K_TO_C;
    x_coord=(theta+DEGREES_C_PER_LOG*(skew_ymax[quadrant]-log_press)-dep_min[quadrant][0])*dep_scale[quadrant][0];
    if (x_coord>0 && x_coord <rwidth[quadrant]) {
     ptlist[npts].x=(short)x_coord;
     ptlist[npts++].y=(short)y_coord;
    }
   }
   (*DrawPolyline[GetDrawType()])(npts,ptlist);
/* continue for next temperature value ...*/
   temp+=dep_intrvl[quadrant][0];
/* until temperature's theta line is entirely outside of plot frame */
  } while (npts);
/* UNRESOLVED LINT PROBLEM */ 
  free((char *)(ptlist));

/* draw brown horizontal isobars */

  for (j=0; j<skew_y_tix[quadrant]; j++)
   (*DrawLine[GetDrawType()])(0,isobar_y[quadrant][j]-rtop[quadrant],rwidth[quadrant],isobar_y[quadrant][j]-rtop[quadrant]);

/* draw brown skewed isotherms at 45 degree angle */

  x1=rightmost_isotherm_x[quadrant]-rleft[quadrant]; y_1=rheight[quadrant]; y2=0;
  line_drawn=FALSE;
  do { 
/* since slope is 1 for 45 degree angle, delta x is equal to delta y */
   x2=x1+rheight[quadrant];
/* starting from right, draw isotherms until clipping returns no more to draw */
   if (( line_drawn=
    (clip_vector(0,rwidth[quadrant],0,rheight[quadrant],
     x1,y_1,x2,y2,&x1_,&y1_,&x2_,&y2_))
    )) {
    (*DrawLine[GetDrawType()])(x1_,y1_,x2_,y2_);
    x1-=isotherm_intrvl[quadrant];
   }
  } while (line_drawn);
  calculations_needed[quadrant]=FALSE;
 }
 (*RasterOp[GetDrawType()])(skew_xid[quadrant],0,0,xid,0,0,rwidth[quadrant],rheight[quadrant],GXcopy);
}

/**********************  GET_SKEW_T_INFO()  *********************************/

get_skewt_info(quadrant)
int quadrant;
/* define parameters here that depend on other things and thus must wait
   to be defined
*/
{
  label_x[quadrant][0] = LABEL_LEFT_X;
  SetPlotBorders(quadrant);
  SetXaxisDepVarYPosns(quadrant);
  SetYaxisIndepVarXposns(quadrant);
  indep_scale[quadrant] = (double)rheight[quadrant]/(skew_ymax[quadrant]-skew_ymin[quadrant]);
  skew_width[quadrant]=DEGREES_C_PER_LOG * skew_height[quadrant] * rwidth[quadrant]/rheight[quadrant];
  dep_min[quadrant][0]=dep_min[quadrant][1]=skew_mintemp[quadrant]; 
  dep_max[quadrant][0]=dep_max[quadrant][1]=skew_mintemp [quadrant]+ skew_width[quadrant];
  dep_scale[quadrant][0]=dep_scale[quadrant][1]=(double)rwidth[quadrant]/(dep_max[quadrant][0]-dep_min[quadrant][0]);
  if (skew_width [quadrant]<= 10.0)
   dep_intrvl[quadrant][0]=dep_intrvl[quadrant][1]=1.0;
  else if (skew_width [quadrant]<= 30.0)
   dep_intrvl[quadrant][0]=dep_intrvl[quadrant][1]=5.0;
  else 
   dep_intrvl[quadrant][0]=dep_intrvl[quadrant][1]=10.0;
  skew_x_tix[quadrant] = skew_width[quadrant]/dep_intrvl[quadrant][0]+1;
}

/******************************************************************************/

GetStackedLabelOffset(quadrant,toplabel)
int quadrant,toplabel;
/*
return y-coord adjustment needed for the shared labels in isolate-trace
plots, either for top or bottom label
*/
{
 if (toplabel)
  return -1 * GetFontAscent(GetDefaultRangeLabelFont(quadrant))/2;
 else
  return -1 * GetFontAscent(GetDefaultRangeLabelFont(quadrant))/2 +
         GetFontAscent(GetSmallRangeLabelFont(quadrant));
}

/******************************************************************************/

GetAscentStackedLabel(quadrant)
int quadrant;
/*
if this an isolate-trace plot display, return the ascent of the stacked label
font; else, return zero
*/
{
 if (get_isolate_traces(quadrant)) {
  return GetFontAscent(GetSmallRangeLabelFont(quadrant));
 } else {
  return 0;
 }
}

/******************************************************************************/

GetHeightStackedLabel(quadrant)
int quadrant;
/*
isolated traces with dependent range labels on one row have shared tick marks
using stacked labels that take a little more than the usual height.  Adjust
for this here.
*/
{
 if (get_isolate_traces(quadrant)) {
  return GetFontHeight(GetSmallRangeLabelFont(quadrant));
 } else {
  return 0;
 }
}

/******************************************************************************/

DrawXaxisDepVarRngLbls(quadrant,color,trace)
int quadrant,color,trace;
/*
draw the x-axis dependent variable range labels
*/
{
int j;
/*
draw dependent (x-axis) label(s)
*/
   for (j=0; j<NUM_TIX; j++) {
    if (get_isolate_traces(quadrant)) {
/*
all range labels are in one row; adjust font and y-coord offset for those labels
that point to shared tick marks
*/
     if (j==NUM_TIX-1 && trace!=0) {
/*
trace's first label, sharing tick with trace on its left: decrease font size
*/
      SetRangeLabelFont(quadrant,color==REVERSE_VIDEO);
     } else if (j==0 && trace!=num_of_traces[quadrant]-1) {
/*
trace's last label, sharing tick with trace on right: decrease font size 
*/
      SetRangeLabelFont(quadrant,color==REVERSE_VIDEO);
     } else {
/*
non-shared tick; normal font size
*/
      SetRangeLabelFont(quadrant,TRUE);
     }
    } else {
/*
normal separated rows, normal fonts
*/
     SetRangeLabelFont(quadrant,TRUE);
    }
    (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,color,0,0,
     FONTID(GetRangeLabelFont(quadrant)));
/*
LAYOUT: write out x-axis dependent var range labels
*/
    (*DrawTtext[GetDrawType()])(
/* 
x position: start at right -- offset if traces are to be isolated.  
*/
     (rright[quadrant]-(get_isolate_traces(quadrant)?
      (num_of_traces[quadrant]-1-trace)*
      rwidth[quadrant]/num_of_traces[quadrant]:0))-
/* 
determine j'th tick mark x-position 
*/
     j*XaxisTickInterval[quadrant] 
/* 
subtract half of label length to center it -- use floating arithmetic to account
for odd-number lengths of strings, so as not to drop 50% of the pixel result
*/
     -(int)((float)((float)(dep_lbl_len[quadrant][trace][j])/2.0
      * FONTWIDTH(GetRangeLabelFont(quadrant)))),
     GetXaxisDepVarYPosn(quadrant,trace,j,color),
     dep_rnglbl[quadrant][trace][j]);
   } 
}

/******************************************************************************/

DrawYaxisIndepVarRngLbls(quadrant,color)
int quadrant,color;
/*
draw the y-axis independent variable range labels
*/
{
int j;
int first_label_y;

/***
   SetYaxisIndepVarXposns(quadrant);
***/
   SetRangeLabelFont(quadrant,TRUE);
   first_label_y=rbottom[quadrant]+
    FONTASCENT(GetDefaultRangeLabelFont(quadrant))/2;
   (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,color,0,0,
    FONTID(GetDefaultRangeLabelFont(quadrant)));
/*
draw independent (y-axis) label
*/
   for (j=0; j<NUM_TIX; j++) {
/*
LAYOUT: write out y-axis independent var range labels
*/
     (*DrawTtext[GetDrawType()])(
/* 
x positions of y-axis labels: leftmost edge of x labels 
*/
      label_x[quadrant][0]+
/* 
...plus maximum label length minus length of this label in pixels to 
left-justify y-axis labels
*/
       (max_indep_len[quadrant]-
       indep_lbl_len[quadrant][j])*
       FONTWIDTH(GetDefaultRangeLabelFont(quadrant)),
       GetYaxisIndepVarYPosn(first_label_y,j,YaxisTickInterval[quadrant]),
      indep_rnglbl[quadrant][j]);
   }
}

/******************************************************************************/

DrawXaxisIndepVarRngLbls(quadrant,color)
int quadrant,color;
/*
draw the x-axis independent variable range labels
*/
{
int j;
/*
draw independent (x-axis) label
*/
   (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,color,0,0,
    FONTID(GetDefaultRangeLabelFont(quadrant)));
   for (j=0; j<NUM_TIX; j++) {
/*
LAYOUT: write out x-axis independent range labels
*/
    (*DrawTtext[GetDrawType()])(
/* 
x position: start at left 
*/
      rleft[quadrant]
/* 
determine j'th tick mark position 
*/
      +j*XaxisTickInterval[quadrant] 
/* 
subtract half of label length to center it 
*/
      -(int)((float)((float)(indep_lbl_len[quadrant][j])/2.0
       *FONTWIDTH(GetDefaultRangeLabelFont(quadrant)))),
      GetXaxisIndepVarYposn(quadrant),
      indep_rnglbl[quadrant][j]);
   }
}

/******************************************************************************/

DrawYaxisDepVarRngLbls(quadrant,color,trace)
int quadrant,color,trace;
/*
draw the y-axis dependent variable range labels
*/
{
int j;
/*
draw dependent (y-axis) labels -- start at y-coord with fonts centered
against tick marks
*/
   for (j=0; j<NUM_TIX; j++) {
    if (get_isolate_traces(quadrant)) {
/*
all range labels are in one column; adjust font 
*/
     if (j==0 && trace!=0) {
/*
trace's first label, sharing tick with trace above it: decrease font size 
*/
      SetRangeLabelFont(quadrant,color==REVERSE_VIDEO);
     } else if (j==NUM_TIX-1 && trace!=num_of_traces[quadrant]-1) {
/*
trace's last label, sharing tick with trace below it: decrease font size 
*/
      SetRangeLabelFont(quadrant,color==REVERSE_VIDEO);
     } else {
/*
non-shared tick; normal fonts
*/
      SetRangeLabelFont(quadrant,TRUE);
     }
    } else {
/*
normal separated rows; normal fonts
*/
      SetRangeLabelFont(quadrant,TRUE);
    }
    (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,color,0,0,
     FONTID(GetRangeLabelFont(quadrant)));
/*
write out y-axis dependent var range labels
*/
    (*DrawTtext[GetDrawType()])(
       GetYaxisDepVarXposn(quadrant,j,trace,color),
       GetYaxisDepVarYPosn(quadrant,trace,j,YaxisTickInterval[quadrant],color),
      dep_rnglbl[quadrant][trace][j]);
   }
}

/******************************************************************************/

GetMaxIndepVarLen(quadrant)
int quadrant;
/*
return length in characters of the longest range label on independent variable
axis
*/
{
 return max_indep_len[quadrant];
}

/******************************************************************************/

GetMaxDepVarLen(quadrant)
int quadrant;
/*
return length in characters of the longest range label on all of the
dependent variable axes
*/
{
int trace,maxlen;

 maxlen=0;
 for (trace=0; trace<num_of_traces[quadrant]; trace++)
  maxlen=MAX(maxlen,max_dep_len[quadrant][trace]);
 return maxlen;
}

/******************************************************************************/

GetMaxLenYaxisLabel(quadrant)
int quadrant;
/*
return the longest string length on the y-axis
*/
{
 if (get_indepvar_yaxis(quadrant))
  return GetMaxIndepVarLen(quadrant);
 else
  return GetMaxDepVarLen(quadrant);
}

/******************************************************************************/

GetMaxLenXaxisLabel(quadrant)
int quadrant;
/*
return the longest string length on the x-axis
*/
{
 if (get_indepvar_yaxis(quadrant))
  return GetMaxDepVarLen(quadrant);
 else
  return GetMaxIndepVarLen(quadrant);
}

/******************************************************************************/

CheckRngLblFontSizes(quadrant,length,xinterval,yinterval)
int quadrant,length,xinterval,yinterval;
/*
verify that current font sizes are not either too large or too small for 
a newly sized plot border, adjust if needed
*/
{
int totalwidth,fontwidth;
int newheight;   

 SetDefaultRangeLabelFont(quadrant,
  FindFont(GetRangeLabelFontHeight(quadrant),GetRangeLabelFontWidth(quadrant)));

 newheight=yinterval+1;
 fontwidth=GetFontWidth(GetDefaultRangeLabelFont(quadrant));
 totalwidth=fontwidth*(length+1);
 
 while ((fontwidth>=GetMinFontWidth() &&
         totalwidth>=xinterval) ||
        (newheight>=GetMinFontHeight() &&
         GetFontAscent(GetDefaultRangeLabelFont(quadrant))>=yinterval)) { 
  SetDefaultRangeLabelFont(quadrant,
   FindFont(MIN(newheight,GetRangeLabelFontHeight(quadrant)),
            MIN(fontwidth,GetRangeLabelFontWidth(quadrant))));
  if (fontwidth>=GetMinFontWidth() && totalwidth>=xinterval) {
   fontwidth--;
   totalwidth=fontwidth*(length+1);
  }
  if (newheight>=GetMinFontHeight() &&
      GetFontAscent(GetDefaultRangeLabelFont(quadrant))>=yinterval) {
   newheight--;
  }
 }
/*
explicitly set the small font at least once to ensure that it is at least
one size in width smaller than default font
*/
 fontwidth=GetFontWidth(GetSmallRangeLabelFont(quadrant))*length;
 newheight=yinterval;
 SetSmallRangeLabelFont(quadrant,
  FindFont(MIN(newheight,
  GetFontHeight(GetDefaultRangeLabelFont(quadrant))-1),
  MIN(fontwidth,GetFontWidth(GetDefaultRangeLabelFont(quadrant))-1)));
 while ((newheight>=GetMinFontHeight() &&
        GetFontHeight(GetSmallRangeLabelFont(quadrant))>=yinterval) ||
        (fontwidth>=GetMinFontWidth() &&
        totalwidth>=xinterval)) {
  SetSmallRangeLabelFont(quadrant,
   FindFont(MIN(newheight,
   GetFontHeight(GetDefaultRangeLabelFont(quadrant))-1),
   MIN(fontwidth,GetFontWidth(GetDefaultRangeLabelFont(quadrant))-1)));
  if (newheight>=GetMinFontHeight() &&
        GetFontHeight(GetSmallRangeLabelFont(quadrant))>=yinterval) {
   newheight--;
  }
  if (fontwidth>=GetMinFontWidth() &&
        totalwidth>=xinterval) {
   fontwidth--;
   totalwidth=fontwidth*(length+1);
  }
 }
}

/**********************  DrawRangeLabels()  ***********************************/

DrawRangeLabels(quadrant,color,trace)
int quadrant,color,trace;
/* 
draw the range labels 
*/
{
 if (plot_type[quadrant]==SKEW_T) {
  DrawSkewtRngLbls(quadrant,color,trace);
  return;
 } 
 if (get_indepvar_yaxis(quadrant)) {
/* 
independent variable on y-axis 
*/
  if (trace == INDEPVAR) {
   DrawYaxisIndepVarRngLbls(quadrant,color);
  } else {
   DrawXaxisDepVarRngLbls(quadrant,color,trace);
  }
 } else {  
/* 
then, the independent variable is on the x-axis  
*/
  if (trace == INDEPVAR) {
   DrawXaxisIndepVarRngLbls(quadrant,color);
  } else {
   DrawYaxisDepVarRngLbls(quadrant,color,trace);
  }
 }
}

/**********************  DrawSkewtRngLbls()  *******************************/

DrawSkewtRngLbls(quadrant,color,trace)
int quadrant,color,trace;
/* draw the range labels -- it is assumed users of this routine manage their 
   own locking and unlocking.  Assumed y-axis labels drawn first, and that
   get_skewt_info() has been called prior to draw of x-axis labels
*/
{
int j;
int first_label_y,isotherm_x;
double value,next_temp,temperature;
int last_printed,xaxis_label_x,last_label_end;

  if (trace == INDEPVAR) { 
/* 
indep't var -- pressure -- labelled on y-axis -- start at bottom:
*/
   first_label_y=rbottom[quadrant];
   last_printed=FALSE;
   (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,color,0,0,
    FONTID(GetDefaultRangeLabelFont(quadrant)));
   for (j=0; j<skew_y_tix[quadrant]; j++){
    (void)sscanf(indep_rnglbl[quadrant][j],"%lf",&value);
    isobar_y[quadrant][j]=first_label_y-
     (int)((float)rheight[quadrant]*(log10(indep_max[quadrant])-log10(value))/
     skew_height[quadrant]);
    if (isobar_y[quadrant][j]>=rtop[quadrant]-1) 
     {
/* 
don't print top label above region 
don't print label if it will obscure last label printed below it 
*/
     if ( !last_printed || isobar_y[quadrant][j] < 
      (isobar_y[quadrant][j-1]-FONTHEIGHT(GetDefaultRangeLabelFont(quadrant)))) {
      last_printed=TRUE;
/*
LAYOUT: write out skew-t independent variable (y-axis) range label
*/
      (*DrawText[GetDrawType()])(
       label_x[quadrant][0]+(max_indep_len[quadrant]-indep_lbl_len[quadrant][j])
        *FONTWIDTH(GetDefaultRangeLabelFont(quadrant)),
       isobar_y[quadrant][j]+FONTASCENT(GetDefaultRangeLabelFont(quadrant))/2,
       indep_rnglbl[quadrant][j]);
     } else
      last_printed=FALSE;
     }
   }
  } else {  
/* 
dependent vars -- temp and dewpoint -- labelled once on x-axis 
*/
   isotherm_x=rleft[quadrant];
   last_label_end=0;
   for (j=0; j<skew_x_tix[quadrant]; j++) {
    (void)sscanf(dep_rnglbl[quadrant][trace][j],"%lf",&temperature);
    if (j==0) {
/* 
need interval (in pixels) between each label to facilitate drawing skewed 
isotherms
*/
     (void)sscanf(dep_rnglbl[quadrant][trace][1],"%lf",&next_temp);
     isotherm_intrvl[quadrant]=(int)
      (rwidth[quadrant]*((next_temp-temperature)/skew_width[quadrant]));
    } 
    xaxis_label_x=isotherm_x-
     (int)((float)((float)(dep_lbl_len[quadrant][trace][j])/2.0*FONTWIDTH(GetDefaultRangeLabelFont(quadrant))));
/* 
print label only if it will not obscure last label printed
*/
    if (xaxis_label_x>last_label_end) {
/*
LAYOUT: write out skew-t dependent variable (x-axis) range label
*/
     (*DrawText[GetDrawType()])(
       xaxis_label_x,
       xaxis_range_label_y[quadrant][0]
/*
don't have ticks in skew-t plots, so remove allowance for that in values
for y-position of labels
*/
      -TICK_LENGTH(quadrant)-1,
      dep_rnglbl[quadrant][trace][j]);
     last_label_end=xaxis_label_x+
      strlen(dep_rnglbl[quadrant][trace][j])*FONTWIDTH(GetDefaultRangeLabelFont(quadrant));
    } 
    isotherm_x+=isotherm_intrvl[quadrant];
   }
/* get position of rightmost label 
*/
   rightmost_isotherm_x[quadrant] = isotherm_x-isotherm_intrvl[quadrant]; 
  }
}

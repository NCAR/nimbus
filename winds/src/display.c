/* display.c -- update the screen display with the function assigned in
  config_display() routine


LINT accomodations: y_1 used instead of simply y1 due to 'redefinition of
earlier one' coming from who know where...

global variable names have first letter capitalized ...

*/
#include <xview/xview.h>
#include <xview/svrimage.h>
#include <stdio.h>
#include <math.h>

/****************************  WINDS INCLUDES  ******************************/

#include <xy_plt.h>
#include <display.h>
#include <lengths.h>
#include <shmem.h>
#include <ops.h>
#include <help.h>
#include <colors.h>
#include <timer.h>
#include <constants.h>
#include <socket.h>
#include <printers.h>
#include <header.h>
#include <macros.h>
#include <track.h>
#include <network.h>
#include <vectors.h>
#include <geom.h>
#include <proto.h>


/****************************  EXPORTABLE  ********************************/

/*
drawing functions
*/
int (*FlushDisplay[NUM_DRAWING_TYPES])()= {
 X11FlushDisplay,PSFlushDisplay,NULL
};
int (*InitDraw[NUM_DRAWING_TYPES])()= {
 X11InitDraw,PSInitDraw,NULL
};
int (*InitMemDraw[NUM_DRAWING_TYPES])()= {
 X11InitMemDraw,NULL,NULL
};
int (*SetHiliteColors[NUM_DRAWING_TYPES])()= {
 X11SetHiliteColors,PSSetHiliteColors,NULL
};
int (*SetContext[NUM_DRAWING_TYPES])()= {
 X11SetContext,PSSetContext,NULL
};
int (*SetClipping[NUM_DRAWING_TYPES])()= {
 X11SetClipping,PSSetClipping,NULL
};
int (*UnsetClipping[NUM_DRAWING_TYPES])()= {
 X11UnsetClipping,PSUnsetClipping,NULL
};
int (*GetYOffset[NUM_DRAWING_TYPES])()= {
 X11GetYOffset,PSGetYOffset,NULL
};
int (*GetXOffset[NUM_DRAWING_TYPES])()= {
 X11GetXOffset,PSGetXOffset,NULL
};
int (*SetOffset[NUM_DRAWING_TYPES])()= {
 X11SetOffset,PSSetOffset,NULL
};
int (*FillArea[NUM_DRAWING_TYPES])()= {
 X11FillArea,PSFillArea,NULL
};
int (*ContextDrawLine[NUM_DRAWING_TYPES])()= {
 X11ContextDrawLine,PSContextDrawLine,NULL
};
int (*DrawFilledPolygon[NUM_DRAWING_TYPES])()= {
 X11DrawFilledPolygon,PSDrawFilledPolygon,NULL
};
int (*DrawCircle[NUM_DRAWING_TYPES])()= {
 X11DrawCircle,PSDrawCircle,NULL
};
int (*DrawLine[NUM_DRAWING_TYPES])()= {
 X11DrawLine,PSDrawLine,NULL
};
int (*LimitCoords[NUM_DRAWING_TYPES])()= {
 X11LimitCoords,PSLimitCoords,NULL
};
int (*DrawDashLine[NUM_DRAWING_TYPES])()= {
 X11DrawDashLine,PSDrawDashLine,NULL
};
int (*ContextDrawPolyline[NUM_DRAWING_TYPES])()= {
 X11ContextDrawPolyline,PSContextDrawPolyline,NULL
};
int (*DrawPolyline[NUM_DRAWING_TYPES])()= {
 X11DrawPolyline,PSDrawPolyline,NULL
};
int (*DrawPolypoint[NUM_DRAWING_TYPES])()= {
 X11DrawPolypoint,PSDrawPolypoint,NULL
};
int (*RasterOp[NUM_DRAWING_TYPES])()= {
 X11RasterOp,PSRasterOp,NULL
};
int (*MonoRasterOp[NUM_DRAWING_TYPES])()= {
 X11MonoRasterOp,PSMonoRasterOp,NULL
};
int (*DestroyImage[NUM_DRAWING_TYPES])()= {
 X11DestroyImage,PSDestroyImage,NULL
};
int (*GetImage[NUM_DRAWING_TYPES])()= {
 X11GetImage,PSGetImage,NULL
};
int (*DrawText[NUM_DRAWING_TYPES])()= {
 X11DrawText,PSDrawText,NULL
};
int (*DrawTtext[NUM_DRAWING_TYPES])()= {
 X11DrawTtext,PSDrawTtext,NULL
};
int (*GetColorMap[NUM_DRAWING_TYPES])()= {
 X11GetColorMap,PSGetColorMap,NULL
};
int (*SetColorMap[NUM_DRAWING_TYPES])()= {
 X11SetColorMap,PSSetColorMap,NULL
};
int (*SetupColors[NUM_DRAWING_TYPES])()= {
 X11SetupColors,PSSetupColors,NULL
};

int (*display_func[NUM_DISPLAY_TYPES])()= {/* function that updates displays  */
 time_series,xy,skew_t,xy,xy,lists,fixes,NULL
};
/* 
scale factors -- declared double to facilitate precision  
*/ 
double indep_scale[NUM_QUADRANTS],dep_scale[NUM_QUADRANTS][MAX_TRACES];  
Server_image mem_image[NUM_DISPLAY_QUADRANTS];

/* display update parameters
*/
int got_start_time[NUM_QUADRANTS]=   /* TRUE after initial data in current 
                                        interval */
{FALSE,FALSE,FALSE,FALSE,FALSE};		

int start_point[NUM_QUADRANTS]=  /* data point at which to start for display */
{0,0,0,0};

/* whether or not a list row has been inverted to reflect current data point
*/
int inversed_row[NUM_DISPLAY_QUADRANTS]=
{FALSE,FALSE,FALSE,FALSE};

/***************************  GLOBAL WITHIN FILE  ****************************/

/* display update parameters
*/
static int svrimage_draw[NUM_QUADRANTS]=
{FALSE,FALSE,FALSE,FALSE,FALSE}; 		
static double left_over[NUM_QUADRANTS]= /* accumulate fractional parts of shift
			left to maintain accurate scaled time-series drawing */
{0.0,0.0,0.0,0.0,0.0};
static int vector_drawn[NUM_QUADRANTS]=
{FALSE,FALSE,FALSE,FALSE,FALSE}; 	/* true after first vector is drawn  */
static int LastDrawEnd[NUM_QUADRANTS]=
{0,0,0,0,0};              		/* last point displayed */
static int NextDrawStart[NUM_QUADRANTS]=
{0,0,0,0,0};          		/* next start point for current drawing op'n */
static int Last_xcoord[NUM_QUADRANTS][MAX_TRACES];  /* changed in time series shift left */
static int Pixel_width[NUM_QUADRANTS]=
{0,0,0,0,0};       		/* # of pixels to shift left for current # of
                                   data points */
static int Raw_start_block,
        Derive_start_block;
static int mode[NUM_QUADRANTS];                /* NEW or ALL points */

static Drawable mem_image_xid[NUM_DISPLAY_QUADRANTS];


#define CIRCLE_DIAMETER 8
static int current_x[NUM_DISPLAY_QUADRANTS][MAX_TRACES];
static int current_y[NUM_DISPLAY_QUADRANTS][MAX_TRACES]; 
static int flash_on[NUM_DISPLAY_QUADRANTS][MAX_TRACES]=  /* TRUE when flashing
				current point is flashed ON */
{FALSE,FALSE,FALSE,FALSE,
 FALSE,FALSE,FALSE,FALSE,
 FALSE,FALSE,FALSE,FALSE,
 FALSE,FALSE,FALSE,FALSE};

static int DrawType=X11;

/************************************************************************/

SetPointsDrawMode(quadrant,allpoints)
int quadrant,allpoints;
{
 if (allpoints)
  mode[quadrant]=ALL_POINTS;
 else
  mode[quadrant]=NEW_POINTS;
}
 
/***************************  GET_DRAW_TYPE()  **************************/

int
GetDrawType()
{
 return DrawType;
}

/***************************  SET_DRAW_TYPE()  **************************/

SetDrawType(type)
int type;
{
 DrawType=type;
}

/***************************  INIT_DISPLAY_PARMS()  **************************/

init_display_parms(quadrant)
int quadrant;
{
 got_start_time[quadrant]=FALSE;
 start_point[quadrant]=0;
 svrimage_draw[quadrant]=FALSE; 		
 left_over[quadrant]=0.0;
 vector_drawn[quadrant]=FALSE; 		
 LastDrawEnd[quadrant]=0;              	
 NextDrawStart[quadrant]=0;          		
 Pixel_width[quadrant]=0;       		
}

/*****************************************************************************/

SetLastDrawEnd(quadrant,value)
int quadrant,value;
{
 LastDrawEnd[quadrant]=value;
}

/*****************************************************************************/

GetNextDrawStart(quadrant)
int quadrant;
{
 return NextDrawStart[quadrant];
}

/*****************************************************************************/

SetNextDrawStart(quadrant,value)
int quadrant,value;
{
 NextDrawStart[quadrant]=value;
}

/***************************  CLEAR_PLOT_FRAME()  ****************************/

clear_plot_frame(quadrant)
int quadrant;
{
 (*FillArea[GetDrawType()])(pw_xid[quadrant],rleft[quadrant],rtop[quadrant],rwidth[quadrant],
                            rheight[quadrant],0);
}

/***************************  UPDATE_VOLTAGE()  ****************************/

update_voltage(quadrant)
int quadrant;
/* per-second update of voltage value in calibration display window 
*/
{
 
 if (!menu_selected) /* no initial menu item selected yet ==> no index set */
  return;
 
 data_stamps(quadrant,((ElapsedTime-1)%get_datablock_size()));
 update_mode[quadrant]=NORMAL_UPDATE;
}

/************************  INIT_CALIB_CURVE()  *******************************/

init_calib_curve()
{
 if (!menu_selected) /* no initial menu item selected yet ==> no index set */
  return(FALSE);

 LastDrawEnd[PLOTS]=time_gone_by[PLOTS]=ElapsedTime;
 init_accums();
 mode[PLOTS]=NEW_POINTS;
 return(TRUE);
}

/***************************  CALIB_CURVE()  ********************************/

calib_curve(set_point)
float set_point;
/* scatter plot designated calibration variable vs. set_point 
*/
{
int numpts;
 
 NextDrawStart[PLOTS] = ((time_gone_by[PLOTS]-1) % get_datablock_size());
 if ((numpts=more_points(PLOTS))) {  

/* there is at least one new point; take one only (the most recent one).  This
   prevents the drawing/accumulation of more points than requested (e.g., if
   2 points requested but numpts is 3, you've overshot...)
*/
  Raw_start_block=((NextDrawStart[PLOTS]+numpts-1)**len_raw)%get_datablock_size();
  data_stamps(PLOTS,((NextDrawStart[PLOTS]+numpts-1)%get_datablock_size()));

/* draw it
*/
  draw_scatter(PLOTS,set_point,NextDrawStart[PLOTS]+numpts-1,1);
 }
 time_gone_by[PLOTS]=ElapsedTime;

/* tell caller that either zero or one point has been drawn
*/
 return(numpts?1:0);
}

/***************************  FITTED_CURVE()  *********************************/

fitted_curve(quadrant,c0,c1,c2)
int quadrant;
float c0,c1,c2;
/* x=c0 + c1*y + c2*y*y

assuming this is used only in calibration window, with one independent
and one dependent variable ONLY...else, use of rheight-1 should be 
adjust as per, e.g., draw_time_series_trace()
*/
{
int ycoord,xcoord;
float x_val, y_val;

static int last_ycoord,last_xcoord;

 first_point[quadrant]=TRUE;
 (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,color[quadrant][0],0,LineSolid,0);
 (*SetClipping[GetDrawType()])(rleft[quadrant],rtop[quadrant],
              rwidth[quadrant],rheight[quadrant]);
 for (ycoord=0; ycoord<rheight[quadrant]; ycoord++) {
  y_val=(float)(rheight[quadrant]-1-ycoord)/dep_scale[quadrant][0]+dep_min[quadrant][0];
  x_val=c0 + c1 * y_val + c2 * y_val * y_val;
  xcoord=(int)(((x_val - indep_min[quadrant]) * indep_scale[quadrant]) + 0.5);
  if (!(first_point[quadrant]))  {
    (*DrawLine[GetDrawType()])(rleft[quadrant]+last_xcoord,rtop[quadrant]+last_ycoord,
              rleft[quadrant]+xcoord,rtop[quadrant]+ycoord);
  } else 
   first_point[quadrant] = FALSE;
  last_ycoord=ycoord;
  last_xcoord=xcoord;
 }
 (*UnsetClipping[GetDrawType()])();
}

/**********************  DRAW_SCATTER()  ***************************/

draw_scatter(quadrant,set_point,start,numpts)
float set_point;
int quadrant,start,numpts;
/* draw vectors from start for numpts points, for each of num_of_traces
   traces. 

assuming this is used only in calibration window, with one independent
and one dependent variable ONLY...else, use of rheight-1 should be 
adjust as per, e.g., draw_time_series_trace()
*/
{
int end_pt,point,trace;
int xcoord,ycoord;

/* for each new point...  */

  end_pt=start + numpts;
  for (point=start;point<end_pt;point++) {

/*  mod the point index to facilitate wraparound from the loop incrementing
*/
   Derive_start_block=(point%get_datablock_size())**len_derive;
   Raw_start_block=(point%get_datablock_size())**len_raw;
/* add 0.5 to round up on conversion to integer */
   if (plot_type[quadrant]==CALIBRATION) {
    xcoord=(int)(((set_point-indep_min[quadrant]) * indep_scale[quadrant]) + 0.5);
    sum_stats(point%get_datablock_size());
   } else {
    switch (indep_mode[quadrant]) {
     case BITS:
     case VOLTAGE:
     case RAW:
      xcoord=(int)(((rawdata[Raw_start_block+indep_ptr[quadrant]]-indep_min[quadrant]) * indep_scale[quadrant]) + 0.5);
      break;
     case DERIVED:
      xcoord=(int)(((data[Derive_start_block+indep_ptr[quadrant]]-indep_min[quadrant]) * indep_scale[quadrant]) + 0.5);
      break;
    }
   }
   

/* for each trace in plot window... */

  for (trace=0; trace<num_of_traces[quadrant]; trace++) {

/* compute y-coordinates of next endpoint */

   switch (dep_mode[quadrant][trace]) {
    case BITS:
     ycoord=(int)((((float)bitdata[Raw_start_block+dep_ptr[quadrant][trace]]
            -dep_min[quadrant][trace]) * dep_scale[quadrant][trace]) + 0.5);
     break;
    case VOLTAGE:
     ycoord=(int)(((voltsdata[Raw_start_block+dep_ptr[quadrant][trace]]
            -dep_min[quadrant][trace]) * dep_scale[quadrant][trace]) + 0.5);
     break;
    case RAW:
     ycoord=(int)(((rawdata[Raw_start_block+dep_ptr[quadrant][trace]]
            -dep_min[quadrant][trace]) * dep_scale[quadrant][trace]) + 0.5);
     break;
    case DERIVED:
     ycoord=(int)(((data[Derive_start_block+dep_ptr[quadrant][trace]]
            -dep_min[quadrant][trace]) * dep_scale[quadrant][trace]) + 0.5);
     break;
   }
   ycoord=rheight[quadrant]-1-ycoord;

   draw_x(quadrant,xcoord,ycoord,color[quadrant][trace],GXcopy,2);
   if (plot_type[quadrant]==CALIBRATION)
    save_coords(xcoord,ycoord);

   }  /* end of loop on number of traces  */
  } /* end of loop on number of points  */
}

/**********************  DRAW_X()  *****************************************/

draw_x(quadrant,xcoord,ycoord,drawcolor,op,length)
int quadrant,op;
int xcoord,ycoord,drawcolor;
{
 (*SetContext[GetDrawType()])(pw_xid[quadrant],op,drawcolor,0,LineSolid,0);
 (*SetClipping[GetDrawType()])(rleft[quadrant],rtop[quadrant],
              rwidth[quadrant],rheight[quadrant]);
 (*DrawLine[GetDrawType()])(rleft[quadrant]+xcoord-length,rtop[quadrant]+ycoord-length,
           rleft[quadrant]+xcoord+length,rtop[quadrant]+ycoord+length);
 (*DrawLine[GetDrawType()])(rleft[quadrant]+xcoord-length,rtop[quadrant]+ycoord+length,
           rleft[quadrant]+xcoord+length,rtop[quadrant]+ycoord-length);
 (*UnsetClipping[GetDrawType()])();
}

/*****************************  XY()  **************************************/

xy(quadrant)
int quadrant;
/* update display(s)  -- perform action as dictated by value of update_mode;
   update_mode must not be reset to NORMAL_UPDATE by anyone
   else except xy() AND functions that set it but
   get ERROR condition returned from set_trace() or indices()
*/


{
int numpts;
int trace;

 set_mode(quadrant);

 if ((numpts=more_points(quadrant))) {

/* new points since last update  
*/
  data_stamps(quadrant,((NextDrawStart[quadrant]+numpts-1)%get_datablock_size()));
  time_color[quadrant]=FOREGROUND;
  time_stamps(quadrant);
  draw_xy_trace(quadrant,NextDrawStart[quadrant],numpts,0,num_of_traces[quadrant]);
  NextDrawStart[quadrant] = ( (start_point[quadrant]+time_gone_by[quadrant])
                                  % get_datablock_size());
 } else if (svrimage_draw[quadrant]) {

  if (plot_type[quadrant]==TRACK_PLOT) {

/* this is an initial rendering on a track plot; draw in any boundaries
   and landmarks
*/
   set_boundary_context(quadrant,TRUE);
   draw_boundaries(quadrant);
   (*UnsetClipping[GetDrawType()])();
   draw_landmarks(quadrant);
  }
  for (trace=0; trace<num_of_traces[quadrant]; trace++)
   flash_on[quadrant][trace]=FALSE;
  (*RasterOp[GetDrawType()])(mem_image_xid[quadrant],0,0,pw_xid[quadrant],
   rleft[quadrant],rtop[quadrant],rwidth[quadrant],rheight[quadrant],GXcopy);
  svrimage_draw[quadrant]=FALSE;
 } 

/* flash current point regardless of new points or not.  Flush display after
   to ensure the graphics are updated.
*/
 for (trace=0; trace<num_of_traces[quadrant]; trace++) {
  flash_current_point(quadrant,trace);
 }
 (*FlushDisplay[GetDrawType()])();
 (*UnsetClipping[GetDrawType()])();
 if (need_rehilite[quadrant]) {
  rehilite(quadrant);
 }
 update_mode[quadrant]=NORMAL_UPDATE;
}


/*****************************  SKEW_T()  ************************************/

skew_t(quadrant)
int quadrant;
/* update display(s)  -- perform action as dictated by value of update_mode;
   update_mode must not be reset to NORMAL_UPDATE by anyone
   else except xy() AND functions that set it but
   get ERROR condition returned from set_trace() or indices()
*/


{
int numpts;
int trace;

 set_mode(quadrant);

 if ((numpts=more_points(quadrant))) {  /* new points since last update  */
  time_color[quadrant]=FOREGROUND;
  time_stamps(quadrant);
  data_stamps(quadrant,((NextDrawStart[quadrant]+numpts-1)%get_datablock_size()));
  draw_xy_trace(quadrant,NextDrawStart[quadrant],numpts,0,num_of_traces[quadrant]);
  NextDrawStart[quadrant] = ( (start_point[quadrant]+time_gone_by[quadrant])
                                  % get_datablock_size());
 } else if (svrimage_draw[quadrant]) {
  for (trace=0; trace<num_of_traces[quadrant]; trace++)
   flash_on[quadrant][trace]=FALSE;
  (*RasterOp[GetDrawType()])(mem_image_xid[quadrant],0,0,pw_xid[quadrant],
   rleft[quadrant],rtop[quadrant],rwidth[quadrant],rheight[quadrant],GXcopy);
  svrimage_draw[quadrant]=FALSE;
 } 

/* flash current point regardless of new points or not.  Flush display after
   to ensure the graphics are updated.
*/
 for (trace=0; trace<num_of_traces[quadrant]; trace++)
  flash_current_point(quadrant,trace);
 (*FlushDisplay[GetDrawType()])();
 (*UnsetClipping[GetDrawType()])();
 if (need_rehilite[quadrant]) {
  rehilite(quadrant);
 }
 update_mode[quadrant]=NORMAL_UPDATE;
}


/**********************  DRAW_XY_TRACE()  ***************************/

draw_xy_trace(quadrant,start,numpts,start_trace,end_trace)
int quadrant,start,numpts,start_trace,end_trace;
/* draw vectors from start for numpts points, for each variable from
   start_trace to end_trace. Set vector_drawn true if at least one vector
   has been drawn.  Refresh screen every UPDATE_RATE operations.
*/
{
int end_pt,point,trace;
int xcoord,ycoord;
int x1,y_1,x2,y2;
double x_value, y_value;

static int last_ycoord[NUM_QUADRANTS][MAX_TRACES];

/* for each new vector...  */

  end_pt=start + numpts;

/* for each trace in plot window... */

  for (trace=0; trace<num_of_traces[quadrant]; trace++)

/* for those current points whose flash is on, flash them off to allow for
   updating of screen w/o disturbing the XOR operation involved in toggling
   the flash on/off
*/
   if (flash_on[quadrant][trace])
    flash_current_point(quadrant,trace);
  (*FlushDisplay[GetDrawType()])();

  for (trace=start_trace; trace<end_trace; trace++) {

/* set width, color, and clipping 
*/
   if (svrimage_draw[quadrant]) {
    (*SetContext[GetDrawType()])(mem_image_xid[quadrant],GXcopy,color[quadrant][trace],
    plot_type[quadrant]==SKEW_T?SKEW_LINEWIDTH:0,LineSolid,0);
    if (get_isolate_traces(quadrant))
     set_isolate_clipping(quadrant,trace);
    else
     (*SetClipping[GetDrawType()])(0,0,rwidth[quadrant],rheight[quadrant]);

/* since flashes are not drawn to server images, set flash_on FALSE for each
   trace
*/
    flash_on[quadrant][trace]=FALSE;
   } else {
    (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,color[quadrant][trace],
    plot_type[quadrant]==SKEW_T?SKEW_LINEWIDTH:0,LineSolid,0);
    if (get_isolate_traces(quadrant))
     set_isolate_clipping(quadrant,trace);
    else
     (*SetClipping[GetDrawType()])(rleft[quadrant],rtop[quadrant],
      rwidth[quadrant],rheight[quadrant]);
   }
       
   first_point[quadrant]=(mode[quadrant]==ALL_POINTS);

   for (point=start;point<end_pt;point++) {
/*  
mod the point index to facilitate wraparound from the loop incrementing
*/
   Derive_start_block=(point%get_datablock_size())**len_derive;
   Raw_start_block=(point%get_datablock_size())**len_raw;
/* 
compute coordinates on independent variable
*/
   switch (indep_mode[quadrant]) {
    case BITS:
    case VOLTAGE:
     break;  /* neither is  allowed as independent var at this time */
    case RAW:
     if (get_indepvar_yaxis(quadrant)) {
      if (plot_type[quadrant]==SKEW_T) {
       x_value=log10(rawdata[Raw_start_block+indep_ptr[quadrant]]);
       xcoord=(int)((x_value-skew_ymin[quadrant]) * indep_scale[quadrant]);
      } else { 
       xcoord=
        (int)(((rawdata[Raw_start_block+indep_ptr[quadrant]]-
        indep_min[quadrant]) * indep_scale[quadrant]) + 0.5);
      }
     } else {
      xcoord=(int)(((rawdata[Raw_start_block+indep_ptr[quadrant]]-
       indep_min[quadrant]) * indep_scale[quadrant]) + 0.5);
     }
     break;
    case DERIVED:
     if (get_indepvar_yaxis(quadrant)) {
      if (plot_type[quadrant]==SKEW_T) {
       x_value=log10(data[Derive_start_block+indep_ptr[quadrant]]);
       xcoord=(int)((x_value-skew_ymin[quadrant]) * indep_scale[quadrant]);
      } else { 
       xcoord=
        (int)(((data[Derive_start_block+indep_ptr[quadrant]]-
        indep_min[quadrant]) * indep_scale[quadrant]) + 0.5);
      }
     } else {
      xcoord=(int)(((data[Derive_start_block+indep_ptr[quadrant]]-
       indep_min[quadrant]) * indep_scale[quadrant]) + 0.5);
     }
     break;
   }
/*
if variable is on y-axis, adjust coord to account for inverted coordinates
*/
   if (get_indepvar_yaxis(quadrant) && plot_type[quadrant]!=SKEW_T)
    xcoord=rheight[quadrant]-1-xcoord;
/* 
compute y-coordinates of next dependent variable endpoint 
*/
   switch (dep_mode[quadrant][trace]) {
    case BITS:
     ycoord=
      (int)((((float)bitdata[Raw_start_block+dep_ptr[quadrant][trace]]
      -dep_min[quadrant][trace]) * dep_scale[quadrant][trace]) + 0.5);
     break;
    case VOLTAGE:
     ycoord=
      (int)(((voltsdata[Raw_start_block+dep_ptr[quadrant][trace]]
      -dep_min[quadrant][trace]) * dep_scale[quadrant][trace]) + 0.5);
     break;
    case RAW:
     if (get_indepvar_yaxis(quadrant)) {
      if (plot_type[quadrant]==SKEW_T) {
       y_value=rawdata[Raw_start_block+dep_ptr[quadrant][trace]];
/* 
x coordinate is to be skewed: value + slope * # logs 
*/
       ycoord=(y_value+DEGREES_C_PER_LOG*(skew_ymax[quadrant]-x_value)-
        dep_min[quadrant][trace])*dep_scale[quadrant][trace];
      } else { 
       ycoord=
       (int)(((rawdata[Raw_start_block+dep_ptr[quadrant][trace]]
       -dep_min[quadrant][trace]) * dep_scale[quadrant][trace]));
      }
     } else {
      ycoord=
      (int)(((rawdata[Raw_start_block+dep_ptr[quadrant][trace]]
      -dep_min[quadrant][trace]) * dep_scale[quadrant][trace]));
     }
     break;
    case DERIVED:
     if (get_indepvar_yaxis(quadrant)) {
      if (plot_type[quadrant]==SKEW_T) {
/* 
y value is the log of computed value 
*/
       y_value=data[Derive_start_block+dep_ptr[quadrant][trace]];
/* 
x coordinate is to be skewed: value + slope * # logs 
*/
       ycoord=(y_value+DEGREES_C_PER_LOG*(skew_ymax[quadrant]-x_value)-
        dep_min[quadrant][trace])*dep_scale[quadrant][trace];
      } else { 
       ycoord=
        (int)(((data[Derive_start_block+dep_ptr[quadrant][trace]]
        -dep_min[quadrant][trace]) * dep_scale[quadrant][trace]) + 0.5);
      }
     } else {
      ycoord=
       (int)(((data[Derive_start_block+dep_ptr[quadrant][trace]]
       -dep_min[quadrant][trace]) * dep_scale[quadrant][trace]) + 0.5);
     }
     break;
   }
/*
if variable is on y-axis, adjust coord to account for inverted coordinates
*/
   if (!get_indepvar_yaxis(quadrant))
    ycoord=rheight[quadrant]/
       (get_isolate_traces(quadrant)?num_of_traces[quadrant]:1)-1-ycoord;
/*  
draw a vector only after at least 2 points are collected  
*/
   if (!(first_point[quadrant]))  {
    if (get_indepvar_yaxis(quadrant)) {
     x1=last_ycoord[quadrant][trace];
     y_1=Last_xcoord[quadrant][trace];
     x2=ycoord; y2=xcoord;
    } else {
     x1=Last_xcoord[quadrant][trace];
     y_1=last_ycoord[quadrant][trace];
     x2=xcoord; y2=ycoord;
    }
    if (get_isolate_traces(quadrant))
     adjust_points(quadrant,trace,&x1,&y_1,&x2,&y2);
    if (svrimage_draw[quadrant]) {
     (*DrawLine[GetDrawType()])(x1,y_1,x2,y2);
     if (plot_type[quadrant]==TRACK_PLOT && 
         get_show_vector(quadrant,trace) &&
         vector_time(quadrant,point%get_datablock_size()) ) {
      (*SetContext[GetDrawType()])(mem_image_xid[quadrant],GXcopy,
       (get_vector_color(quadrant)==CONTRAST_TRACE?
        ((color[quadrant][trace]==BLU || color[quadrant][trace]==GRN)?ORG:BLU)
        :color[quadrant][trace]),
       0,LineSolid,0);
      draw_vector(quadrant,trace,Derive_start_block,Raw_start_block,x2,y2);
      (*SetContext[GetDrawType()])(mem_image_xid[quadrant],GXcopy,color[quadrant][trace],
      0,LineSolid,0);
     }
    } else {
     (*DrawLine[GetDrawType()])(rleft[quadrant]+x1,rtop[quadrant]+y_1,rleft[quadrant]+x2,rtop[quadrant]+y2);
     if (plot_type[quadrant]==TRACK_PLOT && 
         get_show_vector(quadrant,trace) &&
         vector_time(quadrant,point%get_datablock_size()) ) {
      (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,
       (get_vector_color(quadrant)==CONTRAST_TRACE?
        ((color[quadrant][trace]==BLU || color[quadrant][trace]==GRN)?ORG:BLU)
        :color[quadrant][trace]),
       0,LineSolid,0);
      draw_vector(quadrant,trace,Derive_start_block,Raw_start_block,
       rleft[quadrant]+x2,rtop[quadrant]+y2);
      (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,color[quadrant][trace],
      0,LineSolid,0);
     }
    }
    vector_drawn[quadrant]=TRUE;
   } else 
    first_point[quadrant] = FALSE;
   Last_xcoord[quadrant][trace]=xcoord;
   last_ycoord[quadrant][trace]=ycoord;
   }  /* end of loop on number of points  */

/* note points for flashing marker at current point
*/
   current_x[quadrant][trace]=x2;
   current_y[quadrant][trace]=y2;
  } /* end of loop on number of traces  */

  (*UnsetClipping[GetDrawType()])();
  if (svrimage_draw[quadrant]) {
   if (plot_type[quadrant]==TRACK_PLOT) {

/* this is an initial rendering on a track plot; draw in any boundaries
   and landmarks 
*/
    set_boundary_context(quadrant,TRUE);
    draw_boundaries(quadrant);
    (*UnsetClipping[GetDrawType()])();
    draw_landmarks(quadrant);
   }
   (*RasterOp[GetDrawType()])(mem_image_xid[quadrant],0,0,pw_xid[quadrant],rleft[quadrant],rtop[quadrant],rwidth[quadrant],rheight[quadrant],GXcopy);
   svrimage_draw[quadrant]=FALSE;
  }
  (*FlushDisplay[GetDrawType()])();
}

/************************  SET_ISOLATE_CLIPPING()  ****************************/

set_isolate_clipping(quadrant,trace)
int quadrant,trace;
{
 (*SetClipping[GetDrawType()])(
  (!svrimage_draw[quadrant]?rleft[quadrant]:0)+
  (get_indepvar_yaxis(quadrant)?
   trace*rwidth[quadrant]/num_of_traces[quadrant]:0),
  (!svrimage_draw[quadrant]?rtop[quadrant]:0)+
  (get_indepvar_yaxis(quadrant)?
   0:trace*rheight[quadrant]/num_of_traces[quadrant]),
  get_indepvar_yaxis(quadrant)?
   rwidth[quadrant]/num_of_traces[quadrant]:rwidth[quadrant],
  get_indepvar_yaxis(quadrant)?
   rheight[quadrant]:rheight[quadrant]/num_of_traces[quadrant]);
}

/************************  FLASH_CURRENT_POINT()  ****************************/

flash_current_point(quadrant,trace)
int quadrant,trace;

/* given current_x and current_y, toggle off and on a circle at that point
   for the quadrant and trace indicated
*/
{
static int last_x[NUM_DISPLAY_QUADRANTS][MAX_TRACES], 
           last_y[NUM_DISPLAY_QUADRANTS][MAX_TRACES];

/* don't bother with server images or special update modes
*/
 if (svrimage_draw[quadrant] || update_mode[quadrant]!=NORMAL_UPDATE)
  return;

 if (flash_on[quadrant][trace] && 
  (current_x[quadrant][trace]!=last_x[quadrant][trace] || 
   current_y[quadrant][trace]!=last_y[quadrant][trace])) {

/* then the data has moved since last seen here; hide the previous flash
   marker which was on at that point
*/
  draw_flash(quadrant,trace,last_x[quadrant][trace],last_y[quadrant][trace],
   color[quadrant][trace]);
  flash_on[quadrant][trace]=FALSE;
 }

/* toggle on or off, as per last state, the flash marker at current point
*/
 draw_flash(quadrant,trace,
  current_x[quadrant][trace],current_y[quadrant][trace],
 color[quadrant][trace]);

/* save current data points and update flash_on state variable
*/
 last_x[quadrant][trace]=current_x[quadrant][trace];
 last_y[quadrant][trace]=current_y[quadrant][trace];
 flash_on[quadrant][trace]=!flash_on[quadrant][trace];
}

/**********************  DRAW_FLASH()  **************************************/

draw_flash(quadrant,trace,xcoord,ycoord,flash_color)
int quadrant;
int xcoord,ycoord,flash_color;

/* given x and y-coords, a flash_color and a quadrant, draw the flash marker
*/
{
 (*SetContext[GetDrawType()])(pw_xid[quadrant],GXxor,flash_color,0,LineSolid,0);
 (*SetHiliteColors[GetDrawType()])(flash_color,BKGND);
 if (get_isolate_traces(quadrant))
  set_isolate_clipping(quadrant,trace);
 else
  (*SetClipping[GetDrawType()])(rleft[quadrant],rtop[quadrant],
               rwidth[quadrant],rheight[quadrant]);
 xcoord+=rleft[quadrant]; ycoord+=rtop[quadrant];
 (*DrawCircle[GetDrawType()])(xcoord,ycoord,CIRCLE_DIAMETER);
}

/************************  SET_BOUNDARY_CONTEXT()  ****************************/

set_boundary_context(quadrant,memory_image)
int quadrant,memory_image;
{
 if (memory_image) {
  (*SetContext[GetDrawType()])(mem_image_xid[quadrant],GXcopy,BLACK,1,LineSolid,0);
  (*SetClipping[GetDrawType()])(0,0,rwidth[quadrant],rheight[quadrant]);
 } else {
  (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,BLACK,1,LineSolid,0);
  (*SetOffset[GetDrawType()])(rleft[quadrant],rtop[quadrant]);
  (*SetClipping[GetDrawType()])(rleft[quadrant],rtop[quadrant],
   rwidth[quadrant],rheight[quadrant]);
 }
}

/************************  SET_LANDMARK_CONTEXT()  ****************************/

set_landmark_context(quadrant,op,fg,bg)
int quadrant,fg,bg;
{
 if (svrimage_draw[quadrant]) {
  (*SetContext[GetDrawType()])(mem_image_xid[quadrant],op,fg,BKGND,LineSolid,
   GetFontID(GetLandmarkFont(quadrant)));
  (*SetHiliteColors[GetDrawType()])(fg,bg);
  (*SetClipping[GetDrawType()])(0,0,rwidth[quadrant],rheight[quadrant]);
 } else {
  (*SetContext[GetDrawType()])(pw_xid[quadrant],op,fg,BKGND,LineSolid,
   GetFontID(GetLandmarkFont(quadrant)));
  (*SetHiliteColors[GetDrawType()])(fg,bg);
  (*SetClipping[GetDrawType()])(rleft[quadrant],rtop[quadrant],
   rwidth[quadrant],rheight[quadrant]);
 }
}

/****************************  GET_X_COORD()  *********************************/

get_x_coord(quadrant,value)
int quadrant;
float value;
{
return (int)(((value-indep_min[quadrant]) * indep_scale[quadrant]) + 0.5);
}

/****************************  GET_Y_COORD()  *********************************/

get_y_coord(quadrant,trace,value)
int quadrant,trace;
float value;
{
return rheight[quadrant]-1-
 (int)(((value-dep_min[quadrant][trace])*dep_scale[quadrant][trace])+0.5);
}

/****************************  TIME_SERIES()  *********************************/

time_series(quadrant)
int quadrant;

/* update display(s)  -- perform action as dictated by value of update_mode;
   update_mode must not be reset to NORMAL_UPDATE by anyone
   else except time_series() AND functions that set it but
   get ERROR condition returned from set_trace() or indices()
*/


{
int numpts;

 set_mode(quadrant);

 if ((numpts=more_points(quadrant))) {
/* new points since last update  */
  data_stamps(quadrant,((NextDrawStart[quadrant]+numpts-1)%get_datablock_size()));
  time_stamps(quadrant);
  if (update_mode[quadrant]==NORMAL_UPDATE) 
   shift_plots_left(quadrant,numpts*get_update_interval());
  if (Pixel_width[quadrant] || update_mode[quadrant]!=NORMAL_UPDATE) {  

/* don't draw any vectors until enough points have accumulated to warrant a 
   left shift to make room for the vectors 
*/
   draw_time_series_trace(quadrant,NextDrawStart[quadrant],numpts,0,
    num_of_traces[quadrant]);
  }
  NextDrawStart[quadrant] = ( (start_point[quadrant]+time_gone_by[quadrant])
                                 % get_datablock_size());
 } 

 if (need_rehilite[quadrant]) {
  rehilite(quadrant);
 }
 update_mode[quadrant]=NORMAL_UPDATE;
}

/**********************  DRAW_TIME_SERIES_TRACE()  ***************************/

draw_time_series_trace(quadrant,start,numpts,start_trace,end_trace)
int quadrant,start,numpts,start_trace,end_trace;
/* draw vectors from start for numpts points, for each variable from
   start_trace to end_trace. Set vector_drawn true if at least one vector
   has been drawn.  Refresh screen every UPDATE_RATE operations.
*/
{
int end_pt,point,trace;
int ycoord,xcoord;
float i;
double exact_x;
int x1,y_1,x2,y2;
static double pt_left_over[NUM_DISPLAY_QUADRANTS]={0,0,0,0};  /* accumulate
fractional parts to maintain accurate scaled placement of next x-coordinate */

static int last_ycoord[NUM_DISPLAY_QUADRANTS][MAX_TRACES];

/* for each new vector...  */

  end_pt=start + numpts;

/* for each trace in plot window... */

  for (trace=start_trace; trace<end_trace; trace++) {

   if (svrimage_draw[quadrant]) {
    (*SetContext[GetDrawType()])(mem_image_xid[quadrant],GXcopy,color[quadrant][trace],
    0,LineSolid,0);
    if (get_isolate_traces(quadrant))
     set_isolate_clipping(quadrant,trace);
    else
     (*SetClipping[GetDrawType()])(0,0,rwidth[quadrant],rheight[quadrant]);
   } else {
    (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,color[quadrant][trace],
    0,LineSolid,0);
    if (get_isolate_traces(quadrant))
     set_isolate_clipping(quadrant,trace);
    else
     (*SetClipping[GetDrawType()])(rleft[quadrant],rtop[quadrant],
      rwidth[quadrant],rheight[quadrant]);
   }

   first_point[quadrant]=(mode[quadrant]==ALL_POINTS);
   for (i=0,point=start;point<end_pt;i++,point++) {

/* deduct 1 to coincide with clipped area and preclude gaps in steep vertical
   vectors
*/
   exact_x=(((double)xaxis_seconds[quadrant] - 
             (double)(numpts-i-1)*get_update_interval())
             *indep_scale[quadrant]) + 
             pt_left_over[quadrant] - 1; 
   xcoord=(int)exact_x;
   pt_left_over[quadrant]=exact_x - xcoord;
   
/* compute y-coordinates of next endpoint; mod the point index to facilitate 
wraparound from the loop incrementing
*/
     Derive_start_block=(point%get_datablock_size())**len_derive;
     Raw_start_block=(point%get_datablock_size())**len_raw;

     switch (dep_mode[quadrant][trace]) {
     case BITS:
      ycoord=
       (int)((((float)bitdata[Raw_start_block+dep_ptr[quadrant][trace]]
       -dep_min[quadrant][trace]) * dep_scale[quadrant][trace]) + 0.5);
      break;
     case VOLTAGE:
      ycoord=
       (int)(((voltsdata[Raw_start_block+dep_ptr[quadrant][trace]]
       -dep_min[quadrant][trace]) * dep_scale[quadrant][trace])+0.5);
      break;
     case RAW:
      ycoord=
       (int)(((rawdata[Raw_start_block+dep_ptr[quadrant][trace]]
       -dep_min[quadrant][trace]) * dep_scale[quadrant][trace])+0.5);
      break;
     case DERIVED:
      ycoord=
       (int)(((data[Derive_start_block+dep_ptr[quadrant][trace]]
       -dep_min[quadrant][trace]) * dep_scale[quadrant][trace])+0.5);
     break;
    }
/*
adjust coord to account for inverted coordinates on y-axis
*/
    ycoord=rheight[quadrant]/
       (get_isolate_traces(quadrant)?num_of_traces[quadrant]:1)-1-ycoord;
/*  
draw a vector only after at least 2 points are collected and the end-x-coord
is within the plot frame 
*/
    if (!(first_point[quadrant]))  {
     x1=Last_xcoord[quadrant][trace];  x2=xcoord;
     y_1=last_ycoord[quadrant][trace]; y2=ycoord;
     if (xcoord >=0) {
      if (get_isolate_traces(quadrant))
       adjust_points(quadrant,trace,&x1,&y_1,&x2,&y2);
      if (!svrimage_draw[quadrant])
       (*DrawLine[GetDrawType()])(rleft[quadrant]+x1,rtop[quadrant]+y_1,
                 rleft[quadrant]+x2,rtop[quadrant]+y2);
      else 
       (*DrawLine[GetDrawType()])(x1,y_1,x2,y2);
      vector_drawn[quadrant]=TRUE;
     } /* if xcoord >= 0 */
    } else 
     first_point[quadrant] = FALSE;
    last_ycoord[quadrant][trace]=ycoord;
    Last_xcoord[quadrant][trace]=xcoord;
   }  /* end of loop on number of points  */
  } /* end of loop on number of traces  */

  (*UnsetClipping[GetDrawType()])();
  if (svrimage_draw[quadrant]) {
   (*RasterOp[GetDrawType()])(mem_image_xid[quadrant],0,0,pw_xid[quadrant],rleft[quadrant],rtop[quadrant],rwidth[quadrant],rheight[quadrant],GXcopy);
   svrimage_draw[quadrant]=FALSE;
  }
  pt_left_over[quadrant]=0;
}

/************************   SET_MODE()  ************************************/
 
set_mode(quadrant)
int quadrant;
{
 extern Display *win_display;
 int depth = XDefaultDepth(win_display, DefaultScreen(win_display));

 if (update_mode[quadrant]==NORMAL_UPDATE) {
  mode[quadrant]=NEW_POINTS;
 } else {   /* a redraw of entire plot surface is imminent  */
  mode[quadrant]=ALL_POINTS;
  left_over[quadrant]=0;
  switch (update_mode[quadrant]) {
   case START_NOW:
    mode[quadrant]=NEW_POINTS;
    LastDrawEnd[quadrant]=time_gone_by[quadrant];
   case INITIAL:
    draw_bkgnd(quadrant);
    break;
   case RECONFIG:
    draw_bkgnd(quadrant);
   default:
    need_rehilite[quadrant]=TRUE;
  }
/* prepare pixmap for drawing data onto
*/
  (*InitMemDraw[GetDrawType()])
   (&mem_image[quadrant],rwidth[quadrant],rheight[quadrant],depth);
  mem_image_xid[quadrant]=(Drawable)GetWindowXID(mem_image[quadrant]);
  switch (plot_type[quadrant]) {
   case TIME_SERIES:
   case XY:
   case TRACK_PLOT:
   case SOUNDING:
    (*FillArea[GetDrawType()])(mem_image_xid[quadrant],0,0,rwidth[quadrant],rheight[quadrant],0);
    break;
   case SKEW_T:
    draw_skew_bkgnd(mem_image_xid[quadrant],quadrant);
    break;
  }
  svrimage_draw[quadrant]=TRUE;
  got_start_time[quadrant]=FALSE;
 }
}

/**************************  MORE_POINTS()  *********************************/

more_points(quadrant)
int quadrant;
/* determine the start point in shared memory for the next drawing cycle
   in time_series(), as a function of mode and time_gone_by[quadrant]
*/
{
int numpts;

  switch (mode[quadrant]) {

   case ALL_POINTS:
/* 
adjust for a pending total screen redraw request 
*/
    if ((numpts = ((time_gone_by[quadrant]>get_time_frame_size())?
                  get_time_frame_size():time_gone_by[quadrant]))) {
     if (numpts<0) {
      numpts=0;
     }
     LastDrawEnd[quadrant]=time_gone_by[quadrant];
     NextDrawStart[quadrant] = (time_gone_by[quadrant]<get_time_frame_size())?
      start_point[quadrant]:
      (time_gone_by[quadrant]-get_time_frame_size())%get_datablock_size();
     if (frozen[quadrant] && ElapsedTime>get_time_frame_size()) {
/*
adjust for shmem circular queue having moved around since last time through --
frozen displays lose more front end data points each time an ALL_POINTS request
is made
*/
/***
printf("Frozen adjustment, would have been from %d for %d points, change to: ",
        NextDrawStart[quadrant],numpts);
***/
      if (ElapsedTime-time_gone_by[quadrant]>get_time_frame_size()) {
/*
time frame in shared memory excludes all of frozen time frame -- nothing should
be displayed there.  Note that the input process, in ANALYSIS mode, needs to 
maintain *elapsed_time as a pointer into the entire flight, with a separate index
into the shared memory blocks, for this test to work when users frame forward or
backward or change start times.  For now, that is not being done, and as such
frozen WINDS displays will not be changed when input rate is changed or frame
jumps made, but upon changing scales or variables, an unfaithful refresh will
be drawn!!! Note that, in winput.c, an initial attempt has been made, with 
references to ElapsedTime, to fix this, but has been unsuccessful so far...
wraparounds and misplaced frame jumps are happening and it's time to abandon 
this effort for now... (7/20/92)

PS - a different approach would be to provide functions that determine whether
a given time frame is within the stored data history, if so just how much,  
and adjust frozen windows accordingly.  This is a nice solution anyway, since
such a set of functions can be used for other things as well.  And, there's no
need to muck with the input process, which isn't broke so don't fix it.  Note
that the issue of midnite crossover needs to be addressed, and this set of
functions is a good first step in doing so.
*/
       NextDrawStart[quadrant]=numpts=0;
      } else {
       NextDrawStart[quadrant]=(ElapsedTime-get_time_frame_size())
        %get_datablock_size();
       numpts=get_time_frame_size()-(ElapsedTime-time_gone_by[quadrant]);
      }
/***
printf("%d for %d points\n",NextDrawStart[quadrant],numpts);
***/
     }
     return(numpts);
    } else

/* if no add'l points have been written to shared memory, we're done here */

     return(0);

   case NEW_POINTS: 

/* normal update -- from point after last point read */

    if ((numpts=time_gone_by[quadrant] - LastDrawEnd[quadrant])) {
     if (numpts<0) {
      numpts=0;
     }
     LastDrawEnd[quadrant]=time_gone_by[quadrant];
     if (numpts>get_time_frame_size()) 
      numpts=get_time_frame_size();
     return(numpts);
    } else
     return(0);
  }

  return(ERROR); /* this to satisfy lint */
}

/*******************  SHIFT_PLOTS_LEFT()  **********************************/

shift_plots_left(quadrant,numpts)
int quadrant,numpts;
/* determine where next set of start x-coordinates will be as function of
   the number of points to be drawn on this cycle in time_series(); if any
   vectors have been drawn yet (vector_drawn]==TRUE), block move the entire
   plot surface over to the left until it lines up with that start x-coord. 
*/
{
int block_width,
    trace;
double exact_width;


/* accumulate fractional parts to preclude shifting left not far enough  */

  exact_width = (double)numpts * indep_scale[quadrant] + left_over[quadrant]; 

/* make room for new pixels' width -- never more than width of plot frame
*/	
  Pixel_width[quadrant] = MIN((int)exact_width,rwidth[quadrant]);

  left_over[quadrant] =   MIN(exact_width,(double)rwidth[quadrant]) 
                          - Pixel_width[quadrant];

/* don't bother unless at least one pixel's worth of shift needed */

  if (Pixel_width[quadrant]) {

/* calculate width in pixels of block to shift left -- never less than zero
*/
   block_width = rwidth[quadrant]-Pixel_width[quadrant]; 

   for (trace=0; trace<MAX_TRACES; trace++)

/* calculate start for next vector draw 
*/
    Last_xcoord[quadrant][trace] = block_width - 1;   

/* if any lines have been drawn, block move the display to left */

   if (vector_drawn[quadrant]) {

    (*RasterOp[GetDrawType()])(pw_xid[quadrant],

/* source coordinates
*/
                rleft[quadrant]+Pixel_width[quadrant],rtop[quadrant],
              pw_xid[quadrant],

/* destination block coordinates
*/
                rleft[quadrant],rtop[quadrant],block_width,rheight[quadrant],GXcopy); 

/* erase the background on section duplicated to its left  */
 
    (*FillArea[GetDrawType()])(pw_xid[quadrant],rleft[quadrant]+block_width,rtop[quadrant],
      Pixel_width[quadrant],rheight[quadrant],0);
   } 
  } 
}

/****************************  GET_RAW_VALUE()  ***************************/
double
get_raw_value(variable,start_block)
char *variable;
int start_block;
{
int indx;
 
 if ( (indx=GetVarListIndex(variable,TRUE))==ERROR) {
  return ERROR;
 }
 if (get_mode(&indx)!=RAW)
  return ERROR;
 return (double)(rawdata[start_block+indx]);
}
 
/****************************  GET_DERIVED_VALUE()  ***************************/
double get_derived_value(char *variable, int start_block)
{ 
int indx; 
 
 if ( (indx=GetVarListIndex(variable,TRUE))==ERROR) {
  return ERROR;
 }
 if (get_mode(&indx)!=DERIVED)
  return ERROR;
 return (double)(data[start_block+indx]);
}

/**************************  GET_MODE_BY_NAME()  *****************************/
int get_mode_by_name(char *varname)
{
int indx;

 if ( (indx=GetVarListIndex(varname,TRUE))==ERROR) {
  return ERROR;
 }
 return get_mode(&indx);
}

/**************************  CLIP_VECTOR()  **********************************/

int clip_vector(xmin,xmax,ymin,ymax,ox1,oy1,ox2,oy2,x1,y_1,x2,y2)
int xmin,xmax,ymin,ymax;
int ox1,oy1,ox2,oy2;
int *x1,*y_1,*x2,*y2;
/* given the bounds of a clipping rectangle (xmin,xmax,ymin,ymax) and the
   coordinates describing a vector (ox1,oy1,ox2,oy2), set new coordinates
   (x1,y_1,x2,y2) to describe the endpoints of a vector that lies within the
   clipping boundaries.

   This code utilizes the Cohen-Sutherland algorithm, and is a modification
   of the code given in Foley/Van Dam's 'Fundamentals of Interactive Computer
   Graphics'

   This routine is needed because apparently the pixrect clipping does not
   work as advertised.  Plots of noisy data sometimes results in excessively
   large window coordinates, which should be clipped but cause confusion
   instead.  Use of clip_vector() remedies this deficiency.
*/
{
 int i;
 int temp;
 int p1_posn[4],p2_posn[4]; /* codes describing position of points relative
                               to clipping rectangle */

 *x1=ox1; *y_1=oy1; *x2=ox2; *y2=oy2;

 while (TRUE) {
  p1_posn[0]=(*y_1>ymax?TRUE:FALSE);
  p1_posn[1]=(*y_1<ymin?TRUE:FALSE);
  p1_posn[2]=(*x1>xmax?TRUE:FALSE);
  p1_posn[3]=(*x1<xmin?TRUE:FALSE);

  p2_posn[0]=(*y2>ymax?TRUE:FALSE);
  p2_posn[1]=(*y2<ymin?TRUE:FALSE);
  p2_posn[2]=(*x2>xmax?TRUE:FALSE);
  p2_posn[3]=(*x2<xmin?TRUE:FALSE);

/* trivial reject -- note that this step precludes divide by zero in
   slope calculations below
 */

  if ((p1_posn[0] && p2_posn[0]) ||
      (p1_posn[1] && p2_posn[1]) ||
      (p1_posn[2] && p2_posn[2]) ||
      (p1_posn[3] && p2_posn[3]))
   return (FALSE); 

/* trivial accept */

  if (!(p1_posn[0]) && !(p2_posn[0]) &&
      !(p1_posn[1]) && !(p2_posn[1]) &&
      !(p1_posn[2]) && !(p2_posn[2]) &&
      !(p1_posn[3]) && !(p2_posn[3]))
   return (TRUE); 

/* assure that start point p1 is outside of window */

   if (!(p1_posn[0] || p1_posn[1] || p1_posn[2] || p1_posn[3] )) {

/* point p1 is inside window; swap its coordinates with point p2, since
   it must lie outside of window if we got this far
 */

    temp=*x1;*x1=*x2;*x2=temp;
    temp=*y_1;*y_1=*y2;*y2=temp;
    for (i=0;i<4;i++) {
     temp=p1_posn[i];p1_posn[i]=p2_posn[i];p2_posn[i]=temp;
    }
   }

/* subdivide ==> move start point p1 to intersection point */

   if (p1_posn[0]) { /* y_1 > ymax */
    *x1=*x1 + (*x2 - *x1) * (ymax - *y_1) / (*y2 - *y_1);
    *y_1=ymax;
   }

   else if (p1_posn[1]) {  /* y_1 < ymin */
    *x1=*x1 + (*x2 - *x1) * (ymin - *y_1) / (*y2 - *y_1);
    *y_1=ymin;
   }

   else if (p1_posn[2]) { /* x1 > xmax */
    *y_1=*y_1 + (*y2 - *y_1) * (xmax - *x1) / (*x2 - *x1);
    *x1=xmax;
   }

   else if (p1_posn[3]) {  /* x1 < xmin */
    *y_1=*y_1 + (*y2 - *y_1) * (xmin - *x1) / (*x2 - *x1);
    *x1=xmin;
   }
 }
}

/**************************  REHILITE()  **************************************/
rehilite(quadrant)
int quadrant;
/* set flag and generate event so as to re-highlight current field
*/
{

 if (GetMouseWindow()==ERROR)

/* 
per-second time-out functions may set need_rehilite[] before initial
LOC_WINENTER event.  In this case, (GetMouseX(),GetMouseY()) will be (0,0) 
and GetMouseWindow() will be ERROR; highlight_area() will believe that cursor is
at upper left corner and highlight accordingly (wrongly, to wit) 
unless check is made here for value of GetMouseWindow().
*/
  return;

 highlight_area(TRUE,quadrant,GetMouseX(),GetMouseY());
 need_rehilite[quadrant]=FALSE;
}


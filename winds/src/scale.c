/* scale.c -- perform autoscaling operations.  Parameters controlling this
   operation are currently in the file ../ipc_files/plotspex, and are
   referenced as tolerance_factor, clip_chk_interval, and seconds_to_avg.

   SCCS ID: 
@(#)scale.c	1.1 11/14/90

*/

#include <xview/xview.h>
#include <stdio.h>
 
/****************************  WINDS INCLUDES  ******************************/

#include <xy_plt.h>
#include <display.h>
#include <help.h>
#include <shmem.h>
#include <ops.h>
#include <proto.h>


static int Force_autoscale[NUM_DISPLAY_QUADRANTS]=
{FALSE,FALSE,FALSE,FALSE};
static int Same_scale[NUM_DISPLAY_QUADRANTS]=
{FALSE,FALSE,FALSE,FALSE};
static int Need_same_scale[NUM_DISPLAY_QUADRANTS]=
{FALSE,FALSE,FALSE,FALSE};
static double same_scale_min[NUM_DISPLAY_QUADRANTS];
static double same_scale_max[NUM_DISPLAY_QUADRANTS];

/************************** SET_NEED_SAME_SCALE()  ***************************/

int
set_need_same_scale(quadrant,set)
int quadrant,set;
{
 Need_same_scale[quadrant]=set;
}

/************************** GET_NEED_SAME_SCALE()  ***************************/

int
get_need_same_scale(int quadrant)
{
 return Need_same_scale[quadrant];
}

/***************************** GET_SAME_SCALE()  *****************************/

int
get_same_scale(int quadrant)
{
 return Same_scale[quadrant];
}

/***************************** SET_SAME_SCALE()  *****************************/

set_same_scale(int quadrant, int set)
/*
adjust parms relating to same scale for quadrant, as per value of set
*/
{
int trace;
double tempmin,tempmax,range_min,range_max;
extern int trace_number[];

 Same_scale[quadrant]=set;

 if (num_of_traces[quadrant]==1 || !set) {
/*
either 1 trace only or user is unsetting same scale for plot with more than 1
*/
  Same_scale[quadrant]=FALSE;
  set_ScaleType(quadrant);
  set_scale_type_button(quadrant);
  if (set)
/* 
user is setting same scale with plot of only 1 trace; nothing else needed
*/
   return;
/*
user is unsetting same scale, for a plot with 1 or more traces
*/
  for (trace_number[quadrant]=0;
        trace_number[quadrant]<num_of_traces[quadrant];
        trace_number[quadrant]++) {
   set_dep_range(quadrant,CUSTOM);
  }
  (void)refresh_window(INITIAL,quadrant);
  return;
 }
/*
user is setting same scale. Start by determining the lowest and highest
values of all ranges, to be then used as the same scale limits
*/
 for (trace=0; trace<num_of_traces[quadrant]; trace++) {
  tempmin=dep_invert_scale[quadrant][dep_attr_num[quadrant][trace]]?
   dep_max[quadrant][trace]:dep_min[quadrant][trace];
  tempmax=dep_invert_scale[quadrant][dep_attr_num[quadrant][trace]]?
   dep_min[quadrant][trace]:dep_max[quadrant][trace];
  if (trace==0) {
   range_min=tempmin;
   range_max=tempmax;
  } else {
   range_min=MIN(range_min,tempmin);
   range_max=MAX(range_max,tempmax);
  }
 }
/*
set same scale drawing parms
*/
 set_scales_same(quadrant,range_min,range_max);
/*
unset isolation 
*/
 set_isolate_traces(quadrant,FALSE);
 (void)refresh_window(INITIAL,quadrant);
}

/*************************  GET_SAME_SCALE_MAX()  *************************/

double
get_same_scale_max(quadrant)
int quadrant;
{
 return same_scale_max[quadrant];
}

/*************************  GET_SAME_SCALE_MIN()  *************************/

double
get_same_scale_min(quadrant)
int quadrant;
{
 return same_scale_min[quadrant];
}

/*************************  SET_SAME_SCALE_RANGE()  *************************/

set_same_scale_range(quadrant,min,max)
int quadrant;
double min,max;
{
 same_scale_min[quadrant]=min;
 same_scale_max[quadrant]=max;
}

/***************************  SET_SCALES_SAME()  ******************************/

set_scales_same(quadrant,min,max)
int quadrant;
double min,max;
{
int trace;

 for (trace=0; trace<num_of_traces[quadrant]; trace++) {
/* 
change any inverted scales to normal, all scales to fixed
*/
/***
  if (dep_invert_scale[quadrant][dep_attr_num[quadrant][trace]])
   dep_invert_scale[quadrant][dep_attr_num[quadrant][trace]]=FALSE;
***/
  dep_rng_typ[quadrant][trace]=FIXED;
  set_fixed_dep_range(quadrant,trace,min,max,
    dep_attr_num[quadrant][trace]);
 }

 set_same_scale_range(quadrant,min,max);
}

/***************************** FORCE_AUTOSCALE()  *****************************/

force_autoscale(quadrant)
int quadrant;
{
 return Force_autoscale[quadrant];
}

/************************* SET_FORCE_AUTOSCALE()  *************************/

set_force_autoscale(quadrant,value)
int quadrant,value;
{
 if (frozen[quadrant])
/*
don't force autoscale when window is frozen
*/
  value=FALSE;
 Force_autoscale[quadrant]=value;
}

/************************* AUTO_SCALE_IF_NEEDED()  *************************/

auto_scale_if_needed(quadrant)
int quadrant;
/* determine if auto-scaling is needed for any trace currently displayed.
   Rescale indicated trace(s) if needed, redraw the background, and generate
   a LOC_MOVE event to facilitate re-highlighting if needed
*/
{
int needed;          /* is autoscaling necessary? TRUE if the average of the
                        last clip_chk_interval seconds is beyond the threshold
                        defined by clip_tolerance (defined in function 
                        set_float_range()). If so, define a new midpoint for
                        the rescaled plot by averaging seconds_to_avg seconds.
                     */

int trace;           /* index counter  */
double get_avg(),avg,new_avg,acceptable_midpt();

  needed=FALSE;

/* check for dependent variables */

  for (trace=0; trace<num_of_traces[quadrant]; trace++) {
   if (dep_rng_typ[quadrant][trace]==FLOATING) {
    avg=get_avg(quadrant,clip_chk_interval[quadrant],
     dep_ptr[quadrant][trace],dep_mode[quadrant][trace]);
    if ( force_autoscale(quadrant) ||
/*
a complete redraw with rescaling of axes is requested due to some other 
operation
*/
         avg > MAX(dep_min[quadrant][trace],dep_max[quadrant][trace])
          -clip_tolerance[quadrant][trace] ||
/*
running average exceeds the designated clip tolerance threshold
*/
         avg < MIN(dep_min[quadrant][trace],dep_max[quadrant][trace])
          +clip_tolerance[quadrant][trace] 
/*
running average falls below the designated clip tolerance threshold
*/
    ) {
/* call set_float_range with USER_ATTR to preserve a possibly user specified
   floating range; if called with AUTO_ADJUST, the interval will be 
   optimized and hence reset that range 
*/
     new_avg=get_avg(quadrant,seconds_to_avg[quadrant],
      dep_ptr[quadrant][trace],dep_mode[quadrant][trace]);
     set_float_dep_range(quadrant,trace,new_avg,
      dep_intrvl[quadrant][trace]*(NUM_TIX-1)/2,
      USER_ATTR,dep_attr_num[quadrant][trace]);
     needed=TRUE;
    } 
   }
  }

/* repeat for independent variable */

  if (plot_type[quadrant]!=TIME_SERIES && indep_rng_typ[quadrant]==FLOATING) {
   avg=get_avg(quadrant,clip_chk_interval[quadrant],
      indep_ptr[quadrant],indep_mode[quadrant]);
   if ( force_autoscale(quadrant) ||
        avg > MAX(indep_min[quadrant],indep_max[quadrant])
         -indep_clip_tolerance[quadrant] ||
        avg < MIN(indep_min[quadrant],indep_max[quadrant])
         +indep_clip_tolerance[quadrant] 
   ) {
    new_avg=get_avg(quadrant,seconds_to_avg[quadrant],
     indep_ptr[quadrant],indep_mode[quadrant]);
    set_float_indep_range(quadrant,new_avg,
      indep_intrvl[quadrant]*(NUM_TIX-1)/2,
      USER_ATTR,
      indep_attr_num[quadrant]);
    needed=TRUE;
   } 
  }

  if (needed) {
   update_mode[quadrant]=CHG_FLOATING_SCALE;
   draw_bkgnd(quadrant);
  }
  return(needed);
}

/*************************  GET_AVERAGE()  ********************************/

double get_avg(quadrant,seconds,ptr,mode_)
/* get the average for trace indicated over the last seconds seconds
*/
int quadrant,seconds;
int ptr;                          /* pointer into data block */
int mode_;
{
double total;                     /* total of all data points  */
int total_sex,                    /* total seconds to use in averaging  */ 
    end_time,                     /* last data second to use in totalling */
    data_point;                    /* index counter  */

 total_sex=total=0;

/* wait, on real-time startup, for at least the first 2 data points to appear
*/
 if (!post_processing_mode())
  while (*elapsed_time-1<1) ;;
/*
frozen windows may have less than seconds seconds of history to look at
*/
 if (frozen[quadrant] && ElapsedTime>get_time_frame_size())
  seconds=MIN(seconds,time_gone_by[quadrant]-(ElapsedTime%get_datablock_size()));
/*
some frozen windows may have nothing at all to look at
*/
 if (frozen[quadrant] && 
     ElapsedTime-time_gone_by[quadrant]>get_time_frame_size())
  return 0;
/* 
use time_gone_by instead of *elapsed_time in case window is frozen
*/
 end_time=start_point[quadrant]+time_gone_by[quadrant];
/* 
start at first second if seconds seconds have not elapsed yet 
*/
 for (data_point=(end_time<seconds?0:end_time-seconds+1);  
      data_point<=end_time; data_point++) {
/* 
don't allow this loop to outrace the input program on startup
*/
  if (data_point<=*elapsed_time-1) {
   switch (mode_) {
   case VOLTAGE: /* VOLTAGE is always fixed, for now anyway; as such,
                 its average will not be needed */
    return(0);
   case BITS:
    total+=bitdata[(data_point%get_datablock_size())**len_raw+ptr];
    break;
   case RAW:
    total+=rawdata[(data_point%get_datablock_size())**len_raw+ptr];
    break;
   case DERIVED:
    total+=data[(data_point%get_datablock_size())**len_derive+ptr];
    break;
   }
   total_sex++;
  }
 }
 return(total/(double)total_sex);
}


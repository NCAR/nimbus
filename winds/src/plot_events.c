/* events.c -- handle events in the canvas. See commentary under 
   canvas_event_proc() for details.
*/

#include <xview/xview.h>
#include <xview/panel.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include <stdio.h>

/****************************  WINDS INCLUDES  ******************************/

#include <xy_plt.h>
#include <display.h>
#include <lengths.h>
#include <ops.h>
#include <timer.h>
#include <help.h>
#include <constants.h>
#include <errors.h>
#include <header.h>
#include <macros.h>
#include <op_panels.h>
#include <proto.h>

/*********************  PROCESS_CLIX()  ************************************/

process_clix(op)
int op;
/* take action initiated by a potentially multiple click event
*/
{
int type;

 if (window_type[current_quadrant]==DISPTYPE_PMS1D) {
  pmszoom(current_quadrant,num_clix[current_quadrant],op);
 } else if (op==UNZOOM_VECTOR || op==ZOOM_VECTOR) {
  chg_vector_scale(current_quadrant,op,num_clix[current_quadrant]);
  update_mode[current_quadrant]=CHG_VECTOR_SCALE;
 } else if (op==ZOOM || op==UNZOOM) {
  type=trace_number[current_quadrant]==INDEPVAR?indep_rng_typ[current_quadrant]:
  dep_rng_typ[current_quadrant][trace_number[current_quadrant]];
  update_mode[current_quadrant]=(type==FIXED?CHG_FIXED_SCALE:
  (plot_type[current_quadrant]!=TIME_SERIES?CHG_FLOATING_SCALE:CHG_TIME_SCALE));
  chg_scale(current_quadrant,op);
 }
 need_rehilite[current_quadrant]=TRUE;  
}

/**********************  CHG_SCALE()  **********************************/

chg_scale(quadrant,op)
int quadrant,op;
{
int save_trace;

  if (plot_type[quadrant]==SKEW_T)  { 
/* 
no range changes allowed 
*/
   update_mode[quadrant]=NORMAL_UPDATE;
   return;
  }

 switch (op) {
  case ZOOM:
  case UNZOOM:
   zoom_scale(op);
   if (plot_type[quadrant]==TRACK_PLOT) {
/*
retain value of trace just zoomed, so we can reset trace_number and operate
on others, and then restore trace_number as original
*/
    save_trace=trace_number[quadrant];
    if (save_trace==INDEPVAR)  { 
/* 
maintain aspect ratio with trace 0 only
*/
     trace_number[quadrant]=0;
     zoom_scale(op);
    } else if (save_trace==0) {  
/* 
maintain aspect ratio with indep't var only when zooming on trace 0
*/
     trace_number[quadrant]=INDEPVAR;
     zoom_scale(op);
    }
   trace_number[quadrant]=save_trace;
   }
/* 
draw new labels 
*/
   draw_bkgnd(quadrant);
   return;
  default:
   report(quadrant,"WARNING: def. selector for chg_scale() is unimplemented");
   return;
 }
}

/************************  CHG_TRACE()  **********************************/

ChgTrace(event)
Event *event;
{
 ShowVarMenu(current_quadrant,event);
}

/*****************************************************************************/

DeleteTrace()
{
 SetSelectionMade(TRUE);
 update_mode[current_quadrant]=DEL_TRACE;
 unset_trace(current_quadrant);
}

/*************************************************************************/

InstallVariable(selection)
int selection;
{
 if (GetCalibDisplayUp()) {
  InstallCalibSelection(selection);
 } else if (GetChgVectorXComp()) {
  update_mode[current_quadrant]=CHG_VECTOR_COMP;
  ChgVectorXComponent(selection);
  SetChgVectorXComp(FALSE);
  SetHilitedVarName("\0");
 } else if (GetChgVectorYComp()) {
  update_mode[current_quadrant]=CHG_VECTOR_COMP;
  ChgVectorYComponent(selection);
  SetChgVectorYComp(FALSE);
  SetHilitedVarName("\0");
 } else {
  switch (plot_type[current_quadrant]) {
  case LISTS:
   if (set_listee(current_quadrant,selection) != ERROR)
    if (list_printing[current_quadrant])
     print_header(current_quadrant);
   need_rehilite[current_quadrant]=TRUE;
   break;
  case FIXED_FORMAT:
   (void)set_fixee(current_quadrant,selection);
   break;
  default:
   if (update_mode[current_quadrant]==ADD_TRACE)
    num_of_traces[current_quadrant]++;
   if (set_trace(current_quadrant,selection)==ERROR) {
    if (update_mode[current_quadrant]==ADD_TRACE)
     num_of_traces[current_quadrant]--;
    update_mode[current_quadrant]=NORMAL_UPDATE;
   }
   break;
  }
 }
}

/************************  ADD_TRACE()  **********************************/

add_trace(event)
Event *event;
{

 if (num_of_traces[current_quadrant] == MAX_TRACES) {
  update_mode[current_quadrant]=NORMAL_UPDATE;
  return;
 }
 ShowVarMenu(current_quadrant,event);
 return;
}

/**********************  ZOOM_SCALE()  *************************************/ 

zoom_scale(op)
int op;
/*
 expand/contract trace image by narrowing/widening range boundaries
*/
{
double get_avg(),acceptable_interval(),acceptable_midpt(),
       acceptable_time_interval();
/************************  ASSUMPTION  *************************************/
/* the value 1.55 is tied closely to the values installed in the mults[]
   array (file axes.c); it interacts with acceptable_interval(x) in such
   a way as to insure the adjusted interval is different than the original. */
static double chg_factor = 1.55; /* amt to increase/decrease interval */
/**********************  END ASSUMPTION  ************************************/
static int num_divs=(NUM_TIX-1)/2;
int clix;
double intrvl,midpt,bot,top,plus_or_minus;
int trace;

trace=trace_number[current_quadrant];
clix=num_clix[current_quadrant];

while (clix > 0) {  /* change range as many times as user clicked button */

 if (update_mode[current_quadrant]==CHG_FIXED_SCALE) {
  if(trace!=INDEPVAR) {
   top=dep_max[current_quadrant][trace];
   bot=dep_min[current_quadrant][trace];
/* dep_intrvl[current_quadrant][] will be negative if scale is inverted */
   midpt=top - num_divs * dep_intrvl[current_quadrant][trace];
/* make the new labels 'nice'  */
   intrvl=dep_intrvl[current_quadrant][trace]*(op==UNZOOM?chg_factor:1.0/chg_factor);
   intrvl= acceptable_interval(intrvl,dep_mode[current_quadrant][trace]);
/* compute and set new boundaries */
   top=midpt + intrvl * num_divs;
   bot=midpt - intrvl * num_divs;
   set_fixed_dep_range(current_quadrant,trace,bot,top,dep_attr_num[current_quadrant][trace]);
   if (get_same_scale(current_quadrant))
    set_scales_same(current_quadrant,bot,top);
  } else {
   top=indep_max[current_quadrant];
   bot=indep_min[current_quadrant];
   midpt=top - num_divs * indep_intrvl[current_quadrant];
   intrvl=acceptable_interval(indep_intrvl[current_quadrant]
               * (op==UNZOOM? chg_factor: 1.0/chg_factor),DERIVED );
   top=midpt + intrvl * num_divs;
   bot=midpt - intrvl * num_divs;
   set_fixed_indep_range(current_quadrant,bot,top,indep_attr_num[current_quadrant]);
  }
 } else {  /* FLOATING range type  */
/* compute new floating range */
  if(trace!=INDEPVAR) {
   plus_or_minus = dep_intrvl[current_quadrant][trace] * num_divs
               * (op==UNZOOM? chg_factor: 1.0/chg_factor);
/* use AUTO_ADJUST to make new labels 'nice' */
   set_float_dep_range(current_quadrant,trace,get_avg(current_quadrant,seconds_to_avg[current_quadrant],dep_ptr[current_quadrant][trace],dep_mode[current_quadrant][trace]),
    plus_or_minus,AUTO_ADJUST,dep_attr_num[current_quadrant][trace]);
  } else {
   if (plot_type [current_quadrant]!= TIME_SERIES) {
    plus_or_minus = indep_intrvl[current_quadrant] * num_divs
               * (op==UNZOOM? chg_factor: 1.0/chg_factor);
    set_float_indep_range(current_quadrant,get_avg(current_quadrant,seconds_to_avg[current_quadrant],indep_ptr[current_quadrant],indep_mode[current_quadrant]),
     plus_or_minus,AUTO_ADJUST,indep_attr_num[current_quadrant]);
   } else {
    set_time_range(current_quadrant,(TLABELS-1) * (int)acceptable_interval(
          (((double)xaxis_seconds[current_quadrant]/(double)(TLABELS-1))*
           (op==UNZOOM?chg_factor:1.0/chg_factor)),TIME_AXIS));
   }
  }
 }  
 clix--;
} 
}

/***************************  TOGGLE_AUTOSCALE()  ***************************/

toggle_autoscale(quadrant)
int quadrant;
{

 switch (plot_type[quadrant]) {
  case SKEW_T:
  case LISTS:
  case FIXED_FORMAT: 
   return;
  default:
   autoscale[quadrant]=autoscale[quadrant]?FALSE:TRUE;
 }
}

/***************************  TOGGLE_MODE()  *********************************/

toggle_mode(quadrant)
int quadrant;
/* toggle data display input between calibrated and volts or bits, for
   analog and digital, respectively
*/
{
int toggled; /* whether or not a variable was actually toggled */

 toggled=FALSE;
 if (plot_type[quadrant]==FIXED_FORMAT || plot_type[quadrant]==SKEW_T)
  return;
 if (plot_type[quadrant]!=LISTS) {  /* plots */  

  if (trace_number[quadrant]!=INDEPVAR && get_same_scale(quadrant)) 
   set_same_scale(quadrant,FALSE);

/* event in proper area? 
*/
  if ( ( (event_in_chg_y_scale_area(GetMouseX(),GetMouseY()))  || 

/* MUST evaluate event_in_chg_trace_area() last to get proper value on 
   trace_number here !!!
*/
       (event_in_chg_trace_area(GetMouseY())) ) ) { 

/* don't allow on independent axis just yet 
*/
   if (GetVarIsIndep(quadrant))  /* don't allow on independent axis just yet */
    return;
   (void)mode_toggle(quadrant,trace_number[quadrant]);
  } else  

/* if event occurs anywhere else but over highlighted trace, toggle all 
*/
   for (trace_number[quadrant]=0; 
        trace_number[quadrant]<num_of_traces[quadrant]; 
        trace_number[quadrant]++)
    (void)mode_toggle(quadrant,trace_number[quadrant]);

/* generate an event to re-highlight the previously highlighted trace 
*/

 } else {   /* same as above for lists */
  if (event_in_chg_listee_area(GetMouseX())) {
   if (!add_listee) { /* event occurred over occupied slot in list window */
    toggled=mode_toggle(quadrant,listee_number[quadrant]);
/* 
notify print process of change if this process is printing 
*/
    if (list_printing[quadrant] && toggled)
     print_header(quadrant);
   } else { /* event occurred over open slot in list window -- change ALL
    listees on screen AND on printer */
    for (listee_number[quadrant]=0; 
        listee_number[quadrant]<num_of_listees[quadrant]; 
        listee_number[quadrant]++)
     toggled=mode_toggle(quadrant,listee_number[quadrant]);
    if (list_printing[quadrant] && toggled) { /*easiest way to change 
  				entire printer list is to stop and restart */
     toggle_print_lists(quadrant);
     toggle_print_lists(quadrant);
    }
   }
  } else {
   for (listee_number[quadrant]=0; 
        listee_number[quadrant]<num_of_listees[quadrant]; 
        listee_number[quadrant]++)
    if (mode_toggle(quadrant,listee_number[quadrant]))
     toggled=TRUE;
   if (list_printing[quadrant] && toggled) {
    toggle_print_lists(quadrant);
    toggle_print_lists(quadrant);
   }
  }
 }
}

/***************************  MODE_TOGGLE()  *********************************/

mode_toggle(quadrant,number)
int quadrant,number;
{
int indx;

  if (plot_type[quadrant]==FIXED_FORMAT || plot_type[quadrant]==SKEW_T)
   return(FALSE);
  if (plot_type[quadrant]!=LISTS) {
   if ((indx=GetVarListIndex(trace_name[quadrant][number],TRUE))==ERROR) {
    set_notice_needed(VARIABLE_NOT_AVAILABLE);
    report(quadrant,"variable not available");
    return(FALSE); 
   }

/* position in either raw_list or derived_list 
*/
   if (get_mode(&indx)!=RAW)  /* not in raw_list: do naught */
    return(FALSE);
   update_mode[quadrant]=CHG_TRACE;

/* get_mode returns either RAW or DERIVED; dep_mode keeps track of whether
   RAW is in BITS, VOLTAGE or CALIBRATED mode. Analog variables will cycle 
   between BITS, VOLTAGE and RAW(CALIBRATED) modes; digital will toggle between
   BITS and RAW.
*/
   if (is_digital[indx]) {
    switch (dep_mode[quadrant][number]) {
     case BITS:
      (void)set_trace(quadrant,RAW | dep_ptr[quadrant][number] );
      break;
     case VOLTAGE:
      break;
     case RAW:
      (void)set_trace(quadrant,BITS | dep_ptr[quadrant][number] );
      break;
    }
   } else {
    switch (dep_mode[quadrant][number]) { 
     case BITS:
      (void)set_trace(quadrant,VOLTAGE | dep_ptr[quadrant][number] );
      break;
     case VOLTAGE: 
      (void)set_trace(quadrant,RAW | dep_ptr[quadrant][number] );
      break;
     case RAW:
      (void)set_trace(quadrant,BITS | dep_ptr[quadrant][number] );
      break; 
    }
   }
 } else {   /* same as above for lists */
   if ((indx=GetVarListIndex((char *)GetListeeName(quadrant,number),TRUE))
    ==ERROR) {
    set_notice_needed(VARIABLE_NOT_AVAILABLE);
    report(quadrant,"variable not available");
    return(FALSE); 
   }
   if (get_mode(&indx)!=RAW)
    return(FALSE);
   update_mode[quadrant]=CHG_LISTEE;
   if (is_digital[indx]) {
    switch (dep_mode[quadrant][number]) {
     case BITS:
      (void)set_listee(quadrant,RAW | dep_ptr[quadrant][number] );
      break;
     case VOLTAGE: 
      break;
     case RAW:
      (void)set_listee(quadrant,BITS | dep_ptr[quadrant][number] );
      break; 
    } 
   } else { 
    switch (dep_mode[quadrant][number]) {  
     case BITS:    
      (void)set_listee(quadrant,VOLTAGE | dep_ptr[quadrant][number] );
      break; 
     case VOLTAGE:  
      (void)set_listee(quadrant,RAW | dep_ptr[quadrant][number] );
      break; 
     case RAW: 
      (void)set_listee(quadrant,BITS | dep_ptr[quadrant][number] );
      break;  
    } 
   }
 }
 return(TRUE);
}

/*************************  RESET_PLOT_ATTR()  ********************************/

reset_plot_attr(quadrant)
int quadrant;
/* reset the range and minimum to the default parameters as contained in
   the default_range[] and default_mins[] buffers, as well as the range type
   and invert attributes as implied there.  Unset same-scale option.
*/
{
int type;
int reset_all;
 
 if (plot_type[quadrant]==FIXED_FORMAT || plot_type[quadrant]==SKEW_T)
  return;
 if (plot_type[quadrant]!=LISTS) {  /* plots */

  reset_all=FALSE;
  if (trace_number[quadrant]!=INDEPVAR) {

/* unset same-scale and inverted scales
*/
   if (get_same_scale(quadrant)) {
    set_same_scale(quadrant,FALSE);
    reset_all=TRUE;
   } else {
    if (trace_number[quadrant]!=ERROR)
     dep_invert_scale[quadrant][dep_attr_num[quadrant][trace_number[quadrant]]]=
     FALSE;
   }
  }

  if (!reset_all&&(event_in_chg_y_scale_area(GetMouseX(),GetMouseY()) ||

/*  MUST evaluate event_in_chg_trace-area() last to get proper value on 
    trace_number here !!!
*/
      event_in_chg_trace_area(GetMouseY()))) {  /* event in proper area */
   if (GetVarIsIndep(quadrant)) {
    type=indep_rng_typ[quadrant];
    update_mode[quadrant]=(type==FIXED?CHG_FIXED_SCALE:CHG_FLOATING_SCALE);
    set_indep_range(quadrant,DEFAULT);
   } else {
    if (trace_number[quadrant]==INDEPVAR) {
     type=indep_rng_typ[quadrant];
     update_mode[quadrant]=(type==FIXED?CHG_FIXED_SCALE:CHG_FLOATING_SCALE);
     set_indep_range(quadrant,DEFAULT);
    } else {
     type=dep_rng_typ[quadrant][trace_number[quadrant]];
     update_mode[quadrant]=(type==FIXED?CHG_FIXED_SCALE:CHG_FLOATING_SCALE);
     set_dep_range(quadrant,DEFAULT);
    }
   }
  } else if (!reset_all&&event_in_chg_time_scale_area(GetMouseY())){
   type=indep_rng_typ[quadrant];
   update_mode[quadrant]=CHG_TIME_SCALE;
   set_time_range(quadrant,DEFAULT_XAXIS_SECONDS);
  } else if (!reset_all&&event_in_chg_x_scale_area(GetMouseY())) {
   type=get_indepvar_yaxis(quadrant)?
    dep_rng_typ[quadrant][trace_number[quadrant]]:indep_rng_typ[quadrant];
   update_mode[quadrant]=(type==FIXED?CHG_FIXED_SCALE:
      CHG_FLOATING_SCALE);
   if (get_indepvar_yaxis(quadrant))
    set_dep_range(quadrant,DEFAULT);
   else
    set_indep_range(quadrant,DEFAULT);
  } else {   /* either reset_all is set or event not in either area; 
                reset ALL */ 
   for (trace_number[quadrant]=0; 
        trace_number[quadrant]<num_of_traces[quadrant]; 
        trace_number[quadrant]++) {
    dep_invert_scale[quadrant][dep_attr_num[quadrant][trace_number[quadrant]]]=
    FALSE;
    type=dep_rng_typ[quadrant][trace_number[quadrant]];
    update_mode[quadrant]=(type==FIXED?CHG_FIXED_SCALE:CHG_FLOATING_SCALE);
    set_dep_range(quadrant,DEFAULT);
   }
   trace_number[quadrant]=INDEPVAR;
   type=indep_rng_typ[quadrant];
   update_mode[quadrant]=(type==FIXED?CHG_FIXED_SCALE:
    (plot_type[quadrant]!=TIME_SERIES?CHG_FLOATING_SCALE:CHG_TIME_SCALE));
   if (plot_type[quadrant]==TIME_SERIES)
    set_time_range(quadrant,DEFAULT_XAXIS_SECONDS);
   else
    set_indep_range(quadrant,DEFAULT);
  }
 } else {   /* lists */
  return;
 }

/* draw new labels */ 
 draw_bkgnd(quadrant);

/* generate an event to re-highlight the previously highlighted trace */

}

/***************************  SET_HEX()  **************************************/

set_hex(hex_on)
int hex_on;
{
 hex=hex_on;
}
 
/**************************  TOGGLE_FIX_FLOAT()  *****************************/

toggle_fix_float(quadrant)
int quadrant;
{
int old_type;
int trace;

 if (plot_type[quadrant]==SKEW_T)
  return;

 if (trace_number[quadrant]!=INDEPVAR && get_same_scale(quadrant)) 
  set_same_scale(quadrant,FALSE);

 trace=trace_number[quadrant];
 if (event_in_chg_trace_area(GetMouseY())) {
  old_type=GetVarIsIndep(quadrant)?indep_rng_typ[quadrant]:
   dep_rng_typ[quadrant][trace];
  (void)toggle_rngtyp(quadrant,trace,old_type);
 } else if (event_in_chg_time_scale_area(GetMouseY())) {
  return;
 } else if (event_in_chg_x_scale_area(GetMouseY())) {
  old_type=get_indepvar_yaxis(quadrant)?
   dep_rng_typ[quadrant][trace]:indep_rng_typ[quadrant];
  (void)toggle_rngtyp(quadrant,trace,old_type);
 } else if (event_in_chg_y_scale_area(GetMouseX(),GetMouseY())) {
  old_type=get_indepvar_yaxis(quadrant)?indep_rng_typ[quadrant]:
                                       dep_rng_typ[quadrant][trace];
  (void)toggle_rngtyp(quadrant,trace,old_type);
 } else {
/* 
nothing highlighted: toggle range type for all traces, including 
independent var (if any)
*/
   for (trace=0; trace<num_of_traces[quadrant]; trace++) {
    old_type=dep_rng_typ[quadrant][trace];
    (void)toggle_rngtyp(quadrant,trace,old_type);
   }
   if (plot_type[quadrant]==XY 
    || plot_type[quadrant]==SOUNDING 
    || plot_type[quadrant]==TRACK_PLOT 
    ) {
    old_type=indep_rng_typ[quadrant];
    (void)toggle_rngtyp(quadrant,INDEPVAR,old_type);
   }
 }
}

/***********************  TOGGLE_RNGTYP()  **********************************/

toggle_rngtyp(quadrant,trace,old_type)
int quadrant,old_type,trace;
{
int independent_var,new_type;

/* there are 2 different state variables used to see if the independent var is 
   being designated.  One is in event_in_chg_trace_area(), which sets 
   indepvar[] TRUE if it is, the other in event_in_chg_x_scale_area(), which
   sets trace_number to INDEPVAR if it is.  Eventually, these state variables
   should be consolidated into one.  For now, combine them in a kluge-like
   manner:
*/


 independent_var=GetVarIsIndep(quadrant) || 
                 trace==INDEPVAR;

 if (old_type==FIXED) {
/* change fixed to floating 
*/
  new_type=FLOATING;
  (void)sprintf(&custom_range[quadrant][
   (independent_var?indep_attr_num[quadrant]:dep_attr_num[quadrant][trace])*VALUELEN],
    "%lf", -1.0 * (independent_var?ABS(indep_max[quadrant]-indep_min[quadrant]):
                   ABS(dep_max[quadrant][trace]-dep_min[quadrant][trace])));
 } else {
/* change floating to fixed 
*/
  new_type=FIXED;
  (void)sprintf(&custom_range[quadrant][
   (independent_var?indep_attr_num[quadrant]:dep_attr_num[quadrant][trace])*VALUELEN],
   "%lf",independent_var?ABS(indep_max[quadrant]-indep_min[quadrant]):
                        ABS(dep_max[quadrant][trace]-dep_min[quadrant][trace]));
  (void)sprintf(&custom_mins[quadrant][
   (independent_var?indep_attr_num[quadrant]:dep_attr_num[quadrant][trace])*VALUELEN],
    "%lf",independent_var?indep_min[quadrant]:dep_min[quadrant][trace]);
 }

 trace_number[quadrant]=trace;
 if (independent_var) {
  set_indep_range(quadrant,indep_mode[quadrant]==BITS ||
			   indep_mode[quadrant]==VOLTAGE?DEFAULT:CUSTOM);
 } else {
  set_dep_range(quadrant,dep_mode[quadrant][trace]==BITS ||
			 dep_mode[quadrant][trace]==VOLTAGE?DEFAULT:CUSTOM);
 }

 if (new_type==FLOATING) {
/* adjust around current value
*/
  draw_bkgnd(quadrant);
  update_mode[quadrant]=CHG_FLOATING_SCALE;
 }

/* generate an event to re-highlight the previously highlighted trace */

 need_rehilite[quadrant]=TRUE;
}

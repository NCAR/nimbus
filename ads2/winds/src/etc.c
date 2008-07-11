/*  etc.c -- utility functions 

 SCCS ID:
@(#)etc.c	1.1 11/14/90

*/
#include <stdio.h>
#include <ctype.h>
#include <X11/Xlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/types.h>
#include <xview/xview.h>
#include <xview/font.h>
#include <xview/canvas.h>

/*************************  LINT STUFF  **************************************/

Notify_value bye_bye();

/***************************  WINDS INCLUDES  ********************************/

#include <xy_plt.h>
#include <display.h>
#include <constants.h>
#include <ctape.h>
#include <help.h>
#include <files.h>
#include <lengths.h>
#include <macros.h>
#include <errors.h>
#include <ops.h>
#include <header.h>
#include <network.h>
#include <proto.h>


/*****************************  EXPORTABLE   *********************************/

/* pointer into data values for dependent vars 
*/
int dep_ptr[NUM_QUADRANTS][MAX_DISPLAY_VARS]; 

/* Graphics stuff */

int repaint_set[NUM_QUADRANTS]=
{FALSE,FALSE,FALSE,FALSE,FALSE};

int indep_ptr[NUM_QUADRANTS];           /* pointer into data values for independent var */
int color[NUM_QUADRANTS][MAX_TRACES];  /* color assigned to each trace  */
int first_point[NUM_QUADRANTS] =
{TRUE,TRUE,TRUE,TRUE,TRUE};             /* flag for first data point  */
int dep_mode[NUM_QUADRANTS][MAX_DISPLAY_VARS];/* BITS, VOLTAGE, RAW, DERIVED  */
 
/* character string of var name
*/
char trace_name[NUM_QUADRANTS][MAX_TRACES][NAMLEN];
int indep_mode[NUM_QUADRANTS];          /* VOLTAGE, RAW, or DERIVED  */
 
static int MaxNameLength=0;	/* the length of the longest name in either of
				   raw/derived lists */

/****************************************************************************/
 
char *
GetTraceName(quadrant,which)
int quadrant,which;
{
 if (which==INDEPVAR)
  return indepvar_name[which];
 else
  return trace_name[quadrant][which];
}

/****************************************************************************/
 
GetDepVarMode(quadrant,which)
int quadrant,which;
{
 return dep_mode[quadrant][which];
}

/*******************  CanvasRepaintProc()  ***********************************/
 
/*ARGSUSED0*/
void
CanvasRepaintProc(cvas,pwindow,area)
Canvas cvas;
Xv_Window pwindow;
Rectlist *area;
/* catch the REPAINT signal (window needs repainting).  Set flags to refresh
   window, update help images, and set keyboard focus as needed. 
*/
{
Rect *cursor_posn;
struct rectnode *rectangle;
int quadrant;
static int first_call[NUM_DISPLAY_QUADRANTS]=
{TRUE,TRUE,TRUE,TRUE};   /* on startup, CanvasRepaintProc() is called twice
				for each window. Don't set repaint_set[]
				until the second time to preclude setting
				need_rehilite and update_mode incorrectly 
				in refresh_window()
			 */
 win_get_damage(GetBaseFrame());
 
 if (pwindow==paint_window[UPPER_LEFT]) {
  quadrant=UPPER_LEFT;
  set_quad_size(quadrant);
 } else
 if (pwindow==paint_window[UPPER_RIGHT]) {
  quadrant=UPPER_RIGHT;
  set_quad_size(quadrant);
 } else
 if (pwindow==paint_window[LOWER_RIGHT]) {
  quadrant=LOWER_RIGHT;
  set_quad_size(quadrant);
 } else
 if (pwindow==paint_window[LOWER_LEFT]) {
  quadrant=LOWER_LEFT;
  set_quad_size(quadrant);
 }
 if (!first_call[quadrant])
  (void)refresh_window(INITIAL,quadrant);
 else
  first_call[quadrant]=FALSE;
/* 
determine whether cursor is over the exposed section
*/
 cursor_posn=(Rect *)xv_get(cvas,WIN_MOUSE_XY);
/* cover the scenario where one window exec()'s over itself, with cursor
still residing over the new window, to update the help image accordingly.
*/
 rectangle=area->rl_head;
 while (rectangle) {
  if ( within_boundaries(cursor_posn->r_left,cursor_posn->r_top,
        rectangle->rn_rect.r_left,rectangle->rn_rect.r_top,
        rectangle->rn_rect.r_left+rectangle->rn_rect.r_width-1,
        rectangle->rn_rect.r_top+rectangle->rn_rect.r_height-1)) {
   need_rehilite[quadrant]=TRUE;

/* fix it so while loop will terminate
*/
   rectangle=area->rl_tail;
  }
  rectangle=rectangle->rn_next;
 }
 win_get_damage(GetBaseFrame());
}
 
/**************************  REFRESH_WINDOW()  ******************************/

int
refresh_window(mode,quadrant)
int mode,quadrant;
/* set flags as needed to effect a refresh of window
*/
{

/***
 if (frozen[quadrant])
  return FALSE;
***/

 if (mode==START_NOW) {
  switch (window_type[quadrant]) {
   case WTYPE_CALIBDISP:
   case DISPTYPE_PMS1D:
   case DISPTYPE_LISTS:
   case DISPTYPE_FIXEDFORMAT:
   case DISPTYPE_PMS2D:
     return FALSE;
   case DISPTYPE_XYPLOT:
   case WTYPE_SKEWT:
    start_point[quadrant]=ElapsedTime;
    break;
  }
 } else if (mode==INITIAL ) {
  init_display_parms(quadrant);
/*
pms2d windows need graphics init upon each geom chnage
*/
  if (window_type[quadrant]==DISPTYPE_PMS2D)
   init_2d_graphics(quadrant);
 }

/***
 if (!frozen[quadrant]) 
***/
  update_mode[quadrant]=mode;
 if (repaint_set[quadrant]) {
  need_rehilite[quadrant]=TRUE;
 } 

 chg_tb_help_image(last_help_image());
 repaint_set[quadrant]=TRUE;
 return(TRUE);
}

/*********************  GetMaxNameLength()  *****************************/

GetMaxNameLength()
{ 
 return MaxNameLength;
} 
 
/*********************  SetMaxNameLength()  *****************************/

SetMaxNameLength()
{ 
int index;

 for (index=0; index<GetNumMenuEntries(); index++) {
  MaxNameLength=MAX(MaxNameLength,strlen((char *)GetMenuListName(index)));
 }
} 
 
/************************  INDICES()  ************************************/

/*ARGSUSED*/
int indices(quadrant,varname)
int quadrant;
char *varname;  /* variable name requested  */
 
/* given user input character string VAR, first insure that it is completely
   upper case to facilitate a match; then attempt to establish its data block
   position via set_trace
 
   indices is called from two places: on initial startup of each window, as
   per specifications in the PLOTSPEX file, and whenever
   a user elects to change the variable being displayed in the given window.
*/
 
{
 
int indx;

 if (plot_type[quadrant]==LISTS)
  return(set_listee(quadrant,GetVarListIndex(varname,TRUE)));
 else if (plot_type[quadrant]==FIXED_FORMAT)
  return(set_fixee(quadrant,GetVarListIndex(varname,TRUE)));
 else if (plot_type[quadrant]==CALIBRATION && 
          trace_number[quadrant] != INDEPVAR) {
/* 
want dependent variable to be in voltages mode...but GetVarListIndex will
return mode==RAW; so get that mask, strip mode via get_mode(), and mask
it with VOLTAGE for correct result
*/
  if ((indx=GetVarListIndex(varname,TRUE))==ERROR) {
   return(ERROR);
  }
  (void)get_mode(&indx);
  return(set_trace(quadrant,VOLTAGE | indx ));
 } else
  return(set_trace(quadrant,GetVarListIndex(varname,TRUE)));
}

/************************  GET_MODE()  ***************************************/

get_mode(indx)
int *indx;
/* GetVarListIndex() sets the most sig. byte of index to mask indicating the mode
 -- i.e., either VOLTAGE, RAW, or DERIVED -- in its return value.  get_mode()
 strips that mode from indx and returns it.
*/
 
{
 int temp;

 temp=*indx;
 *indx &= STRIP_INDEX;
 return (temp & STRIP_MODE);

}

/**********************  SET_TRACE()  ****************************************/

set_trace(quadrant,index)
int quadrant,index;
/* If there is data for the variable in the index'th position in ptr_names, 
   get it's position in the plot specifications file, set dep_ptr to establish
   this variable as a valid trace, draw the background to prepare for data
   points on next interrupt of display() routine

   calling routine is responsible for setting value of trace_number before
   invoking set_trace
*/
{
int temp;  /* temporary result from get_plot_attr ... might be ERROR  */
int type;  /* DEFAULT or CUSTOM */
int mode_;  /* VOLTAGE, RAW, or DERIVED */
static char varname[NAMLEN];
 
  /* if GetVarListIndex returns an ERROR, done  */

   if (index==ERROR) return(ERROR);

  /* strip leading bits from index to determine mode */ 
 
   switch (mode_=get_mode(&index)) {
  /* check for valid index value  */
    case BITS:
    case VOLTAGE:
    case RAW:
     if ((index < 0) || (index >= *len_raw) ) {
      report(quadrant,"invalid index value");
      return (ERROR);
     }
     (void)sprintf(varname,"%s",&raw_list[index*NAMLEN]);
     break;
    case DERIVED:
     if ((index < 0) || (index >= *len_derive) ) {
      report(quadrant,"invalid index value");
      return (ERROR);
     }
     (void)sprintf(varname,"%s",&derive_list[index*NAMLEN]);
     break;
   }
/* 
variable name found AND there is data for it; set type and get plot attributes  
*/
  type=(mode_==BITS || mode_==VOLTAGE ||
    plot_type[quadrant]==CALIBRATION?DEFAULT:CUSTOM);

  if ( (temp=get_plot_attr(quadrant,mode_,varname))==ERROR)
   return(ERROR);

/* plot specification found ... open a window for this plot if necessary, 
   and redo the background 
*/

  if (trace_number[quadrant] != INDEPVAR)
   set_dep_attr_num(quadrant,trace_number[quadrant],temp);
  else
   set_indep_attr_num(quadrant,temp);

  if (trace_number[quadrant] != INDEPVAR) {
   dep_ptr[quadrant][trace_number[quadrant]]=index;

   color[quadrant][trace_number[quadrant]]=allocate_color(quadrant);
   dep_mode[quadrant][trace_number[quadrant]]=mode_;
   (void)sprintf(trace_name[quadrant][trace_number[quadrant]],"%s",varname);
   set_dep_range(quadrant,type);
  } else {
   indep_mode[quadrant]=mode_;
   indep_ptr[quadrant]=index;
   (void)sprintf(indepvar_name[quadrant],"%s",varname);
   set_indep_range(quadrant,type);
  }

  if (update_mode[quadrant] != INITIAL) /* don't draw and redraw background 
			for each trace on initial entry  */
   draw_bkgnd(quadrant);       
  first_point[quadrant]=TRUE;    /* new plot window won't draw vector until
                           2nd data point -- this must be reset even if
                           dep_ptr is same as before, since next update
                           of display assumes a change has occurred */

  return(TRUE);
}

/**********************  SET_LISTEE()  ****************************************/
set_listee(quadrant,indx)
int quadrant,indx;
{
static char varname[NAMLEN];
int temp;  /* temporary result from get_plot_attr ... might be ERROR  */
int mode_;

  /* if GetVarListIndex returns an ERROR, done  */

   if (indx==ERROR) return(ERROR);

  /* strip leading bits from indx to determine mode */

   switch (mode_=get_mode(&indx)) { 
  /* check for valid index value  */
    case BITS:
    case VOLTAGE:
    case RAW:
     if ((indx < 0) || (indx >= *len_raw) ) {
      report(quadrant,"invalid indx value");
      return (ERROR);
     } 
     (void)sprintf(varname,"%s",&raw_list[indx*NAMLEN]);
     break;
    case DERIVED:
     if ((indx < 0) || (indx >= *len_derive) ) {
      report(quadrant,"invalid indx value");
      return (ERROR);
     }
     (void)sprintf(varname,"%s",&derive_list[indx*NAMLEN]);
     break;    
   } 

  /* variable name found AND there is data for it  */
 
  if ( (temp=get_plot_attr(quadrant,mode_,varname))==ERROR)
   return(ERROR);

/* plot specification found ... open a window for this plot if necessary,
   and redo the background
*/

  if (listee_number[quadrant] >= num_of_listees[quadrant])  {

/* new listee in 1st available column 
*/    
   listee_number[quadrant]=num_of_listees[quadrant]++;
  } else
   if (update_mode[quadrant]!=INITIAL) {
    clear_column(quadrant,listee_number[quadrant]);
   }

  dep_mode[quadrant][listee_number[quadrant]]=mode_;
  set_dep_attr_num(quadrant,listee_number[quadrant],temp);
  (void)sprintf((char *)GetListeeName(quadrant,listee_number[quadrant]),
   "%s",varname);

  dep_ptr[quadrant][listee_number[quadrant]]=indx;
  return(TRUE);
}

/**********************  SET_FIXEE()  ****************************************/
set_fixee(quadrant,indx)
int quadrant,indx;
{
static char varname[NAMLEN];
int temp;  /* temporary result from get_plot_attr ... might be ERROR  */
int mode_;

  /* if GetVarListIndex returns an ERROR, done  */

   if (indx==ERROR) return(ERROR);

  /* strip leading bits from indx to determine mode */

   switch (mode_=get_mode(&indx)) { 
  /* check for valid indx value  */
    case BITS:
    case VOLTAGE:
    case RAW:
     if ((indx < 0) || (indx >= *len_raw) ) {
      report(quadrant,"invalid indx value");
      return (ERROR);
     } 
     (void)sprintf(varname,"%s",&raw_list[indx*NAMLEN]);
     break;
    case DERIVED:
     if ((indx < 0) || (indx >= *len_derive) ) {
      report(quadrant,"invalid indx value");
      return (ERROR);
     }
     (void)sprintf(varname,"%s",&derive_list[indx*NAMLEN]);
     break;    
   } 

  /* variable name found AND there is data for it  */
 
  if ( (temp=get_plot_attr(quadrant,mode_,varname))==ERROR)
   return(ERROR);
 
/* plot specification found ... open a window for this plot if necessary,
   and redo the background
*/

  if (fixee_number[quadrant] >= num_of_fixees[quadrant]) 

/* new fixee in 1st available column 
*/  
   fixee_number[quadrant]=num_of_fixees[quadrant]++;

  dep_mode[quadrant][fixee_number[quadrant]]=mode_;
  set_dep_attr_num(quadrant,fixee_number[quadrant],temp);
  (void)sprintf((char *)GetFixeeName(quadrant,fixee_number[quadrant]),
   "%s",varname);
  dep_ptr[quadrant][fixee_number[quadrant]]=indx;
  return(TRUE);
}

/**********************  UNSET_TRACE()  *************************************/

unset_trace(quadrant)
int quadrant;

/* remove a trace from the trace management state variables; redraw the
   background accordingly, and set first-point to inform display() that
   a complete redraw is needed
*/
{
int k;
 
  for (k=trace_number[quadrant]; k<num_of_traces[quadrant]-1; k++) {
   set_dep_attr_num(quadrant,k,get_dep_attr_num(quadrant,k+1));
   dep_ptr[quadrant][k]=dep_ptr[quadrant][k+1];
   dep_min[quadrant][k]=dep_min[quadrant][k+1];
   dep_max[quadrant][k]=dep_max[quadrant][k+1];
   clip_tolerance[quadrant][k]=clip_tolerance[quadrant][k+1];
   dep_intrvl[quadrant][k]=dep_intrvl[quadrant][k+1];
   dep_rng_typ[quadrant][k]=dep_rng_typ[quadrant][k+1];
   color[quadrant][k]=color[quadrant][k+1];
   dep_mode[quadrant][k]=dep_mode[quadrant][k+1];
   set_show_vector(quadrant,get_show_vector(quadrant,k+1),k,TRUE);
   (void)sprintf(trace_name[quadrant][k],"%s",trace_name[quadrant][k+1]);
  }

  color[quadrant][--num_of_traces[quadrant]]=ERROR;
  draw_bkgnd(quadrant);
  first_point[quadrant]=TRUE;    /* new plot window won't draw vector until
                           2nd data point -- this must be reset even if
                           dep_ptr is same as before, since next update
                           of display assumes a change has occurred */

  return;
}

/*****************************************************************************/

GetTraceColor(quadrant,trace)
int quadrant,trace;
{
 return color[quadrant][trace];
}

/********************  ALLOCATE_COLOR()  *************************************/

allocate_color(quadrant)
int quadrant;
{
 int k,allocated,j;

 if (update_mode[quadrant] == CHG_TRACE) {
  return(color[quadrant][trace_number[quadrant]]);
 } else if (update_mode[quadrant] == ADD_TRACE
         || update_mode[quadrant]==INITIAL
         || update_mode[quadrant]==RECONFIG) {
  int c;
  for (c=0,k=(getpid()%MAX_TRACES); 
       c<=MAX_TRACES; 
       c++,k=(++k)%MAX_TRACES) {    
/* 
check for each valid color  -- use process pid to rotate colors used, thus
varying main colors printed to PaintJet and saving cartridge life.
*/
   allocated=FALSE;
   for (j=0; j<num_of_traces[quadrant]; j++) {   /* for each trace ... */
    if (color[quadrant][j]==k+1)
     allocated=TRUE;
   }
   if (!(allocated)) {
    return(k+1);
   }
  }
  (void)sprintf(errmsg,"WARNING: allocate_color finished loop w/o allocation");
  report(quadrant,errmsg);
  return(ERROR);
 } else if (update_mode[quadrant] == NORMAL_UPDATE) {
   return(1);
 } else {
   (void)sprintf(errmsg,"WARNING: update_mode in allocate_color unexpected");
   report(quadrant,errmsg);
   return(ERROR);
 }
}

/**************************  SET_CURSOR_TRACKING()  ***************************/

set_cursor_tracking(quadrant)
int quadrant;
{
  (void)xv_set(paint_window[quadrant],
         WIN_CONSUME_EVENT, LOC_MOVE,
        NULL);
}
/************************  UNSET_CURSOR_TRACKING()  ***************************/

unset_cursor_tracking(quadrant)
int quadrant;
{
  (void)xv_set(paint_window[quadrant],
         WIN_IGNORE_EVENT, LOC_MOVE,
        NULL);
}

/****************************  REPORT()  *************************************/

report(quadrant,string)
int quadrant;
char *string;
/* write string to error log file 
*/
{
 time_to_log();
 if (cursor_in_ctl_panel || cursor_in_control_panel)
  (void)fprintf(stderr,"WINDS CONTROL PANEL: ");
 else {
  switch (quadrant) {
   case UPPER_LEFT:
    (void)fprintf(stderr,"DISPLAY QUADRANT 0");
    break;
   case UPPER_RIGHT:
    (void)fprintf(stderr,"DISPLAY QUADRANT 1");
    break;
   case LOWER_RIGHT:
    (void)fprintf(stderr,"DISPLAY QUADRANT 2");
    break;
   case LOWER_LEFT:
    (void)fprintf(stderr,"DISPLAY QUADRANT 3");
    break;
   default:
    (void)fprintf(stderr,"%s\n",string);
    return;
  }
  switch (window_type[quadrant]) {
   case DISPTYPE_XYPLOT: 
    (void)fprintf(stderr," (XY PLOT): ");
    break;
   case WTYPE_SKEWT: 
    (void)fprintf(stderr," (SKEW_T): ");
    break;
   case DISPTYPE_LISTS: 
    (void)fprintf(stderr," (LISTS): ");
    break;
   case DISPTYPE_FIXEDFORMAT: 
    (void)fprintf(stderr," (FIXED_FORMAT): ");
    break;
   case WTYPE_CALIBDISP: 
    (void)fprintf(stderr," (CALIBRATION): ");
    break;
   case DISPTYPE_PMS1D:
    (void)fprintf(stderr," (PMS1D): ");
    break;
   case DISPTYPE_PMS2D:
    (void)fprintf(stderr," (PMS2D): ");
    break;
   case DISPTYPE_ETC:
    (void)fprintf(stderr," (ETC): ");
    break;
   default:
    (void)fprintf(stderr,"(window_type=%d): ",window_type[quadrant]);
    break;
  }
 }
 (void)fprintf(stderr,"%s\n",string);
}

/****************************  TIME_TO_LOG()  *****************************/
 
time_to_log()
{
 if (current_time && strlen(current_time))
  (void)fprintf(stderr,"%s ",current_time);
}

/************************  INIT_HDR_TYPES()  ****************************/

init_hdr_types()
{
extern int *is_digital;
int var;
char *type;

 if (is_digital)
  free((char *)is_digital);
 is_digital=(int *)malloc( (unsigned int)(sizeof(int) * *len_raw));
 for (var=0; var<*len_raw; var++) {
  int retval;
  if (GetType(&raw_list[var*NAMLEN],&type)==ERR) {
   (void)fprintf(stderr,
    "WARNING: problem with GetType() on %d'th variable, %s, in header!\n",
     var,&raw_list[var*NAMLEN]);
  }
  if ( (retval=is_digital_type(type)) )
   is_digital[var]=TRUE;
  else
   is_digital[var]=FALSE;
  if (retval==ERROR) {
   (void)fprintf(stderr,
    "FATAL: couldn't find %d'th variable, %s, in header!\n",
     var,&raw_list[var*NAMLEN]);
    RequestAlertNow("FATAL: problem finding variable in header...contact WINDS expert with this information!");
    winds_cleanup(ERROR);
  }
 }
}

/**********************  WaitOnVariableLists()  ********************************/

WaitOnVariableLists()
{
 if (!post_processing_mode() && !WaitOnRawList(10)) {
  (void)fprintf(stderr,"FATAL: waited for 10 seconds...checking winput...\n");
  if (!ConfirmWinputRunning(FALSE)) {
   RequestAlertNow("FATAL: Raw list could not be completed in a reasonable time, and the input process is not responding; it may not be running. Check your console for messages and contact a WINDS expert about this message. This WINDS process will now exit.");
  } else {
   RequestAlertNow("FATAL: Raw list could not be completed in a reasonable time, though the input process IS responding; there may be some mismatch in project setup files, or winput could be in a loop, etc. Check your console for messages and contact a WINDS expert about this message. This WINDS process will now exit.");
  }
  winds_cleanup(ERROR);
 }

 if (!WaitOnDerivedList(10)) {
  (void)fprintf(stderr,"FATAL: waited for 10 seconds...checking winput...\n");
  if (!ConfirmWinputRunning(FALSE)) {
   RequestAlertNow("FATAL: Derived list could not be completed in a reasonable time, and the input process is not responding; it may not be running. Check your console for messages and contact a WINDS expert about this message. This WINDS process will now exit.");
  } else {
   RequestAlertNow("FATAL: Derived list could not be completed in a reasonable time, though the input process IS responding; there may be some mismatch in project setup files, or winput could be in a loop, etc. Check your console for messages and contact a WINDS expert about this message. This WINDS process will now exit.");
  }
  winds_cleanup(ERROR);
 }
}

/**********************  WaitOnRawList()  ***********************************/

WaitOnRawList(waittime)  
int waittime;
/* 
wait a maximum of waittime seconds for raw_list array to be 
"completely" initialized - i.e., all the way up to last element as 
defined by *len_raw. Return FALSE if initialization not done within
prescribed wait, else return TRUE.
*/
{
int waited;

 waited=0; 
 while (!*len_raw || !strlen(&raw_list[(*len_raw-1)*NAMLEN])) {
  (void)fprintf(stderr,"WINDS: waiting for raw_list array...\n");
  (void)sleep(1);
/*
you would think to check for input running here, but that won't always
work since input might be there but too busy to respond
*/
  if (++waited==waittime) {
   return FALSE;
  }
 }
 return TRUE;
}

/****************  WaitOnDerivedList()  ***********************************/

WaitOnDerivedList(waittime)
int waittime;
/*
wait a maximum of waittime seconds for the derive_list array to be
"completely" initialized - i.e., all the way up to last element as 
defined by *len_derive. Return FALSE if initialization not done within
prescribed wait, else return TRUE.
*/
{
int waited, i;
 waited=0;
 while (!*len_derive || !strlen(&derive_list[(*len_derive-1)*NAMLEN])) {
  (void)fprintf(stderr,"waiting for derive_list array...\n");
  (void)sleep(1);
/*
you would think to check for input running here, but that won't always
work since input might be there but too busy to respond
*/
  if (++waited==waittime) {
   return FALSE;
  }
 }
 return TRUE;
}

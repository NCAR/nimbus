/* event_ops.c -- operations started as result of an event
*/

#include <stdio.h>
#include <xview/xview.h>
#include <xview/canvas.h>


/****************************  WINDS INCLUDES  ******************************/

#include <display.h>
#include <ops.h>
#include <help.h>
#include <constants.h>
#include <xy_plt.h>
#include <timer.h>
#include <printers.h>
#include <socket.h>
#include <lengths.h>
#include <colors.h>
#include <vectors.h>
#include <op_panels.h>
#include <network.h>
#include <proto.h>

#define non_click_window(q) (q==HELP_IMAGE || q==TIME_AND_PROJECT ||\
                           q==CTL_PANEL || q==STATS || q==PLOTS || q==DIALOGUE)
Notify_value count_clix();

/*************************  EXPORTABLE  *************************************/

/* parameters describing statistics printouts
 */
struct statistics stats= {DEFAULT_STATS_INTERVAL,FALSE};

/* window status parameters
 */
int hex= TRUE;                  /* print in hex or octal */
int frozen[NUM_QUADRANTS]=
	{FALSE,FALSE,FALSE,FALSE,FALSE}; /* freeze frame boolean   */

int list_printing[NUM_QUADRANTS]=
	{FALSE,FALSE,FALSE,FALSE,FALSE}; /* TRUE when lists are to be printed */

/* multiple clicking parameters:
 */
int num_clix[NUM_QUADRANTS]=
	{0,0,0,0,0};		/* number of multiple clicks detected */

/* highlighting state parameters:
 */
int pending_op[NUM_WINDOWS]=
	{NONE,NONE,NONE,NONE,
	 NONE,NONE,NONE,NONE,
	 NONE,NONE,NONE,NONE,
	 NONE};           	/* pending operation, as per area */

int highlighted_trace[NUM_WINDOWS]=
	{NONE,NONE,NONE,NONE,
	 NONE,NONE,NONE,NONE,
	 NONE,NONE,NONE,NONE,
	 NONE};    	/* trace that either is already highlighted 
              			    or is to be highlighted next */
int need_rehilite[NUM_WINDOWS]=  /* TRUE after operation that redraws a
    				      highlighted label or could result in
				      one needing to be highlighted */
	{FALSE,FALSE,FALSE,FALSE,
	 FALSE,FALSE,FALSE,FALSE,
	 FALSE,FALSE,FALSE,FALSE,
	 FALSE};

/**************************  GLOBAL IN FILE  *********************************/

/* multiple clicking parameters:
*/
static int active_button[NUM_QUADRANTS]=
{NONE,NONE,NONE,NONE,NONE};   	/* button that started click counting */
static struct itimerval clix_interval= { /* allow time for multiple clicks  */
 CLIX_INTERVAL_SEX, CLIX_INTERVAL_USEX, CLIX_INTERVAL_SEX, CLIX_INTERVAL_USEX
};

/* highlighting state parameters:
*/
static int current_area[NUM_WINDOWS]=
{NO_AREA,NO_AREA,NO_AREA,NO_AREA,
 NO_AREA,NO_AREA,NO_AREA,NO_AREA,
 NO_AREA,NO_AREA,NO_AREA,NO_AREA,
 NO_AREA};
/* area to which pointer has moved */
static int last_trace_hilited[NUM_WINDOWS]=
{NONE,NONE,NONE,NONE,
 NONE,NONE,NONE,NONE,
 NONE,NONE,NONE,NONE,
 NONE}; 			/* trace currently highlighted */
static int last_op[NUM_WINDOWS]=
{NONE,NONE,NONE,NONE,
 NONE,NONE,NONE,NONE,
 NONE,NONE,NONE,NONE,
 NONE};              	/* last pending operation  */
static int last_area[NUM_WINDOWS]=
{NONE,NONE,NONE,NONE,
 NONE,NONE,NONE,NONE,
 NONE,NONE,NONE,NONE,
 NONE};         		/* area in which pointer last seen */

/* track ball help images and pointer operation label:

...the order of initializations here MUST correspond to order of AREAS
defined in ../include/help.h, "track ball help images" section

Also be sure there is an if test for the given window type in the LOC_WINENTER
switch in events.c to facilitate correct help imaging. This may involve adding
a #define for the window type in ../include/display.h

See events.c for how to get on-line help going for a new window.
*/
 
char *labels[NUM_AREAS] = {     /* icon label strings */
 "POINTER OP'N: None",
 "POINTER OP'N: Highlited Trace Op'ns",
 "POINTER OP'N: Highlighted Trace Op'ns",
 "POINTER OP'N: Add Trace / Assigned Op'n ",
 "POINTER OP'N: Highlighted Y-scale Op'ns",
 "POINTER OP'N: Highlighted X-scale Op'ns",
 "POINTER OP'N: Assigned Op'n",
 "POINTER OP'N: None",
 "POINTER OP'N: Configure ADS Parameters",
 "POINTER OP'N: None",
 "POINTER OP'N: Delete Setpoint / Calibration Setup Menu",
 "POINTER OP'N: Calibration Setup Menu/Input Item Selection",
 "POINTER OP'N: Calibration Setup Menu",
 "POINTER OP'N: Chg Highlighted Variable / Toggle Mode",
 "POINTER OP'N: General Op'ns",
 "POINTER OP'N: Chg Highlighted Variable",
 "POINTER OP'N: Select/Dismiss from Menu",
 "POINTER OP'N: Dismiss Notice",
 "POINTER OP'N: Vector Scale / Assigned Op'n",
 "POINTER OP'N: Choose Setup Operation",
 "POINTER OP'N: Local Window Op'ns",
 "POINTER OP'N: Time Scale Op'ns",
 "POINTER OP'N: Select Menu Item or Op",
 "POINTER OP'N: Select Menu Item or Op",
 "POINTER OP'N: Select Menu Item or Op",
 "POINTER OP'N: Select Menu Item or Op",
 "POINTER OP'N: Select Menu Item or Op",
 "POINTER OP'N: Select Menu Item or Op",
 "POINTER OP'N: Zoom/Unzoom Accum'n Period",
 "POINTER OP'N: Zoom/Unzoom Y-Maximum",
 "POINTER OP'N: Zoom/Unzoom Y-Minimum",
 "POINTER OP'N: Assigned Op'n/Landmark Op'ns"
};

static char HilitedVarName[NAMLEN]="";
static int ZoomOp;


/*****************************************************************************/

SetNeedRehilite(int quadrant,int need)
{
 need_rehilite[quadrant]=need;
}

/*****************************************************************************/

BaseIsHex()
{
 return hex;
}

/************************  INIT_HILITE_PARMS()  ******************************/

init_hilite_parms(int quadrant)
{
 pending_op[quadrant]= NONE;           	
 highlighted_trace[quadrant]= NONE;    	
 SetHilitedVarName("\0");
 current_area[quadrant]= NO_AREA;      
 last_trace_hilited[quadrant]= NONE; 			
 last_op[quadrant]= NONE;              	
 last_area[quadrant]=NO_AREA;
 need_rehilite[quadrant]=FALSE;
}

/************************  INIT_STATUS_PARMS()  ******************************/

init_status_parms(int quadrant)
{
fprintf(stderr, "In init_status_parms = %d\n", quadrant);
  frozen[quadrant]=FALSE;
  list_printing[quadrant]=FALSE;
  if (GetQuadPrinting()==quadrant) 
    SetQuadPrinting(NONE);
  active_button[quadrant]= NONE;   	
}


/*****************************  FREEZE()  ************************************/

freeze(int quadrant)
{
fprintf(stderr, "In FREEZE = %d\n", quadrant);
 frozen[quadrant] = (frozen[quadrant]) ? FALSE : TRUE;
}

/**********************   TOGGLE_PRINT_LISTS()  *******************************/ 
toggle_print_lists(int quadrant)
/*
   LISTS process has lowest priority. It can only gain control of the printer
   when noone else has it.
 
   If there are more than one list process, only one can have printer at given
   time; if F3 is entered for a second list process, it stops printing for the 
   first one and starts printing for itself.
*/
{

 if (GetQuadPrinting()!=NONE) {

/* someone is already printing
*/
   if (GetQuadPrinting()==quadrant) {

/* toggle printing off
*/
    list_printing[quadrant]=FALSE;
    SetQuadPrinting(NONE);

   } else {

/* toggle printing off for current lists, on for requested lists
*/
    list_printing[GetQuadPrinting()]=FALSE;
    set_genop_hiliting(TRUE,GetQuadPrinting(),
     (get_genop_value(GetQuadPrinting())&(~OP_PRINT)));
    SetQuadPrinting(quadrant);
    SetPrintMode(INITIAL);
    list_printing[quadrant]=TRUE;
    print_header(quadrant);
   }
  } else {

/* no one is printing
*/
   list_printing[quadrant]=TRUE;
   SetQuadPrinting(quadrant);
   SetPrintMode(INITIAL);
   print_header(quadrant);
  }

}

/***************************  SET_CTL_FRAME_LABEL()  **************************/
set_ctl_frame_label(int area)
{
  char frame_label[FRAME_LABEL_LNGTH*3];
  char InputModeMsg[FRAME_LABEL_LNGTH*2];

  switch (GetInputMode()) {
    case ADS_DATA:
      (void)strcpy(InputModeMsg,"MODE: Real Time (control)");
      break;
    case RAWFILE_NETCDF:
      (void)strcpy(InputModeMsg,"MODE: Sim'd R/T-NetCDF");
      break;
    case DISK_DATA:
      (void)strcpy(InputModeMsg,"MODE: Sim'd Real Time");
      break;
    case NETCDF:
      (void)strcpy(InputModeMsg,"MODE: netCDF");
      break;
    case INIT_SETUP:
      (void)strcpy(InputModeMsg,"MODE: Initial Setup");
      break;
    case WINDS_SLAVE:
      (void)strcpy(InputModeMsg,"MODE: Real Time (slave)");
      break;
    default:
      (void)strcpy(InputModeMsg,"MODE: UNKNOWN!");
      break;
    }

  (void)sprintf(frame_label,"%s Display: Proj. %s (%s) on %s (from %s) %s  %s",
	(char *)GetWINDSVersion(),
	(char *)get_proj_number(),(char *)GetWINDSProjectName(),
	(char *)GetDisplayHost(),
	(char *)GetComputeHost(),labels[area],InputModeMsg);

  (void)xv_set(GetBaseFrame(),XV_LABEL,frame_label,NULL);

}

/********************  TOGGLE_STATS_PRINTING()  ******************************/

toggle_stats_printing()
{
   SetStatsAutoPrintEnabled(!GetStatsAutoPrintEnabled());
   time_to_log();
   (void)fprintf(stderr,"toggle stats printing %s\n",
    GetStatsAutoPrintEnabled()?"ON":"OFF");
   set_ctl_frame_label(CTL_AREA);

/* force flush of accumulations whether stats is printing or not
*/
   FlushStatsAccumulations();
}

/************************  HIGHLIGHT_AREA()  **********************************/

highlight_area(force_help_update,quadrant,x,y)
int force_help_update,quadrant,x,y;
/* highlight current area in quadrant as function of (x,y) coordinates.

   set force_help_update to TRUE when track-ball images are desired updated
   regardless of previous image, e.g. on LOC_WINENTER events; use FALSE when
   updates desired only when pending image is different from last one, e.g.
   on LOC_MOVE events.
*/
{

/* Ignore if quadrant specified isn't the current_quadrant.
*/
  if (quadrant!=current_quadrant) 
   return;
/* 
...else, pointer has moved into or within window; set state variables to help
determine if a change of highlighting is needed
*/
   set_current_area(quadrant,x,y); 
 
   if ((last_trace_hilited[quadrant] != highlighted_trace[quadrant]) || 
       (pending_op[quadrant] != last_op[quadrant]) ||
       (need_rehilite[quadrant])
      ) {
/* since the last time the pointer was examined for position, either it has
   moved to a different trace, or is over the same trace but its other
   labelled operation (chg_y_scale/ChgTrace) field, or an operation not
   involving menus has been executed.
*/
 
/* terminate any text input operations underway if this block not executing
   solely from the need_rehilite[] value
*/
    if ((last_trace_hilited[quadrant] != highlighted_trace[quadrant]) ||
       (pending_op[quadrant] != last_op[quadrant])) {
     stop_text_op(quadrant);
     restore_kbd_focus();
    }

/*   Highlight the field under which lies the pointer:
*/
    if (hilite_debug)
     printf("quadrant %d: ",quadrant);
    switch (pending_op[quadrant]) {
     case DELETE_LANDMARK:
      (void)update_hilited_landmark(quadrant,x,y);
      break;
     case CHG_VECTOR_SCALE:
      if (hilite_debug)
       printf("hilite CHG_VECTOR_SCALE\n");
      highlight_vector_scale_area(quadrant);
      break;
     case CHG_X_SCALE:
     case CHG_TIME_SCALE:
      if (hilite_debug)
       printf("hilite CHG_X_SCALE\n");
      highlight_range_label(quadrant);
      break;
     case CHG_Y_SCALE:
      if (hilite_debug)
       printf("hilite CHG_Y_SCALE\n");
      highlight_range_label(quadrant);
      break;
     case CHG_TRACE:
      if (hilite_debug)
       printf("hilite CHG_TRACE\n");
      highlight_trace_label(quadrant,CHG_TRACE);
      break;                         
     case CHG_LISTEE:
      if (hilite_debug)
       printf("hilite CHG_LISTEE\n");
      highlight_listee_label(quadrant);
      break;
     case CHG_FIXEE:
      if (hilite_debug)
       printf("hilite CHG_FIXEE\n");
      highlight_fixee_label(quadrant);
      break;
     case DEL_SETPT:
      if (hilite_debug)
       printf("hilite DEL_SETPT\n");
      highlight_setpt_label(highlighted_trace[quadrant]);
      break;
     case CHG_INDEPVAR:
      if (hilite_debug)
       printf("hilite CHG_INDEPVAR\n");
      highlight_trace_label(quadrant,CHG_INDEPVAR);
      break;
     case ADD_TRACE:
      if  (num_of_traces[quadrant] < MAX_TRACES) {
       int save_mode;  /* you must preserve current value of update_mode  */
       save_mode=update_mode[quadrant];
       update_mode[quadrant]=ADD_TRACE;   /* now allocate_color() knows what to do,
                                   and pending label will be correct color  */
       if (num_of_traces[quadrant]) {/* don't bother if this is a new setup */
        if (hilite_debug)
         printf("hilite ADD_TRACE\n");
        highlight_trace_label(quadrant,ADD_TRACE);
       }
       update_mode[quadrant]=save_mode;
      }
      break;
     case PMS1D_MAXY:
      label_1d_ymax(quadrant,BKGND,BLU);
      break;
     case PMS1D_MINY:
      label_1d_ymin(quadrant,BKGND,BLU);
      break;
     case PMS1D_PERIOD:
      list_sum(quadrant,BKGND,BLU);
      break;
    } 
   
    if (!(need_rehilite[quadrant]) ||
         (last_trace_hilited[quadrant] != highlighted_trace[quadrant])) {
/* Clear the last highlighted field */
     if (hilite_debug)
      printf("quadrant %d: ",quadrant);
     switch (last_area[quadrant]) {
      case PMS1D_YMAX_AREA:
       label_1d_ymax(quadrant,FOREGROUND,BKGND);
       break;
      case PMS1D_YMIN_AREA:
       label_1d_ymin(quadrant,FOREGROUND,BKGND);
       break;
      case PMS1D_ACCUM_AREA:
       list_sum(quadrant,FOREGROUND,BKGND);
       break;
      case CHG_VECTOR_SCALE_AREA:
       if (hilite_debug)
        printf("clear CHG_VECTOR_SCALE_AREA\n");
       redraw_vector_scale_area(quadrant);
       break;
      case CHG_INDEP_AREA:
       if (last_trace_hilited[quadrant] != highlighted_trace[quadrant])  {
/* if not the same row, clear  */
        if (hilite_debug)
         printf("clear CHG_INDEP_AREA\n");
        clear_trace_label(quadrant,REFRESH,last_trace_hilited[quadrant]);
       }
       break;
      case CHG_TRACE_AREA:

/* don't clear any labels if trace was just deleted; firstly, there is no need,
secondly, it may clear the one that was just hilited as result of re-hiliting
when frame is redrawn
*/
       if (update_mode[quadrant]!=DEL_TRACE && last_trace_hilited[quadrant] != highlighted_trace[quadrant])  {
/* if not the same row, clear  */
        if (hilite_debug)
         printf("clear CHG_TRACE_AREA\n");
        clear_trace_label(quadrant,REFRESH,last_trace_hilited[quadrant]);
       }
       break;
      case CHG_LISTEE_AREA:
       if (last_trace_hilited[quadrant] != highlighted_trace[quadrant])  {
/* if not the same row, clear  */
        if (hilite_debug)
         printf("clear CHG_LISTEE_AREA\n");
        clear_listee_label(quadrant,REFRESH,last_trace_hilited[quadrant]);
       }
       break;
      case CHG_FIXEE_AREA:
       if (last_trace_hilited[quadrant] != highlighted_trace[quadrant])  { 
/* if not the same row, clear  */
        if (hilite_debug)
         printf("clear CHG_FIXEE_AREA\n");
        clear_fixee_label(quadrant,REFRESH,last_trace_hilited[quadrant]);
       }
       break;
      case CALIB_SETPT_AREA:
       if (last_trace_hilited[quadrant] != highlighted_trace[quadrant])  { /* if not the same row, clear  */
        if (hilite_debug)
         printf("clear CALIB_SETPT_AREA\n");
        clear_setpt_label(last_trace_hilited[quadrant]);
       }
       break;
      case ADD_TRACE_AREA:

       if (num_of_traces[quadrant] < MAX_TRACES) {

        if (hilite_debug)
         printf("clear ADD_TRACE_AREA; pending_op==%d\n",pending_op[quadrant]);

/* if moved from add_trace to chg_trace_area, erase; else, a new trace has
   been added: refresh slot in previous highlighted area 
*/
        clear_trace_label(quadrant,pending_op[quadrant]==ADD_TRACE?REFRESH:
          ERASE,last_trace_hilited[quadrant]);
       }
       break;
      case CHG_X_SCALE_AREA:
      case CHG_TIME_SCALE_AREA:
       if (hilite_debug)
        printf("clear CHG_X_SCALE_AREA\n");
       clear_range_label(quadrant,get_indepvar_yaxis(quadrant)?
        last_trace_hilited[quadrant]:INDEPVAR);
/* 
cover special case: cursor moves from dependent var range label on
x-axis to trace label, below it; trace label already highlighted, as it
should be, but clear of range label here clears that trace label as
well.  Redraw that trace label after range label is cleared to correct this.
*/
       if (get_indepvar_yaxis(quadrant) 
        && pending_op[quadrant]==CHG_TRACE
        && highlighted_trace[quadrant]==last_trace_hilited[quadrant])
        highlight_trace_label(quadrant,CHG_TRACE);
/*
similar to above, but cursor moves from x-axis dep't range label to indep't 
trace label directly below it
*/
       if (get_indepvar_yaxis(quadrant) 
        && pending_op[quadrant]==CHG_INDEPVAR)
        highlight_trace_label(quadrant,CHG_INDEPVAR);
/*
another special case: traces are isolated, ergo range labels are all on one
row on x-axis. Cursor moves horizontally between different ranges; clear last
one is ok, but must re-hilite current one again.
*/
       if (last_op[quadrant]==pending_op[quadrant] &&
           last_trace_hilited[quadrant]!=highlighted_trace[quadrant])
        highlight_range_label(quadrant);
/* 
yet another special case: isolated traces with all dep't range labels on same 
x-row; redraw all when cursor no longer highlights any one of them any more.
*/
       if (get_indepvar_yaxis(quadrant) &&
           get_isolate_traces(quadrant) && 
           pending_op[quadrant]!=CHG_X_SCALE) {
        int trace;
        for (trace=0; trace<num_of_traces[quadrant]; trace++)
         if (trace!=last_trace_hilited[quadrant])
/*
don't bother with last trace hilited: it's already been redrawn by above
call to clear_range_label()
*/
          DrawRangeLabels(quadrant,color[quadrant][trace],trace);
       }
       break;
      case CHG_Y_SCALE_AREA:
       if (hilite_debug)
        printf("clear CHG_Y_SCALE_AREA\n");
       clear_range_label(quadrant,last_trace_hilited[quadrant]);
/*
special case: traces are isolated, ergo range labels are all on one
row on x-axis. Cursor moves horizontally between different ranges; clear last
one is ok, but must re-hilite current one again.
*/
       if (last_op[quadrant]==pending_op[quadrant] &&
           last_trace_hilited[quadrant]!=highlighted_trace[quadrant])
        highlight_range_label(quadrant);
/* 
special case: isolated traces with all dep't range labels on same 
y-row; redraw all when cursor no longer highlights any one of them any more.
*/
       if (!get_indepvar_yaxis(quadrant) &&
           get_isolate_traces(quadrant) && 
           pending_op[quadrant]!=CHG_Y_SCALE) {
        int trace;
        for (trace=0; trace<num_of_traces[quadrant]; trace++)
         if (trace!=last_trace_hilited[quadrant])
/*
don't bother with last trace hilited: it's already been redrawn by above
call to clear_range_label()
*/
          DrawRangeLabels(quadrant,color[quadrant][trace],trace);
       }
       break;
      case PLOT_FRAME_AREA:
       if (plot_type[quadrant]==TRACK_PLOT)
        clear_hilited_landmark(quadrant);
       break;
      case PMS2D_AREA:
      case ADSPOP_AREA:
      case FLTFRAME_AREA:
      case CALIB_DIALOGUE_AREA:
      case CALIB_PLOT_AREA:
      case UNKNOWN_AREA:
      case CTL_AREA:
      case MENU_AREA:
      case NOTICE_AREA:
      case NEW_SETUP_AREA:
      case OP_PANEL_AREA:
      case NO_AREA:
       if (hilite_debug)
        printf("no relevant area...\n");
       break;
      default:
       if (hilite_debug)
        printf("unexpected last area %d in ENTER/MOVE last_area switch \n",
        last_area[quadrant]);
       break;
     }
    } 
   }
   if (force_help_update || last_area[quadrant] != current_area[quadrant]) {
    chg_tb_help_image(current_area[quadrant]);
    if (!cursor_in_Panel() &&
       (window_type[quadrant]==DISPTYPE_XYPLOT 
     || window_type[quadrant]==DISPTYPE_LISTS)) {
     switch (current_area[quadrant]) {
      case CHG_Y_SCALE_AREA:
      case CHG_X_SCALE_AREA:
      case CHG_TRACE_AREA:
      case CHG_INDEP_AREA:
      case CHG_LISTEE_AREA:
       update_leftop(quadrant,TRUE);
       break;
      case CHG_TIME_SCALE_AREA:
       break;
      default:
       update_leftop(quadrant,FALSE);
       break;
     }
    } 
   }

/*update the state variables: */
   update_area_vars(quadrant,current_area[quadrant],highlighted_trace[quadrant],pending_op[quadrant]);
}

/****************************  GET_CURRENT_AREA()  ****************************/
 
get_current_area()
{
 return current_area[current_quadrant];
}

/******************************************************************************/
 
char *
GetHilitedVarName()
{
 return HilitedVarName;
}

/******************************************************************************/
 
SetHilitedVarName(name)
char *name; 
{
 (void)sprintf(HilitedVarName,"%s",name);
}

/****************************  SET_CURRENT_AREA()  ****************************/
 
set_current_area(quadrant,x,y)
int quadrant,x,y;
/* update highlighted_trace, pending_op, and current_area based on cursor position (x,y)
*/
{
 
 
  highlighted_trace[quadrant]=NONE;
  SetHilitedVarName("\0");
  pending_op[quadrant]=NONE;
  current_area[quadrant]=NO_AREA;

/* check for new setup initiated before looking at window_type[], since it
   won't change until new setup is complete.
*/
  if (event_in_setup_panel()) {
   pending_op[quadrant]=NEW_SETUP_OPS;
   current_area[quadrant]=NEW_SETUP_AREA;
   return;
  }

/* check for cursor in op panel area
*/
  if (event_in_op_panel()) {
   pending_op[quadrant]=OP_PANEL_OPS;
   current_area[quadrant]=OP_PANEL_AREA;
   return;
  }

/* check for cursor in control panel area
*/
  if (event_in_ctl_panel()) {
   pending_op[quadrant]=CTL_PANEL_OPS;
   current_area[quadrant]=CTL_AREA;
   return;
  }

/* check for cursor in panel menu area
*/
  if (event_in_panel_menu()) {
   pending_op[quadrant]=PANEL_MENU_OPS;
   current_area[quadrant]=current_panel_menu_area();
   return;
  }


  if (xy_window_type(quadrant)) {

   switch (plot_type[quadrant]) {
   case LISTS:
    if (event_in_chg_listee_area(x)) {
     highlighted_trace[quadrant]=listee_number[quadrant];
     if (listee_number[quadrant]>=GetNumListees(quadrant))
      SetHilitedVarName("\0");
     else
      SetHilitedVarName(GetListeeName(quadrant,highlighted_trace[quadrant]));
     if (last_area[quadrant]!=CHG_LISTEE_AREA && hilite_debug) {
      time_to_log();
      (void)fprintf(stderr,"new area: CHG_LISTEE_AREA; highlighted_trace= %d\n",
       highlighted_trace[quadrant]);
     }
     pending_op[quadrant]=CHG_LISTEE;
     current_area[quadrant]=CHG_LISTEE_AREA;
    } else {

/* set to area with correct frame label string -- op may vary as per user
   designated left track ball button assignment
*/
     current_area[quadrant]=PLOT_FRAME_AREA;
    } 
    break;
   case FIXED_FORMAT:
    if (event_in_chg_fixee_area(x,y)) {
     highlighted_trace[quadrant]=fixee_number[quadrant];
     if (fixee_number[quadrant]>=GetNumFixees(quadrant))
      SetHilitedVarName("\0");
     else
      SetHilitedVarName(GetFixeeName(quadrant,highlighted_trace[quadrant]));
     if (last_area[quadrant]!=CHG_FIXEE_AREA && hilite_debug)
      printf("new area: CHG_FIXEE_AREA; highlighted_trace[]= %d\n",
       highlighted_trace[quadrant]);
     pending_op[quadrant]=CHG_FIXEE;
     current_area[quadrant]=CHG_FIXEE_AREA;
    }
    break;
   case SKEW_T:
    if (last_area[quadrant]==UNKNOWN_AREA)

/* since SKEW_T has no event_in_xxx() function for areas other than ChgTrace,
   use this test instead to set value for current_area.  It will fall through to
   test below for event_in_chg_trace_area() and correctly update current_area 
   if cursor in that area.  Don't set it every time, however, since cursor
   movement would result in (last_area[q]!=current_area[q]) being TRUE on each
   test, with unneeded track-ball image updates.
*/
     current_area[quadrant]=NO_AREA;
   default:
    if (event_in_chg_trace_area(y)) {
     highlighted_trace[quadrant]=trace_number[quadrant];
     if (!GetVarIsIndep(quadrant)) {
      if (last_area[quadrant]!=CHG_TRACE_AREA && hilite_debug)
       printf("new area: CHG_TRACE_AREA; highlighted_trace[]= %d\n",
       highlighted_trace[quadrant]);
      pending_op[quadrant]=CHG_TRACE;
      current_area[quadrant]=CHG_TRACE_AREA;
      SetHilitedVarName(GetTraceName(quadrant,highlighted_trace[quadrant]));
     } else {
      if (last_area[quadrant]!=CHG_INDEP_AREA && hilite_debug)
       printf("new area: CHG_INDEP_AREA; highlighted_trace[]= %d\n",
       highlighted_trace[quadrant]);
      pending_op[quadrant]=CHG_INDEPVAR;
      current_area[quadrant]=CHG_INDEP_AREA;
      SetHilitedVarName(indepvar_name[quadrant]);
     }
 
    } else if (event_in_add_trace_area(y)) {
      highlighted_trace[quadrant]=trace_number[quadrant];
      SetHilitedVarName("\0");
      if (last_area[quadrant]!=ADD_TRACE_AREA && hilite_debug)
       printf("new area: ADD_TRACE_AREA; highlighted_trace[]= %d\n",
       highlighted_trace[quadrant]);
      current_area[quadrant]=ADD_TRACE_AREA;
      pending_op[quadrant]=ADD_TRACE;
      if (!(num_of_traces[quadrant]))
       need_rehilite[quadrant]=TRUE;

    } else if (event_in_plot_frame_area(x,y)) {
      highlighted_trace[quadrant]=NONE;
      SetHilitedVarName("\0");
      if (last_area[quadrant]!=PLOT_FRAME_AREA && hilite_debug)
       printf("new area: PLOT_FRAME_AREA; highlighted_trace= %d\n",
       highlighted_trace[quadrant]);
      current_area[quadrant]=PLOT_FRAME_AREA;
      pending_op[quadrant]=NONE;

    } else if (event_in_chg_x_scale_area(y)) {
      highlighted_trace[quadrant]=trace_number[quadrant];
      SetHilitedVarName("\0");
      if (last_area[quadrant]!=CHG_X_SCALE_AREA && hilite_debug)
       printf("new area: CHG_X_SCALE_AREA; highlighted_trace= %d\n",
       highlighted_trace[quadrant]);
      current_area[quadrant]=CHG_X_SCALE_AREA;
      pending_op[quadrant]=CHG_X_SCALE;

    } else if (event_in_chg_time_scale_area(y)) {
      trace_number[quadrant]=highlighted_trace[quadrant]=INDEPVAR;
      SetHilitedVarName("\0");
      if (last_area[quadrant]!=CHG_TIME_SCALE_AREA && hilite_debug)
       printf("new area: CHG_TIME_SCALE_AREA; highlighted_trace= %d\n",
       highlighted_trace[quadrant]);
      current_area[quadrant]=CHG_TIME_SCALE_AREA;
      pending_op[quadrant]=CHG_TIME_SCALE;

    } else if (event_in_chg_y_scale_area(x,y)) {
      highlighted_trace[quadrant]=trace_number[quadrant];
      SetHilitedVarName("\0");
      if (last_area[quadrant]!=CHG_Y_SCALE_AREA && hilite_debug)
       printf("new area: CHG_Y_SCALE_AREA; highlighted_trace[]= %d\n",
       highlighted_trace[quadrant]);
      current_area[quadrant]=CHG_Y_SCALE_AREA;
      pending_op[quadrant]=CHG_Y_SCALE;

    } else if (event_in_vector_scale_area(x,y)) {
      current_area[quadrant]=CHG_VECTOR_SCALE_AREA;
      pending_op[quadrant]=CHG_VECTOR_SCALE;
      highlighted_trace[quadrant]=NONE;
      SetHilitedVarName("\0");
    } 
    if (plot_type[quadrant]==TRACK_PLOT) {
     if (update_hilited_landmark(quadrant,x,y)) {
      current_area[quadrant]=LANDMARK_OPS_AREA;
      pending_op[quadrant]=DELETE_LANDMARK;
     }
    }
    break;
   }
  } else if (window_type[quadrant]==WTYPE_CALIBDISP) {
   switch (quadrant) {
    case PLOTS:
     highlighted_trace[quadrant]=NONE;
     SetHilitedVarName("\0");
     if (last_area[quadrant]!=CALIB_PLOT_AREA && hilite_debug)
      printf("new area: CALIB_PLOT_AREA\n");
     current_area[quadrant]=CALIB_PLOT_AREA;
     pending_op[quadrant]=NONE;
     break;
    case STATS:
     highlighted_trace[quadrant]=current_setpt(y);
     SetHilitedVarName("\0");
     if (last_area[quadrant]!=CALIB_SETPT_AREA && hilite_debug)
      printf("new area: CALIB_SETPT_AREA\n");
     current_area[quadrant]=CALIB_SETPT_AREA;
     pending_op[quadrant]=DEL_SETPT;
     break;
    case DIALOGUE:
     highlighted_trace[quadrant]=NONE;
     SetHilitedVarName("\0");
     if (last_area[quadrant]!=CALIB_DIALOGUE_AREA && hilite_debug)
      printf("new area: CALIB_DIALOGUE_AREA\n");
     current_area[quadrant]=CALIB_DIALOGUE_AREA;
     pending_op[quadrant]=NONE;
     break;
   }
 } else if (window_type[quadrant]==DISPTYPE_PMS1D) {
  switch (current_area[quadrant]=get_current_pms1d_area(quadrant,y)) {
   case PMS1D_YMAX_AREA:
    pending_op[quadrant]=PMS1D_MAXY;
    break;
   case PMS1D_YMIN_AREA:
    pending_op[quadrant]=PMS1D_MINY;
    break;
   case PMS1D_ACCUM_AREA:
    pending_op[quadrant]=PMS1D_PERIOD;
    break;
  }
 } else if (pms2d_window_type(quadrant)) {
  switch (GetPms2dDispType(quadrant)) {
   case PMS2D_GREYSCALE:
   case PMS2D_HVPS:
   case PMS2D_STANDARD:
    current_area[quadrant]=PMS2D_AREA;
    break;
  }
 } 
}

/****************************  GET_CURRENT_PMS1D_AREA()  ********************/

get_current_pms1d_area(quadrant,y)
int quadrant,y;
{
 if (y<ymin_ypos(quadrant))
  return PMS1D_YMAX_AREA;
 else if (y<divider_ypos(quadrant))
  return PMS1D_YMIN_AREA;
 else
  return PMS1D_ACCUM_AREA;
}

/***************************  UPDATE_AREA_VARS()  ****************************/

update_area_vars(quadrant,area,trace,op)
int quadrant,area,trace,op;
/* update last_area,last_trace_hilited, and last_op
*/
{
 last_area[quadrant]=area;
 last_trace_hilited[quadrant]=trace;
 last_op[quadrant]=op;
}


/***************************  CLEAR_AREA()  ***********************************/

clear_all_hilited(quadrant)
int quadrant;
{
  pending_op[quadrant]=NONE;
  highlighted_trace[quadrant]=NONE;
  SetHilitedVarName("\0");
  switch (last_area[quadrant]) {
    case PMS1D_YMAX_AREA:
     label_1d_ymax(quadrant,FOREGROUND,BKGND);
     break;
    case PMS1D_YMIN_AREA:
     label_1d_ymin(quadrant,FOREGROUND,BKGND);
     break;
    case PMS1D_ACCUM_AREA:
     list_sum(quadrant,FOREGROUND,BKGND);
     break;
    case CHG_LISTEE_AREA:
     clear_listee_label(quadrant,REFRESH,last_trace_hilited[quadrant]);
     break;
    case CHG_FIXEE_AREA:
     clear_fixee_label(quadrant,REFRESH,last_trace_hilited[quadrant]);
     break;
    case CALIB_SETPT_AREA:
     clear_setpt_label(last_trace_hilited[quadrant]);
     break;
    case CHG_TRACE_AREA:
    case CHG_INDEP_AREA:
     if (hilite_debug)
      printf("clear CHG_INDEP_AREA or CHG_TRACE_AREA in clear_all_hilited\n");
     clear_trace_label(quadrant,REFRESH,last_trace_hilited[quadrant]);
     break;
    case ADD_TRACE_AREA:
     if (num_of_traces[quadrant] < MAX_TRACES) {
      if (hilite_debug)
       printf("clear ADD_TRACE_AREA in clear_all_hilited\n");
      clear_trace_label(quadrant,ERASE,last_trace_hilited[quadrant]);
     }
     break;
    case CHG_Y_SCALE_AREA:
     if (hilite_debug)
      printf("clear previous CHG_Y_SCALE_AREA in clear_all_hilited\n");
     clear_range_label(quadrant,last_trace_hilited[quadrant]);
/*
special case: isolated traces with all dep't range labels on same
y-row; redraw all when cursor no longer highlights any one of them any more.
*/
     if (!get_indepvar_yaxis(quadrant) &&
         get_isolate_traces(quadrant) &&
         pending_op[quadrant]!=CHG_Y_SCALE) {
      int trace;
      for (trace=0; trace<num_of_traces[quadrant]; trace++)
       if (trace!=last_trace_hilited[quadrant])
/*
don't bother with last trace hilited: it's already been redrawn by above
call to clear_range_label()
*/
        DrawRangeLabels(quadrant,color[quadrant][trace],trace);
     }
     break;
    case CHG_X_SCALE_AREA:
     if (hilite_debug)
      printf("clear previous CHG_X_SCALE_AREA in clear_all_hilited\n");
     clear_range_label(quadrant,get_indepvar_yaxis(quadrant)?
      last_trace_hilited[quadrant]:INDEPVAR);
/*
special case: isolated traces with all dep't range labels on same
x-row; redraw all when cursor no longer highlights any one of them any more.
*/
     if (get_indepvar_yaxis(quadrant) &&
         get_isolate_traces(quadrant) &&
         pending_op[quadrant]!=CHG_X_SCALE) {
      int trace;
      for (trace=0; trace<num_of_traces[quadrant]; trace++)
       if (trace!=last_trace_hilited[quadrant])
/*
don't bother with last trace hilited: it's already been redrawn by above
call to clear_range_label()
*/
        DrawRangeLabels(quadrant,color[quadrant][trace],trace);
     }
     break;
    case CHG_TIME_SCALE_AREA:
     if (hilite_debug)
      printf("clear previous CHG_TIME_SCALE_AREA in clear_all_hilited\n");
     clear_range_label(quadrant,INDEPVAR);
     break;
    case CHG_VECTOR_SCALE_AREA:
     redraw_vector_scale_area(quadrant);
     break;
    case PLOT_FRAME_AREA:
     if (plot_type[quadrant]==TRACK_PLOT)
      clear_hilited_landmark(quadrant);
     break;
    case UNKNOWN_AREA:
    case NO_AREA:
     break;
    default:
     break;
  }
  update_area_vars(quadrant,UNKNOWN_AREA,NONE,NONE);
}

/**************************  XYPLOT_MSMIDDLE()  ******************************/

xyplot_msmiddle(quadrant,event,x,y)
Event *event;
int quadrant,x,y;
{
  switch (plot_type[quadrant]) {
   case LISTS:
    if (event_in_chg_listee_area(x)) {
     report(quadrant,"EVENT MS_MIDDLE: chg listee");
     update_mode[quadrant]=CHG_LISTEE;
     chg_listee(event);
    }
    break;
   case FIXED_FORMAT:
    if (event_in_chg_fixee_area(x,y)) {
     report(quadrant,"EVENT MS_MIDDLE: chg fixee");
     update_mode[quadrant]=CHG_FIXEE;
     chg_fixee(event);
     need_rehilite[quadrant]=TRUE;
    }
    break;
   default:

/*** IMPORTANT: keep this test FIRST so that frame events will not be processed
  according to values of previous (x, y) coordinates
***/
    if (event_in_plot_frame_area(x,y)) {
     break;

    } else if (event_in_vector_scale_area(x,y)) {

     zoom_in(ZOOM_VECTOR,quadrant,event_window(event));
     return;

    } else if (event_in_chg_y_scale_area(x,y) || 
               event_in_chg_x_scale_area(y)   ||
               event_in_chg_time_scale_area(y)
     ) {

     zoom_in(ZOOM,quadrant,event_window(event));
     return;

    } else if (event_in_chg_trace_area(y)) {

     if (!GetVarIsIndep(quadrant)) {
      report(quadrant,"EVENT MS_MIDDLE: chg trace");
      update_mode[quadrant]=CHG_TRACE;
     } else {
      report(quadrant,"EVENT MS_MIDDLE: chg indepvar");
      trace_number[quadrant]=INDEPVAR;
      update_mode[quadrant]=CHG_INDEPVAR;
     }

     ChgTrace(event);

    } else if (event_in_add_trace_area(y)) {

     report(quadrant,"EVENT MS_MIDDLE: add trace");
     update_mode[quadrant]=ADD_TRACE;
     if ((num_of_traces[quadrant]==ERROR)) 
/* 
special case, when empty window is filled in via MIDDLE or RIGHT; set variable
need_rehilite to avoid clearing name label on next LOC_MOVE event 
*/
      need_rehilite[quadrant]=TRUE;  
     add_trace(event);
    }
  }
}

/**************************  XYPLOT_MSRIGHT()  ******************************/

xyplot_msright(quadrant,event,x,y)
Event *event;
int quadrant,x,y;
{

 switch (plot_type[quadrant]) {
   case LISTS:
    if (event_in_chg_listee_area(x)) {
     report(quadrant,"EVENT MS_RIGHT: chg listee");
     update_mode[quadrant]=CHG_LISTEE;
     chg_listee(event);
    } 
    break;

   case FIXED_FORMAT:
    if (event_in_chg_fixee_area(x,y)) {
     report(quadrant,"EVENT MS_RIGHT: chg fixee");
     update_mode[quadrant]=CHG_FIXEE;
     chg_fixee(event);
     need_rehilite[quadrant]=TRUE;

    } 
    break;

   default:

/*** IMPORTANT: keep this test FIRST so that frame events will not be processed
  according to values of previous (x, y) coordinates
***/
    if (event_in_plot_frame_area(x,y)) {
     if (plot_type[quadrant]==TRACK_PLOT)
      hide_landmark(quadrant);
     break;

    } else if (event_in_vector_scale_area(x,y)) {

     zoom_out(UNZOOM_VECTOR,quadrant,event_window(event));
     return;

    } else if (event_in_chg_y_scale_area(x,y) ||
               event_in_chg_time_scale_area(y)||
               event_in_chg_x_scale_area(y)) {

     zoom_out(UNZOOM,quadrant,event_window(event));
     return;

    } else if (event_in_chg_trace_area(y)) {

     if (GetVarIsIndep(quadrant)) {
      report(quadrant,"EVENT MS_RIGHT: chg indepvar");
      trace_number[quadrant]=INDEPVAR;
      update_mode[quadrant]=CHG_INDEPVAR;
     } else {
      report(quadrant,"EVENT MS_RIGHT: chg or delete trace");
      update_mode[quadrant]=CHG_TRACE;
     }
     ChgTrace(event);

    } else if (event_in_add_trace_area(y)) {

     report(quadrant,"EVENT MS_RIGHT: add trace");
     update_mode[quadrant]=ADD_TRACE;
     if (num_of_traces[quadrant]==ERROR)   
/* 
special case, when empty window is filled in via
MIDDLE or RIGHT; set this to avoid clearing name label on next
LOC_MOVE event 
*/
      need_rehilite[quadrant]=TRUE;  
     add_trace(event);
    }
  }
}

/********************  StartupClixTimer()  **********************************/

void StartupClixTimer(client)
Notify_client client;
{
 (void) notify_set_itimer_func(client,count_clix,
  ITIMER_REAL,&clix_interval,ITIMER_NULL);
}

/***********************  StopClixTimer()  **********************************/

void StopClixTimer(client)
Notify_client client;
{
 (void) notify_set_itimer_func(client,count_clix,  
  ITIMER_REAL,ITIMER_NULL,ITIMER_NULL);
}

/********************  RecycleClixTimer()  **********************************/

void RecycleClixTimer(client)
Notify_client client;
{
 StopClixTimer(client);
 StartupClixTimer(client);
}

/**************************  ZOOM_CLICK()  **********************************/

zoom_click(event)
Event *event;
{
 if (cursor_in_ctl_panel || 
     cursor_in_control_panel || 
     non_click_window(current_quadrant) )

/* here we must check for BOTH state parameter values because 
   cursor_in_control_panel becomes false when control panel LOC_WINEXIT is
   generated, but current_quadrant doesn't change from previous value until
   LOC_WINENTER is generated on entry to next window.  Failing to check
   value of current_quadrant would index num_clix incorrectly, and any
   non-zero value in that (garbage) space results in incorrectly returning
   TRUE here.  Returning FALSE allows event_proc control to fall through
   to LOC_WINENTER event processing, setting value of current_quadrant as
   desired.
*/
  return (FALSE);

 if (num_clix[current_quadrant]) {
  if (event_id(event)==active_button[current_quadrant]) { /* more clicks 
   detected; increment counter and recycle timer */
   num_clix[current_quadrant]++;
   RecycleClixTimer(event_window(event));
  } 
  return(TRUE);
 } else
  return(FALSE);
}

/**************************  ZOOM_IN()  *************************************/

zoom_in(op,quadrant,window)
int op,quadrant;
Window window;
{
   report(quadrant,"EVENT MS_MIDDLE: zoom in");
   num_clix[quadrant]=1;
   active_button[quadrant]=MS_MIDDLE;
   ZoomOp=op;
   RecycleClixTimer(window);
}

/**************************  ZOOM_OUT()  *************************************/

zoom_out(op,quadrant,window)
int op,quadrant;
Window window;
{
   report(quadrant,"EVENT MS_RIGHT: zoom out");
   num_clix[quadrant]=1;
   active_button[quadrant]=MS_RIGHT;
   ZoomOp=op;
   RecycleClixTimer(window);
}

/************************  COUNT_CLIX()  **********************************/

/*ARGSUSED*/
Notify_value
count_clix(client,which)
Notify_client client;
int which;
 /*
   timer expiration function -- registered for canvas ...
   handle timeout after clicks in window: count the number of clicks
   made, process accordingly
*/

{
Event next_event;

  StopClixTimer(client);

/* consume all events that accumulated since last examination of num_clix
   ... this prevents repeated calls to this function (and attendant confusion
   in display) if user clicked button while it was working.
*/
  xv_input_readevent(client,&next_event,FALSE,(int)NULL,NULL);
 
  process_clix(ZoomOp);
  num_clix[current_quadrant]=0;

  return(NOTIFY_DONE);
}

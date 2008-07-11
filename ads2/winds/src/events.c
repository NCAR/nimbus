/* events.c  -- handle input events
*/

#include <stdio.h>
#include <ctype.h>
#include <xview/xview.h>
#include <xview/canvas.h>

/****************************  WINDS INCLUDES  ******************************/

#include <ops.h>
#include <xy_plt.h>
#include <xview.h>
#include <xview/panel.h>
#include <help.h>
#include <lengths.h>
#include <tasks.h>
#include <constants.h>
#include <display.h>
#include <header.h>
#include <errors.h>
#include <proto.h>

/*************************  EXPORTABLE  *************************************/

/* ON-LINE HELP:

   declare one target string, assumed an entry in $wdoc/WINDS.info, for each 
   area defined in $winc/help.h.  Set the current area on LOC_WINENTER and/or
   LOC_MOVE events with a call to set_current_area().  Ensure the given area
   (and operation) is seen correctly in that routine.  Then, get the proper
   help window on EVENT_HELP_KEY events by tracking those events in 
   window's WIN_EVENT_PROC, setting current_quadrant, and calling 

xv_help_show(window,targets[get_current_area()],event);

   ...and the text at the target string will appear in a help window.  Note 
   that KEY_TOP(1) was removed from detection because its behavior was 
   not consistent across all types of windows.  Sometimes, the Xview package
   would recognize it, sometimes it would get confused and respond that
   no help is available for the given window.  It may be needed later for
   PC's without the HELP key, but the issue will be deferred until then.

   NOTE: with OW3, panel objects do not detect the HELP key, and as such
   must be given help at xv_create() time with the XV_HELP_DATA attribute.
*/

char *targets[NUM_AREAS]= {
	"WINDS:INTRO",
	"WINDS:CHG_TRACE_HELP",
	"WINDS:CHG_INDEP_HELP",
	"WINDS:ADD_TRACE_HELP",
	"WINDS:CHG_Y_SCALE_HELP",
	"WINDS:CHG_X_SCALE_HELP",
	"WINDS:PLOT_FRAME_HELP",
	"WINDS:PMS2D_HELP",
	"WINDS:ADSPOP_HELP",
	"WINDS:FLTFRAME_HELP",
	"WINDS:CALIB_SETPT_HELP",
	"WINDS:CALIB_DIALOGUE_HELP",
	"WINDS:CALIB_PLOT_HELP",
	"WINDS:CHG_LISTEE_HELP",
	"WINDS:CTL_PANEL_HELP",
	"WINDS:CHG_FIXEE_HELP",
	"WINDS:MENU_HELP",
	"WINDS:NOTICE_HELP",
	"WINDS:CHG_VECTOR_SCALE_HELP",
	"WINDS:NEW_SETUP_HELP",
	"WINDS:OP_PANEL_HELP",
	"WINDS:TIME_SCALE_HELP",
	"WINDS:STATS_MENU_HELP",
	"WINDS:ATTRS_MENU_HELP",
	"WINDS:PRINT_MENU_HELP",
	"WINDS:PRINTCONFIG_MENU_HELP",
	"WINDS:PLOTSPEX_MENU_HELP",
	"WINDS:FOLDERS_MENU_HELP",
	"WINDS:PMS1D_ACCUM_HELP",
	"WINDS:PMS1D_YMAX_HELP",
	"WINDS:PMS1D_YMIN_HELP",
	"WINDS:LANDMARK_HELP"
	};

static Xv_Window mouse_win=ERROR; /* window in whose coordinate system 
                             event at (mouse_x,mouse_y) took place */
static int mouse_x,		/* current mouse x,y coordinates */ 
           mouse_y;

int current_quadrant,	/* current quadrant as per cursor location */
    cursor_in_control_panel=FALSE,   /* TRUE when cursor over control panel */
    cursor_in_ctl_panel=FALSE,   /* TRUE when cursor over control panel */
    cursor_in_op_panel=FALSE,   /* TRUE when cursor over an op-panel */
    cursor_in_setup_panel=FALSE,   /* TRUE when cursor over a setup panel */
    cursor_in_menu_panel=FALSE,   /* TRUE when cursor over a panel menu */
    cursor_in_panel_menu=FALSE,   /* TRUE when cursor over a panel menu */
    unset_current_panel;


int hilite_debug=FALSE;        /* debug the highlighting processing */

int last_event=ERROR;	       /* keep track of last event to preclude 
				   multiple events processed when user
				   holds down key */
int last_window=ERROR;	       /* keep track of last window to preclude 
				   multiple events processed when user
				   holds down key */

/**************************  GLOBAL IN FILE  *********************************/

static int event_debug=FALSE;  /* debug the events */
static int focus_debug=FALSE;  /* debug the kbd focus */
static int Serial_feed_debug=FALSE; /* debug serial feeds */
static Window kbd_focus_window;


/**************************************************************************/

SetMouseWindow(Xv_Window window)
{
 mouse_win=window;
}

/**************************************************************************/

Xv_Window 
GetMouseWindow()
{
 return mouse_win;
}

/**************************************************************************/

GetMouseX()
{
 return mouse_x;
}

/**************************************************************************/

GetMouseY()
{
 return mouse_y;
}

/**************************************************************************/

GetCurrentQuadrant()
{
 return current_quadrant;
}

/***********************  SET_KBD_FOCUS_WINDOW()  *****************************/

set_kbd_focus_window(Window which)
{
 if (!which) {
/*
 sometimes state variables don't get reset, maybe because their objects don't 
 receive the proper LOC_WINEXIT event.  Safeguard against that here.
*/
  if (cursor_in_setup_panel ) {
   if (focus_debug)
    (void)fprintf(stderr,"WARNING: cursor_in_setup_panel did not get reset\n");
   cursor_in_setup_panel=FALSE;
  }
  if (cursor_in_menu_panel) {
   if (focus_debug)
    (void)fprintf(stderr,"WARNING: cursor_in_menu_panel did not get reset\n");
   cursor_in_menu_panel=FALSE;
  }
  if (focus_debug)
   (void)fprintf(stderr,
    "WARNING: set requested on kbd_focus_window=0; not done\n");
  return;
 }
 if (focus_debug)
  fprintf(stderr, "set keyboard focus to %d\n",which);
 kbd_focus_window=which;
/***
 xv_set(kbd_focus_window,WIN_SET_FOCUS,NULL);
***/
 win_set_kbd_focus(kbd_focus_window, xv_get(kbd_focus_window, XV_XID));
/***
kbd_focus_window=paint_window[quadrant+FIRST_OP_QUADRANT];
 win_set_kbd_focus(kbd_focus_window, xv_get(kbd_focus_window, XV_XID));
***/
}

/***********************  GET_KBD_FOCUS_WINDOW()  *****************************/
Window get_kbd_focus_window()
{
  return kbd_focus_window;
}

/***********************  RESTORE_KBD_FOCUS()  ********************************/
restore_kbd_focus()
{
  if (focus_debug)
    fprintf(stderr, "restore keyboard focus\n");

  set_kbd_focus_window(get_kbd_focus_window());
}

/***********************  EVENT_PROC()  ***********************************/
void event_proc(Xv_Window window, Event *event)
{

  /* accept only down events, except for special cases:
   */
  if (!event_is_down(event) && event_id(event) != LOC_MOVE) {
    /* reset last event memory if up-stroke detected, so that next down stroke
     * will be processed
     */
    if (event_is_up(event) && event_id(event) == last_event)
      last_event=ERROR;

    return;
    }

  if (event_debug) {
    fprintf(stderr, "--------------------------------\n");
    if (event_id(event) != LOC_MOVE && event_id(event) != 32525) {
      fprintf(stderr, "event: %d at %d,%d in %d\n",
	event_id(event), event_x(event), event_y(event), window);
      }
    }

  /* ignore right keys for panel objects
   */
  if (event_is_key_right(event) && cursor_in_Panel())
    return;

  if (GetMultiPrint() || zoom_click(event) || GetScrollPanelShowing(FALSE))
    return;

  /* not zoom_click or motion tracking event; any other consecutive down events 
   * of same id should be ignored: this provides for user holding down on a 
   * key too long.
   */
  if (event_id(event)!=LOC_MOVE && event_id(event)!=32525
	&& last_window==window && event_id(event)==last_event)
    {
    if (event_debug)
      printf("ignore consecutive event\n");
    return;
    }
  else
    {
    last_event=event_id(event);
    last_window=window;
    }

  /* send selected ascii events to text input procedures
   */
  if (!event_is_key_right(event) && !event_is_key_left(event) && 
      !event_is_key_top(event) && !event_is_button(event) && 
      isprint((char)event_id(event))) {
    start_text_op(current_quadrant,get_current_area());
    return;
    }

  if (set_mouse_xy(event)) {
    sprintf(errmsg,
	"WARNING: x==y==0; event: %d; window: %d\n",event_id(event),mouse_win);
    report(current_quadrant, errmsg);
    }

//fprintf(stderr, "EVENT (%d): %d %d\n", event_id(event), current_quadrant, cursor_in_ctl_panel);

  switch (event_id(event)) {
    case WIN_RESIZE:
/***
      ResizeCanvasWindows();
***/
      break;
    case LOC_WINENTER:
/* check for redundant LOC_WINENTER (event in window already deemed current) 
   or for 'bogus' LOC_WINENTER event (occur when the MS_RIGHT is clicked to 
   dismiss a menu).
*/
      if ((window==mouse_win || winenter_invalid(window,event))
/* ...exception: changing from control panel to calibration window, recognize
   event so keyboard focus can be transferred
*/
	&& !(window==paint_window[CTL_PANEL] && calib_running))
        return;

      if (window==paint_window[TIME_AND_PROJECT] || window==paint_window[HELP_IMAGE]) {
        mouse_win=window;
        cursor_in_control_panel=TRUE;
        current_quadrant=HELP_IMAGE;
        if (focus_debug)
          printf("current quadrant is HELP_IMAGE or TIME_AND_PROJECT (%d) \n",window);
      } else 
      if (window==paint_window[STATS]) {
        mouse_win=window;
        current_quadrant=STATS;
        if (focus_debug)
          printf("current quadrant is STATS (%d) \n",window);
      } else 
      if (window==paint_window[DIALOGUE]) {
        mouse_win=window;
        current_quadrant=DIALOGUE;
        if (focus_debug)
          printf("current quadrant is DIALOGUE (%d) \n",window);
      } else 
      if (window==paint_window[PLOTS]) {
        mouse_win=window;
        current_quadrant=PLOTS;
        if (focus_debug)
          printf("current quadrant is PLOTS (%d) \n",window);
      } else 
      if (window==paint_window[UPPER_LEFT]) {
        mouse_win=window;
        current_quadrant=UPPER_LEFT;
        if (focus_debug)
          printf("current quadrant is UPPER LEFT (%d) \n",window);
      } else 
      if (window==paint_window[UPPER_RIGHT]) {
        mouse_win=window;
        current_quadrant=UPPER_RIGHT;
        if (focus_debug)
          printf("current quadrant is UPPER RIGHT (%d) \n",window);
      } else 
      if (window==paint_window[LOWER_RIGHT]) {
        mouse_win=window;
        current_quadrant=LOWER_RIGHT;
        if (focus_debug)
          printf("current quadrant is LOWER RIGHT (%d) \n",window);
      } else 
      if (window==paint_window[LOWER_LEFT]) {
        mouse_win=window;
        current_quadrant=LOWER_LEFT;
        if (focus_debug)
          printf("current quadrant is LOWER LEFT (%d) \n",window);
      } else 
      if (window==paint_window[CTL_PANEL]) {
        mouse_win=window;
        current_quadrant=CTL_PANEL;
        cursor_in_ctl_panel=TRUE;
        if (focus_debug)
          printf("current quadrant is CONTROL (%d) \n",window);
      } else 
      if (window==paint_window[OP_UPPER_LEFT]) {
        mouse_win=window;
        current_quadrant=UPPER_LEFT;
        cursor_in_op_panel=TRUE;
        if (focus_debug)
          printf("current quadrant is UPPER LEFT, op panel (%d) \n",window);
      } else 
      if (window==paint_window[OP_UPPER_RIGHT]) {
        mouse_win=window;
        current_quadrant=UPPER_RIGHT;
        cursor_in_op_panel=TRUE;
        if (focus_debug)
          printf("current quadrant is UPPER RIGHT, op panel (%d) \n",window);
      } else 
      if (window==paint_window[OP_LOWER_RIGHT]) {
        mouse_win=window;
        current_quadrant=LOWER_RIGHT;
        cursor_in_op_panel=TRUE;
        if (focus_debug)
          printf("current quadrant is LOWER RIGHT, op panel (%d) \n",window);
      } else 
      if (window==paint_window[OP_LOWER_LEFT]) {
        mouse_win=window;
        current_quadrant=LOWER_LEFT;
        cursor_in_op_panel=TRUE;
        if (focus_debug)
          printf("current quadrant is LOWER LEFT, op panel (%d) \n",window);
      } else
      if (window==paint_window[STATS_PANEL_MENU]) {
        mouse_win=window;
        current_quadrant=CTL_PANEL;
        cursor_in_panel_menu=TRUE;
        if (focus_debug)
          printf("current quadrant is STATS, panel menu  (%d) \n",window);
      } else
      if (window==paint_window[PRINTCONFIG_PANEL_MENU]) {
        mouse_win=window;
        current_quadrant=CTL_PANEL;
        cursor_in_panel_menu=TRUE;
        if (focus_debug)
          printf("current quadrant is PRINTCONFIG, panel menu  (%d) \n",window);
      } else
      if (window==paint_window[FOLDERS_PANEL_MENU] ||
           window==paint_window[INTERIORPANEL]) {
        mouse_win=window;
        current_quadrant=CTL_PANEL;
        cursor_in_panel_menu=TRUE;
        if (focus_debug)
          printf("current quadrant is FOLDERS, panel menu  (%d) \n",window);
      } else
      if (window==paint_window[PLOTSPEX_PANEL_MENU]) {
        mouse_win=window;
        current_quadrant=CTL_PANEL;
        cursor_in_panel_menu=TRUE;
        if (focus_debug)
          printf("current quadrant is PLOTSPEX, panel menu  (%d) \n",window);
      } else
      if (window==paint_window[PRINT_PANEL_MENU]) {
        mouse_win=window;
        current_quadrant=CTL_PANEL;
        cursor_in_panel_menu=TRUE;
        if (focus_debug)
          printf("current quadrant is PRINT, panel menu  (%d) \n",window);
      } else
      if (window==paint_window[ATTRS_PANEL_MENU]) {
        mouse_win=window;
        current_quadrant=CTL_PANEL;
        cursor_in_panel_menu=TRUE;
        if (focus_debug)
          printf("current quadrant is ATTRS, panel menu  (%d) \n",window);
      } else
      if (window==paint_window[SETUP_MENU_UPPER_LEFT]) {
        mouse_win=window;
        current_quadrant=UPPER_LEFT;
        cursor_in_menu_panel=TRUE;
        if (focus_debug)
          printf("current quadrant is UPPER LEFT, menu panel (%d) \n",window);
      } else 
      if (window==paint_window[SETUP_MENU_UPPER_RIGHT]) {
        mouse_win=window;
        current_quadrant=UPPER_RIGHT;
        cursor_in_menu_panel=TRUE;
        if (focus_debug)
          printf("current quadrant is UPPER RIGHT, menu panel (%d) \n",window);
      } else 
      if (window==paint_window[SETUP_MENU_LOWER_RIGHT]) {
        mouse_win=window;
        current_quadrant=LOWER_RIGHT;
        cursor_in_menu_panel=TRUE;
        if (focus_debug)
          printf("current quadrant is LOWER RIGHT, menu panel (%d) \n",window);
      } else 
      if (window==paint_window[SETUP_MENU_LOWER_LEFT]) {
        mouse_win=window;
        current_quadrant=LOWER_LEFT;
        cursor_in_menu_panel=TRUE;
        if (focus_debug)
          printf("current quadrant is LOWER LEFT, menu panel (%d) \n",window);
      } else
      if (window==paint_window[SETUP_UPPER_LEFT]) {
        mouse_win=window;
        current_quadrant=UPPER_LEFT;
        cursor_in_setup_panel=TRUE;
        if (focus_debug)
          printf("current quadrant is UPPER LEFT, setup panel (%d) \n",window);
      } else 
      if (window==paint_window[SETUP_UPPER_RIGHT]) {
        mouse_win=window;
        current_quadrant=UPPER_RIGHT;
        cursor_in_setup_panel=TRUE;
        if (focus_debug)
          printf("current quadrant is UPPER RIGHT, setup panel (%d) \n",window);
      } else 
      if (window==paint_window[SETUP_LOWER_RIGHT]) {
        mouse_win=window;
        current_quadrant=LOWER_RIGHT;
        cursor_in_setup_panel=TRUE;
        if (focus_debug)
          printf("current quadrant is LOWER RIGHT, setup panel (%d) \n",window);
      } else 
      if (window==paint_window[SETUP_LOWER_LEFT]) {
        mouse_win=window;
        current_quadrant=LOWER_LEFT;
        cursor_in_setup_panel=TRUE;
        if (focus_debug)
          printf("current quadrant is LOWER LEFT, setup panel (%d) \n",window);
      } else {
        if (focus_debug)
          printf("unknown window: %d; panel parent: %d\n",window,
    	xv_get(window,PANEL_PARENT_PANEL));
        last_event = ERROR;
        } 

      if (GetNewSetupNeeded(current_quadrant))
        return;

      if (mouse_win == window) {
        /* a valid window_type has received a LOC_WINENTER event
         */
        if (set_mouse_xy(event)) {
          sprintf(errmsg,
		"WARNING: x==y==0; event: %d; window: %d\n",event_id(event),mouse_win);
          report(current_quadrant, errmsg);
          }

        /* update right-key/track ball help image
         */
        if (cursor_in_Panel())
          set_current_panel((int)xv_get(window,PANEL_CLIENT_DATA));

        if (calib_running) {
          /* Set focus to DIALOGUE window for any calibration window so
           * that keyboard strokes are directed there regardless of cursor position
           * when calibration window is up.
           */
          set_kbd_focus_window(paint_window[DIALOGUE]);
        } else 
        if (current_quadrant < NUM_DISPLAY_QUADRANTS) {
          /* facilitate proper keyboard focus mgmt when moving between panels &
           * canvasses -- without this, you need to click left over calibration
           * panel to enter text, then click left over display canvasses if toggled
           * back to there from calibration.  
           */
          set_kbd_focus_window(
		(cursor_in_setup_panel ?
		paint_window[current_quadrant+FIRST_SETUP_QUADRANT] :
		(cursor_in_menu_panel ?
		paint_window[current_quadrant+FIRST_SETUP_MENU_QUADRANT] :
		paint_window[current_quadrant+FIRST_OP_QUADRANT])));

        } else if (current_quadrant==HELP_IMAGE) {
          /* allow keyboard input from anywhere in control panel area to be directed
           * to input items in CONTROL window
           */
          set_kbd_focus_window(paint_window[CTL_PANEL]);
        } else
          set_kbd_focus_window(window);

        highlight_area(TRUE,current_quadrant,event_x(event),event_y(event));

/* subwindows grouped under same window_type, some of which do highlighting and
   some of which do not.  Manually change track ball help image here for those
   that do not (and thus chg_tb_help_image() is not called automatically for
   them in highlight_area() ).  Those windows of given window_type that do
   no highlighting (and have no subwindows that do either) are handled in
   call to chg_help_images() above.
*/
        switch (current_quadrant) {
          case DIALOGUE:
            chg_tb_help_image(CALIB_DIALOGUE_AREA);
            break;
          case PLOTS:
            chg_tb_help_image(CALIB_PLOT_AREA);
            break;
          }
        }

      break; 

    case LOC_MOVE:
      /* it turns out that LOC_MOVE's can be generated in a window other than the
       * last one for whom LOC_WINENTER was generated -- e.g., when the user is
       * moving the cursor around while waiting for a window to appear.  That can 
       * cause headaches here.  Ergo, protect thus:
       */
      if (mouse_win != window || GetNewSetupNeeded(current_quadrant))
        return;

      highlight_area(FALSE, current_quadrant, event_x(event), event_y(event));
      break;

    case LOC_WINEXIT:
      if (ignore_this_winexit()) {

        /* if a 'bogus' LOC_WINENTER event was received before this one (see
         * above), then ignore the subsequent LOC_WINEXIT as well.
         */
        if (event_debug)
          printf("winexit ignored\n");

        return;
        }

      unset_current_panel=FALSE;
      if (cursor_in_ctl_panel) {
        cursor_in_ctl_panel=FALSE;
        unset_current_panel=TRUE;
        }
      if (cursor_in_op_panel) {
        cursor_in_op_panel=FALSE;
        unset_current_panel=TRUE;
        } 
      if (cursor_in_setup_panel) {
        cursor_in_setup_panel=FALSE;
        unset_current_panel=TRUE;
        } 
      if (cursor_in_panel_menu) {
        cursor_in_panel_menu=FALSE;
        unset_current_panel=TRUE;
        } 
      if (cursor_in_menu_panel) {
        cursor_in_menu_panel=FALSE;
        unset_current_panel=TRUE;
        }
      if (cursor_in_control_panel) {
        cursor_in_control_panel=FALSE;
        unset_current_panel=TRUE;
        }
      if (unset_current_panel) {
        unset_current_panel=FALSE;
        set_current_panel(NONE);
        }
      else {

/* these types track cursor position and need updates on area highlighting
*/
        if (paint_window[current_quadrant] == window)
/*
...then this in NOT a volunteer LOC_WINEXIT event from some other window...
*/
          clear_all_hilited(current_quadrant);
       }


/* set mouse_win to value that differs from any possible paint_window value,
   so that test in next LOC_WINENTER event for mouse_win==window will be FALSE..
   UNLESS this LOC_WINEXIT event is from appearance of a popup window, so that
   test in winenter_invalid() of mouse_win!=window is FALSE.  This allows the
   next LOC_WINEXIT event to be processed as desired, clearing any hilited
   traces.
*/
      mouse_win=ERROR;
      chg_tb_help_image(NO_AREA);
      break;

    case SERIAL_FEED_DEBUG_KEY:
      if (event_ctrl_is_down(event))
        set_serial_feed_debug(serial_feed_debug()?FALSE:TRUE);
      break;

    case EVENT_DEBUG_KEY:
      if (event_ctrl_is_down(event)) {
        event_debug=event_debug?FALSE:TRUE;
        focus_debug=event_debug;
        }
      break;

    case HILITE_DEBUG_KEY:
      if (event_ctrl_is_down(event))
        hilite_debug=hilite_debug?FALSE:TRUE;
      break;

    case WINDS_EXPERT_KEY:
      if (event_shift_is_down(event) &&window==paint_window[CTL_PANEL]) {
        ToggleCalibAccess(); 
        ToggleExpertAccess();
        }
      break; 

    case KBD_USE:
      break; 

    case KBD_DONE:
/*
clicking LEFT causes current focus window to receive a KBD_DONE event.
Correct that here with a restore of focus, i.e. unless value of mouse_win
indicates cursor has left the WINDS window entirely.
*/
      if (!cursor_in_Panel() && mouse_win!=ERROR && window==get_kbd_focus_window())
        restore_kbd_focus();
      break;

    case MS_LEFT:
    case ACTION_SELECT:
      if (cursor_in_control_panel || cursor_in_Panel())
        return;
      if (GetNewSetupNeeded(current_quadrant))
        return;

      exec_hilite_op(current_quadrant,event);
      break;

    case MS_MIDDLE:
    case ACTION_ADJUST:
      if (cursor_in_control_panel || cursor_in_Panel())
        return;
      if (GetNewSetupNeeded(current_quadrant))
        return;

      if (window_type[current_quadrant]==DISPTYPE_PMS1D) {
        zoom_in(ZOOM,current_quadrant,window);
        return;
      } else if (pms2d_window_type(current_quadrant)) {
        break;
      } else if (window_type[current_quadrant]==WTYPE_CALIBDISP) {
        report(current_quadrant,"EVENT MS_MIDDLE: calib menu");
        calib_proc(event);
      } else 
        xyplot_msmiddle(current_quadrant,event,mouse_x,mouse_y);
      break;

    case MS_RIGHT:
    case ACTION_MENU:
      if (cursor_in_control_panel || cursor_in_Panel())
        return;
      if (GetNewSetupNeeded(current_quadrant))
        return;

      if (window_type[current_quadrant]==DISPTYPE_PMS1D) {
        zoom_out(UNZOOM,current_quadrant,window);
      } else if (pms2d_window_type(current_quadrant)) {
        break;
      } else if (window_type[current_quadrant]==WTYPE_CALIBDISP) {
        report(current_quadrant,"EVENT MS_RIGHT: calib menu");
        calib_proc(event);
      } else { 
        xyplot_msright(current_quadrant,event,mouse_x,mouse_y);
       }
      break;

    case EVENT_HELP_KEY:  /* on line help of sorts */
      if (event_id(event)==EVENT_HELP_KEY && event_action(event)!=ACTION_HELP) {
        break;
        }

      report(current_quadrant,"EVENT HELP: on-line help");
      xv_help_show(window,targets[get_current_area()],event);
      break;
    }


} 

/***********************  SET_MOUSE_STATE()  **********************************/

set_mouse_xy(Event *event)
{
/* invalid coordinates of (0,0) sometimes sent with KBD_USE, KBD_DONE; avoid 
   them.  Make sure mouse_win has been set at least once (via a LOC_WINENTER).
   Make sure current event is in that valid mouse_win window.  If all these
   are true, set the internal notion of cursor position, (mouse_x,mouse_y).
   Return TRUE if the event is at (0,0) and hilight debugging is turned on;
   else return FALSE.
*/
 if ( event_id(event)!=KBD_USE 
   && event_id(event)!=KBD_DONE 
   && event_id(event)!=32525
   && mouse_win && event_window(event)==mouse_win) {
  mouse_x=event_x(event);
  mouse_y=event_y(event);
  if (hilite_debug) {
   if (event_x(event)==0 && event_y(event)==0) {
    return(TRUE);
   }
  }
 }
 return(FALSE);
}

/***********************  HIDE_QUADRANTS()  **********************************/

hide_quadrants()
{
int quadrant;
extern Canvas canvas[];

 for (quadrant=0; quadrant < get_num_displays(); quadrant++) {
  xv_set(canvas[quadrant],XV_SHOW,FALSE,NULL);
  xv_set(op_panel[quadrant],XV_SHOW,FALSE,NULL);
 }
}

/***********************  SHOW_QUADRANTS()  **********************************/

show_quadrants()
{
int quadrant;
extern Canvas canvas[];

 for (quadrant=0; quadrant < get_num_displays(); quadrant++) {
  xv_set(canvas[quadrant],XV_SHOW,TRUE,NULL);
  xv_set(op_panel[quadrant],XV_SHOW,TRUE,NULL);
 }
}

static int last_winenter_invalid=FALSE;

/********************  WINENTER_INVALID()  *******************************/

winenter_invalid(Xv_Window window, Event *event)
{
/* when a 'large' menu appears and MS_RIGHT is clicked to dismiss it with
   cursor over a different window than parent to menu, a volunteer event
   LOC_WINENTER is generated from that window, followed by a LOC_WINEXIT
   from same window.  Detect first event based on window and xbutton values
   and set flag to ignore 2nd to avoid confusion on help image updating.

   Extraneous LOC_WINEXIT event is generated whenever popup window is
   put up, with same parent window for both event and popup.  It is followed
   by a LOC_WINENTER back in to same window again, with the exceptional
   xbutton value; do NOT deem this one invalid since there is no accompanying
   exceptional LOC_WINEXIT to follow.
*/
 last_winenter_invalid=(window!=mouse_win && 
                        event->ie_xevent->xbutton.state == 2);
 if (event_debug)
  printf("ignore next winexit: %d\n",last_winenter_invalid);
 return (last_winenter_invalid);
}

/********************  IGNORE_THIS_WINEXIT()  *******************************/

ignore_this_winexit()
{
  int ignore;

  ignore = last_winenter_invalid;
  last_winenter_invalid = FALSE;
  return(ignore);
}

/************************  CURSOR_IN_PANEL()  ********************************/

cursor_in_Panel()
{
/* return TRUE if cursor is in op_panel, menu_panel, or setup_panel; else,
   return FALSE.
*/
 return ( cursor_in_ctl_panel ||
          cursor_in_op_panel || 
          cursor_in_setup_panel  || 
          cursor_in_panel_menu ||
          cursor_in_menu_panel);
}
 
/*************************  SET_SERIAL_FEED_DEBUG()  *************************/
 
set_serial_feed_debug(int value)
{
 Serial_feed_debug=value;
}
 
/***********************  SERIAL_FEED_DEBUG()  *******************************/
 
serial_feed_debug()
{
 return Serial_feed_debug;
}


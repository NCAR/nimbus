#include <stdio.h>
#include <xview/xview.h>
#include <xview/canvas.h>
#include <xview/panel.h>
#include <xview/cms.h>

extern int *len_raw;     
extern char *raw_list;  

/****************************  WINDS INCLUDES  ******************************/

#include <constants.h>
#include <xview.h>
#include <lengths.h>
#include <calib.h>
#include <help.h>
#include <ops.h>
#include <geom.h>
#include <header.h>
#include <display.h>
#include <proto.h>

/****************************  EXPORTABLE  ********************************/

Panel dialogue;              /* add'l PANEL subwindow */
Panel_item var_name,                      /* dialogue window items */
	   xminimum,
	   xmaximum,
	   var_units,
           sensor_type,
	   sensor_sn,
	   cond_sn,
	   amp_gain,
	   off_set,
	   amp_cutoff,
	   ads_addr,
           tech,
           set_pt,
           num_sx;
Panel_setting get_input_func();
Menu calib_menu;                /* menu */
int menu_selected=FALSE;      /* whether a menu selection has been made  yet */


/****************************  INIT_CALIB_PARMS()  ****************************/
init_calib_parms()
{
  static int first_time=TRUE;

  if (first_time) {
    data_indx=0;
    (void)sprintf(set_pt_string,"\0");
    (void)sprintf(num_sex_string,"10");
    (void)sprintf(new_units,DEFAULT_UNITS);
    bottom=DEFAULT_BOT; 
    top=DEFAULT_TOP;
    num_sex=DEFAULT_SEX;      
    first_time=FALSE;
    }

  init_hilite_parms(STATS);
  init_hilite_parms(PLOTS);
  init_hilite_parms(DIALOGUE);

}

/**********************  REDRAW_CALIB()  ***********************************/
void
redraw_calib(Canvas cvas, Xv_Window pwindow, Rectlist *area)
/* catch the REPAINT signal (window needs repainting).  Set flags to refresh
   window, update help images, and set keyboard focus as needed.
*/
{
  refresh_calib();
}

/*****************************************************************************/
ResizeCalibWindows()
{
  xv_destroy_safe(canvas[STATS]);
  xv_destroy_safe(canvas[PLOTS]);
  xv_destroy_safe(dialogue);
  make_calib_windows();
  refresh_calib();
}

/*************************  MAKE_CALIB_WINDOWS()  ****************************/
make_calib_windows()
{
  (void)sprintf(num_sex_string,"%d",DEFAULT_SEX);
  canvas[STATS] = XSetAttr(GetBaseFrame(),CANVAS,
		XV_SHOW, FALSE,
        	CANVAS_REPAINT_PROC, redraw_calib,
		WIN_CMS, cms,
		XV_X,		0,
		XV_Y,		0,
		XV_WIDTH,
		GetBaseFrameWidth()/2,
		XV_HEIGHT,display_height,
		NULL);

  paint_window[STATS]=canvas_paint_window(canvas[STATS]);
  pw_xid[STATS]=(Drawable)GetWindowXID(paint_window[STATS]);
  (void)XSetAttr(paint_window[STATS],
		WIN_EVENT_PROC, event_proc,
#include <event_attr.h>
		NULL);
  set_cursor_tracking(STATS);

  set_geom_parms(PLOTS,GEOM_QUAD);
  canvas[PLOTS] = XSetAttr(GetBaseFrame(),CANVAS,
		XV_SHOW, FALSE,
        	CANVAS_REPAINT_PROC, redraw_calib,
		WIN_CMS, cms,
		WIN_RIGHT_OF,canvas[STATS],
		XV_WIDTH,GetBaseFrameWidth()/2,
		XV_HEIGHT, window_height[PLOTS],
		NULL);

  paint_window[PLOTS]=canvas_paint_window(canvas[PLOTS]);
  pw_xid[PLOTS]=(Drawable)GetWindowXID(paint_window[PLOTS]);
  (void)XSetAttr(paint_window[PLOTS],
		WIN_EVENT_PROC, event_proc,
#include <event_attr.h>
		NULL);

  dialogue= XSetAttr(GetBaseFrame(),PANEL,
		PANEL_BACKGROUND_PROC, panel_background_proc,
         	PANEL_EVENT_PROC, panel_event_proc,
	        PANEL_REPAINT_PROC, panel_repaint,
         	PANEL_ACCEPT_KEYSTROKE, TRUE,
                XV_HELP_DATA,"WINDS:CALIB_DIALOGUE_HELP",
               	XV_FONT, GetStandardFont(),
 		WIN_BELOW, 	canvas[PLOTS],
		WIN_RIGHT_OF,   canvas[STATS],
		XV_WIDTH,	GetBaseFrameWidth()/2,
		XV_HEIGHT, display_height-window_height[PLOTS], 
		PANEL_ITEM_Y_GAP,7, 
		NULL);

  paint_window[DIALOGUE]=canvas_paint_window(dialogue);
  XSetAttr(paint_window[DIALOGUE],
		WIN_EVENT_PROC, event_proc,
#include <event_attr.h>
		NULL);

  make_calib_items();

  SetCalibDisplayFont();
}

static int CalibWindowsMade=FALSE;

GetCalibWindowsMade()
{
  return CalibWindowsMade;
}

/****************************  SHOW_CALIB()  ********************************/
show_calib()
{

/* initialization */

  get_history();

  if (!CalibWindowsMade) {
    make_calib_windows();
    plot_type[PLOTS]=CALIBRATION;
    CalibWindowsMade=TRUE;
   }

  SetCalibDisplayFont();
  (void)XSetAttr(GetBaseFrame(), XV_LABEL,"Calibration",NULL);
  XSetAttr(canvas[STATS],XV_SHOW,TRUE,NULL);
  XSetAttr(canvas[PLOTS],XV_SHOW,TRUE,NULL);
  XSetAttr(dialogue,XV_SHOW,TRUE,NULL);
  refresh_calib();
}

/*********************** HIDE_CALIB() **********************************/
HideCalibDisplay()
{
  XSetAttr(canvas[STATS],XV_SHOW,FALSE,NULL);
  XSetAttr(canvas[PLOTS],XV_SHOW,FALSE,NULL);
  XSetAttr(dialogue,XV_SHOW,FALSE,NULL);
}

/*********************** REFRESH_CALIB() **********************************/
refresh_calib()
{
  SetSetptsDims();
  stats_header();
  clear_calcoes();
  relist_points();
  init_plots();
  if (menu_selected)
    draw_bkgnd(PLOTS);
  redraw_calib_curve();
  repaint_set[PLOTS]=TRUE;
  need_rehilite[STATS]=TRUE;
}

/***************************  INIT_PLOTS()  **********************************/

init_plots()
/* perform initialization needed to plot stuff */
{
  num_of_traces[PLOTS]=1;
  (*FillArea[GetDrawType()])(pw_xid[PLOTS],0,0,
           window_width[PLOTS],XGetAttr(canvas[PLOTS],XV_HEIGHT),0);
}

/**********************  MAKE_CALIB_ITEMS()  *********************************/

make_calib_items()
{

(void)XSetAttr(dialogue,PANEL_MESSAGE,
			PANEL_LABEL_STRING,"Variable name:",
			NULL);
var_name = XSetAttr(dialogue,PANEL_MESSAGE,
                        PANEL_CLIENT_DATA, VARNAME,
			NULL);
xminimum=XSetAttr(dialogue,PANEL_TEXT,
                        PANEL_CLIENT_DATA, XMINIMUM,
			PANEL_NOTIFY_LEVEL, PANEL_SPECIFIED,
			PANEL_NOTIFY_STRING,NOTIFY_STRING,
    	                PANEL_NOTIFY_PROC, get_input_func,
			PANEL_LABEL_STRING,"Range Minimum:",
			PANEL_VALUE,"",
			NULL);
xmaximum=XSetAttr(dialogue,PANEL_TEXT,
                        PANEL_CLIENT_DATA, XMAXIMUM,
			PANEL_NOTIFY_LEVEL, PANEL_SPECIFIED,
			PANEL_NOTIFY_STRING,NOTIFY_STRING,
    	                PANEL_NOTIFY_PROC, get_input_func,
			PANEL_LABEL_STRING,"Range Maximum:",
			PANEL_VALUE,"",
			NULL);
var_units=XSetAttr(dialogue,PANEL_TEXT,
                        PANEL_CLIENT_DATA, VAR_UNITS,
			PANEL_NOTIFY_LEVEL, PANEL_SPECIFIED,
			PANEL_NOTIFY_STRING,NOTIFY_STRING,
    	                PANEL_NOTIFY_PROC, get_input_func,
			PANEL_LABEL_STRING,"Variable Units:",
			PANEL_VALUE,"",
			NULL);
sensor_type=XSetAttr(dialogue,PANEL_TEXT,
                        PANEL_CLIENT_DATA, SENSOR_TYPE,
			PANEL_NOTIFY_LEVEL, PANEL_SPECIFIED,
			PANEL_NOTIFY_STRING,NOTIFY_STRING,
    	                PANEL_NOTIFY_PROC, get_input_func,
			PANEL_LABEL_STRING,"Sensor type:",
			PANEL_VALUE,"",
			NULL);
sensor_sn=XSetAttr(dialogue,PANEL_TEXT,
                        PANEL_CLIENT_DATA,  SENSOR_SN,
			PANEL_NOTIFY_LEVEL, PANEL_SPECIFIED,
			PANEL_NOTIFY_STRING,NOTIFY_STRING,
    	                PANEL_NOTIFY_PROC, get_input_func,
			PANEL_LABEL_STRING,"Sensor serial number:",
			PANEL_VALUE,"",
			NULL);
cond_sn=XSetAttr(dialogue,PANEL_TEXT,
			PANEL_NOTIFY_LEVEL, PANEL_SPECIFIED,
			PANEL_NOTIFY_STRING,NOTIFY_STRING,
    	                PANEL_NOTIFY_PROC, get_input_func,
                        PANEL_CLIENT_DATA,  COND_SN,
			PANEL_LABEL_STRING,"Conditioner serial number and type:",
			PANEL_VALUE,"",
			NULL);
ads_addr=XSetAttr(dialogue,PANEL_TEXT,
                        PANEL_CLIENT_DATA, ADS_ADDRESS, 
			PANEL_NOTIFY_LEVEL, PANEL_SPECIFIED,
			PANEL_NOTIFY_STRING,NOTIFY_STRING,
    	                PANEL_NOTIFY_PROC, get_input_func,
			PANEL_LABEL_STRING,"ADS Channel:",
			PANEL_VALUE,"",
			NULL);
amp_gain=XSetAttr(dialogue,PANEL_TEXT,
                        PANEL_CLIENT_DATA, AMP_GAIN,
			PANEL_NOTIFY_LEVEL, PANEL_SPECIFIED,
			PANEL_NOTIFY_STRING,NOTIFY_STRING,
    	                PANEL_NOTIFY_PROC, get_input_func,
			PANEL_LABEL_STRING,"Amplifier gain:",
			PANEL_VALUE,"",
			NULL);
off_set=XSetAttr(dialogue,PANEL_TEXT,
                        PANEL_CLIENT_DATA, OFF_SET,
			PANEL_NOTIFY_LEVEL, PANEL_SPECIFIED,
			PANEL_NOTIFY_STRING,NOTIFY_STRING,
    	                PANEL_NOTIFY_PROC, get_input_func,
			PANEL_LABEL_STRING,"Offset:",
			PANEL_VALUE,"no",
			NULL);
amp_cutoff=XSetAttr(dialogue,PANEL_TEXT,
                        PANEL_CLIENT_DATA, AMP_CUT_OFF,
			PANEL_NOTIFY_LEVEL, PANEL_SPECIFIED,
			PANEL_NOTIFY_STRING,NOTIFY_STRING,
    	                PANEL_NOTIFY_PROC, get_input_func,
			PANEL_LABEL_STRING,"Amplifier cut-off frequency:",
			PANEL_VALUE,"",
			NULL);
tech=XSetAttr(dialogue,PANEL_TEXT,
                        PANEL_CLIENT_DATA, TECHNICIAN,
			PANEL_NOTIFY_LEVEL, PANEL_SPECIFIED,
			PANEL_NOTIFY_STRING,NOTIFY_STRING,
    	                PANEL_NOTIFY_PROC, get_input_func,
			PANEL_LABEL_STRING,"Calibration done by:",
			PANEL_VALUE,"",
			NULL);
num_sx=XSetAttr(dialogue,PANEL_TEXT,
                        PANEL_CLIENT_DATA, ACCUM_INTRVL,
			PANEL_NOTIFY_LEVEL, PANEL_SPECIFIED,
			PANEL_NOTIFY_STRING,NOTIFY_STRING,
    	                PANEL_NOTIFY_PROC, get_input_func,
			PANEL_LABEL_STRING,"Accumulation Interval (sec):",
			PANEL_VALUE,num_sex_string,
			NULL);
set_pt=XSetAttr(dialogue,PANEL_TEXT,
                        PANEL_CLIENT_DATA, SET_POINT,
			PANEL_NOTIFY_LEVEL, PANEL_SPECIFIED,
			PANEL_NOTIFY_STRING,NOTIFY_STRING,
    	                PANEL_NOTIFY_PROC, get_input_func,
			PANEL_LABEL_STRING,"Set Point:",
			NULL);
}

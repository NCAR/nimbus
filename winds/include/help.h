/* help.h -- parameters describing the help images
*/

#define ICON_WIDTH_     64
#define ICON_HEIGHT_    64
#define EXTENSION       16 /* bottom of 64x64 icon for aesthetics */
#define BUTTON_WIDTH    ICON_WIDTH_
#define BUTTON_HEIGHT   (ICON_HEIGHT_+EXTENSION)
#define NUM_BUTTONS 3  /* how many mouse-device buttons */

/* track ball help images -- associated frame label strings defined in same
   ordinal position, in event_ops.c.  As well, update help-info labels to
   correspond with these, as defined in events.c.
*/

#define NUM_AREAS               32
#define NO_AREA                 0
#define CHG_TRACE_AREA          1
#define CHG_INDEP_AREA          2
#define ADD_TRACE_AREA          3
#define CHG_Y_SCALE_AREA        4
#define CHG_X_SCALE_AREA        5
#define PLOT_FRAME_AREA         6
#define PMS2D_AREA              7
#define ADSPOP_AREA        	8     
#define FLTFRAME_AREA        	9   
#define CALIB_SETPT_AREA        10
#define CALIB_DIALOGUE_AREA     11
#define CALIB_PLOT_AREA         12
#define CHG_LISTEE_AREA         13
#define CTL_AREA                14
#define CHG_FIXEE_AREA          15
#define MENU_AREA               16
#define NOTICE_AREA             17
#define CHG_VECTOR_SCALE_AREA   18
#define NEW_SETUP_AREA        	19
#define OP_PANEL_AREA        	20
#define CHG_TIME_SCALE_AREA     21
#define STATS_MENU_AREA        	22
#define ATTRS_MENU_AREA        	23
#define PRINTERS_MENU_AREA      24
#define PRINTCONFIG_MENU_AREA   25
#define PLOTSPEX_MENU_AREA      26
#define FOLDERS_MENU_AREA       27
#define PMS1D_ACCUM_AREA        28
#define PMS1D_YMAX_AREA         29
#define PMS1D_YMIN_AREA         30
#define LANDMARK_OPS_AREA	31
#define UNKNOWN_AREA		NUM_AREAS

/* on-line help: event_proc looks at event_id(event), which for ACTION_HELP
event is 0 
*/
#define EVENT_HELP_KEY 		0  
/* 
little known developer's aids: keyboard control of debug statements
*/
#define EVENT_DEBUG_KEY 	22 /* control-v to toggle event debugs */
#define HILITE_DEBUG_KEY 	8  /* control-h to toggle hilite debugs */
#define SERIAL_FEED_DEBUG_KEY 	19 /* control-s to toggle serial feed debugs */
/*
still more secrets: toggle access to various restricted operations (ctl-w)
*/
#define WINDS_EXPERT_KEY	23
/* 
different process types -- used in NON-arbitrary numbering scheme in
make_alt_menu()
*/

#define MAX_TYPE_LNGTH          15

/* values of variable plot_type in XY module; sub-category under HELPIMAGE_XY_PLOT
   below.

   Right Key functions remain the same, but track ball functions vary
   as function of cursor position for these.
*/
#define NUM_XY_TYPES            5    /* number of xy-plot types */
#define TIME_SERIES             0
#define TRACK_PLOT              1
#define SKEW_T                  2
#define SOUNDING                3
#define XY                      4

/* other values for plot_type, grouped as choices on same level as xy-plot
   category
*/
#define LISTS                   5
#define FIXED_FORMAT            6
#define CALIBRATION             7

#define NUM_2D_TYPES		3
#define PMS2D_STANDARD		0
#define PMS2D_GREYSCALE		1
#define PMS2D_HVPS		2
/*
video, etc.
*/
#define NUM_ETC_TYPES		1
#define VIDEO			0
/* 
track ball help images -- argument 'process_type' in
calls to chg_help_image() and value of window_type[] array elements.
    
Functions (and ergo OpPanel buttons and help images) vary for these.  
Help images set on detection of LOC_WINENTER event.
*/

/* first level menu choices on alternates: Display Types
*/
#define DISPTYPE_XYPLOT         0
#define DISPTYPE_PMS1D          1
#define DISPTYPE_PMS2D          2
#define DISPTYPE_LISTS          3
#define DISPTYPE_FIXEDFORMAT    4
#define DISPTYPE_ETC         	5 

/* miscellaneous window types for which functions vary. Some are simply 
different types of windows (e.g., calib and control panel) while others are
subsets of above display types with different op-panel button sets.
*/
#define WTYPE_CALIBDISP      6
#define WTYPE_SKEWT          7
#define WTYPE_CONTROLPANEL   8

/* additional values for special conditions ; argument to chg_help_images() 
   only
*/
#define HELPIMAGE_FRAME_BORDER    9	/* cursor on frame border */
#define HELPIMAGE_NOWHERE         10 	/* cursor in unknown location */
#define HELPIMAGE_MENU_SHOW      11 		/* menu is up */
#define HELPIMAGE_POPUP_SHOW     12 		/* popup is up */
#define HELPIMAGE_NOTICE_SHOW    13 		/* alert is up */
#define HELPIMAGE_ADSPOP_AREA    14 		/* panels with text and button items */
#define HELPIMAGE_ADSEVT_AREA    15 		/* panels with button items only */
#define HELPIMAGE_FLTFRAME_AREA  16 		/* panels with text items only */
#define HELPIMAGE_LAST_AREA      17		/* some temporary modification is 
					   completed; restore last area */
#define HELPIMAGE_PENDING_IMAGE  18          /* some temporary modification is not
					   yet done, but we know that as soon
					   as it is that accompanying argument
					   in call to chg_help_images() is the
					   value to use on subsequent call with
					   HELPIMAGE_LAST_AREA */ 
#define HELPIMAGE_SETUP_AREA     19		/* new setup panel */
#define HELPIMAGE_OP_PANEL_AREA  20		/* op panel */
#define HELPIMAGE_STATS_MENU_AREA 21		/* panel menus */
#define HELPIMAGE_ATTRS_MENU_AREA 22		
#define HELPIMAGE_PRINT_MENU_AREA 23		
#define HELPIMAGE_PRINTCONFIG_MENU_AREA 24		
#define HELPIMAGE_PLOTSPEX_MENU_AREA 25		
#define HELPIMAGE_FOLDERS_MENU_AREA 26		

#define normal_tb_area(area) (area!=MENU_AREA  && \
      area!=NOTICE_AREA)

/* display.h -- define display windows
*/

/* 
values for clear_name_label()  
*/
#define REFRESH         0
#define ERASE           1
/* 
length of time stamp string
*/
#define TIME_STAMP_LEN strlen("hh:mm:ss")

#define MAX_DISPLAY_TYPES    6

#define MAX_DISPLAY_VARS        24  /* must be at least as large as largest of 
				MAX_LISTEES, MAX_FIXEES, MAX_TRACES, and 
				MAX_SKEWT_TRACES */

/*
modes of variables
*/
#ifndef BITS
#define BITS 0x80000000
#endif
#define VOLTAGE 0x40000000
#define RAW 0x20000000
#define DERIVED 0x10000000
#define TIME_AXIS 0x00000000
#define STRIP_MODE 0xF0000000
#define STRIP_INDEX 0x0FFFFFFF
/* 
data display quadrants
*/
#define NUM_QUADRANTS 			5  /* 5 display quadrants */
#define UPPER_LEFT      		0       
#define UPPER_RIGHT     		1
#define LOWER_RIGHT     		2
#define LOWER_LEFT      		3
#define NUM_DISPLAY_QUADRANTS 		4
/* 
calibration windows 
*/
#define NUM_CALIB_WINDOWS 		3  /* 2 calib canvasses and 1 panel */

/* NOTE: keep PLOTS equal to one more than highest value above, since 
   there are a total of five xy-display quadrants (PLOTS being the fifth),
   and many display state parms are dimensioned to and indexed by that number.
*/
#define PLOTS 				LOWER_LEFT + 1
#define STATS 				PLOTS + 1
#define DIALOGUE 			PLOTS + 2

/* control panel
*/
#define NUM_CONTROL_PANEL_WINDOWS 	2  /* time window and help image */
#define TIME_AND_PROJECT   		NUM_DISPLAY_QUADRANTS+NUM_CALIB_WINDOWS
#define HELP_IMAGE   			NUM_DISPLAY_QUADRANTS + 4

/* op panels
*/
#define NUM_OP_PANELS			NUM_DISPLAY_QUADRANTS
#define OP_UPPER_LEFT			NUM_DISPLAY_QUADRANTS + 5
#define OP_UPPER_RIGHT			NUM_DISPLAY_QUADRANTS + 6
#define OP_LOWER_RIGHT			NUM_DISPLAY_QUADRANTS + 7
#define OP_LOWER_LEFT			NUM_DISPLAY_QUADRANTS + 8
#define FIRST_OP_QUADRANT		OP_UPPER_LEFT      /*  (9)  */

/* new setup panels
*/
#define NUM_SETUP_PANELS		NUM_DISPLAY_QUADRANTS
#define SETUP_UPPER_LEFT		NUM_DISPLAY_QUADRANTS + 9
#define SETUP_UPPER_RIGHT		NUM_DISPLAY_QUADRANTS + 10
#define SETUP_LOWER_RIGHT		NUM_DISPLAY_QUADRANTS + 11
#define SETUP_LOWER_LEFT		NUM_DISPLAY_QUADRANTS + 12
#define FIRST_SETUP_QUADRANT		SETUP_UPPER_LEFT   /*  (13)  */

/* new setup menu panels
*/
#define NUM_SETUP_MENU_PANELS		NUM_DISPLAY_QUADRANTS
#define SETUP_MENU_UPPER_LEFT		NUM_DISPLAY_QUADRANTS + 13
#define SETUP_MENU_UPPER_RIGHT		NUM_DISPLAY_QUADRANTS + 14
#define SETUP_MENU_LOWER_RIGHT		NUM_DISPLAY_QUADRANTS + 15
#define SETUP_MENU_LOWER_LEFT		NUM_DISPLAY_QUADRANTS + 16
#define FIRST_SETUP_MENU_QUADRANT	SETUP_MENU_UPPER_LEFT

/* control panel
*/
#define NUM_CTL_PANELS			1	
#define CTL_PANEL			NUM_DISPLAY_QUADRANTS + 17

/* copyright window
*/
#define NUM_COPYRITE_WINDOWS		1	
#define COPYRIGHT			NUM_DISPLAY_QUADRANTS + 18

/* panel-type menus
*/
#define NUM_PANEL_MENUS	6

#define STATS_PANEL_MENU		NUM_DISPLAY_QUADRANTS + 19
#define ATTRS_PANEL_MENU		NUM_DISPLAY_QUADRANTS + 20
#define PRINT_PANEL_MENU		NUM_DISPLAY_QUADRANTS + 21
#define PRINTCONFIG_PANEL_MENU		NUM_DISPLAY_QUADRANTS + 22
#define PLOTSPEX_PANEL_MENU		NUM_DISPLAY_QUADRANTS + 23
#define FOLDERS_PANEL_MENU		NUM_DISPLAY_QUADRANTS + 24
#define FIRST_PANEL_MENU         	STATS_PANEL_MENU
		
#define NUM_SPECIAL_MENUS 		1
#define INTERIORPANEL			NUM_DISPLAY_QUADRANTS + 25

/* total # of windows: deduct one since PLOTS is counted twice, once as a
   display quadrant, once as a calib window.  
*/
#define NUM_WINDOWS 			NUM_QUADRANTS \
					+NUM_CTL_PANELS \
					+NUM_OP_PANELS \
					+NUM_SETUP_PANELS \
					+NUM_SETUP_MENU_PANELS \
					+NUM_CALIB_WINDOWS \
					+NUM_CONTROL_PANEL_WINDOWS \
					+NUM_COPYRITE_WINDOWS \
					+NUM_PANEL_MENUS \
					+NUM_SPECIAL_MENUS \
					- 1

/* margin to deduct as margin from image print dimensions to preclude
   Xview error on XGetImage()
*/
#define IMAGE_MARGIN	2

/* values to describe actions to take at "end of session" request
*/
#define EXIT_HOST_ONLY			0
#define EXIT_ALL			1
#define SHUTDOWN			2
#define DO_NOT_EXIT			3
/*
types of drawing devices -- x11, PostScript, and NULL
*/
#define NUM_DRAWING_TYPES 3
#define X11	0
#define PS	1

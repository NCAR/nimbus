/* flat_panel.h -- parameters associated with data sent out over 
   serial line to a flat panel display
*/

#define MAX_FLAT_PANEL_PARMS 11	/* max # of entries in FLAT_PANEL_PARMS file */

#define MAX_PTRS	2	/* max # of pointers maintained */

#define FP_ROW_LNGTH   20	/* max length of each row going to display */
#define FP_ROWS	4	        /* number of rows going to display */

/* two different types of flat panel displays: LCD and FLIP
*/
#define HOME_LNGTH  4	/* length of 'home' command */
/*
strings that indicate parcel tracking 
*/
#define BEARING1        "bearing_1"
#define BEARING2        "bearing_2"
#define ALTITUDE1       "altitude_1"
#define ALTITUDE2       "altitude_2"
#define DISTANCE1       "distance_1"
#define DISTANCE2       "distance_2"
/* 
string for special parameter time
*/
#define TIME_STRING     "TIME" 

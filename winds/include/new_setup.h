/* new_setup.h -- operations and windows involved in the new setup operation
*/
/* all possible setup window types -- 1d and 2d are special, since they need
   no special selections made before appearing
*/
#define NUM_SETUP_TYPES		(NUM_XY_TYPES+2) /*add 2 for LISTS and FIXED */
#define SETUP_1D		NUM_SETUP_TYPES
#define SETUP_2D_STD		(NUM_SETUP_TYPES+1)
#define SETUP_2D_GREY		(NUM_SETUP_TYPES+2)
#define SETUP_2D_HVPS		(NUM_SETUP_TYPES+3)
#define TOTAL_SETUP_TYPES	(NUM_SETUP_TYPES+4)

/* # of "variable types" in each setup type 
*/
#define TIME_SERIES_VARTYPES 	1	/* one trace */
#define TRKPLT_VARTYPES 	2	/* dependent, independent */
#define SKEWT_VARTYPES 		2	/* dependent, independent */
#define SOUNDING_VARTYPES 	2	/* dependent, independent */
#define XY_VARTYPES 		2	/* dependent, independent */
#define LIST_VARTYPES 		1	/* one list variable */
#define FIXED_VARTYPES 		1	/* one fixed format variable */
#define SETUP_MAX_VARTYPES 	2	/* maximum # of variable types in any
					   of above */
/* for each setup type (i.e., time series, track plot, etc.), number each of its
   variable types (i.e., {dependent, independent}, {lat,lon}, etc.) from 0 since
   these correspond to PANEL_VALUES assigned to the vartype_button choices 
*/
#define SETUP_DEPVAR		0	
#define SETUP_INDEPVAR		1	
/* 
configuration feedback
*/
#define MAX_SETUP_TYPE_LNGTH	50
#define NUM_MESSAGES 		2
#define SETUP_TYPE		0	/* which setup type */
#define SETUP_SELNS_MADE      	1	/* how many selections are made in 
					   current variable type */

/* maximums -- not to be exceeded in selections in non-exclusive menus
*/
#define MAX_INDEPVARS		1
#define MAX_SKEWT_TRACES	2

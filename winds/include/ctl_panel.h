/* ctl_panel.h -- operation panels associated with control panel window
*/

/* layout
*/
#define CTL_ITEMS_XGAP		4	/* pixels between related items */
#define CTL_BUTTON_XGAP		10	/* pixels between non-related items */
#define STATS_INTRVL_LNGTH	5
#define CTL_TOPOPS_Y		4
#define CTL_TOPOPS_X		4
#define CTL_VAROPS_Y		45
#define CTL_VAROPS_X		4

/* two different control panels, one for displays, one for calibration
*/
#define OPS_DISPLAYS		0
#define OPS_CALIB		1

/* display operations
*/
#define NUM_DISPLAY_OPS		41	
#define OP_CTL_QUIT		0
#define OP_CTL_INPUT		1
#define OP_CTL_FRZ_INPUT	2
#define OP_CTL_RESET_INPUT	3
#define OP_CTL_UPDATE_RATE	4
#define OP_CTL_STATS		5
#define OP_CTL_CALIB		6
#define OP_CTL_EVENTS		7
#define OP_CTL_ADSCMD		8	/* Available and unused	*/
#define OP_CTL_DEFAULTS		9
#define OP_CTL_VAROPS 		10
#define OP_CTL_PARCELS 		11
#define OP_CTL_INIT_PARCEL 	12
#define OP_CTL_SHOW_PARCEL 	13
#define OP_CTL_EVT 		14
#define OP_CTL_ETC 		15
#define OP_CTL_HEX 		16
#define OP_CTL_PRINTING 	17
#define OP_CTL_SUBSET 		18
#define OP_CTL_GEOM 		19
#define OP_CTL_INPUT_FRAME	20
#define OP_CTL_START_TIME	21
#define OP_CTL_FRAME_JUMP	22
#define OP_CTL_UPDATE_INTRVL	23
#define OP_CTL_STATS_INTRVL	24
#define OP_CTL_STATS_UNITS 	25
#define OP_CTL_STATS_PRINT	26
#define OP_CTL_STATS_SELECT	27
#define OP_CTL_THINKING		28
#define OP_CTL_ATTRS		29
#define OP_CTL_INPUT_BCAST	30
#define OP_CTL_BKGND_COLOR	31
#define OP_CTL_CONFIG		32
#define OP_CTL_PRINTCONFIG	33
#define OP_CTL_SETUPS      	34
#define OP_CTL_FOLDERS      	35
#define OP_CTL_MULTIPRINT      	36
#define OP_CTL_SELECT_PRINTSETUP 37
#define OP_CTL_SELECT_PRINTTIME 38
#define OP_CTL_CANCEL_MULTIPRINT 39
#define OP_CTL_EXECUTE_MULTIPRINT 40
/* 
possible values of choice buttons
*/
#define VAROP_HEX		0
#define VAROP_OCTAL		1	
#define VAROP_COLOR		0
#define VAROP_BW		1	
#define BKGND_BLUE		0
#define BKGND_BLACK		1	
#define VAROP_FWD		1
#define VAROP_BACK		2	
#define SHOW_PARCEL1		0	
#define SHOW_PARCEL2		1	
#define RATE_REALTIME		0
#define RATE_WARP2		1
#define RATE_WARP4		2
#define RATE_WARP10		3
#define FRAME_INTRVL_120	0
#define FRAME_INTRVL_60		1
#define FRAME_INTRVL_30		2
#define FRAME_INTRVL_15		3
#define FRAME_INTRVL_10		4
#define FRAME_INTRVL_5		5
#define FRAME_INTRVL_1		6
#define FRAME_FWD		1
#define FRAME_BKWD		2
#define BCAST_NONE		0
#define BCAST_RAW		1
#define BCAST_CALIB		2
#define BCAST_BOTH		3

/* two rows of calib ops, in choice items
*/
#define NUM_CALIB_OPS		2
#define OP_CTL_CALIBOPS0	NUM_DISPLAY_OPS+0
#define OP_CTL_CALIBOPS1	NUM_DISPLAY_OPS+1

/* operations on top row of calib ops
*/
#define OP_CALIB_DISPLAYS	1
#define OP_CALIB_SENSOR_IN	2
#define OP_CALIB_REGRESS	4
#define OP_CALIB_REDO		8
#define OP_CALIB_SCROLL_UP	16
#define OP_CALIB_SCROLL_DOWN	32

/* operations on bottom row of calib ops
*/
#define OP_CALIB_PRINT_STATS	1
#define OP_CALIB_PRINT_PLOT 	2
#define OP_CALIB_PRINT_HIST	4
#define OP_CALIB_UPDATE_HIST	8
#define OP_CALIB_QUIT		16

/* total number of panel items
*/
#define NUM_CTL_OPS		NUM_DISPLAY_OPS+NUM_CALIB_OPS

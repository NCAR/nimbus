/* op_panels.h -- operation panels associated with each display window
*/

/* layout
*/
#define OP_PANEL_HEIGHT		64
#define OP_ROW_GAP		9
#define SPEX_BTN_LEFTX		40	/* move away from left so menus don't
					have problem staying up */
#define SPEX_BTN_RIGHTX		4	
#define OP_ITEMS_XGAP		1
#define VAROP_ROWY		38	/* y-coord of 2nd row items, the
					   contents of which will vary */
/* 
general operation buttons.  Defines of same value occupy same ordinal 
position in genop row, but in a different window type, thus having 
different semantics and operations.
*/
#define OP_SAVE			1
#define OP_FREEZE		2
#define OP_CHG_DISP		4
#define OP_PRINT		8
#define OP_HISTORY		16
#define OP_PROBES		16
#define OP_PMS1D_DATATYPE	32
#define OP_HILITED_VAR		32
#define OP_SKEW_RANGES		32
#define OP_LIST_RATE		32
#define OP_PMS1D_SCALES		64
#define OP_AUTOSCL		64
#define OP_ETC    		128
/* 
other sub-buttons which may be exclusive or'd with top row values
*/
#define OP_OP_MSG		256
/* 
sub-menu operations from OP_ETC
*/
#define OP_ETC_LMRKS		(OP_ETC+1)
#define OP_ETC_ADD_LMRK		(OP_ETC+2)
#define OP_ETC_GEOM		(OP_ETC+3)
#define OP_ETC_VECTOR		(OP_ETC+4)
#define OP_ETC_SCALES		(OP_ETC+5)


/* button strings for general ops
*/
#define GENOPS_XYPLOT "Save","Frz","Disp","Prt","Hist","Op","Autsc","Etc"
#define GENOPS_PMS1D  "Save","Frz","Disp","Prt","Prb","Data","Scl","Etc"
#define GENOPS_PMS2D  "Save","Frz","Disp","Prt","Prb","Updates","Etc"
#define GENOPS_LISTS  "Save","Frz","Disp","Prt","Page","Updates","Etc"
#define GENOPS_FIXED  "Save","Frz","Disp","Prt","Etc"
#define GENOPS_ETC    "Save","Frz","Disp","Prt","Channel","Etc"
#define GENOPS_CALIB  "","","","","","","",""
#define GENOPS_SKEWT  "Save","Frz","Disp","Prt","Hist","Ranges","Etc"

/* other label strings and choice strings
*/
#define HILITE_OPS 	"Fix/Float","Mode","Def. Rng"
#define ANALYSIS_HILITE_OPS 	"Fix/Float","Def. Rng"
#define HISTORY_OPS	"ShowHistory","ClearHistory"
#define DATATYPE_LABEL  "Type:"
#define DATATYPE_STRING "Counts","Concentration"
#define UNITS_LABEL 	"Units:"
#define UNITS_STRINGS   "Liters","CC"
#define XAXIS_LABEL	"X-axis:"
#define XAXIS_STRINGS	"Linear","Log"
#define YAXIS_LABEL	"Y-axis:"
#define YAXIS_STRINGS	"Linear","Log"
#define LMRK_OPS	"Add","Show"
#define GEOM_OPS	"Quad","Strip","Stack","Full"
#define NO_PRINTER_MSG  " Sorry, No Printer Available..."
#define PRINT_MISMATCH_MSG  " Mismatch...see Printing v. Config-Printers menus..."
#define NO_MORE_SETUPS  " Sorry, No More Saves Allowed..."
#define NO_2D_PROBES    " No Probes of this type available..."
#define EDITSDISABLED   " Sorry, this folder has password protection"
#define FOLDERDELETED   " Folder has been deleted -- select new one"
#define NOVIDEO_MSG     " No Video signal detected on requested channel"

/* needed constants for layout
*/
#define NORMAL_TEXT_X		5
#define NORMAL_TEXT_LNGTH	32
#define GEOM_X			3
#define GEOM_STRING		"Geometry"
#define INDEPVAR_STRING		"X","Y"
#define INDEPVAR_LABEL_STRING	"IndepVar:"
#define SCALE_STRING		"Scales"
#define SCALE_TYPE_LABEL_STRING	"Type:"
#define SCALE_TYPE_STRING	"Normal","Same","Isolate"
#define NORMAL_SCALE		0
#define SAME_SCALE		1
#define ISOLATE_SCALE		2
#define NUM_VECTOR_ITEMS	2
#define VECTOR_STRING		"Vectors"
#define X_COMP			0
#define Y_COMP			1
#define COMPONENT_STRING	"Components"
#define COMP_X_STRING		"X-comp't"
#define COMP_Y_STRING		"Y-comp't"
#define X_COMP_X		5
#define VECTOR_SAME_COLORS	0
#define VECTOR_DIFF		1
#define INTRVL_STRING		"Intrvl:"
#define INTRVL_TEXT_LNGTH 	5
#define INTRVL_STRLEN		(strlen(INTRVL_STRING)+INTRVL_TEXT_LNGTH+1)
#define LMRK_STRING		"Landmarks"
#define TRK_TYPE_STRINGS	"Posn","Dist"

/* landmark options
*/
#define LMRK_ADD		1
#define LMRK_SHOW		2

/* track plot type options
*/
#define TRK_POSITION		0
#define TRK_DISTANCE		1

/* printer options
*/
#define OP_COLORPRINT		1
#define OP_BWPRINT		2

/* history options
*/
#define OP_SHOW_HIST		1
#define OP_CLEAR_HIST		2

/* hilited var options
*/
#define NUM_VAROPTS   		3
#define OP_TOGGLE_FIX		0
#define OP_TOGGLE_MODE		1
#define OP_DEFRNG		2

/* pms1d units options
*/
#define NUM_1D_DATATYPE_PARMS	2
#define PMS1D_DISP_CNTS		0
#define PMS1D_DISP_LITERS	1

/* pms1d scales options
*/
#define NUM_1D_SCALES_PARMS	2
#define PMS1D_DISP_XTYPE	0
#define PMS1D_DISP_YTYPE	1

/* ops.h -- describe operations WINDS can perform
*/

#define INDEPVAR                -2    /* flag for independent variable  */
#define INITIAL                 -1    /* initial redraw on xy plots */
#define NORMAL_UPDATE           0     /* normal update on xy plots */
#define CHG_Y_SCALE             1
#define CHG_X_SCALE             2
#define CHG_FLOATING_SCALE      3
#define CHG_FIXED_SCALE         4
#define CHG_TIME_SCALE          5
#define CHG_TRACE               6
#define ADD_TRACE               7
#define DEL_TRACE               8
#define CHG_INDEPVAR            9
#define CHG_LISTEE              10
#define STATS_SETUP             11
#define PMS1D_PERIOD            12
#define PMS1D_MINY              13
#define PMS1D_MAXY              14
#define CHG_HILITED             15
#define CHG_FIXEE               16
#define DEL_SETPT               17
#define DESCRIPTION             18    /* description for new plotspex entry */
#define RECONFIG                19    /* reconfigure window, diff't setup */
#define GET_NEW_ATTR            20    /* get plot attribute for calib window */
#define START_NOW               21    /* start refreshed data display w/o any
					 history -- w/history is INITIAL */    
#define CHG_VECTOR_SCALE        22    
#define NEW_SETUP_OPS           23    /* new setup window operations */
#define OP_PANEL_OPS            24    
#define CTL_PANEL_OPS           25    
#define ADD_LANDMARK		26
#define SKEWT_RANGES		27
#define INTERVAL		28
#define CHG_VECTOR_COMP		29
#define SET_LIST_RATE		30
#define PANEL_MENU_OPS		31
#define DELETE_LANDMARK		32
 
#define DEFAULT_DESCRIPTION     "cancel save"  /* used on null entry for
						   new setup operation */


#define ZOOM            0                        /* zoom operation  */
#define UNZOOM          1                        /* unzoom operation  */
#define ZOOM_VECTOR	2
#define UNZOOM_VECTOR	3


#define CLIX_INTERVAL_SEX 0       /* # seconds allowed for multiple clicks */
#define CLIX_INTERVAL_USEX 666666 /* # microseconds for multiple clicks */

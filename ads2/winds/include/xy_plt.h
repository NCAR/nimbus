/* 
xy_plt.h -- parameters governing operation and limits of xy-plot module
*/
#define NUM_DISPLAY_TYPES       8     /* number of update_display() functions */
#define MAX_TRACES              4     /* maximum # of traces per plot window */
#define MAX_SKEWT_TRACES        2     /* maximum # of traces, skew-t window */
#define MAX_PRINTEES            12    /* max. # listees printable --
  it is ASSUMED that this value is at least as large as MIN_LISTEES_PER_PAGE */
#define ALL_POINTS 0           /* flag for more_points() -- get all points */
#define NEW_POINTS 1           /* flag for more_points() -- get new points */
/* 
defaults
*/
#define DEFAULT_XAXIS_SECONDS 900  /* # seconds, default on time range axis */
#define DEFAULT_CLIP_CHK_INTERVAL 5 /* # seconds of interval between checks
                                       for autoscale need */
#define DEFAULT_TOLERANCE_FACTOR .10 /* percent of range within which the data
                                        is deemed to be clipping, thus the
                                        need for autoscale  */
#define DEFAULT_SEX_2_AVG 10 /* seconds of data to average when autoscaling */
#define DEFAULT_MINPRESS 100.0  /* minimum pressure on skew-t plots */
#define DEFAULT_MINTEMP -40.0   /* minimum temperature on skew-t plots */
/* 
skew-t
*/
#define MAX_Y_TIX 10            /* 10 possible y-axis labels, skew-t plots */
#define MAX_X_TIX 20            /* 20 possible x-axis labels, skew-t plots */
#define MAX_PRESS 1050.0        /* maximum pressure on skew-t plots */
#define DEGREES_C_PER_LOG 81.596  /* adjust to produce 45 degree isotherms */
#define SKEW_LINEWIDTH 2	/* width of skew-t data lines */
/* 
plot range labelling  
*/
#define TLABELS         4              /* number of time labels */
#define FIXED           0
#define FLOATING        1
#define AUTO_ADJUST     0
#define USER_ATTR       1
#define NUM_TIX         5
/* 
calibration
*/
#define HDR_X 10               /* left margin in stats window */
#define HDR_Y 20               /* top margin in stats window */
#define HDR_Y2 35              /* y-coordinate for second line of header */
#define HEADER_BOTTOM 55       /* y-coord of bottom of header space */
#define ROW_GAP 16             /* pixel gap between rows of each entry */
#define SET_GAP 21             /* pixel gap between entries */
#define DESC 7                 /* margin to allow for descenders in font */
#define ENTRY_HEIGHT (ROW_GAP+SET_GAP)  /* height of each entry */
#define NUM_CALCOE_ROWS 6      /* number of rows in cal. coeff. space */
#define CALCOE_HT (NUM_CALCOE_ROWS*(GetFontHeight(GetRangeLabelFont(0))+ROW_GAP))
/* 
height of cal. coeff. space 
*/
#define MAX_Y (setpts_height-CALCOE_HT)  /* height of set point entries space */
/* 
plot-specification types: customized or reset to default 
*/
#define DEFAULT 0
#define CUSTOM 1
/* 
range labelling
*/
#define NM_TIME_MULTS 26      /* number of words in time_mults[] array */
#define NM_MULTS 36           /* number of words in mults[] array */
#define NM_OCTAL_MULTS 17     /* number of words in octal_mults[] array*/
#define NM_HEX_MULTS 17       /* number of words in hex_mults[] array */
#define LBL_PRECISION 4       /* number of decimal places for labels
                                 with fractional parts */

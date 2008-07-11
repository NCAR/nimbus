/* calib.h -- definitions for calibration window mechanisms
*/

#define VARNAME 0
#define SENSOR_TYPE 1
#define SENSOR_SN 2
#define COND_SN 3
#define AMP_GAIN 4
#define AMP_CUT_OFF 5
#define ADS_ADDRESS 6
#define SET_POINT 7
#define ACCUM_INTRVL 8
#define TECHNICIAN 9
#define OFF_SET 10
#define C0 11
#define C1 12
#define C2 13
#define XMINIMUM	14
#define XMAXIMUM	15
#define VAR_UNITS	16
#define NO_CHOICE -100
#define UPDATE -200
#define PRT_HIST -300
#define EXIT_NOCHG -400
#define DEFAULT_SEX 10        /* gather data input for 10 seconds */
#define DEFAULT_SET 25.0      /* default set point */
#define DEFAULT_BOT -10.0     /* default minimum plot attribute */
#define DEFAULT_TOP 10.0      /* default maximum plot attribute */
#define DEFAULT_UNITS "vdc"   /* default units plot attribute */
#define MAX_INPUT_LENGTH 80   /* maximum expected # input characters in panel
                                 text items */
/* Original reserved space in history structure was 400 bytes */
/* 1/29/90 update took 4 bytes for off_set member */
#define RESERVED 396          /* Amount of reserved space in hist struct */
#define OFFSET_LENGTH 4          /* Taken from original reserved space of 400 */

struct coordinates {
 int x,y;
 struct coordinates *next;
};

struct set_stats { /* save set point vs. voltage averages per variable */
 int y_bottom;
 float set;
 double volt_avg, volt_sigma, volt_maximum, volt_minimum;
 double value_avg, value_sigma, value_maximum, value_minimum;
 struct coordinates *coords;
 struct set_stats *next,*previous;
};

struct hist {
	char calib_name[NAMLEN];
	char sensor_type[MAX_INPUT_LENGTH];
	char sensor_sn[MAX_INPUT_LENGTH];
	char cond_sn[MAX_INPUT_LENGTH];
	char amp_gain[MAX_INPUT_LENGTH];
	char amp_cut_off[MAX_INPUT_LENGTH];
	char ads_address[MAX_INPUT_LENGTH];
	char technician[MAX_INPUT_LENGTH];
        int valid_coefs;
	float coef_0;
	float coef_1;
	float coef_2;
	char off_set[OFFSET_LENGTH];
        char reserved_space[RESERVED];  /* reserve some space for
                        expansion so previous disk files are still the
                        right size */
	struct hist *next;
	};


#define MAXSIZE 25  /* max number of set points per calibration expected */

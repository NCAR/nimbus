/* 
errors.h -- error codes
*/

/* error condition types  */

#define DELETE_LAST_TRACE 		1           
#define UNKNOWN_VARIABLE 		2
#define TRACE_NOT_DISPLAYED 		3
#define VARIABLE_NOT_AVAILABLE 		4
#define ONLY_ONE_VALUE_ENTERED 		5
#define NON_NUMERIC_VALUE_ENTERED 	6
#define ZERO_UNREASONABLE 		7
#define NO_TRACES 			9
#define LESS_OR_EQUAL_ZERO   		10 
#define LOG_LESS_OR_EQUAL_ZERO 		11 
#define TIME_LONGER_THAN_MAXSEX 	12 
#define NO_LISTEES 			13 
#define NO_FIXEES 			14 
#define NO_SHMEM 			15 
#define PROBE_DISABLED 			16 
#define MIN_GT_MAX 			17
#define INVALID_TIME_ENTRY		18
#define ADS_CHANNEL_DEAD 		19
#define ADS_ERROR 			20
#define INVALID_ADS_MSG 		21
#define TAS_WARNING 			22
#define DATASETNOTFOUND       		23 
#define ADS_INIT_ERROR 			24
#define NO_HDR_FILE 			26
#define NO_READ_HDR 			27
#define NO_START_FILE 			28
#define CTL_QUIT 			31
#define SLAVE_QUIT 			32
#define ONLY_TWO_VALUES_ENTERED 	33
#define NO_GSI_VME 			34
#define NO_VGME512 			35
#define SAVE_HISTORY 			36
#define TOO_MANY_SETUPS			37
#define INVALID_DME_ENTRY               38
#define INVALID_FLIGHT_ENTRY            39
#define KILL_SLAVE_DISPLAYS		40
#define REBOOT_ADS		  	41
#define TOO_MANY_PLOTATTRS		42
#define INVALID_GPS_MAG_ENTRY           43
#define INVALID_DIG_OUT_ENTRY           44
#define DUPLICATE_PLOTSPEX_ENTRY        45
#define NO_PMS1D_DATA                   46
#define NO_PMS2D_DATA                   47
#define INVALID_DESCRIPTION             48
#define INVALID_PLOTATTR                49
#define REMOVE_PLOTSPEX_ENTRY       	50 
#define NEEDPLOTSPEXFOLDERNAME       	51 
#define REMOVEPLOTSPEXFOLDER       	52
#define GENERAL_MESSAGE       		53

/* etc.
*/

#define MAX_NOTICE_LINES    18     /* max # of strings sent to an alert  
			NOTE: if you change this, see ../src/errors.c to
			change a hardwire for NOTICE_MESSAGE_STRINGS value */
#define NOTICE_STR_LNGTH    80  /* max length of each string in an alert */

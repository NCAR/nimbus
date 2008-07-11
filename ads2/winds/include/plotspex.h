/* plotspex.h -- describe parameters used in managing the display setup
array and menus
*/

#define FIRST_WORD "Entry_"     /* keywords disallowed in user descriptions */
#define ARGSWORD "ARGS"
#define ENDARGSWORD "END_ARGS"
#define DESCRIBEWORD "DESCRIPTION"
#define PASSWORDENTRY "ENTRY_PASSWORD"
#define NOPASSWORD	"NOPASSWORD"

/* 
# of plotspec structs to malloc at a time 
*/
#define SETUP_BLOCKSIZE 10	
/* 
how many plot setups are possible by default
*/
#define DEFAULT_MAX_SETUPS 200
/* 
special code for calibration window 
*/
#define CALIB_SETUP DEFAULT_MAX_SETUPS*2  

struct alt {
 long valid;			/* entry is valid and ergo useable */
 char desc[MAX_DESC_LNGTH];	/* description associated with this
                                       pre-defined setup */
 char flightsaved[16];		/* flight # of session when setup saved */
 char datesaved[32];		/* date of session when setup saved */
 char disptype[MAX_ARG_LNGTH];	/* type of display window */
 char subtype[TYPE_LEN];	/* sub-category type descriptor  */
};

/* possible modes of input values in plotspex extract */

#define DESCRIBE 	-2
#define ARGS     	-1
#define TRACES 		0
#define CLIPPING 	1
#define TYPE 		2
#define INDEP_VAR 	3
#define TIME_RANGE 	4
#define LISTEES 	5
#define FIXEES 		6
#define BYPASS 		7
#define SKEW_CORNER 	8
#define VECTORS 	9
#define DEP_RANGE 	10
#define INDEP_RANGE 	11
#define GEOMETRY 	12
#define LIST_RATE 	13
#define DEP_INVRT_SCL   14
#define INDEP_INVRT_SCL 15
#define SCALES_SAME 	16
#define VECTORS_CONTRAST 17
#define VECTORS_ON 	18
#define INDEPVAR_YAXIS 	19
#define ISOLATE_TRACES 	20
#define NPLPROBES 	21
#define PLPROBES 	22
#define DISP_RATE 	23
#define FLIGHTSAVED	24
#define DATESAVED	25
#define CHANNEL		26
#define LAST_VALUE	CHANNEL
/*
set END_ARGS to last value defined above plus one
*/
#define END_ARGS        (LAST_VALUE+1)
#define ARGUMENT_VALUE  (END_ARGS+1)

/* new setup menu item value for all processes: assuming 32-bit words */

#define NEW_SETUP               0xf0000000
#define NewSetup(val)          (val & NEW_SETUP)
/*
new plotspec requests encode the entry # in the menu value attached to each 
plotspec selection in the Display Types menu.  Decode it with this macro.
*/
#define GetEntryNumber(val)            (val & 0x0fffffff)
/*
fields for PLOTSPEXMENU
*/
#define NUMPLOTSPEXFIELDS 9
#define SELECTFOLDER	0
#define NUMSETUPSMSG    1
#define SETUPLIST	2
#define SHOWSETUP	3
#define DELETESPEC	4
#define DELETEFOLDER	5
#define REINITPLOTSPEX	6
#define ENABLEEDITS	7
#define DISABLEEDITS	8
/*
show setup button names
*/
#ifndef NUM_DISPLAY_QUADRANTS
#include <display.h>
#endif
#define MAXSHOWSETUPNAMELENGTH 16
#define NUMEDITMODES	2
/*
folders
*/
#define NUMFOLDERSFIELDS 5
#define CURRENTFOLDERMSG 0
#define SELECTCREATEMODE 1
#define ENTERFOLDERNAME  2
#define ENTERPASSWORD    3
#define STATUSMSG    	 4

#define NUMCREATEMODES	2
#define NEWCREATEMODE   0
#define SAVEASCREATEMODE 1

#define MAXFOLDERNAMELENGTH 17
#define MAXPASSWORDLENGTH 17
#define MAXPLOTSPEXFOLDERS 128
/*
keywords for save values file: order of initialization in char array MUST
match ordinal value of associated #defines in panel_menus.h
*/
#define NUMPLOTSPEXSAVEFIELDS    1
#ifndef MAXKEYWORDLENGTH        
#define MAXKEYWORDLENGTH        32
#endif
#define FOLDERNAMEKEYWORD       "PlotspexFolderName"

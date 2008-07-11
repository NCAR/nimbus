/* colors.h -- constants for color use
*/

#define RGB 3

#define REVERSE_VIDEO 0        /* highlight color, inverse video  */
#define NORMAL_VIDEO  1        /* normal colors  */

/* following MUST be power of 2 */
#define NUMCOLORS 16            /* number of colors used from color map  */

#define LTBLU  0		/* light blue */
#define BLU  1                 	/* blue */
#define GRN  2                 	/* green */
#define RED  3                 	/* red */
#define VIO  4                 	/* violet */
#define BRN  5                 	/* brown */
#define LTG  6                 	/* light green */
#define CYN  7                 	/* cyan */
#define PUR  8                 	/* purple */
#define IND  9                 	/* indigo */
#define ORG 10		       	/* orange */
#define  G3 11 		       	/* shades of gray to fill up unused slots */
#define  G4 12
#define  G5 13
#define WHT 14                 	/* white */
#define BLACK  15		/* black */

#define BKGND  0		/* default background color */
#define FOREGROUND BLACK        /* color for border, time stamps, miscellany */

/*
possible values that define which color map to use for background of 
display area
*/
#define NUMCOLORMAPS	2
#define LTBLU_MAP	0
#define BLACK_MAP	1
#define LTBLU_STRING	"blue"
#define BLACK_STRING	"black"
#define DEFAULT_COLOR_MAP BLACK_STRING
/*
maximum value assignable to certain X11 drawing routine parameters
*/
#define MAX_DRAW_COORDINATE 		32767

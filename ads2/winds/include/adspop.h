/* adspop.h
   Defines window and panel parameters for the ads control panel.
*/

#define ADSPOP_NUM_CHAR_WIDTH	82	/* width of adspop_panel in chars */
#define FLIGHT_HEIGHT   	50	/* height of the flight_panel */
#define MX_MSG			4	/* max # panel items to show */
#define CK_ETHER_INTVL  	5	/* .5 sec tics between ethernet chks */
#define CK_ASYNC_INTVL		5	/* seconds between async alive chks */
#define CK_TAS_INTVL		60	/* seconds between tas checks */
#define MIN_INTVL		60	/* seconds per minute interval */
#define HDR_DELAY		9	/* send header delay interval */
#define EVNT_DISP_INTVL 	1	/* how long to display event sent msg */

#define ESC			0x1B	/* escape event code */
#define LEFT_BRACKET		0x5B	/* left bracket event code */
#define CURS_UP			0x41	/* final cursor up event code */
#define CURS_DOWN		0x42	/* final cursor down event code */

#define PV_RT2D_OFF     	0       /* 2d recording off panel value */
#define PV_RT2D_1       	1       /* 1 rec/sec rate panel value */
#define PV_RT2D_2       	2       /* 2 rec/sec rate panel value */
#define PV_RT2D_4       	3       /* 4 rec/sec rate panel value */
#define PV_RT2D_8       	4       /* 8 rec/sec rate panel value */
#define PV_RT2D_MAX     	5       /* max rate panel value */

#define PV_TAS2D_AUTO   	0       /* auto tas panel value */
#define PV_TAS2D_5      	1       /* 5 m/s manual tas panel value */
#define PV_TAS2D_10     	2       /* 10 m/s manual tas panel value */
#define PV_TAS2D_20     	3       /* 20 m/s manual tas panel value */
#define PV_TAS2D_40     	4       /* 40 m/s manual tas panel value */
#define PV_TAS2D_80     	5       /* 80 m/s manual tas panel value */

#define ADSPOP_TEXT_LENGTH 35		/* # chars to display in a text item */

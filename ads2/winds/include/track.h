/* track.h -- parameters associated with track plot embellishments
*/

/* general
*/
#define PTS_PER_COORD 2
#define LAT_COORD  0
#define LON_COORD  1

/* lamdmarks
*/
#define LMRK_LNGTH 6

/* colors used when draw op is GXcopy
*/
/* 
turn green or red into white 
*/
#define HILITED_LMARK	((*GetColorMap[GetDrawType()])()==LTBLU_MAP?WHT:G4)
#define LMARK_COLOR	GRN	/* green: normal color */
#define HIDDEN_COLOR	RED	/* red: showing a deleted color */

/* boundary data
*/
#define START_NEW_LINE 100      /* invalid latitude value, implying start new
                                   line */



/* tvmDefs.h
   TVM front panel display defines.
 
   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

/* Active display states. */
#define DISPLAY_FLIGHT			0	/* flight number display */
#define DISPLAY_STATUS			1	/* status display active */
#define DISPLAY_TAPE            	2   	/* tape display active */
#define DISPLAY_FIXED_FMT      		3	/* fixed format active */
#define DISPLAY_UTILITIES       	4	/* utility display active */

/* Display action states. */
#define DISPLAY_ACTION_NONE		0	/* no action */
#define DISPLAY_ACTION_KEY		1	/* key was pressed */
#define DISPLAY_ACTION_TOGGLE		2	/* toggle to next display */

#define TVM_FIXED_FMT_MAX_DISPLAYS	20	/* max # fixed fmt displays */
#define TVM_FIXED_FMT_MAX_ENTRIES	6	/* max entries per fix fmt */
#define TVM_FIXED_FMT_TOTAL_ENTRIES	120	/* total fixed fmt entries */

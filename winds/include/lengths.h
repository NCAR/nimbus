/* lengths.h  -- lengths of strings
*/

#define TIME_CHARS          12  /* number of characters, each time label */
#define FRAME_LABEL_LNGTH   80  /* length of frame label  */
#define MAX_DESC_LNGTH      27  /* max length, alternate setup descrip'ns */
#define FRAME_LABEL_LNGTH   80  /* length of frame label  */
#define VALUELEN            64  /* max length, dependent variable values */
#define TYPE_LEN            16  /* length of type strings in alternates */
#define MAX_ARG_LNGTH       32  /* max length of command line arguments  */
#define MAX_FLNM_LNGTH      128 /* max length of filenames */
/*
it's good idea to use same for MAX_MSG_LNGTH as for MAX_SOCKET_BUFF_LNGTH
since some socket communications are then routed to an alert (see socket.h)
*/
#define MAX_MSG_LNGTH       1024 /* max length of error message in alert */
#define LABELLEN            40  /* max length of label, indep. var. axis  */
#define UNITSLEN            12  /* max length, units names, including ()'s*/
#define RANGELEN            16  /* max length, variable range values */
#define MAX_TITLE_LENGTH    44  /* max char. lengths: title,*/
#define CATEGORY_LENGTH     80  /* ... bulletin 9 category   */
#define LMRK_LBL_LNGTH 	    8   /* labels for landmarks, track plots */

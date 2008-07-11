/* tasks.h  --  parameters associated with process management
*/

#define RESET 0
#define INPUT 1
#define WINDCOMM 2
#define POSTSCRIPTSERVER 3
#define PORTSERVER 4

/* exit status values
*/
#ifndef OK

/* #define OK must match #define OK in $LOCAL/include/constants.h, in case
   this include file is seen first (#ifndef takes care of constants.h being
   seen first...)
*/
#define OK 		(0)
#endif
#define INTERRUPT       1
#define AFTER_LOOP      2
#define RECYCLE		3

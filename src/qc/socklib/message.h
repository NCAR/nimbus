#ifndef MESSAGE_H
#define MESSAGE_H

#define SOCK_NAMELEN 30     /* length variable name */
#define SOCK_TYPELEN 10     /* length of test type -- i.e. "range", etc. */
#define SOCK_MSGLEN 25      /* length of error message */
#define SOCK_TSLEN 15       /* length of timestamp */

typedef char* VarName;


/* 
* enum ValType
*
* this is only used internally in the validation library, and no longer
* is used in the message structure.  
*/
typedef enum { 
    TYPE_SHIFT,             /* identifies level shift error */
    TYPE_RANGE,             /* identifies range error */
    TYPE_SPIKE,             /* identifies spike error */
    TYPE_FLATLINE,          /* identifies flatline error */
    TYPE_QUIT               /* sent to GUI to tell it to quit (ehhh -hack) */
} ValType;

typedef float ValDataValue; 
typedef enum { SEV_LOW, SEV_HIGH } ValSeverity;


typedef
struct {
        char vname[SOCK_NAMELEN];
        ValSeverity sev;
        char type[SOCK_TYPELEN];
        ValDataValue val;
        char message [SOCK_MSGLEN];
        char Timestamp[SOCK_TSLEN];
        int Count_Consec;
        int Count_last2min;
        } Message;


#endif



/* 
** file: sigdata.h
**
** Author: Ed Stautler
** Date: 3/95
**
** Define the list data structure and the data dependent
** functions for the signal interface between the GUI and
** the socket library.
*/

#ifndef _SIGDATA_H_
#define _SIGDATA_H_

#include "valklist.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
** int Datum_Procedure()
** This function is called upon alarm timer interrupt.
*/
#ifdef __STDC__
int Datum_Procedure (char*);
#else
int Datum_Procedure();
#endif

/*
** void CallProcedure()
** Perform procedure associated with expired time period.
*/
#ifdef __STDC__
void CallProcedure (ListNode*);
#else
void CallProcedure();
#endif

#ifdef __cplusplus
}
#endif

#endif

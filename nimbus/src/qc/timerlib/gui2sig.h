/*
** file: gui2sig.h
** 
** Author: Ed Stautler
** Date: 3/95
**
** Predefine functions for gui2sig.c
*/


#ifndef _GUI2SIG_H_
#define _GUI2SIG_H_

#define THOUSAND    1000
#define ZERO           0
#define SETQ           1
#define GETQ           0

#ifdef __cplusplus
extern "C" {
#endif

/*
** int sigsetmask()
** Library routine to set interrupt mask.
*/
#ifdef __STDC__
int sigsetmask(int);
#else
int sigsetmask();
#endif

/*
** int TimerCreate()
** Initialize a queue for timed procedure execution.
*/
int TimerCreate();

/*
** int TimerSet()
** Set a procedure call at some predetermined time.
*/
#ifdef __STDC__
int TimerSet(int,const char*);
#else
int TimerSet();
#endif

/*
** int TimerCancel()
** Remove a procedure call initialized by TimerSet().
*/
#ifdef __STDC__
int TimerCancel(int);
#else
int TimerCancel();
#endif

/*
** int TimerDestroy()
** Delete queue of timed procedure execution.
*/
void TimerDestroy();

#ifdef __cplusplus
}
#endif

#endif

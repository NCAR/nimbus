/*
** file: gui2sig.c
**
** Author: Ed Stautler
** Date: 3/95
**
** This file contains the definitions for the interface between
** the valkyrie GUI, timing multiplexer, and socket library.  
** Communications to the socket library include all changes in
** variable status as indicated by GUI messages.
*/

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "gui2sig.h"
#include "valksig.h"
#include "valklist.h"
#include "sigdata.h"
#include "sigerr.h"

#include "error.h"
#include "message.h"

/*
** Define operating system compile flag.
*/
#ifdef  SUN4
#ifdef  SOLARIS
#define CALL_SIG_HOLD
#endif
#endif

/*
** LinkedList* ProcedureQ()
** 
** Routine to either initialize or retrieve the list
** of procedures to be called at a predetermined time.
** In either case, a linked list header is returned.
*/
#ifdef __STDC__
static LinkedList* ProcedureQ
(
	int Flag	/* SETQ to create a list, GETQ to retrieve list */
)
#else
static LinkedList* ProcedureQ(Flag)
	int Flag	/* SETQ to create a list, GETQ to retrieve list */
#endif
{
	static LinkedList* ProcQ; 	/* waiting procedure list */

	if (Flag == SETQ)
	{
		ProcQ = ListCreate();
	}
	return ((LinkedList*)ProcQ);
}



/*
** void HandleTimerSignal()
**
** To be executed when an alarm interrupt occurs.  Since
** this function must be initialized with the 'signal' system
** call, it cannot be declared as static. This routine
** performs procedures associated with the expired time and
** removes them from the list of waiting procedures.
*/
#ifdef __STDC__
void HandleTimerSignal
(
	int Dummy	/* parameter needed for prototyping */
) 
#else
void HandleTimerSignal(Dummy)
	int Dummy	/* parameter needed for prototyping */
#endif
{
	int OrigTime;			/* initial time associated with Node */
	ListNode* Node;			/* node pointer */
	ListNode* NextNode;		/* following node */
	ListData* Datum;		/* node data */
        static LinkedList* ProcQ;       /* waiting procedure list */

	/*
	** Perform procedures with an expired time period and
	** those that will expire within the next one thousand
        ** milliseconds.
	*/
	ProcQ = ProcedureQ(GETQ);
	Node = ListFirst(ProcQ);
	OrigTime = Node->Datum->time;
        do {
                Node = ListFirst(ProcQ);
		CallProcedure(Node);
                NextNode = Node->Next;
		Datum = ListDelete(ProcQ,Node);
        } while ((NextNode!=(ListNode*)NULL) &&
                ((NextNode->Datum->time - Datum->time) < THOUSAND));

	/*
	** Set clock alarm for next node time
	*/
        if (NextNode != (ListNode*)NULL)
        {
		struct itimerval ClockTime;	/* old clock value */

                ClockTime = SetClock(NextNode->Datum->time - Datum->time);
        }
#ifdef __cplusplus
	signal(SIGALRM,&HandleTimerSignal);
#else
	signal(SIGALRM,HandleTimerSignal);
#endif
}


/* 
** int TimerCreate()
**
** Create an instance of a timer.  Only one is allowed
** since all signals are done via the SIGALRM signal.
** Once instantiated, all timed procedures are stored
** within ProcQ, a linked list.
*/
int TimerCreate()
{
	LinkedList* ProcQ;			/* procedure list */
	static int TimerInit=ZERO;		/* timer initialized flag */

	/*
	** Check for existing timer
	*/
	if (TimerInit)
	{
		fprintf(ERRORFILE,"gui2sig: Timer already initialized\n");
		return (FAILURE);
	}

	/*
	** Create queue
	*/
	ProcQ = ProcedureQ(SETQ);
	if (ProcQ==(LinkedList*)NULL)
	{
		fprintf(ERRORFILE,"gui2sig: List create failed\n");
		return (FAILURE);
	}

	ProcQ->SendRoutine = (unsigned int)(new Sleep);
	if (ProcQ->SendRoutine == NULL)
	{
		fprintf(ERRORFILE,"gui2sig: SendRoutine create failed\n");
		return (FAILURE);
	}
	
	TimerInit = 1;

#ifdef __cplusplus
	signal(SIGALRM,&HandleTimerSignal);
#else
	signal(SIGALRM,HandleTimerSignal);
#endif
	return (SUCCESS);
}


/*
** int TimerSet()
**
** Add a timed procedure to the waiting queue.  If successful,
** a unique identifier greater than 0 is returned.  Otherwise,
** the value of FAILURE is returned.
*/
#ifdef __STDC__
int TimerSet
(
	int Time, 			/* time till interrupt, in secs. */
	const char* ProcParam		/* procedure parameter */
)
#else
int TimerSet(Time,ProcParam)
	int Time; 			/* time till interrupt, in secs. */
	const char* ProcParam;		/* procedure parameter */
#endif
{
	int Id;				/* node identifier */
        int (*CallProc)(char*); 	/* pointer to procedure to call */
	LinkedList* ProcQ; 		/* waiting procedure list */
	Sleep* SendMsg;			/* address of routine to call */
	char SendString[MAXPARAM];	/* parameter string */
	 

	/*
	** critical section, mask interrupts
	** while adding procedures to wait queue
	*/
#ifdef CALL_SIG_HOLD
	sighold(SIGALRM);
#else
	sigsetmask(SIGMASK);
#endif

	Time = Time * THOUSAND;
	ProcQ = ProcedureQ(GETQ);

	sprintf(SendString,"%u %s",ProcQ->SendRoutine,ProcParam);
	Id = SetTimer(ProcQ,Time,Datum_Procedure,SendString);
	if (Id == ZERO)
	{
		fprintf(ERRORFILE,"gui2sig: Timer set failed\n");
#ifdef CALL_SIG_HOLD
		sigrelse(SIGALRM);
#else
		sigsetmask(ZERO);
#endif
		return (FAILURE);
	}

	SendMsg = (Sleep *)ProcQ->SendRoutine;
	SendMsg->set((char *)ProcParam);

#ifdef CALL_SIG_HOLD
	sigrelse(SIGALRM);
#else
	sigsetmask(ZERO);
#endif

	/*
	** If quit message sent, delete send class.
	*/
	if (strcmp(ProcParam,"quit") == 0)
	{
		delete ((Sleep *)ProcQ->SendRoutine);
	}

	return (Id);
}


/*
** int TimerCancel()
**
** Cancel interrupt and remove node from list.
*/
#ifdef __STDC__
int TimerCancel
(
	int Id			/* node identifier */
)
#else
int TimerCancel(Id)
	int Id;			/* node identifier */
#endif
{
	ListNode* Node;		/* node pointer */
	LinkedList* ProcQ; 	/* waiting procedure list */
	char Message[MAXPARAM]; /* parameter string */
        unsigned int SendAddr;  /* procedure address */
        Sleep* SendMsg;		/* procedure to be called */

	/*
	** retrieve waiting node with the Id passed
	*/
	ProcQ = ProcedureQ(GETQ);
	Node = ListFind(ProcQ, Id);
	if(Node == (ListNode*)NULL)	
	{
		fprintf(ERRORFILE,"gui2sig: Id not found\n");
		return (FAILURE);
	}
	 
	/*
	** critical section, mask interrupts
	** while removing procedures from wait queue
	*/
#ifdef CALL_SIG_HOLD
	sighold(SIGALRM);
#else
	sigsetmask(SIGMASK);
#endif
        sscanf(Node->Datum->ProcParam,"%u %s",&SendAddr,Message);
	if (!CancelTimer(ProcQ,Node))
	{
		fprintf(ERRORFILE,"gui2sig: Cancel failed\n");
#ifdef CALL_SIG_HOLD
		sigrelse(SIGALRM);
#else
		sigsetmask(ZERO);
#endif
		return (FAILURE);
	}

	/*
	** Send enable message to validation library.
	*/
        SendMsg = (Sleep *)SendAddr;
        SendMsg->unset(Message);

#ifdef CALL_SIG_HOLD
	sigrelse(SIGALRM);
#else
	sigsetmask(ZERO);
#endif
	return (SUCCESS);
}

/*
** int TimerDestroy()
**
** Destroy waiting procedure queue.
*/
void TimerDestroy()
{
	LinkedList* ProcQ; 	/* waiting procedure list */

	ProcQ = ProcedureQ(GETQ);

	/*
	** Destroy queue
	*/
	ListDestroy(ProcQ);
}

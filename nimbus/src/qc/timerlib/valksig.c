/*
** file: valksig.c
**
** Author: Ed Stautler
** Date: 3/95
**
** This file contains the definitions for a signal multiplexer
** where a SIGALRM interrupt is created upon timer expiration.
*/

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include "valksig.h"
#include "sigerr.h"


/*
** struct itimerval SetClock()
**
** Initialize the clock to send an interrupt. 
*/
#ifdef __STDC__
struct itimerval SetClock
(
	int SigTime				/* time till signal, in secs. */
)
#else
struct itimerval SetClock (SigTime)
	int SigTime;				/* time till signal, in secs. */
#endif
{
	int Seconds;				/* Seconds till interrupt */
	float MilliSec;				/* milli seconds to interrupt */
	struct itimerval ClockTime;		/* clock time value */
	struct itimerval PrevClockTime;		/* old clock time value */

/*
** Calculate milli seconds to interrupt time
*/
	MilliSec = SigTime / THOUSAND;
	Seconds = (int)MilliSec;
	MilliSec = MilliSec - Seconds;
/*
** Set timer for alarm clock signal
*/
	ClockTime.it_interval.tv_sec = ZERO;
	ClockTime.it_interval.tv_usec = ZERO;
	ClockTime.it_value.tv_usec = (int)(MilliSec * MILLION);		
	ClockTime.it_value.tv_sec = Seconds;
	if (setitimer(ITIMER_REAL,&ClockTime,&PrevClockTime) == -1)
	{
		fprintf(ERRORFILE,"valksig: Setitimer failed");
	}
	return (PrevClockTime);
}


/*
** int CancelTimer()
**
** Cancel an interrupt that has been Previously
** set.  Remove associated node from the list.
*/
#ifdef __STDC__
int CancelTimer
(
	LinkedList* WaitQueue, 		/* list pointer */
	ListNode* Node			/* node to delete */
)
#else
int CancelTimer(WaitQueue,Node) 
	LinkedList* WaitQueue; 		/* list pointer */
	ListNode* Node;			/* node to delete */
#endif
{
	/*
	** If node is first one waiting (special case treatment)
	*/
	if (Node == ListFirst(WaitQueue))
	{
		ListNode* NodePtr = Node;	/* Node pointer */
		struct itimerval ClockTime;	/* clock time */

		if (getitimer(ITIMER_REAL, &ClockTime) == -1)
		{
			fprintf(ERRORFILE,"valksig: getitimer failed\n");
			return (FAILURE);
		}		

		/*
		** If there are following nodes, adjust clock time 
		** to reflect their true value. 
		*/
		if (NodePtr != (ListNode*)NULL)
		{
			int TimeLeft;		/* time remaining on clock */

			/*
			** Calculate clock adjustment time
			*/
			TimeLeft = ClockTime.it_value.tv_sec;
			TimeLeft = TimeLeft * THOUSAND;
			TimeLeft += (ClockTime.it_value.tv_usec/THOUSAND);

			if (TimeLeft > ZERO)
			{
				int TimeAdjust;		/* time adjustment */

				TimeAdjust = NodePtr->Datum->time-TimeLeft;
				NodePtr = NodePtr->Next;
				while (NodePtr != (ListNode*)NULL)
				{
					NodePtr->Datum->time -= TimeAdjust;
					NodePtr = NodePtr->Next;
				}
			}
		}
		if (Node->Next != (ListNode*)NULL)
		{
			ClockTime = SetClock(Node->Next->Datum->time);
		}
		else
		{
			ClockTime = SetClock(ZERO);
		}
	}

	/*
	** Remove node from list
	*/
	if(ListDelete(WaitQueue,Node) == (ListData*)NULL)
	{
		fprintf(ERRORFILE,"valksig: ListDelete failed\n");
		return (FAILURE);
	}
	return (SUCCESS);
}


/*
** int SetTimer()
**
** Add a new procedure to the waiting list.
** If the new addition requires less wait time than
** the rest, reset the clock.
*/
#ifdef __STDC__
int SetTimer
(
	LinkedList* WaitQueue,		/* list pointer */
	int ExpireTime,			/* time till interrupt (in secs.) */
	int(*Procedure)(char*),		/* procedure to be performed */
	char* ProcParam			/* procedure parameter */
)
#else
int SetTimer(WaitQueue,ExpireTime,Procedure,ProcParam)
	LinkedList* WaitQueue;		/* list pointer */
	int ExpireTime;			/* time till interrupt (in secs.) */
	int(*Procedure)();		/* procedure to be performed */
	char* ProcParam;		/* procedure parameter */
#endif
{
	ListData* Datum;		/* Datum to add to List */
	ListNode* Node;			/* Node to add to List */
	static int Id=ZERO;		/* node identifier */
	struct itimerval ClockTime;	/* clock time structure */

	/*
	** Check for valid expiration time
	*/
	if (ExpireTime <= ZERO)
	{
		return (FAILURE);
	}

	/*
	** Generate new procedure Id number
	*/
	if(ListLength(WaitQueue) == ZERO)
	{
		Id = ZERO;
	}
	Id++;

	/*
	** Get current clock time value
	*/
	if (getitimer(ITIMER_REAL, &ClockTime) == -1)
	{
		fprintf(ERRORFILE,"valksig: getitimer failed\n");
		Id--;
		return (FAILURE);
	}	

	/*
	** Adjust nodes to reflect a new clock setting
	*/
	Node = ListFirst(WaitQueue);
	if (Node != (ListNode*)NULL)
	{
		int TimeLeft;			/* time remaining on clock */

		/*
		** Calculate time to next procedure call
		*/
		TimeLeft = ClockTime.it_value.tv_sec;
		TimeLeft = TimeLeft * THOUSAND;
		TimeLeft += (ClockTime.it_value.tv_usec/THOUSAND);

		if (TimeLeft > ZERO)
		{
			int TimeAdjust;		/* clock time to adjust for */

			TimeAdjust = Node->Datum->time - TimeLeft;
			Node->Datum->time = TimeLeft;
			Node = Node->Next;
			while (Node != (ListNode*)NULL)
			{
				Node->Datum->time -= TimeAdjust;
				Node = Node->Next;
			}
		}
	}
  
	/*
	** Create and initialize Datum
	*/
	Datum = (ListData*)malloc(sizeof(ListData));
	if (Datum == (ListData*)NULL)
	{
		fprintf(ERRORFILE,"valksig.c: Datum malloc failed\n");
		return (FAILURE);
	}	
	strcpy(Datum->ProcParam,ProcParam);
	Datum->Id = Id;
	Datum->Procedure = Procedure;
	Datum->time = ExpireTime;

	/*
	** Insert new procedure in List
	*/
	Node = ListInsert(WaitQueue, Datum);
	if (Node == (ListNode*)NULL)
	{
		fprintf(ERRORFILE,"valksig: List insert failed\n");
		Id--;
		return (FAILURE);
	}	
  
	/*
	** Set clock to new time if the added procedure 
	** is first on the list
	*/
	if ((ClockTime.it_value.tv_sec > ExpireTime) ||
	    (Node == ListFirst(WaitQueue)))
	{
		ClockTime = SetClock(ExpireTime);
	}

	return Id;
}

/*
** file: sigdata.c
**
** Author: Ed Stautler
** Date: 3/95
**
** This file contains all data specific operations from
** the linked list definition for timing signals.  All
** changes to procedure calls, such as new parameters,
** will result in modifications in this file.  Locations
** for modification can be found by grepping for "Datum"
** throughout this file.  Furthermore, changes will also
** be needed in the header file, "sigdata.h".
*/

#include <stdio.h>
#include "valklist.h"
#include "gui2sig.h"
#include "sigerr.h"
#include "error.h"

/*
** int Datum_Procedure()
**
** This function is called upon alarm timer interrupt.
** It calls socket library to communicate enable messages
** to the validation library.
*/
#ifdef __STDC__
int Datum_Procedure
(
        char* ProcParam 	/* sending routine, parameter for proc. call */
)
#else
int Datum_Procedure(ProcParam)
        char* ProcParam;	/* sending routine, parameter for proc. call */
#endif
{
        char Message[MAXPARAM];	/* data associated with passed parameter */
        unsigned int SendAddr;	/* sending routine address */
        Sleep* SendMsg;		/* sending routine */

        sscanf(ProcParam,"%u %s",&SendAddr,Message);
        SendMsg = (Sleep *)SendAddr;
        SendMsg->unset(Message);

        return (SUCCESS);
}


/*
** void CallProcedure()
**
** Perform procedure associated with ExpireTimed Node.
*/
#ifdef __STDC__
void CallProcedure
(
	ListNode* Node 		/* Node with procedure to be called */
)
#else
void CallProcedure(Node)
	ListNode* Node;		/* Node with procedure to be called */
#endif
{
	if (Node != (ListNode*)NULL)
	{
		(*Node->Datum->Procedure)(Node->Datum->ProcParam);
	}

}


/*
** void ListInsert()
**
** Insert into the list according to procedure call back time.
*/
#ifdef __STDC__
ListNode * ListInsert
(
	LinkedList *List,	/* list header */
	ListData  *NewDatum 	/* Datum to be inserted */
)
#else
ListNode * ListInsert(List,NewDatum)
	LinkedList *List;	/* list header */
	ListData  *NewDatum;	/* Datum to be inserted */
#endif
{

	ListNode *NewNode;	/* new Node */
	ListNode *PrevToNew;	/* Node previous to insert position */

	/*
	** Check for errors in passed parameters
	*/
	if ((List == (LinkedList*)NULL)||(List == (LinkedList*)(-1)))
	{
		return((ListNode*)NULL);
	}

        /*
        ** Allocate memmory for the new Node
        */
        NewNode = (ListNode*)malloc(sizeof(ListNode));
        if (NewNode==(ListNode*)NULL)
        {
                return((ListNode*)NULL);
        }

        /*
        ** Insert Datum
        */
        NewNode->Datum = NewDatum;
        if (NewNode->Datum==(ListData*)NULL)
        {
                return((ListNode*)NULL);
        }

	/*
	** Find list insert position based on datum time
	*/
	if (List->First == (ListNode*)NULL)
	{
		PrevToNew = (ListNode*)NULL;
	}
	else
	{
		ListNode *TempNode = List->First;
		PrevToNew = NULL;
		while (TempNode != (ListNode*)NULL)
		{
			if (NewDatum->time <= TempNode->Datum->time)
			{
				TempNode = NULL;
			}
			else
			{
				PrevToNew = TempNode;
				TempNode = TempNode->Next;
			}
		}
	}
				
	/*
	** If Node specified is NULL, then add to the beginning of the List 
	*/	
	if (PrevToNew == (ListNode*)NULL) 
	{	
		/*
		** If the List is empty
		*/
		if (List->First == (ListNode*)NULL)
		{
			List->First = NewNode;
			List->Last = List->First;
			NewNode->Prev = (ListNode*)NULL;
			NewNode->Next = (ListNode*)NULL;
		}

		/*
		** else, the List is not empty
		*/
		else 
		{
			NewNode->Prev = (ListNode*)NULL;
			NewNode->Next = List->First;
			List->First->Prev = NewNode;
			List->First = NewNode;
		}
	}

	/*
	** If the Node is the last one, add to the end of the List
	*/
	else if(PrevToNew->Next == (ListNode*)NULL)
	{
		PrevToNew->Next = NewNode;
		NewNode->Prev = PrevToNew;
		NewNode->Next = (ListNode*)NULL;
		List->Last = NewNode;
	}

	/*
	** Else, the Node is to be inserted between others
	*/
	else 
	{
		NewNode->Prev = PrevToNew;
		NewNode->Next = PrevToNew->Next;
		NewNode->Next->Prev = NewNode;
		PrevToNew->Next = NewNode;
	}
	
	(List->Length)++;	
	return(NewNode);
}
				 

/*
** ListNode* ListFind()
** 
** This procedure searches the linked list and returns
** the node or null depending on if the specified identifier
** is found.
*/
#ifdef __STDC__
ListNode * ListFind
(
	LinkedList *List,	/* list pointer	*/
	int Target		/* datum to find */
)
#else
ListNode * ListFind(List,Target)
	LinkedList *List;	/* list pointer	*/
	int Target;		/* datum to find */
#endif
{
	ListNode* NodePtr;	/* node pointer */

	/*
	** Check for empty list
	*/
	if ( List == (LinkedList*)NULL )
	{
		return((ListNode*)NULL);
	}

	/*
	** Search list for Id specified
	*/	
	NodePtr = ListFirst(List);
	while( (NodePtr != (ListNode*)NULL) && 	(NodePtr->Datum->Id != Target))
	{ 
		NodePtr = NodePtr->Next;
	}

	/*
	** If found, return node
	*/
	if ( (NodePtr != (ListNode*)NULL) && (NodePtr->Datum->Id == Target))
	{
		return(NodePtr);
	}
	else 
	{
		return((ListNode*)NULL);
	}
}

/*
** file: valklist.c
**
** Author: Ed Stautler
** Date: 3/95
**
** This file contains the independent linked list code.
** There are no data dependencies within this section and
** so may be reused by any routine needing a doubly linked
** list.  The only thing that would need to be changed are
** the definitions found in the sigdata.h file.  A new
** file should be created defining the data and be included
** in place of sigdata.h.
*/

#include <stdio.h>
#include "valklist.h"
#include "sigdata.h"
#include "sigerr.h"


/*
** void ListDestroy()
**
** Destroy the linked list, free all memory associated
** with it.
*/
#ifdef __STDC__
void ListDestroy
(
	LinkedList *List 	/* list header */
)
#else
void ListDestroy(List)
	LinkedList *List; 	/* list header */
#endif
{ 
	/*
	** Free all memory associated with list.
	*/
	if (List != (LinkedList*)NULL)     
	{
		ListNode *NodePtr = List->First;        /* current node */
		ListNode *OldNode;  			/* dereferenced Node */

		List->First = (ListNode*)NULL;
		List->Last = (ListNode*)NULL;
		while  (NodePtr != (ListNode*)NULL)
		{
			OldNode = NodePtr;      
			NodePtr = NodePtr->Next; 
			free(OldNode);	    
		}
	}
	free(List);
	List = (LinkedList*)NULL;
}

/*
** LinkedList* ListCreate()
**
** Create a header to a linked list.
*/
LinkedList* ListCreate()
{
	LinkedList* ListHeader;		/* new list pointer */

	ListHeader = (LinkedList*)malloc(HEADERSIZE);
	if (ListHeader == (LinkedList*)NULL)
	{
		return((LinkedList*)NULL);
	}

	ListHeader->First = (ListNode*)NULL;
	ListHeader->Last = (ListNode*)NULL;
	ListHeader->Length = 0;

	return(ListHeader);
}


/*
** LinkedList* ListDelete()
**
** Delete a single node in the linked list.
*/
#ifdef __STDC__
ListData* ListDelete
(
	LinkedList* List,	/* list header */
	ListNode* Node		/* node to delete */
)
#else
ListData* ListDelete(List,Node)
	LinkedList* List;	/* list header */
	ListNode* Node;		/* node to delete */
#endif
{
	ListData *RetDatum;	/* return data */

	/*
	** Check for error conditions 
	*/
	if ( (Node == (ListNode*)NULL) || (List == (LinkedList* )NULL) )
	{  
		return((ListData*)NULL);
	}

	/* 
	** If there is only one Node in the List
	*/
	if (List->First == List->Last)
	{
		List->First = (ListNode*)NULL;
		List->Last = (ListNode*)NULL;
	}
	
	/*
	** If Node is last on the List 
	*/	
	else if (Node->Next == (ListNode*)NULL)
	{
		List->Last = Node->Prev;
		Node->Prev->Next = (ListNode*)NULL;
	}

	/*
	** If Node is first on the List
	*/
	else if (Node->Prev == (ListNode*)NULL)
	{
		List->First = Node->Next;
		Node->Next->Prev = (ListNode*)NULL;
	}

	/*
	** Node is in the middle of the List
	*/
	else		
	{      
		Node->Prev->Next = Node->Next;
		Node->Next->Prev = Node->Prev;
	}
	
	(List->Length)--;
	RetDatum = Node->Datum;
	free(Node); 
	return(RetDatum);
}		

/*
** file: valklist.h
**
** Author: Ed Stautler
** Date: 3/95
**
** Define the linked list functions available.
*/

#include <stdlib.h>
#include "sigdata.h"

#ifndef _VALKLIST_H_
#define _VALKLIST_H_

#define MAXPARAM 80

/*
** List Datum
**
** This is the data pointed to by a node in the linked list.
** It is locally defined since any changes in this will result
** in changes to other definitions within this and the .c file.
*/
typedef struct List_Datum
{
        char ProcParam[MAXPARAM];             /* procedure parameter */
        int Id;                         /* process Id */
#ifdef __STDC__
        int (*Procedure)(char*);        /* procedure to perform */
#else
        int (*Procedure)();             /* procedure to perform */
#endif
        int time;                       /* expiration time */
} ListData;


/* 
** List node 
*/
typedef struct List_node
{
	struct List_node* Next;		/* Next pointer */
	struct List_node* Prev;		/* Previous pointer */
	ListData* Datum;		/* datum pointer */
} ListNode;


/* 
** List header
*/
typedef struct List_header      	/* List header node */
{
	ListNode* First;		/* First node in List */
	ListNode* Last;			/* Last node in List */
	unsigned int Length;		/* Length of List */ 
	unsigned int SendRoutine;	/* Socket interface class address */ 
} LinkedList;


#define HEADERSIZE	((unsigned)sizeof(LinkedList))
#define NODESIZE	((unsigned)sizeof(ListNode))

/************************************************************************
** 			The List functions			       **
************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/* 
** Initialize header
*/
LinkedList* ListCreate();	

/*
** Destroy List
*/
#ifdef __STDC__
void ListDestroy(LinkedList*);
#else
void ListDestroy();
#endif

/* 
** Insert a node 
*/
#ifdef __STDC__
ListNode* ListInsert(LinkedList*, ListData*);             
#else
ListNode* ListInsert();             
#endif

/* 
** Delete a node 
*/
#ifdef __STDC__
ListData* ListDelete(LinkedList*, ListNode*);		    
#else
ListData* ListDelete();		    
#endif

/* 
** Find a node 
*/
#ifdef __STDC__
ListNode* ListFind(LinkedList*, int);  
#else
ListNode* ListFind();  
#endif


/********************
** Macro functions **
********************/

/* 
** ListNode* ListFirst(LinkedList*);	
** Get the First node
*/
#define ListFirst(List)  ( ((List) == (LinkedList*)NULL) 	    \
				? ((ListNode*) NULL)                \
				: ((List) -> First) )

/*
** ListNode* ListLast(LinkedList*);	
** Get the Last node
*/
#define ListLast(List)	  ( ((List) == (LinkedList*)NULL)           \
				? ((ListNode*) NULL) 	            \
				: ((List) -> Last) )

/*
** ListNode* ListNext(ListNode*);		
** Get the Next node
*/
#define ListNext(node)	   ( ((node) == (ListNode*)NULL)	    \
				? ((ListNode*)NULL)	            \
				: ((node) -> Next) )

/* 
** ListNode* ListPrevious(ListNode*);		 
** Get the Previous node 
*/ 
#define ListPrevious(node)   ( ((node) == (ListNode*)NULL)  	    \
				? ((ListNode*)NULL)	  	    \
				: ((node) -> Prev) )
/*
** ListData* ListGetData (ListNode*);		 
** Get the Datum 
*/
#define ListGetData(node)  ( ((node) == (ListNode*)NULL)	    \
			       ? ((ListData*)NULL)		    \
			       : ((node) -> Datum) )

/*
** voId ListPutData (ListNode*, ListData*);	 
** Put in new Datum 
*/
#define ListPutData(node, NewDatum)   ( ((node) != (ListNode*)NULL)     \
					? ((node) -> Datum) = NewDatum )\
					: 0

/*
** unsigned int ListLength (LinkedList*);	 
** Get the Length 
*/
#define ListLength(List)    ( ((List) == (LinkedList*)NULL)	   \
				? (0)   			   \
				: ((List) -> Length) )

#ifdef __cplusplus
}
#endif

#endif

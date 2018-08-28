//
// Filename:  valqueue.cc
// Author:    Marc Anderson
// Compiler:  GNU C++ 2.6.x
// Copyright 1995, All Rights Reserved
//
// Description:
//    implementation of generic queue.
//
//    (one of these days this should be implemented with c++ templates)
//
// Methods:
//    ValQueue::dequeue()
//
//    ValQueue::enqueue()
//

#define VALQUEUE_C

#include "stddefs.h"
#include "valqueue.h"



//
// ValQueue::dequeue()
//
// remove a node from the head of a generic queue.
//
ValQueueNode *ValQueue::dequeue(void)
{
	 ValQueueNode *rtn_value;
	 
	 rtn_value = head;
	 if(!rtn_value)
		return NULL;
	 
	 if(!head->next)
		head = tail = NULL;
	 else
		head = head->next;
	 
	 --num_items;
	 
	 return(rtn_value);
}



//
// ValQueue::enqueue()
//
// enqueue a node to the tail of a generic queue.
//
void ValQueue::enqueue(ValQueueNode *new_node)
{
	 if(!head)     // assume !tail also
	 {
		  head = tail = new_node;
		  new_node->next = NULL;
	 }
	 else
	 {
		  tail->next = new_node;
		  new_node->next = NULL;
		  tail = new_node;
	 }

	 ++num_items;
}



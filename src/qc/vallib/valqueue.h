//
// Filename:  valqueue.h
// Author:    Marc Anderson
// Compiler:  GNU C++ 2.6.x
// Copyright 1995, All Rights Reserved
//
// Description:
//		definitions for generic queue class
//
//

#ifndef VALQUEUE_H
#define VALQUEUE_H

//
// class ValQueueNode
//
// generic node for generic queue.
//
// organization:
//   [   ] -> [   ] -> [   ] -> [   ] -> NULL
//   head                       tail
//   (deq)                      (enq)
//
class ValQueueNode
{
  public:
	 class ValQueueNode *next;
};


//
// class ValQueue
// 
// generic QUEUE class maintaining just the list of pointers to generic 
// data elements.  note that the destructor is particularly dangerous,
// because it will not deallocate memory safely.  (this should be handled
// by the destructor on the higher level)
//
class ValQueue
{
	 ValQueueNode *head;       // where nodes dequeued
	 ValQueueNode *tail;       // where nodes enqueued

	 u_int16 num_items;

  public:

	 ValQueue(void) { head = tail = NULL; num_items = 0; }
	 ~ValQueue() { while(num_items) (void) dequeue(); }

	 ValQueueNode *dequeue(void);
	 void enqueue(ValQueueNode *new_node);
	 
	 ValQueueNode *at_head(void)   { return head; }

	 u_int16 get_num_items(void) { return num_items; }
};

#endif

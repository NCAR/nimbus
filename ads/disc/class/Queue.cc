/*
-------------------------------------------------------------------------
OBJECT NAME:	Queue.cc

FULL NAME:	Generic Queue stuff.

ENTRY POINTS:	Queue(), ~Queue()
		EnQueue(data)
		DeQueue()
		Front()

DESCRIPTION:	

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	none
-------------------------------------------------------------------------
*/

#include "Queue.h"


/* -------------------------------------------------------------------- */
Queue::Queue()
{
  head = tail = (QueueNode *)NULL;
  length = 0;

}	/* END CREATEQUEUE */

/* -------------------------------------------------------------------- */
Queue::~Queue()
{
  QueueNode	*trace, *tmp;

  for (trace = head; trace; )
    {
    tmp = trace;
    trace = trace->next;
    delete tmp;
    }

}	/* END DESTROYQUEUE */

/* -------------------------------------------------------------------- */
bool Queue::EnQueue(void *data)
{
  QueueNode	*tmp;

  if (data == (void *)NULL)
    return(false);

  tmp = new QueueNode;

  tmp->data = data;
  tmp->next = (QueueNode *)NULL;

  if (head == (QueueNode *)NULL)
    head = tail = tmp;
  else
    {
    tail->next = tmp;
    tail = tmp;
    }

  ++length;

  return(true);

}	/* END ENQUEUE */

/* -------------------------------------------------------------------- */
void *Queue::DeQueue()
{
  QueueNode	*tmp;
  void		*td;

  if (head == (QueueNode *)NULL)
    {
    length = 0;
    return((void *)NULL);
    }

  tmp	= head;
  head	= tmp->next;

  if (head == (QueueNode *)NULL)
    tail = (QueueNode *)NULL;

  --length;

  td = tmp->data;

  delete tmp;

  return(td);

}	/* END DEQUEUE */

/* END QUEUE.C */

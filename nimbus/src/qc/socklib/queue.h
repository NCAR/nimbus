#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include "message.h"


struct Q_element 
{	Message *message;
	Q_element *next;
};
	
class Queue 
{
 private: 	
 	 	Q_element *front;
 	 	Q_element *back;
		int Qsize;
 public:
		Queue() 	{ front = back = NULL; Qsize = 0; }
	       	~Queue()	{ };
		int 	size() 	{ return(Qsize); }
	 	Message *next()	{ if (front != NULL) return(front->message); 
				  else return(NULL); }
		int remove();
		int add(Message *msg);
};
				
				
#endif

#include "report.h"
#include "message.h"
#include "queue.h"


int Report::flushQ(void)
// Flushes the message queue to which messages are written previous
//  to being written to the socket.
{
    int messagesSent = 0;
    Message *currMsg;

    for ( currMsg = Report::Q.next();
	  ( (currMsg != NULL) && (sendMessage(Report::messageSkt,currMsg)==1)
);
	  currMsg = Report::Q.next() )
    {
	    Report::Q.remove();
	    messagesSent++;
    }

    if(Report::Q.size() != 0)
    {
	printf("QC: msgs in send queue after flush == %d\n", Report::Q.size());
    }
    return(messagesSent);
}


int 	Queue::remove() 
// Removes the element at the head of the queue.
//  Decrements counters private to the queue class
//  as appropriate. Returns 1 on success and 0 on failure.
{
	Q_element *temp;

	temp = front;
	if (temp != NULL)
	{	front = front->next; 
              		if (front == NULL)
			back = NULL;
		delete temp->message;
                delete temp;
                Qsize--;
                return(1);
	}
	else
		return(0);
}

// Adds an element to the queue. Increments counters
//  of the queue class as appropriate. Parameter msg
//  is a pointer to message to be added. Returns 1 on
//  success.
int Queue::add(Message *msg) 
{ 	
	Q_element *temp;

	if ((temp = (Q_element*) new Q_element) == 0)
	{       std::cout << "New failed in message queueing" << std::endl;
		return(0);
	}

 	temp->message = msg;
	if (back == NULL)
              front = temp;
        else
              back->next = temp;
        back = temp;
        temp->next = NULL;
        Qsize++;
        return(1);
}
			

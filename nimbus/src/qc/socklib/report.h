#ifndef REPORT_H
#define REPORT_H

#include <stdio.h> 
#include <stdlib.h> 
#include <iostream.h> 
#include <unistd.h> 

#include "queue.h"
#include "message.h"
#include "sleep.h"

extern "C" 
{
	int initConnect(int *messageSkt,int *sleepSkt, int *error_init);
	void closeSkt(int fd);
	Sleep_Message *getSleepMessage(int fd);
	int sendMessage(int sock,Message *msg);	
}

class Report 
{
 private:
	int messageSkt; // Socket descriptor for message socket
	int sleepSkt;   // Socket descriptor for sleep messages

 public:
	Queue Q;	// Creates a message queue to facilitate 
			//  non-blocking write to socket.

	int error_constructing;

	Report() { initConnect(&messageSkt,&sleepSkt, &error_constructing); }
	~Report(){ closeSkt(messageSkt);  closeSkt(sleepSkt); }
	void Send(Message *msg) { Q.add(msg); }
	Sleep_Message *nextSleepMessage() { return(getSleepMessage(sleepSkt));}
	int flushQ(void);
};

#endif

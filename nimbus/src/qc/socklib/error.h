#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include "sleep.h"
#include "message.h"

extern "C" {
	int messageInit(int *error_init);
	int sleepInit(int *error_init);
	void closeSleep(int);
	void closeMessage(int);
	int unsleepGlobal(int);
	int sleepGlobal(int);
	int unsleepVar(int,VarName);	
	int sleepVar(int,VarName);	
	Message *getNextMessage(int);	
	}

class Error 
{
	int MessageFD;
public:
	int error_constructing;

	Error() 		{ MessageFD =messageInit(&error_constructing);}
	~Error()  		{ closeMessage(MessageFD); }
	Message *getNext()  	{ return (getNextMessage(Error::MessageFD)); }
	int fd()                { return MessageFD; }
};

class Sleep
{
	int SleepFD;
public: 
	int error_constructing;

	Sleep()			{ SleepFD = sleepInit(&error_constructing); }
	~Sleep()		{ closeSleep(SleepFD); }
	 // Method to set a variable specified by var to sleep mode.
 	void set(VarName var)	{ sleepVar(SleepFD,var); }
	 // Set the variable specified by var to normal (un-sleep)  mode.
	void unset(VarName var) 	{ unsleepVar(SleepFD,var); }	
};

#endif 

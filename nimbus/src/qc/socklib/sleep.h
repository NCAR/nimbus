#ifndef SLEEP_H
#define SLEEP_H

#include "message.h"

typedef enum { SLEEP, UNSLEEP } Sleep_Mode;
typedef enum { SINGLE_VARIABLE,ALL_VARIABLES } Sleep_Scope;

typedef struct {
	Sleep_Mode mode;
	Sleep_Scope scope;
	char name[SOCK_NAMELEN];
	} Sleep_Message;

typedef Sleep_Message *sleepMsgPtr;

#endif

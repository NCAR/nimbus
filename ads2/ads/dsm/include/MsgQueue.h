/* MsgQueue.h
   Class for managing a message queue.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef MSGQUEUE_H
#define MSGQUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <msgQLib.h>

#define MSGQ_MAX_MSG 	10		// max number of messages to queue
#define MSGQ_MCR_MSG 	35		// max number of messages to queue
#define MSGQ_MASP_MSG 	10		// max number of messages to queue
#define MSGQ_P2D_MSG	10		// max number of messages to queue
#define MSGQ_NULL	0		// null message queue
 
class MsgQueue {
public:
  MsgQueue (const int size, int msg_num);
  int msgSend (const char *src, const int len)
	{return msgQSend (que_id, (char*)src, len, NO_WAIT, MSG_PRI_NORMAL);}
  int msgRecv (char *const dest, const int len)
	{return msgQReceive (que_id, dest, len, NO_WAIT);}
  int msgQNum ()
        {return msgQNumMsgs(que_id);}

private:
  MSG_Q_ID que_id;
};

#endif


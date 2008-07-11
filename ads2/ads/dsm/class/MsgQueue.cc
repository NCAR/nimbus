/* MsgQueue.cc
   Class for managing a message queue.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <MsgQueue.h>

/******************************************************************************
** Public Functions
******************************************************************************/
 
MsgQueue::MsgQueue (const int size, int msg_num)
 
// Class constructor.
{
  if (!(int)(que_id = msgQCreate (msg_num, size, MSG_Q_FIFO))) {
    perror ("MsgQueue:msgQCreate");
    exit(ERROR);
  }
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/


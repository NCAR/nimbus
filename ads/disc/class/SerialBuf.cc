/* SerialBuf.cc
   Creates and manages a serial access data buffer.

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <SerialBuf.h>
 
/******************************************************************************
** Public Functions
******************************************************************************/
 
SerialBuf::SerialBuf (int size)
{
// Allocate the buffer plus some overflow space for safety.
  if (!(int)(buffer = (char*)malloc (size + SERBUF_OVERFLOW))) {
    perror ("SerialBuf:malloc");
    exit(ERROR);
  }

  buf_size = size;
  buf_idx = 0;
  buf_full = false;
}

/*****************************************************************************/
void SerialBuf::incrBufIndex(int len)
 
// Increments the buffer index by len.
{
  if ((buf_idx += len) >= buf_size) 
    buf_full = true;
}

/*****************************************************************************/
int SerialBuf::copyToBuf(char *src, int len)

// Copies len bytes to the buffer.
{
  if (spaceAvail() >= len)
    {
    memcpy((void *)&buffer[buf_idx], (void *)src, len);
    incrBufIndex(len);
    return(true);
    }

  return(false);
}

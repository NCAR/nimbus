/* NoNoy.cc
 
   NO_NOY  class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <NoNoy.h>

/******************************************************************************
** Public Functions
******************************************************************************/

NoNoy::NoNoy (const char *base, const int port, 
                void (*statMsg)(const char* msg)) :
                IsioPort (base, port, NONOY_BAUD, NONOY_BITS, NONOY_PARITY,
                NONOY_STOP, NONOY_FLOW, statMsg), statusMsg (statMsg)

// Constructor. The interrupt routine must be previously set up outside this 
// class.
{
  memset (buf, 0, sizeof (buf));
  term_str[0] = '\r';
  term_str[1] = '\n';
  term_str[2] = '\0';
  igetstr (term_str);			// issue first read
}
/*****************************************************************************/

void NoNoy::checkStatus()

// Check the serial port status.
{
  if (checkPort() != TRUE) {
    igetstr (term_str);         // new read if an error or dropped interrupt
    statusMsg ("JplTdl: Dropped isio interrupt.\n");
  } 
}
/*****************************************************************************/

void NoNoy::isr()
 
// Pulls in data from the isio and builds the NoNoy data struct.
{
  char *datap;
  float a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,analog[10];
  char stars[2],other,time[8],state;
  short c0,c1,c2,c3,pio;

// Check for data from the isio port.
  if (!(int)(datap = getData()))
    return;
 
// Pull in the data.
  memcpy (buf, datap, NONOY_DATA_LEN);
 
// Check the data alignment.
  if ((buf[NONOY_SYNC0_IDX]==NONOY_SYNC0) && 
      (buf[NONOY_SYNC1_IDX] > NONOY_SYNC1) && 
      (buf[NONOY_SYNC1_IDX] < NONOY_SYNC2)) {

// Parse the NONOY 3 data string and build the recorded data block.
    sscanf (buf, "%s %s %hd %s %hd %hd %hd %hd %f %f %f %f %f %f %f %f %f %s", 
            &stars, &time, &pio, &state, &c0, &c1, &c2, &c3, &analog[0], 
            &analog[1], &analog[2], &analog[3], &analog[4], &analog[5], 
            &analog[6], &analog[7], &analog[8], &analog[9], &other);

    sscanf(time, "%hd:%hd:%hd",&nonoy_blk.hour,&nonoy_blk.min,&nonoy_blk.min);
    nonoy_blk.label = stars[1];
    nonoy_blk.pio = pio;
    nonoy_blk.state = state;
    nonoy_blk.c0 = c0;
    nonoy_blk.c1 = c1;
    nonoy_blk.c2 = c2;
    nonoy_blk.c3 = c3;
    memcpy (nonoy_blk.analog, analog, sizeof(float)*10);
    nonoy_blk.other = other;
  }
 
  igetstr (term_str);                        // issue next read
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

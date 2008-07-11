/* Ozone.cc
 
   NOAA instruments  class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <Ozone.h>

/******************************************************************************
** Public Functions
******************************************************************************/

Ozone::Ozone (const char *base, const int port, 
                void (*statMsg)(const char* msg)) :
                IsioPort (base, port, OZONE_BAUD, OZONE_BITS, OZONE_PARITY,
                OZONE_STOP, OZONE_FLOW, statMsg), statusMsg (statMsg)

// Constructor. The interrupt routine must be previously set up outside this 
// class.
{
  memset (buf, 0, sizeof (buf));
  term_str[0] = '\r';
  term_str[1] = '\n';
//  term_str[2] = '\0';
  igetstr (term_str);			// issue first read
}
/*****************************************************************************/

void Ozone::checkStatus()

// Check the serial port status.
{
  if (checkPort() != TRUE) {
    igetstr (term_str);         // new read if an error or dropped interrupt
    statusMsg ("Ozone: Dropped isio interrupt.\n");
  } 
}
/*****************************************************************************/

void Ozone::isr()
 
// Pulls in data from the isio and builds the Ozone data struct.
{
  char *datap;
  float ozone, press;
  char stars[2],time[9] ;

// Check for data from the isio port.
  if (!(int)(datap = getData()))
    return;

// Pull in the data.
  memcpy (buf, datap, strlen(datap));
// Check the data alignment.
  if ((buf[OZONE_SYNC0_IDX]==OZONE_SYNC0) && (buf[OZONE_SYNC1_IDX]==OZONE_SYNC1)) {

// Parse the Ozone data string and build the recorded data block.
    sscanf (buf, " %s %s %f %f ",stars,time,&press,&ozone);

    sscanf(time, "%d:%d",&ozone_blk.day,&ozone_blk.time);
    ozone_blk.ozone = ozone;
    ozone_blk.press = press;
    printf("ozone = %f press = %f\n",ozone,press);
  }
 
  igetstr (term_str);                        // issue next read
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

/* JplTdl.cc
 
   JPL TDL  class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <JplTdl.h>

/******************************************************************************
** Public Functions
******************************************************************************/

JplTdl::JplTdl (const char *base, const int port, 
                void (*statMsg)(const char* msg)) :
                IsioPort (base, port, JPLTDL_BAUD, JPLTDL_BITS, JPLTDL_PARITY,
                JPLTDL_STOP, JPLTDL_FLOW, statMsg), statusMsg (statMsg)

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

void JplTdl::checkStatus()

// Check the serial port status.
{
  if (checkPort() != TRUE) {
    igetstr (term_str);         // new read if an error or dropped interrupt
    statusMsg ("JplTdl: Dropped isio interrupt.\n");
  } 
}
/*****************************************************************************/

void JplTdl::isr()
 
// Pulls in data from the isio and builds the JplTdl data struct.
{
  char *datap;
  float vmr, temp, press;
  char stars[2] ;

// Check for data from the isio port.
  if (!(int)(datap = getData()))
    return;
 
// Pull in the data.
  memcpy (buf, datap, strlen(datap));
 
// Check the data alignment.
  if ((buf[JPL_SYNC0_IDX]==JPL_SYNC0) && (buf[JPL_SYNC1_IDX]==JPL_SYNC1)) {

// Parse the JPL TDL 3 data string and build the recorded data block.
    sscanf (buf, " %s  %f  %f  %f", &stars, &vmr, &press, &temp);

    jpltdl_blk.vmr = vmr;
    jpltdl_blk.press = press;
    jpltdl_blk.temp = temp;
  }
 
  igetstr (term_str);                        // issue next read
/*
char *p;
p=strtok(buff," \t,");
jpltdl_blk.vmr=atoi(p);
do
{
  process(p);
}
while((p=strtok(NULL," \t\n\r,"))!=NULL);
*/
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

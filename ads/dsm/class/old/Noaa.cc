/* Noaa.cc
 
   NOAA instruments  class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <Noaa.h>

/******************************************************************************
** Public Functions
******************************************************************************/

Noaa::Noaa (const char *base, const int port, 
                void (*statMsg)(const char* msg)) :
                IsioPort (base, port, NOAA_BAUD, NOAA_BITS, NOAA_PARITY,
                NOAA_STOP, NOAA_FLOW, statMsg), statusMsg (statMsg)

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

void Noaa::checkStatus()

// Check the serial port status.
{
  if (checkPort() != TRUE) {
    igetstr (term_str);         // new read if an error or dropped interrupt
    statusMsg ("Noaa: Dropped isio interrupt.\n");
  } 
}
/*****************************************************************************/

void Noaa::isr()
 
// Pulls in data from the isio and builds the JplTdl data struct.
{
  char *datap;
  float lah2o, fph0,fph1, press;
  char stars[2],time[9] ;

// Check for data from the isio port.
  if (!(int)(datap = getData()))
    return;

// Pull in the data.
  memcpy (buf, datap, strlen(datap));
// Check the data alignment.
  if ((buf[NOAA_SYNC0_IDX]==NOAA_SYNC0) && (buf[NOAA_SYNC1_IDX]==NOAA_SYNC1)) {

// Parse the NOAA data string and build the recorded data block.
    sscanf (buf, " %s %s %f %f %f %f",stars,time,&press,&lah2o,&fph0,&fph1);

    sscanf(time, "%d:%d",&noaa_blk.day,&noaa_blk.time);
    noaa_blk.lah2o = lah2o;
    noaa_blk.press = press;
    noaa_blk.fph0 = fph0; 
    noaa_blk.fph1 = fph1;
//    printf("lah2o = %f press = %f fph0 = %f fph1 = %f\n",lah2o,press,fph0,fph1);
//    printf("day = %d time = %d\n",noaa_blk.day,noaa_blk.time);
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

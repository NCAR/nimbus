/*SerialTod.h
 
   Serial Time-of-Day class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/
 
#include <SerialTod.h>

/******************************************************************************
** Public Functions
******************************************************************************/

SerialTod::SerialTod (const char *base, const int port,
              void (*statMsg)(char* msg), Bc635Vme&tp) :
              IsioPort (base, port, SERTOD_BAUD, SERTOD_BITS, 
              SERTOD_PARITY, SERTOD_STOP, SERTOD_FLOW, statMsg), 
              statusMsg (statMsg),tfp(tp)

// Constructor. The interrupt routine must be previously set up outside this
// class.
{
  st_datap = (int*)&st_data;
  memset ((char*)st_datap, 0, sizeof(st_data));
}
/*****************************************************************************/
 
void SerialTod::sendtod()
 
// Sends the time-of-day to the serial port
{ 
  char buff[27];
  int j;

  st_data.hours = tfp.hour();
  st_data.minutes = tfp.minute();
  st_data.seconds = tfp.second();
  st_data.days = tfp.julianDay();
  sprintf(buff,"DATE  %d  TIME  %02d:%02d:%02d\r\n",
          st_data.days,st_data.hours,st_data.minutes,st_data.seconds);
  pputcnt ((char*)buff, SERTOD_LEN);
/*  if (!(int)pputcnt ((char*)buff, SERTOD_LEN))
    statusMsg ("SerialTod: Time-of-day transmitted.\n");
  else
    statusMsg ("SerialTod: Failure sending the time-of-day.\n");
*/
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

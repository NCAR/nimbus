/* TodClock.cc
   Class for interfacing the BC635VME time and frequency processor.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <TodClock.h>

/******************************************************************************
** Public Functions
******************************************************************************/
 
TodClock::TodClock ()
 
// Class constructor.
{
  tod_regs = (TodClockRegs*)TOD_BASE;
}
/*****************************************************************************/

void TodClock::setTime (int new_year, int new_month, int new_date,
                             int new_hour, int new_minute, int new_sec)
 
// Sets the clock from the new time variables.
// The day field is not set by this routine.
{
  char str[40];

// Convert from decimal to bcd.
//  if(new_year >= 100) new_year = 0;
//  if(new_year != 0) new_year = 0;
  sprintf (str, "%02d %02d %02d %2d %02d %02d", new_year, new_month, new_date,
           new_hour, new_minute, new_sec);

  sscanf (str, "%02X %02X %02X %2X %02X %02X", &new_year, &new_month, &new_date,
          &new_hour, &new_minute, &new_sec);

/**
  printf ("   %02X %02X %02X %2X %02X %02X\n", new_year, new_month, new_date,
           new_hour, new_minute, new_sec);
**/

  tod_regs->control = TOD_WRITE;
  tod_regs->second = new_sec;
  tod_regs->minute = new_minute;
  tod_regs->hour = new_hour;
  tod_regs->date = new_date;
  tod_regs->month = new_month;
  tod_regs->year = new_year;
  tod_regs->control = TOD_RUN;
}
/*****************************************************************************/
 
void TodClock::readTime ()
 
// Reads the clock registers.
{
  char str[40];

  tod_regs->control = TOD_READ;

// Convert from bcd to decimal.
  sprintf (str, "%02X %02X %02X %02X %2X %02X", 
           tod_regs->year, tod_regs->month, tod_regs->date,
           tod_regs->hour, tod_regs->minute, tod_regs->second);
  tod_day = tod_regs->day & 0x07; 

  tod_regs->control = TOD_RUN;

  sscanf (str, "%d %d %d %d %d %d", &tod_year, &tod_month, &tod_date,
          &tod_hour, &tod_minute, &tod_second);

// Make sure time is not corrupted, if it is, hardwire to 1/1/95
  if (tod_month < 1 || tod_month > 12 || tod_date < 1 || tod_date > 31) {
    printf("** Corrupted time, setting to 1/1/97 00:00:00.\n");
    setTime(97, 1, 1, 0, 0, 0);
    }

}
/*****************************************************************************/

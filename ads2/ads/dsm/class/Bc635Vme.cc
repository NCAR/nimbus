/* Bc635Vme.cc
   Class for interfacing the BC635VME time and frequency processor.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <Bc635Vme.h>


// Time Request should be issued into global to avoid being optimized out of
// existence.
static short	tr_dummy;

/******************************************************************************
** Public Functions
******************************************************************************/
 
Bc635Vme::Bc635Vme (char *base_adr)
 
// Class constructor.
{
  bc_reg = (Bc635Regs*)base_adr;
  bc_year = 0;
  bc_month = 0;
  bc_day = 0;
  bc_hour = 0;
  bc_minute = 0;
  bc_msec = 0;
  bc_julian_day = 0;
  path_a = '\0';			// set default path packet data
  path_b = '\0';			// set default path packet data

}

/* -------------------------------------------------------------------- */
void Bc635Vme::masterSyncUp()
{

  readTime();
  ourHour = bc_hour;
  ourMinute = bc_minute;
  ourSecond = bc_second;

}
/* -------------------------------------------------------------------- */
void Bc635Vme::syncUp()
{
  int   consecutiveCnt, prevTime, newTime;
  int   theCount = 10;  // n consecutive seconds to count up to...
 
  // Wait for 15 consecutive time-stamps.  This is not part of the constructor
  // becuase the TodClock initializes this after the constructor and destroys
  // the time.
  prevTime = 0;
  for (consecutiveCnt = 0; consecutiveCnt < theCount; )
    {
    readTime();

    if (newSecond())
      {
      newTime = bc_hour * 3600 + bc_minute * 60 + bc_second;
 
      if (newTime != prevTime + 1 || bc_hour > 23 || bc_minute > 59 ||
          bc_second > 59)
        consecutiveCnt = prevTime = 0;
      else
        ++consecutiveCnt;

      clearNewSecond();
      prevTime = newTime;
//      printf("%d %d\r", consecutiveCnt, theCount); fflush(stdout);
      printf("%02d:%02d:%02d\n", bc_hour, bc_minute, bc_second);
      }
 
    taskDelay(sysClkRateGet()>>2);
    }

  readTime();
  ourHour = bc_hour;
  ourMinute = bc_minute;
  ourSecond = bc_second;

}

/* -------------------------------------------------------------------- */
int Bc635Vme::readmsec()
{
  short treg3;

  tr_dummy = bc_reg->timereq;			// issue read strobe

  treg3 = bc_reg->time3 >> BCD_DIGIT_SHIFT;
//logMsg("treg=%x\n", treg3, 0, 0, 0, 0, 0);
  tr_dummy = bc_reg->timereq;			// issue read strobe
  bc_msec = (treg3 & BCD_DIGIT_MASK) +
            (((treg3 >>= BCD_DIGIT_SHIFT) & BCD_DIGIT_MASK) * 10) +
            (((treg3 >>= BCD_DIGIT_SHIFT) & BCD_DIGIT_MASK) * 100);
 
  return(bc_msec);

}

/* -------------------------------------------------------------------- */
void Bc635Vme::readTime()

// Reads the time from the interface.
{
  short treg1, t1;
  short treg2, t2;
  short treg3, t3;

  tr_dummy = bc_reg->timereq;			// issue read strobe

// Read the time registers.
  treg3 = bc_reg->time3 >> BCD_DIGIT_SHIFT;
  treg2 = bc_reg->time2;
  treg1 = bc_reg->time1;
/*
  t1 = (treg1 >>= BCD_DIGIT_SHIFT) & BCD_DIGIT_MASK;
  t2 = (treg2 >>= BCD_DIGIT_SHIFT) & BCD_DIGIT_MASK;
  t3 = (treg3 >>= BCD_DIGIT_SHIFT) & BCD_DIGIT_MASK;
*/
// Read the bcd format time variables.
  bcd_second = treg2 & BCD_2_DIGIT_MASK;
  bcd_minute = (treg2 >> BCD_2_DIGIT_SHIFT) & BCD_2_DIGIT_MASK;
  bcd_hour = treg1 & BCD_2_DIGIT_MASK;

// Read the time, converting from bcd to decimal.
/*
  bc_msec = (treg3 & BCD_DIGIT_MASK) + (t3 * 10) + (t3 * 100);
  bc_second = (treg2 & BCD_DIGIT_MASK) + (t2 * 10);
  bc_minute = t2 + (t2 * 10);
  bc_hour = (treg1 & BCD_DIGIT_MASK) + (t1 * 10);
*/
// Read the time, converting from bcd to decimal.
  bc_msec = (treg3 & BCD_DIGIT_MASK) +
            (((treg3 >>= BCD_DIGIT_SHIFT) & BCD_DIGIT_MASK) * 10) +
            (((treg3 >>= BCD_DIGIT_SHIFT) & BCD_DIGIT_MASK) * 100);
 
  bc_second = (treg2 & BCD_DIGIT_MASK) +
              (((treg2 >>= BCD_DIGIT_SHIFT) & BCD_DIGIT_MASK) * 10);
 
  bc_minute = ((treg2 >>= BCD_DIGIT_SHIFT) & BCD_DIGIT_MASK) +
              (((treg2 >>= BCD_DIGIT_SHIFT) & BCD_DIGIT_MASK) * 10);
 
  bc_hour = (treg1 & BCD_DIGIT_MASK) +
            (((treg1 >>= BCD_DIGIT_SHIFT) & BCD_DIGIT_MASK) * 10);
 
// Read the julian day.
  treg2 = ((treg1 >>= BCD_DIGIT_SHIFT) & BCD_DIGIT_MASK) +
               (((treg1 >>= BCD_DIGIT_SHIFT) & BCD_DIGIT_MASK) * 10) +
               (bc_reg->time0 & BCD_DIGIT_MASK) * 100;

// Check for a new julian day.
  if (bc_julian_day != (int)treg2) {
//    logMsg("\nreadTime: julianDay changed, bc_julian_day=%d, treg2=%d.\n\n",
//       bc_julian_day, treg2, 0, 0, 0, 0);

    if (bc_julian_day && (treg2 == 1)) 	// if not startup, check for new year
      {
      bc_year++;
//      logMsg("**** tfp->bc_year being incremented by 1 ****\n\n",0,0,0,0,0,0);
      }

    bc_julian_day = treg2;
//    decodeJulianDay();
    computeBcdDate();
  }
}

/* -------------------------------------------------------------------- */
int Bc635Vme::readMsec ()
 
// Fast read to get only the current msec.
{
  short treg;
 
  treg = bc_reg->timereq;                       // issue read strobe
  treg = bc_reg->time3 >> BCD_DIGIT_SHIFT;

  return bc_msec = (treg & BCD_DIGIT_MASK) +
                   (((treg >>= BCD_DIGIT_SHIFT) & BCD_DIGIT_MASK) * 10) +
                   (((treg >>= BCD_DIGIT_SHIFT) & BCD_DIGIT_MASK) * 100);
}
 
/* -------------------------------------------------------------------- */
void Bc635Vme::setPeriodicOutput (int rate, int width)
 
// Programs the periodic output. rate(hertz) = BC635_CLOCK_RATE / (n1 * n2).
// The pulse width (usec) = (n1 * 1000000) / BC635_CLOCK_RATE to a max of 50% 
// duty cycle. n1 and n2 must be >= 2.
{
  int n1;
  int n2;
  char sync;

// Calculate the pulse width variable. width is in usec.
  n1 = (width * BC635_CLOCK_RATE) / 1000000;
  n2 = BC635_CLOCK_RATE / (n1 * rate);

// If n1 * n2 is an even divisor of the clock, then operate in the 
// synchronous mode.  Otherwise operate in the asynchronous mode.
  if (((BC635_CLOCK_RATE / (n1 * n2)) * (n1 * n2)) == BC635_CLOCK_RATE) {
    sync = '5';
    n1--;
    n2--;
  }
  else
    sync = '2';

  sprintf (packet, "%c%c%c%04X%04x%c", SOH, 'F', sync, n1, n2, ETB);
//printf ("setPeriodicOutput packet, %s\n", packet);
  writeFifo (packet);
}
 
/* -------------------------------------------------------------------- */
void Bc635Vme::selectTimeCodeMode ()
 
// Selects synchronization to the input irig B time code.
{
  sprintf (packet, "%cA0%c", SOH, ETB); 
  writeFifo (packet);
}
 
/* -------------------------------------------------------------------- */
void Bc635Vme::selectFreeRunMode ()
 
// Selects the free running mode.
{
  sprintf (packet, "%cA1%c", SOH, ETB);
  writeFifo (packet);
}
 
/* -------------------------------------------------------------------- */
void Bc635Vme::select1PPSMode ()
 
// Selects synchronization to the input 1 PPS.
{
  sprintf (packet, "%cA2%c", SOH, ETB);
  writeFifo (packet);
}
 
/* -------------------------------------------------------------------- */
void Bc635Vme::selectRealTimeClockMode ()
 
// Selects the internal real-time clock for the source of time.
{
  sprintf (packet, "%cA3%c", SOH, ETB);
  writeFifo (packet);
//printf ("selectRealTimeClockMode packet = %s, length = %d\n", 
//        packet, strlen(packet));
}
 
/* -------------------------------------------------------------------- */
void Bc635Vme::selectModulatedInput ()
 
// Selects modulated Irig B input format.
{
// Selects modulated Irig B input format.
  sprintf (packet, "%cHBM%c", SOH, ETB);
// Selects DCLS input format.
//  sprintf (packet, "%cHBD%c", SOH, ETB);
  writeFifo (packet);
}

/* -------------------------------------------------------------------- */
void Bc635Vme::selectModulatedOutput ()
 
// Selects modulated Irig B output format.
{
  sprintf (packet, "%cKB%c", SOH, ETB);
  writeFifo (packet);
}

/*****************************************************************************/
void Bc635Vme::setPath ()
 
// Sends the path selection packet.
// Note, on some boards, enabling jamsynchs causes the board to lose lock
// every few seconds, so jamsynchs are disabled here.
{
  sprintf (packet, "%cP%c%c%c", SOH, path_b, path_a, ETB);
  writeFifo (packet);
}

/*****************************************************************************/
void Bc635Vme::loadDA ()
 
// Sends the default D/A control value.
{
  sprintf (packet, "%cD8000%c", SOH, ETB);
  writeFifo (packet);
}

/*****************************************************************************/
void Bc635Vme::setRealTimeClock (int new_year, int new_month, int new_day,
                             int new_hour, int new_minute, int new_sec)
 
// Sets the internal real-time clock from the current time variables.
{
  bc_year = new_year;
  bc_month = new_month;
  bc_day = new_day;
  bc_hour = ourHour = new_hour;
  bc_minute = ourMinute = new_minute;
  bc_second = ourSecond = new_sec;

  setLeapYear();			// compute date variables
  computeJulianDay();
  computeBcdDate();

  sprintf (packet, "%c%c%02d%02d%02d%02d%02d%02d%c", SOH, 'L', bc_year, 
           bc_month, bc_day, bc_hour, bc_minute, bc_second, ETB);
  writeFifo (packet);
}

/*****************************************************************************/
void Bc635Vme::syncRealTimeClock ()
 
// Syncs the internal real-time clock from the current major time.
{
  sprintf(packet, "%cC5%c", SOH, ETB);
  writeFifo(packet);
}

/*****************************************************************************/
void Bc635Vme::setMajorTime (int new_year, int new_month, int new_day,
                             int new_hour, int new_minute, int new_sec)
 
// Sets the major time from the current time variables.
{
  bc_year = new_year;
  bc_month = new_month;
  bc_day = new_day;
  bc_hour = ourHour = new_hour;
  bc_minute = ourMinute = new_minute;
  bc_second = ourSecond = new_sec;

  setLeapYear();			// compute date variables
  computeJulianDay();
  computeBcdDate();
  sprintf(packet, "%cB%03d%02d%02d%02d%c", SOH,
           bc_julian_day, bc_hour, bc_minute, bc_second, ETB);

  writeFifo (packet);
}

/*****************************************************************************/
void Bc635Vme::jamSync()
{
  // Force minor time to zero on the next 1PPS pulse.
  sprintf (packet, "%cC3%c", SOH, ETB);
  writeFifo (packet);
}

/*****************************************************************************/
void Bc635Vme::enableMinorStrobeInt (int vector, int level, int interval)

// Enables the minor time coincidence strobe.  Assumes the isr has already
// been connected.
{
  strobe_msec_intv = interval;
  bc_reg->stat_ctl = 1;			// reset registers
  taskDelay (sysClkRateGet() / 2);
  bc_reg->vector = vector;
  bc_reg->level = level;

  bc_reg->intstat = 4;			// clear the interrupt
  readTime();
  setMinorStrobeTime();
  bc_reg->cr0 |= BC635_STROBE_ENAB + BC635_STROBE_MINOR;
  bc_reg->mask = 4;			// enable strobe interrupt
}
/*****************************************************************************/

void Bc635Vme::strobeIsr ()
{
  bc_reg->intstat = 4;			// clear the interrupt
//  readTime();
  setMinorStrobeTime();

  if (newSecond())
  {
    // Advance time for next round.
    if (++ourSecond > 59)
    {
      ourSecond = 0;
      if (++ourMinute > 59)
      {
        ourMinute = 0;
        if (++ourHour > 23)
	{
          ourHour = 0;
	  readTime();
	}
      }
    }
  }

}

/******************************************************************************
** Private Functions
******************************************************************************/

void Bc635Vme::setMinorStrobeTime ()

// Sets the minor strobe time for the next msec interval.
{
  int treg;
  int m_sec;

  readmsec();
  m_sec = (((bc_msec/strobe_msec_intv) * strobe_msec_intv) + 
          strobe_msec_intv) % 1000;
  treg = (m_sec / 100) << BCD_DIGIT_SHIFT;
  treg += ((m_sec % 100) / 10);
  treg <<= BCD_DIGIT_SHIFT;
  treg += (m_sec % 10);
  treg <<= BCD_DIGIT_SHIFT;
  bc_reg->evt_stb3 = treg;
}
/*****************************************************************************/

void Bc635Vme::writeFifo (char *pkt)

{
  int j;

// Clear the ack bit.
  bc_reg->ack = 0x01;

// Write the packet.
  for (j = 0; j < strlen (pkt); j++) 
    bc_reg->iofifo = *(pkt + j);

// Request the fifo to take action.
  bc_reg->ack = 0x80;

// Wait for completion.
  for (j= 0; (j < 1000) && !(bc_reg->ack & 0x01); j++)
    taskDelay(1);

  if (j >= 1000)
    logMsg ("Bc635Vme: writeFifo error.\n", 0,0,0,0,0,0);
}
/*****************************************************************************/

void Bc635Vme::computeJulianDay ()

// Computes the julian day from the current year, month, day.
{
  int *cal_ptr;                                 // calendar pointer

  static int calendar[BC635_CALENDAR_SIZE] =
         {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
  static int leap_calendar[BC635_CALENDAR_SIZE] =
         {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366};

  if (bc_year % BC635_LEAP_YEAR_MOD) {
    cal_ptr = calendar;
//    logMsg("bc_year = %d, not_leap\n",bc_year,0,0,0,0,0);
  }
  else {
    cal_ptr = leap_calendar;
//    logMsg("bc_year = %d, leap_year\n",bc_year,0,0,0,0,0);
  }

  bc_julian_day = *(cal_ptr + bc_month - 1) + bc_day;
//  logMsg("Julian_day = %d\n",bc_julian_day,0,0,0,0,0);
//  fflush(stdout);
}
/*****************************************************************************/
 
void Bc635Vme::decodeJulianDay ()
 
// Computes the decimal month and day from the current year and julian_day.
{
  int *cal_ptr;                                 // calendar pointer
  int tmp;
 
  static int calendar[BC635_CALENDAR_SIZE] =
         {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
  static int leap_calendar[BC635_CALENDAR_SIZE] =
         {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366};
 
  if (bc_year % BC635_LEAP_YEAR_MOD)
    cal_ptr = calendar;
  else
    cal_ptr = leap_calendar;

// Find the month.
  for (bc_month = 1; (bc_month < BC635_CALENDAR_SIZE) &&
       (bc_julian_day > *(cal_ptr + bc_month));
       bc_month++);

  bc_day = bc_julian_day - *(cal_ptr + bc_month - 1);
}
/*****************************************************************************/
 
void Bc635Vme::computeBcdDate ()

// Compute the bcd date from the current decimal date.
{
  bcd_year = (bc_year / 10) << BCD_DIGIT_SHIFT;
  bcd_year += bc_year % 10;
  bcd_month = (bc_month / 10) << BCD_DIGIT_SHIFT;
  bcd_month += bc_month % 10;
  bcd_day = (bc_day / 10) << BCD_DIGIT_SHIFT;
  bcd_day += bc_day % 10;
}
/*****************************************************************************/
 
void Bc635Vme::setLeapYear ()

// Sets the leap year bit in the path packet data according to the bc_year 
// value.
{
  if (bc_year % BC635_LEAP_YEAR_MOD)
    path_a &= ~BC635_PATH_LEAP_YEAR;			// disable
  else
    path_a |= BC635_PATH_LEAP_YEAR;			// enable

  setPath();
}
/*****************************************************************************/


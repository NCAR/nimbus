/*
   GpsTans2.h
   Trimble Tans I, II, and III GPS receiver class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
*/

#include <GpsTans2.h>

/******************************************************************************
** Public Functions
******************************************************************************/

GpsTans2::GpsTans2 (const char *base, const int port, 
                    void (*stat_msg)(char* msg_str),
                    void (*set_time)(const int year, const int month, 
                                     const int day, const int hour, 
                                     const int minute, const int second)) : 
                    IsioPort (base, port,  GPS_TANS_BAUD, GPS_TANS_BITS, 
                              GPS_TANS_PARITY, GPS_TANS_STOP, GPS_TANS_FLOW,
                              stat_msg),
                    statusMsg (stat_msg),
                    setTime (set_time)
                   
        
// Class constructor.
{
  memset ((char*)&gps_blk, 0, sizeof(gps_blk));
  buf_idx = -1;
  odd_dle = FALSE;
  rx_rqst = TRUE;				
  rx_init = FALSE;
  time_rqst = TRUE;
  time_set = FALSE;
  sec_cnt = 0;
  term_str[0] = (char)DLE;
  term_str[1] = (char)ETX;
  term_str[2] = '\0';
  igetstr (term_str);
}
/*****************************************************************************/

void GpsTans2::checkStatus()

// Checks the status of the gps receiver and isio port.
{
// Check the isio channels. 
  if (checkPort() != TRUE)
    igetstr (term_str);		// new read if an error or dropped interrupt

// If the receiver is not initialized, go send the initialization packet.
  if (rx_rqst) {
    initTans();
  }

  if (rx_init) {
    statusMsg ("Tans2 GPS has been initialized.\n");
    rx_init = FALSE;
  }

  if (time_set) {
    statusMsg ("System time has been set from the Tans2 GPS.\n");
    time_set = FALSE;
  }
}
/*****************************************************************************/

void GpsTans2::setAltitude (const float alt)

// Sends altitude packets to the Trimble gps receiver. 
{
  static char alt_pkt[] = GPS_SET_ALT;
  char tmp_pkt[sizeof (alt_pkt) * 2];
  int dle_cnt;
  int j;

  memcpy (&alt_pkt[GPS_DATA_IDX], &alt, sizeof(float));
  dle_cnt = stuffDle (alt_pkt, tmp_pkt);
  pputcnt (tmp_pkt, sizeof (alt_pkt) + dle_cnt);
}
/*****************************************************************************/

void GpsTans2::isr()

// Isr to read the Trimble Tans2 stream via the ISIO-2. 
{
  register char *src;
  register int j;

// Check for data from the isio port.
  if (!(int)(src = getData()))
    return;

// Move the packet to gps_buf stripping even numbered occurrences of DLE's. 
  for (gps_buf[++buf_idx]= *src++; (buf_idx < GPSREC) && 
       (gps_buf[buf_idx] !=ETX); gps_buf[++buf_idx] = *src++) {
    if (gps_buf[buf_idx] != DLE)
      odd_dle = FALSE;
    else if (!(odd_dle = !odd_dle))
      --buf_idx;
  }

// Issue the next serial port read.
  igetstr (term_str);

//logMsg ("%2X, %2X %2X %2X %2X %2X %2X\n",gps_buf[0], gps_buf[1],
//        gps_buf[2], gps_buf[3], gps_buf[4], gps_buf[5]);
 
// Check for a buffer overflow.
  if (buf_idx >= GPSREC) {
    buf_idx = -1;
    odd_dle = FALSE;
    logMsg ("GpsTans2: Buffer overflow.\n", 0,0,0,0,0,0);
    return;
  }


// The end of a packet is determined by an odd numbered DLE followed by an ETX. 
  if ((odd_dle) && (gps_buf[buf_idx-1] == DLE)) {
    odd_dle = FALSE;
    buf_idx = -1;

    switch (gps_buf[GPS_TYPE_IDX]) {

      case GPS_TIME:
        gps_blk.gpstime = *((float*)&gps_buf[GPS_TIME_IDX]);
        gps_blk.gpsweek = (int)(*((short*)&gps_buf[GPS_WEEK_IDX]));
        gps_blk.gpsutc = *((float*)&gps_buf[GPS_UTC_IDX]);
        if (time_rqst) {
          if (time_set = calcDateTime ((int)gps_blk.gpsweek, 
             (int)gps_blk.gpstime,(int)gps_blk.gpsutc)) {
            setTime (year, month, day, hour, minute, second);
            time_rqst = FALSE;
          }
        }
        break;

      case GPS_SAT_SELECT:
        gps_blk.gpsmode = (int)gps_buf[GPS_DATA_IDX];
        for (j = 0; j < GPS_SATS; j++)
          gps_blk.gpssats[j] = gps_buf[GPS_SAT_IDX + j];
        break;

      case GPS_HEALTH:
        gps_blk.ghealth = (int)(*(short*)&gps_buf[GPS_DATA_IDX]);
        break;

      case GPS_POS_FIX:
        memcpy ((char*)&gps_blk.glat, (char*)&gps_buf[GPS_DATA_IDX],
                GPOSLEN);
        break;

      case GPS_MACHINE_CODE:	
        gps_blk.addstat = (int)gps_buf[GPS_ADDSTAT_IDX];
        break;

      case GPS_IO_OPTIONS:
        rx_init = TRUE;
        rx_rqst = FALSE;
        break;

      case GPS_VEL_FIX:
        memcpy ((char*)&gps_blk.veleast, (char*)&gps_buf[GPS_DATA_IDX],
                GVELLEN);
        break;

      case GPS_RESET:
        rx_rqst = TRUE;
        break;

      default:
        break;
    }
  }
}
/******************************************************************************
** Private Functions
******************************************************************************/
 
int GpsTans2::initTans ()

// Sends initialization packets to the Trimble gps receiver.
// Returns TRUE when all packets have successfully been sent.
{
  int j;
  static char init_pkt[] = GPS_INIT;

  if (!pputcnt (init_pkt, sizeof (init_pkt))) 
    return TRUE;
}
/*****************************************************************************/
 
int GpsTans2::stuffDle (char *src, char *dest)
 
// Stuffs a second DLE into data bytes if a DLE occurs in the data. 
// Returns the number of DLEs stuffed.
{
  int cnt = 0;

// Copy bytes up through the id.
  *dest++ = *src++;
  *dest++ = *src++;
 
// Parse the data bytes stuffing DLE chars, and looking for the DLE-ETX 
// termination.
  while (TRUE) {
    if ((*dest++ = *src++) == DLE) {
      if (*src == ETX) {
        *dest = ETX;
        break;
      }
      else {
        *dest++ = DLE;
        cnt++;
      }
    }
  }
  return cnt;
}
/*****************************************************************************/

int GpsTans2::calcDateTime (int week, int secs, int gps_off)

// Compute the date and time from the week and second numbers received from
// the GPS receiver.  Returns 1 if time is valid, 0 otherwise.
{
  int tot_days;					// total days since 1/1/1980
  int julian_day;				// current julian day
  int *cal_ptr;					// calendar pointer
  static int calendar[CALENDAR_SIZE] =
         {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
  static int leap_calendar[CALENDAR_SIZE] =
         {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366};

// Apply the gps/utc offset. See the Trimble manual.
  secs -= gps_off;
  if (secs < 0)
    return 0;				// time is not yet valid

// Compute the day, hour, minute, and second of the current week.
  day = secs / SECS_PER_DAY; 
  hour = (secs % SECS_PER_DAY) / SECS_PER_HOUR;
  minute = (secs % SECS_PER_HOUR) / SECS_PER_MIN;
  second = (secs % SECS_PER_MIN);

// GPS time begins on 1/6/80.  Compute the total number of days.  Then
// compute the year and julian day.
  tot_days = (week + 1024) * DAYS_PER_WEEK + day + 5;
  year = tot_days / DAYS_PER_YEAR;
  julian_day = tot_days - (year * DAYS_PER_YEAR) + 1;

// Compute number of previous leap days, and correct the julian day.
  julian_day -= (year - 1) / LEAP_YEAR_MOD + 1; 

// If julian day is <= 0, roll back to the previous year, and correct the 
// julian day.
  if (julian_day <= 0) {
    year--;
    if (year % LEAP_YEAR_MOD)
      julian_day += DAYS_PER_YEAR;
    else 
      julian_day += DAYS_PER_LEAP_YEAR;
  }

  if (year % LEAP_YEAR_MOD)
    cal_ptr = calendar;
  else
    cal_ptr = leap_calendar;

// Compute the month index.
  for (month = 1; (month < CALENDAR_SIZE) &&
       (julian_day > *(cal_ptr + month));
       month++);

// Compute the day of the month for the start of the week.
  day = julian_day - *(cal_ptr + month - 1);

// Add the base year, 1980, to the computed year.
  year += GPS_BASE_YEAR;

/**
  printf ("julian day = %d,  %d %d %d %d %d %d\n", julian_day,
     year, month, day, hour, minute, second);
**/         
  return 1;
}
/*****************************************************************************/

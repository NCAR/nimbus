/*
   GpsTans3.h
   Trimble Tans I, II, and III GPS receiver class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
*/

#include <GpsTans3.h>

/******************************************************************************
** Public Functions
******************************************************************************/

GpsTans3::GpsTans3 (const char *base, const int port, 
                    void (*stat_msg)(char* msg_str),
                    void (*set_time)(const int year, const int month, 
                                     const int day, const int hour, 
                                     const int minute, const int second)) : 
                    IsioPort(base, port, GPS3_TANS_BAUD, GPS3_TANS_BITS, 
                             GPS3_TANS_PARITY, GPS3_TANS_STOP, GPS3_TANS_FLOW,
                             stat_msg), 
                    statusMsg (stat_msg),
                    setTime (set_time)
                 
         
        
// Class constructor.
{
  memset ((char*)&gps_blk, 0, sizeof(gps_blk));
  gps_blk.ghealth = 4;	// Set to init mode until first health comes in.

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
  clrbuf();
//  igetstr (term_str);
}
/*****************************************************************************/

void GpsTans3::checkStatus()

// Checks the status of the gps receiver and isio port.
{
// Check the isio channels. 
  if (checkPort() != TRUE) {
//    printf("Tans3 checkPort error\n");
    igetstr (term_str);		// new read if an error or dropped interrupt
  }
// If the receiver is not initialized, go send the initialization packet.
  if (rx_rqst) {
//    printf("Init Tans3 GPS.\n");
    initTans();
  }
  else if (gps_blk.ghealth > 0 || !(sec_cnt++ % 5)) {
    // request rx health every 5 seconds, or if health == 'No GPS time'.
//    printf("Request Tans3 health.\n");
    requestHealth();
  }
  if (rx_init) {
//    printf ("Tans3 GPS has been initialized.\n");
    statusMsg ("Tans3 GPS has been initialized.\n");
    rx_init = FALSE;
  }

  if (time_set) {
//    printf ("System time has been set from the Tans3 GPS.\n");
    statusMsg ("System time has been set from the Tans3 GPS.\n");
    time_set = FALSE;
  }
}
/*****************************************************************************/

void GpsTans3::setAltitude (const float alt)

// Sends altitude packets to the Trimble gps receiver. 
{
  static char alt_pkt[] = GPS3_SET_ALT;
  char tmp_pkt[sizeof (alt_pkt) * 2];
  int dle_cnt;

  memcpy (&alt_pkt[3], &alt, sizeof(float));
  computeCheckSum (alt_pkt);
  dle_cnt = stuffDle (alt_pkt, tmp_pkt);
  pputcnt (tmp_pkt, sizeof (alt_pkt) + dle_cnt);
}
/*****************************************************************************/

void GpsTans3::isr()

// Isr to read the Trimble Tans3 stream via the ISIO-2. 
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

// logMsg ("%2X, %2X %2X %2X %2X %2X %2X\n",gps_buf[0], gps_buf[1],
//         gps_buf[2], gps_buf[3], gps_buf[4], gps_buf[5]);
 
// Check for a buffer overflow.
  if (buf_idx >= GPSREC) {
    buf_idx = -1;
    odd_dle = FALSE;
    logMsg ("GpsTans3: Buffer overflow.\n", 0,0,0,0,0,0);
    return;
  }


// The end of a packet is determined by an odd numbered DLE followed by an ETX. 
  if ((odd_dle) && (gps_buf[buf_idx-1] == DLE)) {
    odd_dle = FALSE;
    buf_idx = -1;

    switch (gps_buf[GPS3_TYPE_IDX]) {
      case GPS3_INFO :	
        gps_blk.addstat = (int)gps_buf[GPS3_ADDSTAT_IDX];
        break;

      case GPS3_TIME:
        gps_blk.gpstime = *((float*)&gps_buf[GPS3_TIME_IDX]);
        gps_blk.gpsweek = (int)(*((short*)&gps_buf[GPS3_WEEK_IDX]));
        gps_blk.gpsutc = *((float*)&gps_buf[GPS3_UTC_IDX]);

        if (time_rqst) {
          if (time_set = calcDateTime((int)gps_blk.gpsweek, 
             (int)gps_blk.gpstime, (int)gps_blk.gpsutc)) {
            setTime(year, month, day, hour, minute, second);

            if (gps_blk.ghealth == 0)  // Health of 3 is 'No GPS time yet'.
              time_rqst = FALSE;
          }
        }
        break;

      case GPS3_SAT_SELECT:
        gps_blk.gpsmode = (int)gps_buf[GPS3_DATA_IDX];
        for (j = 0; j < GPS_SATS; j++)
          gps_blk.gpssats[j] = gps_buf[GPS3_SAT_IDX + j];
        break;

      case GPS3_HEALTH:
        gps_blk.ghealth = (int)gps_buf[GPS3_DATA_IDX];
        break;

      case GPS3_POS_FIX:
        memcpy ((char*)&gps_blk.glat, (char*)&gps_buf[GPS3_DATA_IDX],
                GPOSLEN);
        break;

      case GPS3_IO_OPTIONS:
        rx_init = TRUE;
        rx_rqst = FALSE;
        break;

      case GPS3_VEL_FIX:
        memcpy ((char*)&gps_blk.veleast, (char*)&gps_buf[GPS3_DATA_IDX],
                GVELLEN);
        break;

      case GPS3_RESET:
        rx_rqst = TRUE;
        break;

      case GPS3_FIX_INFO:
      case GPS3_PKT_STATUS:
        break;

      default:
        break;
    }
  }
}
/******************************************************************************
** Private Functions
******************************************************************************/
 
int GpsTans3::initTans ()

// Sends initialization packets to the Trimble gps receiver.
// Returns TRUE when all packets have successfully been sent.
{
  int j;
  static char init_pkt[] = GPS3_INIT;

  if (!pputcnt (init_pkt, sizeof (init_pkt))) {
    igetstr (term_str);
    return TRUE;
  }
}
/*****************************************************************************/
 
void GpsTans3::requestHealth ()
 
// Sends a health request packet.
{
  static char health_pkt[] = GPS3_RQST_HEALTH;
 
  pputcnt (health_pkt, sizeof (health_pkt));
}
/*****************************************************************************/
 
int GpsTans3::computeCheckSum (char *pkt)

// Computes the packet check sum.
{
  int j;
  int len;
  int sum;

// Get the length of the data bytes, minus the length of the checksum field. 
  len = *(pkt + GPS3_LEN_IDX) - 1;

// Sum the byte count and data bytes;
  for (j = 0, sum = 0; j < len; j++)
    sum += *(pkt + GPS3_LEN_IDX + j);

// Checksum plus the sum == 0.
    *(pkt + GPS3_LEN_IDX + j) = -sum;
}
/*****************************************************************************/
 
int GpsTans3::stuffDle (char *src, char *dest)
 
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

int GpsTans3::calcDateTime (int week, int secs, int gps_off)

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
    return(0);				// time is not yet valid

// Compute the day, hour, minute, and second of the current week.
  day = secs / SECS_PER_DAY; 
  hour = (secs % SECS_PER_DAY) / SECS_PER_HOUR;
  minute = (secs % SECS_PER_HOUR) / SECS_PER_MIN;
  second = (secs % SECS_PER_MIN);

// GPS time begins on 1/6/80.  Compute the total number of days.  Then
// compute the year and julian day.
  tot_days = (week + 1024) * DAYS_PER_WEEK + day + 5;
  year = tot_days / DAYS_PER_YEAR;
//  if (year == 20) year = 0;
//  printf("Year = %d\n",year);
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
  year += GPS3_BASE_YEAR;
  if (year == 100) year = 0;

//logMsg ("julian day = %d, %04d/%02d/%02d ", julian_day, year, month, day,0,0);
//logMsg("%02d:%02d:%02d\n", hour, minute, second,0,0,0);

  return(1);
}
/*****************************************************************************/

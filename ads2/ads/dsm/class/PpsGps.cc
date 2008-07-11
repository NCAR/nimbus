/* PpsGps.cc
 
   PPS GPS class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/
 
#include <PpsGps.h>

/******************************************************************************
** Public Functions
******************************************************************************/

PpsGps::PpsGps (const char *base, const int port,
              void (*statMsg)(char* msg),
              void (*set_time)(const int year, const int month,
                               const int day, const int hour,
                               const int minute, const int second)) :
              IsioPort (base, port, PPSGPS_BAUD, PPSGPS_BITS, PPSGPS_PARITY,
                        PPSGPS_STOP, PPSGPS_FLOW, statMsg),
              statusMsg (statMsg),
              setTime (set_time)

// Constructor. The interrupt routine must be previously set up outside this
// class.
{
  align_cnt = 0;			// misaligned data byte count
  ptog = 0;
  gtog = 0;
  timeRqst = TRUE;
  memset ((char*)ppsgps_blk, 0, sizeof(ppsgps_blk));

  igetcnt (PPSGPS_SMP_ONE);				// issue first read
}
/*****************************************************************************/

void PpsGps::checkStatus()

// Checks the status of the PPS GPS isio port.
{
  if (checkPort() != TRUE) {
    igetcnt (PPSGPS_SMP_ONE);			// issue new read
    statusMsg ("PpsGps: Dropped isio interrupt.\n");
  }
}
/*****************************************************************************/

void PpsGps::secondAlign ()

// This routine is to be called at each 1 second clock tick. The PpsGps_blk
// buffers are toggled.
{
  int ttog;

  ttog = (ptog + 1) % TOG;
  gtog = ptog;
  ptog = ttog;
}
/*****************************************************************************/

void PpsGps::isr ()

// Pulls in the Pps Gps sample block, checks for data alignment, issues a
// new read command.
{
  int	j;
  char	*datap, tmp;

// Check for data from the isio port.
  if (!(int)(datap = getData()))
    return;

// Check alignment:

  if (align_cnt == FALSE) {
    if (*datap == PPSGPS_ALIGN_CHAR1) {
      align_cnt = 1;
      igetcnt(PPSGPS_SMP_ONE);
    }
    else {
      align_cnt = FALSE;
      igetcnt(PPSGPS_SMP_ONE);
    }

    return;
  }    

  // Alright we have what could be the start of a block.
  if (align_cnt == 1) {		// already have 0xFF
    if (*datap == PPSGPS_ALIGN_CHAR2) {
      align_cnt = 2;
      igetcnt(PPSGPS_SMP_FOUR);
    }
    else if (*datap == PPSGPS_ALIGN_CHAR1) {
      align_cnt = 1;
      igetcnt(PPSGPS_SMP_ONE);
    }  
    else {
      align_cnt = FALSE;
      igetcnt(PPSGPS_SMP_ONE);
    }

    return;
  }

  if (align_cnt == 2) {
    if(datap[2] == PPSGPS_LEN_CHAR){
      align_cnt = 5;
      igetcnt(PPSGPS_SMP_LEN);
    }
    else {
      align_cnt = FALSE;
      igetcnt(PPSGPS_SMP_ONE);
    }

    return;
  }


  // We have a full record, byte swap it, and copy it out of here.
  if (align_cnt == 5) {
    align_cnt = 0;
    for (j = 0; j < PPSGPS_SMP_LEN; j += 2) {
      tmp = datap[j];
      datap[j] = datap[j+1];
      datap[j+1] = tmp;
    }

    memcpy((char *)&ppsgps_blk[ptog], datap, 4);
    ppsgps_blk[ptog].gps_time = convertDouble((char *)&datap[4]);
    ppsgps_blk[ptog].utc_time = convertDouble((char *)&datap[12]);
    memcpy((char *)&ppsgps_blk[ptog].resv, &datap[20], PPSGPS_SMP_LEN-12);

    // If UTC time is valid let's set the time on the TFP board.
    if (timeRqst && (ppsgps_blk[ptog].data_used & 0x0100)) {
      decodeAndSetTime(&ppsgps_blk[ptog]);
      timeRqst = FALSE;
    }

    igetcnt(PPSGPS_SMP_ONE);
    return;
  }


  // We shouldn't get here, but this is the catch all.
  align_cnt = FALSE;
  igetcnt(PPSGPS_SMP_ONE);

}
/******************************************************************************
** Private Functions
******************************************************************************/

float PpsGps::convertDouble(char *dataP)
{
  unsigned long	msi, lsi;
  int		sign, exponant;
  double	seconds;

  // Pick the DEC double apart into words.  utc_time is seconds since midnight.
  lsi = *(int*)(dataP+4);
  msi = *(int*)(dataP);
 
  sign = msi & 0x80000000;

  // Remove the DEC exponant (7 bits, excess 127), and put into IEEE exponant
  // (11 bits, excess 1024).
  exponant = msi & 0xef800000;
  exponant <<= 1; exponant >>= 24;
  exponant += 126; exponant -=1024;
 
  // Shift Mantissa down 3 bits to make room for expanded exponant.
  lsi = (lsi >> 3) | ((msi & 0x0007) << 29);
  msi = ((msi >> 3) & 0x000fffff) | sign | ((exponant << 20) & 0x7ff00000);
 
  // Put the pieces together.
  *(int*)&seconds = msi;
  *(int*)(((char*)&seconds)+4) = lsi;
 
  return((float)seconds);
}

/* -------------------------------------------------------------------- */
void PpsGps::decodeAndSetTime(Ppsgps_blk *gpsP)
{
  int	year, month, day, hour, minute, second;
  int	doy, *cal_ptr;
  float utcTime;

  static int calendar[] =
         {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
  static int leap_calendar[] =
         {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366};
 
  utcTime = gpsP->utc_time;

  hour	= (int)utcTime / 3600;	utcTime -= hour * 3600;
  minute = (int)utcTime / 60;	utcTime -= minute * 60;
  second = (int)utcTime;


  // Date.
  doy = (((gpsP->day_of_year & 0xf000) >> 12) * 100) + 
        (((gpsP->day_of_year & 0x0f00) >> 8) * 10) +
        ((gpsP->day_of_year & 0x00f0) >> 4);
 
  year = (((gpsP->time_fom & 0x00f0) >> 4) * 10) + (gpsP->time_fom & 0x000f);

  if (year % 4)
    cal_ptr = calendar;
  else
    cal_ptr = leap_calendar;

// Find the month.
  for (month = 1; (month < 13) && (doy > *(cal_ptr + month)); ++month)
    ;
 
  day = doy - *(cal_ptr + month - 1);

  setTime(year, month, day, hour, minute, second);
 
}

/* END PPSGPS.CC */

/* ArincGps.cc

   GPS arinc 429 interface class.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <ArincGps.h>

/******************************************************************************
** Public Functions
******************************************************************************/

ArincGps::ArincGps (IP429 *ip, Bc635Vme &tp, int txchan) : 
            tfp (tp)
         
// Constructor.
{
  ip429 = ip;
  tx_chan = txchan;

//  ip429->setSpeedLow (tx_chan);		// transmit at low speed
  ip429->setSpeedHigh (tx_chan);		// transmit at high speed
  new_arincgps = FALSE;

// Set arinc block pointer and index.
  arincgpsp = (int*)&arincgps;		
  idx = 0;
}
/*****************************************************************************/
 
void ArincGps::buildArincGps ()

// Builds the AimrArinc block except for the flight, tas and palt fields.
// This routine is called once per second.
{

  int i;
//printf ("arinc.gmt = 0x%X\n", arinc.gmt);
/**
  arincgps.meas_status = ((int)(0x00000000) & 0xfffff800) +
               ARINC_GPS_MEAS_STATUS_LABEL;
  arincgps.utc_time = (tfp.bcdHour() << ARINC_HOUR_SHIFT) +
              (tfp.bcdMinute() << ARINC_MINUTE_SHIFT) +
              (tfp.bcdSecond() << ARINC_SECOND_SHIFT) +
              ARINCGPS_SIGN_MASK + ARINC_GPS_UTC_TIME_LABEL;
  arincgps.hdop = ((int)(0x00abc000) & 0xfffff800) +
               ARINC_GPS_HDOP_LABEL;
  arincgps.vdop = ((int)(0x00abc000) & 0xfffff800) +
               ARINC_GPS_VDOP_LABEL;

  arincgps.altitude = ((int)(0x4e200<<10) & 0xfffff800) +
               ARINC_GPS_GPS_ALT_LABEL;
  arincgps.latitude = ((int)(0x31ae0<<10) & 0xfffff800) +
               ARINC_GPS_LAT_LABEL;
  arincgps.longitude = ((int)(0x950a1<<10) & 0xfffff800) +
               ARINC_GPS_LON_LABEL;
  arincgps.ground_speed = ((int)(0x640<<16) & 0xfffff800) +
               ARINC_GPS_GND_SPEED_LABEL;

  arincgps.lat_fine = ((int)((10122.0/131072.0)*2147483648.0) & 0xfffff800) +
               ARINC_GPS_LAT_FINE_LABEL;
  arincgps.long_fine = ((int)((10122.0/131072.0)*2147483648.0) & 0xfffff800) +
               ARINC_GPS_LON_FINE_LABEL;
  arincgps.vertical_fom = ((int)((10122.0/131072.0)*2147483648.0)&0xfffff800) +
               ARINC_GPS_VERTICAL_FOM_LABEL;
  arincgps.vertical_velocity=((int)((10122.0/131072.0)*2147483648.0)&0xfffff800)
               + ARINC_GPS_VERTICAL_VEL_LABEL;
  arincgps.ns_velocity = ((int)((10122.0/131072.0)*2147483648.0) & 0xfffff800) +
               ARINC_GPS_NS_VEL_LABEL;
  arincgps.ew_velocity = ((int)((10122.0/131072.0)*2147483648.0) & 0xfffff800) +
               ARINC_GPS_EW_VEL_LABEL;
  arincgps.horizontal_fom =((int)((10122.0/131072.0)*2147483648.0)&0xfffff800)+
               ARINC_GPS_HOR_FOM_LABEL;
  arincgps.sensor_status = ((int)((10122.0/131072.0)*2147483648.0)&0xfffff800)+
               ARINC_GPS_SENSOR_STAT_LABEL;
**/
  arincgps.integ_vert_accel[0] = ((int)((10122.0/131072.0)*2147483648.0)
                                 & 0xfffff800) + ARINC_INTEG_VERT_ACCEL_LABEL;
  arincgps.integ_vert_accel[1] = ((int)((10122.0/131072.0)*2147483648.0)
                                 & 0xfffff800) + ARINC_PITCH_LABEL;
  arincgps.integ_vert_accel[2] = ((int)((10122.0/131072.0)*2147483648.0)
                                 & 0xfffff800) + ARINC_ROLL_LABEL;
/*
  arincgps.integ_vert_accel[3] = ((int)((10122.0/131072.0)*2147483648.0)
                                 & 0xfffff800) + ARINC_PITCH_RATE_LABEL;
  arincgps.integ_vert_accel[4] = ((int)((10122.0/131072.0)*2147483648.0)
                                 & 0xfffff800) + ARINC_ROLL_RATE_LABEL;
  arincgps.integ_vert_accel[5] = ((int)((10122.0/131072.0)*2147483648.0)
                                 & 0xfffff800) + ARINC_YAW_RATE_LABEL;
  arincgps.integ_vert_accel[6] = ((int)((10122.0/131072.0)*2147483648.0)
                                 & 0xfffff800) + ARINC_LONG_ACCEL_LABEL;
  arincgps.integ_vert_accel[7] = ((int)((10122.0/131072.0)*2147483648.0)
                                 & 0xfffff800) + ARINC_LAT_ACCEL_LABEL;
  arincgps.integ_vert_accel[8] = ((int)((10122.0/131072.0)*2147483648.0)
                                 & 0xfffff800) + ARINC_NORMAL_ACCEL_LABEL;
  arincgps.integ_vert_accel[9] = ((int)((10122.0/131072.0)*2147483648.0)
                                 & 0xfffff800) + ARINC_TRACK_ANG_RATE_LABEL;
  arincgps.integ_vert_accel[10] = ((int)((10122.0/131072.0)*2147483648.0)
                                 & 0xfffff800) + ARINC_PITCH_ATT_RATE_LABEL;
  arincgps.integ_vert_accel[11] = ((int)((10122.0/131072.0)*2147483648.0)
                                 & 0xfffff800) + ARINC_ROLL_ATT_RATE_LABEL;
  arincgps.integ_vert_accel[12] = ((int)((10122.0/131072.0)*2147483648.0)
                                 & 0xfffff800) + ARINC_POT_VERT_SPEED_LABEL;
  arincgps.integ_vert_accel[13] = ((int)((10122.0/131072.0)*2147483648.0)
                                 & 0xfffff800) + ARINC_VERT_ACCEL_LABEL;
  for(i=14;i<55;i++)
  arincgps.integ_vert_accel[i] = ((int)((10122.0/131072.0)*2147483648.0)
                                 & 0xfffff800) + ARINC_DUMMY_LABEL;
*/
  new_arincgps = TRUE;		// set new block available flag
//  printf("arincgps.meas_status %x \n",arincgps.meas_status);
}
/*****************************************************************************/
 
void ArincGps::sendArincGps ()

// Sends the current arinc data to the Arinc.
{
  int len;
  int gap;

// If a new arinc block is available, then send it in IP429_FIFO_SIZE sections.
//  printf("new_arincgps %d \n",new_arincgps);
//  len = IP429_FIFO_SIZE;
    len = 2;
  if (new_arincgps) {
    new_arincgps = FALSE;
    idx = 0;
  }
  else
    idx += len;
//  ip429->write429 (tx_chan, arincgpsp, 4, 88);
//  new_arincgps = FALSE;			
// Determine the number of words and gap time for this section. 
    if ((ARINCGPS_BLOCK_SIZE - idx) > IP429_FIFO_SIZE) {
      gap = ARINCGPS_BLOCK_INTER_GAP;
    }
    else {
      gap = ARINCGPS_BLOCK_LAST_GAP;
    }
//  printf("sendarincb %d %d %d %d \n",tx_chan,idx,len,gap);
  ip429->write429 (tx_chan, arincgpsp + idx, len, gap);// send the data 
}
/******************************************************************************
** Private Functions
******************************************************************************/

/* HwGps.cc

   Honeywell GPS interface class.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <HwGps.h>

/******************************************************************************
** Public Functions
******************************************************************************/

HwGps::HwGps (IP429 *ip, int rxchan) 
         
// Constructor.
{
  ip429 = ip;
  rx_chan = rxchan;

  ptog = 0;
  gtog = 0;
  new_sec = FALSE;
  memset ((char*)hgps_blk, 0, sizeof (hgps_blk));
  cur_lat = 0;
  cur_lon = 0;

  ip429->setSpeedLow(rx_chan);
}
/*****************************************************************************/
 
void HwGps::secAlign ()

// Called at the start of a second to align the indices and toggle buffers.
{
  new_sec = TRUE;			// set the new second flag
  gtog = ptog;                   	// toggle the get buffer index 
}
/*****************************************************************************/

void HwGps::readgpsIsr ()

// Sampling isr. 
{
  int arinc_data;
  int temp;
//  printf("IN ISR \n");
// Read the data words from the arinc interface.
  *(short*)(((char*)&arinc_data) + sizeof(short)) = ip429->data1 (rx_chan);
  *(short*)&arinc_data = ip429->data2 (rx_chan);

// Process the data. 
  temp = arinc_data & ARINC_LABEL_MASK;
  logMsg("Label = %03o data = 0x%x\n",temp,arinc_data>>10,0,0,0,0);
  switch (arinc_data & ARINC_LABEL_MASK) {  	// switch on the arinc label
 
// The 1 Hz parameters 
    case 060:			// first 50 hz param, starts a new block
      if (new_sec) {					// check for new second
        togglePut();					// toggle put buffer
        new_sec = FALSE;
      }
      hgps_blk[ptog].meas_status = arinc_data;
      break;
    case 074:
      hgps_blk[ptog].utc_time = arinc_data;
      break;
    case 076:
      hgps_blk[ptog].altitude = arinc_data;
      break;
    case 0101:
      hgps_blk[ptog].hdop = arinc_data;
      break;
    case 0102:
      hgps_blk[ptog].vdop = arinc_data;
      break;
    case 0110:
//    case 0310:
      hgps_blk[ptog].latitude= arinc_data;
      cur_lat = arinc_data;
      break;
    case 0111:
//    case 0311:
      hgps_blk[ptog].longitude = arinc_data;
      cur_lon = arinc_data;
      break;
    case 0112:
      hgps_blk[ptog].ground_speed = arinc_data;
      break;
    case 0120:
      hgps_blk[ptog].lat_fine = arinc_data;
      break;
    case 0121:
      hgps_blk[ptog].long_fine = arinc_data;
      break;
    case 0136:
      hgps_blk[ptog].vertical_fom = arinc_data;
      break;
    case 065:
      hgps_blk[ptog].vertical_velocity = arinc_data;
      break;
    case 0166:
      hgps_blk[ptog].velocity_ns = arinc_data;
      break;
    case 0174:
      hgps_blk[ptog].velocity_ew = arinc_data;
      break;
    case 0247:
      hgps_blk[ptog].horizontal_fom = arinc_data;
      break;
    case 0273:
      hgps_blk[ptog].sensor_status = arinc_data;
      break;
  }
}
/******************************************************************************
** Private Functions
******************************************************************************/

void HwGps::togglePut ()

// Fills in any missing samples due to walking clocks, resets the buffer
// indices, and toggles the put index.
{
  int dtog;
  dtog = ++ptog % TOG;

  hgps_blk[ptog] = hgps_blk[dtog];

  ptog = ++ptog % TOG;
}
/*****************************************************************************/


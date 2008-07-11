/* IP429.cc

   IP429-1 Arinc 429 Industry Pack driver.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 

*/

#include <IP429.h>

/******************************************************************************
** Public Functions
******************************************************************************/

IP429::IP429 (char *base_adr, char vector)

// Constructor.
{
// Set the register pointers.
  ip_ctl = (Ip429Ctl*)base_adr;
  ip_data = (Ip429Data*)(base_adr + IP429_DATA_REG_OFFSET);

// Issue a reset.
  ip_ctl->reset = (char)0;

  taskDelay (sysClkRateGet() / 10);

// Init the control registers for no loopback and high speed.
  ctl_a = IP429_CTL_NO_LOOPBACK;
  ip_data->ctl_a = ctl_a;

  ctl_b = ctl_a;
  ip_data->ctl_b = ctl_b;
                                              
// Enable the transmitters and set the gap time resolution to 1 ms per count
  ip_ctl->tx4 = IP429_TX_GAP_SCALE_HIGH | IP429_TX_ENABLE;
  ip_ctl->tx5 = IP429_TX_GAP_SCALE_HIGH | IP429_TX_ENABLE;
  ip_ctl->sync = 0;			// no sync output

// Load the interrupt vector. Set the master interrupt enable bits.
  ip_ctl->int_vect = vector;
  ip_ctl->int_enable = IP429_IE_TEIN | IP429_IE_REIN;

}
/*****************************************************************************/

int IP429::write429 (int chan, int *data, int len, int go)

// Writes data to a specified channel. If called from an isr, this also
// clears the interrupt on this channel.
{
  int j;
  int k;

// Make sure the transmitter is disabled before loading the fifo.
  if (chan == 4) {
    if (ip_ctl->tx4 & IP429_TX_ENTX)	
      return 0;
  }
  else if (ip_ctl->tx5 & IP429_TX_ENTX)
      return 0;

// Load the fifo.
  for (j = 0; j < len; j++) { 
//    logMsg("data1 = %X data2 = %X\n",ip_data->chan[chan].data1,
//                                     ip_data->chan[chan].data2,0,0,0,0);
    ip_data->chan[chan].data1 = *((short*)(data + j) + 1); // least sig word
    ip_data->chan[chan].data2 = *(short*)(data + j);	   // most sig word
  }

// Load the gap timer. This starts the transmitter.
  if (chan == 4)
    ip_ctl->go4 = go;
  else 
    ip_ctl->go5 = go;

  return len;
}
/*****************************************************************************/

void IP429::setSpeedLow (int chan)

// Sets the speed low (12.5 Khz) for the requested channel.
{
  switch (chan) {
    case 0:
    case 1:
      ctl_a |= IP429_CTL_RX_LOW_SPEED;
      ip_data->ctl_a = ctl_a;
      break;

    case 2:
    case 3:
      ctl_b |= IP429_CTL_RX_LOW_SPEED;
      ip_data->ctl_b = ctl_b;
      break;

    case 4:
      ctl_a |= IP429_CTL_TX_LOW_SPEED;
      ip_data->ctl_a = ctl_a;
      break;

    case 5:
      ctl_b |= IP429_CTL_TX_LOW_SPEED;
      ip_data->ctl_b = ctl_b;
      break;

    default:;
  }
}
/*****************************************************************************/
 
void IP429::setSpeedHigh (int chan)
 
// Sets the speed high (100 Khz) for the requested channel.
{
  switch (chan) {
    case 0:
    case 1:
      ctl_a &= ~IP429_CTL_RX_LOW_SPEED;
      ip_data->ctl_a = ctl_a;
      break;
 
    case 2:
    case 3:
      ctl_b &= ~IP429_CTL_RX_LOW_SPEED;
      ip_data->ctl_b = ctl_b;
      break;
 
    case 4:
      ctl_a &= ~IP429_CTL_TX_LOW_SPEED;
      ip_data->ctl_a = ctl_a;
      break;
 
    case 5:
      ctl_b &= ~IP429_CTL_TX_LOW_SPEED;
      ip_data->ctl_b = ctl_b;
      break;
 
    default:;
  }
}
/*****************************************************************************/

void IP429::enableLoopback (int chan)

// Enables the loopback test mode on the requested channel.
{
  if (chan == 4) {
    ctl_a &= ~IP429_CTL_NO_LOOPBACK;	// active low bit
    ip_data->ctl_a = ctl_a;
  }
  else {
    ctl_b &= ~IP429_CTL_NO_LOOPBACK;	// active low bit
    ip_data->ctl_b = ctl_b;
  }
}
/*****************************************************************************/
 
void IP429::disableLoopback (int chan)
 
// Disables the loopback test mode on the requested channel.
{
  if (chan == 4) {
    ctl_a |= IP429_CTL_NO_LOOPBACK;	// active low bit
    ip_data->ctl_a = ctl_a;
  }
  else {
    ctl_b |= IP429_CTL_NO_LOOPBACK;	// active low bit
    ip_data->ctl_b = ctl_b;
  }
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

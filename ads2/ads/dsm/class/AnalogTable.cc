/* AnalogTable.cc
   Class for managing the analog channel sample table.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/
#include <AnalogTable.h>

/******************************************************************************
** Public Functions
******************************************************************************/
 
AnalogTable::AnalogTable () 
 
// Class constructor.
{
  memset ((char*)&gains, 0, sizeof (gains));
  memset ((char*)&dsp_tbl, 0, sizeof (dsp_tbl));
  dsp_tbl.cal_volt = ANA_V10_MASK;    	// valid cal setting always required

  start_high = 0;
  start_250 = 0;
  start_25 = 0;
  start_5 = 0;

  len_high = 0;
  len_250 = 0;
  len_25 = 0;
  len_5 = 0;

  offset_high = 0;
  offset_250 = 0;
  offset_25 = 0;
  offset_5 = 0;

  ana_high_rate = 0;
  cur_rate = RATE_5000;
  ch_idx = 0;

}
/*****************************************************************************/

void AnalogTable::enter (int adr, int rate, int ch_gain, int ch_offset, 
                         int start, int offset, int length) 

// Enters channel information into the sample table.
{
  int setup;

// Verify the correct ordering of the sample rates, highest to lowest.
  if (rate > cur_rate) {
    fprintf (stderr, 
     "AnalogTable: Out of sequence sample rate of %4d hz for channel 0x%04X.\n",
     rate, adr);
    exit (ERROR);
  }

// Increment the channel information for this rate.
  switch (cur_rate = rate) {

    case RATE_5000:                         // highest sample rate
      if (!len_high) {
        start_high = start;                     // first high hz chan
        offset_high = offset;
      }
      len_high += length;               // increment sample block len
      dsp_tbl.cnthigh++;                       // incr dsp table count
      ana_high_rate = 5000;
      break;

    case RATE_1000:				// highest sample rate
      if (!len_high) {
        start_high = start;			// first high hz chan
        offset_high = offset;
      }
      len_high += length;		// increment sample block len
      dsp_tbl.cnthigh++;			// incr dsp table count
      ana_high_rate = 1000;
      break;
 
    case RATE_250:
      if (!len_250) {			
        start_250 = start;		// first 250 hz chan
        offset_250 = offset;
      }
      len_250 += length;		// increment sample block len
      dsp_tbl.cnt250++;				// incr dsp table count
      break;

    case RATE_25:
      if (!len_25) {			
        start_25 = start;		// first 25 hz chan
        offset_25 = offset;
      }
      len_25 += length;		// increment sample block len
      dsp_tbl.cnt25++;				// incr dsp table count
      break;

    case RATE_5:
      if (!len_5) {			
        start_5 = start;		// first 5 hz chan
        offset_5 = offset;
      }
      len_5 += length;		// increment sample block len
      dsp_tbl.cnt5++;				// incr dsp table count
      break;

    case RATE_1:				// only for a dummy variable
      return;					// just ignore

    default:
      fprintf (stderr,
        "AnalogTable: Invalid sample rate of %4d hz for channel 0x%04X.
                      \n", rate, adr);
      exit (ERROR);
  }

// Enter the channel address. 
  dsp_tbl.addrs[dsp_tbl.tot_cnt] = adr;

// Enter the gain and offset information.
  switch (gains[dsp_tbl.tot_cnt] = ch_gain) {

    case ANALOG_GAIN_1_SELECT:
      setup = ANA_GAIN_1_MASK;
      break;
    case ANALOG_GAIN_2_SELECT:
      setup = ANA_GAIN_2_MASK;
      break;
    case ANALOG_GAIN_4_SELECT:
      setup = ANA_GAIN_4_MASK;
      break;
    case ANALOG_GAIN_20_SELECT:
      setup = ANA_GAIN_20_MASK;
      break;
    default:
      fprintf (stderr,
        "AnalogTable: Invalid gain of %4d for channel 0x%04X.\n",
        ch_gain, adr);
      exit (ERROR);
  }
  if (ch_offset)
    setup |= ANA_OFFSET_MASK;

// Odd channels are in the upper 4 bits of each setup byte. Even channels
// are in the lower 4 bits.
  if (adr % 2)
    setup <<= ANA_ODD_SETUP_SHIFT;
  
  dsp_tbl.setup[adr / 2] |= setup;

// Increment the total channel counter.
  dsp_tbl.tot_cnt++;		

  cal_tbl = dsp_tbl;			// update the cal table;
}
/*****************************************************************************/

void AnalogTable::displayTable ()

// Display the table contents.
{
  int j;
/*
  printf ("tot_cnt = %d, cnt_high = %d, cnt250= %d, cnt25 = %d, cnt5 = %d\n",
          dsp_tbl.tot_cnt, dsp_tbl.cnt_high, dsp_tbl.cnt250, dsp_tbl.cnt25,
          dsp_tbl.cnt5);
*/
  printf ("Addresses\n");
  for (j = 0; j < dsp_tbl.tot_cnt; j++) 
    printf ("%04X ", dsp_tbl.addrs[j]);

  printf ("\nSetups\n");
  for (j = 0; j < dsp_tbl.tot_cnt / 2; j++) 
    printf ("%02X   ", dsp_tbl.setup[j]);

  printf ("\nstart_1000 = %d, start250 = %d, start25 = %d, start5 = %d\n",
           start_high, start_250, start_25, start_5);
  printf ("len_1000 = %d, len250 = %d, len25 = %d, len5 = %d\n",
           len_high, len_250, len_25, len_5);
  printf ("offset_1000 = %d, offset250 = %d, offset25 = %d, offset5 = %d\n",
           offset_high, offset_250, offset_25, offset_5);

}
/*****************************************************************************/

int AnalogTable::nextChannel ()
 
// Selects the next channel in the setup table
{
  if (++ch_idx < dsp_tbl.tot_cnt) 
    return TRUE;

  ch_idx--;				// end of the table
  return FALSE;
}
/*****************************************************************************/

int AnalogTable::selectByAddress (int adr)
 
// Selects a channel in the setup table by address.
{
  int stat;

  for (stat = firstChannel(); stat && (adr != address()); stat = nextChannel());
  return stat;
}

/*****************************************************************************/
int AnalogTable::setVolt (int volt)

// Sets the calibration voltage.
{
  switch (volt) {
    case ANALOG_V10_SELECT:
      cal_tbl.cal_volt = ANA_V10_MASK;
      break;
    case ANALOG_V5_SELECT:
      cal_tbl.cal_volt = ANA_V5_MASK;
      break;
    case ANALOG_V2D5_SELECT:
      cal_tbl.cal_volt = ANA_V2D5_MASK;
      break;
    case ANALOG_V1_SELECT:
      cal_tbl.cal_volt = ANA_V1_MASK;
      break;
    case ANALOG_V0_SELECT:
      cal_tbl.cal_volt = ANA_V0_MASK;
      break;
    case ANALOG_V10N_SELECT:
      cal_tbl.cal_volt = ANA_V10N_MASK;
      break;
    default:
      return FALSE;
  }
  return TRUE;
}

/*****************************************************************************/
int AnalogTable::setGain (int gain)
 
// Sets the gain on the current channel.  
{
  int setup;

  switch (gain) {
    case ANALOG_GAIN_1_SELECT:
      setup = ANA_GAIN_1_MASK;
      break;
    case ANALOG_GAIN_2_SELECT:
      setup = ANA_GAIN_2_MASK;
      break;
    case ANALOG_GAIN_4_SELECT:
      setup = ANA_GAIN_4_MASK;
      break;
    case ANALOG_GAIN_20_SELECT:
      setup = ANA_GAIN_20_MASK;
      break;
    default:
      return FALSE;
  }

  if (address() % 2) {					// odd channel
    cal_tbl.setup[address()/2] &= ~(ANA_GAIN_MASK << ANA_ODD_SETUP_SHIFT);
    cal_tbl.setup[address()/2] |= setup << ANA_ODD_SETUP_SHIFT;
  }
  else {						// even channel
    cal_tbl.setup[address()/2] &= ~ANA_GAIN_MASK;	// clear current gain
    cal_tbl.setup[address()/2] |= setup;		// set the gain
  }
  return TRUE;
}

/*****************************************************************************/
int AnalogTable::setOffset (int offset)

// Sets the offset on the current channel.  
{

  switch (offset) {
    case ANALOG_OFFSET_10_SELECT:
      if (address() % 2)				// odd channel
        cal_tbl.setup[address()/2] |= ANA_OFFSET_MASK << ANA_ODD_SETUP_SHIFT;
      else						// even channel
        cal_tbl.setup[address()/2] |= ANA_OFFSET_MASK;
      break;

    case ANALOG_OFFSET_0_SELECT:
      if (address() % 2) 				// odd channel
        cal_tbl.setup[address()/2] &= ~(ANA_OFFSET_MASK<< ANA_ODD_SETUP_SHIFT);
      else						// even channel
        cal_tbl.setup[address()/2] &= ~ANA_OFFSET_MASK;
      break;

    default:
      return FALSE;
  }
}

/*****************************************************************************/
int AnalogTable::enableCal ()

// Enables the calibration on the current channel.
{
  if (address() % 2) 					// odd channel
    cal_tbl.setup[address()/2] |= ANA_CAL_MASK << ANA_ODD_SETUP_SHIFT;
  else							// even channel
    cal_tbl.setup[address()/2] |= ANA_CAL_MASK;
}

/*****************************************************************************/
int AnalogTable::disableCal ()

// Disables the calibration on the current channel.
{
  if (!(address() % 2))
    cal_tbl.setup[address()/2] |= ~(ANA_CAL_MASK << ANA_ODD_SETUP_SHIFT);
  else
    cal_tbl.setup[address()/2] &= ~ANA_CAL_MASK;
}

/* END ANALOGTABLE.CC */

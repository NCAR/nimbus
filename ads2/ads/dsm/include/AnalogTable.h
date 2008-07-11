/* AnalogTable.h
   Class for managing the analog channel sample table.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef ANALOGTABLE_H
#define ANALOGTABLE_H

#include <stdioLib.h>
#include <stdlib.h>
#include <string.h>

#include <dsmctl.h>
#include <header.h>
#include <messageDefs.h>

#define ANA_SETUP_FLAG		0x5555	// new setup flag 
#define ANA_RUNNING_FLAG	0xAAAA	// analog table accepted flag
#define ANA_DATA_READY_FLAG	0x0001	// new sampled data ready flag

#define ANA_V10_MASK		0x6000	// +10 volt calibration mask
#define ANA_V5_MASK		0x6001	// +5 volt calibration mask
#define ANA_V2D5_MASK		0x6002	// +2.5 volt calibration mask
#define ANA_V1_MASK		0x6003	// +1 volt calibration mask
#define ANA_V0_MASK		0x6004	// 0 volt calibration mask
#define ANA_V10N_MASK		0x6005	// 10 volt calibration mask

#define ANA_GAIN_1_MASK		0x00	// channel gain of 1 mask
#define ANA_GAIN_2_MASK		0x01	// channel gain of 2 mask
#define ANA_GAIN_4_MASK		0x02	// channel gain of 4 mask
#define ANA_GAIN_20_MASK	0x03	// channel gain of 20 mask
#define ANA_GAIN_MASK		0x03	// gain bits mask
#define ANA_OFFSET_MASK		0x04	// voltage offset mask	
#define ANA_CAL_MASK		0x08	// calibration enable mask	
#define ANA_ODD_SETUP_SHIFT	4	// odd channel setup in upper 4 bits

#define ANA_MAX_CHAN		64	// max number of analog channels
#define NUM_HIGH_CHAN		7	// No. MCR channels

typedef struct {
  short flag;				// new setup flag
  short data_ready;			// data ready flag
  short buf_adr;			// output buffer address
  short cal_volt;			// calibration voltage to use
  short tot_cnt;			// total number of channels
  short cnthigh;			// number of high rate channels
  short cnt250;				// number of 250 hertz channels
  short cnt25;				// number of 25 hertz channels
  short cnt5;				// number of 5 hertz channels
  short addrs[ANA_MAX_CHAN];		// channel addresses
  short setup[ANA_MAX_CHAN / 2];	// gain, offset, & cal setup
} DspTable;

class AnalogTable {
public:
  AnalogTable();				// constructor
  void enter (int adr, int rate, int ch_gain, int ch_offset, int start, 
              int offset, int length);
  int nextChannel();				// select next channel
  int selectByAddress(int adr);			// select channel by address
  int setVolt (int volt);			// set the cal voltage
  int setGain (int gain);			// set current channel gain
  int setOffset (int offset);			// set current channel offset
  int enableCal();				// enable cal on current chan
  int disableCal();				// disable cal on current chan
  void displayTable();				// display the table contents

  DspTable& dspTable()	{return dsp_tbl;}    	// return the dsp sample table
  DspTable& calTable()	{return cal_tbl;}    	// return the cal sample table
  int starthigh() 	{return start_high;} 	// lr start of high hz channels
  int start250()  	{return start_250;}  	// lr start of 250 hz channels
  int start25()   	{return start_25;}   	// lr start of 25 hz channels
  int start5() 		{return start_5;}    	// lr start of 5 hz channels
  int lenhigh()   	{return len_high;}   	// length of high hz channels 
  int len250()    	{return len_250;}    	// length of 250 hz channels 
  int len25()     	{return len_25;}     	// length of 25 hz channels 
  int len5()      	{return len_5;}      	// length of 5 hz channels 
  int offsethigh()	{return offset_high;}	// offset between high hz samps
  int offset250() 	{return offset_250;} 	// offset between 250 hz samps
  int offset25()  	{return offset_25;} 	// offset between 25 hz samps
  int offset5() 	{return offset_5;} 	// offset between 5 hz samps
  int address()	 	{return dsp_tbl.addrs[ch_idx];}
  int gain()	 	{return gains[ch_idx];}
  void resetCalTable()	{cal_tbl = dsp_tbl;} 
  int firstChannel() 	{return ((ch_idx = 0) < dsp_tbl.tot_cnt);} 
  int high_rate()	{return ana_high_rate;}	// highest sample rate
 
private:
  DspTable dsp_tbl;			// dsp sample table
  DspTable cal_tbl;			// table for calibrations
  int gains[ANA_MAX_CHAN];		// channel gains

  int start_high;                       // start of the analog high hz block
  int start_250;                        // start of the analog 250 hz block
  int start_25;                         // start of the analog 25 hz block
  int start_5;                          // start of the analog 5 hz block

  int len_high;                       	// length of one high hz sample block
  int len_250;                          // length of one 250 hz sample block
  int len_25;                          	// length of one 25 hz sample block
  int len_5;                          	// length of one 5 hz sample block

  int offset_high;                      // offset between high hz samples
  int offset_250;                       // offset between 250 hz samples
  int offset_25;                        // offset between 25 hz samples
  int offset_5;                        	// offset between 5 hz samples

  int cur_rate;				// current sample rate
  int ch_idx;				// current channel index
  int ana_high_rate;
};

#endif



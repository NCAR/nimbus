/* AnaAuxTable.h
   Class for managing the auxiliary analog sample table.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef ANAAUXTABLE_H
#define ANAAUXTABLE_H

#include <vxWorks.h>
#include <stdioLib.h>
#include <stdlib.h>
#include <string.h>

#include <header.h>

#define ANA_AUX_MAX_CHAN	64	// max number of analog channels

class AnaAuxTable {
public:
  AnaAuxTable ();			// constructor
  void enter(int adr, int rate, int ch_gain, int start, int offset, int length);
  int nextChannel();				// select next channel
  int selectByAddress(int adr);			// select channel by address
  void displayTable();				// display the table contents

  inline int firstChannel()   {return ((ch_idx = 0) < tot_cnt);} 
  inline int start25()  {return start_25;}   	// lr start of 25 hz channels
  inline int start5()  	{return start_5;}    	// lr start of 5 hz channels
  inline int len25()    {return len_25;}     	// length of 25 hz channels 
  inline int len5()     {return len_5;}      	// length of 5 hz channels 
  inline int offset25() {return offset_25;} 	// offset between 25 hz samps
  inline int offset5() 	{return offset_5;} 	// offset between 5 hz samps
  inline int count25()  {return cnt_25;} 	// number of 25 hz channels
  inline int count5() 	{return cnt_5;} 	// number of 5 hz channels
  inline int address()	{return addrs[ch_idx];}// current channel address
  inline int gain()	{return gains[ch_idx];}// current channel gain
 
private:
  int addrs[ANA_AUX_MAX_CHAN];		// channel addresses
  int gains[ANA_AUX_MAX_CHAN];		// channel gains

  int start_25;                     	// start of the analog 25 hz block
  int start_5;                      	// start of the analog 5 hz block

  int len_25;                          	// length of one 25 hz sample block
  int len_5;                          	// length of one 5 hz sample block

  int offset_25;                        // offset between 25 hz samples
  int offset_5;                        	// offset between 5 hz samples

  int cnt_25;				// number of 25 hz channels
  int cnt_5;				// number of 5 hz channels
  int tot_cnt;				// number of 5 hz channels

  int cur_rate;				// current sample rate
  int ch_idx;				// current channel index
};
#endif

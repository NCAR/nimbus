/* GreyVmehTable.h
   Class for managing the Grey housekeeping sample table.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef GREYVMEHTABLE_H
#define GREYVMEHTABLE_H

#include <vxWorks.h>
#include <stdioLib.h>
#include <stdlib.h>
#include <dsmctl.h>
#include <messageDefs.h>
#include <header.h>
#include <TapeHeader.h>

#define GREY_MAX_CHAN        2               // max number of PMS Grey probes.
class GreyVmehTable {
public:
  GreyVmehTable ();		        	// constructor
  void enter(TapeHeader&);
  int nextChannel();				// select next channel
  void displayTable();				// display the table contents

  inline int firstChannel()   {return ((ch_idx = 0) < tot_cnt);} 
  inline int start1()  	{return start_1[ch_idx];}// lr start of 1 hz channels
  inline int len1()     {return len_1[ch_idx];}	// length of 1 hz channels 
  int interface()       {return intf[ch_idx];}
  int channel()         {return chan[ch_idx];}
 
private:

  int start_1[MAX_GREYVME_INTFC];     	// start of the block
  int len_1[MAX_GREYVME_INTFC];      	// length of one 1 hz sample block
  int intf [MAX_GREYVME_INTFC];            // interface number 0 or 1
  int chan [MAX_GREYVME_INTFC];            // channel number 0, 1
  int tot_cnt;			
  int ch_idx;                           // current channel index
};
#endif

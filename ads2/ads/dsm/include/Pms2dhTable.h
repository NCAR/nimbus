/* Pms2dhTable.h
   Class for managing the 2-D housekeeping sample table.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef PMS2DHTABLE_H
#define PMS2DHTABLE_H

#include <vxWorks.h>
#include <stdioLib.h>
#include <stdlib.h>
#include <dsmctl.h>
#include <messageDefs.h>
#include <header.h>
#include <TapeHeader.h>

#define PMS2D_MAX_CHAN        2      // max number of PMS 2-D probes.
class Pms2dhTable {
public:
  Pms2dhTable ();			// constructor
  void enter(TapeHeader&);
  int nextChannel();				// select next channel
  void displayTable();				// display the table contents

  inline int firstChannel()   {return ((ch_idx = 0) < tot_cnt);} 
  inline int start5()  	{return start_5[ch_idx];}// lr start of 5 hz channels
  inline int len5()     {return len_5[ch_idx];}	// length of 5 hz channels 
  inline int interface()       {return intf[ch_idx];}
  inline int channel()  {return chan[ch_idx];}
 
private:

  int start_5[MAX_PMSVME2D_INTFC];     	// start of the block
  int len_5[MAX_PMSVME2D_INTFC];      	// length of one 5 hz sample block
  int intf [MAX_PMSVME2D_INTFC];        // interface number 0 or 1
  int chan [MAX_PMSVME2D_INTFC];        // channel number 0, 1
  int tot_cnt;			
  int ch_idx;                           // current channel index
};
#endif

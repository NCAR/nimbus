/* HwIrsTable.h
   Class for managing the Honeywell irs interface sample table.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef HWIRSTABLE_H
#define HWIRSTABLE_H

#include <TapeHeader.h>

#include <dsmctl.h>
#include <header.h>

class HwIrsTable {
public:
  HwIrsTable ();			// constructor
  void enter (TapeHeader&);		// make a table entry
  int nextEntry();			// select the next entry
  void displayTable();			// display the table contents
  int firstEntry() 	{return ((hw_idx = 0) < hw_count);}
  int index()		{return hw_idx;}
  int start() 		{return hw_start[hw_idx];}
  int length() 		{return hw_length[hw_idx];}
  char* name() 		{return hw_name[hw_idx];}
  int rxChan()		{return hw_idx;}
  int txChan() 		{return hw_count && !hw_idx? IP429_NUM_RX_CHANS: FALSE;}
  int aimrChan()	{return hw_count ? IP429_NUM_RX_CHANS + 1 : FALSE;}
 
private:
  int hw_start [MAX_HWIRS_INTFC];	// block starting offsets
  int hw_length [MAX_HWIRS_INTFC];	// block lengths
  str12 hw_name [MAX_HWIRS_INTFC];	// probe names
 
  int hw_idx;				// current entry index
  int hw_count;				// number of entries
};

#endif

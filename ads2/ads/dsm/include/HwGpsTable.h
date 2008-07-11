/* HwGpsTable.h
   Class for managing the Honeywell gps interface sample table.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef HWGPSTABLE_H
#define HWGPSTABLE_H

#include <TapeHeader.h>

#include <dsmctl.h>
#include <header.h>

class HwGpsTable {
public:
  HwGpsTable ();			// constructor
  void enter (TapeHeader&);		// make a table entry
  int nextEntry();			// select the next entry
  void displayTable();			// display the table contents
  int firstEntry() 	{return ((hw_gps_idx = 0) < hw_gps_count);}
  int index()		{return hw_gps_idx;}
  int start() 		{return hw_gps_start[hw_gps_idx];}
  int length() 		{return hw_gps_length[hw_gps_idx];}
  char* name() 		{return hw_gps_name[hw_gps_idx];}
  int rxChan()		{return (hw_gps_idx+2);}
 
private:
  int hw_gps_start [MAX_HWGPS_INTFC];	// block starting offsets
  int hw_gps_length [MAX_HWGPS_INTFC];	// block lengths
  str12 hw_gps_name [MAX_HWGPS_INTFC];	// probe names
 
  int hw_gps_idx;				// current entry index
  int hw_gps_count;				// number of entries
};

#endif

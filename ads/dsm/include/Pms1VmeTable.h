/* Pms1VmeTable.h
   Class for managing the vme interface pms 1d sample table.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef PMS1VMETABLE_H
#define PMS1VMETABLE_H

#include <TapeHeader.h>

#include <dsmctl.h>
#include <header.h>

class Pms1VmeTable {
public:
  Pms1VmeTable ();			// constructor
  void enter (TapeHeader&);		// make a table entry
  int nextChan();			// select the next channel entry
  void displayTable();			// display the table contents
  int firstChan() 	{return ((p1_idx = 0) < p1_count);}
  int interface()	{return p1_intf[p1_idx];}
  int channel()		{return p1_chan[p1_idx];}
  int start() 		{return p1_start[p1_idx];}
  int length() 		{return p1_length[p1_idx];}
  int psStart() 	{return ps_start[p1_idx];}
  int psLength()	{return ps_length[p1_idx];}
  int rate() 		{return p1_rate[p1_idx];}
  int bins() 		{return p1_bins[p1_idx];}
  int range() 		{return p1_range[p1_idx];}
  char* name() 		{return p1_name[p1_idx];}
 
private:
  int p1_intf [MAX_PMS1];		// interface number 0 or 1
  int p1_chan [MAX_PMS1];		// channel number 0, 1, 2
  int p1_start [MAX_PMS1];		// block starting offsets
  int p1_length [MAX_PMS1];		// block lengths
  int ps_start [MAX_PMS1];		// ps block starting offsets
  int ps_length [MAX_PMS1];		// ps block lengths
  int p1_rate [MAX_PMS1];		// sample rates
  int p1_bins [MAX_PMS1];		// number of sample bins
  int p1_range [MAX_PMS1];		// default range or non-ranging
  str12 p1_name [MAX_PMS1];		// probe names
 
  int p1_idx;				// current entry index
  int p1_count;				// number of entries
};

#endif

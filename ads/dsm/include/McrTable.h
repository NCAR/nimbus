/* McrTable.h
   Class for mcr interface sample tables.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef MCRTABLE_H
#define MCRTABLE_H

#include <TapeHeader.h>
#include <dsmctl.h>

class McrTable {
public:
  McrTable ();				// constructor
  void enterMcrOut (TapeHeader&);	// make entry for MCROUT
  void enterMcrStat (TapeHeader&);	// make entry for MCRSTAT
  void displayTable();			// display the table contents
  inline int address()    	{return mcrout_adr;}
  inline int boardNumber()      {return (mcrout_adr >> 4) & 0x000F;}
  inline int startMcrOut() 	{return mcrout_start;}
  inline int startMcrStat() 	{return mcrstat_start;}
 
private:
  int mcrout_adr;			// channel address of MCR_OUT
  int mcrout_start; 			// start of the MCROUT variable 
  int mcrstat_start; 			// start of the MCRSTAT variable 
};

#endif

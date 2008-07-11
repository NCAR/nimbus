/* AdcAoaTable.h
   Class for managing the air data computer aoa interface sample table.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef ADCAOATABLE_H
#define ADCAOATABLE_H

#include <TapeHeader.h>

#include <dsmctl.h>
#include <header.h>

class AdcAoaTable {
public:
  AdcAoaTable ();			// constructor
  void enter (TapeHeader&);		// make a table entry
  int nextEntry();			// select the next entry
  void displayTable();			// display the table contents
  int firstEntry() 	{return ((adc_aoa_idx = 0) < adc_aoa_count);}
  int index()		{return adc_aoa_idx;}
  int start() 		{return adc_aoa_start[adc_aoa_idx];}
  int length() 		{return adc_aoa_length[adc_aoa_idx];}
  char* name() 		{return adc_aoa_name[adc_aoa_idx];}
  int rxChan()		{return adc_aoa_idx+3;}
 
private:
  int adc_aoa_start [MAX_ADCAOA_INTFC];	// block starting offsets
  int adc_aoa_length [MAX_ADCAOA_INTFC];	// block lengths
  str12 adc_aoa_name [MAX_ADCAOA_INTFC];	// probe names
 
  int adc_aoa_idx;				// current entry index
  int adc_aoa_count;				// number of entries
};

#endif

/* CounterTable.h

   Class for managing the IP daughter Counter Table.

   Original Author: Chris Webster
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef COUNTERTABLE_H
#define COUNTERTABLE_H

#include <TapeHeader.h>
#include <dsmctl.h>

#define CTR_MAX_CHAN		6	// max number of digital channels
#define CTR_PORT_32		32	// 24 bit port size
#define CTR_PORT_16		16	// 16 bit port size

class CounterTable {
public:
  CounterTable ();			// constructor
  void enter(TapeHeader&);		// make a table entry
  void displayTable();			// display the table contents
  int start50() 	{return start_50;}
  int start25() 	{return start_25;}
  int start5() 		{return start_5;}
  int len50() 		{return len_50;}
  int len25() 		{return len_25;}
  int len5() 		{return len_5;}
  int offset50() 	{return offset_50;}
  int offset25() 	{return offset_25;}    
  int offset5()  	{return offset_5;}    
  int cnt50() 		{return cnt_50;}
  int cnt25() 		{return cnt_25;}
  int cnt5() 		{return cnt_5;}
  int firstEntry()      {return ((cur_idx = 0) < tot_cnt);}
  int nextEntry()	{return cur_idx < (tot_cnt - 1) ? ++cur_idx: FALSE;}
  int address()		{return adr[cur_idx];}
  int portNumber() 	{return adr[cur_idx] & 0x000F;}
  int boardNumber()	{return (adr[cur_idx] >> 4) & 0x000F;}
  int portType() 	{return (portType[cur_idx]);}
 
private:
  int adr[CTR_MAX_CHAN];		// channel addresses
  int portType[CTR_MAX_CHAN];		// # bits to configure for port

  int start_50;                         // start of the 25 hz block
  int start_25;                         // start of the 25 hz block
  int start_5;                          // start of the 5 hz block
  int len_50;                          	// length of the 25 hz block
  int len_25;                          	// length of the 25 hz block
  int len_5;                           	// length of the 5 hz block
  int offset_50;                        // offset between 25 hz samples
  int offset_25;                        // offset between 25 hz samples
  int offset_5;                         // offset between 5 hz samples
  int cnt_50;				// number of 25 hertz channels
  int cnt_25;				// number of 25 hertz channels
  int cnt_5;				// number of 5 hertz channels
  int tot_cnt;				// total number of channels
  int cur_idx;				// current index
  int cur_rate;				// current sample rate
};

#endif

/* DigitalTable.h

   Class for managing the digital input channel sample table.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef DIGITALTABLE_H
#define DIGITALTABLE_H

#include <TapeHeader.h>
#include <dsmctl.h>

#define DIG_MAX_CHAN		32	// max number of digital channels
#define DIG_PORT_RADAR_ALT	17	// radar altimeter port
#define DIG_PORT_16		16	// sixteen bit port size
#define DIG_PORT_8		8	// eight bit port size
#define DIG_PORT_2		2	// two bit port size

class DigitalTable {
public:
  DigitalTable ();			// constructor
  void enter (TapeHeader&);		// make a table entry
  void displayTable();			// display the table contents
  int start55() 	{return start_55;}
  int start50() 	{return start_50;}
  int start25() 	{return start_25;}
  int start5() 		{return start_5;}
  int start1() 		{return start_1;}
  int len55() 		{return len_55;}
  int len50() 		{return len_50;}
  int len25() 		{return len_25;}
  int len5() 		{return len_5;}
  int len1() 		{return len_1;}
  int offset55() 	{return offset_55;}    
  int offset50() 	{return offset_50;}    
  int offset25() 	{return offset_25;}    
  int offset5()  	{return offset_5;}    
  int cnt55() 		{return cnt_55;}
  int cnt50() 		{return cnt_50;}
  int cnt25() 		{return cnt_25;}
  int cnt5() 		{return cnt_5;}
  int cnt1() 		{return cnt_1;}
  int firstEntry()      {return ((cur_idx = 0) < tot_cnt);}
  int nextEntry()	{return cur_idx < (tot_cnt - 1) ? ++cur_idx: FALSE;}
  int address()		{return adr[cur_idx];}
  int portNumber() 	{return adr[cur_idx] & 0x000F;}
  int boardNumber()	{return (adr[cur_idx] >> 4) & 0x000F;}
  int counterNumber() 	{return (adr[cur_idx] >> 8) & 0x000F;}
  int portType() 	{return (port_type[cur_idx]);}
 
private:
  int adr[DIG_MAX_CHAN];		// channel addresses
  int port_type[DIG_MAX_CHAN];		// # bits to configure for port

  int start_55;                         // start of the 55 hz block
  int start_50;                         // start of the 50 hz block
  int start_25;                         // start of the 25 hz block
  int start_5;                          // start of the 5 hz block
  int start_1;                          // start of the 1 hz block
  int len_55;                          	// length of the 55 hz block
  int len_50;                          	// length of the 50 hz block
  int len_25;                          	// length of the 25 hz block
  int len_5;                           	// length of the 5 hz block
  int len_1;                           	// length of the 1 hz block
  int offset_55;                        // offset between 55 hz samples
  int offset_50;                        // offset between 50 hz samples
  int offset_25;                        // offset between 25 hz samples
  int offset_5;                         // offset between 5 hz samples
  int cnt_55;				// number of 55 hertz channels
  int cnt_50;				// number of 50 hertz channels
  int cnt_25;				// number of 25 hertz channels
  int cnt_5;				// number of 5 hertz channels
  int cnt_1;				// number of 1 hertz channels
  int tot_cnt;				// total number of channels
  int cur_idx;				// current index
  int cur_rate;				// current sample rate
};

#endif



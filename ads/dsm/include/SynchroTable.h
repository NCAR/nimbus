/* SynchroTable.h
   Class for managing the synchro sample table.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef SYNCHROTABLE_H
#define SYNCHROTABLE_H

#include <vxWorks.h>
#include <stdioLib.h>
#include <stdlib.h>

#include <header.h>
#define SYNCHRO_MAX_CHAN        2      // max number of synchro channels
class SynchroTable {
public:
  SynchroTable ();			// constructor
  void enter(int adr, int rate, int start, int offset, int length);
  int nextChannel();				// select next channel
  void displayTable();				// display the table contents

  inline int firstChannel()   {return ((ch_idx = 0) < tot_cnt);} 
  inline int start25()  {return start_25;}   	// lr start of 25 hz channels
  inline int start5()  	{return start_5;}    	// lr start of 5 hz channels
  inline int start1()  	{return start_1;}    	// lr start of 1 hz channels
  inline int len25()    {return len_25;}     	// length of 25 hz channels 
  inline int len5()     {return len_5;}      	// length of 5 hz channels 
  inline int len1()     {return len_1;}      	// length of 1 hz channels 
  inline int offset25() {return offset_25;} 	// offset between 25 hz samps
  inline int offset5() 	{return offset_5;} 	// offset between 5 hz samps
  inline int offset1() 	{return offset_1;} 	// offset between 1 hz samps
  inline int cnt25()  {return cnt_25;} 	// number of 25 hz channels
  inline int cnt5() 	{return cnt_5;} 	// number of 5 hz channels
  inline int cnt1() 	{return cnt_1;} 	// number of 1 hz channels
 
private:

  int addrs[SYNCHRO_MAX_CHAN];          // channel addresses
  int start_25;                     	// start of the analog 25 hz block
  int start_5;                      	// start of the analog 5 hz block
  int start_1;                      	// start of the analog 1 hz block

  int len_25;                          	// length of one 25 hz sample block
  int len_5;                          	// length of one 5 hz sample block
  int len_1;                          	// length of one 1 hz sample block

  int offset_25;                        // offset between 25 hz samples
  int offset_5;                        	// offset between 5 hz samples
  int offset_1;                        	// offset between 1 hz samples

  int cnt_25;				// number of 25 hz channels
  int cnt_5;				// number of 5 hz channels
  int cnt_1;				// number of 1 hz channels
  int tot_cnt;			

  int cur_rate;				// current sample rate
  int ch_idx;                           // current channel index
};
#endif

/* Vmio14In.h
 
   Counter class for the OR VMIO12 with the VMIO14 daughter board.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef VMIO14COUNT_H
#define VMIO14COUNT_H
 
#include <Vmio12.h>				// OR VMIO12 I/O board class

class Vmio14Count {
public:
  Vmio14Count (Vmio12 *vio, int chan);		// constructor
  int count1 ();				// return counter #1 counts
  int count2 ();				// return counter #2 counts
 
private:
  Vmio12 *vmio12;				// board class 
  int cio_num;					// cio number on the board
  int last_count1;				// last value from counter #1
  int last_count2;				// last value from counter #2
};

#endif

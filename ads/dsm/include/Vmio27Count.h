/* Vmio27Count.h
 
   OR VMIO27/28 Counter class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef VMIO27COUNT_H
#define VMIO27COUNT_H
 
#include <Vmio12.h>				// OR VMIO12 I/O board class

#define VMIO27_CTR_1 		0		// select counter 1
#define VMIO27_CTR_2 		1		// select counter 2
#define VMIO27_CTR_3 		2		// select counter 3
#define VMIO27_CTR_4 		3		// select counter 4

#define VMIO27_MODE_1		1		// single count sync with ua1
#define VMIO27_MODE_2		2		// single count sync with ua2
#define VMIO27_MODE_3		3		// double count sync with ua1
#define VMIO27_MODE_4		4		// double count sync with ua2
#define VMIO27_MODE_5	 	5		// quad cnt sync with all edges
#define VMIO27_MODE_6		6		// pulse width measurement
#define VMIO27_MODE_7		7		// frequency measurement

#define VMIO27_NUM_MODE_BITS	3		// # mode bits	     (port a)

#define VMIO27_RD_ENAB		0x0E		// read enable mask  (port c)
#define VMIO27_WR_ENAB		0x0D		// write enable mask (port c)	
#define VMIO27_LSB_SEL		0x0F		// lsb select bits   (port c)
#define VMIO27_LMSB_SEL		0x0B		// lsb+1 select bits (port c)
#define VMIO27_MSB_SEL		0x07		// msb select bits   (port c)
#define VMIO27_RST_SEL		0x07

class Vmio27Count {
public:
  Vmio27Count (Vmio12 &vio, int chan);		// constructor
  void setMode (int ctr_num, char mode);	// set mode
  void resetCounters();				// reset counters to 0
  int readCount (int ctr_num);			// read counter value
 
private:
  Vmio12 &vmio12;				// board class 
  int cio_num;					// cio number on the board
  char mode12;					// mode of counters 1 and 2
  char mode34;					// mode of counters 3 and 4
};

#endif

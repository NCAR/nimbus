/* Counter.h

   Counter sampling class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef COUNTER_H
#define COUNTER_H

#include <DigitalTable.h>
#include <SerialBuf.h>
#include <Vmio12.h>
#include <Vmio14Count.h>

class Counter {
public:
  Counter (DigitalTable&, Vmio12 *vmio12[]);	// constructor
  void getData (int hz_cnt);			// get data samples

  inline char *buf25(){return buf_25[gtog]->getBuf();}
  inline char *buf5() {return buf_5[gtog]->getBuf();}
  inline char *buf1() {return buf_1[gtog]->getBuf();}
 
private:
  DigitalTable &cnt_tbl;			// counter sample table
  Vmio14Count *vmio14_cnt[DIG_MAX_CHAN];	// counter port classes

  SerialBuf *buf_25[TOG];			// 25 hz data buffers
  SerialBuf *buf_5[TOG];			// 5 hz data buffers
  SerialBuf *buf_1[TOG];			// 1 hz data buffers
  int gtog;					// get buffer index
  int ptog;					// put buffer index
};

#endif


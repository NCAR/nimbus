/* Counter.h

   Counter sampling class.

   Original Author: Chris Webster
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef COUNTER_H
#define COUNTER_H

#include <CounterTable.h>
#include <SerialBuf.h>

class Counter {
public:
  Counter(CounterTable&, IPcounter *ip);	// constructor
  void getData(int hz_cnt);			// get data samples

  inline char *buf25()	{return buf_25[gtog]->getBuf();}
  inline char *buf5()	{return buf_5[gtog]->getBuf();}
  inline char *buf1()	{return buf_1[gtog]->getBuf();}
 
private:
  CounterTable &cnt_tbl;			// counter sample table
  IPcounter *IPcnt;

  SerialBuf *buf_25[TOG];			// 25 hz data buffers
  SerialBuf *buf_5[TOG];			// 5 hz data buffers
  SerialBuf *buf_1[TOG];			// 1 hz data buffers
  int gtog;					// get buffer index
  int ptog;					// put buffer index
};

#endif

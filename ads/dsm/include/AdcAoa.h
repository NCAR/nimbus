/* AdcAoa.h
 
   Air data computer angle-of-attack interface class.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef ADCAOA_H
#define ADCAOA_H

#include <IP429.h>

#include <arinc429.h>
#include <dsmctl.h>
#include <header.h>


class AdcAoa {
public:
  AdcAoa (IP429*, int rxchan);
  char* buffer () {return (char*)&adcaoa_blk[gtog];} // get last sec buffer
  void secAlign ();			// one second alignment
  void readadcIsr ();  			// sampling isr
  int angle()        {return cur_aoa;}
 
private:
  void togglePut();			// toggle put buffer
  IP429 *ip429;				// arinc interface class

  AdcAoa_blk adcaoa_blk[TOG];		// data buffers
  int gtog;				// get buffer index
  int ptog;				// put buffer index
  int idx1;				// 2 hz parameters index
  int new_sec;				// start new second flag
  int rx_chan;				// receive arinc channel
  int cur_aoa;                          // current arinc aoa word
};

#endif

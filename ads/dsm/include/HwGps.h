/* HwGps.h
 
   Honeywell GPS interface class.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef HWGPS_H
#define HWGPS_H

#include <string.h>
#include <IP429.h>

#include <arinc429.h>
#include <dsmctl.h>
#include <header.h>


class HwGps {
public:
  HwGps (IP429*, int rxchan);
  char* buffer () {return (char*)&hgps_blk[gtog];} // get last sec buffer
  void secAlign ();			// one second alignment
  void readgpsIsr ();  			// sampling isr
  int latitude()        {return cur_lat;}
  int longitude()       {return cur_lon;}
 
private:
  void togglePut();			// toggle put buffer
  IP429 *ip429;				// arinc interface class

  HwGps_blk hgps_blk[TOG];			// data buffers
  int gtog;				// get buffer index
  int ptog;				// put buffer index
  int new_sec;				// start new second flag
  int rx_chan;				// receive arinc channel
  int cur_lat;                          // current arinc latitude word
  int cur_lon;                          // current arinc longitude word
};

#endif

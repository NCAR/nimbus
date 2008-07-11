/* DigitalIn.h

   Digital input sampling class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef DIGITALIN_H
#define DIGITALIN_H

#include <vxWorks.h>
#include <sysLib.h>
#include <taskLib.h>
#include <logLib.h>

#include <APN232.h>
#include <Bc635Vme.h>	// Added for Bandy 55 Hz crap
#include <DigitalTable.h>
#include <SerialBuf.h>
#include <Vmio12.h>
#include <Vmio14In.h>

#define  END_OF_55		-99

class DigitalIn {
public:
  DigitalIn (DigitalTable&, Vmio12 *vmio12[], Bc635Vme& tp);	// constructor
  void getData (int hz_cnt);			// get data samples
  void enableInt (int port, int vector);	// enable interrupt
  void readIsr ();				// interrupt service routine

  char *buf55()	{return buf_55[gtog]->getBuf();}
  char *buf50()	{return buf_50[gtog]->getBuf();}
  char *buf25()	{return buf_25[gtog]->getBuf();}
  char *buf5()	{return buf_5[gtog]->getBuf();}
  char *buf1()	{return buf_1[gtog]->getBuf();}
 
private:
  short readPort();				// read current port(s)

  DigitalTable &dig_tbl;			// digital sample table
  void *dig_in[DIG_MAX_CHAN];			// digital input port classes
  Vmio12 *vmio_12; 
  Bc635Vme &tfp;                                // time-freq processor intfc

  SerialBuf *buf_55[TOG];			// 55 hz data buffers
  SerialBuf *buf_50[TOG];			// 50 hz data buffers
  SerialBuf *buf_25[TOG];			// 25 hz data buffers
  SerialBuf *buf_5[TOG];			// 5 hz data buffers
  SerialBuf *buf_1[TOG];			// 1 hz data buffers
  int gtog;					// get buffer index
  int ptog;					// put buffer index
};

#endif


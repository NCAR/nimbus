/* Tasx.h
 
   Computes the aircraft true air speed from QCX, PSX, TTX.
   These are uncorrected values.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef TASX_H
#define TASX_H

#include <math.h>

#include <SyncVar.h>

class Tasx {
public:
  Tasx (SyncVar& q, SyncVar& p, SyncVar& t);
  inline float value() 	{return d_value;}
  void compute();			// compute derived value
 
private:
  SyncVar& qcx;				// dynamic pressure
  SyncVar& psx;				// static pressure
  SyncVar& ttx;				// raw temperature

  float d_value;			// current derived value
};

#endif

/* Tasr.h
 
   Computes the aircraft true air speed from QCRXC,PSRXC,TTRX.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef TASR_H
#define TASR_H

#include <math.h>

#include <SyncVar.h>

class Tasr {
public:
  Tasr (SyncVar &q, SyncVar &p, SyncVar &t);
  inline float tasr () 	{return d_value;}
  void computeTasr ();			// compute derived value
 
private:
  SyncVar &qcx;				// dynamic pressure
  SyncVar &psx;				// static pressure
  SyncVar &ttrx;			// raw temperature

  float d_value;			// current derived value
};

#endif

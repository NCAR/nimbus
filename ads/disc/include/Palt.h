/* Palt.h
 
   Computes the aircraft pressure altitude from PSX

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef PALT_H
#define PALT_H

#include <math.h>

#include <SyncVar.h>

#define ASTG    1013.246        // Default altimeter setting (mb)
#define SFCT    288.15          // Default mean surface temperature setting (K)

class Palt {
public:
  Palt (SyncVar& p);
  void compute();			// compute derived value

  inline float value()	{return d_value;}
 
private:
  SyncVar& psx;				// static pressure

  float d_value;			// current derived value
};

#endif

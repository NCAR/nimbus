/* Vmio14In.h
 
   OR VMIO14/16 Digital Input class. 

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef VMIO14IN_H
#define VMIO14IN_H
 
#include <Vmio12.h>				// OR VMIO12 I/O board class

class Vmio14In {
public:
  Vmio14In (Vmio12 *vio, int chan);		// constructor
  inline unsigned char readPort() {return vmio12->readPort (cio_num, cio_port);}
 
private:
  Vmio12 *vmio12;				// board class 
  int cio_num;					// cio number on the board
  int cio_port;					// port number on cio
};

#endif

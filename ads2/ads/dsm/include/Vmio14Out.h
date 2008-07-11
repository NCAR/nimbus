/* Vmio14Out.h
 
   OR VMIO14/16 Digital Output class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef VMIO14OUT_H
#define VMIO14OUT_H
 
#include <Vmio12.h>				// OR VMIO12 I/O board class

class Vmio14Out {
public:
  Vmio14Out (Vmio12 *vio, int chan);		// constructor
  inline void writePort (unsigned char data) {
//     printf("writePort: data=%x\n", data);
           	vmio12->writePort (cio_num, cio_port, data);}
 
private:
  Vmio12 *vmio12;				// board class 
  int cio_num;					// cio number on the board
  int cio_port;					// port number on cio
};

#endif

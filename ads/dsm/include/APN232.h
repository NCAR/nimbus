/* APN232.h
 
   APN232 radar altimeter interface class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef APN232_H
#define APN232_H
 
#include <Vmio12.h>				// OR VMIO12 I/O board class

class APN232 {
public:
  APN232 (Vmio12 *vio, int chan);		// constructor
  short readPort();				// read the port 
 
private:
  Vmio12 *vmio12;				// board class 
  int cio_num;					// cio number on the board
  int cio_port;					// port number on cio
};

#endif

/* cknats.cc
   Reads nats data from the serial port, and checks it. 

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/
#include <stdio.h>

#include <DiscNatsComm.h>

DiscNatsComm nats_comm (240);

main() {

  int len;

  while (1) {
    if (len = nats_comm.recv (240)) {
      printf ("NATS data block received. len = %d\n", len);
      nats_comm.releaseBuf();
    }
  }
}
/*****************************************************************************/

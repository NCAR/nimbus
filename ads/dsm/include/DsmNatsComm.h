/* DsmNatsComm.h
   Handles NATS serial port communications in the DSM.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef DSMNATSCOMM_H
#define DSMNATSCOMM_H

#include <dsmVmeDefs.h>
#include <messageDefs.h>
#include <natsDefs.h>

#include <IsioPort.h>
#include <dsmctl.h>

class DsmNatsComm : public IsioPort {
public:
  DsmNatsComm(void (*stat_msg)(char *msg));
  void send (const char *buf, const int len);
              
private:
  void (*statusMsg)(char* msg);         	// status msg handler

  NatsPacketHdr packet_hdr;
  char msg[DSM_MSG_STRING_SIZE];                // status message string

};

#endif

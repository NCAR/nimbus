/* DiscNatsComm.h
   Handles NATS serial port communications in the DSM.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef DISCNATSCOMM_H
#define DISCNATSCOMM_H

#include <sys/types.h>
#include <sys/param.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/termios.h>


#include <natsDefs.h>
#include <SerialBuf.h>

class DiscNatsComm {
public:
  DiscNatsComm(int buf_size);
  int send (char *buf, int len);		// send nats data 
  int recv (int len);				// receive nats data

  inline char *buffer()         {return lbuf->getBuf();}
  inline void releaseBuf()      {lbuf->releaseBuf();}
              
private:
  void initPort();

  SerialBuf *lbuf;				// local data buffer
  NatsPacketHdr packet_hdr;
  int port;
  int hdr_idx;					// packet header index
  int misalign_cnt;				// misaligned char count
};

#endif

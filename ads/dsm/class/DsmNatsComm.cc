/* DsmNatsComm.cc
   Handles NATS serial port communications in the DSM.

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research

   Revisions:


*/

#include <DsmNatsComm.h>

/******************************************************************************
** Public Functions
******************************************************************************/

DsmNatsComm::DsmNatsComm (void (*stat_msg)(char *msg)) : 
                          IsioPort ((const char*)(A24D16_BASE + ISIO1_BASE), 
                                    NATS_PORT, NATS_BAUD, NATS_DATA_BITS, 
                                    NATS_PARITY, NATS_STOP_BITS, 
                                    NATS_HW_HANDSHAKE, stat_msg),
                          statusMsg (stat_msg)
                        
// Constructor.
{
  packet_hdr.flag = NATS_PACKET_FLAG;
  packet_hdr.type = NATS_SMP_TYPE;	// only sending sampled data for now
  packet_hdr.seq = 1;
  packet_hdr.final_seq = 1;		// no multiple packets for now
}
/*****************************************************************************/

void DsmNatsComm::send (const char *buf, const int len)

// Writes data to the nats serial port.
{
  packet_hdr.length = len;

// Send the packet header and buffer. 
  if (putcnt ((char*)&packet_hdr, sizeof(packet_hdr)) || pputcnt (buf, len)) {
    sprintf (msg, 
             "DsmNatsComm: Failure sending data to the NATS serial port.\n");
    statusMsg (msg);
  }
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

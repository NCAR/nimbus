/* DiscNatsComm.cc
   Handles NATS serial port communications on the DSM control station.

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research

   Revisions:

*/

#include <DiscNatsComm.h>

/******************************************************************************
** Public Functions
******************************************************************************/

DiscNatsComm::DiscNatsComm (int buf_size) 
                        
// Constructor.
{
  lbuf = new SerialBuf (buf_size);	// create the local buffer

  packet_hdr.flag = NATS_PACKET_FLAG;
  packet_hdr.type = NATS_SMP_TYPE;	// only sending sampled data for now
  packet_hdr.seq = 1;
  packet_hdr.final_seq = 1;		// no multiple packets for now
  hdr_idx = 0;

  initPort();
}
/*****************************************************************************/

int DiscNatsComm::send (char *buf, int len)

// Writes data to the nats serial port.
{
  packet_hdr.length = len;

// Send the packet header and buffer. 
  if ((write (port, (char*)&packet_hdr, sizeof(packet_hdr)) != 
       sizeof (packet_hdr)) || (write (port, buf, len) != len)) {
    fprintf (stderr, "DiscNatsComm: Failure sending NATS data.\n");
    return 0;
  }
  return len;
}
/*****************************************************************************/

int DiscNatsComm::recv (int len)

// Reads data from the nats serial port.  Returns len if the full data block
// is complete, 0 otherwise.
{
  int rlen;

// Read the packet id one byte at a time, and make sure the data is aligned.
  if (hdr_idx < sizeof (packet_hdr.flag)) {

    rlen = read (port, (char*)&packet_hdr + hdr_idx, 1);

    switch (rlen) {
      case 0:
        return 0;

      case 1:			// check for valid flag char
        if (*((char*)&packet_hdr + hdr_idx) == NATS_FLAG_BYTE) {
          hdr_idx++;
          if (hdr_idx == sizeof (packet_hdr.flag)) {
//          printf ("Nats packet flag received, misalign_cnt = %d.\n", 
//                    misalign_cnt);
            misalign_cnt = 0;
          }
        }
        else {
          if (!misalign_cnt) 
            fprintf (stderr, "DiscNatsComm: Misaligned NATS packet.\n");
          misalign_cnt++;
          hdr_idx = 0;
      	}
        return 0;

        default: 
          perror ("DiscNatsComm: read");
          hdr_idx = 0;
          misalign_cnt = 0;
    }
    return 0;
  }

// Read the rest of the packet header.
  if (hdr_idx < sizeof (packet_hdr)) {
    if ((rlen = read (port, (char*)&packet_hdr + hdr_idx, 
                      sizeof(packet_hdr) - hdr_idx)) < 0) {
      perror ("DiscNatsComm: read");
      hdr_idx = 0;
      misalign_cnt = 0;
      return 0;
    }
    hdr_idx += rlen;
//  if (hdr_idx == sizeof (packet_hdr))
//    printf ("DiscNatsComm: packet header received.\n");

    return 0;
  }

// Read the data block.
  if ((rlen = read (port, lbuf->getBufPtr(), len - lbuf->bufIndex())) < 0) {
    perror ("DiscNatsComm: read");
    lbuf->releaseBuf();
    hdr_idx = 0;
    return 0;
  }
  if (rlen) {
    lbuf->incrBufIndex (rlen);
//  printf ("DiscNatsComm: rlen = %d, bufIndex = %d\n", rlen, lbuf->bufIndex());
  }

// Check if the data block is complete.
  if (lbuf->bufIndex() < len)
    return 0;

  hdr_idx = 0;
  return len; 
}
/******************************************************************************
** Private Functions
******************************************************************************/
 
void DiscNatsComm::initPort()
 
/* Configure the serial port. */
{
  struct termios tio_cfg;
 
  if ((port = open(NATS_PORT_NAME, O_RDWR)) < 0) {
    perror ("DiscNatsComm: open");
    exit(0);
  }
  memset ((char*)&tio_cfg, 0, sizeof (struct termios));
  fprintf (stderr, "NATS port = %d\n", port);
 
// Configure for no post processing, 2400 baud, 8 data bits, 1 stop bit,
// enable receiver,
  tio_cfg.c_iflag = BRKINT | IGNPAR;
  tio_cfg.c_oflag = 0;
  tio_cfg.c_cflag = B2400 | CS8 | CREAD;
  tio_cfg.c_lflag = ISIG | NOFLSH;
 
  if (ioctl(port, (int)TCSETS, (char*)&tio_cfg) < 0) {
    perror ("DiscNatsComm: ioctl TCSETS");
    exit(0);
  }
}
/*****************************************************************************/

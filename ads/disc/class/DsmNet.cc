/* DsmNet.cc
   General purpose socket communications class for a distributed sampling 
   module. 

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <DsmNet.h>
#ifdef VXWORKS
#include <errno.h>
#else
#include <cerrno>
#endif

/******************************************************************************
** Public Functions
******************************************************************************/

DsmNet::DsmNet(int conn_type, int buf_size, int port, char *host, char *name, 
                void (*netMsg)(int action, char* name, char* msg_str)) 
{
  netMessage = netMsg;		// message handler function pointer
  isDsm = false;
  haveHdr = false;

  dsm_sock = new TcpSocket(port, host);

  if (strncmp(host, "dsm", 3) == 0 ||
      strncmp(host, "masp", 4) == 0)	// incredibly cheezy hack.  CJW
    {
    isDsm = true;
    }
  
  // Create data buffers.
  if (buf_size)
    dsm_buf = new SerialBuf(buf_size);         // sampled data buffer class
  else
    dsm_buf = (SerialBuf*)NULL;

// It is important to have some delay on the select time out, otherwise
// cpu useage goes way up unnecessarily.
  sel_tout.tv_sec = 0;
  sel_tout.tv_usec = 10000;

  connect_type = conn_type;
  write_hdr.id = htonl(DSM_NET_ID);

  strcpy(net_name, name);			// copy this nets name
  net_enable = true;				// enable data flow
  resetNet();

/**
  fprintf (stderr, "DsmNet: port = %d, host = %s, name = %s\n", 
           port, host, name);
**/
}

/*****************************************************************************/
void DsmNet::resetNet()

// Check for network errors.
{
  closeNet();
  dsm_sock->openSock(connect_type);

  if (connect_type == TCP_ACCEPT)
    dsm_sock->listenSock();

  dsm_buf->releaseBuf();
  haveHdr = false;
  sprintf (msg_str, "Network %s is reset.\n", net_name);
  netMessage(NET_DISCONNECTED, net_name, msg_str);

}

/*****************************************************************************/
int DsmNet::connectNet() // Attempt to make the socket connection.
{
  if (!net_connect) {
    dsm_sock->closeSock();
    dsm_sock->openSock(TCP_CONNECT);

    if (net_connect = dsm_sock->connectSock()) {
      sprintf(msg_str, "Network %s is connected.\n", net_name);
      netMessage(NET_CONNECTED, net_name, msg_str);
      }
    }

  return(net_connect);

}

/*****************************************************************************/
int DsmNet::acceptNet() // Accept the socket connection.
{
  if (net_connect)
    {
    /* On the Unix side, check when last data came in (from DSM's only),
     * reset net if too long (in seconds).
     */
    if (isDSM() && time(NULL) - timeOfLastRcv > 10)
      resetNet();
    }
  else
    if ((net_connect = dsm_sock->acceptSock()))
      {
      sprintf(msg_str, "Network %s is connected.\n", net_name);
      netMessage(NET_CONNECTED, net_name, msg_str);

      if (isDSM())
        {
        dsm_sock->setRecvBufSize(65536);
        dsm_sock->setSendBufSize(1024);
        }
      }

  return(net_connect);

}

/*****************************************************************************/
int DsmNet::readNet() // Reads data from the network.
{
  int len;
  int rlen;
 
  if (!net_connect)
    return DSM_NO_DATA;

  if (!dsm_sock->querySock()) 
    return DSM_NO_DATA;

// If this is the start of a new data record.  Read the record header, to
// determine the data type and length.
  if (dsm_buf->bufEmpty() && haveHdr == false)
    {
    for (len = 0; len < sizeof(read_hdr); len += rlen)
      {
      rlen = sizeof(read_hdr) - len;
      if ((rlen = dsm_sock->readSock((char*)&read_hdr, rlen)) <= 0)
        {
        fprintf(stderr, "Network %s read packet header error %d %d.\n", net_name);
        resetNet();
        return(DSM_NO_DATA);
        }
      }

// Verify the header.
    if (len != sizeof(read_hdr))
      {
      fprintf(stderr, "Network %s read an invalid packet header length of %d bytes.\n",
        net_name, len);
      resetNet();
      return(DSM_NO_DATA);
      }

    if (ntohl(read_hdr.id) != DSM_NET_ID)
      {
      fprintf(stderr, "Network %s read an invalid packet header id of %04X.\n", 
        net_name, ntohl(read_hdr.id));
      resetNet();
      return(DSM_NO_DATA);
      }

    // If this is an activity header, just discard it.
    if (ntohl(read_hdr.type) == DSM_ACTIVITY_DATA) 
      return(DSM_NO_DATA);

    haveHdr = true;

    if (!dsm_sock->querySock()) 
      return DSM_NO_DATA;
    }


  // Compute the number of bytes left to read.
  len = ntohl(read_hdr.len) - dsm_buf->bufIndex();

  if (len > dsm_buf->spaceAvail())
    {
    fprintf(stderr, "Network %s has insufficient buffer space.\n", net_name);
    return(DSM_NO_DATA);
    }

  // Read the data.
  if ((rlen = dsm_sock->readSock(dsm_buf->getBufPtr(), len)) <= 0)
    {
    fprintf(stderr, "Network %s read an invalid data length of %d bytes.\n",
	net_name, rlen);
    resetNet();
    return(DSM_NO_DATA);
    }

  // Increment the buffer index.
  dsm_buf->incrBufIndex(rlen);

  if (isDSM())
    timeOfLastRcv = time(NULL);

  // Check for a completed record.
  if (dsm_buf->bufIndex() >= ntohl(read_hdr.len))
    {
    haveHdr = false;
    dsm_buf->setBufFull();
    return(ntohl(read_hdr.type));
    }

  return(DSM_NO_DATA);

}

/*****************************************************************************/
int DsmNet::writeNet(const char *const buf, int len, int type)
{
  int wlen = 0;

  if (!net_enable)		// don't send data if the net is disabled
    return wlen;

// Send the net header.
  write_hdr.type = htonl(type);
  write_hdr.len = htonl(len);

  if (net_connect && 
      (dsm_sock->writeSock ((char*)&write_hdr, sizeof (write_hdr)) == ERROR)) {
    fprintf(stderr, "Network %s writeNet error.\n", net_name);
    resetNet();
    }
 

// Send the data.
  if (net_connect && ((wlen = dsm_sock->writeSock(buf, len)) == ERROR)) {
    fprintf (stderr, "Network %s writeNet error.\n", net_name);
    resetNet();
    }

  return(wlen);

}

/*****************************************************************************/
void DsmNet::closeNet()
{
  dsm_sock->closeSock();
  if ((int)dsm_buf)
    dsm_buf->releaseBuf();

  net_connect = FALSE;

}

/*****************************************************************************/
void DsmNet::sendActivityPacket()
 
// Writes an activity packet to the network.  The activity packet is used to 
// check the net connection when no other data is being sent.  
{
// Send the net header.
  write_hdr.type = htonl(DSM_ACTIVITY_DATA);
  write_hdr.len =  0;

  if (net_connect && (dsm_sock->writeSock ((char*)&write_hdr,
      sizeof (write_hdr)) == ERROR)) {
    fprintf (stderr, "DsmNet: sendActivityPacket error\n");
    resetNet();				// connection is broken
    }

}

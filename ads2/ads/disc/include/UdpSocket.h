/* UdpSocket.h
   Creates and manages a UDP socket.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef VXWORKS
#include <net/if.h>
#include <sys/ioctl.h>
#include <errnoLib.h>
#include <hostLib.h>
#include <in.h>
#include <ioLib.h>
#include <selectLib.h>
#include <sockLib.h>
#include <taskLib.h>

#else

#ifdef SVR4
#include <sys/sockio.h>
#else
#include <sys/time.h>
#endif
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <net/if.h>
#include <netinet/in.h>
#include <unistd.h>

#endif

#include <dsmctl.h>

#define UDP_UNBOUND	0
#define UDP_BOUND	1
#define UDP_BROADCAST	2

class UdpSocket {
public:
  UdpSocket(int, char*);		// constructor
  void openSock (int);			// open the socket
  int querySock();			// query for data avail
  int readSock (char*, int);		// read from the socket
  int writeSock (char*, int);		// write to the socket
  void closeSock();			// close the socket
  inline int sockId() {return udp_sock;}	// get the socket handle

private:
  struct sockaddr_in sock_name;		// network name struct
  char	udp_host[25];			// target machine host name
  int	udp_num;			// port number
  int	udp_sock;			// socket fd
};

#endif

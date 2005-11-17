/* UdpSocket.cc
   Creates and manages a UDP socket.

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <UdpSocket.h>

/******************************************************************************
** Public Functions
******************************************************************************/

UdpSocket::UdpSocket (int sock_num, const char *host)
{
  udp_num = sock_num;
  strcpy (udp_host, host);

}

/*****************************************************************************/
void UdpSocket::openSock (int binding)
{
  struct ifconf ifc;
  struct ifreq *ifr;
  char buf[BUFSIZ], buf2[32];
  int opt = 1;
  int n;

  struct hostent *hp;
  socklen_t length;

// Open the socket.
  if ((udp_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror ("UdpSocket:socket");
    exit(1);
  }

// Allow previously used addresses to be reused.
  if (setsockopt (udp_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt,
                  sizeof(opt)) == (-1)) {
    perror ("UdpSocket:setsockopt SO_REUSEADDR");
    exit (1);
  }

  sock_name.sin_family = AF_INET;
  sock_name.sin_port = htons(udp_num);
  
  switch (binding) {
    case UDP_BOUND:				// bound socket request
      sock_name.sin_addr.s_addr = htonl(INADDR_ANY);
      if (bind(udp_sock, (struct sockaddr *)&sock_name, sizeof(sock_name))< 0) {
        perror ("UdpSocket:bind");
        exit(1);
      }
// Read back the assigned port number. 
      length = sizeof(sock_name);
      if (getsockname (udp_sock, (struct sockaddr*)&sock_name, &length) < 0) {
        perror ("UdpSocket: getsockname");
        exit(1);
      }
      printf ("Datagram socket port #%d\n", ntohs(sock_name.sin_port));
      break;

    case UDP_UNBOUND:				// unbound socket request
      if((hp = gethostbyname(udp_host)) == 0) {
        perror ("UdpSocket:gethostbyname");
        exit(1);
      }
      memcpy ((char*)&sock_name.sin_addr.s_addr, (char*)hp->h_addr_list[0],
             hp->h_length);
      break;

    case UDP_BROADCAST:				// broadcast socket requested
      sock_name.sin_addr.s_addr = htonl(INADDR_ANY);

      if (bind(udp_sock, (struct sockaddr*)&sock_name, sizeof(sock_name))< 0) {
        perror("UdpSocket:bind");
        exit(1);
      }

      if (setsockopt(udp_sock, SOL_SOCKET, SO_BROADCAST, (char*)&opt,
                     sizeof(opt)) == (-1)) {
        perror("UdpSocket:setsockopt SO_BROADCAST");
        exit(1);
      }

      ifc.ifc_len = sizeof(buf);
      ifc.ifc_buf = buf;
      if (ioctl(udp_sock, SIOCGIFCONF, (int)&ifc) < 0) {
        perror("UdpSocket: ioctl SIOCGIFCONF");
        exit (1);
      }

      ifr = ifc.ifc_req;

      for (n = ifc.ifc_len / sizeof (struct ifreq); --n >= 0; ifr++) {
        if (ifr->ifr_addr.sa_family != AF_INET)
          continue;

        if (ioctl(udp_sock, SIOCGIFFLAGS, (int)ifr) < 0) {
          perror("UdpSocket: ioctl SIOCGIFFLAGS");
          exit (1);
        }

        if ((ifr->ifr_flags & IFF_UP) == 0 ||
            (ifr->ifr_flags & IFF_LOOPBACK) ||
            (ifr->ifr_flags & (IFF_BROADCAST | IFF_POINTOPOINT)) == 0)
          continue;

        if (ioctl(udp_sock, SIOCGIFBRDADDR, (int)ifr) < 0) {
          perror("ioctl SIOCGIFBRDADDR");
          exit (1);
        }

        sprintf(buf2, "%u.%u.%u.%u",
           (unsigned char)ifr->ifr_broadaddr.sa_data[2],
           (unsigned char)ifr->ifr_broadaddr.sa_data[3],
           (unsigned char)ifr->ifr_broadaddr.sa_data[4],
           (unsigned char)ifr->ifr_broadaddr.sa_data[5]);

        if (strcmp(buf2, udp_host))
          continue;

        memcpy((char *)&sock_name, (char *)&ifr->ifr_broadaddr,
               sizeof (ifr->ifr_broadaddr));
        }

      // Re-insert port number.
      sock_name.sin_port = htons(udp_num);
      break;

    default:
      fprintf (stderr, "UdpSocket: Unkown binding requested, %d\n", binding);
      exit (1);
  }
}

/*****************************************************************************/
int UdpSocket::querySock ()
{
  fd_set read_mask;                     // select read mask
  struct timeval sel_tout;              // select timeout struct
 
  sel_tout.tv_sec = 0;
  sel_tout.tv_usec = 0;
 
  FD_ZERO (&read_mask);
  FD_SET (udp_sock, &read_mask);
 
  if (select (FD_SETSIZE, &read_mask,(fd_set*)0,(fd_set*)0,&sel_tout) < 0)
    {
    switch (errno) {
      case EINTR :
        return false;
      default:
        perror ("UdpSocket: select");
        exit (1);
    }
  }

  return FD_ISSET (udp_sock, &read_mask);
}
/*****************************************************************************/
int UdpSocket::readSock (char *dest, int len)
{
  int rlen;

  if ((rlen = read (udp_sock, dest, len)) == (-1)) {
    perror ("UdpSocket:read");
  }
  return rlen;
}

/*****************************************************************************/
int UdpSocket::writeSock (const char *src, int len)
{
  int slen;
 
  if ((slen = sendto(udp_sock, src, len, 0,
       (struct sockaddr *)&sock_name, sizeof(sock_name))) == (-1)) {
    perror ("UdpSocket:sendto");
  }
  return slen;
}

/*****************************************************************************/
void UdpSocket::closeSock ()
{
  if (udp_sock) {
    close (udp_sock);
    udp_sock = 0;
  }

}

/* TcpSocket.cc
   Creates and manages a TCP socket.

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <TcpSocket.h>

/******************************************************************************
** Public Functions
******************************************************************************/

TcpSocket::TcpSocket(int sock_num, const char *host)
{
  tcp_num = sock_num;
  strcpy(tcp_host, host);
  tcp_sock = 0;
  tcp_conn = 0;
}

/*****************************************************************************/
void TcpSocket::openSock(int accept)
{
  int opt = 1;

// Open the connection socket if this socket is to accept connections.
  if (accept == TCP_ACCEPT) {
    if ((tcp_conn = socket(AF_INET, SOCK_STREAM, 0)) == ERROR) {
      perror ("TcpSocket:socket");
      exit(ERROR);
    }

// Allow previously used socket addresses to be reused.
    if (setsockopt(tcp_conn, SOL_SOCKET, SO_REUSEADDR, (char*)&opt,
                    sizeof(int)) == ERROR) {
      perror("TcpSocket:setsockopt tcp_conn");
      exit(ERROR);
    }

    memset((void *)&sock_name, 0, sizeof(sock_name));
    sock_name.sin_family = htons(AF_INET);
    sock_name.sin_addr.s_addr = INADDR_ANY;
    sock_name.sin_port = htons(tcp_num);
    if (bind(tcp_conn, (struct sockaddr *)&sock_name, sizeof(sock_name)) == 
        ERROR) {
      perror("TcpSocket:bind");
      printf("tcp_num = %d\n", tcp_num);
      exit(ERROR);
    }

    tcp_sock = tcp_conn;
  }

// Otherwise this socket is to initiate connections.
  else { 
    if ((tcp_sock = socket(AF_INET, SOCK_STREAM, 0)) == ERROR) {
      perror("TcpSocket:socket");
      exit(ERROR);
    }

  }
}
/*****************************************************************************/

int TcpSocket::connectSock ()

// Connects the socket.
{
#ifdef VXWORKS
  int hp;                               /* boot host internet address */
#else
  struct hostent *hp;
#endif
 
  memset((char*)&sock_name, 0, sizeof (sock_name));
  sock_name.sin_family = AF_INET;
  sock_name.sin_port = htons(tcp_num);
 
#ifdef VXWORKS
  if((hp = hostGetByName(tcp_host)) == 0) {
    perror ("TcpSocket:hostGetByName");
    exit(ERROR);
  }
  sock_name.sin_addr.s_addr = hp;

#else
  if ((hp = gethostbyname (tcp_host)) == 0) {
    perror ("TcpSocket:gethostbyname");
    exit(ERROR);
  }
  memcpy ((char*)&sock_name.sin_addr.s_addr, (char*)hp->h_addr_list[0],
         hp->h_length);
#endif

  if (connect(tcp_sock, (struct sockaddr*)&sock_name, sizeof(sock_name)) == 
      ERROR) {
    if (tcp_num == 1112)
      perror("TcpSocket:connect 1112");
    switch (errno) {
      case EINTR:
      case ECONNREFUSED:
      case ETIMEDOUT:
        break;
      default:
        perror("TcpSocket:connect");
        exit (ERROR);
     }
    return FALSE;
  }

  return(TRUE);
}

/*****************************************************************************/
 
int TcpSocket::acceptSock ()
 
// Accepts a socket connection.
{
  int	opt = 65536, len;
  fd_set read_mask;                     // select read mask
  struct timeval sel_tout;              // select timeout struct
 
  sel_tout.tv_sec = 0;
  sel_tout.tv_usec = 0;
 
  FD_ZERO (&read_mask);
  FD_SET (tcp_conn, &read_mask);
  if (select (FD_SETSIZE, &read_mask,(fd_set*)0,(fd_set*)0,&sel_tout) == ERROR){
    perror ("TcpSocket:select");
    switch (errno) {
      case EINTR :
        return FALSE;
      default:
        exit(ERROR);
    }
  }

// Check for a connection request. We don't close the tcp_conn sockect here.
// Reconnections seem to get confused if it is closed.

  if (FD_ISSET(tcp_conn, &read_mask)) {
#ifdef VXWORKS
    if ((tcp_sock = accept(tcp_conn, (struct sockaddr*)0, (int*)0)) == ERROR) {
#else
    if ((tcp_sock = accept(tcp_conn, (struct sockaddr*)0, (socklen_t*)0)) == ERROR) {
#endif
      perror("TcpSocket:accept");
      exit(ERROR);
    }

    return(TRUE);			// connection made
  }

  return(FALSE);

}

/*****************************************************************************/
int TcpSocket::querySock ()
 
// Query the socket for data received.
{
  fd_set read_mask;                     // select read mask
  struct timeval sel_tout;              // select timeout struct

  sel_tout.tv_sec = 0;
  sel_tout.tv_usec = 5000;
 
  FD_ZERO (&read_mask);
  FD_SET (tcp_sock, &read_mask);
 
  if (select(FD_SETSIZE, &read_mask,(fd_set*)0,(fd_set*)0,&sel_tout) == ERROR)
    {
    perror("TcpSocket:select");

    switch (errno) {
      case EINTR :
        return FALSE;

      default:
        perror ("TcpSocket: select");
        exit (ERROR);
      }
    }

  return(FD_ISSET(tcp_sock, &read_mask));

}

/*****************************************************************************/
int TcpSocket::readSock(char *const dest, int len)
{
  int rlen;

  if ((rlen = read (tcp_sock, dest, len)) == ERROR) {
    perror ("TcpSocket:read");
    }

  return(rlen);

}

/*****************************************************************************/
int TcpSocket::writeSock(const char *src, int length)
{
  int len, slen;
 
  for (len = 0; len < length; len += slen) {
    if ((slen = write (tcp_sock, (char*)src + len, length - len)) == ERROR) {
      perror ("TcpSocket:write");
      return ERROR;
      }
    }

  return(length);

}

/*****************************************************************************/
void TcpSocket::closeSock()
{
  if (tcp_sock) 
    close(tcp_sock);

  if (tcp_conn) 
    close(tcp_conn);

  tcp_sock = tcp_conn = 0;

}

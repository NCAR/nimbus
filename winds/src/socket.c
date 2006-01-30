/* socket.c -- provide interface for socket operations
*/
#include <sys/types.h>
#include <sys/errno.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#ifdef SVR4
#include <sys/filio.h>
#include <sys/sockio.h>
#endif
#include <netinet/in.h>
#include <net/if.h>
#include <netdb.h>
#include <stdio.h>
#include <fcntl.h>

/********************   WINDS INCLUDES  **************************************/

#include <socket.h>
#include <constants.h>
#include <proto.h>

/************************  GLOBAL IN FILE  ************************************/

static int sock[NUM_PORTS];
static struct sockaddr_in server[NUM_PORTS];
static struct timeval timeout;
static int ProtocolDebug=FALSE;

/*******************************************************************************
listening posts: listen_on_socket(client,port,addr) 
	 	 bind_socket(port,addr) 
		 read_from_socket(port,length,mode)
*******************************************************************************/

/**********************  GetIPCString()  ************************************/

char *
GetIPCString(string)
char *string;
{
/*
this is not working correctly yet; identifying which messages call for
passing through the next one as literal instead of interpreting as
socket identifier needs some help (nextstringisvalue stuff)
*/

int which;
static int nextstringisvalue=FALSE;

 if (nextstringisvalue) {
  nextstringisvalue=FALSE;
  return string;
 }
 which = atoi(string);
 switch (which) {
  case ARE_YOU_LISTENING:
   return "ARE_YOU_LISTENING";
  case LISTENING:
   return "LISTENING";
  case RESET_PARCEL:
   return "RESET_PARCEL";
  case REQ_SEARCH_FWD:
   return "REQ_SEARCH_FWD";
  case REQ_SEARCH_BACK:
   return "REQ_SEARCH_BACK";
  case SHMEM_REFILLED:
   return "SHMEM_REFILLED";
  case NO_SHMEM_REFILLED:
   return "NO_SHMEM_REFILLED";
  case REQ_FREEZE:
   return "REQ_FREEZE";
  case REQ_UNFREEZE:
   return "REQ_UNFREEZE";
  case REQ_SET_UPDATES:
   nextstringisvalue=TRUE;
   return "REQ_SET_UPDATES";
  case REQ_SET_START_TIME:
   nextstringisvalue=TRUE;
   return "REQ_SET_START_TIME";
  case REQ_DATE:
   return "REQ_DATE";
  case REQ_BCAST:
   nextstringisvalue=TRUE;
   return "REQ_BCAST";
  case REQ_SET_RATE:
   nextstringisvalue=TRUE;
   return "REQ_SET_RATE";
  case REQ_SET_REALTIME:
   return "REQ_SET_REALTIME";
  case REQ_SET_WARP2:
   return "REQ_SET_WARP2";
  case REQ_SET_WARP4:
   return "REQ_SET_WARP4";
  case REQ_SET_WARP10:
   return "REQ_SET_WARP10";
  case REQ_CHG_DATE:
   nextstringisvalue=TRUE;
   return "REQ_CHG_DATE";
  case REQ_USER_ALERT:
   return "REQ_USER_ALERT";
  case INITPLOTSPEXFILE:
   return "INITPLOTSPEXFILE";
  case PLOTSPEXFOLDERREMOVED:
   return "PLOTSPEXFOLDERREMOVED";
  case PRINTRESPONSE:
   return "PRINTRESPONSE";
  case UPDATEFLIGHTNUM:
   return "UPDATEFLIGHTNUM";
  default:
   return string;
 }
}

/**************************  GetPortName()  **********************************/

char *
GetPortName(port)
int port;
{
 switch (port) {
  case DISP_REQ:
   return "DISP_REQ";
  case DISP_REQ_REPLY:
   return "DISP_REQ_REPLY";
  default:
   return "UNKNOWN";
 } 
}

/**********************  SetBlockTimeOut()  **********************************/

SetBlockTimeOut(seconds)
int seconds;
{
 timeout.tv_sec=seconds; timeout.tv_usec=0;
}

/***************************  LISTEN_ON_SOCKET()  *****************************/

listen_on_socket(client,port,addr)
char *client;
int port,addr;
/*
set timeout parameters for blocking-with-time-out socket connections;
call WaitForSocket() and return its return value.
*/
{
int timeout_set=FALSE;

 if (!timeout_set) {
  SetBlockTimeOut(DEFAULT_TIMEOUT_SECONDS);
  timeout_set=TRUE;
 }
 return WaitForSocket(client,port,addr);
}

/***************************  WaitForSocket()  ****************************/

WaitForSocket(client,port,addr)
char *client;
int port,addr;
/* 
attempt a bind_socket with port port and address addr, return ERROR if problem,
else return OK.

argument client is any string used to distinguish which process is
making the call, in case of problem (so many messages go to same log
file that this can prove useful in diagnostics).
*/
{
int attempts,retval;

 attempts=0;
 do {
  switch (retval=bind_socket(port,addr)) {
  case EWRONGPORT:
   (void)fprintf(stderr,
   "%s EWRONGPORT error on %s: port # not same as requested: ",progName,client);
   perror("");
   return(retval);
  case EBADOPEN:
   (void)fprintf(stderr,"%s EBADOPEN error on %s: ",progName,client);
   perror("");
   return(retval);
  case EBADBIND:
   if (errno==EADDRINUSE && attempts++<3) {
/* 
sometimes, due to timing, an old input process has not relinquished its
ownership of this socket before this new input process wants it.  Try 
a few more times before giving up.
*/
    (void)sleep(1);
   } else {
    (void)fprintf(stderr,"%s EBADBIND error on %s: ",progName,client);
    perror("");
    return(retval);
   }
   break;
  case EBADNAME:
   (void)fprintf(stderr,"%s EBADNAME error on %s: ",progName,client);
   perror("");
   return(retval);
  case BIND_OK:
/***
   (void)fprintf(stderr,
    "%s Listening enabled on socket %s: port %d, addr %d\n",
    progName,client,port,addr);
***/
   return OK;
  }
 } while (retval==EBADBIND);
 (void)fprintf(stderr,"%s WARNING: UNK status on socket bind for %s: port %d, addr %d",progName,client,port,addr);
 return OK;
}

/*****************************************************************************/

SocketIsBoundForListening(port)
int port;
{
 return (ntohl(server[port].sin_addr.s_addr) == INADDR_ANY);
}

/**********************  BIND_SOCKET()  **************************************/

bind_socket(port,address)
int port,address;
{
int length;

 sock[port]=socket(AF_INET,SOCK_DGRAM,0);
 if (sock[port] < 0) {
  return(EBADOPEN);
 }

/* set the "close_on_exec" flag for all bound sockets so that new display
   processes may be started while an input process from previous display
   session is to remain running.  This prevents the error condition
   "binding socket: Address already in use" from occurring, since the
   vfork'd children do NOT inherit the listening post socket descriptors.
*/
 (void)fcntl(sock[port],F_SETFD,TRUE);
 server[port].sin_family = htons(AF_INET);
 server[port].sin_addr.s_addr = htonl(INADDR_ANY);
 server[port].sin_port = htons(FIRST_AVAILABLE_PORT+address);
 if (bind(sock[port],(struct sockaddr *)&server[port],sizeof server[port])<0) {
  printf("bind error:  errno=%d\n", errno);
  return(EBADBIND);
 }
 length=sizeof server[port];
 if(getsockname(sock[port],(struct sockaddr *)&server[port],&length) <0) {
  return(EBADNAME);
 }
 if (ntohs(server[port].sin_port) != FIRST_AVAILABLE_PORT+address) {
  return(EWRONGPORT);
 }
 return(BIND_OK);
}

/******************************************************************************/

SetProtocolDebug(debug)
int debug;
{
 return ProtocolDebug=debug;
}

/******************************************************************************/

GetProtocolDebug()
{
 return ProtocolDebug;
}

/************************  READ_FROM_SOCKET()  ********************************/

char *
read_from_socket(int port, int *length, int mode)
{
static char buffer[MAX_SOCKET_BUFF_LNGTH];
unsigned int len;
int retval;
fd_set read_mask;

 if (*length>MAX_SOCKET_BUFF_LNGTH) {
  (void)fprintf(stderr,
   "%s WARNING: pid %d finds %d bytes on socket, max is (%d)\n",
	progName,getpid(), *length,MAX_SOCKET_BUFF_LNGTH);
  *length=MAX_SOCKET_BUFF_LNGTH;
 }
 len=*length;
 if (mode==NON_BLOCKING) {
  (void)fcntl(sock[port],F_SETFL,O_NDELAY);
  if ((*length=read(sock[port],buffer,len)) < 0) {
   if (errno==EWOULDBLOCK) {
    return(NODATA);
   } else {
    perror ("reading non-blocking socket message");
    return(EBADREAD);
   }
  } 

/* socket set, data available
*/
  buffer[*length]='\0';
  if (GetProtocolDebug()) {
   if ( (port==DISP_REQ || port==DISP_REQ_REPLY) && *length ) {
    (void)fprintf(stderr,"%s Read string %s from port %s\n",progName,
     (char *)GetIPCString(buffer),(char *)GetPortName(port));
   }
  }
  return(&buffer[0]);

 } else if (mode==BLOCKING) {
/* 
turn blocking back on
*/
  int non_block=0;
  *length=0;
  if (ioctl(sock[port],FIONBIO,&non_block)<0) {
   perror("ioctl FIONBIO");
   return(EBADREAD);
  }
/* 
blocking socket w/o time out mechanism.  Wait until some response
from socket reads.
*/
  if ((*length=read(sock[port],buffer,len)) < 0) {
   perror ("reading blocking socket message");
   return(EBADREAD);
  }
  if (GetProtocolDebug()) {
   if ( (port==DISP_REQ || port==DISP_REQ_REPLY) && length ) {
    (void)fprintf(stderr,"%s Read string %s from port %s\n",progName,
     (char *)GetIPCString(buffer),(char *)GetPortName(port));
   }
  }
  buffer[*length]='\0';
  return(&buffer[0]);

 } else if (mode==BLOCK_TIME_OUT) {

  int non_block=0;
  *length=0;
  if (ioctl(sock[port],FIONBIO,&non_block)<0) {
   perror("ioctl FIONBIO");
   return(EBADREAD);
  }

/* blocking socket with time out mechanism.  Loop until some response
   from socket reads.
*/
  do {
   FD_ZERO(&read_mask);
   FD_SET(sock[port],&read_mask);
   if ((retval=select(FD_SETSIZE, &read_mask,(fd_set*)0,(fd_set*)0,&timeout))
    ==ERROR){
    switch (errno) {

/* volunteer interrupts can simply be ignored
*/
     case EINTR:
      fprintf(stderr,"%s pid %d blocking socket: interrupted read select\n",
	progName,getpid());
      continue;
     default:
      perror ("blocking socket: read select");
      return(EBADREAD);
    }
   } 
   if (retval==0) {

/* no response within designated time-out period
*/
    (void)fprintf(stderr,"%s blocking socket read timed-out\n", progName);
    return (EBADREAD);
   }

/* even with an interrupted read, the response may have come...check for it 
   here
*/
   if (FD_ISSET (sock[port], &read_mask)) {
    if ((*length=read(sock[port],buffer,len)) < 0) {

/* socket set, but no data there...
*/
     perror ("reading non-blocking socket message");
     return(EBADREAD);
    } else {

/* socket set, data available
*/
     buffer[*length]='\0';
     if (GetProtocolDebug()) {
      if ( (port==DISP_REQ || port==DISP_REQ_REPLY) && length ) {
       (void)fprintf(stderr,"%s Read string %s from port %s\n",progName,
        (char *)GetIPCString(buffer),(char *)GetPortName(port));
      }
     }
     return(&buffer[0]);
    }
   }
  } while (buffer==buffer);
/*
reset default timeout value in case caller has specified a different one
*/
  SetBlockTimeOut(DEFAULT_TIMEOUT_SECONDS);
 }
 return NODATA;
}

/****************************************************************************

talking posts:		talk_to_socket(client,port,addr,host)
			make_client_socket(port,address,host)
			write_to_socket(port,indata,length)

*****************************************************************************/

/***************************  TALK_TO_SOCKET()  *****************************/

talk_to_socket(client,port,addr,host)
char *client;
int port,addr;
char *host;
{
 switch (make_client_socket(port,addr,host)) {
  case EBADCONN:
fprintf(stderr, "errno=%d\n", errno);
    fprintf(stderr, "%s FATAL: %s process is exiting because connect failed: ",progName,client);
    perror("");
    exit(-1);
  case EBADOPEN:
   (void)fprintf(stderr,"%s FATAL: %s process is exiting due to error opening stream socket...contact a WINDS expert about this problem: ",progName,client);
   perror("");
   exit(-1);
  case EBADHOST:
   fprintf(stderr,
    "%s FATAL: %s process is exiting because an unknown host (%s) was requested for socket connection...contact a WINDS expert about this problem: ",progName,client,host);
   perror("");
   exit(-1);
  case CLIENT_OK:
/***
   if (client)
    fprintf(stderr,"%s Writes enabled to socket %s on %s: port %d, addr %d\n",
    progName,client,host,port,addr);
***/
   break;
  default:
   fprintf(stderr,
    "%s WARNING: UNK status on writes to socket %s on %s: port %d, addr %d\n",
    progName,client,host,port,addr);
   break;
 }
 return;
}

/*******************  MAKE_CLIENT_SOCKET()  **********************************/

make_client_socket(port,address,host)
int port,address;
char host[];
{
struct hostent *hp,*gethostbyname();
int on=1;
 
 sock[port]=socket(AF_INET,SOCK_DGRAM,0);
 if (sock[port] < 0) {
  return(EBADOPEN);
 }

 memset((void *)&server[port], 0, sizeof(server[port]));
 server[port].sin_family = AF_INET;
 if (!(strcmp(host,BROADCAST2))) {
  setsockopt(sock[port],SOL_SOCKET,SO_BROADCAST,(const char *)&on,sizeof on);
  setBrdAddr(port);
 } else {
/*
assume host is a hostname for starters, try resolving to hostname if
that fails, asssuming the alternative is an IP number
*/
  hp=gethostbyname(host);
  if (hp==0) {
/*
that didn't work, so try host as an IP address
*/
   int addr;
   addr=inet_addr(host);

   if ( (hp = gethostbyaddr((const char *)&addr,BYTESPERWORD,AF_INET))==0)
/*
no luck either way
*/
    return(EBADHOST);
  }
/* 
the h_addr field actually does not exist in the hostent struct (but h_addrtype
does). But it DOES compile...how, I don't know.
*/
  memcpy((char *)&server[port].sin_addr.s_addr,(char *)hp->h_addr_list[0], hp->h_length);
 }
/* 
assign unique port number as function of which address.  Note that
numbers 0-1023 are reserved.
*/
 server[port].sin_port = htons(FIRST_AVAILABLE_PORT+address);

 if (connect(sock[port], (struct sockaddr*)&server[port], sizeof(server[port])) < 0)
  return(EBADCONN);

 return(CLIENT_OK);
}

/************************  HostIsAlive()  ********************************/
 
HostIsAlive(host)
char *host;
/*
NOTE: Unix/SunOS Dependent code:

send a ping command with a 1 second timeout to see if host is
alive; return the comparison of the second word in ping response
with "answer", as in "no answer from".
*/
{
FILE *pipe; 
char command[MAXHOSTNAMELEN+32],result[3][MAXHOSTNAMELEN]; 
int alive;
 
#ifdef SVR4
 (void)sprintf(command,"/usr/sbin/ping %s 1", host); 
#else
 (void)sprintf(command,"/bin/ping %s -w 1", host); 
#endif
 pipe=popen(command,"r"); 
 fscanf(pipe,"%s %s %s",result[0],result[1],result[2]); 
 pclose(pipe); 
 alive=(strcmp(result[1],"answer"));
 if (alive) {
  return TRUE;
 } else {
  return FALSE;
 }
}


/************************  WRITE_TO_SOCKET()  ********************************/
 
write_to_socket(port,indata,length)
int port;
char indata[];
int length;
{
static int first_call=1;
static char *socket_data;

 if (!sock[port])
  return EBADWRITE;

 if (first_call) {
  socket_data=(char *)malloc( (unsigned int)MAX_SOCKET_BUFF_LNGTH+1);
  first_call=0;
 }
/*
protect length of data sent
*/
 memcpy ((char *)socket_data,(char *)indata,MIN(MAX_SOCKET_BUFF_LNGTH,length));
 if (length>MAX_SOCKET_BUFF_LNGTH) {
  (void)fprintf(stderr,
   "%s WARNING: socket message of length %d is truncated at byte %d\n!!",progName,
   length,MAX_SOCKET_BUFF_LNGTH);
  length=MAX_SOCKET_BUFF_LNGTH;
 } 
 socket_data[length]='\0';
/*
add one to value of length, to accommodate the null terminator
*/
 if (GetProtocolDebug()) {
  if ( (port==DISP_REQ || port==DISP_REQ_REPLY) && length) {
   (void)fprintf(stderr,"%s Writing string %s to port %s\n",progName,
    (char *)GetIPCString(socket_data),GetPortName(port));
  }
 }

 if (sendto(sock[port],socket_data,length+1,0,(struct sockaddr *)&server[port], 
     sizeof server[port]) <0)
  return(EBADWRITE);
/***
 if (port==GENERAL_LISTENING || 
  (port>=INTERWINDS_LISTEN && port<=INTERWINDS_TALK))
/*
some strange messages being received at WINDS side...add debugging stmts to
determine who's doing it.
*/
/***
   (void)fprintf(stderr,"%s Process %d sends network message on port %d\n",
    progName,getpid(),port);
***/
 return(WRITE_OK);
}

/**************************  CLOSE_SOCKET()  **********************************/

close_socket(port)
int port;
{
 (void)close(sock[port]);
}

/********************  PrintResponseToSender ()  *****************************/

PrintResponseToSender (window, port,address,hostname,buffer)
int window,port,address;
char *hostname,*buffer;
/*
this routine is assumed used by a print server to respond to a WINDS
client that previously sent it a print request. window is the WINDS
quadrant number, or some other coded information indicating the nature 
of the request. port, address, and hostname are used to generate the
return address of socket. buffer is the message being sent.

socket connection is made to sender of previous request; protocol is
set up with PRINTRESPONSE keyword, and then remainder of information
is sent and the socket connection is closed.
*/
{
char keyword[MAX_SOCKET_BUFF_LNGTH];
 
 talk_to_socket((char *)NULL,port,address,hostname);
 (void)sprintf(keyword,"%d %d",PRINTRESPONSE,window);
 write_to_socket(port,keyword,strlen(keyword));
 write_to_socket(port,buffer,strlen(buffer));
 close_socket(port);
}

/********************  ConvertIPAddrToName ()  *****************************/

ConvertIPAddrToName(host)
char *host;
{
struct hostent *hp;

 hp=gethostbyname(host);
 if (hp==0) {
/*
that didn't work, so assume host is an IP address
*/
  int addr;
  addr=inet_addr(host);
  if ( (hp = gethostbyaddr((const char *)&addr,BYTESPERWORD,AF_INET))==0) {
/*
no luck either way
*/
   return ERROR;
  } else {
/*
got name, transfer it to incoming arg
*/
   (void)sprintf(host,"%s",hp->h_name);
   return OK;
  }
 }
 return OK;
}

void setBrdAddr(port)
{
  struct ifconf ifc;
  struct ifreq *ifr;
  char buf[BUFSIZ], buf2[32];
  int opt = 1;
  unsigned int length;
  int n;

  ifc.ifc_len = sizeof(buf);
  ifc.ifc_buf = buf;
  if (ioctl(sock[port], SIOCGIFCONF, (int)&ifc) < 0) {
    perror("UdpSocket: ioctl SIOCGIFCONF");
    exit (ERROR);
  }

  ifr = ifc.ifc_req;

  for (n = ifc.ifc_len / sizeof (struct ifreq); --n >= 0; ifr++) {
    if (ifr->ifr_addr.sa_family != AF_INET)
      continue;

    if (ioctl(sock[port], SIOCGIFFLAGS, (int)ifr) < 0) {
      perror("UdpSocket: ioctl SIOCGIFFLAGS");
      exit (ERROR);
    }

    if ((ifr->ifr_flags & IFF_UP) == 0 ||
        (ifr->ifr_flags & IFF_LOOPBACK) ||
        (ifr->ifr_flags & (IFF_BROADCAST | IFF_POINTOPOINT)) == 0)
      continue;

    if (ioctl(sock[port], SIOCGIFBRDADDR, (int)ifr) < 0) {
      perror("ioctl SIOCGIFBRDADDR");
      exit (ERROR);
    }

    sprintf(buf2, "%u.%u.%u.%u",
       (unsigned char)ifr->ifr_broadaddr.sa_data[2],
       (unsigned char)ifr->ifr_broadaddr.sa_data[3],
       (unsigned char)ifr->ifr_broadaddr.sa_data[4],
       (unsigned char)ifr->ifr_broadaddr.sa_data[5]);

    if (strcmp(buf2, "131.24.23.255") == 0)
      memcpy((char *)&server[port], (char *)&ifr->ifr_broadaddr,
           sizeof (ifr->ifr_broadaddr));
    }

}


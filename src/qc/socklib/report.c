#include  	<stdio.h>
#include  	<sys/ioctl.h>
#include	<sys/types.h>
#include	<netinet/in.h>
#include	<sys/socket.h>
#include	<sys/errno.h>
#include        <sys/utsname.h>
#include	<netdb.h>
#include 	<string.h>
#include 	<fcntl.h>

#include 	"sock.h"
#include 	"message.h"
#include  	"sleep.h"
#include 	"port_host.h"
#include 	"protos.h"

int
sendMessage(int sock, Message *msg)
/* writes a message (structure defined in message.h) specified by the
 * pointer msg, to the file descriptor specified in parameter 1. 
 * Returns a value of one if the entire message was written, a 
 * zero if the write was blocked.
 */
{
	if (write(sock,(char *) msg,sizeof(Message)) < sizeof(Message))
		return(0);
	else
		return(1); 
}


int
initConnect(int *messageSkt,int *sleepSkt, int *error_init)
/* Initializes the connection to the Graphical User Interface. Opens a
 *	socket to serve as a rendezvous socket, and then accept
 * 	connections from the Graphical User Interface. A file desciptor
 * 	corresponding to a message socket is returned in the form of a
 *	reference parameter. Error messages are subsequently written
 *	to this socket descriptor(messageSkt). 
 *	In addition, a file descriptor to enable communication from the 
 *	Graphical User Interface to the validation process is also 
 *	returned through use of a reference parameter (sleepSkt).
 *
 *	Note that until calls to fcntl() at the end of this routine,
 *	the calls accept, listen ... are not non-blocking calls. Thus
 * 	these sockets are only non-blocking in nature once this
 *	routine has finished. Only the actions of reading and writing
 * 	the socket are non-blocking.
 */
{
	struct hostent	*gethostbyname();
	int guiLen;
	int opt = 1;
	struct sockaddr *gui;


	char winputHostName[HOST_NAME_LEN];
	int port;
	struct sockaddr_in listener;
	struct hostent	*winput;
	int connectSkt;
	struct utsname utsstruct;

	uname(&utsstruct);

	*error_init = 0;   /* assume no error initially */
	
	strcpy(winputHostName,utsstruct.nodename);  
	port = CONNECT_PORT;	/* set port number of listener */

	if ( (connectSkt = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
	    fprintf(stderr,"QC ERROR: Socket call \n");
	    fprintf(stderr,"QC ERROR: Rendezvous socket not established\n");
	    *error_init = 1;
	}

        /* Allow previously used socket addresses to be reused. */
        if (setsockopt(connectSkt, SOL_SOCKET, SO_REUSEADDR, (char*)&opt,
                                                sizeof(int)) == (-1))
        {
            *error_init = 1;
            fprintf(stderr,"QC ERROR: Error in setsockopt() call (rendezvous socket) \n");
        }


	/* prepare for winput to listen for connection request */	
	winput = gethostbyname(winputHostName);
	memset((char*)&listener, 0, sizeof(listener));
	listener.sin_family = winput->h_addrtype;
	listener.sin_port = htons(port);	
	memcpy((char*) &listener.sin_addr, winput->h_addr, winput->h_length);

	/* Advertise that winput is listening  for GUI connection */
	if (bind(connectSkt, (struct sockaddr*)&listener, sizeof(listener)))
	{
	    fprintf(stderr,"QC ERROR: Failure in binding rendezvous socket \n");
	    *error_init = 1;
	}

	/* Wait for a connection request  */
	if ( listen(connectSkt, BACKLOG) )
	{
	    fprintf(stderr,"QC ERROR:  Listen call unsuccessful\n");
	    *error_init = 1;
	}


	guiLen = sizeof(gui);
	if ( (*messageSkt = accept(connectSkt, (struct sockaddr*) &gui, &guiLen)) < 0) 
	{
	    fprintf(stderr,"QC ERROR: Error in Accept call\n");
	    fprintf(stderr,"QC ERROR: Message socket conection not accepted\n");
	    *error_init = 1;
	}

	if ( (*sleepSkt = accept(connectSkt, (struct sockaddr*) &gui, &guiLen)) < 0)
	{
	    fprintf(stderr,"QC ERROR: Error in Accept call\n");
	    fprintf(stderr,"QC ERROR: Sleep message socket connect  not accepted\n");
	    *error_init = 1;
	}

	/* make sleep socket non-blocking */
	if (fcntl(*sleepSkt,F_SETFL,O_NDELAY) < 0)
	{
	    fprintf(stderr,"QC ERROR: Error in FCNTL call \n"); 
	    fprintf(stderr,"QC ERROR: Sleep socket not made non-blocking\n"); 
	    *error_init = 1;
	}

	/* make message  socket non-blocking */
	if (fcntl(*messageSkt,F_SETFL,O_NDELAY) < 0)
	{
	    fprintf(stderr,"QC ERROR: Error in FCNTL call \n"); 
	    fprintf(stderr,"QC ERROR: Message socket not made non-blocking\n");
	    *error_init = 1;
	}

	/* Close the rendezvous socket */
	if ( (close((int)connectSkt)) == 0 )
		return(1);
	else
	{
		fprintf(stderr,"QC ERROR: Error in closing Rendezvous socket\n");
		*error_init = 1;
		return(0);
	}

}


void
closeSkt(int fd) 
/* Close a socket descriptor specified by parameter fd. */
{
	if ( close(fd) != 0 )
		fprintf(stderr,"QC ERROR: Error in closing Message/Sleep socket \n");
}


Sleep_Message *getSleepMessage(int fd)
/* Returns a pointer to next sleep-message (as defined in sleep.h) if
 *	available, or a NULL pointer should no message be available.
 */
{	
	static Sleep_Message msg;

	/* if read is successful, return pointer to the sleep-message */
        if (read(fd,(char*)&msg,sizeof(Sleep_Message)) == sizeof(Sleep_Message))
                return(&msg);
	else 
		return(NULL);
}


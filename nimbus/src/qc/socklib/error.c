#include	<stdio.h>
#include        <string.h>
#include	<sys/types.h>
#include	<netinet/in.h>
#include	<sys/errno.h>
#include	<sys/ioctl.h>
#include	<sys/socket.h>
#include        <sys/utsname.h>
#include  	<fcntl.h>
#include	<sys/wait.h>
#include	<netdb.h>


#include 	"sock.h"
#include 	"message.h"
#include 	"port_host.h"
#include 	"sleep.h"
#include 	"protos.h"


int 	
unsleepVar(int skt,VarName v)
/*	Sends a message (of structure defined in sleep.h) to the
 *	winput process / validation  to discontinue the sleep 
 *	condition on a variable specified by the parameter v.
 */
{	static Sleep_Message s;

	s.mode = (Sleep_Mode) UNSLEEP;	
	strcpy(s.name, v);
	s.scope = (Sleep_Scope) SINGLE_VARIABLE; 	
	
        while (write(skt,(char*)&s,sizeof(Sleep_Message)) < sizeof(Sleep_Message));
	return(1);

}


int 	
sleepVar(int skt,VarName v)
/*	Sends a message (of structure defined in sleep.h) to the
 *	winput process / validation  to start the sleep 
 *	condition on a variable specified by the parameter v.
 */
{	
	static Sleep_Message s;

	s.mode = (Sleep_Mode) SLEEP;	
	strcpy(s.name, v);
	s.scope = (Sleep_Scope) SINGLE_VARIABLE; 	
	
        while (write(skt,(char*)&s,sizeof(Sleep_Message)) < sizeof(Sleep_Message));
	return(1);
}

Message
*getNextMessage(int fd) 
/*	Returns a pointer to the next sleep message if the message is
 * 	available at the time of the call. Returns a NULL pointer if
 *	a complete message is not available at the time of the call.
 * 	Socket descriptor described specified by the parameter fd is
 *	assumed to have been make non-blocking through use of the 
 *	fcntl call, or GUI will block until a complete message is 
 * 	available.
 */

{ 		
	static Message msg;

#ifdef SOCK_DIAGS
	fprintf(stderr,"QC: getNextMessage:  before call to read\n");
#endif

	if (read(fd,(char*)&msg,sizeof(Message)) == sizeof(Message)) 
	{
#ifdef SOCK_DIAGS
	    fprintf(stderr,"QC: getNextMessage: got msg [%s][%s][%s][%s][%d][%d]\n",
		   msg.vname, msg.type, msg.message, msg.Timestamp,
		   msg.Count_Consec, msg.Count_last2min);
#endif

	    return(&msg);
	}
	else  	
	{
#ifdef SOCK_DIAGS
	    fprintf(stderr,"QC: getNextMessage: no msg\n");
#endif
	    return((Message *) NULL);
	}
}	


int
messageInit(int *error_init)
/*	Initializes the connection from the GUI  to the Winput
 * 	process. Makes a connection call, and establishes a
 *	socket connection for receipt of error messages from the 
 *	validation library/Winput process.  
 *
 *	At the termination of this routine the error message socket
 *	is non-blocking.
 */
{
	struct hostent	*gethostbyname();  	/* proto for gethostbyname() */
	int port;				/* winput connect port# */
	int opt = 1;
	int messageSkt;	

	char winputHostName[HOST_NAME_LEN];
	struct hostent	*winput;
	struct sockaddr_in listener;
	
        struct utsname utsstruct;

	*error_init = 0;   /* assume no error initially */


	uname(&utsstruct);


	strcpy(winputHostName,utsstruct.nodename);

	/* Set port to the port on which the winput process is 
	 * waiiting for a connection request.
	 */
	port = CONNECT_PORT;

	/* gui opens socket to connect with winput */
	if ( (messageSkt = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
	    *error_init = 1;
	    fprintf(stderr,"QC ERROR: Error in socket() call (message socket) \n");
	}

	/* Allow previously used socket addresses to be reused. */
	if (setsockopt(messageSkt, SOL_SOCKET, SO_REUSEADDR, (char*)&opt,
						sizeof(int)) == (-1))
	{
	    *error_init = 1;
	    fprintf(stderr,"QC ERROR: Error in setsockopt() call (message socket) \n");
	}


	/* Use gethostbyname to fill in winput host info */
	winput = gethostbyname(winputHostName);

	/* initialize listener */
	bzero((char*)&listener, sizeof(listener));
	listener.sin_family = winput->h_addrtype;
	listener.sin_port = htons(port);
	bcopy((char*)winput->h_addr, (char*)&listener.sin_addr, 
	      winput->h_length);

	/* Connect with Winput to establish a socket to receive error
	 * messages.
	 */
	if(connect(messageSkt, (struct sockaddr *) &listener, sizeof(listener)))
	{	
		fprintf(stderr,"QC ERROR: Message Socket unable to connect \n");
		fprintf(stderr,"QC ERROR: (Is winput running?)\n");
		*error_init = 1;
		return(0);
	}
	else 	
	{ 	
		/* Make the message socket non-blocking */
		if (fcntl(messageSkt,F_SETFL,O_NDELAY) < 0)
		{
		    fprintf(stderr,"QC ERROR: Message socket \n"); 
		    fprintf(stderr,"QC ERROR: Unable to make socket non-blocking\n"); 
		    *error_init = 1;
		}
		return(messageSkt);
	}

}

int
sleepInit(int *error_init)
/*	Creates a connection from the GUI to the Winput
 * 	process. Makes a connection call, and establishes a
 *	socket connection for the sending of sleep messages to the 
 *	validation library/Winput process.  
 *
 *	At the termination of this routine the sleep message socket
 *	is non-blocking.
 */
{
	struct hostent	*gethostbyname();  	/* proto for gethostbyname() */
	int port;				/* winput connect port# */
	int opt = 1;
	int sleepSkt;	

	char winputHostName[HOST_NAME_LEN];
	struct hostent	*winput;
	struct sockaddr_in listener;
	struct utsname utsstruct;

	uname(&utsstruct);

	*error_init = 0;   /* assume no error initially */


	/* Fill in required information to establish connection with
	 *  Winput. Set the port number to the rendezvous port as
	 *  specified in port_host.h .
	 */
	strcpy(winputHostName,utsstruct.nodename);
	port = CONNECT_PORT;

	/* gui opens socket to connect with winput */
	if ( (sleepSkt = socket(AF_INET, SOCK_STREAM, 0) ) == -1 )
	{
	    fprintf(stderr,"QC: ERROR: socket call (sleep socket) \n");
	    *error_init = 1;
	}

        /* Allow previously used socket addresses to be reused. */
        if (setsockopt(sleepSkt, SOL_SOCKET, SO_REUSEADDR, (char*)&opt,
                                                sizeof(int)) == (-1))
        {
            *error_init = 1;
            fprintf(stderr,"QC ERROR: Error in setsockopt() call (sleep socket) \n");
        }


	/* Use gethostbyname to fill in winput host info */
	winput = gethostbyname(winputHostName);

	/* initialize listener */
	bzero((char*)&listener, sizeof(listener));
	listener.sin_family = winput->h_addrtype;
	listener.sin_port = htons(port);
	bcopy((char*)winput->h_addr, (char*)&listener.sin_addr, winput->h_length);

	/* Connect to Winput to establish the connection for the
 	 * transfer of sleep messages to the validation library.
	 */
	if(connect(sleepSkt,(struct sockaddr *)&listener, sizeof(listener)))
	{	 
		fprintf(stderr,"QC ERROR: Sleep socket unable to connect \n");
		*error_init = 1;
		return(0);
	}
	else 	
	{	
		/* Make the sleep socket non-blocking */
		if (fcntl(sleepSkt,F_SETFL,O_NDELAY) < 0) 
		{
		    fprintf(stderr,"QC ERROR: Sleep message socket\n"); 
		    fprintf(stderr,"QC ERROR: Unable to make socket non-blocking\n"); 
		    *error_init = 1;
		}

		/* return the sleep socket file descriptor */
		return(sleepSkt);
	}

}


int
closeSleep(int fd) 
/* Close the sleep socket. */
{
	if ( close(fd) )
	{
		fprintf(stderr,"QC ERROR: Error in Sleep socket closing\n");
		return (0);
	}
	else
		return (1);
}

int
closeMessage(int fd) 
/* Close the message socket */
{
	if ( close(fd) )
	{	
		fprintf(stderr,"QC ERROR: Error in Message socket closing \n");
		return (0);
	}
	else
		return (1);
}

/*
/opt/gnu/bin/gcc windsprint.c -g -o windsprint -lsocket -lnsl
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netdb.h>

#define PJETSOCKET 		0
#define FIRST_AVAILABLE_PORT 	3324
#define IMAGEHEADERFORMAT "%s %d %s %d %d"
#define STATISTICS		"statisticsprint"
#define SOCKETMAXBYTES 1024
#define FALSE 0
#define TRUE 1
#define ERROR -1

static int sock;
static struct sockaddr_in server;

main(argc,argv)
int argc;
char **argv;
{
 MakeSocket(argv[1]);
 NotifyPrinter(argv[2]);
}

/**************************  NotifyPrinter()  *********************************/

NotifyPrinter(filename)
char *filename;
{
char buffer[256];

 (void)sprintf(buffer,"%s %s","textfile",filename);
 if (WriteToSocket(PJETSOCKET,buffer,strlen(buffer))==ERROR) {
  perror("NotifyPrinter image WriteToSocket");
  (void)fprintf(stderr,"Problem with %s\n",buffer);
 } else {
  (void)fprintf(stderr,"\nwindsprint printing file %s...\n\n",filename);
 }
}

/************************  WriteToSocket()  ********************************/
 
WriteToSocket(port,data,length)
int port;
char *data;
int length;
{
 sendto(sock,data,length+1,0,(struct sockaddr *)&server,sizeof server);
}

/*******************  MakeSocket()  **********************************/

MakeSocket(host)
char host[];
{
struct hostent *hp,*gethostbyname();

 sock=socket(AF_INET,SOCK_DGRAM,0);
 if (sock < 0) {
  return(FALSE);
 }
 server.sin_family = htons(AF_INET);
 hp=gethostbyname(host);
 if (hp==0) {
  return(FALSE);
 }
 memcpy ((char *)&server.sin_addr,(char *)hp->h_addr, hp->h_length);
 server.sin_port = htons(FIRST_AVAILABLE_PORT+PJETSOCKET);
 return(TRUE);
 fprintf(stderr,"\nwindsprint connecting to port server on %s...\n",host);
}

/*
ipc.c -- handle inter-process messages
*/
#include <sys/param.h>
#include <stdio.h>
#include <socket.h>
#include <constants.h>
#include <proto.h>


static int ResponseToNetRequest=FALSE;

/************************  SetListeningPost()  ********************************/

SetListeningPost(client,port,addr)
char *client;
int port,addr;
/*
attempt establishment of a listening socket; return result, or, if
problem is one that clients can't do anything about, exit with ERROR status.
*/
{
int retval;

retval=listen_on_socket(client,port,addr);
switch (retval) {
 case OK:
  return OK;
 case EBADBIND:
/*
socket is already bound. Allow client to deal with this.
*/
  return ERROR;
 default:
  (void)fprintf(stderr,"FATAL: problem (retval=%d) on attempt to bind socket\n",
   retval);
  return ERROR;
 }
}

/********************  SetupWindsListening()  ****************************/

SetupWindsListening(client) 
char *client;
{
/*
listen for requests from other winds processes
 
Note that the address argument (3rd arg in SetListeningPost()) must be unique
for each process running on same cpu. The scheme used here is simple, i.e. using
a multiple of the max # of socket ports defined (in $WINDS/include/socket.h)
and adding the display number of this process to it. Since this display number
value may be as large as the maximum # of sessions defined in the netconfig
file, this scheme may fail if there are more of these netconfig sessions than
there are of socket ports...i.e., there can be an overlap of address values and
ergo the error "address already in use" will occur. At present, that is unlikely,
since there are 13 ports defined and never more than 5 hosts on same WINDS
network. Beware, however, since these things have a way of changing.

If another set of sockets is defined that use this scheme, use NUM_PORTS * 2,
NUM_PORTS * 3 for the next one, etc. (This is where the overlap may occur:
past the end of one set into the start of another...)
*/

 return SetListeningPost(client,INTERWINDS_LISTEN+get_session_number(GetDisplayHost()),
  NUM_PORTS+get_session_number(GetDisplayHost()));
}

/********************  SetupWindsBroadcast()  ****************************/

SetupWindsBroadcast(client)
char *client;
{
char *pnum;
int session;

 pnum=(char *)strdup(get_proj_number());
 set_winds_sessions(client,pnum,GetComputeHost(),GetDisplayHost());
 (void)free(pnum);
 if (post_processing_mode()) {
 
  session=get_session_number(GetDisplayHost());
  talk_to_socket(client,INTERWINDS_TALK+session,NUM_PORTS+session,GetComputeHost());

 } else {

  for (session=0; session<get_num_winds_sessions(); session++) {

   talk_to_socket(client,INTERWINDS_TALK+session,NUM_PORTS+session,GetComputeHost());
  }
 }
}
 
/*****************************  CloseWindsBroadcast()  ***********************/

CloseWindsBroadcast()
{
int session;

 if (post_processing_mode()) {
 
  close_socket(INTERWINDS_TALK+get_session_number(GetDisplayHost()));

 } else {

  for (session=0; session<get_num_winds_sessions(); session++) {
   close_socket(INTERWINDS_TALK+session);
  }
 }
}

/*****************************  CloseWindsListening()  ***********************/

CloseWindsListening()
{
 close_socket(INTERWINDS_LISTEN+get_session_number(GetDisplayHost()));
}

/*************************  GetResponseToNetRequest()  ***********************/

GetResponseToNetRequest()
{
 return ResponseToNetRequest;
}

/*************************  SetResponseToNetRequest()  ***********************/

SetResponseToNetRequest(value)
int value;
{
 ResponseToNetRequest=value;
}


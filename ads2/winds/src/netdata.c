/*
-------------------------------------------------------------------------
OBJECT NAME:	netdata.c

FULL NAME:	Receive net data

ENTRY POINTS:	main()

STATIC FNS:	Initialize()
		ProcessNames()
		ProcessData()

DESCRIPTION:	Sample program to receive WINDS broadcast data over
		ethernet, currently prints a couple selected variables
		to stdout.

INPUT:		

OUTPUT:		

REFERENCES:	socket.c

REFERENCED BY:	none

COMPILE:	gcc -g -o netdata netdata.o socket.o

COPYRIGHT:	University Corporation for Atmospheric Research, 1995
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include <string.h>

#include "constants.h"
#include "socket.h"

#define MAX_VARS	1000


typedef struct
  {
  char	Name[NAMLEN];
  int	Rate;		/* Rate at which var is being brdcst	*/
  int	StartPos;	/* Starting position into data array	*/
  } VAR_INFO;


int		nVariables;
VAR_INFO	*Variable[MAX_VARS];

float		*data;

static void	Initialize();
static void	ProcessNames(char *msg, int nBytes);
static void	ProcessData(char *msg, int nBytes);


/* -------------------------------------------------------------------- */
main(int argc, char *argv[])
{
  int	nBytes;
  char	buff[10000];


  Initialize();
printf("%d\n", CALIB_NETDATA);

  /* Establish socket on which to wait for broadcast
  */
  if (listen_on_socket("NETDATA", CALIB_NETDATA, CALIB_NETDATA) != OK)
    return(ERR);


  /* Wait for names, and info.
  */
  while (ReceiveBroadcast(buff) != 'N')
    ;
printf("Process names\n");
  ProcessNames(buff, nBytes);


  /* Continually wait for data and process upon arrival.
  */
  while (TRUE)
    if (ReceiveBroadcast(buff) == 'S')
      ProcessData(buff, nBytes);


  return(0);

}	/* END MAIN */

/* -------------------------------------------------------------------- */
static void Initialize()
{
  nVariables = 0;

}	/* END INITIALIZE */

/* -------------------------------------------------------------------- */
static void ProcessNames(char *msg, int nBytes)
{
  int	i, nValues = 0;
  char	*p = msg;

  nVariables = atoi(p);
  p = strchr(p, '-')+1;
  p = strtok(p, ":");

  for (i = 0; i < nVariables; ++i)
    {
    Variable[i] = (VAR_INFO *)malloc(sizeof(VAR_INFO));

    /* Format of info is "Name Rate:Name Rate:..."
     */
    sscanf(p, "%s %d", Variable[i]->Name, &Variable[i]->Rate);

    Variable[i]->StartPos = nValues;
    nValues += Variable[i]->Rate;
    p = strtok(NULL, ":");
    }


  if ((data = (float *)malloc(nValues * sizeof(float))) == NULL)
    {
    fprintf(stderr, "netdata: out of memory.\n");
    exit(1);
    }

}	/* END PROCESSNAMES */

/* -------------------------------------------------------------------- */
static void ProcessData(char *msg, int nBytes)
{
  int	i;

  /* Do whatever you want in here.
  */

  for (i = 0; i < nVariables; ++i)
    {
    printf("%s=%f, ", Variable[i]->Name, data[Variable[i]->StartPos]);
    }

  printf("\n\n");

}	/* END PROCESSDATA */

/* -------------------------------------------------------------------- */
ReceiveBroadcast(char buffer[])
{
  int	nBytes, indx, nPckts, pos;
  char	*msg, c;

  pos = 0;

  do
    {
    nBytes = MAX_SOCKET_BUFF_LNGTH;
    msg = (char *)read_from_socket(CALIB_NETDATA, &nBytes, BLOCKING);

    if (strncmp(msg, "WINDS-", 6) == 0)
      {
      sscanf(msg, "WINDS-%c%02d%02d", &c, &indx, &nPckts);
      memcpy(&buffer[pos], &msg[11], nBytes - 11);
      pos += nBytes - 11;
      }
    else
      {
      indx = 0;
      nPckts = 1;
      }

    }
  while (indx < nPckts);

  return(c);

}	/* END RECEIVEBROADCAST */

/* END NETDATA.C */

/*
-------------------------------------------------------------------------
OBJECT NAME:	brdcst.c

FULL NAME:	Broadcast net data.

ENTRY POINTS:	ReadBroadcastVariables()
		EnableBroadcasting()
		DisableBroadcasting()
		BroadcastSyncData()

STATIC FNS:	SendBroadcast()
		BroadcastNames()

DESCRIPTION:	Functions & data associated with broadcasting data.

INPUT:		

OUTPUT:		

REFERENCES:	socket.c

REFERENCED BY:	winputops.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1995
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include "constants.h"
#include "network.h"
#include "socket.h"

#include "nimbus.h"
#include "decode.h"
#include "wproto.h"

#define SDI		0
#define RAW		1
#define DERIVED		2

#define MAX_VARS	512


typedef struct
	{
	int	table;
	int	rate;	/* 1 = low-rate, anything else = sample rate	*/

	union
		{
		SDITBL	*sp;
		RAWTBL	*rp;
		DERTBL	*dp;
		} u;
	} VAR_PTRS;


static int	Broadcasting = FALSE;
static int	nVariables = 0;
static VAR_PTRS	*Variable[MAX_VARS];
static char	*localBuffer = NULL;

static void SendBroadcast(char magicCookie[], char *p);
static void BroadcastNames();


/* -------------------------------------------------------------------- */
void EnableBroadcasting (void)
{
  fprintf(stderr, "winput: Broadcasting enabled.\n");
  Broadcasting = TRUE;

  if (nVariables == 0)
    DisableBroadcasting();
}

/* -------------------------------------------------------------------- */
void DisableBroadcasting (void)
{
  fprintf(stderr, "winput: Broadcasting disabled.\n");
  Broadcasting = FALSE;
}

/* -------------------------------------------------------------------- */
void ReadBroadcastVariables(void)
{
  int	i, index, arraySize;
  char	*Bcast[512], target[NAMELEN], rate[8];

  if (AccessProjectFile(BROADCASTVARS, "r") == FALSE)
    return;

  if (GetInputMode() != ADS_DATA && GetInputMode() != DISK_DATA)
    return;

  ReadTextFile(BROADCASTVARS, Bcast);

  arraySize = 0;

  for (i = 0; Bcast[i]; ++i)
    {
    sscanf(Bcast[i], "%s %s", target, rate);

    if (strcmp(rate, "LR") && strcmp(rate, "SR"))
      {
      printf("Invalid entry in BcastVars file, var=[%s], ignoring.\n", target);
      continue;
      }


    Variable[nVariables] = (VAR_PTRS *)GetMemory(sizeof(VAR_PTRS));


    if ((index = SearchTable((char **)sdi, nsdi, target)) != ERR)
      {
      Variable[nVariables]->table = SDI;
      Variable[nVariables]->u.sp = sdi[index];
      Variable[nVariables]->rate = sdi[index]->SampleRate;
      }
    else
    if ((index = SearchTable((char **)raw, nraw, target)) != ERR)
      {
      Variable[nVariables]->table = RAW;
      Variable[nVariables]->u.rp = raw[index];
      Variable[nVariables]->rate = raw[index]->SampleRate;
      }
    else
    if ((index = SearchTable((char **)derived, nderive, target)) != ERR)
      {
      Variable[nVariables]->table = DERIVED;
      Variable[nVariables]->u.dp = derived[index];
      Variable[nVariables]->rate = 1;
      }
    else
      {
      printf("ReadBroadcastVariables: non-existant variable %s specified, continuing.\n", target);
      free(Variable[nVariables]);
      continue;
      }

    if (strcmp(rate, "LR") == 0)
      Variable[nVariables]->rate = 1;

    arraySize += Variable[nVariables]->rate;

    if (++nVariables >= MAX_VARS)
      {
      printf("Max Broadcast variables reached, ignoring rest.\n");
      break;
      }
    }


  /* 13 is the length of 'printf("%e "..)'.
   */
  localBuffer = (char *)GetMemory(arraySize * 13 + 64);

  FreeTextFile(Bcast);

  if (nVariables > 0)
    EnableBroadcasting();

}	/* END READBROADCASTVARIABLES */

/* -------------------------------------------------------------------- */
int BroadcastSyncData (void)
{
  int	i, j;
  char	tmp[32];

  static int	rollingCounter = 0;
  static int	time_indx[3];

  if (!Broadcasting)
    return(OK);

  if (rollingCounter == 0)
    {
    time_indx[0] = raw[SearchTable((char **)raw, nraw, "HOUR")]->SRstart;
    time_indx[1] = raw[SearchTable((char **)raw, nraw, "MINUTE")]->SRstart;
    time_indx[2] = raw[SearchTable((char **)raw, nraw, "SECOND")]->SRstart;
    }

  /* Broadcast names once a minute for Synchronization purposes.
   */
  if (rollingCounter++ % 60 == 0)
    BroadcastNames();


  /* Time stamp.
   */
  sprintf(localBuffer, " %02d:%02d:%02d ", (int)SampledData[time_indx[0]],
  (int)SampledData[time_indx[1]], (int)SampledData[time_indx[2]]);


  for (i = 0; i < nVariables; ++i)
    {
    switch (Variable[i]->table)
      {
      case SDI:
        {
        SDITBL	*sp = Variable[i]->u.sp;

        if (Variable[i]->rate == 1)
          {
          sprintf(tmp, "%e ", AveragedData[sp->LRstart]);
          strcat(localBuffer, tmp);
          }
        else
          for (j = 0; j < sp->SampleRate; ++j)
            {
            sprintf(tmp, "%e ", SampledData[sp->SRstart + j]);
            strcat(localBuffer, tmp);
            }
        }
        break;

      case RAW:
        {
        RAWTBL	*rp = Variable[i]->u.rp;

        if (Variable[i]->rate == 1)
          {
          sprintf(tmp, "%e ", AveragedData[rp->LRstart]);
          strcat(localBuffer, tmp);
          }
        else
          for (j = 0; j < rp->SampleRate; ++j)
            {
            sprintf(tmp, "%e ", SampledData[rp->SRstart + j]);
            strcat(localBuffer, tmp);
            }
        }
        break;

      case DERIVED:
        {
        DERTBL	*dp = Variable[i]->u.dp;

        sprintf(tmp, "%e ", AveragedData[dp->LRstart]);
        strcat(localBuffer, tmp);
        }
        break;
      }
    }

  SendBroadcast("WINDS-S", localBuffer);

  return(OK);

}	/* END BROADCASTSYNCDATA */

/* -------------------------------------------------------------------- */
static void BroadcastNames (void)
{
  int	i;
  char	temp[32];

  if (!Broadcasting)
    return;

  sprintf(localBuffer, "%d-", nVariables);

  /* Broadcast names with the following format "name,rate:..."
   */
  for (i = 0; i < nVariables; ++i)
    {
    switch (Variable[i]->table)
      {
      case SDI:
        sprintf(temp, "%s %d", Variable[i]->u.sp->name, Variable[i]->rate);
        break;

      case RAW:
        sprintf(temp, "%s %d", Variable[i]->u.rp->name, Variable[i]->rate);
        break;

      case DERIVED:
        sprintf(temp, "%s %d", Variable[i]->u.dp->name, Variable[i]->rate);
        break;
      }

    if (i != nVariables-1)
      strcat(temp, ":");

    strcat(localBuffer, temp);
    }

  SendBroadcast("WINDS-N", localBuffer);

}	/* END BROADCASTNAMES */

/* -------------------------------------------------------------------- */
static void SendBroadcast(char magicCookie[], char *p)
{
  int	i, hdrLength, dataLength, nPckts, nBytes, remainder;
  char	tmp[MAX_SOCKET_BUFF_LNGTH];

  /* Break apart message into segments, with each segment containing the
   * Magic Cookie, i/nPckts, and as much of the message as possible.
   */

  hdrLength = strlen(magicCookie)+4;
  dataLength = MAX_SOCKET_BUFF_LNGTH - (strlen(magicCookie)+4);

  nPckts = (strlen(p)+1) / dataLength;
  remainder = (strlen(p)+1) - (nPckts * dataLength);

  if (remainder)
    ++nPckts;

  strcpy(tmp, magicCookie);

  for (i = 0; i < nPckts; ++i)
    {
    nBytes = (i == nPckts-1) ? remainder : dataLength;

    sprintf(&tmp[hdrLength-4], "%02d%02d", i+1, nPckts);
    memcpy(&tmp[hdrLength], &p[i * dataLength], dataLength);

    if (write_to_socket(CALIB_NETDATA, tmp, nBytes + hdrLength) == EBADWRITE)
      fprintf(stderr, "SendBroadcast: send failure.\n");
    }

}	/* END SENDBROADCAST */

/* END BRDCST.C */

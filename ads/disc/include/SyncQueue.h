/*
-------------------------------------------------------------------------
OBJECT NAME:	SyncQueue.h

FULL NAME:	Header file for Queue
-------------------------------------------------------------------------
*/

#ifndef SYNCQUEUE_H
#define SYNCQUEUE_H

#ifndef NULL
#define NULL	((void *)0)
#endif

#include "DsmNets.h"
#include "Queue.h"

class SyncQueue {
public:
	SyncQueue(DsmNets *dsmNets,
		void (*netMsg)(int action, char* name, char* msg_str));

  bool	EnterNewData(DsmNet *curNet);
  bool	RetrieveNextRecord(DsmNet *curNet);
  void	CheckQuLengths();


private:
  ulong		HdrBlkToSeconds(const short b[])
	{ return(ntohs(b[3]) + (ntohs(b[2]) * 60) + (ntohs(b[1]) * 3600)); }

  int		nDSMs;
  DsmNets	*nets;
  Queue		dsmQ[6];

  ulong		prevTime, minTime;

  void (*netMessage)(int, char*, char*); // message handler function

};

#endif

/* SyncQueue.cc
   Intercepts data from DSM's and Q's it up.  Then releases it after all
   data for a given second has arrived.

   Original Author: Christopher J. Webster
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <SyncQueue.h>
  
/* -------------------------------------------------------------------- */
SyncQueue::SyncQueue(DsmNets *dsmNets,
		void (*netMsg)(int action, char* name, char* msg_str))
{
  int		i;
  DsmNet	*net;

  netMessage = netMsg;          // message handler function pointer
  nets = dsmNets;
  prevTime = 0;

  for (	nDSMs = 0, net = nets->firstNet();
	net->isDSM();
	++nDSMs, net = nets->nextNet())
    ;

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
bool SyncQueue::EnterNewData(DsmNet *curNet)
{
  void		*qp;
  const char	*buf = curNet->dsm_buf->getBuf();
  int		len = curNet->dsm_buf->bufIndex();
  int		index, i;
  ulong		thisTime;
  DsmNet	*net;


  // find index for curNet.  And flush any disconnected nets.
  for (	i = 0, net = nets->firstNet();
	net && net->isDSM();
	++i, net = nets->nextNet())
    {
    if (net == curNet)
      index = i;

    if (!net->connected())
      while (dsmQ[i].Front())
        dsmQ[i].DeQueue();
    }


  // Queue up the data.
  if ( !(int)(qp = malloc(len+2)) ) {
    perror("SyncQueue:malloc");
    exit(ERROR);
    }
//printf("%d queued\n", HdrBlkToSeconds((short *)buf));

  *(ushort *)qp = len;
  memcpy((void *)&((char *)qp)[2], (void *)buf, len);
  dsmQ[index].EnQueue(qp);
  curNet->dsm_buf->releaseBuf();

  // Wait until all connected nets have something in the Queue.
  for (i = 0, net = nets->firstNet(); i < nDSMs; ++i, net = nets->nextNet())
    if (net->connected() && dsmQ[i].Front() == NULL)
      return(false);


//printf("All nets full\n");

  /* Now search Queue's for earliest time, and return it.
   */
  minTime = 100000;	// Set to some large value;

  for (i = 0, net = nets->firstNet(); i < nDSMs; ++i, net = nets->nextNet())
    {
    if ((qp = dsmQ[i].Front()) == NULL)
      continue;

    thisTime = HdrBlkToSeconds(&((short *)qp)[1]);

    if (prevTime > 86300 && thisTime < 100)  // handle mid-nite x-over
      thisTime += 86400;

    if (thisTime < minTime)
      minTime = thisTime;
    }

//printf("%d released\n", minTime);

  if (minTime >= 86400)
    minTime -= 86400;

  return(true);	// data is ready to be dequeued.

}	/* END ENTERNEWDATA */

/* -------------------------------------------------------------------- */
void SyncQueue::CheckQuLengths()
{
  int		i, cnt;
  DsmNet	*net;

  for (i = 0, net = nets->firstNet(); i < nDSMs; ++i, net = nets->nextNet())
    {
    if ((cnt = dsmQ[i].Length()) > 5)
      {
      int	j;
      char	buff[80], temp[12];
      Hdr_blk	*p;

      sprintf(buff, "%s queue length is %d\n", net->netName(), cnt);
      netMessage(NET_STATUS, "CTL", buff);

      strcpy(buff, "  ");

      for (j = 0; j < nDSMs; ++j)
        {
        p = (Hdr_blk *)dsmQ[j].Front();

        if (p)
          sprintf(temp, "  %02d:%02d:%02d", ntohs(p->minute), 
                  ntohs(p->second), ntohs(p->year));
        else
          strcpy(temp, "  --:--:--");

        strcat(buff, temp);
        }

      strcat(buff, "\n");
      netMessage(NET_STATUS, "CTL", buff);
      }
    }

}	/* END CHECKQULENGTH */

/* -------------------------------------------------------------------- */
bool SyncQueue::RetrieveNextRecord(DsmNet *curNet)
{
  int		i, len;
  void		*qp;
  ulong		thisTime;

  for (i = 0; i < nDSMs; ++i)
    {
    if ((qp = dsmQ[i].Front()) == NULL)
      continue;

    if ((thisTime = HdrBlkToSeconds(&((short *)qp)[1])) == minTime)
      {
      qp = dsmQ[i].DeQueue();
      len = *(ushort *)qp;
      curNet->dsm_buf->copyToBuf(&((char *)qp)[2], len);
      prevTime = minTime;
      free(qp);

      return(true);
      }
//printf("%d %d\n", thisTime, minTime);
    }

  return(false);	// No more data for this second.

}	/* END RETREIVENEXTRECORD */

/* END SYNCQUEUE.CC */

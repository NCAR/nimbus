/* DsmCollect.cc
   Collects sampled data and builds logical and physical records.

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research

   Revisions:


*/

#include <DsmCollect.h>

/******************************************************************************
** Public Functions
******************************************************************************/

DsmCollect::DsmCollect (DsmTables &dt, DsmHeaders &dh, 
                        void (*netMsg)(int action, char* name, char* msg_str)):
                        dsm_tbls (dt), DsmConfig ()
                        
// Constructor.
{
  netMessage = netMsg;			// message handler

// Create the physical record buffers.
  smp_buf[0] = new RandomBuf (MX_PHYS);		// physical records
  smp_buf[1] = new RandomBuf (MX_PHYS);		// physical records

// Get the logical and physical record lengths.
  lr_len = dh.mainHeader()->lrlen();
  pr_len = dh.mainHeader()->lrppr() * lr_len;

  initialize ();
}
/*****************************************************************************/

void DsmCollect::initialize ()

// Initialize buffers, flags, and indices.
{
// Flush buffers.
  smp_buf[0]->releaseBuf();
  smp_buf[1]->releaseBuf();

// Clear the connected and received flags.
  clearReceived ();
  clearConnect ();
 
  ptog = 0;
  gtog = 0;
  lr_idx = 0;
  last_lr_idx = 0;
  cur_hour = 0;
  cur_min = 0;
  cur_sec = -1;				// current second < 0 for startup
}
/*****************************************************************************/

int DsmCollect::buildLogical (const char *const buf)

// Builds logical records.
{
  PickTable *pick;			// pickoff table for this dsm
  PickEntry *entry;			// pickoff table entry
  int rate_cnt;				// rate counter
  int sidx;				// source index
  int didx;				// destination index
  int status;				// record complete status
  int j;

// Select the config entry for this location.
  selectByLocn (((Hdr_blk*)buf)->dsm_locn);

// Verify the data.
  if (ntohs(*(unsigned short*)buf) != (unsigned short)SAMPLED_ID) {
    sprintf (msg_str, "Misaligned sampled data block from %s at location %s.\n",
             hostName(), location());
    netMessage (NET_STATUS, location(), msg_str);
    return ERROR;
  }

// Check for a new second started. 
  switch (status = checkTime(	ntohs(((Hdr_blk *)buf)->hour),
				ntohs(((Hdr_blk *)buf)->minute),
				ntohs(((Hdr_blk *)buf)->second))) {
    case DSM_STARTUP:
    case DSM_SAME_SECOND:		// same as the current second
      break;

    case DSM_NEW_SECOND:		// new second
      checkReceived ();			// check which DSMs have not responded
      clearReceived ();			// clear data received flags
      status |= incrLrIndex ();       	// increment the LR index
      break;

// If the time gap is from the main timing dsm, then adjust the time.
    case DSM_TIME_GAP:	
      if (timeMaster()) {
        sprintf (msg_str, 
          "A time gap has occurred from the time master %s at location %s.\n",
                 hostName(), location());
        netMessage (NET_STATUS, location(), msg_str);

        sprintf (msg_str, 
                 "Last time = %02d:%02d:%02d. New time = %02d:%02d:%02d.\n",
                 cur_hour, cur_min, cur_sec, ntohs(((Hdr_blk*)buf)->hour),
                 ntohs(((Hdr_blk*)buf)->minute),ntohs(((Hdr_blk*)buf)->second));

        netMessage (NET_STATUS, location(), msg_str);

        cur_hour = ntohs(((Hdr_blk*)buf)->hour);
        cur_min = ntohs(((Hdr_blk*)buf)->minute);
        cur_sec = ntohs(((Hdr_blk*)buf)->second);

        checkReceived ();		// check which DSMs have not responded
        clearReceived ();		// clear data received flags
        status |= incrLrIndex();       // increment the LR index
        break;
      }

// For all other dsms, throw out the data if a time gap occurs.
      else {
        sprintf(msg_str, "Out of sequence time stamp from %s at location %s.\n",
                 hostName(), location());
        netMessage(NET_STATUS, location(), msg_str);

        sprintf(msg_str, 
          "Current time = %02d:%02d:%02d. Bad DSM time = %02d:%02d:%02d.\n",
		cur_hour, cur_min, cur_sec, ntohs(((Hdr_blk*)buf)->hour),
		ntohs(((Hdr_blk*)buf)->minute),ntohs(((Hdr_blk*)buf)->second));

        netMessage(NET_STATUS, location(), msg_str);

//        return DSM_LR_INCOMPLETE;

        status |= incrLrIndex();       // increment the LR index
        break;		// Record the data anyways.
      }
    default:;
  }


// Select the pickTable for this dsm.
  if (!(int)(pick = dsm_tbls.selectTable (((Hdr_blk*)buf)->dsm_locn))) {
    sprintf (msg_str, "No PickTable for %s location %s.\n",
             hostName(), location());
    netMessage (NET_STATUS, location(), msg_str);
    exit (ERROR);
  }

// Move the data from the buffer into the logical record.
  for (entry = pick->firstEntry(); (int)entry; entry = pick->nextEntry()) {
    for (rate_cnt = 0, sidx = entry->source, didx = entry->dest + lr_idx; 
         rate_cnt < entry->rate; 
         rate_cnt++, sidx += entry->src_off, didx += entry->dest_off) {
      if (entry->rate != 5000)
        smp_buf[ptog]->putBuf (buf + sidx, didx, entry->len);

/**
      if (!rate_cnt)
        printf ("sidx = %d, didx = %d, len = %d\n", sidx, didx, entry->len);
**/
    }

/**
// code to display 1d data.
    if (!strcmp (entry->name, "PMS1V2")) {
printf("------------------------\n");
      for (j = 0; j < 16; j++) 
        printf ("%04d ", ((P16v2_blk*)(buf + entry->source))->bins[j]);
      putchar ('\n');
    }
**/
  }


  setReceived (((Hdr_blk*)buf)->dsm_locn);	// set data received flag

// If the current block is from the time master, set the DSM_TIME_MASTER status
// bit.
  selectByLocn (((Hdr_blk*)buf)->dsm_locn);
  if (timeMaster()) 
    status |= DSM_TIME_MASTER;

  return(status);

}

/******************************************************************************
** Private Functions
******************************************************************************/

int DsmCollect::setReceived (char *locn)

// Sets the data received flag for this dsm location, and determines if the
// logical record is full.
{
  int j;
  int lr_full = TRUE;

// Select the dsm and set the received flag.
  selectByLocn (locn);
  j = index();
  received[j] = TRUE;

// If data was not previously being received from this DSM, generate a message
// and set the connected flag.
  if (!connected[j]) {
    connected[j] = TRUE;
    sprintf (msg_str, "Data now being received from %s at location %s.\n",
             hostName(), location());
    netMessage(NET_STATUS, location(), msg_str);
  }

// Check for a full logical record.
  for (j = firstDsm(); lr_full && j; j = nextDsm()) {
    if (connected[index()] && !received[index()])
      lr_full = FALSE;
  }

  return(lr_full);

}

/*****************************************************************************/
void DsmCollect::checkReceived ()
 
// Checks for data received from all currently connected DSMs.
{
  int stat;

  for (stat = firstDsm(); stat; stat = nextDsm()) {
    if (connected[index()] && !received[index()]) {
      sprintf (msg_str, "Data not being received from %s at location %s.\n",
               hostName(), location());
      netMessage(NET_STATUS, location(), msg_str);
      connected[index()] = FALSE;
    }
  }
}

/*****************************************************************************/
int DsmCollect::checkTime (int hour, int minute, int second)
 
// Compares the passed in time with the current data block time.
{
  int thour;
  int tmin;
  int tsec;

// First check for the same time.
  if ((hour == cur_hour) && (minute == cur_min) && (second == cur_sec))
    return DSM_SAME_SECOND;

// Check for the next second time.
  if (!(tsec = (cur_sec + 1) % 60)) {	// compute next second time
    if (!(tmin = (cur_min + 1) % 60))
      thour = (cur_hour + 1) % 24;
    else
      thour = cur_hour;
  }
  else {
    thour = cur_hour;
    tmin = cur_min;
  }

  if ((hour == thour) && (minute == tmin) && (second == tsec)) {
    cur_hour = hour;
    cur_min = minute;
    cur_sec = second;
    return DSM_NEW_SECOND;
  }

// Check for the startup condition.
  if (cur_sec < 0) {			// startup condition
    cur_hour = hour;
    cur_min = minute;
    cur_sec = second; 
    return DSM_STARTUP;
  }

// Otherwise a time gap has occurred.
  return DSM_TIME_GAP;
}
/*****************************************************************************/
 
int DsmCollect::incrLrIndex ()
 
// Increment the logical record index. Check for a full physical record.
{
  int len;

  last_lr_idx = lr_idx;

  if ((lr_idx += lr_len) >= pr_len) {
    smp_buf[ptog]->setLen (pr_len);		// set full buffer length
    lr_idx = 0;
    ptog = ++ptog % TOG;			// toggle buffers
    if (len = smp_buf[ptog]->getLen()) {
      sprintf (msg_str, 
               "DsmCollect: Buffer not released, data will be overwritten.\n");
      netMessage (NET_STATUS, location(), msg_str);
    }

//  printf ("ptog = %d, lr_idx = %d, len = %d\n", ptog, lr_idx, len); 
    return DSM_PR_COMPLETE | DSM_LR_COMPLETE;
  }
  return DSM_LR_COMPLETE;
}
/*****************************************************************************/


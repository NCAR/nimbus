/* Rdma.cc
 
   RDMA  class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <Rdma.h>

static short rdma_data_flag;

/******************************************************************************
** Public Functions
******************************************************************************/

Rdma::Rdma (const char *base, const int port, 
                void (*statMsg)(char* msg)) :
                IsioPort (base, port, RDMA_BAUD, RDMA_BITS, RDMA_PARITY,
                RDMA_STOP, RDMA_FLOW, statMsg), statusMsg (statMsg)

// Constructor. The interrupt routine must be previously set up outside this 
// class.
{
  ptog = 0;
  gtog = 0;
  idx = 0;
  rdma_data_flag = FALSE;
  fflush(stdout);
  memset (hk_buf, 0, sizeof (hk_buf));
  memset (cnt_buf, 0, sizeof (cnt_buf));
  term_str[0] = '\r';
  term_str[1] = '\n';
  term_str[2] = '\0';
//  igetstr (term_str);			// issue first read
}
/*****************************************************************************/
void Rdma::initRdma()
{
  igetstr (term_str);			// issue first read

}

/*****************************************************************************/
void Rdma::checkStatus()

// Check the serial port status.
{
  if (checkPort() != TRUE) {
    igetstr (term_str);         // new read if an error or dropped interrupt
    statusMsg ("Rdma: Dropped isio interrupt.\n");
  } 
}

/*****************************************************************************/
void Rdma::secondAlign()
 
// Called at the start of a second to align the indices and toggle buffers.
{
  int i;

  gtog = ptog;                          // toggle the get buffer index
  ptog = 1 - gtog;
//  idx = 0;
/*
  for (i = 0; i < SCAN_ARRAY_SIZE; i++) {
    rdma_blk[ptog].scan[i] = 0.0;
  }
*/
//  memset (rdma_blk[ptog].scan, 0, sizeof (rdma_blk[ptog].scan));
 
}

/*****************************************************************************/
void Rdma::isr()
 
// Pulls in data from the isio and builds the Rdma data struct.
{
  char *datap, *dest;

// Check for data from the isio port.
  if (!(int)(datap = getData()))
    return;
 
  if (*datap != 'c')  // then it's housekeeping
    {
    dataAvailFlag[HK][ptog] = true;
    dest = hk_buf[ptog];
    }
  else
    {
    dataAvailFlag[CNT][ptog] = true;
    dest = cnt_buf[ptog];
//logMsg("Rdma: recvd cnt\n", 0,0,0,0,0,0);
    }

  for (; *dest = *datap; ++dest, ++datap)
    *datap = '\0';

  igetstr(term_str);                        // issue next read

}

/*****************************************************************************/
const char *Rdma::buffer()
{
  strcpy(rdma_blk.item_type, "hk");

  if (dataAvailFlag[HK][gtog]) 
    {
    dataAvailFlag[HK][gtog] = false;
    sscanf(hk_buf[gtog], "%f %f %f %f %f %f %f %f %f %f",
	&rdma_blk.vd, &rdma_blk.vsh, &rdma_blk.qa, &rdma_blk.qsh,
	&rdma_blk.qs, &rdma_blk.qex, &rdma_blk.pabs, &rdma_blk.rh,
	&rdma_blk.temp, &rdma_blk.hvps);
    }


  if (dataAvailFlag[CNT][gtog]) 
    {
    char	*p;
    int		idx;

    dataAvailFlag[CNT][gtog] = false;
    p = strtok(cnt_buf[gtog], " ");
    strcpy(rdma_blk.item_type, p);

    for (idx = 0; (p = strtok(NULL, " \n")); rdma_blk.scan[idx++] = atof(p))
      ;

    rdma_blk.scan[idx] = -99.0;
    }
  else
    memset((char *)rdma_blk.scan, 0, sizeof(rdma_blk.scan));

  return((char *)&rdma_blk);

}

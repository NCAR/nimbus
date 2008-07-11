/* UvHyg.h
 
   UV Hygrometer class.
 
   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/
 
#include <UvHyg.h>

/******************************************************************************
** Public Functions
******************************************************************************/

UvHyg::UvHyg (const char *base, const int port,
              void (*statMsg)(char* msg)) :
              IsioPort (base, port, UVH_BAUD, UVH_BITS, UVH_PARITY,
              UVH_STOP, UVH_FLOW, statMsg), statusMsg (statMsg)

// Constructor. The interrupt routine must be previously set up outside this
// class.
{
  align_cnt = 0;			// misaligned data byte count
  ptog = 0;
  gtog = 0;
  memset ((char*)uv_blk, 0, sizeof(uv_blk));
  igetcnt (UVH_SMP_LEN);				// issue first read
}

/*****************************************************************************/
void UvHyg::checkStatus()

// Checks the status of the UV hygrometer isio port.
{
  if (checkPort() != TRUE) {
    igetcnt (UVH_SMP_LEN);			// issue new read
    statusMsg ("UvHyg: Dropped isio interrupt.\n");
  }
}

/*****************************************************************************/
void UvHyg::secondAlign ()

// This routine is to be called at each 1 second clock tick. The Uv_blk
// buffers are toggled.
{
  int ttog = 1 - ptog;

  uv_blk[ttog].firstSample();
  gtog = ptog;
  ptog = ttog;
  full = FALSE;
}

/*****************************************************************************/
void UvHyg::isr()

// Pulls in the UV hygrometer sample block, checks for data alignment, issues a
// new read command.
{
  int	j;
  char	*datap;
  char	tbuf[UVH_SMP_LEN+2];

// Check for data from the isio port.
  if (!(int)(datap = getData()))
    return;
 
// If this interrupt is from a previous alignment read, then ignore the data
// and issue another read for a full data sample.  The data should be
// aligned on the completion of the next read.
  if (align_cnt) {
    igetcnt (UVH_SMP_LEN);        		// issue next read 
    align_cnt = FALSE;
    return;
  }

// Check the data alignment, and pull in the data if the buffer is not 
// already full for this second.
  if (*(short*)datap == UVH_ALIGN_FLAG) { 
    if (!full) {
      uv_blk[ptog].setSample ((Uv_smp*)(datap + sizeof(UVH_ALIGN_FLAG)));
//    logMsg ("t %1d, i = %2d.\n", ptog, uv_blk[ptog].index(), 0,0,0,0);
    }
    if (!uv_blk[ptog].nextSample())
      full = TRUE;
    igetcnt (UVH_SMP_LEN); 	// issue next read
  }

// If the data is misaligned, parse until the alignment flag is found, then
// issue a read for the remainder of the misaligned block.
  else {
    memcpy (tbuf, datap, UVH_SMP_LEN); 

    for (align_cnt = 1; align_cnt < UVH_SMP_LEN; align_cnt++) {
       if ((tbuf[align_cnt] == UVH_ALIGN_CHAR) && 
           (tbuf[align_cnt + 1] == UVH_ALIGN_CHAR))
         break;
    } 
    igetcnt (align_cnt); 		// issue alignment read

    for (j = 0; j < UVH_SMP_LEN; j++)
      sprintf (&msg[j*2], "%2X", (unsigned int)tbuf[j]);
    sprintf (&msg[j*2], "\n");
    logMsg ("UvHyg: Misaligned data, %s", (int)msg, 0,0,0,0,0);
  }
}

/* Pms2dHouse.cc

   Pms-2D HouseKeeping interface.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 

*/

#include <Pms2dHouse.h>

/******************************************************************************
** Public Functions
******************************************************************************/

Pms2dHouse::Pms2dHouse (char *intf_base) 

// Constructor.
{
  int j; 

  gtog = 1;
  ptog = 0;

// Set the register pointers.
  house[0] = (P2d_house *)(intf_base + PMS2D_HOUSE_BUF_0);
  house[1] = (P2d_house *)(intf_base + PMS2D_HOUSE_BUF_1);
  dpr_sem = (short*)(intf_base + PMS2D_DPR_SEM);

}
/*****************************************************************************/
 
void Pms2dHouse::secondAlign ()
 
// This routine is to be called at each 1 second clock tick. The 2D 
// buffers are toggled.
{
  gtog = ptog;
  ptog = 1 - ptog;
 
}
/*****************************************************************************/

void Pms2dHouse::getData (int chan)

// Reads housekeeping and shadowor data from one or two 2-D probes.
{
  P2dChan* rec;

// If this is the start of a new second, release the last second's buffers
// and toggle the buffers.

// Get 5 hz data.
//  if (hz_cnt % RATE_5)
//    return;

  rec = (P2dChan*)gbuffer(chan);

  getSem();
  memcpy ((void *)rec, (void *)house[chan], sizeof(P2d_house));
  releaseSem();

/*
  logMsg("hdata[0] = %x hdata[1] = %x,hdata[2] = %x hdata[3] = %x\n",
         house[chan]->hdata[0], house[chan]->hdata[1],
         house[chan]->hdata[2], house[chan]->hdata[3],0,0);
  logMsg("hdata[4] = %x hdata[4] = %x,hdata[6] = %x hdata[7] = %x\n",
         house[chan]->hdata[4], house[chan]->hdata[5],
         house[chan]->hdata[6], house[chan]->hdata[7],0,0);
*/
}

/* GreyVmeHouse.cc

   Pms-Grey HouseKeeping interface.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 

*/

#include <GreyVmeHouse.h>

/******************************************************************************
** Public Functions
******************************************************************************/

GreyVmeHouse::GreyVmeHouse (char *intf_base) 

// Constructor.
{
  int j; 

  gtog = 0;
  ptog = 0;

// Set the register pointers.
  house[0] = (Grey_house *)(intf_base + GREY_HOUSE_BUF_0);
  house[1] = (Grey_house *)(intf_base + GREY_HOUSE_BUF_1);
  dpr_sem = (short*)(intf_base + GREY_DPR_SEM);

}
/*****************************************************************************/
 
void GreyVmeHouse::secondAlign ()
 
// This routine is to be called at each 1 second clock tick. The Grey 
// buffers are toggled.
{
  int ttog;
 
  ttog = (ptog + 1) % TOG;
  idx=0;
  gtog = ptog;
  ptog = ttog;
}
/*****************************************************************************/

void GreyVmeHouse::getData (int chan)

// Reads housekeeping data from one or two Grey probes.
{
  GreyChan* rec;

// If this is the start of a new second, release the last second's buffers
// and toggle the buffers.

  getSem();

  rec = (GreyChan*)gbuffer(chan);
  memcpy ((char *)rec, (char *)house[chan], sizeof (Grey_house));
//  logMsg("hdata[0] = %x hdata[1] = %x\n",house[chan]->hdata[0],
//         house[chan]->hdata[1],0,0,0,0);
  releaseSem(); 
}
/*****************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

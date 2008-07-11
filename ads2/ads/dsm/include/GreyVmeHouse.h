/* GreyVmeHouse.h
 
   PMS Grey VME housekeeping interface.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#ifndef GREYVMEHOUSE_H
#define GREYVMEHOUSE_H

#include <vxWorks.h>
#include <sysLib.h>
#include <taskLib.h>
#include <logLib.h>
#include <string.h>

#include <header.h>
#include <dsmctl.h>
#include <messageDefs.h>

#define GREY_HOUSE_VAR		2	// no. housekeeping variables
#define GREY_HOUSE_BUF_0        0x3FC0
#define GREY_HOUSE_BUF_1        0x3FE0
#define GREY_DPR_SEM            0x4000  // dp ram semaphore


typedef struct {
  Grey_house hbuf[TOG];
} GreyChan;

class GreyVmeHouse {
public:
  GreyVmeHouse (char *intf_base);
  void	secondAlign();
  void	getData(int chan);
  inline char	*gbuffer(int chan) {return (char*)&grey_chan[chan].hbuf[gtog];}
  inline char	*pbuffer(int chan) {return (char*)&grey_chan[chan].hbuf[ptog];}
  GreyChan grey_chan[GREY_NUM_PROBES]; 
  Grey_house	*house[GREY_NUM_PROBES];

private:
  inline void getSem()     {for (*dpr_sem = 0; *dpr_sem & 0x01; *dpr_sem = 0);}
  inline void releaseSem()           {*dpr_sem = 1;}

  int ptog;                             // put buffer index
  int gtog;                             // get buffer index

  short	*dpr_sem;                       // dual-port ram semaphore
  int	idx;
};

#endif

/* Pms2dHouse.h
 
   PMS 2-D VME housekeeping interface.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#ifndef PMSVME2DHOUSE_H
#define PMSVME2DHOUSE_H

#include <vxWorks.h>
#include <vxLib.h>
#include <sysLib.h>
#include <taskLib.h>
#include <logLib.h>
#include <string.h>

#include <SerialBuf.h>

#include <header.h>
#include <dsmctl.h>
#include <messageDefs.h>
#include <dsmVmeDefs.h>

#define HOUSE_VAR		8	// no. housekeeping variables
#define PMS2D_HOUSE_BUF_0       0x3200
#define PMS2D_HOUSE_BUF_1       0x3220
#define PMS2D_DPR_SEM           0x4006  // dp ram semaphore


typedef struct {
  P2d_house hbuf[TOG];
} P2dChan;

class Pms2dHouse {
public:
  Pms2dHouse (char *intf_base);
  void	secondAlign();
  void	getData(int chan);
  inline char	*gbuffer(int chan) {return (char*)&p2d_chan[chan].hbuf[gtog];}
  inline char	*pbuffer(int chan) {return (char*)&p2d_chan[chan].hbuf[ptog];}
  P2dChan p2d_chan[PMS2D_NUM_PROBES]; 
  P2d_house	*house[PMS2D_NUM_PROBES];

private:
  inline void getSem()    {for (*dpr_sem = 0; *dpr_sem & 0x01; *dpr_sem = 0);}
  inline void releaseSem()           {*dpr_sem = 1;}
  int probeInterface(char *);          // test for interface card.

  int ptog;                             // put buffer index
  int gtog;                             // get buffer index

  short	*dpr_sem;                       // dual-port ram semaphore
  int	idx;
  bool  interfaceInstalled;

};

#endif

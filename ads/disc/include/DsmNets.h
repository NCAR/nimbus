/* DsmNets.h
   Manages dsm network interfaces.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef DSMNETS_H
#define DSMNETS_H

#include <dsmctl.h>
 
#include <DsmConfig.h>
#include <DsmNet.h>
 
class DsmNets : public DsmConfig {
public:
  DsmNets (void (*netMsg)(int action, char* name, char* msg_str));
  DsmNet *firstNet();                      	// select first net
  DsmNet *nextNet();                       	// select next net
  DsmNet *selectNet(char *locn);           	// select net by dsm location
  inline DsmNet *curNet()	{return cur_net;}
  inline DsmNet *windsNet() 	{return winds_net;}
 
private:
  DsmNet *dsm_net[MAX_DSM];			// network classes
  DsmNet *cur_net;				// winds network class
  DsmNet *winds_net;				// winds network class
  int nats_mode;				// nats mode flag
};

#endif


/* Mcr.h

   Mcr control class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef MCR_H
#define MCR_H

#include <McrTable.h>
#include <Vmio12.h>
#include <Vmio14Out.h>
#include <DsmMessage.h>
#include <Bc635Vme.h>
#include <Dsp56002.h>
#include <dsmctl.h>
#include <messageDefs.h>
#include <AnalogTable.h>

// Define the output port control bits.
#define MCR_REMOTE_HEART_BIT	0x01		// 1 hz heartbeat to the mcr
#define MCR_PREAMP_ENABLE_BIT	0x02		// preamp enable 
#define MCR_DOOR_ENABLE_BIT	0x04		// door motor enable 
#define MCR_DOOR_SELECT_BIT	0x08		// open/close doors
#define MCR_HEATER_SELECT_BIT	0x10		// turn heaters on/off

#define MCR_INITIAL_STATUS	0xFFFF		// initial current status

struct Mcr_hdr {
  str8 id;                             /* Mcr channel no. */
  short hour;
  short minute;
  short second;
};
typedef struct Mcr_hdr Mcr_hdr;

class Mcr {
public:
  Mcr (McrTable& mcr_tbl, Vmio12 *vmio12[], Bc635Vme& tp, Dsp56002 *dp, 
       DsmMessage& dm, AnalogTable&);	// constructor
  void setStatus (int status);			// set current status
  void control (int action, int value);	
  void toggleHeartBeat();			// toggle heart beat bit
  void secondAlign ();
  int curOut ()		{return cur_out;}	// current ouput value
  AnalogTable &ana_tbl;                         // analog sample table

private: 
  void setRemote (int value);			// set/clear remote enable flag
  void setPreampEnable (int value);		// set/clear preamp enable bit
  void setDoorEnable (int value);		// set/clear door enable bit
  void setDoorSelect (int value);		// set/clear door select bit
  void setHeaterSelect (int value);		// set/clear heater select bit
  Vmio14Out *mcr_out;				// mcr output port class
  DsmMessage& dsm_msg;				// message handler class
  Bc635Vme &tfp;                                // time-freq processor intfc
  Dsp56002 *dsp;                                // M56002 dsp board interface
  Mcr_hdr mcr_hdr;				// Mcr header struct
  int cur_stat;					// current input status value
  int cur_out;					// current output port value
  int remote;					// remote enable flag
};

#endif


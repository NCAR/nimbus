/* DsmCollect.h
  Collects sampled data and builds logical and physical records.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef SMPCOLLECT_H
#define SMPCOLLECT_H

#include <dsmctl.h>
#include <header.h>
#include <portable.h>

#include <DsmConfig.h>
#include <DsmHeaders.h>
#include <DsmMessage.h>
#include <DsmTables.h>
#include <RandomBuf.h>

#define DSM_LR_INCOMPLETE	0x00	// logical record is incomplete
#define DSM_LR_COMPLETE		0x01	// logical record is complete
#define DSM_PR_COMPLETE		0x02	// physical record is complete

#define DSM_STARTUP		0x04	// first data block received
#define DSM_SAME_SECOND		0x08	// data matches current second
#define DSM_NEW_SECOND		0x10	// data is for a new second
#define DSM_TIME_MASTER		0x20	// current block is from time master
#define DSM_TIME_GAP		0x40	// time gap has occurred

class DsmCollect : private DsmConfig {
public:
  DsmCollect (DsmTables&, DsmHeaders&, 
              void(*netMsg)(int action, char* name, char* msg_str));
              
  void initialize();			// init buffers and variables
  int buildLogical (const char *const buf);	// build a logical record
  const char *const getLogical()  {return smp_buf[gtog]->getBuf()+ last_lr_idx;}
  const char *const getPhysical() {return smp_buf[gtog]->getBuf();}
  void releasePhysical(){smp_buf[gtog]->releaseBuf();gtog =++gtog % TOG;}
  int getPhysLen () 	{return smp_buf[gtog]->getLen();}
  int lrLen ()		{return lr_len;} 

private:
  inline void clearReceived()	{memset (received, 0, sizeof (received));}
  inline void clearConnect() 	{memset (connected, 0, sizeof (connected));}
  int setReceived (char*);		// set a locn data received flag
  void checkReceived();			// check for rcvd data from all dsms
  int checkTime (int, int, int);	// check for a new second
  int incrLrIndex ();			// increment logical record index

  void (*netMessage)(int, char*, char*); // message handler	

  RandomBuf *smp_buf[TOG];		// toggling physical buffers
  int ptog;				// put buffer index
  int gtog;				// get buffer index

  DsmTables &dsm_tbls;
  int connected[MAX_DSM];		// dsm connected flags
  int received[MAX_DSM];		// current second data received flags
  int lr_len;				// logical record length
  int pr_len;				// physical record length
  int lr_idx;				// current logical record index
  int last_lr_idx;			// last logical record index
  int cur_hour;				// current record hour
  int cur_min;				// current record minute
  int cur_sec;				// current record second
  char msg_str[DSM_MSG_STRING_SIZE];	// message string
};

#endif

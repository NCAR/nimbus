/* DsmTape.h
   Manages the Exabyte tape drives.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef DSMTAPE_H
#define DSMTAPE_H

#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include <dsmctl.h>

#include <DsmMessage.h>
#include <TapeDrv.h>
#include <TapeHeader.h>

#ifdef SVR4
#define DRV_0_NAME "/dev/rmt/0"			// drive 0 low density
#else
#define DRV_0_NAME "/dev/st0"			// drive 0 low density
#endif

class DsmTape {
public:
  DsmTape(TapeHeader& th, DsmMessage &win_m, DsmMessage &comm_m);
  void checkDrives();				// checks the drive status
  void loadDrive(int);				// load a drive
  void unloadDrive(int);			// unload a drive
  int writeDrive(char*, int);			// write to current rec drive
  void requestRecording(int); 			// request recording
  inline int recordingEnabled()
	{ return(drv_state[0] == RECORDING); }
  void ejectTape(int);				// eject drive

private:
  void enableRecording(int);			// attempt to enable recording
  int writeHeaderRecs(int);			// write header records

  TapeHeader &tape_header;
  DsmMessage &win_msg;
  DsmMessage &comm_msg;
  TapeDrv *exa[NDRV];				// Exabyte drive classes

  char leader[FIRST_REC_SIZE];			// first tape record
  char drv_name[NDRV][64];			// drive device names

  int drv_state[NDRV];           		// drive states
  int chk_drv;					// current drive to check
  int first_load;				// first load time out
  pthread_t tid[NDRV];				// Eject thread ID
  char msg_str[DSM_MSG_STRING_SIZE];          	// message string data
};

#endif

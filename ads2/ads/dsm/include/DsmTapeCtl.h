/* DsmTapeCtl.h
   Dsm tape control class for the local Exabyte tape drives.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef DSMTAPECTL_H
#define DSMTAPECTL_H

#include <fcntl.h>
#include <unistd.h>

#include <dsmctl.h>

#include <DsmMessage.h>
#include <Exb162.h>
#include <TapeHeader.h>

#define DRV_0_LUN	4			// drive 0 lun

class DsmTapeCtl {
public:
  DsmTapeCtl (TapeHeader& th, DsmMessage &tape_m);
  void checkDrives ();				// checks the drive status
  void loadDrive(int);				// load a drive
  void unloadDrive(int);			// unload a drive
  int writeDrive(char*, int);			// write to current rec drive
  void requestRecording (int); 			// request recording
  void stopRecording();				// stop recording & unload
  inline int recordingEnabled () {return (drv_state[rec_drv] == RECORDING);}

private:
  void enableRecording ();			// attempt to enable recording
  int writeHeaderRecs ();			// write header records
  void stampHeader ();				// time stamp the tape header

  TapeHeader &tape_header;
  DsmMessage &tape_msg;
  Exb162 *exa[NDRV];				// MVME162 Exabyte drive classes

  char leader[FIRST_REC_SIZE];			// first tape record
  char drv_name[NDRV][80];			// drive device names

  u_char drv_lun[NDRV];           		// drive luns
  int drv_state[NDRV];           		// drive states
  int rec_drv;					// recording drive
  int chk_drv;					// current drive to check
  int rec_rqst;					// recording requested flag
  int first_load;				// first load time out
  char msg_str[DSM_MSG_STRING_SIZE];          	// message string data
};

#endif

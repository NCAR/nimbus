/* DsmTape.cc
   Manages the Exabyte tape drives.

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <DsmTape.h>

void *loadolaDrive(void *arg);
void *ejectolaTape(void *arg);

/* -------------------------------------------------------------------- */
DsmTape::DsmTape(TapeHeader &th, DsmMessage &win_m, DsmMessage &comm_m) :
                 tape_header (th), win_msg (win_m), comm_msg (comm_m)
{
  int j;

  for (j = 0; j < NDRV; j++)
    drv_state[j] = UNLOADED;

  chk_drv = DRV_0;
  first_load = 10;			// initial time to wait for load

  strcpy(leader, FIRST_REC_STRING);
  strcpy(drv_name[DRV_0], DRV_0_NAME);

  // Create the Exabyte classes.
  exa[DRV_0] = new TapeDrv(drv_name[DRV_0]);

}  /* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void DsmTape::checkDrives()

// Checks the loading and recording state of the drives. This routine is to be
// called once each second.
{
  // If recording is not enabled, the flight number has been entered, 
  // the starting date and time have been received from the dsms, and
  // the drive is unloaded, then attempt to load the drive.

  if (drv_state[chk_drv] == UNLOADED)
    if (strlen(tape_header.fltnum()) && strlen(tape_header.date()))
      loadDrive(chk_drv);

}  /* END CHECKDRIVES */

/* -------------------------------------------------------------------- */
void DsmTape::loadDrive(int drv)
{
// Opens a drive and writes the leader record to it.

//  comm_msg.sendNetMsg(NET_SUSPEND, "", ""); // suspend data flow from comm task

  drv_state[drv] = LOADING;

  if (exa[drv]->Open(drv_name[drv]) == ERROR) {
    drv_state[drv] = UNLOADED;
    return;
  }

// Write the leader record.
  sprintf(msg_str, "Writing the leader record on drive %1d.\n", drv); 
  win_msg.sendTapeMsg(LOADING, drv, msg_str); 

  if (exa[drv]->Write(leader, sizeof (leader)) != sizeof (leader)) {
    sprintf(msg_str, "Error writing the leader record on drive %1d.\n", drv);  
    win_msg.sendTapeMsg(FAILED, drv, msg_str); 
    drv_state[drv] = FAILED;
    return;
  }


// The drive has been successfully loaded.
  drv_state[drv] = LOADED;
  sprintf(msg_str, "Drive %1d is loaded.\n", drv);
  win_msg.sendTapeMsg(LOADED, drv, msg_str); 

// Check for recording, and enable if not.
  enableRecording(drv);

}  /* END LOADDRIVE */

/* -------------------------------------------------------------------- */
void DsmTape::unloadDrive(int drv)
{

  if (drv_state[drv] == UNLOADED) {
    sprintf(msg_str, "Drive %d is not loaded.\n", drv);
    win_msg.sendTapeMsg(drv_state[drv], drv, msg_str); 
    return;
  }

  if (drv_state[drv] == UNLOADING) {
    sprintf(msg_str, "Drive %d is unloading.\n", drv);
    win_msg.sendTapeMsg(drv_state[drv], drv, msg_str); 
    return;
  }

//  comm_msg.sendNetMsg(NET_SUSPEND, "", ""); // stop data flow from comm task

  // If drive to unload is the recording drive, stop recording.
  if (drv_state[drv] == RECORDING) {
    drv_state[drv] = LOADED;
    sprintf(msg_str, "Recording disabled on drive %1d.\n", drv);
    win_msg.sendTapeMsg(drv_state[drv], drv, msg_str);
 
    // Write the filemarks.
    sprintf(msg_str, "Writing filemarks on drive %1d.\n", drv);
    win_msg.sendTapeMsg(drv_state[drv], drv, msg_str);
 
    if (exa[drv]->Filemark(2, 0)) {
      drv_state[drv] = FAILED;
      sprintf (msg_str, "Error writing filemarks on drive %1d.\n", drv);
      win_msg.sendTapeMsg (drv_state[drv], drv, msg_str);
    }
  }


  // Issue the eject command.
  drv_state[drv] = UNLOADING;
  sprintf (msg_str, "Unloading drive %1d.\n", drv);
  win_msg.sendTapeMsg (drv_state[drv], drv, msg_str); 

  pthread_create(&tid[drv], NULL, ejectolaTape, (void *)drv);

}  /* END UNLOADDRIVE */
 
/* -------------------------------------------------------------------- */
int DsmTape::writeDrive(char *buf, int len)
{
  int	i;

  for (i = 0; i < NDRV; ++i)
    if (drv_state[i] == RECORDING)
      if (exa[i]->Write(buf, len) != len)
        {
        sprintf(msg_str, "Failure writing to drive %1d.\n", i);
        win_msg.sendTapeMsg(FAILED, i, msg_str); 
        drv_state[i] = FAILED;
        return ERROR;
        }

  return(len);

}  /* END WRITEDRIVE */
 
/* -------------------------------------------------------------------- */
void DsmTape::ejectTape(int drv)
{
  if (!exa[drv]->Eject()) {
    exa[drv]->Close();
    drv_state[drv] = UNLOADED;
    sprintf (msg_str, "Drive %1d is unloaded.\n", drv);
    win_msg.sendTapeMsg (drv_state[drv], drv, msg_str); 
  }
  else {
    drv_state[drv] = FAILED;
    sprintf (msg_str, "Eject drive %1d returned error.\n", drv);
    win_msg.sendTapeMsg (drv_state[drv], drv, msg_str); 
  }

}  /* END EJECTTAPE */

/* -------------------------------------------------------------------- */
void DsmTape::enableRecording(int drv)
{
// If the recording drive is loaded, and the other drive is loaded or timed
// out, and the flight number has been entered, then enable recording.

  if (drv_state[drv] == LOADED)
    if (writeHeaderRecs(drv) != ERROR) {          // write the header records
      drv_state[drv] = RECORDING;
      sprintf(msg_str, "Recording enabled on drive %1d.\n", drv);
      win_msg.sendTapeMsg(drv_state[drv], drv, msg_str); 
      }
    else
      drv_state[drv] = FAILED;

}  /* END ENABLERECORDING */
 
/* -------------------------------------------------------------------- */
int DsmTape::writeHeaderRecs(int drv)
{
  // Write the first tape header record.
  if (exa[drv]->Write(tape_header.getHeader(), tape_header.thdrlen())
      != tape_header.thdrlen()) {
    sprintf(msg_str, 
             "Failure writing the first tape header on drive %1d.\n", drv);
    win_msg.sendTapeMsg(FAILED, drv, msg_str); 
    return ERROR;
  }
 
  // Write the second tape header record.
  if (exa[drv]->Write(tape_header.getHeader(), tape_header.thdrlen())
      != tape_header.thdrlen()) {
    sprintf(msg_str, 
      "Failure writing the second tape header on drive %1d.\n", drv);
    win_msg.sendTapeMsg(FAILED, drv, msg_str); 
    return ERROR;
  }

  sprintf(msg_str, "Header records written on drive %1d.\n", drv);
  win_msg.sendTapeMsg(LOADED, drv, msg_str); 

  return(OK);

}  /* END WRITEHEADERRECS */

/* END DSMTAPE.CC */

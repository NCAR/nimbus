/* DsmTapeCtl.cc
   Tape control class for the DSM local Exabyte tape drives.

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <DsmTapeCtl.h>

/******************************************************************************
** Public Functions
******************************************************************************/

DsmTapeCtl::DsmTapeCtl (TapeHeader &th, DsmMessage &tape_m) :
                  tape_header (th), tape_msg (tape_m)

// Constructor.
{
  int j;

  for (j = 0; j < NDRV; j++) {
    drv_state[j] = UNLOADED;
  }
  rec_drv = DRV_0;				// default recording drive
  chk_drv = DRV_0;
  rec_rqst = TRUE;				// recording requested flag
  first_load = 2;				// initial time to wait for load
  strcpy (leader, FIRST_REC_STRING);
  drv_lun[DRV_0] = DRV_0_LUN;

// Create the MVME162 Exabyte classes.
  exa[DRV_0] = new Exb162();

}
/*****************************************************************************/

void DsmTapeCtl::checkDrives ()

// Checks the loading and recording state of the drives. This routine is to be
// called once each second.
{
  switch (drv_state[chk_drv]) {

// If recording is not enabled, the flight number has been entered, 
// the starting date and time have been received from the dsms, and
// the drive is unloaded, then attempt to load the drive.
    case UNLOADED:
      if (!recordingEnabled () && strlen (tape_header.fltnum()) &&
          strlen (tape_header.date()))
        loadDrive (chk_drv);
      break;

    case FAILED:
      break;

    default:;
  }

// Enable recording if requested.
  if (rec_rqst)
    enableRecording();			
}
/*****************************************************************************/

void DsmTapeCtl::loadDrive (int drv)

// Opens a drive and writes the leader record to it.
{
// Suspend data flow from comm task. 
  tape_msg.sendNetMsg (NET_SUSPEND, "", ""); 

// Attempt to open the drive in unbuffered mode and waiting for unit ready.
  if (exa[drv]->open(drv_lun[drv], EXB_OPT_UNBUFFERED |
      EXB_OPT_VARIABLE_BLOCK_LENGTH | EXB_OPT_UNIT_READY_ON_OPEN, 0) == ERROR) {
    tape_msg.sendNetMsg (NET_ENABLE, "", ""); 	// resume data flow 
    return;
  }

// Load the drive.
  sprintf (msg_str, "Loading drive %1d.\n", drv);
  tape_msg.sendTapeMsg (LOADING, drv, msg_str); 

  if (exa[drv]->load (TRUE) == ERROR) { 
    sprintf (msg_str, "Error loading drive %1d.\n", drv);
    tape_msg.sendTapeMsg (FAILED, drv, msg_str);
    drv_state[rec_drv] = FAILED;
    tape_msg.sendNetMsg (NET_ENABLE, "", "");   // resume data flow
    return;
  }

// Write the leader record.
  sprintf (msg_str, "Writing the leader record on drive %1d.\n", drv); 
  tape_msg.sendTapeMsg (LOADING, drv, msg_str); 

  if (exa[drv]->write (leader, sizeof (leader)) != sizeof (leader)) {
    sprintf (msg_str, "Error writing the leader record on drive %1d.\n", drv);  
    tape_msg.sendTapeMsg (FAILED, drv, msg_str); 
    drv_state[rec_drv] = FAILED;
    tape_msg.sendNetMsg (NET_ENABLE, "", ""); 	// resume data flow 
    return;
  }
  sprintf (msg_str, "Leader record written on drive %1d.\n", drv);
  tape_msg.sendTapeMsg (LOADED, drv, msg_str); 

// Close the drive, reopen it in buffered mode, waiting for a unit ready.
  exa[drv]->close();
  if (exa[drv]->open (drv_lun[drv],
      EXB_OPT_VARIABLE_BLOCK_LENGTH | EXB_OPT_UNIT_READY_ON_OPEN, 0) == ERROR) {
    sprintf (msg_str,"Error opening drive %1d in buffered mode.\n", drv);
    tape_msg.sendTapeMsg (FAILED, drv, msg_str); 
    drv_state[rec_drv] = FAILED;
    tape_msg.sendNetMsg (NET_ENABLE, "", ""); 	// resume data flow 
    return;
  }

// The drive has been successfully loaded.
  drv_state[drv] = LOADED;
  sprintf (msg_str, "Drive %1d is loaded.\n", drv);
  tape_msg.sendTapeMsg (LOADED, chk_drv, msg_str); 
  tape_msg.sendNetMsg (NET_ENABLE, "", ""); 	// resume data flow 
}
/*****************************************************************************/
 
void DsmTapeCtl::unloadDrive (int drv)
 
// Issues an unload command to the current drive.
{
  if (recordingEnabled()) {
    sprintf (msg_str, "Cannot unload while recording is enabled.\n", drv);
    tape_msg.sendTapeMsg (drv_state[drv], drv, msg_str); 
    return;
  }

// Cancel any pending recording requests.
  rec_rqst = FALSE;
  
 
// Issue the eject command.
  drv_state[drv] = UNLOADING;
  sprintf (msg_str, "Unloading drive %1d.\n", drv);
  tape_msg.sendTapeMsg (drv_state[drv], drv, msg_str); 

  tape_msg.sendNetMsg (NET_SUSPEND, "", ""); // suspend data flow from comm task

  if (exa[drv]->load (FALSE) == OK) {
    printf("exa[drv].load(FALSE_ = OK\n");
    exa[drv]->close();
    drv_state[drv] = UNLOADED;
    sprintf (msg_str, "Drive %1d is unloaded.\n", drv);
    tape_msg.sendTapeMsg (drv_state[drv], drv, msg_str); 
  }
  else {
    printf("exa[drv].load(FALSE_ != OK\n");
    taskDelay (sysClkRateGet() / 4);	// don't overflow the message queue
    drv_state[drv] = FAILED;
    sprintf (msg_str, "Unload drive %1d returned error.\n", drv);
    tape_msg.sendTapeMsg (drv_state[drv], drv, msg_str); 
  }
  tape_msg.sendNetMsg (NET_ENABLE, "", ""); // resume data flow from comm task 
}
/*****************************************************************************/
 
int DsmTapeCtl::writeDrive (char *buf, int len)
 
// Writes to the current recording drive.
{
  if (!recordingEnabled())
    return OK;
 
// Write the record.

  if (exa[rec_drv]->write (buf, len) != len) {
    sprintf (msg_str, "Failure writing to drive %1d.\n", rec_drv);
    tape_msg.sendTapeMsg (FAILED, rec_drv, msg_str); 
    drv_state[rec_drv] = FAILED;
    return ERROR;
  }
  return len;
}
/*****************************************************************************/
 
void DsmTapeCtl::requestRecording (int drv)
 
// Request recording on the specified drive.
{
  if (recordingEnabled()) {
    sprintf (msg_str, "Recording is already enabled on drive %1d.\n", rec_drv);
    tape_msg.sendTapeMsg (drv_state[drv], rec_drv, msg_str); 
    return;
  }

  if (drv_state[drv] != LOADED) {
    sprintf (msg_str, "Drive %1d is not loaded.\n", drv);
    tape_msg.sendTapeMsg (drv_state[drv], drv, msg_str); 
    return;
  }

  rec_rqst = TRUE;
  rec_drv = drv;
  sprintf (msg_str, "Recording requested on drive %1d.\n", drv);
  tape_msg.sendTapeMsg (drv_state[drv], drv, msg_str); 
                        
  return;
}
/*****************************************************************************/
 
void DsmTapeCtl::stopRecording ()
 
// Stop recording, write the file marks, and unload the recording drive.
{
  if (!recordingEnabled()) {
    sprintf (msg_str, "Recording is not enabled.\n", rec_drv); 
    tape_msg.sendTapeMsg (drv_state[rec_drv], rec_drv, msg_str); 
    return;
  }

  tape_msg.sendNetMsg (NET_SUSPEND, "", ""); // suspend data flow from comm task

  drv_state[rec_drv] = LOADED;
  sprintf (msg_str, "Recording disabled on drive %1d.\n", rec_drv);
  tape_msg.sendTapeMsg (drv_state[rec_drv], rec_drv, msg_str); 

// Write the filemarks.
  sprintf (msg_str, "Writing filemarks on drive %1d.\n", rec_drv);
  tape_msg.sendTapeMsg (drv_state[rec_drv], rec_drv, msg_str); 

  if (exa[rec_drv]->writeFilemarks (2) == ERROR) {
    drv_state[rec_drv] = FAILED;
    sprintf (msg_str, "Error writing filemarks on drive %1d.\n", rec_drv);
    tape_msg.sendTapeMsg (drv_state[rec_drv], rec_drv, msg_str); 
  }

// Delay before issuing unload, or an error occurs.
  taskDelay (sysClkRateGet() * 5);	  	

// Unload the drive.
  unloadDrive (rec_drv);
  tape_msg.sendNetMsg (NET_ENABLE, "", ""); // resume data flow from comm task 
}
/******************************************************************************
** Private Functions
******************************************************************************/

void DsmTapeCtl::enableRecording ()

// Attempts to enable recording.
{
// If the recording drive is loaded, and the other drive is loaded or timed
// out, and the flight number has been entered, then enable recording.

  if ((drv_state[rec_drv] == LOADED) &&
      ((drv_state[(rec_drv + 1) % NDRV] == LOADED) || (first_load-- <= 0))) {

    if (writeHeaderRecs () != ERROR) {          // write the header records
      drv_state[rec_drv] = RECORDING;
      rec_rqst = FALSE;
      sprintf (msg_str, "Recording enabled on drive %1d.\n", rec_drv);
      tape_msg.sendTapeMsg (drv_state[rec_drv], rec_drv, msg_str); 
    }
    else {
      drv_state[rec_drv] = FAILED;
    }
  }
}
/*****************************************************************************/
 
int DsmTapeCtl::writeHeaderRecs ()
 
// Writes two tape header records on the current drive.
{
// Time stamp the header.

// Write the first tape header record.
  if (exa[rec_drv]->write (tape_header.getHeader(), tape_header.thdrlen())
      != tape_header.thdrlen()) {
    sprintf (msg_str, 
             "Failure writing the first tape header on drive %1d.\n", rec_drv);
    tape_msg.sendTapeMsg (FAILED, rec_drv, msg_str); 
    return ERROR;
  }
 
// Write the second tape header record.
  if (exa[rec_drv]->write (tape_header.getHeader(), tape_header.thdrlen())
      != tape_header.thdrlen()) {
    sprintf (msg_str, 
      "Failure writing the second tape header on drive %1d.\n", rec_drv);
    tape_msg.sendTapeMsg (FAILED, rec_drv, msg_str); 
    return ERROR;
  }
  sprintf (msg_str, "Header records written on drive %1d.\n", rec_drv);
  tape_msg.sendTapeMsg (LOADED, rec_drv, msg_str); 

  return OK;
}
/*****************************************************************************/

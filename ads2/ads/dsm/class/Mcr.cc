/* Mcr.cc
   Mcr control class.

   Original Author: Mike Spowart 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <Mcr.h>

char *mcr_name[] = {"MCR-0","MCR-1","MCR-2",
                    "MCR-3","MCR-4","MCR-5","MCR-6"};

/******************************************************************************
** Public Functions
******************************************************************************/
 
Mcr::Mcr (McrTable &mcr_tbl, Vmio12 *vmio12[], Bc635Vme& tp, Dsp56002 *dp, 
          DsmMessage& dm, AnalogTable &at) : 
          dsm_msg (dm), tfp (tp), ana_tbl (at)
 
// Class constructor.
{
// Create the Vmio14Out digital output port classes.
  mcr_out = new Vmio14Out (vmio12[mcr_tbl.boardNumber()], mcr_tbl.address());
  dsp = dp;
  int i,j;

// Set the default port output.
  cur_out = 0;
  mcr_out->writePort (cur_out);

  remote = FALSE;
  cur_stat = MCR_INITIAL_STATUS;
}
/*****************************************************************************/

void Mcr::secondAlign ()

// Packs the 7 Mcr channels.
{
  int i,j;

  mcr_hdr.hour = tfp.hour();
  mcr_hdr.minute = tfp.minute();
  mcr_hdr.second = tfp.second();
//  logMsg("hour = %d, minute = %d, second = %d\n",mcr_hdr.hour,mcr_hdr.minute,
//          mcr_hdr.second,0,0,0);
  j = ana_tbl.firstChannel();
  for (i=0; i < NUM_HIGH_CHAN; i++) {
    j = ana_tbl.address();
//    sprintf (mcr_hdr.id, "MCR-%1d", j);
    strcpy (mcr_hdr.id, mcr_name[j]);
    j = ana_tbl.nextChannel();
    dsp->buf_5000[dsp->ptog][i]->copyToBuf ((char*)&mcr_hdr, sizeof (Mcr_hdr));
  }
}
/*****************************************************************************/

void Mcr::setStatus (int status)

// Gets the status word from the passed in logical record buffer.
{
// If this is the first time the status is being set, set the 
// MCR_DOOR_SELECT_BIT to match the current state of the doors.
  if ((cur_stat == MCR_INITIAL_STATUS) && 
      (status & (MCR_RIGHT_DOOR_OPEN | MCR_LEFT_DOOR_OPEN)))
    setDoorSelect (TRUE);

  if (cur_stat != status)
    dsm_msg.sendMcrMsg (MCR_STATUS, status, "", "");

// Put in checks for status changes, and output messages.
  cur_stat = status;
}
/*****************************************************************************/

void Mcr::control (int action, int value)

// Handles commands to the mcr.
{
  switch (action) {
    case MCR_REMOTE_ENABLE:		// enable/disable dsm control
      setRemote (value);
      break;
    case MCR_PREAMP_ENABLE:		// enable/disable preamp power
      setPreampEnable (value);
      break;
    break;
    case MCR_DOOR_ENABLE:		// enable/disable door motors
      setDoorEnable (value);
      break;
    case MCR_DOOR_SELECT:		// open/close doors
      setDoorSelect (value);
      break;
    case MCR_HEATER_SELECT:		// heaters on/off
      setHeaterSelect (value);
      break;
    break;
  }
}
/*****************************************************************************/

void Mcr::toggleHeartBeat ()

// Toggles the heartbeat bit.
{
  if (!remote)			// check for control enabled
    return;
  if (cur_out & MCR_REMOTE_HEART_BIT)
    cur_out &= ~MCR_REMOTE_HEART_BIT;
  else
    cur_out |= MCR_REMOTE_HEART_BIT;
  mcr_out->writePort (cur_out);
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

void Mcr::setRemote (int value)

// Sets/clears the remote enable bit.
{
  remote = value;

  if (remote)
    dsm_msg.sendMcrMsg (MCR_REMOTE_ENABLE, value, "", 
                        "MCR control enabled.\n");
  else
    dsm_msg.sendMcrMsg (MCR_REMOTE_ENABLE, value, "", 
                        "MCR control disabled.\n");
}
/*****************************************************************************/

void Mcr::setPreampEnable (int value)

// Sets/clears the preamp enable bit.
{
  if (value) {
    cur_out |= MCR_PREAMP_ENABLE_BIT;
    dsm_msg.sendMcrMsg (MCR_PREAMP_ENABLE, value, "", 
                        "MCR preamp enabled.\n");
  }
  else {
    cur_out &= ~MCR_PREAMP_ENABLE_BIT;
    dsm_msg.sendMcrMsg (MCR_PREAMP_ENABLE, value, "", 
                        "MCR preamp disabled.\n");
  }
  mcr_out->writePort (cur_out);
}
/*****************************************************************************/

void Mcr::setDoorEnable (int value)

// Sets/clears the door enable bit.
{
  if (value) {
    cur_out |= MCR_DOOR_ENABLE_BIT;
    dsm_msg.sendMcrMsg (MCR_DOOR_ENABLE, value, "", 
                        "MCR door motor enabled.\n");
  }
  else {
    cur_out &= ~MCR_DOOR_ENABLE_BIT;
    dsm_msg.sendMcrMsg (MCR_DOOR_ENABLE, value, "", 
                        "MCR door motor disabled.\n");
  }
  mcr_out->writePort (cur_out);
}
/*****************************************************************************/

void Mcr::setDoorSelect (int value)

// Sets/clears the door select bit.
{
  if (value) {
    cur_out |= MCR_DOOR_SELECT_BIT;
    dsm_msg.sendMcrMsg (MCR_DOOR_SELECT, value, "", 
                        "MCR doors open command issued.\n");
  }
  else {
    cur_out &= ~MCR_DOOR_SELECT_BIT;
    dsm_msg.sendMcrMsg (MCR_DOOR_SELECT, value, "", 
                        "MCR doors close command issued.\n");
  }
  mcr_out->writePort (cur_out);
}
/*****************************************************************************/

void Mcr::setHeaterSelect (int value)

// Sets/clears the heater select bit.
{
  if (value) {
    cur_out |= MCR_HEATER_SELECT_BIT;
    dsm_msg.sendMcrMsg (MCR_HEATER_SELECT, value, "", 
                        "MCR heaters on.\n");
  }
  else {
    cur_out &= ~MCR_HEATER_SELECT_BIT;
    dsm_msg.sendMcrMsg (MCR_HEATER_SELECT, value, "", 
                        "MCR heaters off.\n");
  }
  mcr_out->writePort (cur_out);
}

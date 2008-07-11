/* dsmDisplay.cc
   Task for managing the front panel display.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

// System header files.
#include <vxWorks.h>
#include <stdioLib.h>
#include <sysLib.h>
#include <wdLib.h>

// Application header files.
#include <dsmctl.h>
#include <header.h>
#include <dsmVmeDefs.h>
#include <tvmDefs.h>

// Class include files.
#include <DsmConfig.h>
#include <DsmMessage.h>
#include <SerialBuf.h>
#include <SyncVar.h>
#include <TapeHeader.h>
#include <TvmConfig.h>
#include <TvmDisplay.h>
#include <TvmFixedFmt.h>
#include <TvmFltNumber.h>
#include <TvmStatus.h>
#include <TvmTape.h>
#include <TvmUtilities.h>

// Program functions.
static void checkData();			// check for and process msgs
static void checkMessage();			// check for and process msgs
static void checkDisplay();			// check for and process msgs
static void cycleDisplay();			// cycle to the next display
static int selectNextFixedFmt();		// select the next fixed fmt
static void initFixedFmt();			// init fixed format displays
static void initSyncVar();			// init SyncVar classes

// Class declarations.
static DsmConfig *dsm_config; 			// network configuration class
static MsgQueue *disp_dataQ;  			// data message queue
static MsgQueue *disp_txQ;   			// recv message queue
static MsgQueue *disp_rxQ;   			// send message queue
static DsmMessage *disp_msg; 			// message handler class
static SyncVar *sync_var[TVM_FIXED_FMT_TOTAL_ENTRIES];	// computed variables
static TapeHeader *dsm_hdr; 			// tape header class
static TvmFixedFmt *tvm_fixed[TVM_FIXED_FMT_MAX_DISPLAYS];// fixed fmt displays
static SerialBuf *smp_buf;  			// sampled data buffer
static TvmConfig *tvm_config;			// display configuration class
static TvmDisplay *tvm_display;			// base display class
static TvmFltNumber *tvm_flight;		// flight number display class
static TvmStatus *tvm_status;			// tape display class
static TvmTape *tvm_tape;			// tape display class
static TvmUtilities *tvm_utilities;		// utilities display class

extern BOOT_PARAMS sysBootParams;
static int display_select = DISPLAY_STATUS;	// active display state var
static int fixed_idx = TVM_FIXED_FMT_MAX_DISPLAYS; // current fix fmt disp idx

int dsmDisplay (MsgQueue *dataQ, MsgQueue *txQ, MsgQueue *rxQ)
{
// Create the message queues.
  disp_dataQ = (MsgQueue*)malloc (sizeof (MsgQueue));
  *disp_dataQ = *dataQ;
  disp_txQ = (MsgQueue*)malloc (sizeof (MsgQueue));
  *disp_txQ = *txQ;
  disp_rxQ = (MsgQueue*)malloc (sizeof (MsgQueue));
  *disp_rxQ = *rxQ;

// Initialize the network configuration class, using the class already in SRAM.
//  dsm_config = (DsmConfig*)malloc (sizeof (DsmConfig));
//  *dsm_config = *(DsmConfig*)SRAM_DSMCONFIG_BASE;

  dsm_config = new DsmConfig ();
  if (!dsm_config->selectByHost (sysBootParams.targetName)) {
    fprintf (stderr, "dsmDisplay exiting.\n");
    exit (ERROR);
  }

// Initialize the tape header class, using the header already in SRAM.
//  dsm_hdr = (TapeHeader*)malloc (sizeof (TapeHeader));
//  *dsm_hdr = *(TapeHeader*)SRAM_TAPEHEADER_BASE;
  dsm_hdr = new TapeHeader();
  dsm_hdr->readFile (dsm_config->dsmHeaderName());

// Initialize the data buffer class.
  smp_buf = new SerialBuf (dsm_hdr->lrlen());

/* Create and initialize the display configuration class in SRAM.  If not 
   operating in the standalone mode, attempt to read the tvmconfig file
   by running the constructor.  Otherwise just use the existing configuration
   in SRAM.
*/
  tvm_config = (TvmConfig*)SRAM_TVMCONFIG_BASE;
  if (!dsm_config->standalone()) {
    tvm_config = new TvmConfig(sysBootParams.hostName, dsm_config->location());
    printf("dsmDisplay: not standalone.\n");
    taskDelay(200);
  }

// Initialize the display classes.
  tvm_display = new TvmDisplay();
  tvm_flight = new TvmFltNumber (*tvm_display);
  tvm_status = new TvmStatus (*tvm_display);
  tvm_tape = new TvmTape(*tvm_display);
  tvm_utilities = new TvmUtilities (*tvm_display);

// Create and initialize the fixed format displays.
  initFixedFmt();

// Initialize the computed variables classes.
//  initSyncVar();

  tvm_status->initDisplay();
  tvm_status->displayMachineName (sysBootParams.targetName);
  tvm_status->displayProjectNumber (dsm_hdr->projectNumber());

// Initialize the message handler class. txQ is for messages to this task,
// rxQ is for messages from this class.
  disp_msg = new DsmMessage (*disp_rxQ, *disp_txQ);

// Main loop.
  while (TRUE) {
    checkData();			// check for received data to display
    checkMessage ();			// check for received messages
    checkDisplay();			// check display for keypresses
    taskDelay(sysClkRateGet() / 4);	// delay 250 ms
  }
}
/*****************************************************************************/

static void checkData()

// Check for received data for display.
{
  int stat;
  Hdr_blk *hdr;

  if (disp_dataQ->msgRecv ((char*)(hdr = (Hdr_blk*)smp_buf->getBufPtr()), 
      MX_PHYS) != ERROR) {
    
    switch (display_select) {
      case DISPLAY_STATUS:
        tvm_status->displayDate (hdr->year, hdr->month, hdr->day);
        tvm_status->displayTime (hdr->hour, hdr->minute, hdr->second);
        break;

      case DISPLAY_FIXED_FMT:

// Update the current fixed format display time.
        tvm_fixed[fixed_idx]->displayTime (hdr->hour, hdr->minute, hdr->second);

// For each variable in the display, compute the engineering units value,
// and update the display entry.
        for (stat=tvm_config->firstVar(); stat; stat = tvm_config->nextVar()) {
          if (tvm_config->displayNumber() == fixed_idx) {
            sync_var[tvm_config->index()]->computeFromLR (smp_buf->getBufPtr());
            tvm_fixed[fixed_idx]->updateVariableEntry (
                                    sync_var[tvm_config->index()]->value(), 
                                    tvm_config->displayEntry());
          }
        }
        break;

      default:;
    }
  }
}
/*****************************************************************************/

static void checkMessage ()

// Check for received status messages.
{
  if (!disp_msg->readMsg ()) 
    return;

// Process the command.
  switch (disp_msg->type()) {
    case TAPE_MSG:
      printf ("dsmDisplay: checkMessage, drive = %1d, action = %1d\n", 
              disp_msg->drive(), disp_msg->action());
      tvm_tape->displayDriveStatus (disp_msg->drive(), disp_msg->action());
      break;

    default:
      fprintf (stderr, 
        "dsmDisplayTx: Unknown message type received from disp_msg.\n");
  }
}
/*****************************************************************************/

static void checkDisplay()

// Checks the display for changes.
{

// Check for a key press.
  if (tvm_display->chkKeypress()) {

// Determine which display is active, and process the key for that display.
    switch (display_select) {
      case DISPLAY_STATUS:			
        switch (tvm_status->processKeys()) {
          case DISPLAY_ACTION_NONE:
          case DISPLAY_ACTION_KEY:
            break;
          case DISPLAY_ACTION_TOGGLE:
            cycleDisplay();
            break; 
        }
        break;

      case DISPLAY_FLIGHT:
        switch (tvm_flight->processKeys()) {
          case DISPLAY_ACTION_NONE:
            break;
          case DISPLAY_ACTION_KEY:
            disp_msg->sendFlightMsg (tvm_flight->flightNumber());
            tvm_status->displayFlightNumber (tvm_flight->flightNumber());
            break;
          case DISPLAY_ACTION_TOGGLE: 		// flight number entered
            cycleDisplay();
            break; 
        }
        break;

      case DISPLAY_TAPE:
        switch (tvm_tape->processKeys()) {
          case DISPLAY_ACTION_NONE:
            printf("dsmDisplay action_none\n");
            break;
          case DISPLAY_ACTION_KEY:		// tape action requested
            disp_msg->sendTapeMsg (tvm_tape->action(), tvm_tape->drive(), "");
            printf("dsmDisplay action_key\n");
            break;
          case DISPLAY_ACTION_TOGGLE:
            printf("dsmDisplay action_toggle\n");
            cycleDisplay();
            break; 
        }
        break;

      case DISPLAY_FIXED_FMT:
        printf ("fixed_idx = %d\n", fixed_idx);
        switch (tvm_fixed[fixed_idx]->processKeys()) {
          case DISPLAY_ACTION_NONE:
          case DISPLAY_ACTION_KEY:   
            break;
          case DISPLAY_ACTION_TOGGLE:
            cycleDisplay();
            break;
        }
        break;

      case DISPLAY_UTILITIES:
        switch (tvm_utilities->processKeys()) {
          case DISPLAY_ACTION_NONE:
          case DISPLAY_ACTION_KEY:
            break;
          case DISPLAY_ACTION_TOGGLE:
            cycleDisplay();
            break; 
        }
        break;
    }
  }
}
/*****************************************************************************/

static void cycleDisplay()

// Cycles to the next display.
{
  switch (display_select) {

    case DISPLAY_STATUS:
      tvm_status->clearDisplay();

// If standalone operation is set, toggle to the flight number entry display.
      if (dsm_config->standalone()) {
        display_select = DISPLAY_FLIGHT;
        tvm_flight->initDisplay();      
      }

// Else if local recording is set, toggle to the tape control display.
      else if (dsm_config->localRecord()) {
        display_select = DISPLAY_TAPE;      // toggle to next display
        tvm_tape->initDisplay();
      }

// Else if  there is a fixed format display, initialize it.
      else if (selectNextFixedFmt()) {
        tvm_fixed[fixed_idx]->initDisplay();
        display_select = DISPLAY_FIXED_FMT;
      }

// Else toggle to the utilities display.
      else {
        tvm_utilities->initDisplay();
        display_select = DISPLAY_UTILITIES;
      }
      break;

    case DISPLAY_FLIGHT:
      tvm_flight->clearDisplay();

// If local recording is set, toggle to the tape control display.
      if (dsm_config->localRecord()) {
        display_select = DISPLAY_TAPE;      // toggle to next display
        tvm_tape->initDisplay();
      }

// Else if  there is a fixed format display, initialize it.
      else if (selectNextFixedFmt()) {
        tvm_fixed[fixed_idx]->initDisplay();
        display_select = DISPLAY_FIXED_FMT;
      }

// Else toggle to the utilities display.
      else {
        display_select = DISPLAY_UTILITIES; // toggle to next display
        tvm_utilities->initDisplay();
      }
      break;
 
    case DISPLAY_TAPE:
      tvm_tape->clearDisplay();
      
// If  there is a fixed format display, initialize it.
      if (selectNextFixedFmt()) {
        tvm_fixed[fixed_idx]->initDisplay();
        display_select = DISPLAY_FIXED_FMT;
      }

// Else toggle to the utilities display.
      else {
        tvm_utilities->initDisplay();
        display_select = DISPLAY_UTILITIES;
      }
      break;

    case DISPLAY_FIXED_FMT:
      tvm_fixed[fixed_idx]->clearDisplay();

// If  there is another fixed format display, initialize it.
      if (selectNextFixedFmt())
        tvm_fixed[fixed_idx]->initDisplay();

// Else toggle to the utilities display.
      else {
        tvm_utilities->initDisplay();
        display_select = DISPLAY_UTILITIES;
      }
      break;

    case DISPLAY_UTILITIES:
      tvm_utilities->clearDisplay();

// Toggle to the status display.
      tvm_status->initDisplay();
      display_select = DISPLAY_STATUS; 
      break;
  }
}
/*****************************************************************************/

static int selectNextFixedFmt()

// Selects the index of the next fixed format display.
{
  printf ("1: fixed_idx = %d\n", fixed_idx);

  if (fixed_idx >= TVM_FIXED_FMT_MAX_DISPLAYS)
    fixed_idx = 0;
  else
    fixed_idx++;

  printf ("2: fixed_idx = %d\n", fixed_idx);
// Find the next initialized display.
  for (; (fixed_idx < TVM_FIXED_FMT_MAX_DISPLAYS) &&
       !(int)tvm_fixed[fixed_idx]; fixed_idx++);
 
  printf ("3: fixed_idx = %d\n", fixed_idx);
// Return TRUE if a display has been selected, FALSE otherwise.
  if (fixed_idx < TVM_FIXED_FMT_MAX_DISPLAYS) 
    return TRUE;
  else
    return FALSE;
}
/*****************************************************************************/
 
static void initFixedFmt()
 
// Creates and initializes the fixed format display classes.
{
  int stat;

// Null all of the class pointers.
  for (stat = 0; stat < TVM_FIXED_FMT_MAX_DISPLAYS; stat++)
    tvm_fixed[stat] = (TvmFixedFmt*)0;

// Create display classes and initialize entries as specified in the tvmconfig
// file.
  for (stat = tvm_config->firstVar(); stat; stat = tvm_config->nextVar()) {

// Create the display class if it has not already been created.
    if (!(int)tvm_fixed[tvm_config->displayNumber()]) {
      tvm_fixed[tvm_config->displayNumber()] = new TvmFixedFmt (*tvm_display, 
                                                tvm_config->displayNumber());
      if (tvm_fixed[tvm_config->displayNumber()] == NULL) {
        perror ("dsmDisplay, Creating TvmFixedFmt:");
        exit (ERROR);
      }
    }

// Initialize the variable entry.
    tvm_fixed[tvm_config->displayNumber()]->initVariableNames(
                       tvm_config->varName(), tvm_config->displayEntry());
  }
}
/*****************************************************************************/

static void initSyncVar()

// Creates and initializes the computed variables classes.
{
  int stat;

  for (stat = tvm_config->firstVar(); stat; stat = tvm_config->nextVar()) {
    sync_var[tvm_config->index()] = new SyncVar(dsm_hdr, tvm_config->varName());
    if (sync_var[tvm_config->index()] == NULL) { 
      perror ("dsmDisplay, Creating SyncVar:");
      exit (ERROR);
    }
  }
}
/*****************************************************************************/

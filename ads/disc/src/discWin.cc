/* discWin.cc
   DIstributed Sampling Control WINdowed user interface program.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/


// System header files.
#include <stdlib.h>
#include <time.h>

// Application header files.
#include <dsmctl.h>

// Class include files.
#include <AnalogWin.h>				// analog control window
#include <DsmHeaders.h>				// tape headers class
#include <DsmMessage.h>				// message handler
#include <DsmNet.h>				// network communications
#include <FlightWin.h>				// flight number entry window
#include <InstrumentWin.h>			// Instrument control
#include <GenericButtons.h>			// GenericButtons
#include <McrWin.h>				// mcr control window
#include <Pms1dWin.h>				// pms 1d control window
#include <Pms2dWin.h>				// pms 1d control window
#include <StatusWin.h>				// quit control window
#include <UdpSocket.h>				// Udp socket class
#include <UnixTask.h>				// child task manager


/* -------------------------------------------------------------------- */
// Program functions.
void initPms1Win(Widget top_pane);	// create & init pms 1d window
void initPms2Win(Widget top_pane);	// create & init pms 2d window
void initMcrWin(Widget top_pane);	// create & init mcr window
void initGenButtons(Widget top_pane);	// create & init mcr window
void checkMessage();			// check for received messages
void sendInstrumentControlStates();
#ifdef SOCK_WINDS
void windsFlightMessage();		// send flight number to winds
#endif
void netMessage(int, char*, char*);    	// network message handler

// Callbacks.
void AnalogWinToggleButtonCallbackWrap(Widget, XtPointer, 
                                        XmToggleButtonCallbackStruct*);
void AnalogWinPushButtonCallbackWrap(Widget, XtPointer, 
                                      XmPushButtonCallbackStruct*);
void AnalogWinWarningCallbackWrap(Widget, XtPointer, XtPointer);
void FlightWinCallbackWrap(Widget, XtPointer, XmSelectionBoxCallbackStruct*);
void Pms1dWinCallbackWrap(Widget, XtPointer, XmToggleButtonCallbackStruct*);
void Pms2dWinTasCallbackWrap(Widget, XtPointer, XmToggleButtonCallbackStruct*);
void Pms2dWinRateCallbackWrap(Widget, XtPointer, XmToggleButtonCallbackStruct*);
void Pms2dWinProbeCallbackWrap(Widget, XtPointer,XmToggleButtonCallbackStruct*);
void InstrumentWinPushButtonWrap
			(Widget, XtPointer, XmPushButtonCallbackStruct *);
void McrWinToggleButtonCallbackWrap
			(Widget, XtPointer, XmToggleButtonCallbackStruct *);
void GenericButtonToggleButtonCallbackWrap
			(Widget, XtPointer, XmToggleButtonCallbackStruct *);

void QuitButtonWrap(Widget, XtPointer, XmPushButtonCallbackStruct *);
void AnalogButtonWrap(Widget, XtPointer, XmPushButtonCallbackStruct *);
void InstrumentButtonWrap(Widget, XtPointer, XmPushButtonCallbackStruct *);
void RecordWrap(Widget, XtPointer, XmPushButtonCallbackStruct *);

void timerCallback(XtPointer, XtIntervalId*);
                              
/* -------------------------------------------------------------------- */
// Class declarations.

StatusWin	*statusWin;		// Status window (Main window)
FlightWin	*flightWin;		// flight number entry window
AnalogWin	*analogWin;		// Analog control window
InstrumentWin	*instrumentWin;		// Instrument control window

Pms1dWin	*pms1_win;		// pms 1d control window
Pms2dWin	*pms2_win;		// pms 2d control window
GenericButtons	*genButtons;		// Generic Control Buttons
McrWin		*mcr_win;		// mcr control window


DsmConfig	dsm_config;                          	// config file class
DsmHeaders	dsm_headers (TRUE);			// tape headers class

DsmNet	comm_win_net(TCP_CONNECT, DSM_MSG_MAX_LEN,   	// Tape to Win net
         DSM_COMM_WIN_PORT, getenv ("HOST"), "Win-Comm", netMessage);
DsmNet	tape_win_net(TCP_ACCEPT, DSM_MSG_MAX_LEN,   	// Comm to Win net
         DSM_TAPE_WIN_PORT, getenv ("HOST"), "Win-Tape", netMessage);

DsmMessage	dsm_msg(comm_win_net);		// dsm messages class
DsmMessage	tape_msg(tape_win_net);		// tape messages class
#ifdef SOCK_WINDS
UdpSocket	*winds_sock;			// socket for talking to winds
#endif

UnixTask	comm_task("discComm");
UnixTask	tape_task("discTape");


/* -------------------------------------------------------------------- */
// Global variables.

static XtAppContext	app;
static Widget		AppShell, Shell000, Shell001, Shell002;


/* -------------------------------------------------------------------- */
main (int argc, char *argv[])
{
  while (!comm_win_net.connectNet() || !tape_win_net.acceptNet())
    sleep (1);

// Create the winds message socket.
  dsm_config.selectByLocn(WINDS_LOCN_STR); 		// get winds host
#ifdef SOCK_WINDS
  winds_sock = new UdpSocket(DSM_WINDS_PORT, dsm_config.hostName());
  winds_sock->openSock(UDP_UNBOUND);
#endif

// Create the top level application shell and paned window.
  AppShell = XtAppInitialize(&app, "DiscWin", NULL,0, &argc,argv, NULL,NULL,0);
 
  Shell000 = XtCreatePopupShell("topLevelShell",
			topLevelShellWidgetClass, AppShell, NULL, 0);

  Shell001 = XtCreatePopupShell("analogShell",
			topLevelShellWidgetClass, AppShell, NULL, 0);

  Shell002 = XtCreatePopupShell("instrumentShell",
			topLevelShellWidgetClass, AppShell, NULL, 0);

// Create the project information window.
  statusWin = new StatusWin(Shell000, tape_msg,
				QuitButtonWrap,
				AnalogButtonWrap,
				InstrumentButtonWrap,
				RecordWrap);


// Create the analog control window.
  analogWin = new AnalogWin(Shell001, dsm_msg, dsm_headers, 
     AnalogWinToggleButtonCallbackWrap, AnalogWinPushButtonCallbackWrap,
     AnalogWinWarningCallbackWrap);


// Create the instrument window frame work.
  instrumentWin = new InstrumentWin(Shell002);

// Create the pms 1d control window if needed.
  initPms1Win(instrumentWin->RowColumn());

// Create the pms 2d control window if needed.
  initPms2Win(instrumentWin->RowColumn());

// Create the mcr control window if needed.
  initMcrWin(instrumentWin->RowColumn());

// Create the mcr control window if needed.
  initGenButtons(instrumentWin->RowColumn());

  instrumentWin->addDismiss(InstrumentWinPushButtonWrap);

// Display the project number.
  statusWin->displayProjectNumber(dsm_headers.mainHeader()->projectNumber());

//  XtRealizeWidget (AppShell);
  XtManageChild(statusWin->mainForm);
  XtPopup(XtParent(statusWin->mainForm), XtGrabNone);

  flightWin = new FlightWin(AppShell, FlightWinCallbackWrap,
		dsm_headers.mainHeader()->projectNumber());

// Start the timer.
  XtAppAddTimeOut(app, 2000, timerCallback, 0);

  XtAppMainLoop (app);
}

/* -------------------------------------------------------------------- */
void initPms1Win (Widget instRC)

// Parses the tape header and initializes the pms 1d control window if needed.
{
  int stat;

  pms1_win = (Pms1dWin*)0;

// Parse the header looking for ranging 1d probes.
  for (stat = dsm_headers.mainHeader()->firstDesc(); stat; 
       stat = dsm_headers.mainHeader()->nextDesc()) {

    if (dsm_headers.mainHeader()->pms1v2Type() || 
        dsm_headers.mainHeader()->pms1Type()) {
      if (!strcmp (dsm_headers.mainHeader()->name(), FSSP_STR)) {

        if (!(int)pms1_win)		// create pane if not yet created
          pms1_win = new Pms1dWin(instRC, dsm_msg);

        pms1_win->createFssp (dsm_headers.mainHeader()->name(), 
                              dsm_headers.mainHeader()->locn(),
                              dsm_headers.mainHeader()->dsm_locn(), 
                              Pms1dWinCallbackWrap);
      }
      else if (!strcmp (dsm_headers.mainHeader()->name(), ASAS_STR) ||
               !strcmp (dsm_headers.mainHeader()->name(), PCAS_STR)) {

        if (!(int)pms1_win)		// create win if not yet created
          pms1_win = new Pms1dWin(instRC, dsm_msg);

        pms1_win->createAsas (dsm_headers.mainHeader()->name(), 
                              dsm_headers.mainHeader()->locn(),
                              dsm_headers.mainHeader()->dsm_locn(), 
                              Pms1dWinCallbackWrap);
      }
    }
  }
}
 
/* -------------------------------------------------------------------- */
void initPms2Win (Widget top_pane)
 
// Parses the tape header and initializes the pms 2d control window if needed.
{
  int stat;
 
  pms2_win = (Pms2dWin*)0;
 
// Parse the header looking for pms 2d probes.
  for (stat = dsm_headers.mainHeader()->firstDesc(); stat; 
       stat = dsm_headers.mainHeader()->nextDesc()) {

    if (dsm_headers.mainHeader()->pms2Type() ||
        dsm_headers.mainHeader()->greyhType()) {
      if (!(int)pms2_win)             // create pane if not yet created
          pms2_win = new Pms2dWin (top_pane, dsm_msg, 
                                   Pms2dWinTasCallbackWrap, 
                                   Pms2dWinRateCallbackWrap);
      pms2_win->createProbe (dsm_headers.mainHeader()->name(), 
                             dsm_headers.mainHeader()->locn(),
                             dsm_headers.mainHeader()->dsm_locn(), 
                             Pms2dWinProbeCallbackWrap);
    }
  }
}
 
/* -------------------------------------------------------------------- */
void initGenButtons(Widget top_pane)
{
  genButtons = new GenericButtons(top_pane, dsm_msg,
				  GenericButtonToggleButtonCallbackWrap);

}

/* -------------------------------------------------------------------- */
void initMcrWin(Widget top_pane)
 
// Parses the tape header and initializes the MCR control window if needed.
{
  int stat;
 
  mcr_win = (McrWin*)0;
 
// Parse the header looking for the mcr digital output variable.
  for (stat = dsm_headers.mainHeader()->firstDesc(); stat && !(int)mcr_win; 
       stat = dsm_headers.mainHeader()->nextDesc()) {
 
    if (dsm_headers.mainHeader()->digoutType() && 
        !strcmp (dsm_headers.mainHeader()->name(), MCROUT_NAME)) {
       mcr_win = new McrWin (top_pane, dsm_msg, 
                             dsm_headers.mainHeader()->dsm_locn(), 
                             McrWinToggleButtonCallbackWrap);
    }
  }
}

/* -------------------------------------------------------------------- */
void checkMessage ()

// Check for received messages.
{
  while (dsm_msg.readMsg()) {
//printf("discWin:checkMsg: type = %d\n", dsm_msg.type());
    switch (dsm_msg.type()) {
      case ANALOG_MSG:		// update analog state display
        analogWin->displayState(dsm_msg.action(), dsm_msg.location());
        statusWin->displayMsg(dsm_msg.string());  // display message string
        break;

      case DSMTIME_MSG:         // update time from given DSM
        statusWin->displayDSMTime(dsm_msg.name(), dsm_msg.hour(),
                dsm_msg.minute(), dsm_msg.second());
        break;

      case NET_MSG:		// update the network state display
        statusWin->displayNetState(dsm_msg.name(), dsm_msg.action());
        statusWin->displayMsg(dsm_msg.string());  // display message string
        sendInstrumentControlStates();
        break;

      case MCR_MSG:		// update the mcr state display
        mcr_win->displayState(dsm_msg.action(), dsm_msg.value());
        statusWin->displayMsg(dsm_msg.string());  // display message string
        break;

      case PMS1_MSG:
      case STATUS_MSG:
        if (flightWin->flightNumber() == NULL ||
		!(strncmp(dsm_msg.string(), "System time", 11) == 0 &&
		  flightWin->flightNumber()[0] == '0'))
          statusWin->displayMsg (dsm_msg.string());  // display message string
        break;

      case TIME_MSG:             // update the date/time display
        statusWin->displayTime(dsm_msg.year(), dsm_msg.month(),
          dsm_msg.day(), dsm_msg.hour(), dsm_msg.minute(), dsm_msg.second());
#ifdef SOCK_WINDS
        if (!dsm_msg.second())	// send flight number to winds once per minute
          windsFlightMessage();
#endif
        break;

      case LOG_MSG:
        dsm_msg.logMsgToFile(dsm_msg.string());
        break;
    }
  }

// Check for tape task messages.
  if (tape_msg.readMsg()) {
    statusWin->displayDriveState(tape_msg.drive(), tape_msg.action()); 
    statusWin->displayMsg(tape_msg.string());

    // if we started recording, then write out the new 'flightNums' file.
    if (tape_msg.drive() == DRV_0 && tape_msg.action() == RECORDING)
      flightWin->updateFlightNums(dsm_headers.mainHeader()->projectNumber());
  }


}	/* END CHECKMESSAGE */

#ifdef SOCK_WINDS
/* -------------------------------------------------------------------- */
void windsFlightMessage ()

// Sends the flight number to winds.
{
  static char message[20];
  int len;

// Make sure the flight number has been entered before attempting to send it.
  if ((int)flightWin->flightNumber()) {
    sprintf(message,"%d %d %s",UPDATEFLIGHTNUM,TRUE,flightWin->flightNumber());
    len = winds_sock->writeSock(message, strlen(message)+1);
  }
}
#endif
/* -------------------------------------------------------------------- */
void netMessage (int action, char *name, char *msg_str)
 
// Network message handler.
{
  static int firstTime = TRUE;
  static FILE *fp;

  if (firstTime) {
    time_t	ct;
    char        *p, buffer[256];
 
    p = getenv("PROJ_DIR");
    dsm_config.selectByLocn("CTL");
    sprintf(buffer, "%s/hosts/%s/discWin.log", p, dsm_config.hostName());
 
    if ((fp = fopen(buffer, "a")) == NULL)
      fp = stderr;

    ct = time(NULL);
    fprintf(fp, "discWin: reset %s", ctime(&ct));
    firstTime = FALSE;
    }

  fprintf(fp, msg_str);

  if (name == NULL && fp != stderr)
    fclose(fp);

}

/* -------------------------------------------------------------------- */
void AnalogWinToggleButtonCallbackWrap(Widget w, XtPointer client,
                                        XmToggleButtonCallbackStruct *cbs)
{
  analogWin->toggleButtonCallbackProc(w, client, cbs);

}

/* -------------------------------------------------------------------- */
void AnalogWinPushButtonCallbackWrap(Widget w, XtPointer client,
                                      XmPushButtonCallbackStruct *cbs)
{
  analogWin->pushButtonCallbackProc(w, client, cbs);

}
 
/* -------------------------------------------------------------------- */
void AnalogWinWarningCallbackWrap(Widget w, XtPointer client_data,
                               XtPointer call_data)
{
  analogWin->warningCallbackProc(w, client_data, call_data);

}
 
/* -------------------------------------------------------------------- */
void FlightWinCallbackWrap (Widget w, XtPointer client,
                              XmSelectionBoxCallbackStruct *cbs)
{
  int stat;

// If the user flight number entry is valid, send the flight number to the
// tape task, and display it in the project window.

  if (flightWin->readFlightNumber(w, client, cbs)) {
    tape_msg.sendFlightMsg(flightWin->flightNumber());
    dsm_msg.sendFlightMsg(flightWin->flightNumber());
    statusWin->displayFlightNumber(flightWin->flightNumber());
    sendInstrumentControlStates();
#ifdef SOCK_WINDS
    windsFlightMessage();
#endif
  }
}

/* -------------------------------------------------------------------- */
void sendInstrumentControlStates()
{
  // Reset instrument control states.  CAlled upon startup of discWin or
  // if any of the DSM connected state changes.
  if (flightWin->flightNumber() == NULL)
    return;

//printf("Sending Instr states.\n");
  genButtons->sendState();

  if (pms1_win)
    pms1_win->sendState();

  if (pms2_win)
    pms2_win->sendState();

  if (mcr_win)
    mcr_win->sendState();

}

/* -------------------------------------------------------------------- */
void RecordWrap(Widget w, XtPointer client,
                              XmPushButtonCallbackStruct *cbs)
{
  statusWin->recordCB(w, client, cbs);
}

/* -------------------------------------------------------------------- */
void Pms1dWinCallbackWrap (Widget w, XtPointer client,
                           XmToggleButtonCallbackStruct *cbs)
 
// Pms1dWin call back procedure wrapper.
{
  pms1_win->callbackProc (w, client, cbs);
}
 
/* -------------------------------------------------------------------- */
void Pms2dWinTasCallbackWrap (Widget w, XtPointer client,
                              XmToggleButtonCallbackStruct *cbs)
 
// Pms2dWin tas selection call back procedure wrapper.
{
  pms2_win->tasCallbackProc (w, client, cbs);
}
 
/* -------------------------------------------------------------------- */
void Pms2dWinRateCallbackWrap (Widget w, XtPointer client,
                              XmToggleButtonCallbackStruct *cbs)
 
// Pms2dWin record rate selection call back procedure wrapper.
{
  pms2_win->rateCallbackProc (w, client, cbs);
}
 
/* -------------------------------------------------------------------- */
void Pms2dWinProbeCallbackWrap (Widget w, XtPointer client,
                              XmToggleButtonCallbackStruct *cbs)
 
// Pms2dWin record probe selection call back procedure wrapper.
{
  pms2_win->probeCallbackProc (w, client, cbs);
}
 
/* -------------------------------------------------------------------- */
void GenericButtonToggleButtonCallbackWrap (Widget w, XtPointer client,
                              XmToggleButtonCallbackStruct *cbs)
 
// Generic Buttons toggle button call back procedure wrapper.
{
  genButtons->toggleButtonCallbackProc(w, client, cbs);
}

/* -------------------------------------------------------------------- */
void TurnOffMCRheater(int v)            // SIGALARM handler.
{
  mcr_win->turnOffHeater();
}

/* -------------------------------------------------------------------- */
void McrWinToggleButtonCallbackWrap (Widget w, XtPointer client,
	XmToggleButtonCallbackStruct *cbs)
 
// McrWin toggle button call back procedure wrapper.
{
  mcr_win->toggleButtonCallbackProc (w, client, cbs, TurnOffMCRheater);
}

/* -------------------------------------------------------------------- */
void QuitButtonWrap(Widget w, XtPointer client,
			XmPushButtonCallbackStruct *cbs)
{
  comm_win_net.closeNet();
  tape_win_net.closeNet();

  comm_task.killTask();
//  tape_task.killTask();

  statusWin->Quit(w, client, cbs);
  netMessage(0, NULL, "discWin: Quit.\n");

}

/* -------------------------------------------------------------------- */
void AnalogButtonWrap(Widget w, XtPointer client,
			XmPushButtonCallbackStruct *cbs)
{
  analogWin->popUp(w, client, cbs);
}

/* -------------------------------------------------------------------- */
void InstrumentButtonWrap(Widget w, XtPointer client,
			XmPushButtonCallbackStruct *cbs)
{
  instrumentWin->popUp(w, client, cbs);
}

/* -------------------------------------------------------------------- */
void InstrumentWinPushButtonWrap(Widget w, XtPointer client,
			XmPushButtonCallbackStruct *cbs)
{
  instrumentWin->popDown(w, client, cbs);
}

/* -------------------------------------------------------------------- */
void timerCallback(XtPointer data, XtIntervalId *timer_id)
{
  if (flightWin->flightNumber() == NULL)
    flightWin->popupFlightWin();

  if (!comm_win_net.connected())
    {
    fprintf(stderr, "discWin: Lost connection with discComm, quiting\n");
    exit(1);
    }
 
  if (!tape_win_net.connected())
    {
    fprintf(stderr, "discWin: Lost connection with discTape, quiting\n");
    exit(1);
    }

  checkMessage();			// check for received messages

// Restart the timer.
  XtAppAddTimeOut(app, 300, timerCallback, 0);

}

/* END DISCWIN.CC */

/*
multiprint.c -
support multiple print operations from WINDS control panel. Provide the
ability to print a single window, all visible windows, or all setups from
the current folder for either the current time only, from the current time
to EOF, or all time frames in file. Only current time frame is an available
option in real-time mode.

it'd be nice to add the following:

 - print more than one image to a PostScript page 
 - provide control over which time span, which windows
 
users should know that only saved setups will get printed when requesting
the entire folder, and that unsaved setups will be lost on these folder print 
operations

*/

#include <stdio.h>
#include <xview/xview.h>

#include <constants.h>
#include <display.h>
#include <help.h>
#include <printers.h>
#include <socket.h>
#include <proto.h>

int PrintCurrentWindowsForCurrentTimeFrameOnly(),
    PrintCurrentWindowsFromStartTimeThruEOF(),
    PrintCurrentWindowsFromCurrentTimeThruEOF(),

    PrintEntireFolderForCurrentTimeFrameOnly(),
    PrintEntireFolderFromStartTimeThruEOF(),
    PrintEntireFolderFromCurrentTimeThruEOF(),

    PrintWindowForCurrentTimeFrameOnly(),
    PrintWindowFromStartTimeThruEOF(),
    PrintWindowFromCurrentTimeThruEOF();

/* 
MultiPrintOn: TRUE when multiprint operation is underway 
*/
static int MultiPrintOn=FALSE;	
/* 
SetupsPrinted: how many setups have been examined for printing 
*/
static int SetupsPrinted=0;	
/*
NumSetupsToPrint: how many setups are to be examined for printing
*/
static int NumSetupsToPrint=0;
/*
PrintNextTimeFrame: TRUE if next time frame is to be procured for printing
*/
static int PrintNextTimeFrame=FALSE;
/*
WindowRequestedPrint: TRUE if given window has a pending print request
*/
static int WindowRequestedPrint[NUM_DISPLAY_QUADRANTS]=
{FALSE,FALSE,FALSE,FALSE};
/*
RotateSetups: TRUE when current subset of setups is to be rotated to next one for printing
*/
static int RotateSetups=FALSE;

int (*MultiPrintFunc[][NUM_MULTIPRINT_TIME_TYPES])()={ 
	PrintWindowForCurrentTimeFrameOnly,
	PrintWindowFromCurrentTimeThruEOF, 
	PrintWindowFromStartTimeThruEOF, 

	PrintWindowForCurrentTimeFrameOnly,
	PrintWindowFromCurrentTimeThruEOF, 
	PrintWindowFromStartTimeThruEOF, 

	PrintWindowForCurrentTimeFrameOnly,
	PrintWindowFromCurrentTimeThruEOF, 
	PrintWindowFromStartTimeThruEOF, 

	PrintWindowForCurrentTimeFrameOnly,
	PrintWindowFromCurrentTimeThruEOF, 
	PrintWindowFromStartTimeThruEOF, 

	PrintCurrentWindowsForCurrentTimeFrameOnly,
	PrintCurrentWindowsFromCurrentTimeThruEOF,
	PrintCurrentWindowsFromStartTimeThruEOF,

	PrintEntireFolderForCurrentTimeFrameOnly,
	PrintEntireFolderFromCurrentTimeThruEOF,
	PrintEntireFolderFromStartTimeThruEOF
	};

/***************************  GetMultiPrint()  **********************************/

GetMultiPrint()
/*
return the current value of MultiPrintOn
*/
{
 return MultiPrintOn;
}

/***************************  MultiPrint()  **********************************/

MultiPrint(setup,time)
int setup,time;
/*
set MultiPrintOn to TRUE, and invoke the appropriate multi-print function
as indicated by setup to be printed (setup) for which time frame (time)
*/
{
 MultiPrintOn=TRUE;
 (*MultiPrintFunc[setup][time])(setup);
}

/*********************** SetNumSetupsToPrint() ********************************/

SetNumSetupsToPrint(num)
int num;
/*
set value of NumSetupsToPrint, number of setups to be printed to num
*/
{
 NumSetupsToPrint=num;
}

/*********************** GetNumSetupsToPrint() ********************************/

GetNumSetupsToPrint()
/*
return value of NumSetupsToPrint, number of setups to be printed
*/
{
 return NumSetupsToPrint;
}

/************************* SetSetupsPrinted() **********************************/

SetSetupsPrinted(num)
int num;
/*
set value of SetupsPrinted, number of setups printed to num
*/
{
 SetupsPrinted=num;
}

/************************* GetSetupsPrinted() **********************************/

GetSetupsPrinted()
/*
return value of SetupsPrinted, number of setups printed to num
*/
{
 return SetupsPrinted;
}

/************************** GetPrintNextTimeFrame() ****************************/

GetPrintNextTimeFrame()
/*
return value of PrintNextTimeFrame, whether additional time frames are to
be examined for printing
*/
{
 return PrintNextTimeFrame;
}

/************************* SetPrintNextTimeFrame() *****************************/

SetPrintNextTimeFrame(print)
int print;
/*
set value of PrintNextTimeFrame, whether additional time frames are to
be examined for printing
*/
{
 PrintNextTimeFrame=print;
}

/*************************** SetRotateSetups() ********************************/

SetRotateSetups(rotate)
int rotate;
/*
set value of RotateSetups, whether current setups are to be cycled forward to
next subset for printing
*/
{
 RotateSetups=rotate;
}

/*************************** GetRotateSetups() *********************************/

GetRotateSetups()
/*
return value of RotateSetups, whether current setups are to be cycled forward to
next subset for printing
*/
{
 return RotateSetups;
}

/************************** SetWindowRequestedPrint() **************************/

SetWindowRequestedPrint(quadrant,print)
int quadrant,print;
/* 
set value of WindowRequestedPrint, whether window in quadrant is to be printed
*/
{
 WindowRequestedPrint[quadrant]=print;
}

/************************* GetWindowRequestedPrint() ***************************/

GetWindowRequestedPrint(quadrant)
/* 
return value of WindowRequestedPrint, whether window in quadrant is to be printed
*/
{
 return WindowRequestedPrint[quadrant];
}

/************************** SetAllWindowsRequestPrint() ************************/

SetAllWindowsRequestPrint(print)
int print;
/*
set value WindowRequestedPrint for all windows to print
*/
{
int quadrant;

 for (quadrant=0; quadrant<NUM_DISPLAY_QUADRANTS; quadrant++)
  SetWindowRequestedPrint(quadrant,print);
}

/************************* RotateSetupsIfNeeded() ******************************/

RotateSetupsIfNeeded(whence)
int whence;
/*
cycle currently visisble setups forward to next subset, if called for
*/
{
int quadrant;
/*
execute one more cycle through setups to get to next subset
*/
 if (GetRotateSetups()) {
  fill_displays(whence);
/*
fill_displays() results in a hidden window, which won't get unhidden normally
until *after* GetImage() is called; this would result in an X drawing error.
Preclude this by unhiding window(s) now. Then, fill in newly configured
window type with UpdateDisplay(), which normally wouldn't get called, again,  
until after call to GetImage().
*/
  for (quadrant=0; quadrant<NUM_DISPLAY_QUADRANTS; quadrant++) {
   xv_set(GetWINDSCanvas(quadrant),XV_SHOW,TRUE,NULL);
  }
 }
}

/**************************** TurnMultiPrintOff() **************************/

TurnMultiPrintOff()
/*
reset all parameters to default condition so as to indicate that multiprinting
is now terminated
*/
{
 MultiPrintOn=FALSE;
 SetPrintNextTimeFrame(FALSE);
 SetAllWindowsRequestPrint(FALSE);
 SetNumSetupsToPrint(0);
 SetSetupsPrinted(0);
 SetRotateSetups(FALSE);
/*
also reset control and op panel buttons to active condition
*/
 ActivateControlPanel(TRUE);
 ActivateOpPanels(TRUE);
}

/****************** PrintWindowForCurrentTimeFrameOnly() ***********************/

PrintWindowForCurrentTimeFrameOnly(quadrant)
int quadrant;
/*
the name of the function says it all
*/
{
/**
(void)fprintf(stderr,"PrintWindowForCurrentTimeFrameOnly(%d)\n",quadrant);
***/
 SetNumSetupsToPrint(1);
 SetWindowRequestedPrint(quadrant,TRUE);
 SetRotateSetups(FALSE);
 SetSetupsPrinted(0);
 SetPrintNextTimeFrame(FALSE);
}

/********************** PrintWindowFromStartTimeThruEOF() **********************/

PrintWindowFromStartTimeThruEOF(quadrant)
int quadrant;
/*
the name of the function says it all
*/
{
/***
(void)fprintf(stderr,"PrintWindowFromStartTimeThruEOF(%d)\n",quadrant);
***/
 if (!post_processing_mode())
  return;
 ResetInputProc();
 SetNumSetupsToPrint(1);
 SetWindowRequestedPrint(quadrant,TRUE);
 SetRotateSetups(FALSE);
 SetSetupsPrinted(0);
 SetPrintNextTimeFrame(TRUE);
}

/********************* PrintWindowFromCurrentTimeThruEOF() *********************/

PrintWindowFromCurrentTimeThruEOF(quadrant)
int quadrant;
/*
the name of the function says it all
*/
{
/***
(void)fprintf(stderr,"PrintWindowFromCurrentTimeThruEOF(%d)\n",quadrant);
***/
 if (!post_processing_mode())
  return;
 SetWindowRequestedPrint(quadrant,TRUE);
 SetNumSetupsToPrint(1);
 SetRotateSetups(FALSE);
 SetSetupsPrinted(0);
 SetPrintNextTimeFrame(TRUE);
}

/*************** PrintCurrentWindowsForCurrentTimeFrameOnly() ******************/

PrintCurrentWindowsForCurrentTimeFrameOnly()
/*
the name of the function says it all
*/
{
/***
(void)fprintf(stderr,"PrintCurrentWindowsForCurrentTimeFrameOnly\n");
***/
 SetNumSetupsToPrint(get_num_displays());
 SetRotateSetups(FALSE);
 SetAllWindowsRequestPrint(TRUE);
 SetSetupsPrinted(0);
 SetPrintNextTimeFrame(FALSE);
}

/****************** PrintCurrentWindowsFromStartTimeThruEOF() ******************/

PrintCurrentWindowsFromStartTimeThruEOF()
/*
the name of the function says it all
*/
{
/***
(void)fprintf(stderr,"PrintCurrentWindowsFromStartTimeThruEOF\n");
***/
 if (!post_processing_mode())
  return;
 ResetInputProc();
 SetNumSetupsToPrint(get_num_displays());
 SetSetupsPrinted(0);
 SetAllWindowsRequestPrint(TRUE);
 SetRotateSetups(FALSE);
 SetPrintNextTimeFrame(TRUE);
}

/******************* PrintCurrentWindowsFromCurrentTimeThruEOF() ***************/

PrintCurrentWindowsFromCurrentTimeThruEOF()
/*
the name of the function says it all
*/
{
/***
(void)fprintf(stderr,"PrintCurrentWindowsFromCurrentTimeThruEOF\n");
***/
 if (!post_processing_mode())
  return;
 SetNumSetupsToPrint(get_num_displays());
 SetSetupsPrinted(0);
 SetAllWindowsRequestPrint(TRUE);
 SetRotateSetups(FALSE);
 SetPrintNextTimeFrame(TRUE);
}

/******************* PrintEntireFolderForCurrentTimeFrameOnly() ****************/

PrintEntireFolderForCurrentTimeFrameOnly()
/*
the name of the function says it all
*/
{
/***
(void)fprintf(stderr,"PrintEntireFolderForCurrentTimeFrameOnly\n");
***/
 SetRotateSetups(TRUE);
 RotateSetupsIfNeeded(0); 
 SetNumSetupsToPrint(num_valid_setups());
 SetSetupsPrinted(0);
 SetAllWindowsRequestPrint(TRUE);
 SetPrintNextTimeFrame(FALSE);
}

/****************** PrintEntireFolderFromStartTimeThruEOF() ********************/

PrintEntireFolderFromStartTimeThruEOF()
/*
the name of the function says it all
*/
{
/***
(void)fprintf(stderr,"PrintEntireFolderFromStartTimeThruEOF\n");
***/
 if (!post_processing_mode())
  return;
 ResetInputProc();
 SetRotateSetups(TRUE);
 RotateSetupsIfNeeded(0); 
 SetNumSetupsToPrint(num_valid_setups());
 SetSetupsPrinted(0);
 SetAllWindowsRequestPrint(TRUE);
 SetPrintNextTimeFrame(TRUE);
}

/******************* PrintEntireFolderFromCurrentTimeThruEOF() *****************/

PrintEntireFolderFromCurrentTimeThruEOF()
/*
the name of the function says it all
*/
{
/***
(void)fprintf(stderr,"PrintEntireFolderFromCurrentTimeThruEOF\n");
***/
 if (!post_processing_mode())
  return;
 SetRotateSetups(TRUE);
 RotateSetupsIfNeeded(0); 
 SetNumSetupsToPrint(num_valid_setups());
 SetSetupsPrinted(0);
 SetAllWindowsRequestPrint(TRUE);
 SetPrintNextTimeFrame(TRUE);
}

/************************ CheckForMultiPrints() ********************************/

CheckForMultiPrints(quadrant)
int quadrant;
/*
if window indicated by quadrant has a pending print request, first verify that
it is a window that *should* be printed, as function of its window-type; if
so, set flag to print it in current image-mode; also, update that display
so current data is showing; regardless, increment number of setups "printed".
*/
{
 if (GetWindowRequestedPrint(quadrant)) {
  if (  GetWINDSWindowType(quadrant)==DISPTYPE_XYPLOT 
     || GetWINDSWindowType(quadrant)==DISPTYPE_PMS1D 
     || GetWINDSWindowType(quadrant)==WTYPE_SKEWT 
     || GetWINDSWindowType(quadrant)==DISPTYPE_PMS2D 
     || GetWINDSWindowType(quadrant)==DISPTYPE_FIXEDFORMAT 
     ) {
   if (GetCurrentImageMode()== BWPRINT) {
    SetWINDSSnapshot(quadrant,TRUE);
   } else {
    SetWINDSColorSnapshot(quadrant,TRUE);
   }
/*
to update displays immediately after they are rotated or time is framed is a 
mistake because the time stamp (upper right corner of plot) will reflect 
previous time after the time frame is changed, since variables current_hr, 
current_minute, current_sec are used to draw this and they don't get updated 
until start of next catchalrm() execution. Fix this by updating displays now, 
after these variables have been updated.
*/
   UpdateDisplay(quadrant);
  } 
  SetSetupsPrinted(GetSetupsPrinted()+1);
 }
}

/********************* AdjustForNextMultiPrint() *******************************/

AdjustForNextMultiPrint()
/*
take steps as called to prepare for next round of printing, else, if done,
turn multi-printing off
*/
{
 if (GetNumSetupsToPrint()) {

  if (GetSetupsPrinted()>=GetNumSetupsToPrint()) {
/*
we've looked at all necessary setups for printing
*/
   if (GetPrintNextTimeFrame()) {
/*
we must be in post-processing mode
*/
    if (FrameForward()==NO_SHMEM_REFILLED) { 
/*
then EOF was reached last time around
*/
     TurnMultiPrintOff();
    } else {
/*
ready to start next time frame's worth of setups
*/
     SetSetupsPrinted(0);
     RotateSetupsIfNeeded(get_num_displays());
     if (GetWINDSElapsedTime() < get_time_frame_size())  {
/*
(*Assume* that this means that EOF has been reached...)

EOF was reached on this time around, with a less than "full" frame. Next
FrameForward will actually adjust frame to a full one, doing so by starting
a full frame backwards from EOF, which is nice for viewing the displays
but confusing and redundant in printing the end of data view twice with 
differing start times. Set a flag to preclude printing the *next* time 
frame, while allowing print processing loop before this to execute final set 
of images.
*/
      SetPrintNextTimeFrame(FALSE);
     }
    }
   } else {
/*
done
*/
    TurnMultiPrintOff();
   } 
  } else {
   RotateSetupsIfNeeded(get_num_displays());
  }
 }
}

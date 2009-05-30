/*
-------------------------------------------------------------------------
OBJECT NAME:	xpms2d.cc

FULL NAME:	X PMS 2D

ENTRY POINTS:	main()
		ErrorMsg()

STATIC FNS:	Initialize()
		ProcessArgs()

DESCRIPTION:	See man page.

INPUT:		Command line options

AUTHOR:		cjw@ucar.edu

COPYRIGHT:      University Corporation for Atmospheric Research, 1997
-------------------------------------------------------------------------
*/

#include <unistd.h>

#include "define.h"
#include "fbr.h"

#include <raf/Application.h>
#include <CanvasWindow.h>
#include <ControlWindow.h>
#include <Colors.h>
#include <raf/Cursor.h>
#include <DataFile.h>
#include <FileMgr.h>
#include <Magnify.h>
#include <MagnifyWindow.h>
#include <MagnifyCanvas.h>
#include <MainCanvas.h>
#include <raf/Printer.h>
#include <raf/XFonts.h>
#include <raf/XPen.h>
#include <raf/XmError.h>
#include <raf/XmFile.h>
#include <raf/XmWarn.h>

Application	*application;
CanvasWindow	*canvasWindow;
ControlWindow	*controlWindow;
Magnify		*mag;
MagnifyWindow	*magWindow;
MagnifyCanvas	*magPlot = NULL;
MainCanvas	*mainPlot = NULL;
XCursor		cursor;
Printer		*printerSetup;
Colors		*color;

FileManager	fileMgr;

XFonts		*fonts;
XPen		*pen;
XmFile		*fileSel;


static void  Initialize();
static void  ProcessArgs(char *argv[]);


/* --------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  application	= new Application("XmPMS2D", &argc, argv, fallback_resources);
  canvasWindow	= new CanvasWindow(application->Shell());
  controlWindow	= new ControlWindow(application->Shell());
  magWindow	= new MagnifyWindow(application->Shell());

  printerSetup	= new Printer(application->Shell());
  mag		= new Magnify();

  Initialize();
  ProcessArgs(argv);

  fileSel = new XmFile(application->Shell());

  canvasWindow->PopUp();
  controlWindow->PopUp();

  fonts	= new XFonts(application->Shell());
  pen	= new XPen(canvasWindow->DrawingArea());
  color	= new Colors(canvasWindow->DrawingArea());
  mainPlot	= new MainCanvas(canvasWindow->DrawingArea());

  SetSampleArea();

  XtAppMainLoop(application->Context());

  return(0);

}	/* END MAIN */

/* --------------------------------------------------------------------- */
static void Initialize()
{
  char	*p;

  DataChanged		= true;
  UTCseconds		= false;

  if ((p = (char *)getenv("RAW_DATA_DIR")) == NULL)
    p = (char *)getenv("DATA_DIR");

  if (p != NULL)
    {
    strcpy(DataPath, p);
    strcat(DataPath, "/*2d*");
    }
  else
    strcpy(DataPath, "*2d*");

  strcpy(pngPath, "*.png");
  strcpy(psPath, "*.ps");

}	/* END INITIALIZE */

/* --------------------------------------------------------------------- */
static void ProcessArgs(char **argv)
{
  while (*++argv)
    if ((*argv)[0] == '-')
      switch ((*argv)[1])
        {
        }
    else
      {
      if (access(*argv, R_OK) == 0)
        fileMgr.NewFile(*argv);
      }

}	/* END PROCESSARGS */

/* --------------------------------------------------------------------- */
void ErrorMsg(const char msg[])
{
  new XmError(application->Shell(), msg);

}	/* END ERRORMSG */

/* --------------------------------------------------------------------- */
void WarnMsg(const char msg[], XtCallbackProc okCB, XtCallbackProc cancelCB)
{
  new XmWarn(application->Shell(), msg, okCB, cancelCB);

}	/* END WARNMSG */

/* --------------------------------------------------------------------- */
void FlushEvents()
{
  application->FlushEvents();

}	/* END FLUSHEVENTS */

/* END XPMS2D.CC */

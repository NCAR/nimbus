/*
-------------------------------------------------------------------------
OBJECT NAME:	xhd.c

FULL NAME:	X Header Dump

ENTRY POINTS:	main()


STATIC FNS:	none

DESCRIPTION:	

REFERENCES:	

REFERENCED BY:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1996
-------------------------------------------------------------------------
*/

#include <stdlib.h>

#include <Xm/Protocols.h>
#include <Xm/Text.h>

#define APP_NAME    "xhd"
#define APP_CLASS   "XmHdrDmp"

#include "fbr.h"

char	fileName[512], buffer[1024];
Widget	hdText, HDwindow;

void	CreateHDwindow(Widget), CreateErrorBox(Widget),
	CreateFileSelectionBox(Widget),
	Quit(Widget, XtPointer, XtPointer),
	HeaderDump(Widget, XtPointer, XtPointer);

int fd;
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


/* -------------------------------------------------------------------- */
static char *ac_names[] = {
	"",
	"C130_N130AR",
	"KingAir_N312D",
	"P3_NRL-P3",
	"",
	"",
	"",
	"Saberliner_N307D",
	"Electra_N308D",
	"Sailplane_N9929J"
	};

char * getAircraftName(int num)
{
  return ac_names[num/100];
}

/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  Widget	AppShell, Shell000;
  XtAppContext	context;
  Arg		args[8];
  Cardinal	n;
  Atom		WM_DELETE_WINDOW;

  int		pnum = atoi(argv[1]);

  if (argc < 2)
  {
    printf("Usage: xhd proj_num | header_filename\n");
    return(1);
  }

  n = 0;
  AppShell = XtAppInitialize(&context, APP_CLASS, NULL, 0, &argc, argv, fallback_resources, NULL, 0);
  n = 0;
  Shell000 = XtCreatePopupShell("hdShell", topLevelShellWidgetClass, AppShell, args, n);

  CreateHDwindow(Shell000);
  CreateErrorBox(AppShell);
  CreateFileSelectionBox(AppShell);

  WM_DELETE_WINDOW = XmInternAtom(XtDisplay(AppShell), "WM_DELETE_WINDOW", False);
  XmAddWMProtocolCallback(Shell000, WM_DELETE_WINDOW, Quit, NULL);

  if (pnum > 99)
  {
    char * p = getenv("PROJ_DIR");

    if (p == NULL)
    {
      fprintf(stderr, "xhd: environment variable PROJ_DIR undefined.\n");
      return(1);
    }

    sprintf(fileName, "%s/%d/%s/header", p, pnum, getAircraftName(pnum));
  }
  else
    strcpy(fileName, argv[1]);


  HeaderDump(NULL, NULL, NULL);
  XtAppMainLoop(context);

  return(0);

}	/* END MAIN */

/* END XHD.C */

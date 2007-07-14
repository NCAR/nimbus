#include <Xm/Xm.h>
#include <stdio.h>
#include <raf/header.h>

Widget	CreateMainWindow(Widget Shell000);


void	Initialize(int argc, char *argv[]),
	OpenNewFile(Widget w, XtPointer client, XtPointer call),
	OpenNewFile_OK(Widget w, XtPointer client, XmFileSelectionBoxCallbackStruct *call),
	SaveFile(Widget w, XtPointer client, XtPointer call),
	Quit(Widget w, XtPointer client, XtPointer call);


/*
-------------------------------------------------------------------------
OBJECT NAME:	mcrtd.h

FULL NAME:	Include File to Include the Include Files

DESCRIPTION:	
-------------------------------------------------------------------------
*/

#ifndef DEFINE_H
#define DEFINE_H

#define _NO_PROTO

#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <Xm/Xm.h>

#ifndef NULL
#define NULL		(0)
#endif
 
#ifndef OK
#define OK		(0)
#endif

#ifndef ERR
#define ERR		(-1)
#endif
 
#define COMMENT		'#'	/* Comment character for textfiles  */
 
#define SecondsSinceMidnite(t)	(t[0] * 3600 + t[1] * 60 + t[2])

#define HdrBlkTimeToSeconds(hdr) \
	(((struct Mcr_rec *)hdr)->second + \
	(((struct Mcr_rec *)hdr)->minute * 60) + \
	(((struct Mcr_rec *)hdr)->hour * 3600))


#define BUFFSIZE	2048
#define PATH_LEN	256
#define NAMELEN		20

#define MAX_DATASETS	7

#define TITLESIZE	80

#define MAX_FONTS	6


/* Values for "HandleError"		*/
enum { RETURN, EXIT, IRET };


extern char buffer[], *outFile, DataPath[], UserPath[];

void	PrintSave(Widget, XtPointer, XtPointer),
	Quit(Widget, XtPointer, XtPointer);

void	CanvasExpose(Widget, XtPointer, XtPointer),
	CanvasResize(Widget, XtPointer, XtPointer);

void	SetChannel(Widget, XtPointer, XtPointer),
	Record(Widget, XtPointer, XtPointer),
	Record_Go(Widget, XtPointer, XtPointer),
	PrintSave(Widget, XtPointer, XtPointer),
	Freeze(Widget, XtPointer, XtPointer),
	SetStartVoltage(Widget, XtPointer, XtPointer),
	SetEndVoltage(Widget, XtPointer, XtPointer),
	Quit(Widget, XtPointer, XtPointer);

void	DismissWindow(Widget, XtPointer, XtPointer),
	DestroyWindow(Widget, XtPointer, XtPointer);

#endif

/* END MCRTD.H */

/*
-------------------------------------------------------------------------
OBJECT NAME:	define.h

FULL NAME:	Include File to Include the Include Files

DESCRIPTION:	
-------------------------------------------------------------------------
*/

#ifndef DEFINE_H
#define DEFINE_H

#define _NO_PROTO

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <netinet/in.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <Xm/Xm.h>

#define CONSTANTS_H
#define NAMLEN          13
#include "nimbus.h"

#define BUFFSIZE	2048
#define PATH_LEN	1024

#define	SPACE		' '

#define MAX_VARIABLES	2048
#define MAXDATASETS	5

#define TITLESIZE	128
#define LEGENDSIZE	(TITLESIZE<<1)

#define NUMTICMARKS	10

/* Defines for 'Mode'	*/
#define COUNTS		0
#define VOLTS		1
#define ENGINEERING	2


/* Values for "axies_to_scale"		*/
#define SCALE_XAXIS	0x01
#define SCALE_YAXIS	0x02

/* Values for "HandleError"		*/
#define RETURN		0	/* Continue after displaying error	*/
#define EXIT		1	/* Exit after displaying error		*/
#define IRET		2	/* if (interactive) return else exit(1)	*/

/* Values for reading Font Resources	*/
#define XtNtitleFont		"titleFont"
#define XtCTitleFont		"Titlefont"
#define XtNsubtitleFont		"subtitleFont"
#define XtCSubtitleFont		"Subtitlefont"
#define XtNticFont		"ticFont"
#define XtCTicFont		"Ticfont"



/* Struct for reading font resources from resource file	*/
typedef struct _insRec
	{
	char	*titleFont;
	char	*subtitleFont;
	char	*ticFont;
	} instanceRec;

typedef struct
	{
	char    name[NAMLEN];
	int     SampleRate;
	int     SRstart;
	int     nPoints;
	XPoint	*pts;
	int	buffIndex;
	} VARTBL;


#include "extern.h"

#endif

/* END DEFINE.H */

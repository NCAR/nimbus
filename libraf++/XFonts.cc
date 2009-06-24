/*
-------------------------------------------------------------------------
OBJECT NAME:	XFonts.cc

FULL NAME:	

ENTRY POINTS:	

STATIC FNS:	

DESCRIPTION:	

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1997
-------------------------------------------------------------------------
*/

#include "XFonts.h"


/* Values for reading Font Resources    */
char XtNfont24[] = "font24";
char XtCFont24[] = "Font24";
char XtNfont18[] = "font18";
char XtCFont18[] = "Font18";
char XtNfont14[] = "font14";
char XtCFont14[] = "Font14";
char XtNfont12[] = "font12";
char XtCFont12[] = "Font12";
char XtNfont10[] = "font10";
char XtCFont10[] = "Font10";
 
typedef struct _insRec
        {
        const char    *font24;
        const char    *font18;
        const char    *font14;
        const char    *font12;
        const char    *font10;
        } instanceRec;

static XtResource resources[] = {
	{XtNfont24, XtCFont24, XtRString, sizeof(char *),
	 XtOffsetOf(instanceRec, font24), XtRString, NULL},

	{XtNfont18, XtCFont18, XtRString, sizeof(char *),
	 XtOffsetOf(instanceRec, font18), XtRString, NULL},

	{XtNfont14, XtCFont14, XtRString, sizeof(char *),
	 XtOffsetOf(instanceRec, font14), XtRString, NULL},

	{XtNfont12, XtCFont12, XtRString, sizeof(char *),
	 XtOffsetOf(instanceRec, font12), XtRString, NULL},

	{XtNfont10, XtCFont10, XtRString, sizeof(char *),
	 XtOffsetOf(instanceRec, font10), XtRString, NULL},
	};

/* -------------------------------------------------------------------- */
XFonts::XFonts(Widget shell)
{
  instanceRec	iv;
  Display	*dpy = XtDisplay(shell);

  XtGetApplicationResources(shell, (caddr_t)&iv, resources,
                            XtNumber(resources), NULL, 0);

  if ((fontInfo[Point24] = XLoadQueryFont(dpy, iv.font24)) == NULL ||
      (fontInfo[Point18] = XLoadQueryFont(dpy, iv.font18)) == NULL ||
      (fontInfo[Point14] = XLoadQueryFont(dpy, iv.font14)) == NULL ||
      (fontInfo[Point12] = XLoadQueryFont(dpy, iv.font12)) == NULL ||
      (fontInfo[Point10] = XLoadQueryFont(dpy, iv.font10)) == NULL)
    std::cerr << "XFonts: can't load font.\n";

}	/* END CONSTRUCTOR */

/* END XFONTS.CC */

/*
-------------------------------------------------------------------------
OBJECT NAME:	Colors.cc

FULL NAME:	X Color values.

ENTRY POINTS:	Colors()

STATIC FNS:		

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1997
-------------------------------------------------------------------------
*/

#include "Colors.h"

struct {
        char    *name;
        float   rgb[3];         /* For PostScript       */
        unsigned long   pixel;
} colorInfo[] = {
        { "Black", { 0, 0, 0 }, 0 },
        { "blue", { 0, 0, 1 }, 0 },
        { "maroon", { 0.69, 0.1882, 0.3765 }, 0 },
        { "green", { 0, 1, 0 }, 0 },
        { "red", { 1, 0, 0 }, 0 },
        { "violet", { 0.9333, 0.5098, 0.9333 }, 0 },
        { "yellow", { 1, 1, 0 }, 0 },
        { "purple", { 0.6275, 0.1255, 0.9412 }, 0 },
        { NULL, { 0, 0, 0 }, 0 } };

/* -------------------------------------------------------------------- */
Colors::Colors(const Widget w)
{
  int		i = 5, defaultDepth;
  Colormap	defaultCMap;
  Display	*dpy;
  int		screenNum;
  XVisualInfo	visualInfo;
  XColor	screenDef, exactDef;

  Color = true;

  dpy = XtDisplay(w);
  screenNum = XScreenNumberOfScreen(XtScreen(w));
  defaultDepth = DefaultDepth(dpy, screenNum);

  if (defaultDepth == 1)
    {
    Color = false;
    return;
    }

  while (!XMatchVisualInfo(dpy, screenNum, defaultDepth, i, &visualInfo))
    --i;

  if (i < StaticColor)
    {
    Color = false;
    return;
    }


  /* OK, we have a color scree, allocate colors.
   */
  defaultCMap = DefaultColormap(dpy, screenNum);

  for (i = 0; colorInfo[i].name; ++i)
    {
    if (!XAllocNamedColor(
         dpy, defaultCMap, colorInfo[i].name, &screenDef, &exactDef))
      {
      cerr << "Color.cc: Can't allocate color, reverting to B&W.\n";
      Color = false;
      return;
      }

    colorInfo[i].pixel = exactDef.pixel;
    }

}	/* END CONSTRUCTOR */

/* -------------------------------------------------------------------- */
void Colors::ResetColors()
{
  colorIndex = 0;
}
 
/* -------------------------------------------------------------------- */
unsigned long Colors::GetColor(int indx)
{
  return(colorInfo[indx].pixel);
}

/* -------------------------------------------------------------------- */
unsigned long Colors::NextColor()
{
  return(colorInfo[++colorIndex].pixel);
}

/* -------------------------------------------------------------------- */
float *Colors::GetColorPS(int indx)
{
  return(colorInfo[indx].rgb);
}

/* -------------------------------------------------------------------- */
float *Colors::NextColorPS()
{
  return(GetColorPS(++colorIndex));
}

/* -------------------------------------------------------------------- */
float *Colors::CurrentColorPS()
{
  return(GetColorPS(colorIndex));
}

/* END COLORS.CC */

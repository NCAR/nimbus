/*
-------------------------------------------------------------------------
OBJECT NAME:	Xplot.c

FULL NAME:	Plot Graph in X window

ENTRY POINTS:	PlotData()

STATIC FNS:	set_clipping()
		do_ytics()
		PlotLines()

DESCRIPTION:	This is the Expose event procedure to regenerate the
		whole image.

INPUT:		none

OUTPUT:		none

COPYRIGHT:	University Corporation for Atmospheric Research, 1994-9
-------------------------------------------------------------------------
*/

#include "define.h"
#include "header.h"
#include "Canvas.h"
#include "Colors.h"
#include "PixMap.h"
#include "XPen.h"
#include "XFonts.h"

#include <X11/Xutil.h>
#include <Xm/DrawingA.h>

extern Canvas	*canvas;

/* Y offset from Graph Origin   */
#define YLEGEND(s)      (canvas->Height() - 10 - (s * 12))

static int	HD, VD, LV, RV, TH, BH, TICLEN, TITLE_OFFSET, SUBTITLE_OFFSET,
		XLABEL_OFFSET, YLABEL_OFFSET, Y_TIC_LABEL_OFFSET,
		X_TIC_LABEL_OFFSET, XLEGEND_START, XLEGEND_END, XLEGEND_TEXT;


static XPen	*blackPen, *colorPen;
static XFonts	*fonts;
static Colors	*colors;

static XImage	*im_out = NULL, *im_in;
static PixMap	*in_pm, *out_pm;

static int	pixLen, charSize;

static void do_ytics();
static void set_clipping(Display *dpy, GC gc);

void	PlotLines(), SetYlabelPixmap();
void	MakeTicLabel(char buffer[], double diff, double value);
NR_TYPE	GetData(int set, int indx);



/* -------------------------------------------------------------------- */
void ResizePlotWindow(Widget w, XtPointer client, XtPointer call)
{
  canvas->Resize();

 
  /* NOTE: In an X window the origin is the upper left corner with X
   *  going accross, and Y going down.  I have also noticed that
   *  there are 83 pixels/inch across and 82 pixels/inch down.
   */
  HD = (int)(canvas->Width() * 0.78);
  VD = (int)(canvas->Height() * 0.676);
  LV = (int)(canvas->Width() * 0.15);
  TH = (int)(canvas->Height() * 0.15);
 
  RV = LV + HD;
  BH = TH + VD;
 
  TICLEN = HD > 400 ? 10 : 5;
  TITLE_OFFSET    = (int)(TH * 0.5);
  SUBTITLE_OFFSET = (int)(TH * 0.75);
  XLABEL_OFFSET   = BH + 50;
  YLABEL_OFFSET   = 20;
  Y_TIC_LABEL_OFFSET = 5;
  X_TIC_LABEL_OFFSET = 20;
 
  XLEGEND_START   = 12;
  XLEGEND_END     = 50;
  XLEGEND_TEXT    = 55;
 
}	/* END RESETWINDOWSIZE */

/* -------------------------------------------------------------------- */
void PlotData(Widget w, XtPointer client, XtPointer call)
{
  int	offset;
  XmDrawingAreaCallbackStruct *cb = (XmDrawingAreaCallbackStruct *)call;

  static bool	firstTime = true;


  /* If there are more Expose Events on the queue, then ignore this
   * one.
   */
  if (cb && cb->event && ((XExposeEvent *)cb->event)->count > 0)
    return;


  /* Set default Graphics Context stuff and get the GC
   */
  if (firstTime)
    {
    blackPen = new XPen(canvas->Wdgt());
    colorPen = new XPen(canvas->Wdgt());
    fonts = new XFonts(canvas->Wdgt());
    colors = new Colors(canvas->Wdgt());

    in_pm = new PixMap(canvas->Wdgt(), 300, 30);
    out_pm = new PixMap(canvas->Wdgt(), 30, 300);

    ResizePlotWindow(NULL, NULL, NULL);
    ToggleUnits(NULL, (XtPointer)VOLTS, NULL);

    blackPen->SetLineWidth(1);

    firstTime = false;
    }


  /* Set clipping to whole window */
  blackPen->ClearClipping();
  canvas->Clear();

  blackPen->DrawRectangle(canvas->Surface(), LV, TH, HD, VD);

  /* Display title, then do subtitle and ylabel.
   */
  if (title.length() > 0)
    {
    blackPen->SetFont(fonts->Font(XFonts::Point24));
    offset = LV + (HD>>1) - (fonts->StringWidth(XFonts::Point24, title)>>1);
    blackPen->DrawText(canvas->Surface(), offset, TITLE_OFFSET, title);
    }

  blackPen->SetFont(fonts->Font(XFonts::Point18));

  if (subtitle.length() > 0)
    {
    offset = LV + (HD>>1) - (fonts->StringWidth(XFonts::Point18, subtitle)>>1);
    blackPen->DrawText(canvas->Surface(), offset, SUBTITLE_OFFSET, subtitle);
    }

  if (ylabel.length() > 0)
    {

    /* Write out the rotated text
     */
    offset = TH + (VD >> 1) - (pixLen >> 1);

    blackPen->PutImage(canvas->Surface(), im_out, 0, 0, YLABEL_OFFSET,
			offset, charSize, pixLen);
    }

  if (xlabel.length() > 0)
    {
    int	offset;

    offset = LV + (HD>>1) - (fonts->StringWidth(XFonts::Point18, xlabel)>>1);
    blackPen->DrawText(canvas->Surface(), offset, XLABEL_OFFSET, xlabel);
    }


  blackPen->SetFont(fonts->Font(XFonts::Point12));
  do_ytics();

  PlotLines();

  canvas->ExposeAll();

}	/* END PLOTDATA */

/* -------------------------------------------------------------------- */
void PlotLines()
{
  int		i, n;
  NR_TYPE	xscale, yscale, halfSecond;

  if (nVariables == 0)
    return;


  /* Draw X-axis tic marks and #'s
   */
{
  int	offset, ticlen, nsex, nh, nm, ns;

  ticlen = Grid ? TH : BH-TICLEN;

  for (i = 0; i <= numtics; ++i)
    {
    if ((offset = (int)(LV + ((float)HD / numtics) * i)) > RV)
      offset = RV;

    blackPen->DrawLine(canvas->Surface(), offset, BH, offset, ticlen);

    if (i % 2)
      continue;

    nsex = i * NumberSeconds / numtics;

    nh = nsex / 3600;
    nm = (nsex - (nh * 3600)) / 60;
    ns = nsex - (nh * 3600) - (nm * 60);

    sprintf(buffer, "%02d:%02d:%02d", nh, nm, ns);
    string tmp = buffer;

    offset -= fonts->StringWidth(XFonts::Point14, tmp) >> 1;
    blackPen->DrawText(canvas->Surface(), offset, BH+X_TIC_LABEL_OFFSET, tmp);
    }
}

  /* Plot data
   */
  colors->ResetColors();

  for (CurrentDataSet = 0; CurrentDataSet < NumberDataSets; ++CurrentDataSet)
    {
    // Erase old points.

    colorPen->SetColor(colors->NextColor());
/*
    colorPen->DrawLines(canvas->Surface(), Variable[CurrentDataSet].pts,
		Variable[CurrentDataSet].nPoints);
*/

    xscale = (NR_TYPE)HD / Variable[CurrentDataSet].nPoints;
    yscale = (NR_TYPE)VD / (ymax - ymin);

    /* Center 1hz data.
     */
    if (Variable[CurrentDataSet].nPoints == NumberSeconds)
      halfSecond = HD / NumberSeconds / 2;
    else
      halfSecond = 0.0;

    /* Calculate points for lines		*/
    for (i = 0; i < Variable[CurrentDataSet].nPoints; ++i)
      {
      Variable[CurrentDataSet].pts[i].x = (int)(LV + (xscale * i) + halfSecond);
      Variable[CurrentDataSet].pts[i].y = (int)(BH - (yscale * (GetData(CurrentDataSet, i) - ymin)));
      }

    /* Display lines			*/
    colorPen->SetClipping(LV, TH, HD, VD);
    colorPen->DrawLines(canvas->Surface(), Variable[CurrentDataSet].pts, i);
    colorPen->ClearClipping();


    /* Display legend for each dataset
     */
    {
    VARTBL	*vp = &Variable[CurrentDataSet];
    int		ylegend = YLEGEND(CurrentDataSet);

    sprintf(buffer, "%s, %d s/sec", vp->name, vp->SampleRate);

    colorPen->DrawLine(canvas->Surface(), XLEGEND_START, ylegend,
			XLEGEND_END, ylegend);
    blackPen->DrawText(canvas->Surface(), XLEGEND_TEXT, ylegend+3, buffer);
    }
    }

  blackPen->ClearDash();
  blackPen->DrawRectangle(canvas->Surface(), LV, TH, HD, VD);

}	/* END PLOTLINES */

/* -------------------------------------------------------------------- */
static void do_ytics()
{
  int	i, xoffset, yoffset, len;
  int	ticlen;
  double	y_diff, value;

  /* Draw Y-axis tic marks and #'s
  */
  ticlen = Grid ? RV : LV + TICLEN;
  y_diff = ymax - ymin;

  for (i = 0; i <= numtics; ++i)
    {
    yoffset = (int)(BH - ((float)VD / numtics) * i);
    if (yoffset < TH)
      yoffset = TH;

    blackPen->DrawLine(canvas->Surface(), LV, yoffset, ticlen, yoffset);

    value = ymin + (y_diff / numtics * i);

    MakeTicLabel(buffer, y_diff, value);
    len = strlen(buffer);

    xoffset = LV - Y_TIC_LABEL_OFFSET - fonts->StringWidth(XFonts::Point12, buffer);

    blackPen->DrawText(canvas->Surface(), xoffset, yoffset, buffer);
    }

}	/* END DO_YTICS */

/* -------------------------------------------------------------------- */
void SetYLabelPixmap()
{
  int	i, j, ascent, descent;
  const XFontStruct     *fontInfo = fonts->Font(XFonts::Point18);

  ascent	= fontInfo->max_bounds.ascent;
  descent	= fontInfo->max_bounds.descent;
  pixLen	= fonts->StringWidth(XFonts::Point18, ylabel);
  charSize	= ascent + descent;

  if (im_out)
    XDestroyImage(im_out);

  blackPen->SetFont(fonts->Font(XFonts::Point18));
  pixLen = fonts->StringWidth(XFonts::Point18, ylabel);

  /* Create a pixmap, draw string to it and read it back out as an Image.
   */
  in_pm->Clear();
  blackPen->DrawText(in_pm->Surface(), 0, ascent, ylabel);

  im_in = in_pm->GetImage(pixLen, charSize);
  im_out = out_pm->GetImage(charSize, pixLen);


  /* Rotate pixmap
   */
  for (int i = 0; i < charSize; i++)
    for (int j = 0; j < pixLen; j++)
      XPutPixel(im_out, i, pixLen-j-1, XGetPixel(im_in, j, i));

  XDestroyImage(im_in);

}	/* END SETYLABELPIXMAP */

/* END XPLOT.C */

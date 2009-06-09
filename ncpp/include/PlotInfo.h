/*
-------------------------------------------------------------------------
OBJECT NAME:	PlotInfo.h

FULL NAME:	Plot Information

TYPE:		

DESCRIPTION:	Contains everything needed to produce the plot (X & PS).
		One of these exists for each plot type.  field plotType
		will be a constant for each instantiation of this class.

COPYRIGHT:	University Corporation for Atmospheric Research, 1997-8
-------------------------------------------------------------------------
*/

#ifndef PLOTINFO_H
#define PLOTINFO_H

#include <raf/Canvas.h>
#include "Panel.h"
#include <raf/PixMap.h>
#include <raf/PostScript.h>
#include <raf/Printer.h>
#include "SetMgr.h"
#include "Colors.h"
#include <raf/XFonts.h>
#include <raf/XPen.h>

struct plotOffset
        {
        int     titleOffset, subTitleOffset,
		xLabelOffset, yLabelOffset, yTicLabelOffset, xTicLabelOffset,
		ticLength;

	int	xLegendStart, xLegendEnd, xLegendText;
        };


/* -------------------------------------------------------------------- */
class PlotInfo
{
public:
	PlotInfo(PlotType, SetManager&, Canvas&,
			XFonts&, Colors&, XPen&, XPen&);

  friend class	Panel;
  friend class	PlotManager;

  std::string	Title()			{ return(title); }
  std::string	SubTitle()		{ return(subTitle); }

  void	SetTitle(const std::string& newT);
  void	SetSubtitle(const std::string& newT);
  void  UpdateSubtitle(DataSet *set);
  void	SetDefaultLabels(int panel);
  void	AutoScale(int panel);
  void	SetDefaultTics(int panel);
  void	SetRowsCols(int rows, int cols);
  void	SetLineWidth(int w)		{ lineWidth = w; }
  void	SetNormalization(NormType n);
  void	ToggleDITC()			{ ditc = 1 - ditc; }
  void	ToggleGrid()			{ grid = 1 - grid; }
  void	ToggleColor()			{ color = 1 - color; }

//  bool	PlotParmsChanged() const	{ return(plotParmsChanged); }
//  bool	WindowOpen()	const		{ return(windowOpen); }

  void	SetDataTypes(DataType dt);
  void	AddPanel(DataType dt) {}
  void	DeletePanel(DataType dt) {}

  void	Draw();
  void	Print(const char *name);
  void	ResizeX(), ResizePS();

private:
  void		drawHistograms(DataType, int),
		printHistograms(PostScript&, DataType, int);
  void		drawTitles(int), printTitles(PostScript& pen);
  void		drawLegend(int), printLegend(PostScript& pen);
  void		insertPanel(int pos, DataType dt), deletePanel(int pos);

  bool		autoTitles;
  PlotType	plotType;	/* POSTAGE, MOVIE, or THREE_D	*/
  DataType	dataTypes;	/* COUNTS/CONCEN/SURFACE/VOLUME	*/
  NormType	normalization;
  bool		plotParmsChanged;
  bool		grid, color, ditc;

  std::string	title, subTitle;
  static const std::string prelimWarning;

  struct plotOffset	ps;	/* PostScript info, valid for all panels*/
  struct plotOffset	x;	/* Xwindow info, valid for all panels	*/

  int	nPanels;
  int	lineWidth;
  Panel	*panel[4];
  int	nCols, nRows;

  SetManager&	sets;

  Canvas&	canvas;
  Colors&	colors;
  XFonts&	fonts;
  XPen&		blackPen;
  XPen&		colorPen;

};	// END PLOTINFO.H

#endif

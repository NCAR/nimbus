/*
-------------------------------------------------------------------------
OBJECT NAME:	extern.h

FULL NAME:	Global Variable External Declarations

DESCRIPTION:	

AUTHOR:		websterc@ncar
-------------------------------------------------------------------------
*/

#ifndef EXTERN_H
#define EXTERN_H

extern bool	Grid, Frozen, FlashMode;

extern char	buffer[], DataPath[], *ProjectName, FlightNumber[];

extern SDITBL  *sdi[];          /* SDI (aka raw) variable list  */
extern RAWTBL  *raw[];          /* Alphabeticly sorted pointers */
extern DEFAULT *Defaults[];

extern int	nsdi, nraw, nVariables;
extern VARTBL	Variable[];


extern int	NumberDataSets, NumberSeconds, NumberElements[], CurrentDataSet;
extern int	Aircraft, Units;

extern char	*ADSrecord;
extern NR_TYPE	*bits, *volts, *SampledData;

extern NR_TYPE	*plotData[];

extern NR_TYPE	ymin, ymax, smallest_y, biggest_y;
extern int	numtics;

/* Parameter File Variables     */
extern string	title, subtitle, xlabel, ylabel;

/* Error messages	*/
extern char	*o_mem;

/* X vars	*/
extern Widget	varList;

extern instanceRec	iv;


/* Procedures	*/
char	*get_legend();
void	Quit(Widget, XtPointer, XtPointer), ErrorMsg(char msg[]),
	Freeze(Widget, XtPointer, XtPointer), DecodeRecord(),
	AddDataToBuffer(NR_TYPE *newData),
	ModifyActiveVars(Widget, XtPointer, XtPointer),
	ToggleUnits(Widget, XtPointer, XtPointer),
	DoAutoScale(Widget, XtPointer, XtPointer),
	ToggleGrid(Widget, XtPointer, XtPointer),
	ToggleFlash(Widget, XtPointer, XtPointer),
	ResizePlotWindow(Widget w, XtPointer client, XtPointer call),
	PlotData(Widget w, XtPointer client, XtPointer call),
	FindMinMax(), ResizePlotWindow(), AutoScale(char axis);

void	EditParms(), DismissParmsWindow(), ApplyParms(), AddVariable(),
	DeleteVariable(), ToggleGrid(), DoAutoScale(), Quit(), ToggleFlash(),
	PlotData(), AutoScale(), ToggleUnits(), ModifyActiveVars();
	
#endif

/* END EXTERN.H */

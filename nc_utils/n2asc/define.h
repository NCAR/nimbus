/*
-------------------------------------------------------------------------
OBJECT NAME:	define.h

DESCRIPTION:	Header File for Nimbus Skeleton.
-------------------------------------------------------------------------
*/

#ifndef DEFINE_H
#define DEFINE_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sys/types.h>
#include <Xm/Xm.h>

#include <string>
#include <vector>

#define NO_NETCDF_2

//typedef unsigned char   bool;

#ifndef ERR
#define OK              (0)
#define ERR             (-1)
#endif


#define COMMENT         '#'     /* Comment character for textfiles      */

#define MAX_VARIABLES		2000
#define MAX_TIME_SLICES		5

/* Values for "XaxisType"	*/
#define TIME			0x01
#define COLONLESS		0x03
#define RUNCOUNT		0x04
#define UTS			0x08

/* PauseWhatToDo values.		*/
#define P_QUIT			0
#define P_CONTINUE		1

#define BEG_OF_TAPE		(-1)
#define END_OF_TAPE		(-1)

/* OpenProjectFile action values	*/
#define RETURN			0
#define EXIT			1


#define LOW_RATE		1
#define HIGH_RATE		25


/* Nimbus Record Info		*/
#define NR_TYPE float
#define NR_SIZE sizeof(NR_TYPE)


typedef struct
  {
  std::string	name;
  std::string	units;

  int	inVarID;		/* netCDF variable ID		*/
  int	SampleRate;
  int	OutputRate;
  size_t	VectorLength;
  bool	Output;
  float	MissingValue;
  char	Format[12];		/* Format to use for printf	*/
  } VARTBL;


/*		Global Variables
 */
extern char	*ProjectNumber, *ProjectName, DefaultFormat[];
extern char	*Aircraft, *FlightNumber, *FlightDate, *Defaults;
extern std::vector<VARTBL*> Variable, outputList;
extern bool	PauseFlag, Interactive, AmesFormat, PrintUnits, OutputDate;
extern int	InputFile, PauseWhatToDo, XaxisType, *time_data;
extern FILE	*OutputFile;
extern char	buffer[];
extern size_t	nRecords;
extern long	CurrentInputRecordNumber;

extern time_t	FileStartTime, FileEndTime, UserStartTime, UserEndTime;

/* Setup window widgets */
extern Widget	Shell001;
extern Widget	SetupWindow, EditWindow, TimeSliceWindow;
extern Widget	inputFileText, outputFileText;
extern Widget	timeDisplayText;
extern Widget	list1, goButton, menuBar, readHeaderButton;

/* Edit Variable window widgets */
extern Widget	ts_text[];


/*		Local Functions
 */
int	SearchTable(std::vector<VARTBL*>&, int n, char target[]);
int	SearchTableSansLocation(std::vector<VARTBL*>&, int n, char target[]);

void	CloseNetCDF(), FlushXEvents(), GetUserTimeIntervals(), Initialize(),
	LogMessage(char msg[]), HandleError(char msg[]),
	HandleWarning(char msg[], void (*callBack)(Widget, XtPointer, XtPointer)),
	ReadBatchFile(char fileName[]);

/* Callbacks	*/
void	CancelWarning(Widget w, XtPointer client, XtPointer call),
	CancelSetup(Widget w, XtPointer client, XtPointer call),
	EditTimeSlices(Widget w, XtPointer client, XtPointer call),
	DismissEditWindow(Widget w, XtPointer client, XtPointer call),
	EditVariable(Widget w, XtPointer client, XtPointer call),
	Quit(Widget w, XtPointer client, XtPointer call),
	ReadHeader(Widget w, XtPointer client, XtPointer call),
	StartProcessing(Widget w, XtPointer client, XtPointer call),
	ValidateTime(Widget w, XtPointer client, XtPointer call),
	Proceed(Widget w, XtPointer client, XtPointer call),
	ToggleOutput(Widget w, XtPointer client, XtPointer call),
	PauseProcessing(Widget w, XtPointer client, XtPointer call),
	PauseStop(Widget w, XtPointer client, XtPointer call),
	PauseContinue(Widget w, XtPointer client, XtPointer call),
	DismissTimeSliceWindow(Widget w, XtPointer client, XtPointer call),
	ResetTimeSliceWindow(Widget w, XtPointer client, XtPointer call);

#endif

/* END DEFINE.H */

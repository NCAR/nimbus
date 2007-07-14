/*
-------------------------------------------------------------------------
OBJECT NAME:	define.h

DESCRIPTION:	Header File for Nimbus Skeleton.
-------------------------------------------------------------------------
*/

#ifndef _ncav_define_h_
#define _ncav_define_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <Xm/Xm.h>

#include <raf/constants.h>
#include <raf/raf.h>

#define NAMELEN		32
#define MAX_VARIABLES	2000
#define MAX_TIME_SLICES	5


/* PauseWhatToDo values.		*/
#define P_QUIT		0
#define P_CONTINUE	1

#define BEG_OF_TAPE	(-1)
#define END_OF_TAPE	(-1)

/* OpenProjectFile action values	*/
#define RETURN		0
#define EXIT		1


#define LOW_RATE	1
#define HIGH_RATE	25


/* Nimbus Record Info		*/
#define NR_TYPE float
#define NR_SIZE sizeof(NR_TYPE)


typedef struct
	{
	char	name[NAMELEN];
	int	inVarID;
	int	outVarID;
	size_t	SampleRate;
	size_t	OutputRate;
	size_t	VectorLength;
	bool	Output;
	} VARTBL;


/*		Global Variables
 */
extern char	*ProjectNumber, *ProjectName, *TimeInterval;
extern char	*Aircraft, *FlightNumber, *FlightDate;
extern VARTBL	*Variable[];
extern bool	PauseFlag, Interactive;
extern int	nVariables, InputFile, OutputFile, PauseWhatToDo, AverageRate,
		mvThreshold;
extern char	buffer[];


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
int	ReadTextFile(), AccessProjectFile(),
	SearchTable(char *table[], int ntable, char target[]);
void	FreeTextFile(), ReadBatchFile(char file[]),
	SortTable(char **table, int beg, int end);

void	CreateNetCDF(), WriteNetCDF(), CloseNetCDF(),
	SetBaseTime(), FlushXEvents(), GetUserTimeIntervals(), Initialize(),
	LogMessage(), FormatTimeSegmentsForOutputFile(), AverageSampledData(),
	AllocateDataArrays(), FreeDataArrays(), HandleError(), HandleWarning();

/* Callbacks	*/
void	CancelWarning(), ApplyVariableMods(), CancelSetup(), EditTimeSlices(),
	DismissEditWindow(), EditVariable(), MapCalCof(), Quit(), ReadHeader(),
	StartProcessing(), ValidateTime(), Proceed(), ToggleOutput(),
	PauseProcessing(), VerifyLagText(), ResetTimeSliceWindow(),
	PauseStop(), PauseContinue(), DismissTimeSliceWindow();

#endif

/* END DEFINE.H */

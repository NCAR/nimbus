/*
-------------------------------------------------------------------------
OBJECT NAME:	define.h

DESCRIPTION:	Header File for Nimbus Skeleton.
-------------------------------------------------------------------------
*/

#ifndef DEFINE_H
#define DEFINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <Xm/Xm.h>

#define NO_NETCDF_2

typedef unsigned char   bool;

#ifndef TRUE
#define TRUE            1
#define FALSE           0
#endif

#ifndef ERR
#define OK              (0)
#define ERR             (-1)
#endif

#ifndef MAX
#define MAX(x,y) (x>y?x:y)
#endif

#ifndef MIN
#define MIN(x,y) (x>y?y:x)
#endif


#define COMMENT         '#'     /* Comment character for textfiles      */

/* This is the number of records that you wish to have access to.  This is
 * used to determine the size of the circular buffer.  This number should
 * be odd.
 */
#define NBUFFERS		1
#define CURRENT_BUFFER		-((NBUFFERS >> 1) + 1))


#define NAMELEN			20
#define MAX_VARIABLES		2000
#define MAX_TIME_SLICES		5


/* Values for "XaxisType"	*/
#define TIME			0x01
#define COLONLESS		0x03
#define RUNCOUNT		0x04
#define UTS			0x08

/* Values for "VariableType"	*/
#define SDI			0
#define RAW			1
#define DERIVED			2


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
	char	name[NAMELEN];
	int	inVarID;		/* netCDF variable ID		*/
	int	SampleRate;
	int	OutputRate;
	int	VectorLength;
	bool	Output;
	float	MissingValue;
	char	Format[12];		/* Format to use for printf	*/
	} VARTBL;


/*		Global Variables
 */
extern char	*ProjectNumber, *ProjectName, DefaultFormat[];
extern char	*Aircraft, *FlightNumber, *FlightDate, *Defaults;
extern VARTBL	*Variable[];
extern bool	PauseFlag, Interactive, AmesFormat;
extern int	nVariables, InputFile, PauseWhatToDo, XaxisType;
extern FILE	*OutputFile;
extern char	buffer[];
extern size_t	nRecords;
extern long	CurrentInputRecordNumber;


/* Setup window widgets */
extern Widget	Shell001;
extern Widget	SetupWindow, EditWindow, TimeSliceWindow;
extern Widget	inputFileText, outputFileText;
extern Widget	timeDisplayText;
extern Widget	list1, goButton, menuBar, readHeaderButton;

/* Edit Variable window widgets */
extern Widget	ts_text[];


double	atof();

/*		Local Functions
 */
char	*GetMemory();
int	SearchTable(char *table[], int ntable, char target[]);
int	SearchTableSansLocation(char *table[], int ntable, char target[]);

void	CloseNetCDF(), FlushXEvents(), GetUserTimeIntervals(), Initialize(),
	LogMessage(), HandleError(), HandleWarning(), ReadBatchFile();

/* Callbacks	*/
void	CancelWarning(), CancelSetup(), EditTimeSlices(),
	DismissEditWindow(), EditVariable(), Quit(), ReadHeader(),
	StartProcessing(), ValidateTime(), Proceed(), ToggleOutput(),
	PauseProcessing(), PauseStop(), PauseContinue(),
	DismissTimeSliceWindow(), ResetTimeSliceWindow();


#endif

/* END DEFINE.H */

#define NO_NETCDF_2

#include <netcdf.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifndef TRUE
#define TRUE            1
#define FALSE           0
#endif

typedef unsigned char   bool;

#define BUFFSIZE            8192
#define MAX_VARS            500
#define DEFAULT_TI_LENGTH   (20)
#define MISSING_VALUE       (-32767.0)

#define PLAIN_FILE	0
#define NASA_AMES	1
#define NASA_LANGLEY	2

extern char	buffer[];
extern int	ncid, varid[], nVariables, timeOffsetID;
extern float	scale[], offset[], missingVals[];
extern char	*time_vars[];
extern const char	*noTitle, *noUnits;
extern const int	rateOne;
extern time_t	BaseTime;
extern int	BaseDataRate, dataRate;

extern int	SkipNlines;

void CreateNASAamesNetCDF(FILE *fp);
void CreateNASAlangNetCDF(FILE *fp);
void CreatePlainNetCDF(FILE *fp);
void SetNASABaseTime(int, int, int), SetPlainBaseTime(void);

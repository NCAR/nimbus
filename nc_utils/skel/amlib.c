/*
-------------------------------------------------------------------------
OBJECT NAME:	amlib.c

FULL NAME:	

ENTRY POINTS:	SetComputeFunctions()
		sadifr()
		slatc()
		sxnoym()

STATIC FNS:	none

DESCRIPTION:	Template for AMLIB std & user directories.

INPUT:			

OUTPUT:			

REFERENCES:	none

REFERENCED BY:	compute.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1993,1994
-------------------------------------------------------------------------
*/

#include "define.h"
#include <netcdf.h>


/* STEP 4: To appease the compiler, place all AMLIB function names here.
 * By convention, AMLIB functions start with the letter 's'.  These should
 * be the same names you used in steps 2 & 3.
 */
void	sadifr(), slatc(), sxnom();


/* STEP 1: List variables to pass through unaltered by default.  HOUR,
 * MINUTE & SECOND are mandatory.  NULL must be last.
 */
char	*passThrough[] = 
	{
	"HOUR",
	"MINUTE",
	"SECOND",
	NULL
	};


/* STEP 2: New variables get added here.  Also add existing variables that
 * you would like to enter additional processing for.  Remove the examples
 * given below, leaving the "NULL, NULL" line in.  Make sure to leave off
 * the parentheses from the function names.
 *
 * Attributes: (Units, Rate, Vector Len, & Title) only need to be defined
 * for variables that do NOT exist in the input file.
 */
DERVAR	derivftns[] =
	{
/*			Out  Vector			*/
/*  Name	Units Rate Length  Function Title	*/
/* ------------------------------------------------------------------------ */
  { "ADIFR",	"",	1,	1,	   sadifr,	"" },
  { "LATC",	"deg",	1,	1,	   slatc,	"Corrected Latitude" },
/*  { "XNOYM",	"ppm",	50,	1,	   sxnoym,	"Nitro Measured" }, */
  { NULL,		NULL,	0,	0,	   NULL }
  };


/* STEP 3: Create AMLIB functions here.  Use these examples as templates.
 * First example is one of modifying an existing variable (ADIFR).  The
 * second example is of a new variable, using 2 existing inputs.  The third
 * example is more complex, using high-rate data.  You want one function
 * for each variable output, do NOT have one function outputting several
 * variables.  However you may use the same function for multiple variables.
 * Each of these functions will be called once per second of data, so if
 * you are dealing with high-rate data or vector data, make sure you handle
 * all the samples for a given second.
 */

static long	mindex[3] = {0, 0, 0};

/* -------------------------------------------------------------------- */
void sadifr(varp)
VARTBL	*varp;		/* Information about current variable	*/
{
	NR_TYPE	adifr;

	/* Get current value of ADIFR.
	 */
	mindex[0] = CurrentInputRecordNumber;
	ncvarget1(InputFile, varp->inVarID, mindex, (void *)&adifr);

	adifr += 1.0;

	/* Write out new ADIFR
	 */
	mindex[0] = CurrentOutputRecordNumber;
	ncvarput1(OutputFile, varp->outVarID, mindex, (void *)&adifr);

}	/* END SADIFR */

/* -------------------------------------------------------------------- */
void slatc(varp)
VARTBL	*varp;		/* Information about current variable	*/
{
	NR_TYPE	lat, glat, latc;

	static bool	first_time = TRUE;
	static int	inVarID[2];

	if (first_time)
		{
		/* Obtain ID's of input variables.
		 */
		inVarID[0] = ncvarid(InputFile, "LAT");
		inVarID[1] = ncvarid(InputFile, "GLAT");

		first_time = FALSE;
		}

	/* Get current values of inputs.
	 */
	mindex[0] = CurrentInputRecordNumber;
	ncvarget1(InputFile, inVarID[0], mindex, (void *)&lat);
	ncvarget1(InputFile, inVarID[1], mindex, (void *)&glat);

	latc = (lat + glat) / 2;

	/* Write out new variable.
	 */
	mindex[0] = CurrentOutputRecordNumber;
	ncvarput1(OutputFile, varp->outVarID, mindex, (void *)&latc);

}	/* END SLATC */

/* -------------------------------------------------------------------- */
void sxnoym(varp)
VARTBL	*varp;		/* Information about current variable	*/
{
	NR_TYPE	xnoym, xno[50], xnoy[50];
	long	start[3], count[3];
	int		i;

	static bool	first_time = TRUE;
	static int	inVarID[2];

	if (first_time)
		{
		/* Obtain ID's of input variables.
		 */
		inVarID[0] = ncvarid(InputFile, "XNO");
		inVarID[1] = ncvarid(InputFile, "XNOY");

		first_time = FALSE;
		}


	/* Example to get 50 samples of 1hz data

	start[0] = CurrentInputRecordNumber - 25, start[1] = start[2] = 0;
	count[0] = 50, count[1] = 1, count[2] = 0;
*/

	/* Example to get 50 samples of 50hz data

	start[0] = CurrentInputRecordNumber, start[1] = start[2] = 0;
	count[0] = 1, count[1] = 50, count[2] = 0;
*/

	/* Get current values of inputs.
	 */
	ncvarget(InputFile, inVarID[0], start, count, (void *)xno);
	ncvarget(InputFile, inVarID[1], start, count, (void *)xnoy);


	for (i = 0; i < 50; ++i)
		printf("%6.1f ", xno[i]);

	printf("\n");

	for (i = 0; i < 50; ++i)
		printf("%6.1f ", xnoy[i]);

	printf("\n");


	mindex[0] = CurrentOutputRecordNumber;
	ncvarput1(OutputFile, varp->outVarID, mindex, (void *)&xnoym);

}	/* END SXNOYM */

/* -------------------------------------------------------------------- */
/* Leave this function alone.
 */
void SetComputeFunctions()
{
  int	i, indx;

  for (i = 0; derivftns[i].Compute; ++i)
    if ((indx = SearchTable((char **)Variable,nVariables,derivftns[i].Name)) != ERR)
      Variable[indx]->Compute = derivftns[i].Compute;

}	/* END SETCOMPUTEFUNCTIONS */

/* END AMLIB.C */

/*
-------------------------------------------------------------------------
OBJECT NAME:	lrloop.c

FULL NAME:	Main loop for low rate processing

ENTRY POINTS:	LowRateLoop()

STATIC FNS:	PrintVariables()
		Month()
		FindFirstLogicalRecord()
		FindNextLogicalRecord()

DESCRIPTION:	

INPUT:		long beginning and ending times

OUTPUT:		

REFERENCES:	average.c, compute.c, netcdf.c, timeseg.c

REFERENCED BY:	StartProcessing()

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include "define.h"
#include "netcdf.h"

static const int	NVARS_PER_LINE = 10;

static long	timeVarID[3];
static int	currentTime[3];

extern XtAppContext context;

static int	highestRate;

static void PrintVariables();
static int FindFirstRecordNumber(long starttime);
static int FindNextRecordNumber(long endtime);

static int Month(char s[]);
void UpdateTime(int currentTime[]);

/* -------------------------------------------------------------------- */
int LowRateLoop(long starttime, long endtime)
{
  int	rc, i, Cntr = 0;
  char	tmp[128], units[32], long_name[64];
  float	miss;

  if ((rc = FindFirstRecordNumber(starttime)) == ERR)
    goto exit;

  /* Print Title
   */
  if (AmesFormat)
    {
    int	mon, nVars = nVariables - 3, xTraLines;

    xTraLines = (nVars-1) / NVARS_PER_LINE;

    fprintf(OutputFile, "%3d 1001\n", 15 + nVars + (xTraLines<<1));
//    fprintf(OutputFile, "Lastname, Firstname\n");
fprintf(OutputFile, "Schanot, Allen\n");
    ncattget(InputFile, NC_GLOBAL, "Source", tmp);
    fprintf(OutputFile, "%s\n", tmp);
    ncattget(InputFile, NC_GLOBAL, "Aircraft", tmp);
    fprintf(OutputFile, "%s\n", tmp);
    ncattget(InputFile, NC_GLOBAL, "ProjectName", tmp);
    fprintf(OutputFile, "%s\n", tmp);
    fprintf(OutputFile, "  1  1\n");

    ncattget(InputFile, NC_GLOBAL, "FlightDate", tmp);
    fprintf(OutputFile, "%s %c%c %c%c  ", &tmp[6], tmp[0],
		tmp[1], tmp[3], tmp[4]);

    ncattget(InputFile, NC_GLOBAL, "DateProcessed", tmp);
    mon = Month(tmp);
    fprintf(OutputFile, "%s %2d %c%c\n", strstr(tmp, "UTC")+4,
		mon, tmp[4], tmp[5]);

    fprintf(OutputFile, "1.0\n");
    fprintf(OutputFile, "Time in seconds from 00Z\n");
    fprintf(OutputFile, "%3d\n", nVars);

    for (i = 0; i < nVariables; ++i)
      if (Variable[i]->Output)
        fprintf(OutputFile, "1.0 ");

    fprintf(OutputFile, "\n");

    for (i = 0; i < nVariables; ++i)
      if (Variable[i]->Output)
        {
        ncattget(InputFile, Variable[i]->inVarID, "missing_value", &miss);
        fprintf(OutputFile, "99999 ");
        Variable[i]->MissingValue = miss;

        if (++Cntr % NVARS_PER_LINE == 0)
          fprintf(OutputFile, "\n");
        }

    if (Cntr % NVARS_PER_LINE)
      fprintf(OutputFile, "\n");

    for (i = 0; i < nVariables; ++i)
      if (Variable[i]->Output)
        {
        ncattget(InputFile, Variable[i]->inVarID, "units", units);
        ncattget(InputFile, Variable[i]->inVarID, "long_name", long_name);
        fprintf(OutputFile, "%s (%s)\n", long_name, units);
        }

    fprintf(OutputFile, "  0\n");
    fprintf(OutputFile, "%3d\n", 1 + xTraLines);
    }


  fprintf(OutputFile, "UTC   ");

  highestRate = 1;
  Cntr = 0;

  for (i = 0; i < nVariables; ++i)
    if (Variable[i]->Output)
      {
//      if ((siz = atoi(&Variable[i]->Format[1])) < 5)
//        siz = 13;

      fprintf(OutputFile, " %-8s", Variable[i]->name);

      if (Variable[i]->VectorLength > 1)
        {
        int	j;

        for (j = 2; j < Variable[i]->VectorLength; ++j)
          fprintf(OutputFile, " %3d", j);
        }

      highestRate = MAX(highestRate, Variable[i]->OutputRate);

      if (AmesFormat && ++Cntr % NVARS_PER_LINE == 0)
        fprintf(OutputFile, "\n");
      }

  if (!AmesFormat || (AmesFormat && Cntr % NVARS_PER_LINE))
    fprintf(OutputFile, "\n");


  /* This is the main control loop.
  */
  do
    {
    PrintVariables();
    UpdateTime(currentTime);

    while (PauseFlag == TRUE)
      XtAppProcessEvent(context, XtIMAll);

    if (PauseWhatToDo == P_QUIT)
      {
      rc = ERR;
      goto exit;
      }
    }
  while ((rc = FindNextRecordNumber(endtime)) == OK);

  rc = OK;

exit:
  return(rc);

}	/* END LOWRATELOOP */

/* -------------------------------------------------------------------- */
static void PrintVariables()
{
  int		i, j, msec, cntr = 0;
  NR_TYPE	data[640];
  VARTBL	*vp;

  static int	prevHour = 0;
  static long	start[] = {0, 0, 0};
  static long	count[] = {1, 1, 1};

  start[0] = CurrentInputRecordNumber;
  start[1] = 0;

  for (msec = 0; msec < 1000; msec += (1000 / highestRate))
    {
    /* Print timestamp / rolling counter.
     */
    if (XaxisType & TIME)
      {
      static int	prevVal = 0, val;
      val = currentTime[0] * 3600 + currentTime[1] * 60 + currentTime[2];

      if (XaxisType == COLONLESS)
        fprintf(OutputFile, "%02d%02d%02d",
              currentTime[0], currentTime[1], currentTime[2]);
      else
        fprintf(OutputFile, "%02d:%02d:%02d",
              currentTime[0], currentTime[1], currentTime[2]);

      if (prevVal+1 != val)
        printf("Gap - %02d:%02d:%02d\n",
		currentTime[0], currentTime[1],currentTime[2]);
      prevVal = val;

      if (highestRate > 1)
        fprintf(OutputFile, ".%03d", msec);
      }
    else
    if (XaxisType == UTS)
      {
      static int	offset = 0;

      if (currentTime[0] == 0 && prevHour == 23)	/* midnight wrap */
        offset = 86400;

      fprintf(OutputFile, "%d", currentTime[0] * 3600 +
		currentTime[1] * 60 + currentTime[2] + offset);


      prevHour = currentTime[0];
      }
    else
      fprintf(OutputFile, "%ld", CurrentInputRecordNumber);


    for (i = 0; i < nVariables; ++i)
      if ((vp = Variable[i])->Output)
        {
        if (vp->VectorLength > 1)
          {
          count[2] = vp->VectorLength;

          ncvarget(InputFile, vp->inVarID, start, count, (void *)data);

          if (AmesFormat)
            for (j = 1; j < vp->VectorLength; ++j)
              if (data[j] == vp->MissingValue)
                data[j] = 99999;

          if (strchr(vp->Format, 'd'))
            for (j = 1; j < vp->VectorLength; ++j)
              fprintf(OutputFile, vp->Format, (int)data[j]);
          else
            for (j = 1; j < vp->VectorLength; ++j)
              fprintf(OutputFile, vp->Format, data[j]);
          }
        else
          {
          ncvarget1(InputFile, vp->inVarID, start, (void *)data);

          if (AmesFormat && data[0] == vp->MissingValue)
              data[0] = 99999;

          if (strchr(vp->Format, 'd'))
            fprintf(OutputFile, vp->Format, (int)data[0]);
          else
            fprintf(OutputFile, vp->Format, data[0]);
          }

        if (AmesFormat && ++cntr % NVARS_PER_LINE == 0)
          fprintf(OutputFile, "\n");
        }

    if (!AmesFormat || (AmesFormat && cntr % NVARS_PER_LINE))
      fprintf(OutputFile, "\n");
    ++start[1];
    }

}	/* END PRINTVARIABLES */
 
/* -------------------------------------------------------------------- */
static int FindFirstRecordNumber(long starttime)
{
  timeVarID[0] = Variable[SearchTable(Variable, nVariables, "HOUR")]->inVarID;
  timeVarID[1] = Variable[SearchTable(Variable, nVariables, "MINUTE")]->inVarID;
  timeVarID[2] = Variable[SearchTable(Variable, nVariables, "SECOND")]->inVarID;

  CurrentInputRecordNumber = -1;

  if (starttime != BEG_OF_TAPE)
    while (FindNextRecordNumber(starttime) == OK)
      ;
  else
    FindNextRecordNumber(starttime);

  return(OK);

}	/* END FINDFIRSTRECORDNUMBER */

/* -------------------------------------------------------------------- */
static int FindNextRecordNumber(long endtime)
{
  int		current_time;
  long		mindex[1];
  NR_TYPE	f;

  static bool	rollOver = FALSE;

  if (++CurrentInputRecordNumber >= nRecords)	/* End of tape	*/
    return(ERR);

  mindex[0] = CurrentInputRecordNumber;

  ncvarget1(InputFile, timeVarID[0], mindex, (void *)&f);
  current_time = (int)f * 3600;
  currentTime[0] = (int)f;

  ncvarget1(InputFile, timeVarID[1], mindex, (void *)&f);
  current_time += (int)f * 60;
  currentTime[1] = (int)f;

  ncvarget1(InputFile, timeVarID[2], mindex, (void *)&f);
  current_time += (int)f;
  currentTime[2] = (int)f;

  if (endtime == END_OF_TAPE)
    return(OK);


  if (current_time == 0 && CurrentInputRecordNumber > 0)
    rollOver = TRUE;

  if (rollOver)
    current_time += 86400;

  if (current_time < endtime)
    return(OK);
  else
    return(ERR);

}	/* END FINDNEXTRECORDNUMBER */

/* -------------------------------------------------------------------- */
static char *mons[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
	"Aug", "Sep", "Oct", "Nov", "Dec" };

static int Month(char s[])
{
  int	i;

  for (i = 0; i < 12; ++i)
    if (strncmp(mons[i], s, 3) == 0)
      return(i+1);

  return(0);

}

/* END LRLOOP.C */

/*
-------------------------------------------------------------------------
OBJECT NAME:	lrloop.c

FULL NAME:	Main loop for low rate processing

ENTRY POINTS:	LowRateLoop()

STATIC FNS:	PrintVariables()
		FindFirstLogicalRecord()
		FindNextLogicalRecord()

DESCRIPTION:	

REFERENCES:	average.c, compute.c, netcdf.c, timeseg.c

REFERENCED BY:	StartProcessing()

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2005
-------------------------------------------------------------------------
*/

#include "define.h"
#include <netcdf.h>

#include <math.h>

static const int	NVARS_PER_LINE = 20;

static struct tm *currentTime;

extern XtAppContext context;

static int	highestRate;

static void PrintVariables();
static int FindFirstRecordNumber(long starttime);
static int FindNextRecordNumber(long endtime);

void UpdateTime(struct tm *currentTime);
void getNCattr(int ncid, const char attr[], char **dest);
bool isMissingValue(float target, float fillValue);

/* -------------------------------------------------------------------- */
int LowRateLoop(long starttime, long endtime)
{
  size_t i;
  int	rc, Cntr = 0;
  char	*tmp, units[32], long_name[64];
  float	miss;
  size_t len;

  if ((rc = FindFirstRecordNumber(starttime)) == ERR)
    goto exit;

  /* Print Title
   */
  if (AmesFormat)
  {
    int	nVars = outputList.size(), xTraLines;

    xTraLines = (nVars-1) / NVARS_PER_LINE;

    fprintf(OutputFile, "%3d 1001\n", 15 + nVars + (xTraLines<<1));
    fprintf(OutputFile, "Lastname, Firstname\n");
    getNCattr(InputFile, "Source", &tmp);
    fprintf(OutputFile, "%s\n", tmp);
    getNCattr(InputFile, "Aircraft", &tmp);
    fprintf(OutputFile, "%s\n", tmp);
    getNCattr(InputFile, "ProjectName", &tmp);
    fprintf(OutputFile, "%s\n", tmp);
    fprintf(OutputFile, "  1  1\n");

    getNCattr(InputFile, "FlightDate", &tmp);
    fprintf(OutputFile, "%s %c%c %c%c  ", &tmp[6], tmp[0],
		tmp[1], tmp[3], tmp[4]);

    getNCattr(InputFile, "DateProcessed", &tmp);
    tmp[10] = '\0';
    while (strchr(tmp, '-'))
      *strchr(tmp, '-') = ' ';

    fprintf(OutputFile, "%s\n", tmp);

    fprintf(OutputFile, "1.0\n");
    fprintf(OutputFile, "Time in seconds from 00Z\n");
    fprintf(OutputFile, "%3d\n", nVars);

    for (i = 0; i < outputList.size(); ++i)
      fprintf(OutputFile, "1.0 ");

    fprintf(OutputFile, "\n");

    for (i = 0; i < outputList.size(); ++i)
    {
      if (nc_get_att_float(InputFile, i, "_FillValue", &miss) != NC_NOERR)
        if (nc_get_att_float(InputFile, i, "missing_value", &miss) != NC_NOERR)
          if (nc_get_att_float(InputFile, i, "MissingValue", &miss) != NC_NOERR)
            miss = -32767.0;	// Kinda Bogus.

      fprintf(OutputFile, "99999 ");
      outputList[i]->MissingValue = miss;

      if (++Cntr % NVARS_PER_LINE == 0)
        fprintf(OutputFile, "\n");
    }

    if (Cntr % NVARS_PER_LINE)
      fprintf(OutputFile, "\n");

    for (i = 0; i < outputList.size(); ++i)
    {
      nc_get_att_text(InputFile, outputList[i]->inVarID, "units", units);
      nc_get_att_text(InputFile, outputList[i]->inVarID, "long_name", long_name);
      nc_inq_attlen(InputFile, outputList[i]->inVarID, "long_name", &len);
      long_name[len] = '\0';
      nc_inq_attlen(InputFile, outputList[i]->inVarID, "units", &len);
      units[len] = '\0';

      fprintf(OutputFile, "%s (%s)\n", long_name, units);
    }

    fprintf(OutputFile, "  0\n");
    fprintf(OutputFile, "%3d\n", 1 + xTraLines);
  }


  if (OutputDate)
    fprintf(OutputFile, "DATE     ");
  fprintf(OutputFile, "UTC     ");

  highestRate = 1;
  Cntr = 0;

  for (i = 0; i < outputList.size(); ++i)
  {
//    if ((siz = atoi(&outputList[i]->Format[1])) < 5)
//      siz = 13;

    fprintf(OutputFile, " %s ", outputList[i]->name.c_str());

    if (outputList[i]->VectorLength > 1)
    {
      for (size_t j = 1; j < outputList[i]->VectorLength; ++j)
        fprintf(OutputFile, " %3d", j);
    }

    highestRate = std::max(highestRate, outputList[i]->OutputRate);

    if (AmesFormat && ++Cntr % NVARS_PER_LINE == 0)
      fprintf(OutputFile, "\n");
  }

  if (!AmesFormat || (AmesFormat && Cntr % NVARS_PER_LINE))
    fprintf(OutputFile, "\n");


  if (!AmesFormat && PrintUnits)
  {
    fprintf(OutputFile, "Time    ");

    for (i = 0; i < outputList.size(); ++i)
      fprintf(OutputFile, " %s ", outputList[i]->units.c_str());

    fprintf(OutputFile, "\n");
  }


  /* This is the main control loop.
  */
  do
    {
    PrintVariables();
    UpdateTime(currentTime);

    while (PauseFlag == true)
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
  size_t	i, j;
  int		msec, cntr = 0;
  float		data[640];
  VARTBL	*vp;

  static int	prevHour = 0;
  static size_t	start[] = {0, 0, 0};
  static size_t	count[] = {1, 1, 1};

  start[0] = CurrentInputRecordNumber;
  start[1] = 0;

  for (msec = 0; msec < 1000; msec += (1000 / highestRate))
  {
    /* Print timestamp / rolling counter.
     */
    if (OutputDate)
      fprintf(OutputFile, "%04d-%02d-%02d ", currentTime->tm_year+1900,
		currentTime->tm_mon+1, currentTime->tm_mday);

    if (XaxisType & TIME)
    {
      static int	prevVal = -1, val;
      val = currentTime->tm_hour * 3600 + currentTime->tm_min * 60 + currentTime->tm_sec;

      if (XaxisType == COLONLESS)
        fprintf(OutputFile, "%02d%02d%02d",
              currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
      else
        fprintf(OutputFile, "%02d:%02d:%02d",
              currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);

      if (	prevVal != -1 && msec == 0 &&
		!(prevVal == 86399 && val == 0) && prevVal+1 != val)
        printf("Gap - %02d:%02d:%02d\n",
		currentTime->tm_hour, currentTime->tm_min,currentTime->tm_sec);
      prevVal = val;
    }
    else
    if (XaxisType == UTS)
    {
      static int	offset = 0;

      if (currentTime->tm_hour == 0 && prevHour == 23)	/* midnight wrap */
        offset = 86400;

      fprintf(OutputFile, "%d", currentTime->tm_hour * 3600 +
		currentTime->tm_min * 60 + currentTime->tm_sec + offset);


      prevHour = currentTime->tm_hour;
    }
    else
      fprintf(OutputFile, "%ld", CurrentInputRecordNumber);

    if (highestRate > 1)
      fprintf(OutputFile, ".%03d", msec);

    for (i = 0; i < outputList.size(); ++i)
    {
      vp = outputList[i];
      if (vp->VectorLength > 1)
        {
        count[2] = vp->VectorLength;

        nc_get_vara_float(InputFile, vp->inVarID, start, count, data);

        if (AmesFormat)
          for (j = 0; j < vp->VectorLength; ++j)
            if (isMissingValue(data[j], vp->MissingValue))
              data[j] = 99999.0;

        if (strchr(vp->Format, 'd'))
          for (j = 0; j < vp->VectorLength; ++j)
            fprintf(OutputFile, vp->Format, (int)data[j]);
        else
          for (j = 0; j < vp->VectorLength; ++j)
            fprintf(OutputFile, vp->Format, data[j]);
      }
      else
      {
        nc_get_var1_float(InputFile, vp->inVarID, start, data);

        if (AmesFormat && isMissingValue(data[0], vp->MissingValue))
            data[0] = 99999.0;

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
  if (++CurrentInputRecordNumber >= (long)nRecords)	/* End of tape	*/
    return(ERR);

  time_t t = FileStartTime + time_data[CurrentInputRecordNumber];
  currentTime = gmtime(&t);

  if (	endtime == END_OF_TAPE ||
	FileStartTime + time_data[CurrentInputRecordNumber] < endtime)
    return(OK);
  else
    return(ERR);

}	/* END FINDNEXTRECORDNUMBER */

/* -------------------------------------------------------------------- */
bool isMissingValue(float target, float fillValue)
{
  if (isnan(fillValue))
    return(isnan(target));
  else
    return(fillValue == target);

}       /* END ISMISSINGVALUE */

/* END LRLOOP.C */

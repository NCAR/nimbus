/* lstats.c -- manage statistics setup parameters and output

*/

#include <stdio.h>

#include <printers.h>
#include <constants.h>
#include <shmem.h>
#include <macros.h>
#include <header.h>
#include <lengths.h>
#include <socket.h>
#include <files.h>
#include <proto.h>

/**************** FILE-SPECIFIC DEFINITIONS  ******************************/

#define TIME_LENGTH 9   /* length of time label (hh:mm:ss\0) */
#define STATS_ROWS 60   /* # of rows to print */
#define HEADER_ROWS 6   /* # of rows in stats header */


/************************  LOCALLY GLOBAL  ************************************/

static int NeedStatsInit=FALSE;
static int StatsFileNumber=0;
static int EndPeriodTotalSeconds;
static int CurrentHr,          /* current hour */
           CurrentMinute,      /* current minute */
           CurrentSecond,
           StartPeriodHr,
           StartPeriodMinute,
           StartPeriodSecond,
           EndPeriodHr,
           EndPeriodMinute,
           EndPeriodSecond;          /* total minute since midnite for start and end
                                   of current dump */
static int print_total;         /* saved values for current dump */
static double *sum,
              *sumsq,
              *minimum,
              *maximum;

static int *spec_index;  /* plot_spex index, used for units */
static int NumSecondsAccum=0;
double sqrt();
static int FlushStats=FALSE;
static int rows_printed=0, headers_printed=0;
static FILE *stream;
static char StatsOutputFilename[MAX_FLNM_LNGTH];
static int *VarInStatsOutput;
static char StatsSelnsFileName[MAX_FLNM_LNGTH];
static FILE *StatsSelnsFile;
static char FlushStartTime[TIME_LENGTH], FlushEndTime[TIME_LENGTH];
static char NextFlushString[TIME_LENGTH];
static char StatsFlushListTitle[256];

/***********************   CreateStatsIndexArray()  *************************/

CreateStatsIndexArray()
{
 VarInStatsOutput = (int *)calloc((unsigned)GetNumMenuEntries(),sizeof(int));
}

/**********************  GetVarInStatsOutput()  *****************************/

GetVarInStatsOutput(index)
int index;
/*
return whether or not the given index is set
*/
{
 return VarInStatsOutput[index];
}

/************************  SetVarInStatsOutput()  *****************************/

SetVarInStatsOutput(index,selected)
int index,selected;
/* set the given index to value of selected
*/
{
 VarInStatsOutput[index]=selected;
}

/***************************************************************************/

char *
GetNextStatsSeln()
{
 static char varname[NAMLEN];

 if ( (fscanf(StatsSelnsFile,"%s",varname))!=1)
  return (char *)NULL;
 return varname; 
}

/**********************  GET_NEXT_STATS_SELN()  ****************************/

get_next_stats_seln()
{
int selection;

 if ( (fscanf(StatsSelnsFile,"%d ",&selection))!=1)
  return FALSE;
 return selection; 
}

/**********************  OpenStatsSelnsFile()  ****************************/

OpenStatsSelnsFile(mode)
char *mode;
{
 (void)sprintf(StatsSelnsFileName,STATS_SELNS,ProjectDirectory,get_proj_number());
 if ( (StatsSelnsFile=WINDSfopen(StatsSelnsFileName,mode)) == NULL) {
  if (!strcmp(mode,"w")) {
   time_to_log();
   (void)fprintf(stderr,"WARNING: can't open StatsSelnsFile for writing\n");
   perror("StatsSelnsFile");
   return FALSE;
  } else {
   time_to_log();
   (void)fprintf(stderr,"WARNING: can't open StatsSelnsFile for reading\n");
   perror("StatsSelnsFile");
   return FALSE;
  }
  return FALSE;
 } 
 return TRUE;
}

/******************************************************************************/

WriteStatsSelnsFile(names,numnames)
char **names;
int numnames;
{
int seln;

 if (!OpenStatsSelnsFile("w"))
  return;
 for (seln=0; seln<numnames; seln++)
  (void)fprintf(StatsSelnsFile,"%s\n",names[seln]);
 (void)WINDSfclose(StatsSelnsFile);
}

/******************************************************************************/

char *
GetStatsFileDir()
{
 static char filename[256];

 (void)sprintf(filename,"%s/%s",(char *)GetWindsHostsPath(),
  (char *)GetDisplayHost());
 return filename;
}

/******************************************************************************/

char *
GetStatsPrefix()
{
 return STATS_PREFIX;
}

/******************************************************************************/

char *
GetStatsFilename(suffix)
int suffix;
{

 (void)sprintf(StatsOutputFilename,STATS_FILENAME,
  (char *)GetWindsHostsPath(),(char *)GetDisplayHost(),
  GetStatsPrefix(),suffix);
 return StatsOutputFilename;
}

/***************************  OPEN_OUTPUT_FILE()  *****************************/

OpenStatsOutputFile()
/* prepare file for statistics output
*/
{
int status;

 status=OK;
 if ( (stream=WINDSfopen(
  (char *)GetStatsFilename(StatsFileNumber),"w"))==NULL) {
  time_to_log();
  (void)fprintf(stderr,"WARNING: can't open %s in lstats.c\n",
   StatsOutputFilename);
  (void)WINDSfclose(stream);
  return ERROR;
 }
 return OK;
}

/***************************  PRINT_STATS()  *********************************/

print_stats()
/* 
print 'em out; open new intermediate file
*/
{
 (void)fprintf(stderr,"%s flush stats \n",current_time);
 FlushStats=FALSE;
 headers_printed=0;
 if (OpenStatsOutputFile()!=ERROR) {
  print_stat_header();
  lstatout();
  AddToStatsFlushMenu(StatsFileNumber++,FlushStartTime,FlushEndTime);
  (void)WINDSfclose(stream);
  if (GetStatsAutoPrintEnabled() && get_lstat_port()!=ERROR)
//   NotifyPrinter(0,get_lstat_port(),STATISTICS,StatsOutputFilename);
   NotifyPrinter(0,get_lstat_port(),TEXTFILE,StatsOutputFilename);
 } 
 clear_buffers();
}

/*************************  PRINT_STAT_HEADER  *******************************/

print_stat_header()
{
static int first_entry=TRUE;
static char *year,*month,*day;
char *get_year(),*get_month(),*get_day();

 if (first_entry) {
  first_entry=FALSE;
  month=get_month();
  year=get_year();
  day=get_day();

 }
 (void)sprintf(FlushStartTime,"%02d:%02d:%02d",
    StartPeriodHr,StartPeriodMinute,StartPeriodSecond);
 (void)sprintf(FlushEndTime,"%02d:%02d:%02d",
  EndPeriodHr,EndPeriodMinute,EndPeriodSecond);
 (void)sprintf(&sock_buff[0],"\n\n%s  FLT # %s   DATE %s %s %s \nStatistics on %d samples from %s to %s\n",project,get_fltno(),month,day,year,print_total,FlushStartTime,FlushEndTime);
 (void)fprintf(stream,"%s",sock_buff);
/*
note that the following spaces are hardwired to correspond to formats 
specified for NAMLEN and UNITSLEN in lstatout(). Change these as needed.
*/
 (void)sprintf(StatsFlushListTitle,
  " Name            Units          Mean         Sigma       Maximum       Minimum");
 (void)sprintf(&sock_buff[0],"%s\n\n",StatsFlushListTitle);
 (void)fprintf(stream,"%s",sock_buff);
 headers_printed++;
 rows_printed=0;
}

/****************************************************************************/

char *
GetStatsFlushListTitle()
{
 return StatsFlushListTitle;
}

/***************************  LSTATOUT()  ***********************************/

lstatout()
{
float count,mean,arg,sigma;
int i;
static char name[NAMLEN];

 count = (float)NumSecondsAccum;
 for (i=0;i<GetNumMenuEntries();i++) {
/***
  (void)sprintf(name,"%s",(i<*len_raw?&raw_list[i*NAMLEN]:&derive_list[(i-*len_raw)*NAMLEN]));
***/
  if (GetVarInStatsOutput(i)) {
   (void)sprintf(name,"%s",(char *)GetMenuListName(i));
   mean = sum[i]/count;
   arg = (sumsq[i]-sum[i]*sum[i]/count)/(count-1.);
   if(arg<0) arg = -arg;
   sigma = sqrt((double)arg);
   if (spec_index[i]!=ERROR)  { /* if there is a plot spec for this var ... */
/*
note that the following are hardwired formats for NAMLEN and UNITSLEN, i.e.
values 13 and 12 in the format specs. Change these as needed.
*/
    sprintf(sock_buff, "%-12s  %8s%14.4f%14.4f%14.4f%14.4f\n",
     name,&units[spec_index[i]*UNITSLEN],mean,sigma,maximum[i],minimum[i]);
   } else {/* variable without a plot specification in ../proj/<#>/plot_attr */
    sprintf(sock_buff, "%-12s       ???%14.4f%14.4f%14.4f%14.4f\n",
     name,mean,sigma,maximum[i],minimum[i]);
   }
   (void)fprintf(stream,"%s",sock_buff);
   if (++rows_printed >= STATS_ROWS)
    print_stat_header();
  }
 }

/* send extra line to flush buffer at socket
*/
  fprintf(stream,"                                                     \n");
}

/**********************  NEXT_END_INTERVAL()  ****************************/

next_end_interval(current,interval)
int current,interval;
/* 
find the next appropriate interval end second, given the current second and 
interval desired
*/
{
 if (current+interval>59)
  return 60;

 while ((current++)%interval);
 
 return(current-1);
}

/***************************************************************************/

GetNeedStatsInit()
{
 return NeedStatsInit;
}

/***************************************************************************/

SetNeedStatsInit(init)
int init;
{
  NeedStatsInit=init;
}

/***************************************************************************/

SetStatsAutoPrintEnabled(print)
int print;
{
 stats.printing=print;
}

/***************************************************************************/

GetStatsAutoPrintEnabled()
{
 return stats.printing;
}

/***************************************************************************/

SetCurrentStatsInterval(value)
int value;
{
 stats.interval=value;
}

/***************************************************************************/

GetCurrentStatsInterval()
{
 return stats.interval;
}

/**********************  FLUSH_ACCUMULATIONS()  ****************************/

FlushStatsAccumulations()
/* 
statistics interval has been changed; set end dump time so that a dump will
be initiated either at the end of the current interval or at the end of 
the current minute, whichever is sooner

also called when stats.printing is toggled on with same effect, sans any 
change to stats.interval
*/
{
 EndPeriodTotalSeconds=MIN(EndPeriodTotalSeconds,
  CurrentHr*3600+(CurrentMinute)*60+
  next_end_interval(CurrentSecond,stats.interval));

/* 
set flag to flush stats at time = EndPeriodTotalSeconds, unless this was 
simple change of interval while stats.printing was off 
*/
 FlushStats=TRUE;   
 SetNextFlushString();
 ChgFlushMessage(NextFlushString);
}

/**************************************************************************/

char *
GetNextFlushString() 
{
 return NextFlushString;
} 

/*****************************************************************************/

SetNextFlushString()
{
 (void)sprintf(NextFlushString,"%02d:%02d:%02d",
  EndPeriodTotalSeconds/3600,
  (EndPeriodTotalSeconds%3600)/60,
  (EndPeriodTotalSeconds%3600)%60);
}

/***********************  CHECK_STATS_INTERVAL()  ****************************/

StatsIsFlushed(hr,minute,sec)
int hr,minute,sec;
/*  
update current lstat minute and hour with arguments sent; 
if it's time to perform dump, initiate that process and reset start and end 
times for next one

if it's dump time and statistics printing is enabled, return TRUE; else, 
return FALSE
*/
{
 int total_rows,CurrentTotalSeconds;

 CurrentMinute=minute;
 if (hr-CurrentHr<0) 
/* 
we've either crossed midnite or possibly system time has been reset with
an hour value smaller than previous; to facilitate the arithmetic,
increment lstat hour after crossing midnite instead of doing modular math.
Note that at real-time startup, initial time may be changed to a different
one when the header is received by the windComm process, resulting in many
consecutive seconds of adjustment to next stats flush time until catchup
is accomplished. This is a nuisance in the log file, but harmless.
*/
  CurrentHr=hr+24; 
 else
  CurrentHr=hr; 
 CurrentSecond=sec;
 
 CurrentTotalSeconds=CurrentHr*3600+CurrentMinute*60+CurrentSecond;
 if (CurrentTotalSeconds>=EndPeriodTotalSeconds){
/* 
current lstat time has gone past the designated end time for stats flush.
Save values NOW to avoid add'l accumulation while waiting for I/O, etc., since 
AccumulateStats() is being called by a one-second timer  
*/
  save_stats(); 
/*
calculate end time hr, minute, second for output header
*/
  EndPeriodHr=((EndPeriodTotalSeconds)/3600)%24;
  EndPeriodMinute=((EndPeriodTotalSeconds)/60)%60;
  EndPeriodSecond=(EndPeriodTotalSeconds)%60;
  print_stats();
/*
calculate new start time hr, minute, second for output header
*/
  StartPeriodHr=((CurrentTotalSeconds+1)/3600)%24;
  StartPeriodMinute=((CurrentTotalSeconds+1)/60)%60;
  StartPeriodSecond=(CurrentTotalSeconds+1)%60;
/*
increment end time counter
*/
  EndPeriodTotalSeconds=CurrentTotalSeconds+stats.interval;
  SetNextFlushString();
  ChgFlushMessage(NextFlushString);

  if (headers_printed) {
/* 
if true, then print_stats() did a printout 
*/
   total_rows=headers_printed*HEADER_ROWS+    /* lines for each header ... */
              (headers_printed-1)*STATS_ROWS  /* lines per complete page ... */
              +rows_printed;                  /* lines on last page */
   headers_printed=0;
   return(total_rows); 
  }
 }
 return FALSE;
}

/*****************************  INIT_LSTATS()  ******************************/

init_lstats(hr,minute,sec)
int hr,minute,sec;
/* initialize stats time markers and accumulation arrays; open intermediate file
   for dumps to printer
*/
{
 int i;
 int dbl_words;

  dbl_words=(int)(GetNumMenuEntries());
  if (sum) {

/* then assume all of below have already been malloc()'d once; free them to
   properly manage things so this routine can be recalled.
*/
   free((char *)sum);
   free((char *)sumsq);
   free((char *)minimum);
   free((char *)maximum); 
   free((char *)spec_index); 
  }
  sum=(double *)malloc((unsigned)(dbl_words*sizeof(double)));
  sumsq=(double *)malloc((unsigned)(dbl_words*sizeof(double)));
  minimum=(double *)malloc((unsigned)(dbl_words*sizeof(double)));
  maximum=(double *)malloc((unsigned)(dbl_words*sizeof(double)));
  spec_index=(int *)malloc((unsigned)(dbl_words*sizeof(int)));

  CurrentHr=StartPeriodHr=hr;
  CurrentMinute=StartPeriodMinute=minute;
  CurrentSecond=StartPeriodSecond=sec;

  EndPeriodTotalSeconds=CurrentHr*3600+CurrentMinute*60+stats.interval; 
  clear_buffers();
  SetNextFlushString();
  ChgFlushMessage(NextFlushString);
/* 
get units for all variables. 
*/
 for (i=0; i<GetNumMenuEntries(); i++) {
  spec_index[i]=GetMenuListAttrIndex(i);
 }
 SetNeedStatsInit(FALSE);
}

/**************************  SAVE_STATS()  *********************************/

save_stats()
/* save statistics totals in separate place to protect them until printout
   is done
*/
{
 print_total=NumSecondsAccum;
}

/***************************  CLEAR_BUFFERS()  *******************************/

clear_buffers()
{
  int i;
  NumSecondsAccum = 0;
  for(i=0;i<GetNumMenuEntries();i++)
	{
	sum[i] = 0;
	sumsq[i] = 0;
	maximum[i] = -99999.;
	minimum[i] = 99999.;
	}
}

/***********************  SUM_LSTATS()  **************************************/

AccumulateStats(time_slice)
int time_slice;
{
int var;
double arg;

 for (var=0; var<GetNumMenuEntries(); var++) {
  if (VarIsRaw(var)) {
   arg=(double)GetRawDataValue(time_slice**len_raw+GetMenuListIndex(var));
  } else {
   arg=(double)GetDeriveDataValue(time_slice**len_derive+GetMenuListIndex(var));
  }
  sum[var] += arg;
  sumsq[var] += arg*arg;
  maximum[var]=MAX(arg,maximum[var]);
  minimum[var]=MIN(arg,minimum[var]);
 }  /* for # of variables */
 NumSecondsAccum++;
}

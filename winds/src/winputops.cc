/*
-------------------------------------------------------------------------
OBJECT NAME:	winputops.c

FULL NAME:	winput Operations

ENTRY POINTS:	main()

STATIC FNS:	

DESCRIPTION:	Read and process real-time data or netCDF files and place
		data into shared memory for WINDS.

INPUT:		

OUTPUT:		

REFERENCES:	

REFERENCED BY:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1995
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "ctape.h"
#include "raf.h"

#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <ctype.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h> 
#include <stdlib.h> 

#include <shmem.h>
#include <lengths.h>
#include <files.h>
#include <network.h>
#include <eth_shm.h>
#include <pms1d.h>
#include <socket.h>
#include <config.h>
#include <macros.h>
#include <ctl_panel.h>
#include <wproto.h>
#include "qc.h"

// Comment out if we don't want to link against QC program.
//#define QC

static int infd;
static int set_start_time=FALSE;
static int last_recs_read=0;
static char buff[MAX_SOCKET_BUFF_LNGTH];

static int BroadCast = BCAST_NONE;
static int start_record=ERROR;	/* position in data file from whence data
				   history start */
static int requested_start_record=0;
				/* pending request for new position in data 
			   	   file from whence data history start. NOTE
				   that this must be initialized to zero, and
			  	   start_record to ERROR so that startup cond'ns
			 	   are correct, since they must have different
				   values on first examination in function
				   adjust_start_record() */
static int StartTime=0;
static int DayOfYear=0;
static char TZString[64];


/*********************  GLOBAL, DEFINED HERE ******************************/
int FlightDate[3];
int recs_read;
int recs_to_read;
int first_frame_seconds=0;
int start_hr,start_minute,start_sec;  /* initial start times */
int update_rate=1;
int total_seconds=0;   /* how many seconds have been processed -- note
                                 that *elapsed_time is how many of them have
                                 been stuffed into shared memory */
struct itimerval itimeit;          /* timer -- check for new data */


long LRlen; /* size in bytes of each physical record read in */

char	*progName = "WINPUT:";


/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  Mode = REALTIME;
  QCenabled = FALSE;

  DoWinputInitStuff();
  DoWinputStartupStuff(argv, argc); 
  DoWinputShmemStuff();

  FinishWinputShmemStuff();
  FinishDataInitStuff();

  LoopAndGetDataToShmem();

  return(0);

}	/* END MAIN */

/* -------------------------------------------------------------------- */
void DoDataInitStuff()
{
 int  i;

 /* already done once for this mode, don't do extra work
  */
 if (GetInputMode()!=RAWFILE_NETCDF) {
  DecodeHeader(header_filename);
  infd = open(header_filename, O_RDONLY);
  RunAMLIBinitializers();
  }

#ifdef QC
 if (QCenabled && qc_rt_init() == ERR) {
  QCenabled = FALSE;
  /* Pop up message some day. */
  }
#endif

 if (infd==ERROR) {
  AppendToSearchPath(header_filename);
 }
 if (GetInputMode()==DISK_DATA || GetInputMode()==RAWFILE_NETCDF)
  SetDatasetIsFound(infd!=ERR?DATASETISFOUND:DATASETNOTFOUND);
 else
  (void)close(infd);

 SetUpPMS1D_SHMEM();
 AllocateDataArrays();

 /* For realtime we want ADSrecord pointing to shared mem, so free it up.
  * Ditto for AVAPS.
  */
 free((char *)ADSrecord);  /* De-allocate ADSrecord	*/
 ADSrecord = (char *)eth_shm->syncBuf;

 for (i = 0; i < MAX_AVAPS; ++i)
   {
   free((char *)AVAPSrecord[i]);
   AVAPSrecord[i] = (char *)eth_shm->avapsBuf[i];
   }
 
 get_lrlen(&LRlen);
 *len_raw = nsdi;
 *len_derive = nraw+nderive;
 if(*len_raw>MAX_RAW) {
   NotifyWINDS("FATAL: Input process has exited due to exceeding MAXRAW with # of raw variables. This WINDS process will now also exit...contact a WINDS expert about this problem.",TRUE);
 }
 ProjectNumber=proj_number;
}

/**************************  LoopAndGetDataToShmem() *************************/
void LoopAndGetDataToShmem()
{
 RunStandardWINDSLoop();
 (void)fprintf(stderr,"WINPUT WARNING: endless while loop just ended!\n");
 cleanup();
}

/********************  DoWinputInitStuff()  ********************************/
void DoWinputInitStuff()
{
/*
establish signal handlers
*/
 if ((int)signal(SIGINT,catchint) < 0) {
  perror("sigset, input catchint");
 }
}

/************************  InitRawDataMode()  *********************************/
void InitRawDataMode()
{
  int	in, jn; 
  
  /* due to convoluted implementation this mode doesn't easily init things
   * in correct order. Kluge it here. Must call DecodeHeader() before
   * calling InitAircraftDependencies().
   */
  if (GetInputMode() == RAWFILE_NETCDF)
    {
    DecodeHeader(header_filename);
    infd = open(header_filename, O_RDONLY);
    }
  else
    {
    InitAircraftDependencies();
    GenerateComputeOrder(); 
    }

  ReadBroadcastVariables();


  /* Transfer names to shared memory.
   */
  for (in = 0; in < nsdi; ++in) 
    (void)strcpy(&raw_list[in*NAMLEN], sdi[in]->name);

  for (in = 0; in < nraw; ++in)
    (void)strcpy(&derive_list[in*NAMLEN], raw[in]->name);

  jn = in;                                                      
  for (in = 0; in < nderive; ++in, ++jn)
    (void)strcpy(&derive_list[jn*NAMLEN], derived[in]->name);
 
}

/******************   CHECK_FOR_MIDNITE()  ************************************/
void check_for_midnite(long offset)
{
long session,temp;
char date_buff[MAX_SOCKET_BUFF_LNGTH];

 if ( (temp=get_day_of_year(StartTime+offset))!=DayOfYear) {
  DayOfYear=temp;
/*
if this session is not described in netconfig file, get_num_winds_sessions() 
will equal zero. Allow for these types of sessions by using the greater of one
or get_num_winds_sessions() as limit in for loop.
*/
  for (session=0; session<MAX(1,get_num_winds_sessions()); session++) {
   (void)sprintf(&date_buff[0],"%d",REQ_CHG_DATE);
   if (GetProtocolDebug())
    (void)fprintf(stderr,
     "WINPUT PROTOCOL: notify INTERWINDS_TALK about need for changing date\n");
   if (write_to_socket(INTERWINDS_TALK+session,&date_buff[0],strlen(&date_buff[0]))==EBADWRITE)
    perror("acknowledge response write_to_socket");
   (void)sprintf(&date_buff[0],"%s",(char *)get_date_from_base(StartTime+offset));
   if (GetProtocolDebug())
   (void)fprintf(stderr,
    "WINPUT PROTOCOL: notify INTERWINDS_TALK with new date\n");
   if (write_to_socket(INTERWINDS_TALK+session,&date_buff[0],strlen(&date_buff[0]))==EBADWRITE)
    perror("acknowledge response write_to_socket");
   (void)fprintf(stderr,"NOTICE: CHG DATE REQUEST on %s, to %s at %s\n",
    (char *)get_display_host(session),date_buff,(char *)get_time_from_base(StartTime+offset));
  }
 }
}

/******************   SET_START_DATE()  ************************************/
int set_start_date(int index)
{
  int	month,day,year;
  char	*s;
  struct tm tm;
  static int init=FALSE;

 if (init)
  return TRUE;

 if (GetInputMode()==NETCDF) {
/* 
analysis mode -- get date from info in data file
*/
  s=(char *)get_netcdf_date();
  (void)sscanf(s,"%d/%d/%d",&month,&day,&year);
 } else {
/* 
real-time mode -- get date as of today
*/
  month=get_month_num();
  (void)sscanf((char *)get_day(),"%d",&day);
  (void)sscanf((char *)get_year(),"%d",&year);
  if (year < 70)
    year += 100;
 }
/*
assuming time has been initialized correctly, get time from shared mem; init
tm struct, convert to basetime, save for later reference.
*/
 if (hhmmss[index*3]!=0 || hhmmss[index*3+1]!=0 || hhmmss[index*3+2]!=0) {
  tm.tm_hour=hhmmss[index*3];
  tm.tm_min=hhmmss[index*3+1];
  tm.tm_sec=hhmmss[index*3+2];
  tm.tm_mon=month-1;
  tm.tm_mday=day;
  tm.tm_year=year;
  tm.tm_isdst=0;
/*
set StartTime to # of seconds since the epoch, in CUT
*/
  (void)sprintf(TZString,"%s","TZ=GMT");
  (void)putenv(TZString);
  (void)fprintf(stderr,"WINPUT: time zone used is %s\n",(char *)getenv("TZ"));
  StartTime= mktime(&tm);
  init=TRUE;
  (void)fprintf(stderr,"WINPUT has set StartTime to %s on ",
   (char *)get_time_from_base(StartTime));
  (void)fprintf(stderr,"%s\n",(char *)get_date_from_base(StartTime));
  DayOfYear=get_day_of_year(StartTime);
  (void)fprintf(stderr,"WINPUT has set DOY at %d\n",DayOfYear);
 }

 FlightDate[0] = month, FlightDate[1] = day, FlightDate[2] = year;

 if (FlightDate[2] < 1000)
   {
   if (FlightDate[2] > 90)
     FlightDate[2] += 1900;
   else
     FlightDate[2] += 2000;
   }

 return init;
}

/******************   GET_DAY_OF_YEAR()  ************************************/
int get_day_of_year(long basetime)
{
struct tm *tm;
  tm = (struct tm *)gmtime(&basetime);
  return tm->tm_yday;
}

/******************   GET_DISK_SDIREC()  ************************************/
int get_disk_sdirec()
{
  int	insize;
  ushort	first_word;
  char	*pms2d_buff;
  int status;

  do
    {
    insize = sizeof(short);

    status = read(infd, (char *)&first_word, insize);

    switch (status)
      {
      case 0:
        return(EOF);

      case -1:
        perror("WINPUT read, FATAL");
        NotifyWINDS("FATAL: Input process has exited due to an error in reading the disk data file. This WINDS process will now also exit...contact a WINDS expert about this problem.",TRUE);
        break;

      default:

        switch (first_word)
          {
          case SDIWRD:
            insize = LRlen - sizeof(short);
            ((ushort *)ADSrecord)[0] = first_word;
            read(infd, (void *)&ADSrecord[sizeof(short)], insize); 
            break;

          case PMS2DP1: case PMS2DP2:
          case PMS2DC1: case PMS2DC2:
          case PMS2DH1: case PMS2DH2:
            pms2d_buff = (char *)eth_shm->twoBuf;
            insize = sizeof(struct P2d_rec) - sizeof(short);
            ((ushort *)pms2d_buff)[0] = first_word;
            read(infd, (void *)&pms2d_buff[sizeof(short)], insize); 
            eth_shm->two.rseq++;
            eth_shm->two.rlen = sizeof(struct P2d_rec);
            lseek(infd, 6*sizeof(struct P2d_rec), SEEK_CUR);
            break;

          case AVAPS_WORD:
            break;
/*
          case PMS2DG1: case PMS2DG2:
            pms2d_buff = (char *)eth_shm->greyBuff;
            insize = sizeof(struct P2d_rec) - sizeof(short);
            ((ushort *)pms2d_buff)[0] = first_word;
            read(infd, &pms2d_buff[sizeof(short)], insize); 
            eth_shm->grey.rseq++;
            eth_shm->grey.rlen = sizeof(struct P2d_rec);
            break;
*/
          }
      }
    }
  while (first_word != SDIWRD);

  return(TRUE);

}	/* END GET_DISK_SDIREC */

/********************  ADJUST_NETCDF_START_TIME()  ***************************/
int adjust_netcdf_start_time()
/*
adjust user requests for start time so that requests for before start of file
become the start of file, and so that framing forward results in a partial 
display of data to indicate end of file.  At that end of file point, a frame
backward will maintain the same frames as seen framing forward, while a frame
forward will display a full frame of data and subsequently alter the frames
seen on the next frame back.  Returning backwards to start of file will then
reset the start times so that frames are identical to those seen initially.

Requests identical to the current start point are ignored, unless there has
been a change to the update rate.
*/
{
int start,chg_rate;
static int last_rate=0;
/*
preserve current update rate if it has changed from original
*/
 chg_rate=FALSE;
 if (last_rate!=update_rate) {
  last_rate=update_rate;
  chg_rate=TRUE;
 }
/*
no less than zero
*/
 start=MAX(requested_start_record,0);
/*
if later than last record in file, adjust to show one last full frame of data
*/
 if (start>(get_netcdf_dataset_size()-1))
  start=MAX(0,get_netcdf_dataset_size()-update_rate*get_time_frame_size());
/*
check for request identical to current position
*/
 if (!chg_rate && start==start_record) {
  (void)fprintf(stderr,"WINPUT: start request identical to current value\n");
  return ERROR;
 } else {
  if (start!=requested_start_record)
   (void)fprintf(stderr,"WINPUT: adjust netCDF start request to %d\n",start);
  return start; 
 }
}

/*************************************************************************/
bool NoStartFlagFileExists()
{
 char NoStartWinputFilename[MAX_FLNM_LNGTH];
 FILE *handle;

 (void)sprintf(&NoStartWinputFilename[0],NO_START_INPUT,
  (char *)getenv("WINDS"),proj_number);
 if ((handle=(FILE *)WINDSfopen(NoStartWinputFilename,"r"))==NULL) {
  WINDSfclose(handle);
  return FALSE;
 } else {
  WINDSfclose(handle);
  return TRUE;
 }
}

/************************  CATCHALRM()  **********************************/
void catchalrm(int sig)
{
char *s;
int length,disp_req;
 
/*
check that parent WINDS has not exited without telling us. If start-flag
file exists, however, then WINDS Control Host has exited with intention
of leaving winput running. Don't exit.
*/
 if (getppid()==1 && !NoStartFlagFileExists()) {
  (void)fprintf(stderr,"WARNING: Winput process has determined that WINDS has terminated. The winput process will now also exit.\n");
  cleanup();
 }
/* 
check for display side commands/requests
*/
 length=MAX_SOCKET_BUFF_LNGTH;
 s=(char *)read_from_socket(DISP_REQ,&length,NON_BLOCKING);
 if (length>0) {
  if (GetProtocolDebug())
  (void)fprintf(stderr,"WINPUT PROTOCOL: got msg from DISP_REQ_REPLY...\n");
  (void)sscanf(s,"%d",&disp_req);
  (void)sprintf(&buff[0],"%d",LISTENING);
  if (GetProtocolDebug())
   (void)fprintf(stderr,
    "WINPUT PROTOCOL: respond to DISP_REQ_REPLY that WINPUT is listening\n");
  if (write_to_socket(DISP_REQ_REPLY,&buff[0],strlen(&buff[0]))==EBADWRITE)
   perror("acknowledge response write_to_socket");
  switch (disp_req) {
/*** not yet being used, but keep around for when protocol is adjusted
to allow checks on winput still running after starting to process 
dataset
   case ISDATASETFOUND:
    NotifyDatasetFound();
    break;
***/
   case ARE_YOU_LISTENING:
    (void)fprintf(stderr,"WINPUT responds to an ARE_YOU_LISTENING request\n");
/*
nothing more to do, we've already responded that we are LISTENING just
before getting here
*/
    break;
   case REQ_DATE:
    s=(char *)get_netcdf_date();
    if (GetProtocolDebug())
     (void)fprintf(stderr,
      "WINPUT PROTOCOL: respond to DISP_REQ_REPLY with netcdf date\n");
    if (write_to_socket(DISP_REQ_REPLY,&s[0],strlen(&s[0]))==EBADWRITE)
     perror("return date response write_to_socket");
    break;
   case REQ_BCAST:
    length=MAX_SOCKET_BUFF_LNGTH;
    s=(char *)read_from_socket(DISP_REQ,&length,BLOCK_TIME_OUT);
    if (strcmp(s,NODATA)) {
     (void)sscanf(s,"%d",&BroadCast);
     (void)fprintf(stderr,
      "WINPUT got BROADCAST request value %d...\n",BroadCast);
     if (BroadCast)
      EnableBroadcasting();
     else
      DisableBroadcasting();
    } else {
     (void)fprintf(stderr,
      "WARNING: WINPUT got BROADCAST request but no broadcast value...\n");
    }
    break;
   case REQ_FREEZE:
    Freeze=TRUE;
    break;
   case REQ_UNFREEZE:
    Freeze=FALSE;
    break;
   case REQ_SET_START_TIME:
    length=MAX_SOCKET_BUFF_LNGTH;
    if (GetProtocolDebug())
     (void)fprintf(stderr,
      "WINPUT PROTOCOL: got request to set start time; waiting on time...\n");
    s=(char *)read_from_socket(DISP_REQ,&length,BLOCK_TIME_OUT);
    if (strcmp(s,NODATA)) {
     static int init=FALSE;
     int proceed;
     if (GetProtocolDebug())
      (void)fprintf(stderr,"WINPUT PROTOCOL: got set start time\n");
/* read in a total seconds value, expected to be computed from SOF; convert to
   total bytes to lseek in file from start.
*/
     (void)sscanf(s,"%d",&requested_start_record);
     (void)fprintf(stderr,"WINPUT: got start time request for %d\n",
      requested_start_record);
     proceed=TRUE;
     if (requested_start_record<0) {
/*
allow for midnite crossover by subtracting any negative number from the number
of seconds in a day, simulating modular math.
*/
      requested_start_record=86400+requested_start_record;
/*
but do NOT allow user specified start times that are OUTSIDE of the dataset to
change anything. We've already covered less than zero above; now check with
size of dataset to see if adjusted request is past EOF; don't use the request in
this case.

This is distinct from framing where adjustments ARE made and honored.  This
philosophy is chosen to minimize confusion to user who asks for a specific time:
the time returned should not be an adjusted one, it should be as requested or
unchanged.
*/
      if (requested_start_record>(get_netcdf_dataset_size()-1))
       proceed=FALSE;
     } else {
/*
request greater than zero; verify not greater than dataset size
*/
      if (requested_start_record>(get_netcdf_dataset_size()-1))
       proceed=FALSE;
     }
/*
The only exception to this is on startup, where the display side calls here
with a value of zero, to imply start at start of file.
*/
     if (!init) {
      proceed=init=TRUE;
     } 
     if (proceed) {
      (void)fprintf(stderr,
       "Proceeding to set user-requested start time\n");
      set_start_time=TRUE;
      update_data_history(NETCDF);
     } else {
      (void)fprintf(stderr,"WINPUT: request outside dataset... do nothing\n");
      (void)sprintf(&buff[0],"%d",NO_SHMEM_REFILLED);
      if (write_to_socket(DISP_REQ_REPLY,&buff[0],strlen(&buff[0]))==EBADWRITE)
       perror("frame jump response write_to_socket");
     }
    } else {
     if (GetProtocolDebug())
      (void)fprintf(stderr,"WINPUT PROTOCOL: got NO start time to set!\n");
     else
      (void)fprintf(stderr,
       "WARNING: WINPUT got START_TIME command but no start time...\n");
    }
    break;
   case REQ_SET_UPDATES:
    length=MAX_SOCKET_BUFF_LNGTH;
    s=(char *)read_from_socket(DISP_REQ,&length,BLOCK_TIME_OUT);
    if (strcmp(s,NODATA)) {
     int temp;
     (void)sscanf(s,"%d",&temp);

     (void)fprintf(stderr,
      "WINPUT got SET_UPDATES command for rate %d...\n",temp);
     if (GetInputMode()==NETCDF) {
      update_rate=temp;
      set_start_time=TRUE;
      update_data_history(NETCDF);
     } else {

/* development or one of the ADS data modes (onboard or networked)
*/
      update_rate=temp;
      (void)sprintf(&buff[0],"%d",SHMEM_REFILLED);
      if (GetProtocolDebug())
       (void)fprintf(stderr,
        "WINPUT PROTOCOL: notify DISP_REQ_REPLY that shared mem is refilled\n");
      if (write_to_socket(DISP_REQ_REPLY,&buff[0],strlen(&buff[0]))==EBADWRITE)
       perror("frame jump response write_to_socket");
     }
    } else {
     (void)fprintf(stderr,
      "WARNING: WINPUT got SET_UPDATES command but no update rate...\n");
    }
    break;
   case REQ_SEARCH_BACK:
   case REQ_SEARCH_FWD:

    (void)fprintf(stderr,"WINPUT: got time frame request at et %d\n",
     *elapsed_time);
    if (disp_req==REQ_SEARCH_FWD) {
     requested_start_record=start_record+update_rate*get_time_frame_size();
    } else {
     requested_start_record=start_record-update_rate*get_time_frame_size();
    }
    set_start_time=TRUE;
    update_data_history(NETCDF);
    break;
   case RESET_PARCEL:
    length=MAX_SOCKET_BUFF_LNGTH;
    s=(char *)read_from_socket(DISP_REQ,&length,BLOCK_TIME_OUT);
    if (strcmp(s,NODATA)) {
     int temp;
     (void)sscanf(s,"%d",&temp);
     (void)fprintf(stderr,
      "WINPUT got RESET_PARCEL command for parcel %d...\n",temp);
     parcel_reset(temp);
    } else {
     (void)fprintf(stderr,
      "WARNING: WINPUT got RESET_PARCEL command but no rate...\n");
    }             
    break;
   case REQ_SET_RATE:
    length=MAX_SOCKET_BUFF_LNGTH;
    s=(char *)read_from_socket(DISP_REQ,&length,BLOCK_TIME_OUT);
    if (strcmp(s,NODATA)) {
     int temp;
     (void)sscanf(s,"%d",&temp);
     (void)fprintf(stderr,
      "WINPUT got SET_RATE command for rate %d...\n",temp);
     SetInputRate(temp);
    } else {
     (void)fprintf(stderr,
      "WARNING: WINPUT got SET_RATE command but no rate...\n");
    }
    break;
   default:
    break;
  }
 }

 if (!Freeze)
  update_data_history(GetInputMode());
}

/****************************  UPDATE_DATA_HISTORY()  ************************/
void update_data_history(int inputmode)
/* get data and process it as per inputmode
 */
{
 int temp;
 static int index;               /* current second block in shared memory */

if (inputmode==NETCDF) {
 if (set_start_time) {
/*
this call is made as result of request from display side to explicitly set the
start time of the time frame.
*/
  set_start_time=FALSE;
  if ((temp=adjust_netcdf_start_time())!=ERROR) {
   start_record=temp;
   *elapsed_time=get_netcdf_data((long *)hhmmss,(float *)data,start_record,
     get_time_frame_size(),update_rate);
   (void)fprintf(stderr,"WINPUT: shmem refilled with %d points starting at %d\n",
    *elapsed_time,start_record);
   (void)sprintf(&buff[0],"%d",SHMEM_REFILLED);
   if (set_start_date(0))
/*
deduct 1 from file position because file pointer is one ahead of actual offset,
which starts at zero for the first record read
*/
    check_for_midnite(get_netcdf_fileposn()-1);
  } else {
/*
some kind of problem with requested start time. Do nothing.
*/
   (void)sprintf(&buff[0],"%d",NO_SHMEM_REFILLED);
   (void)fprintf(stderr,"WINPUT: shmem refill NOT DONE; start time remains %d\n",
    start_record);
  }
  if (write_to_socket(DISP_REQ_REPLY,&buff[0],strlen(&buff[0]))==EBADWRITE)
   perror("frame jump response write_to_socket");
 } else {
/*
this call as part of normal playback mode, with Freeze set FALSE
*/
  if ((temp=get_netcdf_fileposn())<(get_netcdf_dataset_size()-1)) {
   if (
    get_netcdf_data((long *)&hhmmss[3*(*elapsed_time%get_datablock_size())],
	    (float *)&data[*len_derive*(*elapsed_time%get_datablock_size())],
	    temp,1,update_rate) > 0) {
    (*elapsed_time)++;
    start_record+=update_rate;
    if (set_start_date(0))
     check_for_midnite(get_netcdf_fileposn()-1);
   }
  }
 }
} else do {
 index=(*elapsed_time%get_datablock_size());
 if (get_data()) {
  if (inputmode==DISK_DATA || inputmode==ADS_DATA || inputmode==RAWFILE_NETCDF){
   if (!(++total_seconds%update_rate)){
    DecodeADSrecord((short *)ADSrecord, SampledData);
    ApplyCalCoes(SampledData);
    AverageSampledData();

#ifdef QC
    if (QCenabled && qc_rt_exec() == ERR) {
     QCenabled = FALSE;
     qc_rt_cleanup();
    }
#endif
    ComputeLowRateDerived();
    if (inputmode!=RAWFILE_NETCDF) {
     TransferDataToSHMEM(AveragedData, index);
     WritePMS1DdataToSHMEM(AveragedData);
     if (set_start_date(index))
      check_for_midnite(total_seconds-1);
     (*elapsed_time)++;
    } else {
/*     WriteNetCDF();
     SyncNetCDF(); */
     update_data_history(NETCDF);
    }
    BroadcastSyncData();
   } 
  } else if (inputmode==NETCDF) {
   if (set_start_date(index))
/*
check for midnite only if unfrozen and not refilling shmem, or last record is 
read in a frame jump/set start time operation
*/
    if ((!refill_shmem && !Freeze) || 
         (refill_shmem && recs_read == recs_to_read))
     check_for_midnite(get_netcdf_fileposn()-1);
   if (!(total_seconds%update_rate)) {
    (*elapsed_time)++;
   }
   total_seconds++;
  } 
 }
/* 
if refill requested, continue refilling shared memory until end 
time (or possible EOF) is reached
*/
 refill_shmem=(refill_shmem && recs_read < recs_to_read);
 if (recs_read && !refill_shmem) {

/* save number of records just read in for frame jumping assistance at SOF
   and EOF
*/
  last_recs_read=*elapsed_time;
  (void)sprintf(&buff[0],"%d",SHMEM_REFILLED);
   if (GetProtocolDebug())
   (void)fprintf(stderr,
    "WINPUT PROTOCOL: notify DISP_REQ_REPLY that shmem is refilled\n");
  if (write_to_socket(DISP_REQ_REPLY,&buff[0],strlen(&buff[0]))==EBADWRITE)
   perror("frame jump response write_to_socket");
 }
} while (refill_shmem);
recs_read=0;

}

/************************   GET_DATA()  ************************************/
/* acquire or simulate acquisition of raw data
 */
bool get_data() 
{
  int		i;
  static int	syncSeqNum = 0,
		avapsSeqNum[MAX_AVAPS];

  switch (GetInputMode())
    {
    case ADS_DATA:
      for (i = 0; i < MAX_AVAPS; ++i)
        if (eth_shm->avaps[i].rseq - avapsSeqNum[i])
          {
          avapsSeqNum[i] = eth_shm->avaps[i].rseq;
/*        printf("WINPUT: %d: %s", i, eth_shm->avapsBuf[i]); */
          }

      if (eth_shm->sync.rseq - syncSeqNum)
        { 
        syncSeqNum = eth_shm->sync.rseq;
        CheckForRealTimeGap("WINPUT", (Hdr_blk *)ADSrecord);

        return(TRUE);
        }
      else
        return(FALSE);

    case NETCDF:
      break;

    case DISK_DATA:
    case RAWFILE_NETCDF:
      if (get_disk_sdirec() == EOF)
        {
        refill_shmem = FALSE;
        return(FALSE);
        }

      return(TRUE);
    }

  return(FALSE);
}

/*************  FormatTimeSegmentsForOutputFile() ***************************/
void FormatTimeSegmentsForOutputFile(char *buff)
/*
stub to get $NOBJ/netcdf.o linked ok for the ctlmode/netcdf file stuff
*/
{

}

/*****************************  Cleanup()  **********************************/
void Cleanup(int s)
/* catch SIGUSR2 
*/
{
 cleanup();
}

/*****************************  CATCHINT()  **********************************/
void catchint(int s)
/* catch SIGINT 
*/
{
}

/******************************  CLEANUP()  ******************************/
void cleanup()
{
/* release resources and exit
*/
#ifdef QC
 if (QCenabled)
  qc_rt_cleanup();
#endif

 if (infd)
  (void)close(infd);
 close_socket(DISP_REQ);
 close_socket(GENERAL_LISTENING);
/*** remove this if writing of netcdf if handled by nimbus; else, uncomment
when needed....
 if (GetInputMode()==CTL_NETCDF || GetInputMode()==RAWFILE_NETCDF)
  CloseNetCDF();
***/
 CloseWindsBroadcast();
 (void)fprintf(stderr,"WINPUT: exiting \n");
 exit(0);
}


/* -------------------------------------------------------------------- */
void TransferDataToSHMEM(NR_TYPE record[], int index)
{
  register int	i, offset;
  int		ptr3;

  extern ushort	*bits;
  extern NR_TYPE	*volts;

  static bool	firstTime = TRUE;
  static int	timeIndex[3];

  if (firstTime)
    {
    firstTime = FALSE;

    timeIndex[0] = raw[SearchTable((char **)raw, nraw, "HOUR")]->LRstart;
    timeIndex[1] = raw[SearchTable((char **)raw, nraw, "MINUTE")]->LRstart;
    timeIndex[2] = raw[SearchTable((char **)raw, nraw, "SECOND")]->LRstart;
    }

  ptr3		= 3 * index;
  hhmmss[ptr3]    = (volatile int)record[timeIndex[0]];
  hhmmss[ptr3+1]  = (volatile int)record[timeIndex[1]];
  hhmmss[ptr3+2]  = (volatile int)record[timeIndex[2]];

  offset  = nsdi * index;
  memcpy((char *)&bitdata[offset], (char *)bits, sizeof(short) * nsdi);
  memcpy((char *)&voltsdata[offset], (char *)volts, NR_SIZE * nsdi);
  memcpy((char *)&rawdata[offset], (char *)record, NR_SIZE * nsdi);

/*  for (i = 0; i < nsdi; ++i, ++offset)
    {
    bitdata[offset]		= bits[sdi[i]->LRstart];
    voltsdata[offset]	= volts[sdi[i]->LRstart];
    rawdata[offset]		= record[sdi[i]->LRstart];
    }
*/
  offset  = (nraw+nderive) * index;

  for (i = 0; i < nraw; ++i, ++offset)
    data[offset] = record[raw[i]->LRstart];

  for (i = 0; i < nderive; ++i, ++offset)
    data[offset] = record[derived[i]->LRstart];

}	/* END GETNIMBUSDATA */

/* -------------------------------------------------------------------- */

void HandleError(char s[])
{
  (void)fprintf(stderr, s);
}

void LogMessage(char s[])
{
  (void)fprintf(stderr, s);
}

char *ExtractHeaderIntoFile(char *fileName)
{
  int	nBytes;
  char	*tmpFile;
  FILE	*fp;
  Fl	fi;
  char	phys_rec[20000];

  tmpFile = tempnam("/tmp/", "hdr.");

  if ((fp = fopen(tmpFile, "wb")) == NULL)
    {
    fprintf(stderr, "Unable to open tmpFile %s, fatal.\n", tmpFile);
    exit(1);
    }

  if ((infd = open(fileName, O_RDONLY)) < 0)
    {
    fprintf(stderr, "Failure opening input file %s.\n", fileName);
    exit(1);
    }
 
    read(infd, phys_rec, 200);
 
  if (strncmp(FIRST_REC_STRING, (char *)phys_rec, strlen(FIRST_REC_STRING)) != 0)
    {
    memcpy((char *)&fi, phys_rec, sizeof(Fl));
    lseek(infd, 0L, SEEK_SET);
    }
  else
    {
    memcpy((char *)&fi, &phys_rec[20], sizeof(Fl));
    lseek(infd, 20L, SEEK_SET);
    }
 
  nBytes = ntohl(fi.thdrlen);

  read(infd, phys_rec, nBytes);
  fwrite(phys_rec, nBytes, 1, fp);

  close(infd);
  fclose(fp);
  return(tmpFile);

}

/* Shutup Linker */
void AddVariableToRAWlagList(RAWTBL *varp) {}
void AddVariableToSDIlagList(SDITBL *varp) {}

/* END WINPUT.C */

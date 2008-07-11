/* netcdf.c -- read netcdf format files 
*/

#include <stdio.h>
#include <time.h>
#include <netcdf.h>
#include <constants.h>
#include <proto.h>

struct _DatasetInfo {
        int num_dims ;
        int num_vars ;
        int num_attrs ;
        int record_dim_id;
};
struct _VarInfo {
        char mnem[MAX_NC_NAME] ;
        nc_type type ;
        int ndims ;
        int dims[MAX_NC_DIMS] ;
        int num_attrs ;
        int id ;
};
/***
struct _AttributeInfo {
        char mnem[MAX_NC_NAME] ;
        nc_type type ;
        long len ;
};
***/
/* 
major assumptions being made here: 

all records are contiguous seconds, i.e. no time gaps;

there are two variables, long base_time and float time_offset, in this 
dataset, representing, respectively, # of seconds 
since Jan. 1, 1970, and the offset index from this start time, in seconds,
defining the time for each record in dataset.

time is the only dimension, and all record variables use it as their index;
i.e., an ncdump of the netCDF file would start off looking like this:

dimensions:
	time = UNLIMITED ; // (15872 currently)

variables:
	long base_time ;
	float time_offset(time) ;
	float totalsec(time) ;
		totalsec:missing_value = -32768.f ;
	float z(time) ;
		z:missing_value = -32768.f ;

	....(etc.)

data:

 base_time = 700079665 ;

 time_offset = 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 , 
    14 , 15 , 16 , 17 , 18 , 19 , 20 , 21 , 22 , 23 , 24 , 25 , 26 , 27 , 
    28 , 29 , 30 , 31 , 32 , 33 , 34 , 35 , 36 , 37 , 38 , 39 , 40 , 41 , 
	...(etc.)

WINDS does not use any particular strategy for dealing with missing value
indicators, but simply plots that value as if it were the next data point. 
As such, the use of "missing_value" attribute is optional for WINDS' needs,
and serves only as a flag during perusal of dataset.

*/
#define BASETIME "base_time"
#define TIME_OFFSET "time_offset"
static long NetcdfFile;		/* handle for netcdf file access 	*/
static time_t BaseTime;		/* BASETIME for dataset         	*/
static long BaseTimeID;		/* variable id for BASETIME 		*/
static long TimeOffsetID;	/* variable id for TIME_OFFSET 		*/
static nc_type TimeOffsetType;  /* hopefully NC_FLOAT or NC_DOUBLE	*/
static long FileIndex;		/* current position in netcdf file	*/
/* 
general information about dataset	
*/
static struct _DatasetInfo DatasetInfo[1];
/* 
general information about variables	
*/
static struct _VarInfo *VarInfo;
/***
general information about attributes	
static struct _AttributeInfo AttributeInfo[1];		
***/
/*
First, last value for TIME_OFFSET. Might be double or float in netcdf file.
*/
static double DblFirstTimeOffset;
static double DblLastTimeOffset;
static float FloatFirstTimeOffset;
static float FloatLastTimeOffset;
/*
size of dataset
*/
static long DatasetSize;
/*
array of pointers to data variables 
*/
static unsigned int *datap=NULL;

#include <sys/time.h>

/*************************  NETCDF ROUTINES ***********************************/

/************************  ERROR_RESPONSE()  **********************************/

error_response(cdfcall,severity)
char *cdfcall;
long severity;
{
int exitstatus;
char message[1024];

 if (severity==NC_FATAL) {
  (void)sprintf(message,"FATAL: Input process has exited due to FATAL error in netCDF request '%s'. This WINDS process will now also exit...contact a WINDS expert about this problem.",cdfcall);
  exitstatus=1;
 } else if (severity==NC_VERBOSE) {
  (void)sprintf(message,"WARNING: Input process has experienced an error in netCDF request '%s'. If this warning re-appears repeatedly, you should exit this WINDS session and contact a WINDS expert about the problem. If this message re-appears so often that you have trouble getting to the Quit button, you should kill this WINDS session from some other login.",cdfcall);
  exitstatus=0;
 }
 NotifyWINDSProcess(message);
 if (exitstatus)
  exit(0);
}

/*****************************************************************************/

OpenNetCDFFile(path,notifyuser)
char *path;
int notifyuser;
/*
open a NetCDF file pointed to by path and perform initial setup tasks
*/
{
/*
set error handling to NC_VERBOSE
*/
 ncopts = NC_VERBOSE;
 if((NetcdfFile = ncopen(path, NC_NOWRITE)) == -1) {
  if (notifyuser)
   error_response("ncopen",NC_VERBOSE);
  return -1;
 }
 return 0;
}

/************************  DATASET_INIT()  **********************************/

dataset_init()
{
long valindex[1];
/*
do basic inquiry about dataset
*/
 if (ncinquire(NetcdfFile, &(DatasetInfo->num_dims), (int *)&(DatasetInfo->num_vars),
  &(DatasetInfo->num_attrs), (int *)&(DatasetInfo->record_dim_id))==-1)
  error_response("ncinquire in dataset_init()",NC_VERBOSE);
/*
allocate array of pointers to destination variables for ncrecget() call
*/
 datap=(unsigned int *)malloc((unsigned int)DatasetInfo->num_vars*sizeof(int));
/*
allocate variable information structures based on size just now determined
*/
 VarInfo=(struct _VarInfo *)
  malloc(DatasetInfo->num_vars * sizeof(struct _VarInfo));
/*
set the base time variable value after coercing system to think in GMT
*/
 set_BASETIME();
/*
set id for TIME_OFFSET variable
*/
 if ((TimeOffsetID=ncvarid(NetcdfFile,TIME_OFFSET))==-1)
  error_response("ncvarid of TimeOffsetID",NC_VERBOSE);
/*
TIME_OFFSET is sometimes float, sometimes double
*/
 if (ncvarinq(NetcdfFile, TimeOffsetID, NULL, &TimeOffsetType,
  NULL, NULL, NULL)==-1)
  error_response("ncvarinq of TimeOffsetID type",NC_VERBOSE);
/*
get first time offset to facilitate finding start of file later on
*/ 
 valindex[0]=0;
 if (TimeOffsetType==NC_FLOAT) {
  if (ncvarget1(NetcdfFile, TimeOffsetID, (const long *) valindex,(float *)&FloatFirstTimeOffset)==-1)
    error_response("ncvarget1 of float TimeOffsetID",NC_VERBOSE);
 }  else {
  if (ncvarget1(NetcdfFile, TimeOffsetID, (const long *) valindex,(double *)&DblFirstTimeOffset)==-1)
    error_response("ncvarget1 of double TimeOffsetID",NC_VERBOSE);
 }
 (void)get_netcdf_dataset_size();
}

/*****************************************************************************/

GetNetCDFVarValues(varname,start,numvalues,values)
char *varname;
long start,numvalues;
void *values;
/*
fill values array with numvalues values, starting at start in the currently
netcdf file, for variable varname
*/
{
 int varid;
 long starttime[1],count[1];

 if ((varid=ncvarid(NetcdfFile,varname))==-1) {
  error_response("WARNING: problem getting varname id",NC_VERBOSE);
  return -1;
 }
 starttime[0]=start;
 count[0]=numvalues;
 if (ncvarget(NetcdfFile, varid, starttime, count, values)==-1) {   
  error_response("WARNING: problem getting variable data request",NC_VERBOSE);
  return -1;
 } 
 return 1;
}

/*******************  GET_NETCDF_DATASET_SIZE()  *****************************/

get_netcdf_dataset_size()
{
/*
get end time of file to smooth out positioning of file later on
*/ 
 if (ncdiminq(NetcdfFile, 0, (char *)0, &DatasetSize)==-1)
  error_response("ncdiminq in get_netcdf_dataset_size()",NC_VERBOSE);
 GetLastTimeOffset();
 return DatasetSize;
}

/*********************  GetLastTimeOffset()  *******************************/

GetLastTimeOffset()
{
long valindex[1];
 valindex[0]=DatasetSize-1;
 if (TimeOffsetType==NC_FLOAT) {
  if (ncvarget1(NetcdfFile, TimeOffsetID, (const long *) valindex,(float *)&FloatLastTimeOffset)==-1)
   error_response("ncvarget1 of LAST float TimeOffsetID",NC_VERBOSE);
 } else {
  if (ncvarget1(NetcdfFile,TimeOffsetID,(const long *) valindex,(double *)&DblLastTimeOffset)==-1)
   error_response("ncvarget1 of LAST double TimeOffsetID",NC_VERBOSE);
 }
}

/*********************  GET_NUM_NETCDF_VARS()  *******************************/

get_num_netcdf_vars()
/*
return the # of variables in dataset.  Deduct one because BASETIME will not be
included in the name list sent out or in looping for filling the data arrays.
*/
{
 return DatasetInfo->num_vars-1;
}

/************************  SET_BASETIME()  *******************************/

set_BASETIME()
/*
determine value of dataset base time, assumed in seconds from Jan. 1, 1970.
*/
{
 if ((BaseTimeID=ncvarid(NetcdfFile,BASETIME))==-1)
  error_response("ncvarid of BaseTimeID",NC_VERBOSE);
 else {
  if (ncvarget1(NetcdfFile, BaseTimeID, 0, (void *)&BaseTime)==-1)
   error_response("ncvarget1 of BaseTimeID",NC_VERBOSE);
 }
}

/**********************  GET_NETCDF_END_TIME()  ****************************/

get_netcdf_end_time()
/*
must uncomment init of LastTimeOffset in get_netcdf_dataset_size() before
using this
*/
{
 if (TimeOffsetType==NC_DOUBLE) 
  return BaseTime+(int)DblLastTimeOffset;
 else
  return BaseTime+(int)FloatLastTimeOffset;
}

/**********************  GET_NETCDF_START_TIME()  ****************************/

get_netcdf_start_time()
{
 if (TimeOffsetType==NC_DOUBLE) 
  return BaseTime+(int)DblFirstTimeOffset;
 else
  return BaseTime+(int)FloatFirstTimeOffset;
}

/*********************  GET_NETCDF_FILEPOSN()  *******************************/

get_netcdf_fileposn()
{
 return FileIndex;
}

/***************************  GET_NETCDF_DATE()  *******************************/

char *
get_netcdf_date()
{
time_t basetime;
struct tm *time_info;  
static char date[8];

 basetime=(time_t)BaseTime;
 time_info=(struct tm *)gmtime(&basetime);
 (void)sprintf(date,"%02d/%02d/%02d",
  time_info->tm_mon+1,time_info->tm_mday,time_info->tm_year);
 return date;
}

/*********************  GET_NETCDF_VARNAMES()  *******************************/

char *
get_netcdf_varnames(namelist,namelen)
char *namelist;
long namelen;
/*
fill in character pointer namelist with names in dataset, truncating as needed
to the desired length namelen.  Add one to number of vars since one is deducted
when value is determined to account for excluding BASETIME.  Looping from zero
will most likely include its id in the loop, so you must provide for running
into BASETIME.
*/
{
long cdfvar,client_var,vectorlength;

 for (cdfvar=client_var=0; cdfvar<get_num_netcdf_vars()+1; cdfvar++) {
  if (ncvarinq(NetcdfFile, cdfvar,
   VarInfo[cdfvar].mnem,
   &(VarInfo[cdfvar].type),
   (int*)&(VarInfo[cdfvar].ndims),
   VarInfo[cdfvar].dims,
   &(VarInfo[cdfvar].num_attrs))==-1)
    error_response("ncvarinq in get_netcdf_varnames",NC_VERBOSE);
  if ((VarInfo[cdfvar].id=ncvarid(NetcdfFile,VarInfo[cdfvar].mnem))==-1) {
   error_response("ncvarid in get_netcdf_varnames",NC_VERBOSE);
  }
  if (strcmp(VarInfo[cdfvar].mnem,BASETIME)) {
/*
do NOT include BASETIME variable in display varnames list
*/
   (void)sprintf(&namelist[client_var*namelen],"%s",VarInfo[cdfvar].mnem);
   namelist[(client_var+1)*namelen-1]='\0';
   if (VarInfo[cdfvar].ndims==3) {
    if(ncdiminq(NetcdfFile,VarInfo[cdfvar].dims[2],NULL,&vectorlength)==-1){
//    if(ncattget(NetcdfFile,VarInfo[cdfvar].id,"VectorLength",&vectorlength)==-1){
     continue;
    } else {
     SetDataPtr(client_var,SetPMS1DnetCDF(VarInfo[cdfvar].mnem,vectorlength));
    }
   } else if (VarInfo[cdfvar].ndims==2) {
    NotifyWINDSProcess(
     "FATAL: Netcdf file is hi-rate, and WINDS Input cannot yet handle these. WINDS Input process will now exit.");
    LogMsgToConsole((char *)GetDisplayHost(),"WINPUT:InputFile",
     "FATAL: Netcdf file is hi-rate, and WINDS Input cannot yet handle these. WINDS Input process will now exit.");
    exit(0);
   }
   client_var++;
  }
 }
}

/**************************  SetDataPtr()  *********************************/

SetDataPtr(index,address)
int index;
unsigned int address;
{
 datap[index]=address;
}

/************************  GET_NETCDF_DATA()  *******************************/

get_netcdf_data(timeptr,dataptr,start,numslices,update_rate)
long *timeptr;
float *dataptr;
long start,numslices;
long update_rate;
/*
assuming time as the one and only dimension for all variables except BASETIME;

get dataset values for timeptr (assumed to be 3-word long pointer representing 
hours, minutes, and seconds) and dataptr (assumed to be a float pointer), 
starting from offset start and proceeding for numslices records.

this version of get_netcdf_data assumes data is stored by time, with each
record consisting of all variables per time record.
*/
{
static long time_slice,numvars,cdfvar,client_var,id;
static long basetime,varindex,dataptr_index,startblock,time_next;
static double DblCurrentTime;
/***static float FloatCurrentTime;***/
static struct tm *time_info;  
int ndim;
long time_index;

 numvars=get_num_netcdf_vars();
 FileIndex=start;
/*
check that not too many records requested read
*/
 if (start+numslices*update_rate>get_netcdf_dataset_size()) {
  numslices=(get_netcdf_dataset_size()-start)/update_rate;
 }
/*
time_slice is n'th record to be stored in data destinations
*/
 for (time_slice=start; time_slice<start+numslices; time_slice++) {
/*
determine which second (relative to start address sent) in data array is to be 
filled.  Caller is responsible for ensuring no bounds violations in fill loop.
*/
  dataptr_index=time_slice-start;
/*
from this, compute the start address of variable record block in data array,
as function of # of names per block.  startblock then is index into data array.
*/
  startblock=dataptr_index*numvars;
/* 
get variable id from variable name.  Add one since BASETIME is not included
in the name list, but its id is somewhere in the id list and must be skipped.
*/
  for (cdfvar=client_var=0; cdfvar<numvars+1; cdfvar++) {
/* 
skip the BASETIME variable - it's global only, not in every time slice
*/
   if ((id=VarInfo[cdfvar].id)!= BaseTimeID) {
/*
so varindex is the index to each individual variable
*/
    varindex=startblock+client_var;
/*
assign addresses to pointer array used later in ncrecget call
*/
    if (ncvarinq(NetcdfFile, id, NULL, NULL, &ndim, NULL, NULL)==-1)
     error_response("ncvarinq on dimensions only",NC_VERBOSE);
    switch (ndim) {
     case 1:
      SetDataPtr(client_var,(unsigned int)&dataptr[varindex]);
      break;
    }
/*
starting from start, acquire numslices data values for all variables
*/
    if (id==TimeOffsetID) {
/*
TIME_OFFSET: either get value now if it's a double or later if float;
remember this index in either event to determine current hhmmss time later  
*/
     time_index=varindex;
     if (TimeOffsetType==NC_DOUBLE) { 
      if (ncvarget1(NetcdfFile, id, (const long *)FileIndex,
       (double *)&DblCurrentTime)==-1)
       error_response("ncvarget1 of DblCurrentTime",NC_VERBOSE);
      dataptr[time_index]=(float)DblCurrentTime;
/*** only need to bother with NC_DOUBLE, type NC_FLOAT is read in below in
a ncreget call...
     } else if ( ncvarget1(NetcdfFile, id, (const long *)FileIndex,
       (float *)&FloatCurrentTime)==-1) {
       error_response("ncvarget1 of FloatCurrentTime",NC_VERBOSE);
      dataptr[time_index]=(float)FloatCurrentTime;
***/
     }
    } else if (TimeOffsetType==NC_DOUBLE) {
/*
can't use ncrecget here because not all record variables are floats. Do
them one at a time to accommodate this brain-damaged scheme. Why would
anyone need a double for time_offset anyway? Some aircraft data processing
is done this way, so provide for it here.
*/
     if (ndim==1)
      if (ncvarget1(NetcdfFile, id, (const long *)FileIndex,(float *)&dataptr[varindex])==-1)
       error_response("ncvarget1 of data with double time_offset",NC_VERBOSE);
    } 
    client_var++;
   }
  }
  if (TimeOffsetType==NC_FLOAT) {
/*
read in the entire FileIndex'th record into destinations indicated
*/
   ncrecget(NetcdfFile,FileIndex,(void *)datap);
  } else {
/*
Whoops
*/
  }
/*
Constant 3 is how many words compose the timeptr array (hh,mm,ss); constant 
offsets 1 and 2 are indices into the mm and ss portions.
*/
  basetime=BaseTime+(int)dataptr[time_index];
  time_info=(struct tm *)gmtime(&basetime);
  time_next=dataptr_index*3;
  timeptr[time_next]=time_info->tm_hour;  
  timeptr[time_next+1]=time_info->tm_min;  
  timeptr[time_next+2]=time_info->tm_sec;  
  free(time_info);
/*
update file position by update rate seconds
*/
  FileIndex+=update_rate;
 }
 return numslices;
}

get_BASETIME() 
/*
return the base time of dataset, assumed in seconds from Jan. 1, 1970
*/
{
 return BaseTime;
}


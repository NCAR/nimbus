/*   shmem.c -- operations involving shared memory


SCCS ID:

*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

/************************  WINDS INCLUDES  **********************************/

#include <shmem.h>
#include <constants.h>
#include <display.h>
#include <lengths.h>
#include <header.h>
#include <proto.h>
#include <pms1d.h>
#include <files.h>
#include <eth_shm.h>

/****************************  EXPORTABLE  ************************************/
/* Shared memory pointers.
 */

volatile int *elapsed_time;	/* total time elapsed in terms of one-second
				 * data records	*/
volatile int *hhmmss;		/* hh:mm:ss	*/
volatile struct SUN_ETH_SHM *eth_shm;	/* socket communications from ads */
volatile float *data;		/* data values for derived variables	*/
volatile float *rawdata;	/* data values for raw variables	*/
volatile unsigned short int *bitdata; /* data values for bits mode variables */
volatile float *voltsdata;	/* data values for volts mode variables	*/

int *input_pid;	/* process id for input process		*/
int *windComm_pid;	/* process id for windComm process	*/

int *len_raw;		/* list of raw variable names		*/
char *raw_list;		/* how many raw variables names		*/
int *len_derive;	/* how many derived variables names	*/
char *derive_list;	/* list of derived variable names	*/

int *nPMS1Dprobes;	/* number of PMS1D probes on project	*/
struct pms1d_probe *PMS1Dprobe;	/* Pms1d data arrays.	*/


/**************************  GLOBAL WITHIN FILE  ******************************/

/* 
# of seconds stored in shared mem 
*/
static int max_shmem_seconds=DEFAULT_SHMEM_SEX; 
static int DECL_STATIC_ID_VARS;
static int  DECL_DYNAMIC_ID_VARS;
static int input_id,
           windComm_id;
static char shmem_ids_filename[MAX_FLNM_LNGTH];

extern char *ProjectDirectory;

/****************************************************************************/

float
GetDeriveDataValue(index)
int index;
{
 return data[index];
}

/****************************************************************************/

float
GetRawDataValue(index)
int index;
{
 return rawdata[index];
}

/****************************************************************************/

float
GetVoltsDataValue(index)
int index;
{
 return voltsdata[index];
}

/****************************************************************************/

unsigned short int
GetBitDataValue(index)
int index;
{
 return bitdata[index];
}

/****************************************************************************/

GetNumDerivedNames()
{
 return *len_derive;
}

/****************************************************************************/

GetNumRawNames()
{
 return *len_raw;
}

/****************************************************************************/

GetHMS(index)
int index;
{
 return hhmmss[index];
}

/****************************  GetWINDSElapsedTime()  *************************/

GetWINDSElapsedTime()
{
 return *elapsed_time;
}

/**************************  GET_TIME_FRAME_SIZE()  ***************************/

get_time_frame_size()
/*
this is the size in shared memory that is "available" to display side processes
for data history.
*/
{
 return get_shmem_size();
}

/**************************  GET_DATABLOCK_SIZE()  ***************************/

get_datablock_size()
/*
this is the actual size of the datablocks in shared memory storing raw data,
derived data, hours-minutes-seconds, bits data, and volts data.  It is larger
than the time frame size to provide a buffer zone which prevents overwrites
by the input process on the next index in the circular queue when a display
process is wanting to read that index: if the display side is too slow, input
will write over that value (which would have been the first value in a total
history refresh) with the current value, resulting in a wraparound in the 
trace drawing.  Preclude that by providing the QUEUE_MARGIN buffer zone.
*/
{
 return get_time_frame_size()+QUEUE_MARGIN;
}

/**************************  SET_SHMEM_SIZE()  ********************************/

int
set_shmem_size(size)
int size;
{
 max_shmem_seconds=size;
}

/**************************  GET_SHMEM_SIZE()  ********************************/

int
get_shmem_size()
{
 return max_shmem_seconds;
}

/**************************  GETMEM()  **************************************/

getmem(char *client, key_t key, int size)
/* 
getmem(key,size) -- get size bytes of memory assoc'd with key
... returns memory id (possibly 0); if error, returns ERROR
*/
{
 int shmid;
 int original_size;

printf("%s req %d shmem, key=%d\n", client, size, key);

  original_size = size;

  do {
   if ( (shmid=shmget(key,size,IPC_CREAT|RWUGO))==ERROR) {
/* 
assume previous shared memory segment associated with key was of a smaller size,
and that the process owning that segment did not clean up the shared memory
keys and segments cleanly (e.g., it aborted prematurely).  Loop with decreasing 
sizes until find one that is small enough to be OK
*/
    if (size==0)
     break;
/***
     (void)fprintf(stderr,
     "%s: attach failed, need to release; reduce size (key %d) to facilitate\n",
     client,key);
***/
    size/=2;
   } else {
    if (original_size!=size) {
/*
attempt to clean up old key/size combo to allow getting new (presumably larger) 
size segment
*/
     int shmaddr;
     shmaddr=memaddr(shmid,"TestKey");
     (void)ungetmem(shmid,"TestKey");
     (void)memdetach((char *)shmaddr,"TestKey");
     shmid=shmget(key,original_size,IPC_CREAT|RWUGO);
/***
     if ( (shmid=shmget(key,original_size,IPC_CREAT|RWUGO))==ERROR)
/*
size is still not acceptable
*/
/***
      (void)fprintf(stderr,
       "%s: no can do; size still too large? Try again...\n",client);
***/
    }
    return(shmid);
   }
  } while (size >= 0);
/*
size at zero, but still nogo: something besides the size is not acceptable
*/
  (void)fprintf(stderr,
   "%s: size of ZERO is STILL too large! What's the problem here?\n",client);
  perror("getmem(), shmget: ");
sleep(15);
  return(ERROR);
}

/**************************  MEMADDR()  **************************************/

int memaddr(shmid,key)
/* 
memaddr(memid) -- get memory address associated with memid
... returns address if successful, else returns 0
*/
int shmid;
char *key;
{
 int shmaddr;
/* 
note that in Solaris 2.x, default # of segments is only 6; must enter line

set shmsys:shminfo_shmseg=50

in /etc/system to enable enough for WINDS
*/
 if ( (shmaddr = (int)(shmat(shmid,(char *)NULL,SHM_RND))) == ERROR) {
  (void)fprintf(stderr,"WARNING: problem with shmat (id %d, key %s)",
   shmid,key);
  perror("");
  return((int)NULL);
 }
 return(shmaddr);
}

/**************************  MEMDETACH()  ***********************************/

int memdetach(shmaddr,key)
/* 
memdetach(shmaddr) -- detach memory segment starting at shmaddr
... returns 1 if successful, else returns ERROR
*/

char *shmaddr,*key;
{
 if (!shmaddr)
  return 1;
 if ( (shmdt(shmaddr)) == ERROR) {
  (void)fprintf(stderr,"WARNING: shmdt problem with %s key...",key);
  perror("");
  return(ERROR);
 }
 return(1);
}

/**************************  UNGETMEM()  ************************************/

int ungetmem(int memid, char *key)
/* 
ungetmem(memid) -- release memory segment assoc'd 
with memid ... returns 1 if successful, else returns ERROR
*/
{
/*  release system memory id associated with memid  */
 if (!memid)
  return 1;
 if ( (shmctl(memid,IPC_RMID,(struct shmid_ds *)0)) == ERROR) {
  (void)fprintf(stderr,"WARNING: shmctl problem with %s key...",key);
  perror("");
  return(ERROR);
 }
 return(1);
}

/*************************  InitTimeStamps()  ********************************/

InitTimeStamps()
{
int second;

  for (second=0; second < get_datablock_size()*3; second++)
   hhmmss[second]=0;
}

/*************************  GetProcessPids()  ********************************/

GetProcessPids()
{
 if ((input_pid=(int *)memaddr(input_id,"input_pid"))==NULL ) {
  return(FALSE);
 }
 if ((windComm_pid=(int *)memaddr(windComm_id,"windComm_id"))==NULL ) {
  return(FALSE);
 }
 return TRUE; 
}

/************************ SET_MAX_SHMEM_SIZE()  ******************************/

set_max_shmem_size(client)
char *client;
/*
determine largest segment that will be requested, then test that it is within 
kernel limits.  If not, loop decreasing # of seconds to store in shared
memory until largest segment is small enough for kernel limits.
*/
{
  int	maxseg, reduced, id;

  reduced = 0;

  do {
    errno = 0;
    maxseg = MAX(
    /***RAWKEY***/	sizeof(float)*(*len_raw+1)*get_datablock_size(),
    /***DERIVEKEY***/	sizeof(float)*(*len_derive+1)*get_datablock_size() );

   if ((id=getmem(client, (key_t)8/*any id# will work here.*/,maxseg))==ERROR) {
/*
save original request
*/
     if (!reduced)
       reduced = get_shmem_size();
fprintf(stderr, "set_max_shmem_size: exiting.\n");
exit(1);
     set_shmem_size(get_datablock_size()-60);
   } else {
     errno=0;
   }
  } while (errno);
  (void)fprintf(stderr,"%s: Stored memory seconds: %d\n", client,get_shmem_size());
  if (reduced)
    (void)fprintf(stderr,
     "%s WARNING: This is REDUCED from original request of %d\n",client,reduced);
  (void)ungetmem(id,"TestKey");
 
}

/********************* WAIT_ON_ALLOCN_SIZING  ********************************/

wait_on_allocn_sizing(client)
char *client;
/* 
wait for the input process to give *len_derive a non-zero value
*/
{
int waited,first;

 waited=0;
 first=TRUE;
 while (*len_derive==0) {
  if (first) {
   (void)fprintf(stderr,"%s: waiting on input to init *len_derive ...\n",
   client);
   first=FALSE;
  }
  (void)sleep(1);
  if (++waited==600) {
   (void)fprintf(stderr,"%s WARNING: waited 600 seconds for initial'd len_derive...giving up...\n",client);
   return; 
  }
 }
}

/********************* WRITE_STATIC_IDS  ************************************/

write_static_ids(host)
char *host;
{
FILE *fd, *WINDSfopen();
 
 (void)sprintf(shmem_ids_filename,STATIC_IDS,ProjectDirectory,host);
 (void)unlink(shmem_ids_filename);
 if ( (fd=WINDSfopen(shmem_ids_filename,"w"))==NULL) {
  (void)WINDSfclose(fd);
  (void)fprintf(stderr,"WARNING: problem opening writable %s\n",shmem_ids_filename);
  return ERROR;
 }
 (void)fprintf(fd,STATIC_ID_ARGLIST,STATIC_ID_VARS);
 (void)fflush(fd);
 (void)WINDSfclose(fd);

/* as well, remove the file of dynamic ids before starting the processes
   that read it, so that they wait for the new one to be written.  Since 
   the input task must be started before getting the sizes for the dynamic
   ids, do this here, assuming this is called before start_winds_tasks().
*/
 (void)sprintf(shmem_ids_filename,DYNAMIC_IDS,ProjectDirectory,host);
 (void)unlink(shmem_ids_filename);
 return OK;
}

/********************* READ_STATIC_IDS  ************************************/

read_static_ids(host)
char *host;
{
FILE *fd, *WINDSfopen();
 
 (void)sprintf(shmem_ids_filename,STATIC_IDS,ProjectDirectory,host);
 if ( (fd=WINDSfopen(shmem_ids_filename,"r"))==NULL) {
  (void)fprintf(stderr,"error opening file shmem_ids_filename\n");
  (void)WINDSfclose(fd);
  return(FALSE);
 }
 (void)fscanf(fd,STATIC_ID_ARGLIST,READ_STATIC_ID_VARS);
 (void)WINDSfclose(fd);
 return(TRUE);
}

/********************* WRITE_DYNAMIC_IDS  ************************************/

write_dynamic_ids(client)
char *client;
{
FILE *fd, *WINDSfopen();
/* 
assumed file created already by write_static_ids()
*/
 if ( (fd=WINDSfopen(shmem_ids_filename,"w"))==NULL) {
  (void)fprintf(stderr,"%s FATAL: can't open writable %s in write_dynamic_ids\n",
   client,shmem_ids_filename);
  (void)WINDSfclose(fd);
  return(ERROR);
 }
 (void)fprintf(fd,DYNAMIC_ID_ARGLIST,DYNAMIC_ID_VARS);
 (void)fflush(fd);
 (void)WINDSfclose(fd);
 (void)fprintf(stderr,"%s: dynamic ids file is written\n",client);
 return(OK);
}

/********************* READ_DYNAMIC_IDS  ************************************/

read_dynamic_ids(client,host)
char *client,*host;
{
FILE *fd, *WINDSfopen();
int first;
 
 (void)sprintf(shmem_ids_filename,DYNAMIC_IDS,ProjectDirectory,host);
 LAST_DYNAMIC_ID=0;
 first=TRUE;
 do {
/* 
in case of race condition between this process and process that writes the
file, loop until the last variable read is non-zero to ensure that all the
values were written
*/
  if ( (fd=WINDSfopen(shmem_ids_filename,"r"))!=NULL) {
   (void)fprintf(stderr,
    "%s: dynamic file created, now checking to see if it's complete...\n",
    client);
   (void)fscanf(fd,DYNAMIC_ID_ARGLIST,READ_DYNAMIC_ID_VARS);
  } else {
   if (first) {
    (void)fprintf(stderr,
     "%s: waiting for dynamic ids file creation...\n",client);
    first=FALSE;
   }
  }
  (void)sleep (1);
  (void)WINDSfclose(fd);
 } while (LAST_DYNAMIC_ID==0);
 (void)fprintf(stderr,
  "%s: dynamic ids file is completed and read in...\n",client);
}

/**************************  GET_IDS_STATIC()  ******************************/

get_ids_static(client)
char *client;
/* allocate if necessary and get keys for statically sized shared 
   memory segments
*/
{

 if ( (input_id=getmem(client,(key_t)INPUT_PIDKEY,sizeof(int)) )==ERROR) {
  (void)fprintf(stderr,"FATAL error on getmem INPUT_PIDKEY\n");
  return(ERROR);
 }
 if ( (windComm_id=getmem(client,(key_t)WINDCOMM_PIDKEY,sizeof(int)) )==ERROR) {
  (void)fprintf(stderr,"FATAL error on getmem INPUT_PIDKEY\n");
  return(ERROR);
 }
 if ( (eth_shm_id=getmem(client,(key_t)ETHKEY,sizeof(struct SUN_ETH_SHM)) )==ERROR) {
  (void)fprintf(stderr,"FATAL error on getmem ETHKEY\n");
  return(ERROR);
 }
 if ( (etid=getmem(client,(key_t)ETKEY,sizeof(int))  )==ERROR) {
  (void)fprintf(stderr,"FATAL error on getmem ETKEY\n");
  return(ERROR);
 }
 if ( (lenraw_id=getmem(client,(key_t)LENRAW_KEY,sizeof(int)))==ERROR) {
  (void)fprintf(stderr,"FATAL error on getmem LENRAW_KEY\n");
  return(ERROR);
 }
 if ( (lender_id=getmem(client,(key_t)LENDER_KEY,sizeof(int)))==ERROR) {
  (void)fprintf(stderr,"FATAL error on getmem LENDER_KEY\n");
  return(ERROR);
 }
 if ( (nPMS1Dprobes_id=getmem(client,(key_t)NPMS1DPROBES_KEY,sizeof(int))  )==ERROR) {
  (void)fprintf(stderr,"FATAL error on getmem NPMS1DPROBES_KEY\n");
  return(ERROR);
 }
 if ( (PMS1Dprobe_id=getmem(client,(key_t)PMS1DPROBE_KEY,
  sizeof(struct pms1d_probe)*MAX1DPROBES)  )==ERROR) {
  (void)fprintf(stderr,"FATAL error on getmem PMS1DPROBE_KEY\n");
  return(ERROR);
 }
 return(0);
}


/**********************   GETADDR_STATIC()  *********************************/

getaddr_static()
/* given the keys initialized in get_ids_static, attach to their shared memory
   segment addresses.
*/
{

 if ((eth_shm=(struct SUN_ETH_SHM *)memaddr(eth_shm_id,"eth_shm_id"))==NULL ) {
  return(ERROR);
 }
 if ((elapsed_time=(int *)memaddr(etid,"elapsed_time"))==NULL ) {
  return(ERROR);
 }
 if ((len_raw=(int *)memaddr(lenraw_id,"lenraw_id"))==NULL ) {
  return(ERROR);
 }
 if ((len_derive=(int *)memaddr(lender_id,"lender_id"))==NULL ) {
  return(ERROR);
 }
 if ((nPMS1Dprobes=(int *)memaddr(nPMS1Dprobes_id,"nPMS1Dprobes_id"))==NULL ) {
  return(ERROR);
 }
 if ((PMS1Dprobe=(struct pms1d_probe *)memaddr(PMS1Dprobe_id,"PMS1Dprobe_id"))==NULL ) {
  return(ERROR);
 }
 return(0);
}

/************************   GET_IDS_DYNAMIC()  *****************************/

get_ids_dynamic(client)
char *client;
/* allocate if necessary and get keys for dynamically sized shared 
   memory segments.  This can ONLY be called AFTER *len_raw and *len_derive
   have been initialized by the input process.
*/
{  

/* IMPORTANT NOTE: the raw and derived segments are declared as one larger
   than their corresponding character arrays.  This is because the function
   search() will return the length of the array when a string is not found,
   implying the variable is not present.  This returned value is the index in
   the raw/derived array where the computed (by calib()) value is stored; of
   course, this value is meaningless, and thus is stored in a meaningless
   location -- one higher than will ever be requested via the character array
   in question (assuming the "+ 1" remains intact in values() references in
   the calib() routine)
*/

 if ( (rawid=getmem(client,(key_t)RAWKEY,sizeof(float)*(*len_raw+1)*get_datablock_size())   )==ERROR) {
  (void)fprintf(stderr,"FATAL error on getmem RAWKEY\n");
  return(ERROR);
 }
 if ( (derivid=getmem(client,(key_t)DERIVEKEY,sizeof(float)*(*len_derive+1)*get_datablock_size()))==ERROR) {
  (void)fprintf(stderr,"FATAL error on getmem DERIVEKEY\n");
  return(ERROR);
 }
 if ( (time_id=getmem(client,(key_t)TIME_KEY,sizeof(int)*3*get_datablock_size())  )==ERROR) {
  (void)fprintf(stderr,"FATAL error on getmem TIME_KEY\n");
  return(ERROR);
 }
/* if *len_raw or *len_derive is zero (as they may be in analysis modes), 
   use MAX() to ensure getmem() gets a non-zero value.
*/
 if ( (inname_id=getmem(client,(key_t)INNAME_KEY,NAMLEN*MAX(1,*len_raw)))==ERROR) {
  (void)fprintf(stderr,"FATAL error on getmem INNAME_KEY\n");
  return(ERROR);
 }
 if ( (block_name_id=getmem(client,(key_t)BLOCK_NAME_KEY,MAX(1,*len_derive)*NAMLEN))==ERROR) {
  (void)fprintf(stderr,"FATAL error on getmem BLOCK_NAME_KEY\n");
  return(ERROR);
 }
 if ( (bits_id=getmem(client,(key_t)BITSKEY,sizeof(short int)*(*len_raw+1)*get_datablock_size()) )==ERROR) {
  (void)fprintf(stderr,"FATAL error on getmem BITSKEY\n");
  return(ERROR);
 }
 if ( (volts_id=getmem(client,(key_t)VOLTSKEY,sizeof(float)*(*len_raw+1)*get_datablock_size()) )==ERROR) {
  (void)fprintf(stderr,"FATAL error on getmem VOLTSKEY\n");
  return(ERROR);
 }
 return OK;
}

/************************   GETADDR_DYNAMIC()  *****************************/

getaddr_dynamic()
{
 if ((rawdata=(float *)memaddr(rawid,"rawid")) ==NULL) {
  return(ERROR);
 }
 if ((data=(float *)memaddr(derivid,"derivid"))==NULL ) {
  return(ERROR);
 }
 if ((raw_list=(char *)memaddr(inname_id,"inname_id"))==NULL ) {
  return(ERROR);
 }
 if ((hhmmss=(int *)memaddr(time_id,"time_id"))==NULL ) {
  return(ERROR);
 }
 if ((derive_list=(char *)memaddr(block_name_id,"block_name_id"))==NULL ) {
  return(ERROR);
 }
 if ((bitdata=(unsigned short int*)memaddr(bits_id,"bits_id"))==NULL ) {
  return(ERROR);
 }
 if ((voltsdata=(float *)memaddr(volts_id,"volts_id"))==NULL ) {
  return(ERROR);
 }
 return OK;
}

/**************************  DETACH_SHMEM()  ********************************/

detach_shmem()
/* detach from all shared memory id keys and release the memory segments.
*/
{
 int UngetmemError=FALSE,MemdetachError=FALSE;
/* 
release the id keys
*/
 if (ungetmem(eth_shm_id,"ETHKEY")==ERROR) {
  UngetmemError=TRUE;
 }
 if (ungetmem(etid,"ETKEY")==ERROR) {
  UngetmemError=TRUE;
 }
 if (ungetmem(lenraw_id,"LENRAW_KEY")==ERROR) {
  UngetmemError=TRUE;
 }
 if (ungetmem(lender_id,"LENDER_KEY")==ERROR) {
  UngetmemError=TRUE;
 }
 if (ungetmem(nPMS1Dprobes_id,"NPMS1DPROBES_KEY")==ERROR) {
  UngetmemError=TRUE;
 }
 if (ungetmem(PMS1Dprobe_id,"PMS1DPROBE_KEY")==ERROR) {
  UngetmemError=TRUE;
 }
 if (ungetmem(time_id,"TIME_KEY")==ERROR) {
  UngetmemError=TRUE;
 }
 if (ungetmem(rawid,"RAWKEY")==ERROR) {
  UngetmemError=TRUE;
 }
 if (ungetmem(derivid,"DERIVEKEY")==ERROR) {
  UngetmemError=TRUE;
 }
 if (ungetmem(inname_id,"INNAME_KEY")==ERROR) {
  UngetmemError=TRUE;
 }
 if (ungetmem(block_name_id,"BLOCK_NAME_KEY")==ERROR) {
  UngetmemError=TRUE;
 }
 if (ungetmem(bits_id,"BITSKEY")==ERROR) {
  UngetmemError=TRUE;
 }
 if (ungetmem(volts_id,"VOLTSKEY")==ERROR) {
  UngetmemError=TRUE;
 }

/* release the memory segments
*/
 if (memdetach((char *)eth_shm,"ETHKEY")==ERROR) {
  MemdetachError=TRUE;
 }
 if (memdetach((char *)elapsed_time,"ETKEY")==ERROR) {
  MemdetachError=TRUE;
 }
 if (memdetach((char *)len_raw,"LENRAW_KEY")==ERROR) {
  MemdetachError=TRUE;
 }
 if (memdetach((char *)len_derive,"LENDER_KEY")==ERROR) {
  MemdetachError=TRUE;
 }
 if (memdetach((char *)nPMS1Dprobes,"NPMS1DPROBES_KEY")==ERROR) {
  MemdetachError=TRUE;
 }
 if (memdetach((char *)PMS1Dprobe,"PMS1DPROBE_KEY")==ERROR) {
  MemdetachError=TRUE;
 }
 if (memdetach((char *)hhmmss,"TIME_KEY")==ERROR) {
  MemdetachError=TRUE;
 }
 if (memdetach((char *)rawdata,"RAWKEY")==ERROR) {
  MemdetachError=TRUE;
 }
 if (memdetach((char *)data,"DERIVEKEY")==ERROR) {
  MemdetachError=TRUE;
 }
 if (memdetach((char *)raw_list,"INNAME_KEY")==ERROR) {
  MemdetachError=TRUE;
 }
 if (memdetach((char *)derive_list,"BLOCK_NAME_KEY")==ERROR) {
  MemdetachError=TRUE;
 }
 if (memdetach((char *)bitdata,"BITSKEY")==ERROR) {
  MemdetachError=TRUE;
 }
 if (memdetach((char *)voltsdata,"VOLTSKEY")==ERROR) {
  MemdetachError=TRUE;
 }
 if (UngetmemError)
  (void)fprintf(stderr,
   "WARNING: some problem(s) with ungetmem() in detach_shmem()\n");
 if (MemdetachError)
  (void)fprintf(stderr,
   "WARNING: some problem(s) with memdetach() in detach_shmem()\n");
}

/***************************************************************************/

SetRawValue(which,value)
int which;
float value;
{
 rawdata[which]=value;
}

/***************************************************************************/

 
SetDerivedValue(which,value)
int which;
float value;
{
 data[which]=value;
}

/***************************************************************************/

char *
GetDerivedNameList()
{
 return derive_list;
}

/***************************************************************************/

char *
GetRawNameList()
{
 return raw_list;
}

/***************************************************************************/

char *
GetDerivedVarName(which)
int which;
{
 return &derive_list[which*NAMLEN];
}

/***************************************************************************/

char *
GetRawVarName(which)
int which;
{
 return &raw_list[which*NAMLEN];
}

/***************************************************************************/

SetDerivedVarName(which,string)
int which;
char *string;
{
 (void)sprintf(&derive_list[which*NAMLEN],"%s",string);
}

/***************************************************************************/

SetRawVarName(which,string)
int which;
char *string;
{
 (void)sprintf(&raw_list[which*NAMLEN],"%s",string);
}


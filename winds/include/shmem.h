/* shared memory keys:  these must be changed whenever
    the size of the shared memory object INCREASES in size

   two types of keys: shared and non-shared.  Shared keys must all be unique;
   they are effectively keys for read-only shared memory, shared by all
   display stations WINDS processes.  Non-shared keys are keys to 
   session-unique segments of shared memory, effectively isolated from
   other display stations.  Their final values, in calls to getmem(), are
   determined by adding value of variable display_number to following keys.
   Hence, each of the non-shared keys must vary by at least the value of
   the maximum # listed in $WINDS/defaults/netconfig to avoid overlap.

*/

/* processes allocating and processes needing the shared memory id keys
   share information via disk files; here are the I/O lists, associated
   Sdeclarations, etc.
*/

#define DECL_STATIC_ID_VARS  eth_shm_id=0, etid=0, lenraw_id=0, lender_id=0, \
	nPMS1Dprobes_id=0,PMS1Dprobe_id=0 
#define STATIC_ID_VARS  eth_shm_id, etid, lenraw_id, lender_id, \
	nPMS1Dprobes_id,PMS1Dprobe_id 
#define DECL_DYNAMIC_ID_VARS rawid=0, derivid=0, inname_id=0, block_name_id=0, \
	bits_id=0, volts_id=0, time_id=0
#define DYNAMIC_ID_VARS rawid, derivid, inname_id, block_name_id, bits_id, \
        volts_id, time_id

#define STATIC_ID_ARGLIST   "%d %d %d %d %d %d"
#define DYNAMIC_ID_ARGLIST  "%d %d %d %d %d %d %d"

#define READ_STATIC_ID_VARS &eth_shm_id,&etid,\
	&lenraw_id,&lender_id, &nPMS1Dprobes_id, &PMS1Dprobe_id

#define READ_DYNAMIC_ID_VARS &rawid,&derivid,&inname_id,&block_name_id, \
                        &bits_id,&volts_id,&time_id

/* BE SURE LAST_DYNAMIC_ID IS THE SAME AS THE LAST ID IN READ_DYNAMIC_ID_VARS
*/
#define LAST_DYNAMIC_ID time_id

/* shared data segments */

#define HDRKEY             2  /* header segment (no longer used...) */
#define BITSKEY 	   3  /* raw bits */
#define VOLTSKEY 	   4  /* voltages */
#define RAWKEY 		   5  /* raw data segment  */
#define DERIVEKEY 	   6  /* derived data block */
#define ETKEY 		   7  /* elapsed time  */
#define TIME_KEY 	   8  /* time array, in hh:mm:ss */
#define NPMS1DPROBES_KEY  24  /* number of pms1d probes in project */
#define PMS1DPROBE_KEY    25  /* array of data for pms1d probes */

/* shared interprocess communications */

#define LENRAW_KEY 	   14 /* number of valid strings in &inname_[] */
#define LENDER_KEY 	   15 /* number of valid strings in block_name[] */
#define INNAME_KEY 	   16 /* inname_[] array */
#define BLOCK_NAME_KEY 	   17 /* block_name[] array */
#define ETHKEY	   	   18 /* ethernet shared memory access key */
#define WINDCOMM_PIDKEY    19 /* process id for windComm process */
#define INPUT_PIDKEY 	   20 /* process id for input process */

#define DEFAULT_SHMEM_SEX 14400  /* seconds stored in shared mem default */

#define RWUGO 0666          /* mask for creation of shared mem ids */

#define QUEUE_MARGIN    30      /* buffer zone to prevent overrun on writes
                                from input process before display process
                                has a chance to read first point on full
                                refreshes */


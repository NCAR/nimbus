/* config.h -- parameters associated with the proj-specific configuration
*/

/* possible types of arguments
*/
#define VECTORS		0
#define SHMEM_SEX	1
#define MAXSETUPS	2

/* keywords associated with possible arguments
*/
#define VECTORS_KEYWD	"VECTORS"
#define SHMEM_KEYWD	"MAXSEX"
#define MAXSETUPS_KEYWD	"MAXSETUPS"
/*
winds, input command line arguments
 
-F		   # set fullscreendebug for help with server grabs under dbx
-P                 # enable special Control Panel buttons for parcel tracking
-h <display host>  # display host 
-c <blue | black>  # background color 
-m <mode>          # input mode 
-p <pnum>          # project number 
-f <file>          # file name for dataset in post-processing mode only 
-N <pnum-cdffile>  # project # and netcdf file name for dataset in netcdf mode 
-r <pnum-cdffile>  # project # and netcdf file name, raw datafile in, netcdf
		     out, same netcdf in, for full history (development mode)
-D <pathname>      # pathname for DATA_DIR env variable
-d < 0 | 1 | 2 >   # turn debug on, input process only
-q                 # startup QA process
 
in CMDLINE_OPTIONS, add -, n, and o single chars sans arguments for 
getopt()'s sake in get_winds_args(). These args correspond to options
sent to winds script in reference to logging and onboard operation.
*/
#define CMDLINE_OPTIONS "-noh:c:m:p:I:f:d:D:N:r:qFP" 

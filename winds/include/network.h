/* network.h -- parameters describing the display network layout
*/

/* arg to close_netconfig() as function of which get_entry_by_<value> function
   opened the ../defaults/netconfig file.
*/
#define BY_NAME		0
#define BY_LOCN		1
/* 
maximum length of record in netconfig file
*/
#define MAX_NETCONFIG_RECORD_LENGTH 128
/* 
strings used in netconfig file to determine host function
*/
#define NUM_CONTROLHOST_LOCNS	5 	/* all of below */
#define CONTROL_CENTRAL	"ctl"		/* loc'n of host connected to ADS */
#define DEV_CENTRAL	"devctl"  	/* loc'n of winds host using raw data 
				           file for input */
#define NETCDF_MODE     "netcdf"	/* netcdf file as input */
#define INIT_SETUP_MODE	"setup"		/* loc'n of winds doing initial setup */
#define DEVCTL_CDF_MODE "devctl_cdf"	/* raw data file in, write out a netcdf 
					   file, read this file as input */
/*
slave positions
*/
#define NUM_WINDS_SLAVE_LOCNS	13	/* all of below */
#define COCKPIT		"ckp"		/* cockpit location */
#define FORWARD		"fwd"		/* forward cabin location */
#define OUTBOARD_LEFT	"obl"		/* outboard left location */
#define OUTBOARD_RIGHT	"obr"		/* outboard right location */
#define INBOARD_LEFT	"ibl"		/* inboard left location */
#define INBOARD_RIGHT	"ibr"		/* inboard right location */
/*
DHCP postitions
*/
#define DHCP_0          "dy0"
#define DHCP_1          "dy1"
#define DHCP_2          "dy2"
#define DHCP_3          "dy3"
#define DHCP_4          "dy4"
#define DHCP_5          "dy5"
/*
development position
*/
#define DEV		"dev"		/* utility development location */
/* 
input process operation modes
*/
#define ADS_DATA	0
#define NETCDF		1
#define DISK_DATA	2
#define INIT_SETUP	3
#define WINDS_SLAVE	4
#define RAWFILE_NETCDF  5
/*
maximum number of network hosts allowed to participate in
assuming the burden of running winds if previously requested
host is not alive or is already running it.
*/
#define MAXWINDSNETHOSTS 16
/*
define maximum acceptable load on candidate windsnet host above which
that host is deemed too busy to take on a winds display
*/
#define MAXWINDSNETLOAD 3.0

/* 
socket.h -- parameters dealing with socket communications
*/
#define FIRST_AVAILABLE_PORT 	3324
#define MAX_SOCKET_BUFF_LNGTH 	1024
/*
maximum # of expected sessions running concurrently in one WINDS network
...this value should always be less than NUM_PORTS 
*/
#define MAX_EXPECTED_SESSIONS	16
/* 
3 types of sockets -- blocking, non-blocking, block-with-time-out
*/
#define BLOCKING 		0
#define NON_BLOCKING 		1
#define BLOCK_TIME_OUT 		2
#define DEFAULT_TIMEOUT_SECONDS 5
/*
define NUM_PORTS sockets
*/
#define PJETSOCKET 		0
#define HP9876SOCKET 		1
#define EPSONSOCKET 		2
#define ASCIIVALUES 		3
#define FLAT_PANEL_DATA 	4
#define DISP_REQ 		5
#define DISP_REQ_REPLY 		6
#define POSTSCRIPT		7
#define GENERAL_LISTENING	8
#define GENERAL_RESPONSE	9
#define INTERMONITOR_LISTEN	10
#define INTERMONITOR_TALK	(INTERMONITOR_LISTEN+MAX_EXPECTED_SESSIONS)
#define INTERWINDS_LISTEN	(INTERMONITOR_TALK+1)
#define INTERWINDS_TALK		(INTERWINDS_LISTEN+MAX_EXPECTED_SESSIONS)
/*
now increment numbering by MAX_EXPECTED_SESSIONS*2 so as to
reserve that many connections for input requests to diff't
sessions, both for listening AND for talking!
*/
#define GET_NETDATA_SOCKET 	(INTERWINDS_LISTEN+MAX_EXPECTED_SESSIONS*2)
#define REQ_NETDATA 		(GET_NETDATA_SOCKET+1)
#define CALIB_NETDATA 		(GET_NETDATA_SOCKET+2)
#define RAW_NETDATA 		(GET_NETDATA_SOCKET+3)
/*
change this value as more ports are added
*/
#define NUM_PORTS   		(RAW_NETDATA+1)
/*
special broadcast host value; note that if a host named broadcast is targetted
for non-broadcast socket transmission, that transmission *will* be broadcast
(see usage in make_client_socket() )
*/
#define BROADCAST2 		"broadcast"
/*
preamble identifiers of data type to follow in broadcasts of data
*/
#define CALIB_RAW_VARS		0
#define CALIB_DERIVE_VARS	1
#define RAW_VARS		2
/*
serial port server keywords
*/
#define PJETCOLORIMAGE		"pjetcolorimage"
#define PJETBWIMAGE		"pjetbwimage"
#define STATISTICS		"statisticsprint"
#define FLUSHBUFFER		"flushbuffer"
#define PSCOLORIMAGE 		"pscolorimage"
#define PSGREYSCALEIMAGE    	"psgreyscaleimage"
#define PSBWIMAGE    		"psbwimage"
#define TEXTFILE    		"textfile"
/* 
return values
*/
#define BIND_OK 		0
#define CLIENT_OK 		0
#define WRITE_OK 		0
#define EBADREAD 		""
#define NODATA 			"none"
#define EBADOPEN 		-1
#define EWRONGPORT 		-2
#define EBADBIND 		-3
#define EBADNAME 		-4
#define EBADWRITE 		-5
#define EBADHOST 		-6
#define EBADCONN 		-7
/* 
interprocess communication commands/requests
*/
#define ARE_YOU_LISTENING	0
#define LISTENING		1
/* 
initialize parcel tracking parameters 
*/
#define RESET_PARCEL		2   
#define UNDEFINED		3 /* used to have PARCEL_1 and PARCEL_2 here */
/* 
set start time 
*/
#define REQ_SEARCH_FWD		4
#define REQ_SEARCH_BACK		5
#define SHMEM_REFILLED		6
#define NO_SHMEM_REFILLED	7
/* 
freeze/un-freeze input processing
*/
#define REQ_FREEZE		8
#define REQ_UNFREEZE		9
/* 
update rate for input process
*/
#define REQ_SET_UPDATES		10
/* 
start time for input process
*/
#define REQ_SET_START_TIME	11
/* 
request date from input process, analysis mode
*/
#define REQ_DATE		12
/*
broadcast request: none, raw, calib'd, both
*/
#define REQ_BCAST		13
/* 
set rate values to desired # of u-seconds between updates
*/
#define REQ_SET_RATE		14
#define REQ_SET_REALTIME        1000000
#define REQ_SET_WARP2           500000
#define REQ_SET_WARP4           250000
#define REQ_SET_WARP10          20000
/*
requests from input process to display process
*/
#define REQ_CHG_DATE		15
#define REQ_USER_ALERT		16
/*
request between winds processes
*/
#define INITPLOTSPEXFILE	17
#define PLOTSPEXFOLDERREMOVED	18
/*
response from printer processes
*/
#define PRINTRESPONSE		19
/*
update flight # in control panel display
*/
#define UPDATEFLIGHTNUM		20
/*
looking for dataset
*/
#define ISDATASETFOUND		21
#define DATASETISFOUND		22
#define DATASETNOTFOUND		23
#define SEARCHNOTDONE		24

/*
-------------------------------------------------------------------
OBJECT NAME:	ftape.h

DESCRIPTION:	Header file for fortran programs using header API

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------
*/

#ifndef FTAPE_H
#define FTAPE_H

#ifndef NULL
#define NULL            (0)
#endif

#ifndef ERR
#define ERR             (-1)
#endif

#ifndef OK
#define OK              (0)
#endif

#ifndef TRUE
#define TRUE            (1)
#define FALSE           (0)
#endif

#define SPACE           ' '

#define LEAVE_OPEN      0
#define CLOSE           1

/* Struct Types, just add new ones on the bottom, it can only support 32
 */
#define THDR    0x00000000L
#define SH      0x00000001L
#define PMS1D   0x00000002L
#define INS     0x00000004L
#define IRS     0x00000008L
#define DME     0x00000010L
#define EVENT   0x00000020L
#define BLK     0x00000040L
#define PMS2D   0x00000080L
#define ASYNC   0x00000100L
#define PMS1V2  0x00000200L
#define UVHYG	0x00000400L
#define ALL     0xffffffffL


/* Error Messages for 'taperr'          */

#define NOINIT          200     /* InitFlightHeader was not called      */
#define BADTYPE         201     /* The var does not contain that field  */
#define BADVAR          202     /* Var name does not exist in list      */
#define BADHDR          203     /* This was a bad header file           */
#define READERR         204     /* fread failure                        */
#define BADVER          205     /* field does not exist for this version*/



	CHARACTER	GETFIRST*8, GETNEXT*8

	INTEGER	INITFLIGHTHEADER, GETVERSION, GETPROJECTNUMBER
	INTEGER	GETFLIGHTNUMBER, GETTAPENUMBER, GETHEADERDATE, GETHEADERTIME
	INTEGER	GETTIMEZONE, GETAIRCRAFT, GETNUMBERITEMS, GET_LRLEN
	INTEGER	GET_LRPPR, GET_THDRLEN, GETCALCOEFF, GETCONVERSIONFACTOR
	INTEGER	GETITEMLEN, GETITEMTYPE, GETLRLENGTH, GETLRPPR, GETLENGTH
	INTEGER	GETLOCATION, GETNAME, GETORDER, GETPRIMARYADDR, GETRATE
	INTEGER	GETREADADDR1, GETREADEADDR2, GETRESOLUTION, GETSAMPLEOFFSET
	INTEGER	GETSECONDARYADDR, GETSTART, GETTYPE, GETWRITEADDR

#endif

/* exbDefs.h 2.2 1/10/94 NOAO */
 
/*
Developed 1991-1993 by the National Optical Astronomy Observatories(*)
 
(*) Operated by the Association of Universities for Research in
    Astronomy, Inc. (AURA) under cooperative agreement with the
    National Science Foundation.
*/
 
/*
modification history
--------------------
02c,26aug94,rjh  Converted exbLib.h to exbDefs.h for C++.
02b,10jan94,jmv  Modified header to new format.
02a,10jun93,bdg  new release, added compression for 8505s.
01e,02jun93,bdg  added exbLoad().
01d,17jul92,bdg  added option EXB_SUPPRESS_ILI
01c,12nov91,bdg  added EXB_OPT_XFER_RATE_SLOW and FAST to the open modes.
01b,29sep91,bdg  Added open options, space parameters, and additional
                 error number values.
01a,08mar91,bdg  Created.
*/

#ifndef EXBDEFS_H 
#define EXBDEFS_H 


/* exbOpen() options */

#define EXB_OPT_SUPPRESS_ILI		0x0001
#define EXB_OPT_8200			0x0002
#define EXB_OPT_NO_DISCONNECT		0x0004
#define EXB_OPT_PARITY_CHECK		0x0008
#define EXB_OPT_EVEN_BYTE_DISCONNECT	0x0010
#define EXB_OPT_VARIABLE_BLOCK_LENGTH	0x0020
#define EXB_OPT_XFER_RATE_SLOW		0x0040
#define EXB_OPT_XFER_RATE_FAST		0x0080
#define EXB_OPT_UNIT_READY_ON_OPEN	0x0100
#define EXB_OPT_IMMEDIATE		0x0200
#define EXB_OPT_SHORT_FILEMARK		0x0400
#define EXB_OPT_NO_COMPRESS		0x0800
#define EXB_OPT_SENSE_DUMP		0x1000
#define EXB_OPT_UNBUFFERED		0x2000
#define EXB_OPT_CARTRIDGE_112M		0x4000
#define EXB_OPT_REWIND_ON_OPEN		0x8000

/* default buffer sizes for some SCSI commands */

#define EXB_REQUEST_SENSE_SIZE		29
#define EXB_REQUEST_SENSE_SIZE_8500	EXB_REQUEST_SENSE_SIZE
#define EXB_REQUEST_SENSE_SIZE_8200	14
#define EXB_INQUIRY_SIZE		106
#define EXB_INQUIRY_SIZE_8500		EXB_INQUIRY_SIZE
#define EXB_INQUIRY_SIZE_8200		36
#define EXB_MODE_SENSE_SIZE		17
#define EXB_MODE_SENSE_SIZE_8500	EXB_MODE_SENSE_SIZE
#define EXB_MODE_SENSE_SIZE_8200	17
#define EXB_MODE_SENSE_PARAM_SIZE	4
#define EXB_MODE_SENSE_BLOCK_SIZE	8
#define EXB_MODE_SENSE_VENDOR_SIZE	5
#define EXB_BUFFER_SIZE_8505		347688
#define EXB_BUFFER_SIZE_8500		175656
#define EXB_MODE_PAGES_SIZE		64

/* Exabyte density options for MODE_SELECT */

#define EXB_DENSITY_DEFAULT		0x00
#define EXB_DENSITY_8200		0x14
#define EXB_DENSITY_8500		0x15
#define EXB_DENSITY_NO_CHANGE		0x7f
#define EXB_DENSITY_8500C		0x8c
#define EXB_DENSITY_8200C		0x90

/* Exabyte positioning options for SPACE */

#define EXB_SPACE_RECORD		0x00	/* space by records */
#define EXB_SPACE_FILEMARK		0x01	/* by filemarks */
#define EXB_SPACE_END_OF_DATA		0x03	/* to end-of-data */
#define EXB_SPACE_SETMARK		0x04	/* by setmarks (8505 only) */

/* exbOpen() TEST_UNIT_READY this many seconds if EXB_OPT_UNIT_READY_ON_OPEN */

#define EXB_OPEN_WAIT			5	/* seconds max open wait */

/* additional errnos for the scsiLib */

#define S_exbLib_FILEMARK		(M_scsiLib | 0x81)
#define S_exbLib_END_OF_MEDIUM		(M_scsiLib | 0x82)
#define S_exbLib_ILLEGAL_LENGTH		(M_scsiLib | 0x83)
#define S_exbLib_POSITION_UNKNOWN	(M_scsiLib | 0x84)
#define S_exbLib_PHYSICAL_END_OF_TAPE	(M_scsiLib | 0x85)
#define S_exbLib_TARGET_IS_BUSY		(M_scsiLib | 0x90)
#define S_exbLib_TARGET_IS_RESERVED	(M_scsiLib | 0x91)

#endif	/* EXBDEFS_H */

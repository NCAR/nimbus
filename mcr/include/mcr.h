/*
-------------------------------------------------------------------------
OBJECT NAME:	mcr.h

FULL NAME:	Include File to Include the Include Files

DESCRIPTION:	
-------------------------------------------------------------------------
*/

#ifndef DEFINE_H
#define DEFINE_H

#define _NO_PROTO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ERR
#define OK		(0)
#define ERR		(-1)
#endif
 
#define COMMENT	'#' /* Comment character for textfiles  */
 

#define BUFFSIZE	2048
#define PATH_LEN	256
#define NAMELEN		16

#define	SPACE		' '

#define	N_CHANNELS	7

#define TITLESIZE	80

#define HdrBlkTimeToSeconds(hdr) \
	(ntohs(((struct Mcr_rec *)hdr)->second) + \
	(ntohs(((struct Mcr_rec *)hdr)->minute) * 60) + \
	(ntohs(((struct Mcr_rec *)hdr)->hour) * 3600))


typedef struct
	{
	char	instrumentName[16];
	char	projectName[16];
	char	platformName[16];
	char	flightNumber[16];
	long	nBytes_header;
	long	recordLength;
	long	pixPerRecord;
	long	byteOrdering;	/* 0 = little endin, 1 = big endin	*/
	long	missingValue;
	long	nChannels;
	} MCR_HDR;

typedef struct
	{
	char	name[8];
	float	waveLength;	/* or frequency 			*/
	float	bandWidth;
	float	ifov;		/* Instantaneous field of view		*/
	float	swathWidth;
	long	dataType;	/* 1=byte,2=short,3=long,4=float	*/
	long	descriptor;	/* 0=W/cm2-um-sr, 1=degreesK		*/
	long	slope;		/* 0 if unused				*/
	long	intercept;	/* 0 if unused				*/
	long	headerOffset;
	long	dataOffset;
	} MCR_CHAN_INFO;

typedef struct
	{
	long	recordNumber;
	short	year;
	short	month;
	short	day;
	short	hour;
	short	minute;
	short	second;
	short	millisecond;
	short	unused;
	float	scanRate;	/* RPS			*/
	float	latitude;
	float	longitude;
	float	track;
	float	heading;
	float	AGL_alt;
	float	MSL_alt;
	float	ws;		/* Wind Speed		*/
	float	wd;		/* Wind Direction	*/
	float	tas;		/* True airspeed	*/
	float	gspd;		/* Ground speed		*/
	float	solarZenith;
	float	solarAzimuth;
	float	pixWidth;	/* Cross track		*/
	float	pixLength;	/* Along track		*/
	} MCR_REC_HDR;

typedef struct
	{
	char	name[8];
	float	waveLength;
	long	dataQuality;	/* 0=good, non-zero is bad, with exact meanings
				 * defined in Global header comments.	*/
	} MCR_CHANNEL_HDR;

#endif

/* END MCR.H */

/*
-------------------------------------------------------------------------
OBJECT NAME:	oldhdr.h

FULL NAME:		HP hdr struct

COPYRIGHT:		University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

struct old_hdr
	{
	short	flag;			/* = -1		*/
	short	tapenum;
	short	flightnum;
	short	yymm;
	short	ddhh;
	short	mmss;
	char	aircraft[4];
	char	projname[4];
	short	hdr_len;		/* = 1655	*/
	short	lrppr;
	short	lrlen;
	char	unused1[14];

	char	blkName[8][4];
	short	blkSiz[8];
	short	startWord[8];
	short	lastWord[8];
	short	maxPhysChannel;	/* = 120	*/
	short	locate[120][2];
	short	maxBuff;		/* = 10		*/
	short	buffOffset[10];
	short	maxSDI;			/* = 120	*/
	short	sampleRate[120];
	char	SDIname[120][6];
	short	maxAnalogChannel;	/* = 120	*/
	float	cals[3][120];
	short	nFull;
	char	type[120][2];	/* "DI" or "AN"	*/
	char	pmsName[4][4];
	short	probeSize[4];
	short	pmsRate[4];
	short	pmsStart[4];
	};


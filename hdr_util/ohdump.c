/*
-------------------------------------------------------------------------
OBJECT NAME:	ohdump.c

FULL NAME:		Old Header Dump

ENTRY POINTS:	main

STATIC FNS:	

DESCRIPTION:	

INPUT:			Old HP style header

OUTPUT:			New header.

COPYRIGHT:		University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include "oldhdr.h"

float	fphp2mc();

/* -------------------------------------------------------------------- */
main(argc, argv)
int		argc;
char	*argv[];
{
	int		i, n = 120;
	struct old_hdr	OldHdr;
	FILE	*fp;

	if ((fp = fopen(argv[1], "r")) == NULL)
		{
		fprintf(stderr, "Can't open %s\n", argv[1]);
		exit(1);
		}

	fread(&OldHdr, sizeof(struct old_hdr), 1, fp);
	fclose(fp);

	printf("Flag = %d\n", OldHdr.flag);
	printf("TapeNum = %d\n", OldHdr.tapenum);
	printf("FlightNum = %d\n", OldHdr.flightnum);
	printf("Date = %d/%d/%d\n", OldHdr.yymm & 0x00ff, OldHdr.ddhh >> 8, OldHdr.yymm >> 8);
	printf("Time = %d:%d:%d\n", OldHdr.ddhh & 0x00ff, OldHdr.mmss >> 8, OldHdr.mmss & 0x00ff);

	printf("Aircraft = %-4.4s\n", OldHdr.aircraft);
	printf("ProjName = %-4.4s\n", OldHdr.projname);
	printf("HdrLen = %d, better = 1655\n", OldHdr.hdr_len);
	printf("lrppr = %d\n", OldHdr.lrppr);
	printf("lrlen = %d\n", OldHdr.lrlen);


	/* SDI vars	*/
	printf("\nName  Rate  Start Type Chnl            c0           c1           c2\n");

	fphp2mc_(OldHdr.cals[0], &n);
	fphp2mc_(OldHdr.cals[1], &n);
	fphp2mc_(OldHdr.cals[2], &n);

	for (i = 0; i < OldHdr.nFull; ++i)
		printf("%-6.6s %3d  %5d %-2.2s %5d   %12.6f %12.6f %12.6f\n",
			OldHdr.SDIname[i],
			OldHdr.sampleRate[i],
			OldHdr.locate[i][1],
			OldHdr.type[i],
			OldHdr.locate[i][0],
			OldHdr.cals[0][i], OldHdr.cals[1][i], OldHdr.cals[2][i]);


	/* Block Probes	*/
	printf("\nName   Size  Start   Last\n");

	for (i = 0; i < 8; ++i)
		if (OldHdr.blkSiz[i] > 0)
			printf("%-4.4s %6d %6d %6d\n", OldHdr.blkName[i], OldHdr.blkSiz[i],
									OldHdr.startWord[i], OldHdr.lastWord[i]);


	/* PMS1D probes	*/
	printf("\nName  Rate  Start  Size\n");

	for (i = 0; i < 4; ++i)
		printf("%-4.4s %4d %6d %6d\n", OldHdr.pmsName[i], OldHdr.pmsRate[i],
			OldHdr.pmsStart[i], OldHdr.probeSize[i]);

	printf("\n\n");

	return(0);

}	/* END MAIN */

/* -------------------------------------------------------------------- */
fphp2mc_(a, n)
int	a[], *n;
{
	int signfr,frac,expon,signex,i;

	for (i = 0; i < *n; i++)
		{
		if (a[i] == 0x00000000)
			continue; 

		signfr=a[i] & 0x80000000;
		frac = a[i] & 0x3fffff00;
		expon= a[i] & 0xfe;
		signex=a[i] & 1;
		frac= frac>>7;
		if (signfr != 0)
			frac = (frac ^ 0x7fffff) + 1;
		expon= expon >> 1;
		expon= expon+126-signex*128 ;
		expon= expon << 23;
		a[i] = signfr+expon+frac;
		}

}

/* END OHDUMP.C */

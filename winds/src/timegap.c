/*
-------------------------------------------------------------------------
OBJECT NAME:	timegap.c

ENTRY POINTS:	CheckForRealTimeGap()

STATIC FNS:	none

DESCRIPTION:	Check for anomolies in time sequence of ADS data for
		realtime operation.

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	winputops.c, windComm.cc

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1995
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include "header.h"
#include <netinet/in.h>

#define HdrBlkTimeToSeconds(hdr)		\
	(ntohs(((struct Hdr_blk  *)hdr)->second) +	\
	(ntohs(((struct Hdr_blk  *)hdr)->minute) * 60) +	\
	(ntohs(((struct Hdr_blk  *)hdr)->hour) * 3600))


/* -------------------------------------------------------------------- */
void CheckForRealTimeGap(char *prog, Hdr_blk *ADShdr)
{
  long    	new_time;
  static int	prev_time = (-1);

  new_time = (long)HdrBlkTimeToSeconds(ADShdr);

  /* If everthing is peachy, then bail out
  */
  if (prev_time+1 == new_time || prev_time == (-1) ||
    (new_time == 0 && prev_time == 86399L)) /* Midnight crossover */
    ;
  else
  if (prev_time == new_time)
    fprintf(stderr, "%s: Duplicate time stamp @ %02d:%02d:%02d.\n",
      prog, ntohs(ADShdr->hour), ntohs(ADShdr->minute), ntohs(ADShdr->second));
  else
  if (new_time < prev_time)
    fprintf(stderr, "%s: Time went backwards @ %02d:%02d:%02d.\n",
      prog, ntohs(ADShdr->hour), ntohs(ADShdr->minute), ntohs(ADShdr->second));
  else
    fprintf(stderr, "%s: Time gap @ %02d:%02d:%02d.\n",
      prog, ntohs(ADShdr->hour), ntohs(ADShdr->minute), ntohs(ADShdr->second));

  prev_time = new_time;

}	/* END CHECKFORREALTIMEGAP */

/* END TIMEGAP.C */

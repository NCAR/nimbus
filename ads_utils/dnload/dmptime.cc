/*
-------------------------------------------------------------------------
OBJECT NAME:	dmptime.cc

FULL NAME:	ADS tape copier & downloader.

DESCRIPTION:	Print time stamps from all records in an ADS file.
		to individual disk files [ads|2d|MCR|AVAPS].

COPYRIGHT:	University Corporation for Atmospheric Research, 2003-06
-------------------------------------------------------------------------
*/

#include <cctype>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

#include <netinet/in.h>

#include "adsIO.h"


char	buffer[0x8000];
char	sourceTape[200];

int WriteDisk(char buff[], int nBytes);


/* -------------------------------------------------------------------- */
main(int argc, char *argv[])
{
  int	rc, nBytes;
  ADS_DataFile	*source;

  if (argc < 2)
  {
    printf("Enter file : ");
    fgets(sourceTape, 200, stdin);
    sourceTape[strlen(sourceTape)-1] = '\0';
  }
  else
    strcpy(sourceTape, argv[1]);

  source = new ADS_DataFile(sourceTape);


  rc = 0;	/* Return Code	*/

  while ((nBytes = source->NextPhysicalRecord(buffer)) > 0)
    {
//    if (strncmp(buffer, "MCR", 3) == 0)
    WriteDisk(buffer, nBytes);
    }


  if (nBytes == ERR)
    {
    fprintf(stderr, "Read error, errno = %d.\n", errno);
    rc = 1;
    }


  return(rc);

}	/* END MAIN */

/* -------------------------------------------------------------------- */
int WriteDisk(char buff[], int nBytes)
{
  int	rc, thisTime;
  Hdr_blk *hdr;
  P2d_rec *p2d;
  Mcr_rec *mcr;

  static size_t recCntr = 0;
  static size_t prevTime = 0;

  hdr = (Hdr_blk *)buffer;
  p2d = (P2d_rec *)buffer;
  mcr = (Mcr_rec *)buffer;

  switch (ntohs(*(unsigned short *)buff))
    {
    case 0x8681:
      thisTime = ntohs(hdr->hour) * 3600 + ntohs(hdr->minute) * 60 + ntohs(hdr->second);

      printf("[%d] Sync, nBytes=%d, %02d/%02d/%02d - %02d:%02d:%02d",
	recCntr++, nBytes,
	ntohs(hdr->year), ntohs(hdr->month), ntohs(hdr->day),
	ntohs(hdr->hour), ntohs(hdr->minute), ntohs(hdr->second));

      if (prevTime + 1 != thisTime)
        printf(" gap?");

      printf("\n");
      prevTime = thisTime;
      break;

    case 0x4331:	// PMS2D types.
    case 0x4332:
    case 0x5031:
    case 0x5032:
    case 0x4731:
    case 0x4732:
    case 0x4831:
    case 0x4832:
        printf("PMS 2d %c, nBytes=%d\n", buff[0], nBytes);
      for (int i = 0; i < 7; ++i)
        printf("  %c %02d:%02d:%02d.%03d, tas=%d\n", ((char*)&p2d[i])[0], ntohs(p2d[i].hour), ntohs(p2d[i].minute), ntohs(p2d[i].second), ntohs(p2d[i].msec), ntohs(p2d[i].tas));
      break;

    case 0x4d43:	// MCR
      printf("    %s, nBytes=%d, %02d:%02d:%02d\n", mcr->id, nBytes, ntohs(mcr->hour), ntohs(mcr->minute), ntohs(mcr->second));
      break;
    }

  return(OK);

}	/* END WRITEDISK */

/* END DNLOAD.CC */

/*
-------------------------------------------------------------------------
OBJECT NAME:	dnload.cc

FULL NAME:	ADS tape copier & downloader.

DESCRIPTION:	Copy a tape from one tape to another and also dump a copy
		to individual disk files [ads|2d|MCR|AVAPS].

INPUT:		

OUTPUT:		

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1992-9
-------------------------------------------------------------------------
*/

#include <cctype>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

#include "adsIO.h"


char	buffer[65000];
char	sourceTape[64], destTape[64], destFile[80], dest2d[80], destMCR[80],
	destAVAPS[80], destMASP[80];
int	outTape, outDisk, out2D, outMCR, outAVAPS, outMASP;
int	cntTape, cntDisk, cnt2D, cntMCR, cntAVAPS, cntMASP;
int	pms2d = false, mcr = false, avaps = false, masp = false;

void	CloseDiskFiles();
int	WriteDisk(char buff[], int nBytes);

Hdr_blk	startTime, endTime;

int	hdrLen = 0;
char	hdrBuffer[65000];

/* -------------------------------------------------------------------- */
main(int argc, char *argv[])
{
  int	rc, nBytes;
  ADS_DataFile	*source;


  outTape = outDisk = out2D = outMCR = outAVAPS = outMASP = ERR;


  printf("Enter source tape drive [/dev/rmt/0] : ");
  gets(sourceTape);

  if (strlen(sourceTape) == 0)
    strcpy(sourceTape, "/dev/rmt/0");


  printf("Enter destination tape drive [return for no tape copy] : ");
  gets(destTape);

  printf("Enter destination disk file [return for no disk file] : ");
  gets(destFile);

  if (strlen(destTape) == 0 && strlen(destFile) == 0)
    {
    fprintf(stderr, "Ppppphhhhhhtttttt!\n");
    return(1);
    }


  source = new ADS_DataFile(sourceTape);


  if (strlen(destFile))
    {
    if (strstr(destFile, ".ads") == 0)
      strcat(destFile, ".ads");

    if (source->hdr->GetFirst(PMS2D_STR))
      {
      do
        {
        printf("Do you want a PMS 2D disk file (y/n) ? ");
        gets(buffer);
        }
      while (toupper(buffer[0]) != 'Y' && toupper(buffer[0]) != 'N');

      pms2d = toupper(buffer[0]) == 'Y' ? true : false;
      }

    if (source->hdr->GetSDI("MCROUT"))
      {
      do
        {
        printf("Do you want a MCR disk file (y/n) ? ");
        gets(buffer);
        }
      while (toupper(buffer[0]) != 'Y' && toupper(buffer[0]) != 'N');

      mcr = toupper(buffer[0]) == 'Y' ? true : false;
      }

    if (source->hdr->GetFirst(AVAPS_STR))
      {
      do
        {
        printf("Do you want an AVAPS disk file (y/n) ? ");
        gets(buffer);
        }
      while (toupper(buffer[0]) != 'Y' && toupper(buffer[0]) != 'N');

      avaps = toupper(buffer[0]) == 'Y' ? true : false;
      }

    if (source->hdr->GetFirst(MASP_STR))
      {
      do
        {
        printf("Do you want an MASP disk file (y/n) ? ");
        gets(buffer);
        }
      while (toupper(buffer[0]) != 'Y' && toupper(buffer[0]) != 'N');

      masp = toupper(buffer[0]) == 'Y' ? true : false;
      }
    }



  printf("\n");

  if (strlen(destTape))
    {
    printf("Tape copy from %s to %s.\n", sourceTape, destTape);

    if ((outTape = open(destTape, O_WRONLY)) == ERR)
      {
      fprintf(stderr, "Can not open destination tape %s\n", destTape);
      return(1);
      }
    }


  if (strlen(destFile))
    {
    char *p;

    printf("Downloading tape to disk file %s.\n", destFile);
    strcpy(dest2d, destFile);
    strcpy(destMCR, destFile);
    strcpy(destAVAPS, destFile);
    strcpy(destMASP, destFile);

    if (pms2d)
      {
      if ((p = strchr(dest2d, '.')) == NULL)
        strcat(dest2d, ".2d");
      else
        strcpy(p, ".2d");

      printf("PMS 2D data to disk file %s.\n", dest2d);
      }

    if (mcr)
      {
      if ((p = strchr(destMCR, '.')) == NULL)
        strcat(destMCR, ".rawMCRa");
      else
        strcpy(p, ".rawMCRa");

      printf("MCR data to disk file %s.\n", destMCR);
      }

    if (avaps)
      {
      if ((p = strchr(destAVAPS, '.')) == NULL)
        strcat(destAVAPS, ".AVAPS");
      else
        strcpy(p, ".AVAPS");

      printf("AVAPS data to disk file %s.\n", destAVAPS);
      }

    if (masp)
      {
      if ((p = strchr(destMASP, '.')) == NULL)
        strcat(destMASP, ".MASP");
      else
        strcpy(p, ".MASP");

      printf("MASP data to disk file %s.\n", destMASP);
      }




    if ((outDisk = open(destFile, O_CREAT | O_WRONLY | O_TRUNC, 0666)) == ERR)
      {
      fprintf(stderr, "Can not open destination file %s\n", destFile);
      return(1);
      }

    if (pms2d &&
       (out2D = open(dest2d, O_CREAT | O_WRONLY | O_TRUNC, 0666)) == ERR)
      {
      fprintf(stderr, "Can not open 2D destination file %s\n", dest2d);
      return(1);
      }

    if (mcr &&
       (outMCR = open(destMCR, O_CREAT | O_WRONLY | O_TRUNC, 0666)) == ERR)
      {
      fprintf(stderr, "Can not open MCR destination file %s\n", destMCR);
      return(1);
      }

    if (avaps &&
       (outAVAPS = open(destAVAPS, O_CREAT | O_WRONLY | O_TRUNC, 0666)) == ERR)
      {
      fprintf(stderr, "Can not open AVAPS destination file %s\n", destAVAPS);
      return(1);
      }

    if (masp &&
       (outMASP = open(destMASP, O_CREAT | O_WRONLY | O_TRUNC, 0666)) == ERR)
      {
      fprintf(stderr, "Can not open MASP destination file %s\n", destMASP);
      return(1);
      }
    }


  rc = 0;	/* Return Code	*/
  cntTape = cntDisk = cnt2D = cntAVAPS = 0;

  while ((nBytes = source->NextPhysicalRecord(buffer)) > 0)
    {
    if (outTape != ERR)
      {
      if (write(outTape, buffer, nBytes) == ERR)
        {
        fprintf(stderr, "Write failure to dest tape, errno = %d.\n", errno);
        rc = 1;
        break;
        }
      else
        ++cntTape;
      }

    if (outDisk != ERR)
      if (WriteDisk(buffer, nBytes) == ERR)
        break;
    }


  if (nBytes == ERR)
    {
    fprintf(stderr, "READ ERROR, errno = %d.\n", errno);
    rc = 1;
    }

  printf("\n\n");

  printf("%02d:%02d:%02d - %02d:%02d:%02d\n\n",
    startTime.hour, startTime.minute, startTime.second,
    endTime.hour, endTime.minute, endTime.second);

  if (outTape != ERR)
    {
    close(outTape);
    printf("%d records successfully written to tape copy.\n", cntTape);
    }

  CloseDiskFiles();
  return(rc);

}	/* END MAIN */

/* -------------------------------------------------------------------- */
void CloseDiskFiles()
{
  if (outDisk != ERR)
    {
    close(outDisk);
    printf("%d records successfully written to disk.\n", cntDisk);
    }

  if (out2D != ERR)
    {
    close(out2D);
    printf("%d PMS 2D records successfully written to disk.\n", cnt2D);
    }

  if (outMCR != ERR)
    {
    close(outMCR);
    printf("%d MCR records successfully written to disk.\n", cntMCR);
    }

  if (outAVAPS != ERR)
    {
    close(outAVAPS);
    printf("%d AVAPS records successfully written to disk.\n", cntAVAPS);
    }

  if (outMASP != ERR)
    {
    close(outMASP);
    printf("%d MASP records successfully written to disk.\n", cntMASP);
    }

  outDisk = out2D = outMCR = outAVAPS = outMASP = ERR;
  pms2d = mcr = avaps = masp = false;

}	/* END CLOSEDISKFILES */

/* -------------------------------------------------------------------- */
int WriteDisk(char buff[], int nBytes)
{
  int	rc;
  static bool firstSync = true;

  switch (*(unsigned short *)buff)
    {
    case 0x8681:
      rc = write(outDisk, buff, nBytes);
      if (firstSync)
        {
        memcpy((char *)&startTime, buff, sizeof(Hdr_blk));
        firstSync = false;
        }
      else
        memcpy((char *)&endTime, buff, sizeof(Hdr_blk));

      ++cntDisk;
      break;

    case 0x4331:	// PMS2D types.
    case 0x4332:
    case 0x5031:
    case 0x5032:
    case 0x4731:
    case 0x4732:
      if (!pms2d)
        return(OK);

      rc = write(out2D, buff, nBytes);
      ++cnt2D;
      break;

    case 0x4d43:	// MCR
      if (!mcr)
        return(OK);

      rc = write(outMCR, buff, nBytes);
      ++cntMCR;

      if (cntMCR >= 100800)	// close file after 4 hours.
        {
        close(outMCR);
        ++destMCR[strlen(destMCR)-1];	// increment file name.

        if ((outMCR = open(destMCR, O_CREAT | O_WRONLY | O_TRUNC, 0666)) == ERR)
          {
          fprintf(stderr, "Can not open MCR destination file %s\n", destMCR);
          fprintf(stderr, "Continuing without MCR disk file.\n");
          mcr = false;
          }

        write(outMCR, hdrBuffer, hdrLen);
        cntMCR = 0;
        }

      break;

    case 0x4156:	// AVAPS Dropsonde.
      if (!avaps)
        return(OK);

      rc = write(outAVAPS, buff, nBytes);
      ++cntAVAPS;
      break;

    case 0x4450:	// MASP.
      if (!masp)
        return(OK);

      rc = write(outMASP, buff, nBytes);
      ++cntMASP;
      break;

    case 0x4144:	// Header records.
    case 0x5448:
      memcpy(&hdrBuffer[hdrLen], buff, nBytes);
      hdrLen += nBytes;

      rc = write(outDisk, buff, nBytes);

      if (pms2d)
        rc = write(out2D, buff, nBytes);

      if (mcr)
        rc = write(outMCR, buff, nBytes);

      break;
    }


  if (rc == ERR)
    {
    if (errno == ENOSPC)
      {
      fprintf(stderr, ">>> DISK FULL, closing disk file(s) and continuing.\n");
      CloseDiskFiles();
      }

    fprintf(stderr, "Write failure to dest disk, errno = %d.\n", errno);
    return(ERR);
    }

  return(OK);

}	/* END WRITEDISK */

/* END DNLOAD.CC */

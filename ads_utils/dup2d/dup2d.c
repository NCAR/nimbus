/*
-------------------------------------------------------------------------
OBJECT NAME:	dup2d.c

FULL NAME:	ADS tape copier & downloader.

DESCRIPTION:	Remove duplicate 2d records from a file.

INPUT:		

OUTPUT:		

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1999
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include "header.h"

#define ERR	(-1)

#define TRUE	1
#define FALSE	0

char	buffer[65000];
char	sourceTape[64], destTape[64], destFile[80];
int	outTape, outDisk, out2D;
int	cntDisk, cntTape, cnt2D, cntAVAPS;
int	pms2d;

int	cntP2Dtossed, cntP2D, cntC2Dtossed, cntC2D;

void WriteDisk(char buff[], int nBytes);


/* -------------------------------------------------------------------- */
main(int argc, char *argv[])
{
  int		rc, nBytes;
  P2d_rec	prevP, prevC;


  outTape = outDisk = out2D = ERR;


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


  if (strlen(destFile))
    {
    do
      {
      printf("Do you want 2D data (y/n) ? ");
      gets(buffer);
      }
    while (toupper(buffer[0]) != 'Y' && toupper(buffer[0]) != 'N');

    pms2d = toupper(buffer[0]) == 'Y' ? TRUE : FALSE;
    }
  else
    pms2d = FALSE;



  printf("\n");

  if (TapeOpen(sourceTape) == ERR)
    {
    fprintf(stderr, "Can't open source tape %s\n", sourceTape);
    return(1);
    }


  printf("\n\n");

  if (strlen(destTape))
    {
    printf("Tape copy from %s to %s.\n", sourceTape, destTape);

    if ((outTape = open(destTape, O_WRONLY)) == ERR)
      {
      fprintf(stderr, "Can't open destination tape %s\n", destTape);
      TapeClose();
      return(1);
      }
    }


  if (strlen(destFile))
    {
    char *p;

    strcpy(buffer, destFile);

    if ((p = strchr(buffer, '.')) == NULL)
      p = buffer;

    strcpy(p, ".2D");


    printf("Downloading tape to disk file %s.\n", destFile);
    printf("PMS 2D data to disk file %s.\n", buffer);


    if ((outDisk = open(destFile, O_CREAT | O_WRONLY | O_TRUNC, 0666)) == ERR)
      {
      fprintf(stderr, "Can't open destination file %s\n", destFile);
      TapeClose();
      if (outTape != ERR) close(outTape);
      return(1);
      }

    if (pms2d && (out2D = open(buffer, O_CREAT | O_WRONLY | O_TRUNC, 0666)) == ERR)
      {
      fprintf(stderr, "Can't open 2D destination file %s\n", buffer);
      TapeClose();
      if (outTape != ERR) close(outTape);
      close(outDisk);
      return(1);
      }
    }


  rc = 0;	/* Return Code	*/
  cntTape = cntDisk = cnt2D = cntAVAPS = 0;
  cntP2Dtossed = cntP2D = cntC2Dtossed = cntC2D = 0;

  while ((nBytes = TapeRead(buffer)) > 0)
    {
    if (outTape != ERR)
      {
      if (write(outTape, buffer, nBytes) == ERR)
        {
        fprintf(stderr, "Write failure to dest tape, errno = %d.\n", errno);
        rc = 1;
        goto exit;
        }
      else
        ++cntTape;
      }

    if (outDisk != ERR)
      WriteDisk(buffer, nBytes);

    if (outTape == ERR && outDisk == ERR && out2D == ERR)
      break;
    }


  if (nBytes == ERR)
    {
    fprintf(stderr, "Read error, errno = %d.\n", errno);
    rc = 1;
    }

exit:
  printf("\n\n");
  TapeClose();

  if (outTape != ERR)
    {
    close(outTape);
    printf("%d records successfully written to tape copy.\n", cntTape);
    }

  if (outDisk != ERR)
    {
    close(outDisk);
    printf("%d records successfully written to disk.\n", cntDisk);
    }

  if (out2D != ERR)
    {
    close(out2D);
    printf("%d 2D-P logical records found, %d non-duplicates.\n", cntP2D, cntP2Dtossed);
    printf("%d 2D-C logical records found, %d non-duplicates.\n", cntC2D, cntC2Dtossed);
    }

  if (cntAVAPS != 0)
    printf("%d AVAPS records completely ignored.\n", cntAVAPS);

  return(rc);

}	/* END MAIN */

/* -------------------------------------------------------------------- */
void WriteDisk(char buff[], int nBytes)
{
  int		rc, i;
  P2d_rec	*p2, prevP, prevC;

  switch (*(unsigned short *)buff)
    {
    case 0x8681:
      rc = write(outDisk, buff, nBytes);
      ++cntDisk;
      break;

    case 0x4331:	/* PMS2D types. */
    case 0x4332:
    case 0x5031:
    case 0x5032:
    case 0x4731:
    case 0x4732:
      if (!pms2d)
        return;

      for (p2 = (P2d_rec *)buff, i = 0; i < 7; ++i, ++p2)
        {
        if (((char *)p2)[0] == 'P')
          {
          if (memcmp((void *)p2, (void *)&prevP, sizeof(P2d_rec)))
            {
            rc = write(out2D, (void *)p2, sizeof(P2d_rec));
            memcpy((void *)&prevP, (void *)p2, sizeof(P2d_rec));
            ++cntP2Dtossed;
            }

          ++cntP2D;
          }
        else
          {
          if (memcmp((void *)p2, (void *)&prevC, sizeof(P2d_rec)))
            {
            rc = write(out2D, (void *)p2, sizeof(P2d_rec));
            memcpy((void *)&prevC, (void *)p2, sizeof(P2d_rec));
            ++cntC2Dtossed;
            }

          ++cntC2D;
          }
        }

      break;

    case 0x4156:	/* AVAPS Dropsonde. */
      ++cntAVAPS;
      break;

    case 0x4144:	/* Header records. */
    case 0x5448:
      rc = write(outDisk, buff, nBytes);
      rc = write(out2D, buff, nBytes);
      break;
    }


  if (rc == ERR)
    {
    if (errno == ENOSPC)
      {
      fprintf(stderr, "Disk full, closing disk file(s) and continuing.\n");
      printf("%d records successfully written to disk.\n", cntDisk);
      printf("%d 2D records successfully written to disk.\n", cnt2D);
      close(outDisk);
      close(out2D);
      outDisk = out2D = ERR;
      }

    fprintf(stderr, "Write failure to dest disk, errno = %d.\n", errno);
    }

}	/* END WRITEDISK */

/* END DNLOAD.C */

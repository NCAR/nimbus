/*
-------------------------------------------------------------------------
OBJECT NAME:	tptcp.c

FULL NAME:	Tape Tee Copy

DESCRIPTION:	Copy a tape from one tape to another and also dump a copy
		to disk sans 2d records

INPUT:		

OUTPUT:		

NOTES:		gcc tptcp.c -lraf -o tptcp

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#define STDERR(msg)		fprintf(stderr, msg)
#define STDERRA(msg, arg)	fprintf(stderr, msg, arg)

#define ERR	(-1)

/* -------------------------------------------------------------------- */
main(argc, argv)
int	argc;
char	*argv[];
{
  int	outd, diskd, rc, nbytes, cnt = 0, flag;
  char	buffer[65000];

  if (argc < 3)
    {
    STDERR("usage: tptcp src dest [disk_file]\n");
    STDERR(" example: tptcp /dev/rex0 /dev/rex1 /home/tmp/ads.out\n");
    return(1);
    }

  if (argc < 4)
    printf("No disk file being created.\n");


  if (TapeOpen(argv[1]) == ERR)
    {
    STDERRA("Can't open source tape %s\n", argv[1]);
    return(1);
    }

  if ((outd = open(argv[2], O_WRONLY)) == ERR)
    {
    STDERRA("Can't open destination tape %s\n", argv[2]);
    TapeClose();
    return(1);
    }

  if (argc == 4 &&
      (diskd = open(argv[3], O_CREAT | O_WRONLY | O_TRUNC, 0666)) == ERR)
    {
    STDERRA("Can't open destination file %s\n", argv[3]);
    TapeClose();
    close(outd);
    return(1);
    }


  rc = 0;	/* Return Code	*/
  flag = 1;

  while ((nbytes = TapeRead(buffer)) > 0)
    {
    if (flag && *(unsigned short *)buffer == 0x8681 && ((unsigned short *)buffer)[1] < 12)
      continue;

    if (*(unsigned short *)buffer == 0x8681 && ((unsigned short *)buffer)[1] > 20)
      flag = 0;

    if (write(outd, buffer, nbytes) == ERR)
      {
      STDERRA("Write failure to dest tape, errno = %d\n", errno);
      rc = 1;
      goto exit;
      }

    if (*(unsigned short *)buffer == 0x8681 ||
        *(unsigned short *)buffer == 0x4144 ||
        *(unsigned short *)buffer == 0x5448)
      {
      if (argc == 4 && write(diskd, buffer, nbytes) == ERR)
        {
        if (errno == ENOSPC)
          {
          STDERR("Disk full, closing disk file and continuing.\n");
          STDERRA("%d records successfully written to disk file.\n", cnt);
          close(diskd);
          argc = 3;
          }

        STDERRA("Write failure to dest disk, errno = %d\n", errno);
        rc = 1;
        goto exit;
        }
      }

    ++cnt;
    }

  if (nbytes == ERR)
    {
    STDERRA("Read error, errno = %d\n", errno);
    rc = 1;
    }

exit:
  TapeClose();
  close(outd);

  if (argc == 4)
    close(diskd);

  printf("%d records successfully read and written\n", cnt);
  return(rc);

}	/* END MAIN */

/* END TPTCP.C */

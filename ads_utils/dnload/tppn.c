/*
:set tabstop=4
-------------------------------------------------------------------------
OBJECT NAME:	tptcp.c

FULL NAME:		Tape Tee Copy

DESCRIPTION:	Copy a tape from one tape to another and also dump a copy
				to disk sans 2d records

INPUT:		

OUTPUT:		

NOTES:			gcc tptcp.c -lraf -o tptcp

COPYRIGHT:		University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#include "/opt/local/include/raf/header.h"

#define STDERR(msg)		fprintf(stderr, msg)
#define STDERRA(msg, arg)	fprintf(stderr, msg, arg)

#define ERR	(-1)

main(argc, argv)
int	argc;
char	*argv[];
{
	int	outd, rc, nbytes, cnt = 0;
	char	buffer[65000], date[32], fltnum[32];

	if (argc < 3)
		{
		STDERR("usage: tptcp src dest disk_file\n");
		STDERR(" example: tptcp /dev/rex0 /dev/rex1\n");
		return(1);
		}


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


	printf("Enter flight number : ");
	gets(fltnum);

	printf("Enter date [mm/dd/yyyy]: ");
	gets(date);

	rc = 0;	/* Return Code	*/

	while ((nbytes = TapeRead(buffer)) > 0)
		{
		if (*(unsigned short *)buffer == 0x5448)
			{
			struct Fl	*p = (struct Fl *)buffer;

			strcpy(p->fltnum, fltnum);
			strcpy(p->date, date);
			strcpy(p->prnum, "127");
			}

		if (write(outd, buffer, nbytes) == ERR)
			{
			STDERRA("Write failure to dest tape, errno = %d\n", errno);
			rc = 1;
			goto exit;
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

	printf("%d records successfully read and written.\n", cnt);

{	int	hour, min, sec;

	cnt -= 3;
	hour = cnt / 3600;
	min = (cnt - (hour * 3600)) / 60;
	sec = cnt - (hour * 3600) - (min * 60);
	printf("Flight duration = %02d:%02d:%02d\n", hour, min, sec);
	}
	return(rc);

}

/*
-------------------------------------------------------------------------
OBJECT NAME:	hdrdump.c

FULL NAME:	Header Dump

ENTRY POINTS:	main()

DESCRIPTION:	

INPUT:		Filename of header

OUTPUT:		Tape dump to stdout

REFERENCES:	Header API (libhdr_api.a)

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include "ctape.h"


char	buffer[1024];

char	*sdititle =
	"\nType    Name    Start  Len  Rate  Offset Type\n";

char	*otitle =
	"\n\n  Block name  Start byte     Length (bytes)\n";
	
char	*atitle =
	"\n\n  Async blocks\n";
	


/* -------------------------------------------------------------------- */
main(argc, argv)
int	argc;
char	*argv[];
{
	int	i;
	char	*cvar;		/* Pointer to current variable	*/
	char	*cp[3];		/* Some generic Char Pointers	*/
	long	l;		/* Long result storage		*/
	float	f;		/* Float result storage		*/
	float	*cal;		/* Cal Coeff storage		*/


	if (argc < 2)
		{
		fprintf(stderr, "usage:\thdrdump [prj# | headerfile]\n");
		exit(1);
		}


	/* Deterimine if header feader filename is a project# or filename
	 */
	if (isdigit(argv[1][0]))
		sprintf(buffer, "/home/local/winds/proj/%s/header", argv[1]);
	else
		strcpy(buffer, argv[1]);


	if (InitFlightHeader(buffer, CLOSE) == ERR)
		{
		fprintf(stderr, "hdrdump: init error, taperr = %d\n", taperr);
		exit(1);
		}


	GetAircraft(&cp[0]);
	GetProjectNumber(&cp[1]);
	GetFlightNumber(&cp[2]);
	printf(" Aircraft: %s, Project: %s, Flight: %s\n", cp[0], cp[1], cp[2]);

	GetHeaderDate(&cp[0]);
	GetHeaderTime(&cp[1]);
	printf(" Date: %s  Time: %s\n", cp[0], cp[1]);

	GetNumberItems(&l);
	printf(" Number of data items: %ld", l);
	get_lrlen(&l);
	printf(" Logical record length: %ld\n", l);



	/* Do SDI vars first
	 */
	printf(sdititle);

	cvar = GetFirst();

	while (cvar = GetNext())
		{
		GetItemType(cvar, &cp[0]);
		if (!(strcmp(cp[0], "SDI") == 0 || strcmp(cp[0], "HSKP") == 0
		   || strcmp(cp[0], "DIGOUT") == 0))
			continue;

		printf("%-8s%-8s ", cp[0], cvar);

		if (GetStart(cvar, &l) == ERR)
			printf("<%d> ", taperr);
		else
			printf("%4ld ", l);

		if (GetLength(cvar, &l) == ERR)
			printf("<%d> ", taperr);
		else
			printf("%4ld ", l);

		if (GetRate(cvar, &l) == ERR)
			printf("<%d> ", taperr);
		else
			printf("%5ld ", l);

		if (GetSampleOffset(cvar, &l) == ERR)
			printf("<%d> ", taperr);
		else
			printf("%7ld ", l);

		if (GetType(cvar, &cp[0]) == ERR)
			printf("<%d> ", taperr);
		else
			printf(" %s ", cp[0]);

		printf("\n");
		}



	/* Do the rest of the vars, except ASYNC
	 */
	printf(otitle);

	cvar = GetFirst();

	while (cvar = GetNext())
		{
		GetItemType(cvar, &cp[0]);
		if (strcmp(cp[0], "SDI") == 0 || strcmp(cp[0], "HSKP") == 0 ||
		   strcmp(cp[0], "DIGOUT") == 0 || strcmp(cp[0], "ASYNC") == 0)
			continue;

		printf("    %-8s ", cvar);

		if (GetStart(cvar, &l) == ERR)
			printf("  <%d> ", taperr);
		else
			printf("%7ld ", l);

		if (GetLength(cvar, &l) == ERR)
			printf("\t\t<%d>", taperr);
		else
			printf("\t\t%5ld", l);

		printf("\n");
		}


	/* Do ASYNC
	 */
	printf(atitle);

	cvar = GetFirst();

	while (cvar = GetNext())
		{
		GetItemType(cvar, &cp[0]);
		if (strcmp(cp[0], "ASYNC") != 0)
			continue;

		printf("    %-8s ", cvar);

		if (GetLength(cvar, &l) == ERR)
			printf("        <%d>", taperr);
		else
			printf("%        5ld", l);

		printf("\n");
		}

	ReleaseFlightHeader();

	return(0);

}	/* END MAIN */

/* END HDRDUMP.C */

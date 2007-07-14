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

COMPILE:	gcc -I/home/local/include hdrdump.c -o hdrdump -lhdr_api
-------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <raf/ctape.h>


char	buffer[1024];

char *sdititle = "\n Name    Start  Rate  Type  Conv.Fact. Offset   DSM\n";
char *otitle = "\n\n  Block name  Rate  Start byte     Length (bytes)\n";
char *new_otitle = "\n\n  Block name  DSM     Locn     Rate  Start byte     Length (bytes)\n";

char *ptitle = "\n\n  Probe name  Location  LRlength  LRPPR  Resolution\n";
char *new_ptitle = "\n\n  Probe name  DSM      Location  LRlength  LRPPR  Resolution\n";

char *atitle = "\n\nAsync blocks";
	


/* -------------------------------------------------------------------- */
main(int argc, char *argv[])
{
  int	i;
  char	*cvar;		/* Pointer to current variable	*/
  char	*loc;		/* Probe Location		*/
  char	*cp[3];		/* Some generic Char Pointers	*/
  long	l;		/* Long result storage		*/
  short	s;		/* Short result storage		*/
  float	f;		/* Float result storage		*/
  float	*cal;		/* Cal Coeff storage		*/
  float	version;	/* Cal Coeff storage		*/


  if (argc < 2)
    {
    fprintf(stderr, "usage:\thdrdump [prj# | headerfile]\n");
    exit(1);
    }


  /* Deterimine if header filename is a project# or filename
   */
  if (isdigit(argv[1][0]))
    {
    char	*proj_dir;

    if ((proj_dir = getenv("PROJ_DIR")) == NULL)
      proj_dir = "/home/local/proj";

    sprintf(buffer, "%s/%s/header", proj_dir, argv[1]);
    }
  else
    strcpy(buffer, argv[1]);


  if (InitFlightHeader(buffer, CLOSE) == ERR)
    {
    fprintf(stderr, "hdrdump: init error, taperr = %d\n", taperr);
    exit(1);
    }


  GetVersion(&cp[0]);
  version = atof(cp[0]);
  printf("Header Version = %4.1f\n\n", version);

  GetAircraft(&cp[0]);
  GetProjectNumber(&cp[1]);
  GetFlightNumber(&cp[2]);
  printf(" Aircraft: %s, Project: %s, Flight: %s\n", cp[0], cp[1], cp[2]);

  GetHeaderDate(&cp[0]);
  GetHeaderTime(&cp[1]);
  printf(" Date: %s  Time: %s\n", cp[0], cp[1]);

  GetNumberItems(&l);
  printf(" Number of data items: %6ld,", l);
  get_lrlen(&l);
  printf(" Logical record length: %6ld,", l);
  get_lrppr(&l);
  printf(" LR/PR: %6ld\n", l);



  /* Do SDI vars first
   */
  printf(sdititle);

  cvar = GetFirst();

  do
    {
    GetItemType(cvar, &cp[0]);
    if (!(strcmp(cp[0], "SDI") == 0 || strcmp(cp[0], "HSKP") == 0
		   || strcmp(cp[0], "DIGOUT") == 0))
      continue;

    printf("%-8s ", cvar);

    if (GetStart(cvar, &l) == ERR)
      printf("<%d> ", taperr);
    else
      printf("%5ld ", l);

    if (GetRate(cvar, &l) == ERR)
      printf("<%d> ", taperr);
    else
      printf("%4ld  ", l);

    if (GetType(cvar, &cp[0]) == ERR)
      printf("<%d> ", taperr);
    else
      printf("  %s  ", cp[0]);

    if (GetConversionFactor(cvar, &f) == ERR)
      printf("<%d>", taperr);
    else
      printf("%10.6f ", f);

    if (GetConversionOffset(cvar, &l) == ERR)
      printf("<%d>", taperr);
    else
      printf("%6ld     ", l);

    if (GetDSMlocation(cvar, &cp[0]) == ERR)
      printf("<%d>", taperr);
    else
      printf("%-10s", cp[0]);


    printf("\n");
    }
  while (cvar = GetNext());



  /* Do block probes (no async).
   */
  if (version >= 3.0)
    printf(new_otitle);
  else
    printf(otitle);

  cvar = GetFirst();

  do
    {
    GetItemType(cvar, &cp[0]);
    if (strcmp(cp[0], "SDI") == 0 || strcmp(cp[0], "HSKP") == 0 ||
	strcmp(cp[0], "DIGOUT") == 0 || strcmp(cp[0], "ASYNC") == 0 ||
	strcmp(cp[0], "PMS2D") == 0)
      continue;

    printf("    %-8s ", cvar);

    if (version >= 3.0)
      {
      GetDSMlocation(cvar, &cp[0]);
      printf(" %-8s", cp[0]);

      if (GetLocation(cvar, &cp[0]) == ERR)
        printf("  N/A   ", taperr);
      else
        printf("%-8s ", cp[0]);
      }

    if (GetRate(cvar, &l) == ERR)
      printf("  N/A ", taperr);
    else
      printf("%5ld ", l);

    if (GetStart(cvar, &l) == ERR)
      printf("     N/A ", taperr);
    else
      printf("%8ld ", l);

    if (GetLength(cvar, &l) == ERR)
      printf("\t     N/A", taperr);
    else
      printf("\t%8ld", l);

    printf("\n");
    }
  while (cvar = GetNext());


  /* Do PMS2D
   */
  printf(atitle);

  if (version >= 3.0)
    printf(new_ptitle);
  else
    printf(ptitle);

  cvar = GetFirst();

  do
    {
    GetItemType(cvar, &cp[0]);
    if (strcmp(cp[0], "PMS2D") != 0)
      continue;

    printf("    %-8s ", cvar);

    if (version >= 3.0)
      {
      GetDSMlocation(cvar, &cp[0]);
      printf(" %-8s", cp[0]);
      }

    if (GetLocation(cvar, &loc) == ERR)
      printf("   <%d>", taperr);
    else
      printf("   %-8s", loc);

    if (GetLRLength(cvar, &l) == ERR)
      printf("  <%d>", taperr);
    else
      printf("  %5ld", l);

    if (GetLRPPR(cvar, &l) == ERR)
      printf("  <%d>", taperr);
    else
      printf("  %5ld", l);

    if (GetResolution(cvar, &s) == ERR)
      printf("   <%d>", taperr);
    else
      printf("   %5ld", s);

    printf("\n");
    }
  while (cvar = GetNext());


  /* Do ASYNC
   */
  cvar = GetFirst();

  do
    {
    GetItemType(cvar, &cp[0]);
    if (strcmp(cp[0], "ASYNC") != 0)
      continue;

    printf("    %-8s ", cvar);

    if (GetLength(cvar, &l) == ERR)
      printf("        <%d>", taperr);
    else
      printf("        %5ld", l);

    printf("\n");
    }
  while (cvar = GetNext());

  ReleaseFlightHeader();

  return(0);

}	/* END MAIN */

/* END HDRDUMP.C */

/*
-------------------------------------------------------------------------
OBJECT NAME:	textfile.c

FULL NAME:	Read/Free Text File

ENTRY POINTS:	ReadTextFile(filename, list)
		FreeTextFile(list)

DESCRIPTION:	Reads a text file into the user supplied list of pointers.
		This program WILL malloc(3) space.  The text file is read
		in one line to a pointer and the list is NULL terminated.
		Comment lines are removed.  A comment line is one which
		starts with '#'.

INPUT:		Winds filename (see file.h), Pointer to array of pointers.

OUTPUT:		Number Read in, list is NULL terminated.

REFERENCES:	OpenProjectFile()

REFERENCED BY:	hdr_decode.c

EXAMPLE:	char	*masterlist[MAX];

		n = ReadTextFile(MASTERLIST, masterlist);
			.
			.
			.
		FreeTextFile(masterlist);


COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include "nimbus.h"


/* -------------------------------------------------------------------- */
int ReadTextFile(char filename[], char **list)
{
  FILE	*fp;
  int	cnt, len;
  char	buffer[BUFSIZ];

  if ((fp = OpenProjectFile(filename, "r", RETURN)) == NULL)
    {
    sprintf(buffer, filename, ProjectDirectory, ProjectNumber);
    fprintf(stderr, "ReadTextFile: can't open %s\n", buffer);
    *list = NULL;
    return(0);
    }

  for (cnt = 0; fgets(buffer, BUFSIZ, fp); )
    {
    if (buffer[0] == COMMENT)
      continue;

    *list = (char *)GetMemory((len = strlen(buffer)));

    buffer[len-1] = '\0';
    strcpy(*list++, buffer);
    ++cnt;
    }

  *list = NULL;

  fclose(fp);

  return(cnt);

}       /* END READTEXTFILE */

/* -------------------------------------------------------------------- */
void FreeTextFile(char **list)
{
  while (*list)
    free(*list++);

  list[0] = NULL;

}	/* END FREETEXTFILE */

/* END TEXTFILE.C */

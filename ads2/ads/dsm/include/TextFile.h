/*
-------------------------------------------------------------------------
OBJECT NAME:	textfile.c

FULL NAME:	Read/Free Text File

ENTRY POINTS:	ReadTextFile(fileName, list)
		FreeTextFile(list)

DESCRIPTION:	Reads a text file into the user supplied list of pointers.
		This program WILL malloc(3) space.  The text file is read
		in one line to a pointer and the list is NULL terminated.
		Comment lines are removed.  A comment line is one which
		starts with '#'.

INPUT:		fileName, Pointer to array of pointers.

OUTPUT:		Number Read in, list is NULL terminated.

REFERENCES:	OpenProjectFile()

EXAMPLE:	char	*masterlist[MAX];

	n = ReadTextFile(MASTERLIST, masterlist);
		.
		.
		.
	FreeTextFile(masterlist);


COPYRIGHT:	University Corporation for Atmospheric Research, 1996
-------------------------------------------------------------------------
*/

#ifndef TEXTFILE_H
#define TEXTFILE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef VXWORKS
#include <vxWorks.h>
#endif

#define COMMENT		'#'

class TextFile {
public:
  TextFile();
  int	readTextFile(char filename[], char **list);
  void	freeTextFile(char **list);

};

#endif

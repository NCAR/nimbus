
#include "TextFile.h"


/* -------------------------------------------------------------------- */
TextFile::TextFile()
{
}

/* -------------------------------------------------------------------- */
int TextFile::readTextFile(char fileName[], char **list)
{
	FILE	*fp;
	int	cnt, len;
	char	buffer[BUFSIZ];

	if ((fp = fopen(fileName, "r")) == NULL)
		{
		fprintf(stderr, "ReadTextFile: can't open %s\n", fileName);
		*list = (char *)NULL;
		return(0);
		}

	for (cnt = 0; fgets(buffer, BUFSIZ, fp); )
		{
		if (buffer[0] == COMMENT)
			continue;

		len = strlen(buffer);

		if ((*list = (char *)malloc(len)) == NULL)
			fprintf(stderr,"ReadTextFile: out of memory, fatal.\n");

		buffer[len-1] = '\0';
		strcpy(*list++, buffer);
		++cnt;
		}

	*list = (char *)NULL;

	fclose(fp);

	return(cnt);

}       /* END READTEXTFILE */

/* -------------------------------------------------------------------- */
void TextFile::freeTextFile(char **list)
{
	while (*list)
		free(*list++);

}	/* END FREETEXTFILE */

/* END TEXTFILE.C */

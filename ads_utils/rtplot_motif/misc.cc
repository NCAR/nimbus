/*
-------------------------------------------------------------------------
OBJECT NAME:	misc.c

FULL NAME:	Calculate Spacing & Bar Width

DESCRIPTION:	

INPUT:		

OUTPUT:		
-------------------------------------------------------------------------
*/

#include "define.h"
#include <string.h>

/* -------------------------------------------------------------------- */
void MakeTicLabel(char buffer[], double diff, double value)
{
	long	i_diff = (long)diff;

	if (diff < 0.01)
		sprintf(buffer, "%f", value);
	else
	if (diff < 0.1)
		sprintf(buffer, "%.4f", value);
	else
	if (diff < 1.0)
		sprintf(buffer, "%.3f", value);
	else
	if (i_diff % 10 == 0)
		sprintf(buffer, "%ld", (long)value);
	else
	if (diff < 10.0)
		sprintf(buffer, "%.2f", value);
	else
	if (diff < 40.0)
		sprintf(buffer, "%.1f", value);
	else
		sprintf(buffer, "%ld", (long)value);

}	/* MakeTicLabel */

char *GetComputeHost()
{
	return(NULL);
}

/* MISC.C */

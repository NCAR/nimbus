/*
-------------------------------------------------------------------------
OBJECT NAME:	compute.c

FULL NAME:	Compute derived variables

ENTRY POINTS:	ComputeDerived()

DESCRIPTION:	Compute, via AMLIB functions, any new derived variables

INPUT:		NimbusRecord

OUTPUT:		none

REFERENCES:	User AMLIB functions.

REFERENCED BY:	lrloop.c

COPYRIGHT:	University Corporation for Atmospheric Research, 1993
-------------------------------------------------------------------------
*/

#include "define.h"


/* -------------------------------------------------------------------- */
void ComputeDerived()
{
	int	i;

	for (i = 0; i < nVariables; i++)
		if (Variable[i]->Compute && Variable[i]->Output)
			(*Variable[i]->Compute)(Variable[i]);

}	/* END COMPUTEDERIVED */
 
/* END COMPUTE.C */

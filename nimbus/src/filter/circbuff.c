/*
-------------------------------------------------------------------------
OBJECT NAME:	circbuff.c

FULL NAME:	Circular Buffers

ENTRY POINTS:	CreateCircularBuffer()
		AddToCircularBuffer()
		GetBuffer()
		ReleaseCircularBuffer()

DESCRIPTION:	

INPUT:		CircularBuffer *cb (except Create)

OUTPUT:		varies

REFERENCES:	none

REFERENCED BY:	lrloop.c, hrloop.c, despike.c, phase_shift.c

NOTE:		In the interest of performance, GetBuffer is not robust,
		'offset' must be <= 0 and !> -(nbuffers), no checks are
		made to insure such.

COPYRIGHT:	University Corporation for Atmospheric Research, 1992
-------------------------------------------------------------------------
*/

#include <memory.h>
#include "nimbus.h"
#include "circbuff.h"


/* -------------------------------------------------------------------- */
CircularBuffer *CreateCircularBuffer(int nbuffers, int nbytes)
{
	int		i;
	CircularBuffer	*cb;

	cb = (CircularBuffer *)GetMemory(sizeof(CircularBuffer));

	cb->nbuffers	= nbuffers;
	cb->buffsize	= nbytes;
	cb->index	= 0;

	cb->buffer = (char **)GetMemory(nbuffers * sizeof(char *));

	for (i = 0; i < nbuffers; ++i)
		cb->buffer[i] = (char *)GetMemory(nbytes);

	return(cb);

}	/* END CREATECIRCULARBUFFER */

/* -------------------------------------------------------------------- */
char *AddToCircularBuffer(CircularBuffer *cb)
{
	char	*buff = cb->buffer[cb->index];

	if (++cb->index >= cb->nbuffers)
		cb->index = 0;

	return(buff);

}	/* END ADDTOCIRCULARBUFFER */

/* -------------------------------------------------------------------- */
char *GetBuffer(CircularBuffer *cb, int offset)
{
	if (offset > 0)
		{
		fprintf(stderr, "GetBuffer: offset = %d\n", offset);
		return(NULL);
		}

	offset += cb->index;

	if (offset < 0)
		offset += cb->nbuffers;

	return(cb->buffer[offset]);

}	/* END GETBUFFER */

/* -------------------------------------------------------------------- */
void ReleaseCircularBuffer(CircularBuffer *cb)
{
	int	i;

	for (i = 0; i < cb->nbuffers; ++i)
		free(cb->buffer[i]);

	free(cb->buffer);
	free(cb);

}	/* END RELEASECIRCULARBUFFER */

/* END CIRCBUFF.C */

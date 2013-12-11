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
  CircularBuffer	*cb;

  cb = new CircularBuffer;

  cb->nbuffers	= nbuffers;
  cb->buffsize	= nbytes;
  cb->index	= 0;

  cb->buffer = new char*[nbuffers];

  for (int i = 0; i < nbuffers; ++i)
  {
    cb->buffer[i] = new char[nbytes];
    // Some of the algorithms (eg, Despike) expect the preceding buffers to
    // be valid even when only one buffer has been read so far.  So the
    // best work-around I can think for this is to initialize the buffer
    // with NaNs.  To see the code which assumes valid preceding buffers,
    // remove these lines and run nimbus through valgrind.
    for (unsigned int j = 0; j < nbytes/sizeof(NR_TYPE); ++j)
    {
      ((NR_TYPE*)(cb->buffer[i]))[j] = floatNAN;
    }
  }

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
  for (int i = 0; i < cb->nbuffers; ++i)
    delete [] cb->buffer[i];

  delete [] cb->buffer;
  delete cb;

}	/* END RELEASECIRCULARBUFFER */

/* END CIRCBUFF.C */

/*
-------------------------------------------------------------------------
OBJECT NAME:	circbuff.h

FULL NAME:	Circular Buffer Header File
-------------------------------------------------------------------------
*/

#ifndef CIRCBUFF_H
#define CIRCBUFF_H

typedef struct
	{
	int	nbuffers;	/* Number of buffers to create		*/
	int	buffsize;	/* Size of each buffer in bytes		*/
	int	index;		/* Current Pointer into buffers		*/
	char	**buffer;	/* Pointers to buffers			*/
	} CircularBuffer;


CircularBuffer	*CreateCircularBuffer(int nbuffers, int nbytes);
void		ReleaseCircularBuffer(CircularBuffer *cb);
char		*GetBuffer(CircularBuffer *cb, int offset),
		*AddToCircularBuffer(CircularBuffer *cb);

#endif

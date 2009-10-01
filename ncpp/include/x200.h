/*
-------------------------------------------------------------------------
OBJECT NAME:	x200.h

FULL NAME:	200X

TYPE:		Derived class

COPYRIGHT:	University Corporation for Atmospheric Research, 1999
-------------------------------------------------------------------------
*/

#ifndef X200_H
#define X200_H

#include "Probe200.h"


/* -------------------------------------------------------------------- */
class X200 : public Probe200
{
public:
	X200(NcFile *file, NcVar *av);

private:

};	// END X200.H

#endif

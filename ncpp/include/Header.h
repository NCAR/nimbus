/*
-------------------------------------------------------------------------
OBJECT NAME:	Header.h

FULL NAME:	View NetCDF Header

TYPE:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1997
-------------------------------------------------------------------------
*/

#ifndef HEADER_H
#define HEADER_H

#include "define.h"

#include <raf/TextWindow.h>


/* -------------------------------------------------------------------- */
class Header : public TextWindow
{
public:

	Header(const Widget parent);
  void	Update(const std::string fileName);

private:

};	// END HEADER.H

#endif

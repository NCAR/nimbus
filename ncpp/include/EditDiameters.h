/*
-------------------------------------------------------------------------
OBJECT NAME:	EditDiameters.h

FULL NAME:	Create Basic Edit Parameters window.

TYPE:		Base class

COPYRIGHT:	University Corporation for Atmospheric Research, 1998-9
-------------------------------------------------------------------------
*/

#ifndef EDITDIAM_H
#define EDITDIAM_H

#include "define.h"

#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/TextF.h>
#include "FileMgr.h"
#include <raf/Window.h>


/* -------------------------------------------------------------------- */
class EditDiameters : public WinForm
{
public:
	EditDiameters(const Widget parent);

  void	SetProbeNames(DataFile *file);
  void	SetDiameters(Probe *probe);
  void	ApplyDiameters(Probe *probe);

private:
  Widget	fileLabel, probeLabel, probeB[MAX_PROBES];
  Widget	miscLabel[12], text[12];
  int		txtCnt;

  void		setSpex(const char *, int, float);

};	// END EDITDIAMETERS.H

#endif

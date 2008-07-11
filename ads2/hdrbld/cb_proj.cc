#include "hdrbld.h"
#include <raf/pms.h>
#include <raf/vardb.h>
#include <unistd.h>

#include <Xm/TextF.h>

/* -------------------------------------------------------------------- */
void SetProject(Widget w, XtPointer client, XtPointer call)
{
  char		*p;
  int		pn;

  p = XmTextFieldGetString(projText[0]);
  pn = atoi(p);
  XtFree(p);

  if (pn == 0)
    ProjectNumber = "defaults";

  sprintf(flightInfo.prnum, "%d", pn);
  XmTextFieldSetString(projText[0], flightInfo.prnum);
  ProjectNumber = flightInfo.prnum;

printf("SetProject: new proj# = %s\n", flightInfo.prnum);

  ReleaseVarDB();
  ReleasePMSspecs();

  MakeProjectFileName(buffer, "%s/%s/%s");

  if (access(buffer, R_OK) == (-1))
    {
    fprintf(stderr, "Can't open %s for reading, does project exist?\n", buffer);
    return;
    }
printf(">>>>SetProj needs work still.\n");
//return;
  MakeProjectFileName(buffer, "%s/%s/%s/PMSspecs");
  InitPMSspecs(buffer);

  MakeProjectFileName(buffer, "%s/%s/%s/VarDB");
  InitializeVarDB(buffer);

}

/* END CB_PROJ.CC */

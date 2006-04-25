/*
-------------------------------------------------------------------------
OBJECT NAME:	projFile.cc

FULL NAME:	Edit Project File Window Callbacks

ENTRY POINTS:	CreateEditWindow()
		EditDefaultFile()
		EditProjectFile()
		EditHostsFile()
		CreateProject()
		DismissEditWindow()

STATIC FNS:	PrintProjectFile()
		SaveProjectFile()

DESCRIPTION:	

REFERENCES:	

REFERENCED BY:	

NOTES:		

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-2004
-------------------------------------------------------------------------
*/

#include "hdrbld.h"
#include "hardwire.h"
#include "ac.h"
#include <unistd.h>

#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Text.h>

static char	currentProjFile[512];
static Widget	EditWindow, projFileText;

void DismissEditWindow(Widget w, XtPointer client, XtPointer call);
static void EditProjFile();
static void PrintProjectFile(Widget w, XtPointer client, XtPointer call);
static void SaveProjectFile(Widget w, XtPointer client, XtPointer call);

extern Widget	Shell001;

/* -------------------------------------------------------------------- */
int MakeProjectFileName(char file[], const char format[])
{
  char platform[128];
  file[0] = platform[0] = '\0';

  if (strlen(flightInfo.acraft) == 0 || ProjectNumber == 0)
  {
    ShowError("No valid project opened.");
    return ERR;
  }

  GetAircraftFullyQualifiedName(flightInfo.acraft, platform);

  if (strlen(platform) == 0)
  {
    ShowError("No valid project opened.");
    return ERR;
  }

  (void)sprintf(file, format, ProjectDirectory, ProjectNumber, platform);
  return OK;
}

/* -------------------------------------------------------------------- */
void CreateEditWindow(Widget parent)
{
  Arg		args[32];
  Cardinal	n;
  Widget	drFrame, drRC, b[3];

  n = 0;
  EditWindow = XmCreateForm(parent, "topLevelForm", args, n);


  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  drFrame = XmCreateFrame(EditWindow, "buttonFrame", args, n);
  XtManageChild(drFrame);
 
  n = 0;
  drRC = XmCreateRowColumn(drFrame, "buttonRC", args, n);
  XtManageChild(drRC);
 
 
  n = 0;
  b[0] = XmCreatePushButton(drRC, "saveButton", args, n);
  XtAddCallback(b[0], XmNactivateCallback, SaveProjectFile, NULL);
 
  n = 0;
  b[1] = XmCreatePushButton(drRC, "printButton", args, n);
  XtAddCallback(b[1], XmNactivateCallback, PrintProjectFile, NULL);
 
  n = 0;
  b[2] = XmCreatePushButton(drRC, "dismissButton", args, n);
  XtAddCallback(b[2], XmNactivateCallback, DismissEditWindow, NULL);
  XtManageChildren(b, 3);
 

  n = 0;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, drFrame); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  projFileText = XmCreateScrolledText(EditWindow, "projFileText", args, n);
  XtManageChild(projFileText);

}	/* END CREATEDITWINDOW */

/* -------------------------------------------------------------------- */
void EditProjectFile(Widget w, XtPointer client, XtPointer call)
{
  if (strcmp((char *)client, "netconfig") == 0)
    sprintf(buffer, "%s/%s", ProjectDirectory, (char *)client);
  else
  {
    char tmp[256];
    strcpy(tmp, "%s/%s/%s/");
    strcat(tmp, (char*)client);
    MakeProjectFileName(buffer, tmp);
  }

  EditProjFile();
}

/* -------------------------------------------------------------------- */
void EditDefaultFile(Widget w, XtPointer client, XtPointer call)
{
  sprintf(buffer, "%s/Configuration/raf//%s", ProjectDirectory, (char *)client);

  EditProjFile();
}

/* -------------------------------------------------------------------- */
void EditHostsFile(Widget w, XtPointer client, XtPointer call)
{
return;
//  sprintf(buffer, "%s/hosts/%s/%s", ProjectDirectory, host, (char *)client);

  EditProjFile();
}

/* -------------------------------------------------------------------- */
void EditProjFile()
{
  FILE	*fp;
  char	*p;
  int		len;
  Arg		args[2];
  Cardinal	n;

  if (strlen(buffer) == 0)
    return;

  if ((fp = fopen(buffer, "r")) == NULL)
  {
    sprintf(&buffer[2000], "Can't open %s for reading.", buffer);
    ShowError(&buffer[2000]);
    return;
  }

  strcpy(currentProjFile, buffer);

  n = 0;
  XtSetArg(args[n], XmNtitle, currentProjFile); ++n;
  XtSetValues(Shell001, args, n);


  fseek(fp, 0L, SEEK_END);
  len = ftell(fp);
  p = (char *)GetMemory(len+3);
  rewind(fp);
  fread(p, len, 1, fp);
  fclose(fp);

  p[len] = '\0';

  XtManageChild(EditWindow);
  XtPopup(XtParent(EditWindow), XtGrabNone);

  XmTextSetString(projFileText, p);
  free(p);

}


/* -------------------------------------------------------------------- */
static void PrintProjectFile(Widget w, XtPointer client, XtPointer call)
{
  FILE	*fp;
  char	*p;

#ifdef SVR4
  if ((fp = popen("lp -o nobanner", "w")) == NULL)
#else
  if ((fp = popen("lpr -H", "w")) == NULL)
#endif
    {
    ShowError("PrintProjectFile: can't open pipe to 'lp(r)'");
    return;
    }


  p = XmTextGetString(projFileText);

  fwrite(p, strlen(p), 1, fp);

  pclose(fp);
  XtFree(p);

}	/* END PRINTPROJECTFILE */

/* -------------------------------------------------------------------- */
static void SaveProjectFile(Widget w, XtPointer client, XtPointer call)
{
  FILE	*fp;
  char	*p;

  if ((fp = fopen(currentProjFile, "w+")) == NULL)
    {
    sprintf(buffer, "Can't open %s for writing.", currentProjFile);
    ShowError(buffer);
    return;
    }

  p = XmTextGetString(projFileText);

  fwrite(p, strlen(p), 1, fp);

  fclose(fp);
  XtFree(p);

}	/* END SAVEPROJECTFILE */

/* -------------------------------------------------------------------- */
void DismissEditWindow(Widget w, XtPointer client, XtPointer call)
{
  XtPopdown(XtParent(EditWindow));
  XtUnmanageChild(EditWindow);

}

/* -------------------------------------------------------------------- */
void CreateProject_OK(Widget w, XtPointer client, XtPointer call)
{
  forkXterm(NULL, (XtPointer)"CreateProject", NULL);

}

/* -------------------------------------------------------------------- */
void CreateProject2(Widget w, XtPointer client, XtPointer call)
{
  char	temp[512];

  QueryCancel(NULL, NULL, NULL);
  ExtractAnswer(buffer);
  sprintf(temp, "%s/%s", ProjectDirectory, buffer);

  if (strlen(buffer) == 0)
    return;

  ProjectNumber = (char *)GetMemory(strlen(buffer)+1);
  strcpy(ProjectNumber, buffer);

  if (access(temp, R_OK) == 0)
    WarnUser("Project directory exists, proceed?", CreateProject_OK, NULL);
  else
    CreateProject_OK(NULL, NULL, NULL);

}

/* -------------------------------------------------------------------- */
void CreateProject(Widget w, XtPointer client, XtPointer call)
{
  /* Check to see if user is 'nimbus'.
   */
  if (getuid() != 20000)
    ShowError("Must be logged in as 'nimbus' to create a project.");
  else
    QueryUser("Enter Project Number:", 12, CreateProject2);

}

/* END PROJFILE.CC */

/*
-------------------------------------------------------------------------
OBJECT NAME:	ccb.c

FULL NAME:	Command Callbacks

ENTRY POINTS:	Quit()
		OpenNewFile()
		SaveFile()

STATIC FNS:	none

DESCRIPTION:	

INPUT:		

OUTPUT:		none

REFERENCES:	none

REFERENCED BY:	XtAppMainLoop()

COPYRIGHT:	University Corporation for Atmospheric Research, 2000
-------------------------------------------------------------------------
*/

#include <Xm/RowColumn.h>
#include <Xm/TextF.h>

#include "define.h"
#include <errno.h>

static int	ChangesMade = FALSE, currentCategory = 0;

extern Widget	EFtext[];
extern char	buffer[], FileName[], *DataDirectory;
extern Fl	flInfo;

void	exit();
double	atof();

/* -------------------------------------------------------------------- */
void Quit(Widget w, XtPointer client, XtPointer call)
{
  if (ChangesMade)
    {
    WarnUser("You have not saved this file.", exit);
    return;
    }

  exit(0);

}	/* END QUIT */

/* -------------------------------------------------------------------- */
void OpenNewFile(Widget w, XtPointer client, XtPointer call)
{
  sprintf(buffer, "%s/*", DataDirectory);
  QueryFile("Enter file name to load:", buffer, OpenNewFile_OK);

}	/* END OPENNEWFILE */

/* -------------------------------------------------------------------- */
void OpenNewFile_OK(Widget w, XtPointer client, XmFileSelectionBoxCallbackStruct *call)
{
  char	*file;
  FILE	*fp;

  if (w)
    {
    ExtractFileName(call->value, &file);
    FileCancel((Widget)NULL, (XtPointer)NULL, (XtPointer)NULL);

    strcpy(FileName, file);
    }


  if ((fp = fopen(FileName, "rb+")) == NULL)
    {
    fprintf(stderr, "Failed to open file for output.\n");
    }
  else
    {
    fseek(fp, 20, SEEK_SET);
    fread((char *)&flInfo, sizeof(Fl), 1, fp);
    fclose(fp);

    if (strcmp(flInfo.thdr, "THDR"))
      {
      fprintf(stderr, "Not a valid ADS file.\n");
      return;
      }

    XmTextFieldSetString(EFtext[0], flInfo.prnum);
    XmTextFieldSetString(EFtext[1], flInfo.fltnum);
    XmTextFieldSetString(EFtext[2], flInfo.tpnum);
    XmTextFieldSetString(EFtext[3], flInfo.date);
    XmTextFieldSetString(EFtext[4], flInfo.time);
    XmTextFieldSetString(EFtext[5], flInfo.acraft);
    }

}	/* END OPENNEWFILE_OK */

/* -------------------------------------------------------------------- */
void SaveFile(Widget w, XtPointer client, XtPointer call)
{
  FILE	*fp;

  if ((fp = fopen(FileName, "rb+")) == NULL)
    {
    if (errno == EACCES)
      fprintf(stderr, "Write permission denied.\n");

    fprintf(stderr, "Failed to open file for output.\n");
    }
  else
    {
    char	*p;

    p = XmTextFieldGetString(EFtext[0]);
    strcpy(flInfo.prnum, p);
    XtFree(p);

    p = XmTextFieldGetString(EFtext[1]);
    strcpy(flInfo.fltnum, p);
    XtFree(p);

    p = XmTextFieldGetString(EFtext[2]);
    strcpy(flInfo.tpnum, p);
    XtFree(p);

    p = XmTextFieldGetString(EFtext[3]);
    strcpy(flInfo.date, p);
    XtFree(p);

    p = XmTextFieldGetString(EFtext[4]);
    strcpy(flInfo.time, p);
    XtFree(p);

    p = XmTextFieldGetString(EFtext[5]);
    strcpy(flInfo.acraft, p);
    XtFree(p);


    fseek(fp, 20, SEEK_SET);
    fwrite((char *)&flInfo, sizeof(Fl), 1, fp);
    fclose(fp);
    }

}	/* END SAVEFILE */

/* END CCB.C */

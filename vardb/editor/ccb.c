/*
-------------------------------------------------------------------------
OBJECT NAME:	ccb.c

FULL NAME:	Command Callbacks

ENTRY POINTS:	Quit()
		OpenNewFile()
		SaveFile()
		SaveFileAs()
		EditVariable()
		Accept()
		Delete()
		SetCategory()

STATIC FNS:	none

DESCRIPTION:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1993-2007
-------------------------------------------------------------------------
*/

#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <netinet/in.h> // htonl macros.

#include <Xm/List.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>

#include "define.h"
#include <raf/portable.h>
#include <raf/vardb.h>

static int	ChangesMade = FALSE, currentCategory = 0, currentStdName = 0;

extern Widget	catXx, stdNameXx, catMenu, stdNameMenu, list, referenceButton,
		EFtext[], fixedButton, floatButton;
extern char	buffer[], FileName[], *catList[], *ProjectDirectory,
		*stdNameList[];
extern long	VarDB_nRecords, VarDB_RecLength;

extern "C" {
char *strupr(char *);
void ShowError(char msg[]), FileCancel(Widget, XtPointer, XtPointer);
void QueryFile(char *, char *, XtCallbackProc), ExtractFileName(XmString, char **);
void WarnUser(char msg[], XtCallbackProc, XtCallbackProc);
}

/* -------------------------------------------------------------------- */
void Quit(Widget w, XtPointer client, XtPointer call)
{
  if (ChangesMade)
    {
    WarnUser("You have not saved this file.", (XtCallbackProc)exit, NULL);
    return;
    }

  exit(0);

}	/* END QUIT */

/* -------------------------------------------------------------------- */
void SetCategory(Widget w, XtPointer client, XtPointer call)
{
  Arg		args[2];
  XmString	name;

  currentCategory = (long)client;

  if (!w)	/* If this is being called from EditVariable()	*/
    {
    name = XmStringCreateLocalized(catList[currentCategory]);
    XtSetArg(args[0], XmNlabelString, name);
    XtSetValues(XmOptionButtonGadget(catXx), args, 1);
    XmStringFree(name);
    }

}	/* END SETCATEGORY */

/* -------------------------------------------------------------------- */
void SetStandardName(Widget w, XtPointer client, XtPointer call)
{
  Arg		args[2];
  XmString	name;

  currentStdName = (long)client;

  if (!w)	/* If this is being called from EditVariable()	*/
    {
    name = XmStringCreateLocalized(stdNameList[currentStdName]);
    XtSetArg(args[0], XmNlabelString, name);
    XtSetValues(XmOptionButtonGadget(stdNameXx), args, 1);
    XmStringFree(name);
    }

}	/* END SETCATEGORY */

/* -------------------------------------------------------------------- */
void OpenNewFile_OK(Widget w, XtPointer client, XmFileSelectionBoxCallbackStruct *call)
{
  int		i, n;
  char		*file;
  Arg		args[8];
  XmString	name;

  static Widget	b[64];


  if (w)
    {
    ExtractFileName(call->value, &file);
    FileCancel((Widget)NULL, (XtPointer)NULL, (XtPointer)NULL);

    strcpy(FileName, file);
    }


  /* Init new VarDB and generate list items.
   */
  if (InitializeVarDB(FileName) == ERR)
    {
    fprintf(stderr, "Can't initialize variable database %s.", FileName);
    exit(1);
    }

  SortVarDB();

  for (i = 0; i < VarDB_nRecords; ++i)
    {
    name = XmStringCreateLocalized(((struct var_v2 *)VarDB)[i].Name);
    XmListAddItem(list, name, 0);
    XmStringFree(name);
    }


  /* Remove old categories, and new ones.
   */
  for (i = 0; catList[i]; ++i)
    {
    XtUnmanageChild(b[i]);
    XtDestroyWidget(b[i]);
    }

  ReadCategories();
  VarDB_GetCategoryList(catList);

  for (i = 0; catList[i]; ++i)
    {
    name = XmStringCreateLocalized(catList[i]);

    n = 0;
    XtSetArg(args[n], XmNlabelString, name); ++n;
    b[i] = XmCreatePushButton(catMenu, "opMenB", args, n);
    XtAddCallback(b[i], XmNactivateCallback, SetCategory, (XtPointer)i);

    XmStringFree(name);
    }

  XtManageChildren(b, i);

  ReadStandardNames();
  VarDB_GetStandardNameList(stdNameList);

  for (i = 0; stdNameList[i]; ++i)
    {
    name = XmStringCreateLocalized(stdNameList[i]);

    n = 0;
    XtSetArg(args[n], XmNlabelString, name); ++n;
    b[i] = XmCreatePushButton(stdNameMenu, "opMenB", args, n);
    XtAddCallback(b[i], XmNactivateCallback, SetStandardName, (XtPointer)i);

    XmStringFree(name);
    }

  XtManageChildren(b, i);

  ChangesMade = FALSE;
  currentCategory = 0;
  currentStdName = 0;

}	/* END OPENNEWFILE_OK */

/* -------------------------------------------------------------------- */
void OpenNewFile(Widget w, XtPointer client, XtPointer call)
{
  sprintf(buffer, "%s/*", ProjectDirectory);
  QueryFile("Enter file name to load:", buffer, (XtCallbackProc)OpenNewFile_OK);

}	/* END OPENNEWFILE */

/* -------------------------------------------------------------------- */
void SaveFileAs_OK(Widget w, XtPointer client, XmFileSelectionBoxCallbackStruct *call)
{
  char	*file;

  ExtractFileName(call->value, &file);
  FileCancel((Widget)NULL, (XtPointer)NULL, (XtPointer)NULL);

  if (SaveVarDB(file) == ERR)
    ShowError("Error trying to save, aborted.");
  else
    ChangesMade = FALSE;

}	/* END SAVEFILEAS_OK */

/* -------------------------------------------------------------------- */
void SaveFileAs(Widget w, XtPointer client, XtPointer call)
{
  strcpy(buffer, FileName);
  QueryFile("Save as:", buffer, (XtCallbackProc)SaveFileAs_OK);

}	/* END SAVEFILEAS */

/* -------------------------------------------------------------------- */
void SaveFile(Widget w, XtPointer client, XtPointer call)
{
  if (SaveVarDB(FileName) == ERR)
    ShowError("Error trying to save, aborted.");
  else
    ChangesMade = FALSE;

}	/* END SAVEFILE */

/* -------------------------------------------------------------------- */
void EditVariable(Widget w, XtPointer client, XmListCallbackStruct *call)
{
  int		pos, *pos_list, pcnt;
  float		fl;
  static char	*format = "%.6f";

  if (XmListGetSelectedPos(list, &pos_list, &pcnt) == FALSE)
    return;

  pos = pos_list[0] - 1;

  if (pos > VarDB_nRecords)
    return;


  XmTextFieldSetString(EFtext[0], ((struct var_v2 *)VarDB)[pos].Name);
  XmTextFieldSetString(EFtext[1], ((struct var_v2 *)VarDB)[pos].Title);
  XmTextFieldSetString(EFtext[2], ((struct var_v2 *)VarDB)[pos].Units);
  XmTextFieldSetString(EFtext[3], ((struct var_v2 *)VarDB)[pos].AlternateUnits);

  fl = ntohf(((struct var_v2 *)VarDB)[pos].FixedRange[0]);
  sprintf(buffer, format, fl);
  XmTextFieldSetString(EFtext[4], buffer);

  fl = ntohf(((struct var_v2 *)VarDB)[pos].FixedRange[1]);
  sprintf(buffer, format, fl);
  XmTextFieldSetString(EFtext[5], buffer);

  fl = ntohf(((struct var_v2 *)VarDB)[pos].FloatRange);
  sprintf(buffer, format, fl);
  XmTextFieldSetString(EFtext[6], buffer);

  fl = ntohf(((struct var_v2 *)VarDB)[pos].MinLimit);
  sprintf(buffer, format, fl);
  XmTextFieldSetString(EFtext[7], buffer);

  fl = ntohf(((struct var_v2 *)VarDB)[pos].MaxLimit);
  sprintf(buffer, format, fl);
  XmTextFieldSetString(EFtext[8], buffer);

  fl = ntohf(((struct var_v2 *)VarDB)[pos].CalRange[0]);
  sprintf(buffer, format, fl);
  XmTextFieldSetString(EFtext[9], buffer);

  fl = ntohf(((struct var_v2 *)VarDB)[pos].CalRange[1]);
  sprintf(buffer, format, fl);
  XmTextFieldSetString(EFtext[10], buffer);

  switch (ntohl(((struct var_v2 *)VarDB)[pos].type))
    {
    case FIXED:
      XmToggleButtonSetState(fixedButton, TRUE, FALSE);
      XmToggleButtonSetState(floatButton, FALSE, FALSE);
      break;

    case FLOATING:
      XmToggleButtonSetState(fixedButton, FALSE, FALSE);
      XmToggleButtonSetState(floatButton, TRUE, FALSE);
      break;

    default:
      fprintf(stderr, "Impossible type.\n");
    }

  SetCategory(NULL, (XtPointer)ntohl(((struct var_v2 *)VarDB)[pos].Category), NULL);
  SetStandardName(NULL, (XtPointer)ntohl(((struct var_v2 *)VarDB)[pos].standard_name), NULL);

  XmToggleButtonSetState(referenceButton,
		ntohl(((struct var_v2 *)VarDB)[pos].reference), False);

}	/* END EDITVARIABLE */

/* -------------------------------------------------------------------- */
void Accept(Widget w, XtPointer client, XtPointer call)
{
  char		*p;
  XmString	name[1000];
  int		i, pos, *pos_list, pcnt, firstVisPos;
  Arg		args[5];
  float		f;


  p = XmTextFieldGetString(EFtext[0]);
  strupr(p);
  XmTextFieldSetString(EFtext[0], p);
  name[0] = XmStringCreateLocalized(p);

  i = 0;
  XtSetArg(args[i], XmNtopItemPosition, &firstVisPos); ++i;
  XtGetValues(list, args, i);

  if (XmListGetMatchPos(list, name[0], &pos_list, &pcnt))
    pos = pos_list[0] - 1;
  else
    {
    void	*tmp;

    if ((tmp = (void *)realloc(VarDB, (unsigned)(VarDB_nRecords+1) *
				(unsigned)VarDB_RecLength)) == NULL)
      {
      ShowError("Memory allocation error, failed to ADD variable.");
      return;
      }

    VarDB = tmp;

    for (pos = VarDB_nRecords-1; pos >= 0; --pos)
      if (strcmp(p, ((struct var_v2 *)VarDB)[pos].Name) < 0)
        memcpy(	&((struct var_v2 *)VarDB)[pos+1],
		&((struct var_v2 *)VarDB)[pos],
		sizeof(struct var_v2));
      else
        break;

    ++pos;
    ++VarDB_nRecords;
    }

  XmStringFree(name[0]);

  strcpy(((struct var_v2 *)VarDB)[pos].Name, p);
  XtFree(p);

  p = XmTextFieldGetString(EFtext[1]);
  strcpy(((struct var_v2 *)VarDB)[pos].Title, p);
  XtFree(p);

  p = XmTextFieldGetString(EFtext[2]);
  strcpy(((struct var_v2 *)VarDB)[pos].Units, p);
  XtFree(p);

  p = XmTextFieldGetString(EFtext[3]);
  strcpy(((struct var_v2 *)VarDB)[pos].AlternateUnits, p);
  XtFree(p);


  f = atof(p = XmTextFieldGetString(EFtext[4]));
  (((struct var_v2 *)VarDB)[pos].FixedRange[0]) = htonf(f);
  XtFree(p);

  f = atof(p = XmTextFieldGetString(EFtext[5]));
  (((struct var_v2 *)VarDB)[pos].FixedRange[1]) = htonf(f);
  XtFree(p);

  f = atof(p = XmTextFieldGetString(EFtext[6]));
  (((struct var_v2 *)VarDB)[pos].FloatRange) = htonf(f);
  XtFree(p);

  f = atof(p = XmTextFieldGetString(EFtext[7]));
  (((struct var_v2 *)VarDB)[pos].MinLimit) = htonf(f);
  XtFree(p);

  f = atof(p = XmTextFieldGetString(EFtext[8]));
  (((struct var_v2 *)VarDB)[pos].MaxLimit) = htonf(f);
  XtFree(p);

  f = atof(p = XmTextFieldGetString(EFtext[9]));
  (((struct var_v2 *)VarDB)[pos].CalRange[0]) = htonf(f);
  XtFree(p);

  f = atof(p = XmTextFieldGetString(EFtext[10]));
  (((struct var_v2 *)VarDB)[pos].CalRange[1]) = htonf(f);
  XtFree(p);

  if (XmToggleButtonGetState(fixedButton) == TRUE)
    ((struct var_v2 *)VarDB)[pos].type = htonl(FIXED);
  else
    ((struct var_v2 *)VarDB)[pos].type = htonl(FLOATING);

  XmListDeleteAllItems(list);

  for (i = 0; i < VarDB_nRecords; ++i)
    name[i] = XmStringCreateLocalized(((struct var_v2 *)VarDB)[i].Name);


  XmListAddItems(list, name, VarDB_nRecords, 0);

  for (i = 0; i < VarDB_nRecords; ++i)
    XmStringFree(name[i]);

  XmListSetPos(list, firstVisPos);
  XmListSelectPos(list, pos+1, FALSE);

  ((struct var_v2 *)VarDB)[pos].Category = htonl(currentCategory);
  ((struct var_v2 *)VarDB)[pos].standard_name = htonl(currentStdName);
  ((struct var_v2 *)VarDB)[pos].reference =
			htonl(XmToggleButtonGetState(referenceButton));

  ChangesMade = TRUE;

}	/* END ACCEPT */

/* -------------------------------------------------------------------- */
void Clear(Widget w, XtPointer client, XtPointer call)
{
  int	i;

  for (i = 0; i < 11; ++i)
    XmTextFieldSetString(EFtext[i], "");

  SetCategory(NULL, 0, NULL);
  SetStandardName(NULL, 0, NULL);
  XmToggleButtonSetState(referenceButton, False, False);

}	/* END CLEAR */

/* -------------------------------------------------------------------- */
void Delete(Widget w, XtPointer client, XtPointer call)
{
  int	i, pos, *pos_list, pcnt;

  if (XmListGetSelectedPos(list, &pos_list, &pcnt) == FALSE)
    {
    ShowError("No variable selected to delete.");
    return;
    }


  pos = pos_list[0];

  for (i = pos; i < VarDB_nRecords; ++i)
    memcpy(	(char *)&((struct var_v2 *)VarDB)[i-1],
		(char *)&((struct var_v2 *)VarDB)[i],
		sizeof(struct var_v2));

  --VarDB_nRecords;

  XmListDeletePos(list, pos_list[0]);

  ChangesMade = TRUE;
		
}	/* END DELETE */

/* END CCB.C */

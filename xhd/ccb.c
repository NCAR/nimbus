/*
-------------------------------------------------------------------------
OBJECT NAME:	xhd.c

FULL NAME:	Callbacks

ENTRY POINTS:	Quit()
		Save()
		Print()
		HeaderDump()
		DumpPMS()
		LoadFile()

STATIC FNS:	StartUp()

DESCRIPTION:	

REFERENCES:	

REFERENCED BY:	

COPYRIGHT:	University Corporation for Atmospheric Research, 1996-2001
-------------------------------------------------------------------------
*/

#include "constants.h"
#include "ctape.h"

#include <stdio.h>
#include <stdlib.h>

#include <Xm/Text.h>


static char    *sdiTitle =
        "\n Name    DSM    Type Rate Addr Gain Offset Start Leaf     A/D Cal            Cals\n";
 
static char    *pinTitle =
        "\n Name    DSM    Type Rate Addr Gain Offset  Connector     Pinout\n";

static char    *blockTitle =
        "\n\n Block name  DSM      Start byte   Length (bytes)\n";
 
static char    *pms1dTitle =
        "\n PMS1D probe  DSM      Rate   Start   Length  Interface  Channel   SerialNumber   nChannels\n";
 
static char    *pms2dTitle =
        "\n PMS2D probe  DSM      Location  LRlength  LRPPR  Resolution\n";
 
static char    *asyncTitle =
        "\n Async blocks DSM      LRlength  LRPPR\n";


char	*pin[] =
	{
	"A-,B+,C sh",
	"D-,E+,F sh",
	"G-,H+,J sh",
	"K-,L+,M sh",
	"N-,P+,R sh",
	"S-,T+,U sh",
	"V-,W+,X sh",
	"Y-,Z+,a sh",
	};

extern char	fileName[], buffer[];
extern Widget	hdText, HDwindow;
void	*GetMemory(size_t);

void Save_OK();
static void StartUp();
void    FileCancel(Widget w, XtPointer clientData, XtPointer callData),
        ShowError(char str[]),
        QueryFile(char *prompt, char *directory, XtCallbackProc),
        ExtractFileName(XmString str, char **text);

/* -------------------------------------------------------------------- */
void Save(Widget w, XtPointer client, XtPointer call)
{
  sprintf(buffer, "%s/*", getenv("HOME"));
  QueryFile("Enter file name to save:", buffer, Save_OK);

}	/* END SAVE */

/* -------------------------------------------------------------------- */
void Save_OK(Widget w, XtPointer client, XmFileSelectionBoxCallbackStruct *call)
{
  FILE    *fp;
  char    *p, *file;

  ExtractFileName(call->value, &file);
  FileCancel((Widget)NULL, (XtPointer)NULL, (XtPointer)NULL);

  if ((fp = fopen(file, "w")) == NULL)
    {
    sprintf(buffer, "Save: can't open %s.", file);
    ShowError(buffer);
    return;
    }

  p = XmTextGetString(hdText);

  fwrite(p, strlen(p), 1, fp);

  fclose(fp);
  XtFree(p);

}	/* END SAVE_OK */

/* -------------------------------------------------------------------- */
void Print(Widget w, XtPointer client, XtPointer call)
{
  FILE    *fp;
  char    *p;

#ifdef SVR4
  if ((p = getenv("LPDEST")) != NULL)
#else
  if ((p = getenv("PRINTER")) != NULL)
#endif
    printf("Output being sent to %s.\n", p);

#ifdef SVR4
  if ((fp = popen("lp -o nobanner", "w")) == NULL)
#else
  if ((fp = popen("lpr -h", "w")) == NULL)
#endif
    {
    ShowError("Print: can't open pipe to 'lp(r)'");
    return;
    }

  p = XmTextGetString(hdText);

  fwrite(p, strlen(p), 1, fp);

  pclose(fp);
  XtFree(p);

}	/* END PRINT */

/* -------------------------------------------------------------------- */
void Quit(Widget w, XtPointer client, XtPointer call)
{
  exit(0);

}	/* END QUIT */

/* -------------------------------------------------------------------- */
void HeaderDump(Widget w, XtPointer client, XtPointer call)
{
  int	i;
  char	*cvar;		/* Pointer to current variable	*/
  char	*cp[4];		/* Some generic Char Pointers	*/
  long	l[8];		/* Long result storage			*/
  short	s;		/* Short result storage			*/
  float	f, *fp;		/* Float result storage			*/
  bool	pinOut = FALSE;
  bool	pms1d = FALSE, pms2d = FALSE, async = FALSE;

  if (client)
    pinOut = TRUE;

  StartUp();


  /* Do SDI vars first
   */
  if (pinOut)
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), pinTitle);
  else
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), sdiTitle);

  cvar = GetFirst();

  do
    {
    GetItemType(cvar, &cp[0]);

    if (strncmp(cp[0], "PMS1", 4) == 0)
      pms1d = TRUE;

    if (strncmp(cp[0], "PMS2", 4) == 0)
      pms2d = TRUE;

    if (strcmp(cp[0], "ASYNC") == 0)
      async = TRUE;

    if (strcmp(cp[0], "SDI") && strcmp(cp[0], "DIGOUT") && strcmp(cp[0], "HSKP"))
      continue;

    GetDSMlocation(cvar, &cp[0]);
    GetType(cvar, &cp[1]);
    sprintf(buffer, "%-8s%-8s  %-3s", cvar, cp[0], cp[1]);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

    GetRate(cvar, &l[0]);
    GetPrimaryAddr(cvar, &l[1]);
    GetChannelGain(cvar, &l[2]);
    GetChannelOffset(cvar, &l[3]);
    GetStart(cvar, &l[4]);
    GetSampleOffset(cvar, &l[5]);
    GetConversionOffset(cvar, &l[6]);
    GetConversionFactor(cvar, &f);

    if (pinOut)
      {
      sprintf(buffer, "%4ld %4lx %3ld  %4ld      ", l[0], l[1], l[2], l[3]);
      XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

      if (strcmp(cp[1], "A") == 0)
        sprintf(buffer, "A%ld %ld-%ld\t%s\n",
                (l[1] / 16) + 1,	((l[1] / 8) * 8) + 1,
                ((l[1] / 8) * 8) + 8,	pin[l[1] % 8]);
      else
        if (strcmp(cp[1], "D") == 0)
          sprintf(buffer, "D%ld\n", (l[1] / 8) + 1);
        else
          strcpy(buffer, "\n");

      XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);
      continue;
      }
    else
      sprintf(buffer, "%4ld %4lx %3ld  %4ld   %5ld %4ld %6ld %10.6f    ", l[0], l[1], l[2], l[3], l[4], l[5], l[6], f);

    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);


    GetOrder(cvar, &l[0]);
    GetCalCoeff(cvar, &fp);

    for (i = 0; i < l[0]; ++i)
      sprintf(&buffer[11*i], "%12.6f  ", fp[i]);

    strcat(buffer, "\n");
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);
    }
  while ((cvar = GetNext()));


  /* Standard block probes.
   */
  XmTextInsert(hdText, XmTextGetLastPosition(hdText), blockTitle);

  cvar = GetFirst();

  do
    {
    GetItemType(cvar, &cp[0]);
    if (strcmp(cp[0], "SDI") == 0 || strcmp(cp[0], "HSKP") == 0 ||
	strcmp(cp[0], "ASYNC") == 0 ||
	(strncmp(cp[0], "PMS", 3) == 0 && strcmp(cp[0], "PMS2DH")))
      continue;

    sprintf(buffer, "   %-8s ", cvar);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

    GetDSMlocation(cvar, &cp[0]);
    sprintf(buffer, " %-8s", cp[0]);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

    GetStart(cvar, &l[0]);
    sprintf(buffer, "%8ld ", l[0]);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

    GetLength(cvar, &l[0]);
    sprintf(buffer, "\t%8ld\n", l[0]);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);
    }
  while ((cvar = GetNext()));


  /* PMS1D probes.
   */
  if (pms1d)
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), pms1dTitle);

  cvar = GetFirst();

  do
    {
    GetItemType(cvar, &cp[0]);
    if (strcmp(cp[0], "PMS1V2") && strcmp(cp[0], "PMS1V3"))
      continue;

    sprintf(buffer, "   %-9s", cvar);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

    GetDSMlocation(cvar, &cp[1]);
    sprintf(buffer, "  %-8s", cp[1]);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

    GetRate(cvar, &l[0]);
    sprintf(buffer, "%4ld ", l[0]);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

    GetStart(cvar, &l[0]);
    sprintf(buffer, "%8ld ", l[0]);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

    GetLength(cvar, &l[0]);
    sprintf(buffer, "%7ld", l[0]);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

    GetInterfaceNumber(cvar, &l[0]);
    sprintf(buffer, "%9ld", l[0]);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

    if (strncmp(cp[0], "PMS1V", 5) == 0)
      GetInterfaceChannel(cvar, &l[0]);
    else
      l[0] = 0;

    sprintf(buffer, "%10ld", l[0]);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

    GetSerialNumber(cvar, &cp[1]);
    sprintf(buffer, "      %-15s", cp[1]);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

    if (strcmp(cp[0], "PMS1V3") == 0)
    {
      GetNumberBins(cvar, &l[0]);
      sprintf(buffer, "  %ld", l[0]);
      XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);
    }

    XmTextInsert(hdText, XmTextGetLastPosition(hdText), "\n");
    }
  while ((cvar = GetNext()));


  /* PMS2D probes.
   */
  if (pms2d)
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), pms2dTitle);

  cvar = GetFirst();

  do
    {
    GetItemType(cvar, &cp[0]);
    if (strcmp(cp[0], "PMS2D"))
      continue;

    sprintf(buffer, "   %-8s ", cvar);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

    GetDSMlocation(cvar, &cp[0]);
    sprintf(buffer, "  %-8s", cp[0]);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

    GetLocation(cvar, &cp[0]);
    sprintf(buffer, "   %-8s", cp[0]);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

    GetLRLength(cvar, &l[0]);
    sprintf(buffer, "%7ld ", l[0]);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

    GetLRPPR(cvar, &l[0]);
    sprintf(buffer, "%6ld ", l[0]);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

    GetResolution(cvar, &s);
    sprintf(buffer, "%8d ", (int)s);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

    GetInterfaceChannel(cvar, &l[0]);
    sprintf(buffer, "%10ld\n", l[0]);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);
    }
  while ((cvar = GetNext()));


  /* Async probes.
   */
  if (async)
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), asyncTitle);

  cvar = GetFirst();

  do
    {
    GetItemType(cvar, &cp[0]);
    if (strcmp(cp[0], "ASYNC"))
      continue;

    GetName(cvar, &cp[0]);
    sprintf(buffer, "   %-8s ", cp[0]);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

    GetDSMlocation(cvar, &cp[0]);
    sprintf(buffer, "  %-8s", cp[0]);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

    GetLRLength(cvar, &l[0]);
    sprintf(buffer, "%7ld ", l[0]);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

    GetLRPPR(cvar, &l[0]);
    sprintf(buffer, "%6ld\n", l[0]);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);
    }
  while ((cvar = GetNext()));


  ReleaseFlightHeader();

  XtManageChild(HDwindow);
  XtPopup(XtParent(HDwindow), XtGrabNone);

}	/* END HEADERDUMP */

/* -------------------------------------------------------------------- */
void DumpPMS(Widget w, XtPointer client, XtPointer call)
{
  char	*cvar;		/* Pointer to current variable	*/
  char	*cp[4];		/* Some generic Char Pointers	*/

  StartUp();

  cvar = GetFirst();

  /* Do 3 passes.  PMS1V2, PMS1V3, PMS2D.
   */

  XmTextInsert(hdText, XmTextGetLastPosition(hdText),
	"\n  Original PMS electronics into ADS-II PMS1D V2 interface card\n\n");
  do
    {
    GetItemType(cvar, &cp[0]);

    if (strcmp(cp[0], "PMS1V2"))
      continue;

    GetName(cvar, &cp[0]);
    sprintf(buffer, "   %-10s", cp[0]);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

    GetSerialNumber(cvar, &cp[0]);
    sprintf(buffer, "  %-10s", cp[0]);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

    GetLocation(cvar, &cp[0]);
    sprintf(buffer, "  %-8s", cp[0]);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

    GetDSMlocation(cvar, &cp[0]);
    sprintf(buffer, "  %-8s\n", cp[0]);
    XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);



    XmTextInsert(hdText, XmTextGetLastPosition(hdText), "\n");
    }
  while ((cvar = GetNext()));




  ReleaseFlightHeader();
 
  XtManageChild(HDwindow);
  XtPopup(XtParent(HDwindow), XtGrabNone);

}	/* END DUMPPMS */

/* -------------------------------------------------------------------- */
static void StartUp()
{
  char	*cp[4];		/* Some generic Char Pointers	*/
  long	l[8];		/* Long result storage			*/
  float	version;	/* Header Version #			*/


  if (InitFlightHeader(fileName, CLOSE) == ERR)
    {
    fprintf(stderr, "hdrdump: init error, taperr = %d\n", taperr);
    exit(1);
    }

  GetVersion(&cp[0]);
  version = atof(cp[0]);
  sprintf(buffer, "Header Version = %4.1f\n\n", version);
  XmTextSetString(hdText, buffer);
  
  GetAircraft(&cp[0]);
  GetProjectNumber(&cp[1]);
  GetFlightNumber(&cp[2]); 
  sprintf(buffer, " Aircraft: %s, Project: %s, Flight: %s\n", cp[0], cp[1], cp[2]);
  XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);
  
  GetHeaderDate(&cp[0]);
  GetHeaderTime(&cp[1]);
  sprintf(buffer, " Date: %s  Time: %s\n", cp[0], cp[1]);
  XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

  GetNumberItems(&l[0]);
  sprintf(buffer, " Number of data items: %6ld,", l[0]);
  XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);
  get_lrlen(&l[0]);
  sprintf(buffer, " Logical record length: %6ld,", l[0]);
  XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);
  get_lrppr(&l[0]);
  sprintf(buffer, " LR/PR: %6ld\n", l[0]);
  XmTextInsert(hdText, XmTextGetLastPosition(hdText), buffer);

}	/* END STARTUP */

/* -------------------------------------------------------------------- */
void LoadFile(Widget w, XtPointer client, XtPointer call)
{
  FILE	*fp;
  int	len;
  char	*p;

  if ((p = getenv("PROJ_DIR")) == NULL)
    {
    ShowError("PROJ_DIR undefined.");
    return;
    }

  sprintf(buffer, "%s/defaults/%s", p, (char *)client);

  if ((fp = fopen(buffer, "r")) == NULL)
    {
    char	temp[256];

    sprintf(temp, "Can't open %s.", buffer);
    ShowError(buffer);
    return;
    }

  XmTextSetString(hdText, buffer);
  XmTextInsert(hdText, XmTextGetLastPosition(hdText), "\n\n");

  fseek(fp, 0L, SEEK_END);
  len = ftell(fp);
  p = GetMemory(len+3);
  rewind(fp);
  fread(p, len, 1, fp);
  fclose(fp);

  p[len] = '\0';

  XmTextInsert(hdText, XmTextGetLastPosition(hdText), p);
  free(p);

}	/* END LOADFILE */

/* END CCB.C */

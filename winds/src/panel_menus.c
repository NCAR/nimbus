/* panel_menus.c --- operations associated with PANEL type menus
*/

#include <stdio.h>
#include <xview/xview.h>
#include <xview/panel.h>
#include <math.h>

#include <panel_menus.h>
#include <display.h>
#include <help.h>
#include <lengths.h>
#include <constants.h>
#include <files.h>
#include <plotattrs.h>
#include <printers.h>
#include <plotspex.h>
#include <colors.h>
#include <ctl_panel.h>
#include <socket.h>
#include <errors.h>
#include <proto.h>

/****************************   LOCAL  ***************************************/

/*
Menus listing enabled/disabled printers on net
*/
static Menu DisabledPrintersMenu=0;
static Menu EnabledPrintersMenu=0;
static Menu UserPrintersMenu=0;
static Panel panel_menu[NUM_PANEL_MENUS];	/* overall panel object */
static Panel_item menu_title[NUM_PANEL_MENUS];	/* menu's title object */
static Panel_item dismiss_menu[NUM_PANEL_MENUS];/* dismiss the menu */
static Panel_item resetprinter;                 /* invoke reset-printer op */
static Panel_item save_selns[NUM_PANEL_MENUS];	/* save current setup to file */
static int last_control[NUM_PANEL_MENUS];
static Panel_item menu_field[NUM_PANEL_MENUS] [PANEL_MAX_FIELDS];	
static char MessageLabel[NUM_PANEL_MENUS][PANEL_MAX_FIELDS][MAXMSGLABELLENGTH];	
/* 
which menu field is at top of current page 
*/
static int current_menu_field[NUM_PANEL_MENUS]=
{0,0,0,0,0};	
static int num_menu_fields[NUM_PANEL_MENUS]= {0,0,0,0,0}; /* how many menu fields */
static int CurrentPanelMenu=ERROR;		/* which menu is displayed */
static int panel_menu_top_y[NUM_PANEL_MENUS];	/* y-coord to show menu pages */
static int max_fields_per_page[NUM_PANEL_MENUS];/* fields per page, each menu */
static int BaseFrame, Width, Height;
/*
stuff needed to save PRINTINGMENU parameters operation
*/
static char filename[MAX_FLNM_LNGTH];
static char PrintEntryKeyword[NUMPRINTFIELDS][MAXKEYWORDLENGTH]={
/*
initialize these in the same order as the ordinal value of associated #define's 
in ../include/panel_menus.h
*/
IMAGEPRINTERKEYWORD,IMAGEMODEKEYWORD/***,TEXTPRINTERKEYWORD***/
};
/*
current state variable strings and flags
*/
static char CurrentConfigPrinterName[MAXPRINTERNAMELENGTH];
static char CurrentAddPrinterName[MAXPRINTERNAMELENGTH];
static int CurrentConfigPrinter=ERROR;
static int CurrentAddPrinter=ERROR;
static int PendingConfigMode=ERROR;
static char ShowSetupName[NUM_DISPLAY_QUADRANTS][MAXSHOWSETUPNAMELENGTH]=
/*
following names are set to associate n'th value panel button choice with
order used for quadrants, i.e. clockwise from upper left. Thus, when buttons
appear 2x2 in Display Setups Menu, their positioning will correspond to
the quadrant that they return for a value.
*/
{"Upper Left","Upper Right","Lower Left","Lower Right"};
static char PendingConfigModeName[NUMCONFIGMODES][MAXCONFIGMODENAMELEN]=
{"Modify Printer","Disable Printer","Enable Printer"};
static char CreateModeNames[NUMCREATEMODES][64]=
{"New","Save Current Folder As"};
/*
Plotspex stuff
*/
static char CurrentPlotspecName[MAX_DESC_LNGTH];
static int PlotspecEntryNumber=ERROR;   	/* ordinal number in file */
static int NeedFillDisplays=FALSE;
static int PasswordGiven=FALSE;
static int CurrentFolderDeleted=FALSE;
static int FolderChanged=FALSE;
static int SavedValue=ERROR;
static int NeedRefreshFolderList=FALSE;
static int FieldPosnsSet[NUM_PANEL_MENUS]= {FALSE,FALSE,FALSE,FALSE,FALSE,FALSE};

/**************************   ShowCurrentPanelMenu()  ***********************/

ShowCurrentPanelMenu()
{
 if (CurrentPanelMenu==ERROR)
  return;
 if (CurrentPanelMenu==ATTRSMENU || CurrentPanelMenu==STATSMENU)
  return;
 xv_set(panel_menu[CurrentPanelMenu],
  XV_SHOW,TRUE,
  NULL);
 if (!GetScrollPanelShowing(FALSE))
/*
re-position cursor only when scroll panel is not already showing, this is an
unnecessary irritation. Why is this function called when panel is already 
showing? In order to refresh panel's image after some update of underlying 
display has obscured it.
*/
  xv_set(panel_menu[CurrentPanelMenu],
   WIN_MOUSE_XY,0,0, 
   NULL);
 if (CurrentPanelMenu==PLOTSPEXMENU)
  SetNeedRefreshFolderList(TRUE);
 SetScrollPanelShowing(TRUE);
}

/****************************************************************************/

GetNeedRefreshFolderList()
{
 return NeedRefreshFolderList;
}

/****************************************************************************/

SetNeedRefreshFolderList(refresh)
{
 NeedRefreshFolderList=refresh;
}

/**************************   HideCurrentPanelMenu()  ***********************/

HideCurrentPanelMenu()
{
 if (CurrentPanelMenu==ERROR)
  return;
 if (CurrentPanelMenu==ATTRSMENU || CurrentPanelMenu==STATSMENU)
  return;
 RestoreCursorPosn();
 xv_set(panel_menu[CurrentPanelMenu],XV_SHOW,FALSE,NULL);
 SetScrollPanelShowing(FALSE);
}

/******************************   HIDE_PANEL_MENU()  **************************/

/*ARGSUSED*/
hide_panel_menu(Panel_item item, Event *event)
/* hide the current menu
*/
{
 
 if (CurrentPanelMenu == ERROR) 
  return;
 if (CurrentPanelMenu==ATTRSMENU || CurrentPanelMenu==STATSMENU)
  return;
 HideCurrentPanelMenu();
 UnhiliteControlPanelButton();
}

/**************************************************************************/

UnhiliteControlPanelButton()
{
/* 
reflect hidden menu on Select button
*/
 switch (CurrentPanelMenu) {
 case STATSMENU:
  SetControlButtonValue(OP_CTL_STATS_SELECT,FALSE);
  break;
 case ATTRSMENU:
  SetControlButtonValue(OP_CTL_ATTRS,FALSE);
  break;
 case PRINTINGMENU:
  SetControlButtonValue(OP_CTL_PRINTING,FALSE);
  break;
 case PRINTCONFIGMENU:
  SetControlButtonValue(OP_CTL_PRINTCONFIG,FALSE);
  break;
 case PLOTSPEXMENU:
  SetControlButtonValue(OP_CTL_SETUPS,FALSE);
  break;
 case FOLDERSMENU:
  SetControlButtonValue(OP_CTL_FOLDERS,FALSE);
  break;
 }
 SetCurrentPanelMenu(ERROR);
}

/**************************************************************************/

RemoveAllPanelMenus()
{
 RemoveCurrentPanelMenu();
 if (GetScrollPanelShowing(FALSE))
  DismissScrollPanel();
}

/**************************************************************************/

RemoveCurrentPanelMenu()
{
 HideCurrentPanelMenu();
 UnhiliteControlPanelButton();
}

/**************************************************************************/

SetCurrentPanelMenu(int which)
{
 CurrentPanelMenu=which;
}

/***************************  ResetPrinterFunc()  *************************/

/*ARGSUSED*/
void
ResetPrinterFunc(Panel_item item, Event *event)
/*
issue request to kill any existing print servers and replace them with new
ones
*/
{
 StartWindsPrinterTasks(TRUE);
}

/**********************  GetDisplayHeight()  *********************************/

GetDisplayHeight(int which, int field)
{
 switch (which) {
  case PLOTSPEXMENU:
   switch (field) {
    case SETUPLIST:
     return GetMenuItemHeight(PLOTSPEXMENU,SETUPLIST)*GetNumDispRows();
   }
   break;
 }
 return 0;
}

/**********************  GetNumDispRows()  *********************************/

GetNumDispRows()
{
 return GetBaseFrameHeight()/68;
}

/****************************   MAKE_PANEL_MENUS()  **************************/

make_panel_menus(Frame frame, int width, int height)
/* make the generic panel_menu object
*/
{
int which,menu;

/* 
any new panel-menus installed here must be installed also in panels.c, 
starting with following required match in current_panel_menu_area(). The
corresponding on-line help must be installed in events.c; see also event_ops.c
for track-ball help image stuff.
*/
static int panel_menu_area[NUM_PANEL_MENUS]={
HELPIMAGE_STATS_MENU_AREA,
HELPIMAGE_ATTRS_MENU_AREA,
HELPIMAGE_PRINT_MENU_AREA,
HELPIMAGE_PRINTCONFIG_MENU_AREA,
HELPIMAGE_PLOTSPEX_MENU_AREA,
HELPIMAGE_FOLDERS_MENU_AREA,
};
static char *help_info_target[NUM_PANEL_MENUS]={
/*
these are the same as the strings in events.c, targets[] array, with the index 
being the area assigned to each in ../include/help.h
*/
"WINDS:STATS_MENU_HELP",
"WINDS:ATTRS_MENU_HELP",
"WINDS:PRINT_MENU_HELP",
"WINDS:PRINTCONFIG_MENU_HELP",
"WINDS:PLOTSPEX_MENU_HELP",
"WINDS:FOLDERS_MENU_HELP",
};

 ClearAllFieldPosnsSet();
/*
save values for use later on by other menus types
*/
 BaseFrame=frame; Width=width; Height=height; 

/* create the objects for generic panel menu
*/
 for (which=0; which<NUM_PANEL_MENUS; which++) {
  switch (which) {
   case STATSMENU:
   case ATTRSMENU:
    break;
   default:
    if (panel_menu[which])
     xv_destroy_safe(panel_menu[which]);
    panel_menu[which]=xv_create(frame,PANEL,
	XV_HELP_DATA,help_info_target[which],
	PANEL_ACCEPT_KEYSTROKE, TRUE,
	XV_SHOW,FALSE,

/* even though width is set explicitly at end of this func, OW3 bug (?) sets
   1st panel to 571 by default, but 2nd to only 71, forcing the panel_items
   to lay out in vertical fashion instead of across one row.  Preclude that
   here with pre-set width.
*/
	XV_WIDTH,width/2,
	XV_FONT, GetStandardFont(),
	PANEL_CLIENT_DATA,panel_menu_area[which],
	PANEL_REPAINT_PROC, panel_repaint,
	NULL);
    menu=which+FIRST_PANEL_MENU;
    paint_window[menu]=canvas_paint_window(panel_menu[which]);
    xv_set(paint_window[menu],
	   PANEL_CLIENT_DATA,panel_menu_area[which],
#include <event_attr.h>
	  NULL);
    notify_interpose_event_func(panel_menu[which],
     ScrollPanelInterposer,NOTIFY_SAFE);
   menu_title[which]=xv_create(panel_menu[which],PANEL_MESSAGE,
	 PANEL_LABEL_STRING,get_menu_title_string(which),
	 NULL);
   save_selns[which]=xv_create(panel_menu[which],PANEL_BUTTON,
	 PANEL_NEXT_ROW, FIELD_MARGIN,
	 PANEL_LABEL_STRING,which==FOLDERSMENU?"Apply":
         (which==PLOTSPEXMENU?"MakeStartFolder":"Save"),
	 PANEL_NOTIFY_PROC, save_selns_proc,
	 NULL);
   dismiss_menu[which]=xv_create(panel_menu[which],PANEL_BUTTON,
	 PANEL_LABEL_STRING,"Dismiss",
	 PANEL_NOTIFY_PROC, hide_panel_menu,
	 NULL);
  }
  switch (which) {
   case PRINTCONFIGMENU: 
    last_control[PRINTCONFIGMENU]=dismiss_menu[which];
    break;
   case FOLDERSMENU:
    last_control[FOLDERSMENU]=dismiss_menu[which];
    break;
   case PRINTINGMENU: 
/*
reset printer operation
*/
    resetprinter = xv_create(panel_menu[which],PANEL_BUTTON,
        PANEL_LABEL_STRING,"Reset",
        PANEL_NOTIFY_PROC, ResetPrinterFunc,
        NULL);
    last_control[PRINTINGMENU]=resetprinter;
    break;
   case PLOTSPEXMENU:
    last_control[PLOTSPEXMENU]=dismiss_menu[which];
    break;
  }
/* 
determine where to position menu fields as per position and size of last
object created above
*/
  if (which!=ATTRSMENU && which!=STATSMENU) {
   panel_menu_top_y[which]=xv_get(last_control[which],XV_Y)+
    xv_get(last_control[which],XV_HEIGHT)+ FIELD_MARGIN;
/* 
fill the menu items as per menu-type
*/
   fill_menu(which);
  }
 }
}

/****************************  ZERO_TRIMMED()  ********************************/

char *
zero_trimmed(float val)

/* given a float val, return its string counterpart w/o any trailing zeroes
*/
{
static char string[VALUELEN];
 
 (void)sprintf(string,"%f",val);
 trim_trailing_zeros(string);
 return string;
}

/***************************  EnterFolderInfo()  ****************************/

/*ARGSUSED*/
Panel_setting
EnterFolderInfo(Panel_item item, Event *event)
{
int field;

 field=xv_get(item,PANEL_CLIENT_DATA);
 switch (field) {
  case ENTERPASSWORD:
  case ENTERFOLDERNAME:
   return PANEL_NEXT;
  case ENABLEEDITS:
   SetPasswordGiven(!strcmp((char*)xv_get(menu_field[PLOTSPEXMENU][ENABLEEDITS],
    PANEL_VALUE),GetFolderPassword()));
   xv_set(menu_field[PLOTSPEXMENU][ENABLEEDITS],PANEL_VALUE,"",NULL);
   SetFolderEditItems();
   return PANEL_NEXT;
 }
 return PANEL_NEXT;
}

/***************************  SetFolderEditItems()  ****************************/

SetFolderEditItems()
{
/*
set items active or not as function of various things...
*/
 xv_set(menu_field[PLOTSPEXMENU][DISABLEEDITS],
/*
don't want to offer disable folder editing if folder is deleted or
if password is not given. If not protected, it shouldn't even be visible, as per
function FitFolderHeight().
*/
  PANEL_INACTIVE, (GetCurrentFolderDeleted() | !GetPasswordGiven()),
  NULL);
 xv_set(menu_field[PLOTSPEXMENU][ENABLEEDITS],
/*
don't want to offer enable folder editing if folder is deleted or
if password is given. If not protected, it shouldn't even be visible, as per
function FitFolderHeight().
*/
  PANEL_INACTIVE, (GetCurrentFolderDeleted() | GetPasswordGiven()),
  NULL);
 xv_set(menu_field[PLOTSPEXMENU][DELETESPEC],
/*
don't want to offer delete plotspec operation if there are no valid setups to
delete, or if entire folder has been deleted, or if editing is disabled
*/
  PANEL_INACTIVE,!num_valid_setups() | 
                 GetCurrentFolderDeleted() | 
                 !GetFolderEditsEnabled(),NULL);
 xv_set(menu_field[PLOTSPEXMENU][DELETEFOLDER],
/*
don't want to offer delete folder operation if entire folder has been deleted, 
or if editing is disabled 
*/ 
  PANEL_INACTIVE,GetCurrentFolderDeleted() | !GetFolderEditsEnabled(),NULL);
}

/************************  GetFolderEditsEnabled()  ****************************/

GetFolderEditsEnabled()
{
 return ((GetFolderIsProtected() & GetPasswordGiven()) | 
         !GetFolderIsProtected());
}

/***************************  FoldersProc()  ****************************/

/*ARGSUSED*/
FoldersProc(Panel_item item, int value, Event *event)
{
int field;

 field=xv_get(item,PANEL_CLIENT_DATA);
 switch (field) {
  case SELECTCREATEMODE:
   break;
  case CURRENTFOLDERMSG:
   break;
  case STATUSMSG:
   break;
 }
}

/**********************  SetMenuFieldAttrs()  ******************************/

SetMenuFieldAttrs(int which, int field)
{
 switch (which) {
  case FOLDERSMENU:
   xv_set(menu_field[which][field],
     PANEL_CLIENT_DATA, field,
     PANEL_NOTIFY_PROC, FoldersProc,
   NULL);
   switch (field) {
    case CURRENTFOLDERMSG:
     xv_set(menu_field[FOLDERSMENU][CURRENTFOLDERMSG],
      XV_Y,YBelow(dismiss_menu[FOLDERSMENU]),
      XV_X,AtLeftMargin()+ExtraLeftMargin(),
     NULL);
     break;
    case SELECTCREATEMODE:
     xv_set(menu_field[FOLDERSMENU][SELECTCREATEMODE],
      XV_Y,YBelow(menu_field[FOLDERSMENU][CURRENTFOLDERMSG]),
      XV_X,AtLeftMargin()+ExtraLeftMargin(),
      PANEL_LABEL_STRING,"Create Mode",
     NULL);
     break;
    case ENTERFOLDERNAME:
     xv_set(menu_field[FOLDERSMENU][ENTERFOLDERNAME],
      XV_Y,YBelow(menu_field[FOLDERSMENU][CURRENTFOLDERMSG]),
      XV_X,XRightOfMenuField(FOLDERSMENU,SELECTCREATEMODE),
      PANEL_NOTIFY_PROC, EnterFolderInfo,
     NULL);
     break;
    case ENTERPASSWORD:
     xv_set(menu_field[FOLDERSMENU][ENTERPASSWORD],
      PANEL_MASK_CHAR,'*',
      XV_Y,YBelow(menu_field[FOLDERSMENU][SELECTCREATEMODE]),
      XV_X,AtLeftMargin()+ExtraLeftMargin(),
      PANEL_LABEL_STRING,"Protection Password (optional)",
      PANEL_NOTIFY_PROC, EnterFolderInfo,
     NULL);
     break;
    case STATUSMSG:
     xv_set(menu_field[FOLDERSMENU][STATUSMSG],
      XV_Y,YBelow(menu_field[FOLDERSMENU][ENTERPASSWORD]),
      XV_X,AtLeftMargin(),
     NULL);
     break;
   }
   break;
  case PLOTSPEXMENU:
   xv_set(menu_field[which][field],
     PANEL_CLIENT_DATA, field,
     PANEL_NOTIFY_PROC, PlotspexProc,
   NULL);
   switch (field) {
    case SELECTFOLDER:
     xv_set(menu_field[PLOTSPEXMENU][SELECTFOLDER],
      XV_Y,YBelow(dismiss_menu[PLOTSPEXMENU]),
      XV_X,AtLeftMargin(),
      PANEL_LABEL_STRING,"Select Folder",
     NULL);
     break;
    case NUMSETUPSMSG:
     xv_set(menu_field[PLOTSPEXMENU][NUMSETUPSMSG],
      XV_X,XRightOfPanel(menu_field[PLOTSPEXMENU][SELECTFOLDER]),
      XV_Y,YBelow(dismiss_menu[PLOTSPEXMENU]),
     NULL);
     break;
    case SETUPLIST:
     xv_set(menu_field[PLOTSPEXMENU][SETUPLIST],
      XV_Y,YBelow(menu_field[PLOTSPEXMENU][SELECTFOLDER]),
      XV_X,AtLeftMargin(),
      PANEL_NOTIFY_PROC,FolderListProc,
     NULL);
     break;
    case SHOWSETUP:
     xv_set(menu_field[PLOTSPEXMENU][SHOWSETUP],
      XV_Y,xv_get(last_control[PLOTSPEXMENU],XV_Y),
      XV_X,XRightOfPanel(menu_field[PLOTSPEXMENU][NUMSETUPSMSG])-
       strlen("Show Setup In:")*GetFontWidth(GetStandardFont()),
      PANEL_LABEL_STRING,"Show Setup In:",
     NULL);
     break;
    case DELETESPEC:
     xv_set(menu_field[PLOTSPEXMENU][DELETESPEC],
      XV_X,AtLeftMargin(),
      XV_Y,YBelow(menu_field[PLOTSPEXMENU][SETUPLIST]),
     NULL);
     break;
    case DELETEFOLDER:
     xv_set(menu_field[PLOTSPEXMENU][DELETEFOLDER],
      XV_X,XRightOfPanel(menu_field[PLOTSPEXMENU][DELETESPEC]),
      XV_Y,YBelow(menu_field[PLOTSPEXMENU][SETUPLIST]),
     NULL);
     break;
    case REINITPLOTSPEX:
     xv_set(menu_field[PLOTSPEXMENU][REINITPLOTSPEX],
      XV_X,XRightOfPanel(menu_field[PLOTSPEXMENU][DELETEFOLDER]),
      XV_Y,YBelow(menu_field[PLOTSPEXMENU][SETUPLIST]),
     NULL);
     break;
    case ENABLEEDITS:
     xv_set(menu_field[PLOTSPEXMENU][ENABLEEDITS],
      PANEL_MASK_CHAR,'*',
      PANEL_NOTIFY_PROC, EnterFolderInfo,
      PANEL_LABEL_STRING,"Folder Edits Disabled; Enter Password <r> To Enable:",
      XV_Y, MAX(YBelow(menu_field[PLOTSPEXMENU][REINITPLOTSPEX]),
 		YBelow(menu_field[PLOTSPEXMENU][NUMSETUPSMSG])),
      XV_X,AtLeftMargin(),
     NULL);
     break;
    case DISABLEEDITS:
     xv_set(menu_field[PLOTSPEXMENU][DISABLEEDITS],
      XV_Y,YBelow(menu_field[PLOTSPEXMENU][ENABLEEDITS]),
      XV_X,AtLeftMargin(),
      PANEL_LABEL_STRING,"Folder Edits Enabled; Click Here To Disable",
     NULL);
     break;
   }
   break;
  case PRINTCONFIGMENU:
   xv_set(menu_field[which][CONFIGAPPLY],
    XV_Y,panel_menu_top_y[which],
    XV_X,XRightOfMenuField(which,DELPRINTER),
    PANEL_LABEL_STRING,"Apply",
   NULL);
   xv_set(menu_field[which][CONFIGPENDING],
    XV_X,AtLeftMargin(),
    XV_Y,YBelow(menu_field[which][MODPRINTER]),
   NULL);
   xv_set(menu_field[which][PRINTERNAME],
    XV_X,AtLeftMargin(),
    XV_Y,YBelow(menu_field[which][CONFIGPENDING]),
   NULL);
   xv_set(menu_field[which][PRINTERMODE],
    XV_X,AtLeftMargin(),
    XV_Y,YBelow(menu_field[which][PRINTERNAME]),
    PANEL_LABEL_STRING,"Printer Image Mode",
   NULL);
   xv_set(menu_field[which][PRINTERTYPE],
    PANEL_INACTIVE,TRUE,
    XV_X,AtLeftMargin(),
    XV_Y,YBelow(menu_field[which][PRINTERMODE]),
    PANEL_LABEL_STRING,"Printer Type",
   NULL);
   UpdateConfigFields("None",ERROR);
   break;
  case PRINTINGMENU:
   xv_set(menu_field[which][field],
        PANEL_NOTIFY_PROC, PrintersProc,
        PANEL_CLIENT_DATA, field,
   NULL);
   switch (field) {
   case IMAGEPRINTER:
    xv_set(menu_field[which][field],
     XV_X,AtLeftMargin(),
     XV_Y,YBelow(dismiss_menu[which]),
     PANEL_LABEL_STRING,"Image Printer",
    NULL);
    break;
   case IMAGEMODE:
    xv_set(menu_field[which][field],
     XV_X,AtLeftMargin(),
     XV_Y,YBelow(menu_field[which][IMAGEPRINTER]),
     PANEL_LABEL_STRING,"Image Mode",
    NULL);
    break;
   case TEXTPRINTER:
    xv_set(menu_field[which][field],
     XV_X,AtLeftMargin(),
     XV_Y,YBelow(menu_field[which][IMAGEMODE]),
     PANEL_LABEL_STRING,"Text Printer",
    NULL);
    break;
   case BACKGROUND:
    xv_set(menu_field[which][field],
     PANEL_LABEL_STRING,"Background Color",
    NULL);
    break;
   case NUMLISTCOLS:
    xv_set(menu_field[which][field],
     PANEL_LABEL_STRING,"# List Columns",
    NULL);
    break;
   case PRINTTOFILE:
    break;
   case ORIENTATION:
    xv_set(menu_field[which][field],
     PANEL_LABEL_STRING,"Orientation",
    NULL);
    break;
   case NUMPERPAGE:
    xv_set(menu_field[which][field],
     PANEL_LABEL_STRING,"#/pg",
    NULL);
    break;
   case LASTPGFIRST:
    break;
  }
 }
}

/*******************  DestroyMenuFieldButton()  ******************************/

DestroyMenuFieldButton(int which, int field)
{
 xv_destroy_safe(menu_field[which][field]);
}

/****************************************************************************/

GetSetupListTitleLength()
{
 return MAX_DESC_LNGTH+1;
}

/****************************************************************************/

char *
GetSetupListTitle(int row)
{
 static char title[128];
 char format[32];

 char *string=(char *)strdup(
  (char *)xv_get(menu_field[PLOTSPEXMENU][SETUPLIST],PANEL_LIST_STRING,row));
/*
note that length to be read in must match that used to initialize the string,
as done with LeftJustify() call in CreatePlotspexChoices()
*/
 (void)sprintf(format,"%%%dc",GetSetupListTitleLength());
 (void)sscanf(string,format,title);
 return title;
}

/****************************************************************************/

void
FolderListProc(item,string,client_data,op,event,row)
Panel_item item;
char *string;
caddr_t client_data;
Panel_list_op op;
Event *event;
int row;
{
 if (op==PANEL_LIST_OP_DESELECT)
  return;
 UpdatePlotspexFields((char *)GetSetupListTitle(row),row);
}

/*******************  CreateMenuFieldButton()  ******************************/

CreateMenuFieldButton(int which,int field)
{
 extern Notify_value ScrollPanelInterposer();
 static char PanelListTitle[128];

 switch (which) {
  case PRINTINGMENU:
   menu_field[PRINTINGMENU][field]=xv_create(panel_menu[which],PANEL_CHOICE,
    PANEL_DISPLAY_LEVEL,PANEL_CURRENT,
   NULL);
   break;
  case PLOTSPEXMENU:
   if (field==SETUPLIST) {
/*
note that length to be used in LeftJustify() must match that used to read in 
the string, as done in GetSetupListTitle()
*/
    LeftJustify("Setup Title",PanelListTitle,GetSetupListTitleLength());
   LeftJustify("Setup Type",&PanelListTitle[strlen(PanelListTitle)],16);
   LeftJustify("Date Saved",&PanelListTitle[strlen(PanelListTitle)],23);
    menu_field[PLOTSPEXMENU][SETUPLIST]=xv_create(
     panel_menu[PLOTSPEXMENU],PANEL_LIST,
     PANEL_LIST_TITLE,PanelListTitle,
     PANEL_LIST_SELECT,0,TRUE,
     PANEL_CHOOSE_ONE,TRUE,
     PANEL_ITEM_COLOR,BLACK,
     XV_SHOW,!GetNeedRefreshFolderList(),
     PANEL_LIST_DISPLAY_ROWS,GetNumDispRows(),
     PANEL_NOTIFY_PROC,FolderListProc,
    NULL);
   } else {
    menu_field[PLOTSPEXMENU][field]=xv_create(panel_menu[which],PANEL_CHOICE,
     PANEL_LAYOUT,PANEL_VERTICAL,
    NULL);
   }
   break;
  case FOLDERSMENU:
   break;
 }
}

/*****************************************************************************/

GetFieldPosnsSet(int which)
{
 return FieldPosnsSet[which];
}

/*****************************************************************************/

SetFieldPosnsSet(int which,int set)
{
 FieldPosnsSet[which]=set;
}

/*****************************************************************************/

ClearAllFieldPosnsSet()
{
 int set;

 for (set=0; set<NUM_PANEL_MENUS; set++)
  SetFieldPosnsSet(set,FALSE);
}

/*****************************************************************************/

set_field_positions(int which)
/* 
set x-y coords on first request for display of object; if set during 
creation, values are brain-damaged because button items are not filled
in yet
*/
{
int field;

 if (GetFieldPosnsSet(which))
  return;

 for (field=0; field<num_menu_fields[which]; field++) {
   switch (which) {
    case FOLDERSMENU:
     for (field=0; field<FieldsPerMenu(which); field++) {
      SetMenuFieldAttrs(FOLDERSMENU,field);
     }
     break;
    case PLOTSPEXMENU:
     for (field=0; field<FieldsPerMenu(which); field++) {
      SetMenuFieldAttrs(PLOTSPEXMENU,field);
     }
     break;
    case PRINTCONFIGMENU:
     AttachPrintConfigMenus();
     SetMenuFieldAttrs(PRINTCONFIGMENU,NONE);
     break;
    case PRINTINGMENU:
     for (field=0; field<FieldsPerMenu(which); field++) {
      SetMenuFieldAttrs(PRINTINGMENU,field);
     }
     break;
   }
 }
 SetFieldPosnsSet(which,TRUE);
}

/******************************   GetMenuItemHeight()  **************************/

GetMenuItemHeight(int which,int field)
/* 
return height of each item in the menu_field[which][field]
*/
{
 switch (which) {
  case PLOTSPEXMENU:
   switch (field) {
    case SETUPLIST: 
     return xv_get(menu_field[PLOTSPEXMENU][SETUPLIST],XV_HEIGHT)/
        ItemsPerField(PLOTSPEXMENU,SETUPLIST);
    default:
     return xv_get(menu_field[which][field],XV_HEIGHT);
   }
   break;
 }
 return 0;
}

/***************************   FitFolderHeight()  **************************/

FitFolderHeight(int which)
{
 switch (which) {
  case PLOTSPEXMENU:
/*
needs adjustment as function of needing to see protection objects
*/
   if (GetFolderIsProtected())
/*
need to see all objects, including protection units
*/
    window_fit_height(panel_menu[PLOTSPEXMENU]);
   else
/*
cut off just above protection objects
*/
    xv_set(panel_menu[PLOTSPEXMENU],
     XV_HEIGHT,xv_get(menu_field[PLOTSPEXMENU][ENABLEEDITS],XV_Y,NULL),
    NULL);
   break;
 }
}

/******************************   SizePanelMenu()  **************************/

SizePanelMenu(int which)
{
/*
delay setting width and height, title and menu positions until now on USERLEVEL
type menus because their panel components vary, which affects the overall size
*/
 if (which==PLOTSPEXMENU) {
/*
special case, needs to include size of interior panel
*/
  FitFolderHeight(PLOTSPEXMENU);
  window_fit_width(panel_menu[PLOTSPEXMENU]);
  xv_set(panel_menu[PLOTSPEXMENU],
       XV_X,Width/2-xv_get(panel_menu[PLOTSPEXMENU],XV_WIDTH)/2,
       XV_Y,Height/2-xv_get(panel_menu[PLOTSPEXMENU],XV_HEIGHT)/2,
  NULL);
 } else {
/*
normal panel menus
*/
  window_fit_height(panel_menu[which]);
  window_fit_width(panel_menu[which]);
  xv_set(panel_menu[which],
       XV_X,Width/2-xv_get(panel_menu[which],XV_WIDTH)/2,
       XV_Y,Height/2-xv_get(panel_menu[which],XV_HEIGHT)/2,
  NULL);
 }
 xv_set(menu_title[which],
  XV_X, (int)(xv_get(panel_menu[which],XV_WIDTH)/2)-
   (int)(xv_get(menu_title[which],XV_WIDTH)/2),
 NULL);
}

/******************************   SHOW_PANEL_MENU()  **************************/

show_panel_menu(int which)
/* show the which'th panel menu and all of its fields
*/
{
int field,inactive;

 set_field_positions(which);
/*
delay setting width and height, title and menu positions until now 
because panel components vary, which affects the overall dimensions
*/
 SizePanelMenu(which);
 hide_panel_menu((Panel_item)0,(Event *)0);
 CurrentPanelMenu=which;
 ShowCurrentPanelMenu();
 for (field=current_menu_field[which]; 
      field<MIN(
       current_menu_field[which]+max_fields_per_page[which],
       num_menu_fields[which]); 
      field++) {
  xv_set(menu_field[which][field],
         XV_SHOW,TRUE,
         NULL);
 }
 switch (which) {
  case FOLDERSMENU:
   CreateFolderButtons();
   break;
  case PLOTSPEXMENU:
/*
create select menu again so current folder name gets updated
*/
   SetPlotspexFolderMenu();
   break;
  case PRINTCONFIGMENU:
   if (!GetNumEnabledPrinters()) {
    xv_set(menu_field[PRINTCONFIGMENU][DELPRINTER],PANEL_INACTIVE,TRUE,NULL);
    xv_set(menu_field[PRINTCONFIGMENU][MODPRINTER],PANEL_INACTIVE,TRUE,NULL);
   }
   if (!GetNumDisabledPrinters())
    xv_set(menu_field[PRINTCONFIGMENU][ADDPRINTER],PANEL_INACTIVE,TRUE,NULL);
   break;
  case PRINTINGMENU:
   inactive=!GetNumImagePrinters();
   xv_set(menu_field[PRINTINGMENU][IMAGEPRINTER],
         PANEL_INACTIVE,inactive,
         NULL);
   if (inactive)
    xv_set(menu_field[PRINTINGMENU][IMAGEPRINTER],
         PANEL_LABEL_STRING,"Image Printer: none",
         NULL);
   xv_set(menu_field[PRINTINGMENU][IMAGEMODE],
         PANEL_INACTIVE,inactive,
         NULL);
/*** uncomment when objects are made and their procedures are implemented:

   xv_set(menu_field[PRINTINGMENU][BACKGROUND],
         PANEL_INACTIVE,inactive,
         NULL);
   xv_set(menu_field[PRINTINGMENU][ORIENTATION],
         PANEL_INACTIVE,inactive,
         NULL);
   xv_set(menu_field[PRINTINGMENU][NUMPERPAGE],
         PANEL_INACTIVE,inactive,
         NULL);
   xv_set(menu_field[PRINTINGMENU][LASTPGFIRST],
         PANEL_INACTIVE,inactive,
         NULL);
   inactive=!GetNumTextPrinters();
   xv_set(menu_field[PRINTINGMENU][TEXTPRINTER],
         PANEL_INACTIVE,inactive,
         NULL);
   if (inactive)
    xv_set(menu_field[PRINTINGMENU][TEXTPRINTER],
         PANEL_LABEL_STRING,"Text Printer: none",
         NULL);
   xv_set(menu_field[PRINTINGMENU][NUMLISTCOLS],
         PANEL_INACTIVE,inactive,
         NULL);
***/
   break;
 }
}

/**************************  GetMessageLabel()  *************************/

char *
GetMessageLabel(int which,int field,char string[])
/*
set message labels for items whose message will change during run-time
*/
{

 switch (which) {
 case PRINTCONFIGMENU:
  switch (field) {
  case PRINTERNAME:
   (void)sprintf(MessageLabel[which][field],"Printer Name: %s",string);
   break;
  case CONFIGPENDING:
   (void)sprintf(MessageLabel[which][field],"Pending Operation: %s",string);
   break;
  }
  break;
 case FOLDERSMENU:
  switch (field) {
  case CURRENTFOLDERMSG:
   (void)sprintf(MessageLabel[which][field],"Current Folder: %s",string);
    break;
  case STATUSMSG:
   (void)sprintf(MessageLabel[which][field],"%s",string);
    break;
  }
  break;
 case PLOTSPEXMENU:
  switch (field) {
  case NUMSETUPSMSG:
   (void)sprintf(MessageLabel[which][field],"%d Setups/%d Max",
    GetCurrentFolderDeleted()?0:num_valid_setups(),get_maxsetups());
   break;
  } 
 }
 return MessageLabel[which][field];
}

/**************************  SET_MENU_FIELD_STRING()  *************************/

set_menu_field_string(int which,int field,int item_number,char string[])
/* initialize the strings for items
*/
{
 switch (which) {
  case FOLDERSMENU:
   switch (field) {
    case SELECTCREATEMODE:
     xv_set(menu_field[FOLDERSMENU][field],
        PANEL_CHOICE_STRING,item_number,string,
     NULL);
     break;
    case CURRENTFOLDERMSG:
     xv_set(menu_field[FOLDERSMENU][field],
      PANEL_LABEL_STRING,(char *)GetMessageLabel(which,field,string),
     NULL);
     break;
    case STATUSMSG:
     xv_set(menu_field[FOLDERSMENU][field],
      PANEL_LABEL_STRING,(char *)GetMessageLabel(which,field,string),
     NULL);
     break;
    case ENTERFOLDERNAME:
    case ENTERPASSWORD:
     xv_set(menu_field[FOLDERSMENU][field],
      PANEL_VALUE,string,
     NULL);
     break;
   }
   break;
  case PLOTSPEXMENU:
   switch (field) {
    case REINITPLOTSPEX:
    case DELETEFOLDER:
    case DELETESPEC:
     xv_set(menu_field[PLOTSPEXMENU][field],
        PANEL_LABEL_STRING,string,
     NULL);
     break;
    case NUMSETUPSMSG:
     xv_set(menu_field[PLOTSPEXMENU][field],
      PANEL_LABEL_STRING,(char *)GetMessageLabel(which,field,string),
     NULL);
     break;
    case SELECTFOLDER:
     xv_set(menu_field[PLOTSPEXMENU][SELECTFOLDER],
        PANEL_CHOICE_STRING,item_number,string,
     NULL);
     break;
    case DISABLEEDITS:
    case ENABLEEDITS:
     break;
    case SHOWSETUP:
     xv_set(menu_field[PLOTSPEXMENU][SHOWSETUP],
        PANEL_CHOICE_STRING,item_number,string,
     NULL);
     break;
    case SETUPLIST:
     xv_set(menu_field[PLOTSPEXMENU][SETUPLIST],
         PANEL_LIST_STRING,item_number,string,
     NULL);
     break;
    }
   break;
  case PRINTCONFIGMENU:
   switch (field) {
    case ADDPRINTER:
     xv_set(DisabledPrintersMenu,
      MENU_ITEM,
       MENU_STRING, string,
       MENU_VALUE, item_number,
       MENU_NOTIFY_PROC, EnablePrinter,
      0,
     NULL);
     break;
    case DELPRINTER:
     xv_set(EnabledPrintersMenu,
      MENU_ITEM,
       MENU_STRING, string,
       MENU_VALUE, item_number,
       MENU_NOTIFY_PROC, DisablePrinter,
      0,
     NULL);
     break;
    case MODPRINTER:
     xv_set(UserPrintersMenu,
      MENU_ITEM,
       MENU_STRING, string,
       MENU_VALUE, item_number,
       MENU_NOTIFY_PROC, EditPrinter,
      0,
     NULL);
     break;
    case PRINTERMODE:
    case PRINTERTYPE:
     xv_set(menu_field[PRINTCONFIGMENU][field],
      PANEL_CHOICE_STRING,item_number,string,
     NULL);
     break;
    case PRINTERNAME:
     xv_set(menu_field[PRINTCONFIGMENU][PRINTERNAME],
      PANEL_LABEL_STRING,(char *)GetMessageLabel(PRINTCONFIGMENU,PRINTERNAME,string),
     NULL);
     break;
    case CONFIGPENDING:
     xv_set(menu_field[PRINTCONFIGMENU][CONFIGPENDING],
      PANEL_LABEL_STRING,(char *)GetMessageLabel(which,CONFIGPENDING,string),
     NULL);
     break;
    case CONFIGAPPLY:
     xv_set(menu_field[which][field],
      PANEL_LABEL_STRING,string,
     NULL);
     break;
   }
   break;
  case PRINTINGMENU:
   xv_set(menu_field[which][field],
       PANEL_CHOICE_STRING,item_number,string,
   NULL);
   break;
 }
}

/*****************************************************************************/

YBelowPanel(Panel panel)
{
 return xv_get(panel,XV_Y)
       +xv_get(panel,XV_HEIGHT)
       +FIELD_MARGIN;
}

/*****************************************************************************/

XRightOfPanel(Panel panel)
{
 return xv_get(panel,XV_X)
       +xv_get(panel,XV_WIDTH)
       +FIELD_MARGIN;
}

/*****************************************************************************/

XRightOfMenuField(int which,int field)
{
 return xv_get(menu_field[which][field],XV_X)
       +xv_get(menu_field[which][field],XV_WIDTH)
       +FIELD_MARGIN;
}

/*****************************************************************************/

YSameAsMenuField(int which,int field)
{
 return xv_get(menu_field[which][field],XV_Y);
}

/*****************************************************************************/

YBelow(Panel_item object)
{
 return xv_get(object,XV_Y)+xv_get(object,XV_HEIGHT)+FIELD_MARGIN;
}

/*****************************************************************************/

ExtraLeftMargin() 
{
 return MENU_FIELD_X*7;
}

/*****************************************************************************/

AtLeftMargin()
{
 return MENU_FIELD_X;
}

/**********************  GetMaxPanelMenuWidth()  ****************************/

GetMaxPanelMenuWidth()
{
 return xv_get(BaseFrame,XV_WIDTH) * PANELMENUSIZEFACTOR;
}

/********************  GetMaxPanelMenuHeight()  ****************************/

GetMaxPanelMenuHeight()
{
 return xv_get(BaseFrame,XV_HEIGHT) * PANELMENUSIZEFACTOR;
}

/**************************  CREATE_MENU_FIELD()  ****************************/

create_menu_field(int which,int field)
/* called by client, to make a menu field for its menu
*/
{
 if (field==PANEL_MAX_FIELDS) {
  (void)fprintf(stderr,"WARNING: asking for too many menu fields\n");
  return;
 }
 switch (which) {
  case FOLDERSMENU:
   for (field=0; field<FieldsPerMenu(which); field++) {
    switch (field) {
     case SELECTCREATEMODE:
      menu_field[which][field]=xv_create(panel_menu[which],PANEL_CHOICE,NULL);
      break;
     case CURRENTFOLDERMSG:
     case STATUSMSG:
      menu_field[which][field]=xv_create(panel_menu[which],PANEL_MESSAGE,NULL);
      break;
     case ENTERPASSWORD:
     case ENTERFOLDERNAME:
      menu_field[which][field]=xv_create(panel_menu[which],PANEL_TEXT,
	 PANEL_VALUE_DISPLAY_LENGTH,MAXFOLDERNAMELENGTH-1,
         PANEL_VALUE_STORED_LENGTH, MAXFOLDERNAMELENGTH-1,
      NULL);
      break;
    }
    xv_set(menu_field[FOLDERSMENU][field],
     XV_SHOW,FALSE,
    NULL);
    SetMenuFieldAttrs(FOLDERSMENU,field);
   }
   break;
  case PLOTSPEXMENU:
   for (field=0; field<FieldsPerMenu(which); field++) {
    switch (field) {
     case NUMSETUPSMSG:
      menu_field[which][field]=xv_create(panel_menu[which],PANEL_MESSAGE,NULL);
      break;
     case SETUPLIST:
      CreateMenuFieldButton(which,field);
      break;
     case SELECTFOLDER:
      menu_field[which][field]=xv_create(panel_menu[which],PANEL_CHOICE,NULL);
      SetSelectFolderDisplayLevel();
      break;
     case DISABLEEDITS:
      menu_field[which][field]=xv_create(panel_menu[which],PANEL_BUTTON,NULL);
      break;
     case ENABLEEDITS:
      menu_field[which][field]=xv_create(panel_menu[which],PANEL_TEXT,
	 PANEL_VALUE_DISPLAY_LENGTH,MAXPASSWORDLENGTH-1,
         PANEL_VALUE_STORED_LENGTH, MAXPASSWORDLENGTH-1,
      NULL);
      break;
     case SHOWSETUP:
      menu_field[which][field]=xv_create(panel_menu[which],PANEL_CHOICE,
       PANEL_CHOICE_NCOLS,ItemsPerField(which,field)/2,
       PANEL_CHOICE_NROWS,ItemsPerField(which,field)/2,
       PANEL_CHOOSE_NONE,TRUE,
       PANEL_VALUE,-1,
       NULL);
      break;
     case REINITPLOTSPEX:
     case DELETEFOLDER:
     case DELETESPEC:
      menu_field[which][field]=xv_create(panel_menu[which],PANEL_BUTTON,NULL);
      break;
    }
    xv_set(menu_field[PLOTSPEXMENU][field],
     XV_SHOW,FALSE,
    NULL);
    SetMenuFieldAttrs(PLOTSPEXMENU,field);
   }
   break;
  case PRINTCONFIGMENU:
   for (field=0; field<FieldsPerMenu(which); field++) {
    switch (field) {
    case CONFIGPENDING:
    case PRINTERNAME:
     menu_field[which][field]=xv_create(panel_menu[which],PANEL_MESSAGE,NULL);
     break;
    case MODPRINTER:
    case DELPRINTER:
    case ADDPRINTER:
     menu_field[which][field]=xv_create(panel_menu[which],PANEL_BUTTON,NULL);
     break;
    case PRINTERTYPE:
    case PRINTERMODE:
     menu_field[which][field]=xv_create(panel_menu[which],PANEL_CHOICE,
       PANEL_DISPLAY_LEVEL,PANEL_CURRENT,
     NULL);
     break;
    case CONFIGAPPLY:
     menu_field[which][field]=xv_create(panel_menu[which],PANEL_BUTTON,NULL);
     break;
    }
    xv_set(menu_field[which][field],
         XV_SHOW,FALSE,
         PANEL_NOTIFY_PROC, PrintConfigProc,
         PANEL_CLIENT_DATA, field,
    NULL);
   }
   SetMenuFieldAttrs(PRINTCONFIGMENU,NONE);
   break;
  case PRINTINGMENU:
   for (field=0; field<FieldsPerMenu(which); field++) {
    switch (field) {
    case IMAGEMODE:
    case IMAGEPRINTER:
    case TEXTPRINTER:
    case ORIENTATION:
    case BACKGROUND:
    case NUMPERPAGE:
    case NUMLISTCOLS:
     CreateMenuFieldButton(which,field);
     break;
    case LASTPGFIRST:
    case PRINTTOFILE:
     menu_field[which][field]=xv_create(panel_menu[which],PANEL_TOGGLE,NULL);
     break;
    }
    xv_set(menu_field[which][field],
        XV_SHOW,FALSE,
    NULL);
    SetMenuFieldAttrs(which,field);
  }
 }
}

/**************************  GetPanelListString()  *************************/

char *
GetPanelListString(int which,int field)
{
 return (char *)GetSetupListTitle(GetFieldValue(PLOTSPEXMENU,SETUPLIST));
}

/**************************  GetFieldValue()  *************************/

GetFieldValue(int which,int field)
/* 
return the value of the field'th field on the which'th menu 
*/
{
 if (which==PLOTSPEXMENU && field==SETUPLIST) {
  return xv_get(menu_field[PLOTSPEXMENU][SETUPLIST],PANEL_LIST_FIRST_SELECTED);
 } else {
  return xv_get(menu_field[which][field],PANEL_VALUE);
 }
}

/**************************  SetFieldValue()  *************************/

SetFieldValue(int which,int field,int value)
{
 xv_set(menu_field[which][field],
   PANEL_VALUE,value,
 NULL);
}

/*********************  SetCurrentPlotspecName()  ****************************/

SetCurrentPlotspecName(char string[])
{
 (void)sprintf(CurrentPlotspecName,"%s",(char *)TrimTrailingBlanks(string));
}
 
/********************  GetCurrentPlotspecName()  ****************************/

char *
GetCurrentPlotspecName()
{
 return (char *)TrimTrailingBlanks(CurrentPlotspecName);
}
 
/******************  GetCurrentPlotspecNumber()  ****************************/

GetCurrentPlotspecNumber()
/*
return the ordinal position in the plotspex file of the current plotspec being
examined in the Display Setups Editing Menu (PLOTSPEXMENU)
*/
{
 return PlotspecEntryNumber;
}

/******************  SetCurrentPlotspecNumber()  ****************************/

SetCurrentPlotspecNumber(int value)
/*
set the ordinal position in the plotspex file of the current plotspec being
examined in the Display Setups Editing Menu (PLOTSPEXMENU) to value. 
*/
{
 PlotspecEntryNumber=value;
}

/************************  UpdatePlotspexFields()  ****************************/

UpdatePlotspexFields(char string[], int menuvalue)
/*
update the messages and position of Setup List in Display Setups Editing Menu upon
selection of a new current setup, whether through direct click selection or
deletion of a setup that calls for re-calculation of new current one.
*/
{
int value;

 value=get_valid_setup(menuvalue);
 SetCurrentPlotspecNumber(value);
 SetPlotspexMsgValues(string,value);
 SetPlotspexFieldsActive();
}

/**************************************************************************/

RefreshFolderList()
{
 ShowPanelItem(menu_field[PLOTSPEXMENU][SETUPLIST]);
 xv_set(menu_field[PLOTSPEXMENU][SETUPLIST],
  PANEL_LIST_SELECT,GetSavedValue(),TRUE, NULL);
}

/***********************  SetPlotspexMsgValues()  *************************/

SetPlotspexMsgValues(char string[], int value)
/*
given the string value of current setup and its menu value, set the message fields
accordingly. Check first on whether the folder is deleted and then whether there
are any setups left in an undeleted folder.
*/
{
 if (!GetCurrentFolderDeleted() && num_valid_setups()) {
  SetCurrentPlotspecName(string);
 } else {
  SetCurrentPlotspecName("n/a");
 }
 set_menu_field_string(PLOTSPEXMENU,NUMSETUPSMSG,0,"");
}

/***********************  SetPlotspexFieldsActive()  *************************/

SetPlotspexFieldsActive()
/*
set relevant fields active or not as function of whether there are any setups
in the current folder
*/
{

 xv_set(menu_field[PLOTSPEXMENU][SETUPLIST],
  PANEL_INACTIVE,!num_valid_setups(),
  NULL);
 xv_set(menu_field[PLOTSPEXMENU][SHOWSETUP],
  PANEL_INACTIVE,!num_valid_setups(),
  NULL);
/*
mucking with SHOWSETUP hides NUMSETUPSMSG; rectify that with a flush here
*/
 xv_set(menu_field[PLOTSPEXMENU][NUMSETUPSMSG],
  XV_SHOW,TRUE,
  NULL);
 xv_set(menu_field[PLOTSPEXMENU][DELETESPEC],
  PANEL_INACTIVE,!num_valid_setups(),
  NULL);
 SetFolderEditItems();
}

/***************************  PlotspexProc()  ****************************/

/*ARGSUSED*/
void
PlotspexProc(Panel_item item, int value, Event *event)
{
int quadrant,field;
 field=xv_get(item,PANEL_CLIENT_DATA);
 switch (field) {
  case REINITPLOTSPEX:
   time_to_log();
   (void)fprintf(stderr,"Re-initialize plotspex\n",GetCurrentPlotspecName());
   SetNeedInitPlotspex(TRUE);
   SetNeedFillDisplays(TRUE);
   break;
  case DELETEFOLDER:
   notify_user_error(REMOVEPLOTSPEXFOLDER);
   if (overwrite)
    RemovePlotspexFolder((char *)GetCurrentPlotspexFolderName());
   break;
  case DELETESPEC:
   SetPendingDescription(GetCurrentPlotspecName());
   save_setup(ERROR);
   SetDeletePlotspec(FALSE);
   break;
  case SELECTFOLDER:
   if (strcmp((char *)GetCurrentPlotspexFolderName(),
              (char *)xv_get(item,PANEL_CHOICE_STRING,value))) {
/*
this is NOT the current folder: change it
*/
    time_to_log();
    (void)fprintf(stderr,"Chg folders, from '%s' to ",
     GetCurrentPlotspexFolderName());
    SetCurrentPlotspexFolderName((char *)xv_get(item,PANEL_CHOICE_STRING,value));
    (void)fprintf(stderr,"'%s'\n",GetCurrentPlotspexFolderName());
    SetCurrentFolderDeleted(FALSE);
    SetPlotspexFilename();
    SetNeedInitPlotspex(TRUE);
    SetNeedFillDisplays(TRUE);
    SetPasswordGiven(FALSE);
    HidePanelItem(menu_field[PLOTSPEXMENU][SETUPLIST]);
   }
   break;
  case SHOWSETUP:
/*
adjust panel button values to jive with quadrants (numbered clockwise from 
upper left, whereas buttons are numbered ordinally left to right, each row)
*/
   quadrant=value;
   if (quadrant==LOWER_RIGHT)
    quadrant=LOWER_LEFT;
   else if (quadrant==LOWER_LEFT)
    quadrant=LOWER_RIGHT;
   time_to_log();
   (void)fprintf(stderr,"show setup %s in quadrant %d\n",GetCurrentPlotspecName(),
    quadrant);
   xv_set(item,PANEL_VALUE,-1,NULL);
   config_display(quadrant,GetDispType(GetCurrentPlotspecNumber()),
/*
set to one more than panel value selected because the file search proceeds from 
1, whereas the panel values are counted from 0.
*/
    GetCurrentPlotspecNumber()+1);
   SetSavedValue(GetFieldValue(PLOTSPEXMENU,SETUPLIST));
   UpdatePlotspexFields((char *)GetSetupListTitle(GetSavedValue()),
    GetSavedValue());
   if (GetScrollPanelShowing(FALSE))
/*
update of displays is blocked; flush relevant display manually instead here
*/
    UpdateAndShowDisplay(quadrant);
/***
   set_next_tb_image(GetDispType(GetCurrentPlotspecNumber()));
   need_rehilite[quadrant]=TRUE;
***/
   break;
  case DISABLEEDITS:
   SetPasswordGiven(FALSE);
   SetFolderEditItems();
   break;
 }
}

/***************************  PrintersProc()  ****************************/

/*ARGSUSED*/
void
PrintersProc(Panel_item item, int value, Event *event)
{
int field;

 field=xv_get(item,PANEL_CLIENT_DATA);
 switch (field) {
  case PRINTTOFILE:
   break;
  case ORIENTATION:
   break;
  case BACKGROUND:
   break;
  case NUMPERPAGE:
   break;
  case IMAGEMODE:
   SetCurrentImageMode(value);
   break;
  case NUMLISTCOLS:
   break;
  case LASTPGFIRST:
   break;
  case IMAGEPRINTER:
   SetCurrentImagePrinter(value);
   break;
  case TEXTPRINTER:
   SetCurrentTextPrinter(value);
   break;
  default:
   break;
 }
}

/********************  AttachPrintConfigMenus()  ******************************/

AttachPrintConfigMenus()
{
  xv_set(menu_field[PRINTCONFIGMENU][MODPRINTER],
   XV_X,AtLeftMargin(),
   XV_Y,panel_menu_top_y[PRINTCONFIGMENU],
   PANEL_ITEM_MENU,UserPrintersMenu,
   PANEL_LABEL_STRING,"Edit",
  NULL);
  xv_set(menu_field[PRINTCONFIGMENU][DELPRINTER],
   XV_Y,panel_menu_top_y[PRINTCONFIGMENU],
   XV_X,XRightOfMenuField(PRINTCONFIGMENU,ADDPRINTER),
   PANEL_ITEM_MENU,EnabledPrintersMenu,
   PANEL_LABEL_STRING,"Disable",
  NULL);
  xv_set(menu_field[PRINTCONFIGMENU][ADDPRINTER],
   XV_Y,panel_menu_top_y[PRINTCONFIGMENU],
   XV_X,XRightOfMenuField(PRINTCONFIGMENU,MODPRINTER),
   PANEL_ITEM_MENU,DisabledPrintersMenu,
   PANEL_LABEL_STRING,"Enable",
  NULL);
}

/************************** GetCurrentAddPrinter() **************************/

GetCurrentAddPrinter()
{
 return CurrentAddPrinter;
}

/************************** SetCurrentAddPrinter() **************************/

SetCurrentAddPrinter(int printer)
{
 SetCurrentAddPrinterName(GetDisabledPrinterName(printer));
 CurrentAddPrinter=printer;
}

/********************** GetCurrentConfigPrinter() **************************/

GetCurrentConfigPrinter()
{
 return CurrentConfigPrinter;
}

/************************** SetCurrentConfigPrinter() **************************/

SetCurrentConfigPrinter(int printer)
{
 SetCurrentConfigPrinterName(GetEnabledPrinterName(printer));
 CurrentConfigPrinter=printer;
}

/********************** GetCurrentAddPrinterName() **************************/

char *
GetCurrentAddPrinterName()
{
 return CurrentAddPrinterName;
}

/********************** SetCurrentAddPrinterName() **************************/

SetCurrentAddPrinterName(char printername[])
{
 (void)sprintf(CurrentAddPrinterName,"%s",printername);
}

/********************** GetCurrentConfigPrinterName() **************************/

char *
GetCurrentConfigPrinterName()
{
 return CurrentConfigPrinterName;
}

/********************** SetCurrentConfigPrinterName() **************************/

SetCurrentConfigPrinterName(char printername[])
{
 (void)sprintf(CurrentConfigPrinterName,"%s",printername);
}

/**************************  UpdateConfigFields() ****************************/

UpdateConfigFields(char printername[], int pendingmode)
{
int inactive;

 if (pendingmode==ERROR) {
  inactive=TRUE;
  set_menu_field_string(PRINTCONFIGMENU,PRINTERNAME,0,"None");
  set_menu_field_string(PRINTCONFIGMENU,CONFIGPENDING,0,"None");
 } else {
  inactive=FALSE;
  set_menu_field_string(PRINTCONFIGMENU,PRINTERNAME,0,printername);
  SetFieldValue(PRINTCONFIGMENU,PRINTERMODE,
   GetPrinterMode(GetPrinterIndex(printername)));
  SetFieldValue(PRINTCONFIGMENU,PRINTERTYPE,
   GetPrinterType(GetPrinterIndex(printername)));
  set_menu_field_string(PRINTCONFIGMENU,CONFIGPENDING,
   0,PendingConfigModeName[pendingmode]);
 }
 xv_set(menu_field[PRINTCONFIGMENU][PRINTERMODE],
  PANEL_INACTIVE,inactive,
 NULL);
 xv_set(menu_field[PRINTCONFIGMENU][PRINTERTYPE],
  PANEL_INACTIVE,inactive,
 NULL);
 xv_set(menu_field[PRINTCONFIGMENU][CONFIGAPPLY],
  PANEL_INACTIVE,inactive,
 NULL);
}

/**************************  EditPrinter()  ****************************/

/*ARGSUSED*/
void
EditPrinter(Menu menu, Menu_item item)
{
 SetPendingConfigMode(MODPRINTER);
 SetCurrentConfigPrinter((int)xv_get(item,MENU_VALUE));
 UpdateConfigFields(GetEnabledPrinterName(GetCurrentConfigPrinter()),EDITPRINTER);
}

/**************************  DisablePrinter()  ****************************/

/*ARGSUSED*/
void
DisablePrinter(Menu menu, Menu_item item)
{
 SetPendingConfigMode(DELPRINTER);
 SetCurrentConfigPrinter((int)xv_get(item,MENU_VALUE));
 UpdateConfigFields(GetCurrentConfigPrinterName(),DISABLEPRINTER);
}

/**************************  EnablePrinter()  ****************************/

/*ARGSUSED*/
void
EnablePrinter(Menu menu, Menu_item item)
{
 SetPendingConfigMode(ADDPRINTER);
 SetCurrentAddPrinter((int)xv_get(item,MENU_VALUE));
 UpdateConfigFields((char *)xv_get(item,MENU_STRING),ENABLEPRINTER);
}

/***************************  GetPendingConfigMode()  **************************/

GetPendingConfigMode()
{
 return PendingConfigMode;
}

/***************************  SetPendingConfigMode()  **************************/

SetPendingConfigMode(int pendingmode)
{
 PendingConfigMode=pendingmode;
}

/***************************  PrintConfigProc()  ****************************/

/*ARGSUSED*/
void
PrintConfigProc(Panel_item item, int value, Event *event)
{
int field;
char printername[MAXPRINTERNAMELENGTH];

 field=xv_get(item,PANEL_CLIENT_DATA);
 switch (field) {
  case PRINTERNAME:
  case CONFIGPENDING:
  case PRINTERTYPE:
  case PRINTERMODE:
  case ADDPRINTER:
  case DELPRINTER:
  case MODPRINTER:
   break;
  case CONFIGAPPLY:
   switch (GetPendingConfigMode()) {
    case ADDPRINTER:
     (void)sprintf(printername,"%s",(char *)GetCurrentAddPrinterName());
     if (PrinterNameinEnabledList(printername)) 
      break;
     ConfigurePrinter(TRUE,printername,
      GetPrinterTypeName(GetFieldValue(PRINTCONFIGMENU,PRINTERTYPE)),
      GetImageModeName(GetFieldValue(PRINTCONFIGMENU,PRINTERMODE)),
      DEFAULTBKGND,DEFAULTORIENT,1/*DEFAULTNUMPERPAGE*/);
     CreatePrintConfigMenus();
     CreatePrintingChoices();
     if (GetNumEnabledPrinters()) {
      xv_set(menu_field[PRINTCONFIGMENU][DELPRINTER],PANEL_INACTIVE,FALSE,NULL);
      xv_set(menu_field[PRINTCONFIGMENU][MODPRINTER],PANEL_INACTIVE,FALSE,NULL);
     }
     if (!GetNumDisabledPrinters())
      xv_set(menu_field[PRINTCONFIGMENU][ADDPRINTER],PANEL_INACTIVE,TRUE,NULL);
     UpdateConfigFields("None",ERROR);
     break;
    case MODPRINTER:
     (void)sprintf(printername,"%s",(char *)GetCurrentConfigPrinterName());
     ConfigurePrinter(TRUE,printername,
      GetPrinterTypeName(GetFieldValue(PRINTCONFIGMENU,PRINTERTYPE)),
      GetImageModeName(GetFieldValue(PRINTCONFIGMENU,PRINTERMODE)),
      DEFAULTBKGND,DEFAULTORIENT,1/*DEFAULTNUMPERPAGE*/);
/*
available vs. disabled lists don't change, but text vs. image mode capability
may have changed
*/
     CreatePrintingChoices();
     break;
    case DELPRINTER:
     (void)sprintf(printername,"%s",(char *)GetCurrentConfigPrinterName());
     if (!PrinterNameinEnabledList(printername)) 
      break;
     ConfigurePrinter(FALSE,printername,
      GetPrinterTypeName(GetFieldValue(PRINTCONFIGMENU,PRINTERTYPE)),
      GetImageModeName(GetFieldValue(PRINTCONFIGMENU,PRINTERMODE)),
      DEFAULTBKGND,DEFAULTORIENT,1/*DEFAULTNUMPERPAGE*/);
     CreatePrintConfigMenus();
     CreatePrintingChoices();
     if (!PrinterNameinEnabledList(GetCurrentImagePrinterName()))
/*
the user-designated current image printer was just disabled; set current image
printer to default selection
*/
      if (GetNumImagePrinters())
       SetCurrentImagePrinter(0);
/***
     if (!PrinterNameinEnabledList(GetCurrentTextPrinterName()))
/*
the user-designated current text printer was just disabled; set current text
printer to default selection
*/
/***
      if (GetNumTextPrinters())
       SetCurrentTextPrinter(0);
***/
     if (GetNumDisabledPrinters())
      xv_set(menu_field[PRINTCONFIGMENU][ADDPRINTER],PANEL_INACTIVE,FALSE,NULL);
     if (!GetNumEnabledPrinters()) {
      xv_set(menu_field[PRINTCONFIGMENU][DELPRINTER],PANEL_INACTIVE,TRUE,NULL);
      xv_set(menu_field[PRINTCONFIGMENU][MODPRINTER],PANEL_INACTIVE,TRUE,NULL);
     }
     UpdateConfigFields("None",ERROR);
     break;
   }
   break;
  default:
   break;
 }
}

/**************************  MODULE USERS STUFF  ****************************/

/***********************   GET_VAR_DESCRIPTION()  *************************/

char *
get_var_description(int index)
/* return the bulletin 9 (or user-defined) description for a variable, given
   its index into raw and derived string arrays
*/
{
char *varname;
static char descript[MAX_TITLE_LENGTH + NAMLEN + 3];
char *title;

 varname=GetVarListName(index);
 title=get_vartitle(index);
 (void)sprintf(descript,"%s: %s",varname,title);
 return descript;
}

/***********************   GET_MENU_TITLE_STRING()  **************************/

char *
get_menu_title_string(int menu)
/* return the title string for given menu
*/
{
 switch (menu) {
  case PRINTINGMENU:
   return "Printing Menu";
  case PRINTCONFIGMENU:
   return "Printer Configuration Menu";
  case PLOTSPEXMENU:
   return "Display Folder Editing Menu";
  case FOLDERSMENU:
   return "Display Folder Creation Menu";
 }
 return "";
}

/*************************  ErrorReport() **********************************/

ErrorReport(char message[]) 
{
 time_to_log();
 (void)fprintf(stderr,"%s\n",message);
 return;
}

/******************************   SAVE_SELNS_PROC()  **************************/

/*ARGSUSED*/
void
save_selns_proc(Panel_item item, Event *event)
/* save the current setup
*/
{
 switch (CurrentPanelMenu) {
 case FOLDERSMENU:
  SaveNewFolder((char *)xv_get(menu_field[FOLDERSMENU][ENTERFOLDERNAME],
   PANEL_VALUE));
  break;
 case PLOTSPEXMENU:
  (void)sprintf(filename,USERINIT,UserHomeDir);
  time_to_log();
  (void)fprintf(stderr,"Save %s as startup plotspex folder\n",
   GetCurrentPlotspexFolderName());
  SaveEntriesToFile(filename,NUMPLOTSPEXSAVEFIELDS,GetPlotspexParmKeyword,
   GetPlotspexParmValues,ErrorReport);
  break;
 case PRINTCONFIGMENU:
  (void)sprintf(filename,PRINTCONFIG,ProjectDirectory);
  SaveEntriesToFile(filename,GetTotalNumPrinters(),
   GetPrinterNameKeyword,GetPrinterNameValues,ErrorReport);
  break;
 case PRINTINGMENU:
  (void)sprintf(filename,USERINIT,UserHomeDir);
  SaveEntriesToFile(filename,NUMPRINTFIELDS,GetPrintEntryKeyword,
   GetPrintEntryValue,ErrorReport);
  break;
 }
}

/****************  ConstructPlotspexFilenameFromFolderName()  ****************/

char *
ConstructPlotspexFilenameFromFolderName(char foldername[])
{
static char PlotspexFilename[MAX_FLNM_LNGTH];

 if (PlotspexFolderIsDefault(foldername))
/*
the default folder name ("default") gets mapped to the default plotspex folder, 
i.e.  "plotspex", to maintain backward compatibility
*/
   (void)sprintf(PlotspexFilename,PLOTSPEX,ProjectDirectory,get_proj_number(),
   PLOTSPEXFOLDERPREFIX,"");
 else
  (void)sprintf(PlotspexFilename,PLOTSPEX,ProjectDirectory,get_proj_number(),
  PLOTSPEXFOLDERPREFIX,foldername);
 return PlotspexFilename;
}

/*********************  GetCurrentFolderDeleted()  ****************************/

GetCurrentFolderDeleted()
{
 return CurrentFolderDeleted;
}

/*********************  SetMenuFieldInactive()  ****************************/

SetMenuFieldInactive(int which,int field,int value)
{
 if (menu_field[which][field])
  xv_set(menu_field[which][field],
   PANEL_INACTIVE,value,
  NULL);
}

/*********************  SetCurrentFolderDeleted()  ****************************/

SetCurrentFolderDeleted(int value)
/*
set relevant buttons and operations active or not, and change various messages, 
as function of whether the current folder has been deleted or not.
*/
{
 CurrentFolderDeleted=value;
 SetMenuFieldInactive(PLOTSPEXMENU,SETUPLIST,value);
 SetMenuFieldInactive(PLOTSPEXMENU,SHOWSETUP,value);
 SetMenuFieldInactive(PLOTSPEXMENU,DELETESPEC,value);
 SetMenuFieldInactive(PLOTSPEXMENU,DELETEFOLDER,value);
 SetMenuFieldInactive(PLOTSPEXMENU,REINITPLOTSPEX,value);
 SetMenuFieldInactive(PLOTSPEXMENU,ENABLEEDITS,value);
 SetMenuFieldInactive(PLOTSPEXMENU,DISABLEEDITS,value);
 xv_set(save_selns[PLOTSPEXMENU],
  PANEL_INACTIVE,value,
 NULL);
 SetPlotspexMsgValues(GetPanelListString(PLOTSPEXMENU,SETUPLIST),
                      GetFieldValue(PLOTSPEXMENU,SETUPLIST));
 SetControlButtonInactive(OP_CTL_SUBSET,value);
 SetSelectFolderDisplayLevel();
 SetCurrentFolderMessage();
 if (value) 
/*
just in case one of the setup windows has the Disp menu sub-category type menus
showing, hide them to prevent access, attempted selection and subsequent 
confusion
*/
  HideDispSubMenus();
}


/******************  SetSelectFolderDisplayLevel()  ****************************/

SetSelectFolderDisplayLevel()
{
 if (menu_field[PLOTSPEXMENU][SELECTFOLDER]) {
  if (GetCurrentFolderDeleted())
   xv_set(menu_field[PLOTSPEXMENU][SELECTFOLDER],
    PANEL_DISPLAY_LEVEL,PANEL_NONE,
   NULL);
  else
   xv_set(menu_field[PLOTSPEXMENU][SELECTFOLDER],
    PANEL_DISPLAY_LEVEL,PANEL_CURRENT,
   NULL);
 }
}

/*********************  SetCurrentFolderMessage()  ****************************/

SetCurrentFolderMessage()
{
 if (menu_field[FOLDERSMENU][CURRENTFOLDERMSG]) 
  set_menu_field_string(FOLDERSMENU,CURRENTFOLDERMSG,0,
   GetCurrentFolderDeleted()?"(none)": (char *)GetCurrentPlotspexFolderName());
}

/*********************  RemovePlotspexFolder()  ****************************/

RemovePlotspexFolder(char name[])
{
char message[MAX_SOCKET_BUFF_LNGTH];
int session;

 (void)unlink((char *)ConstructPlotspexFilenameFromFolderName(name));
/*
notify any other winds processes that this folder has been deleted
*/
 time_to_log();
 (void)fprintf(stderr,
  "%s notifies %d other sessions with PLOTSPEXFOLDERREMOVED\n",
  GetDisplayHost(),get_num_winds_sessions());
 (void)sprintf(message,"%d %s %s",
  PLOTSPEXFOLDERREMOVED,GetDisplayHost(),name);
 for (session=0; session<get_num_winds_sessions(); session++) {
  if (write_to_socket(INTERWINDS_TALK+session,message,strlen(message)) ==EBADWRITE) 
   perror("RemovePlotspexFolder write_to_socket");
 }
 SetCurrentPlotspexFolderName("");
 SetCurrentFolderDeleted(TRUE);
}

/*********************  SaveNewFolder()  ****************************/

SaveNewFolder(char name[])
{
int result,mode;
char foldername1[MAXFOLDERNAMELENGTH*2];
char *foldername;
static char msg[128];

 (void)strncpy(foldername1,name,MAXFOLDERNAMELENGTH-1);
 if (!(strlen(foldername1)))  {
/* 
null string: take no action
*/
  set_menu_field_string(FOLDERSMENU,STATUSMSG,0,
   "Can't Execute -- Please Enter a Folder Name");
  return;
 }
/* 
trim leading and trailing blanks and adjust text item value accordingly
*/
 foldername=(char *) strdup((char *)GetUnixCleanFilename(foldername1));
 xv_set(menu_field[FOLDERSMENU][ENTERFOLDERNAME],
        PANEL_VALUE,foldername,
        NULL);
/*
determine whether this is create new or save as mode
*/
 mode=xv_get(menu_field[FOLDERSMENU][SELECTCREATEMODE],PANEL_VALUE);
 if (mode==NEWCREATEMODE) {
  SetNumFileEntries(0);
  result=CopyPlotspexFile((char *)NULL,foldername);
 } else {
/*
mode is save as; if no current folder, no can do
*/
  if (GetCurrentFolderDeleted()) {
   result=ERROR;
  } else
   result=CopyPlotspexFile(GetCurrentPlotspexFolderName(),foldername);
 }
 if (result==OK) {
  if (mode==NEWCREATEMODE)
   (void)sprintf(msg,"Created New Folder '%s'\n",foldername);
  else
   (void)sprintf(msg, "Copied Folder '%s' to New Folder '%s'\n",
    GetCurrentPlotspexFolderName(),foldername);
  set_menu_field_string(FOLDERSMENU,STATUSMSG,0,msg);
  SetCurrentPlotspexFolderName(foldername);
  set_menu_field_string(FOLDERSMENU,CURRENTFOLDERMSG,0,
   GetCurrentPlotspexFolderName());
/***
  SetPlotspexFolderMenu();
***/
  SetNeedInitPlotspex(TRUE);
  SetNeedFillDisplays(TRUE);
  SetPasswordGiven(FALSE);
  SetCurrentFolderDeleted(FALSE);
 } else {
  if (mode==NEWCREATEMODE)
   (void)sprintf(msg,
    "Failed: Folder '%s' Already Exists\n",foldername);
  else
   (void)sprintf(msg,
    "Failed: '%s' Missing or '%s' Exists\n",
    GetCurrentPlotspexFolderName(),foldername);
  set_menu_field_string(FOLDERSMENU,STATUSMSG,0,msg);
 }
 time_to_log();
 (void)fprintf(stderr,"SaveNewFolder: %s\n",msg);
 (void)free((char *)foldername);
}

/*********************  CopyPlotspexFile()  ****************************/

CopyPlotspexFile(char srcname[], char destname[])
{
char *PlotspexFilename;
 
 if (srcname) {
  PlotspexFilename=ConstructPlotspexFilenameFromFolderName(srcname);
  if (!FileExists(PlotspexFilename)) {
/*
source file does not exist
*/
   time_to_log();
   (void)fprintf(stderr,
    "CopyPlotspexFile: source file %s does not exist\n",PlotspexFilename);
   return ERROR;
  } else 
/*
read in source file
*/
   (void)ReadFileIntoEntryArray(PlotspexFilename,ErrorReport);
 }
 PlotspexFilename=ConstructPlotspexFilenameFromFolderName(destname);
 if (FileExists(PlotspexFilename)) {
/*
destination file already exists
*/
  time_to_log();
  (void)fprintf(stderr,
    "CopyPlotspexFile: destination file %s already exists\n",PlotspexFilename);
  return ERROR;
 } else {
/*
update folder password entry and write file out
*/
  ChangePasswordEntry((char *)xv_get(menu_field[FOLDERSMENU][ENTERPASSWORD],
   PANEL_VALUE));
  if (WriteEntriesToFile(PlotspexFilename)==ERROR)
   (void)fprintf(stderr,
    "WARNING: CopyPlotspexFile couldn't write out file %s\n",PlotspexFilename);
 }
 return OK;
}

/*********************  ChangePasswordEntry()  ****************************/

ChangePasswordEntry(char password[])
{
int entry;
char passwordstring[128];

 (void)sprintf(passwordstring,"%s %s",
  strlen(password)?password:NOPASSWORD,ENDARGSWORD);
 if ((entry=FindEntryWithKeyword(PASSWORDENTRY))!=ERROR) {
  SetEntryString(entry,PASSWORDENTRY,passwordstring);
 } else {
  AppendEntry(PASSWORDENTRY,passwordstring);
 }
}

/*********************  SetPlotspexFolderMenu()  ****************************/

SetPlotspexFolderMenu()
/*
re-generate the plotspex folder menu after presumably reading in the filenames
upon user click of menu button
*/
{
int item,savevalue;

 savevalue=0;
/*
get all currently available names, init menu strings to nothing
*/
 SetPlotspexFolderNames();
 xv_set(menu_field[PLOTSPEXMENU][SELECTFOLDER],
  PANEL_CHOICE_STRINGS," ",NULL,
 NULL);
/*
set up each menu item with next name
*/
 for (item=0; item<GetNumPlotspexFolders(); item++) {
  set_menu_field_string(PLOTSPEXMENU,SELECTFOLDER,
   item,GetPlotspexFolderName(item));
  if (!(strcmp((char *)GetCurrentPlotspexFolderName(),
   GetPlotspexFolderName(item))))
   savevalue=item;
 }
 xv_set(menu_field[PLOTSPEXMENU][SELECTFOLDER],
  PANEL_INACTIVE,!GetNumPlotspexFolders(),
 NULL);
 xv_set(menu_field[PLOTSPEXMENU][SELECTFOLDER],
  PANEL_VALUE,savevalue,
  NULL);
 SetSelectFolderDisplayLevel();
 SetFolderEditItems();
}

/**********************  InitPlotspexParmValue()  ****************************/

InitPlotspexParmValue(int field, char string[])
/*
*/
{
char *entryvalue;

 entryvalue=&string[strlen(GetPlotspexParmKeyword(field))+1];
 SetCurrentPlotspexFolderName(entryvalue);
}

/**********************   GetSavedPlotspexFolderName()  ************************/

GetSavedPlotspexFolderName() 
{
 (void)sprintf(filename,USERINIT,UserHomeDir);
 RestoreEntriesFromFile(filename,NUMPLOTSPEXSAVEFIELDS,
  GetPlotspexParmKeyword,InitPlotspexParmValue,ErrorReport);

}

/******************  InitPlotspexFolderMenuValue()  ****************************/

InitPlotspexFolderMenuValue()
{
int value;

 SetPlotspexFolderMenu();
 for (value=0; value<ItemsPerField(PLOTSPEXMENU,SELECTFOLDER); value++) {
  if (!(strcmp((char *)GetCurrentPlotspexFolderName(),
    (char *)xv_get(menu_field[PLOTSPEXMENU][SELECTFOLDER],
     PANEL_CHOICE_STRING,value)))) {
   xv_set(menu_field[PLOTSPEXMENU][SELECTFOLDER],PANEL_VALUE,value,NULL);
  }
 }
}

/**********************  InitPrintFieldValue()  ****************************/

InitPrintFieldValue(int field, char string[])
/*
given which field of the PRINTINGMENU menu and a string containing a keyword 
and a value, proceed to initialize the panel-item's value by extracting the
value from the string and comparing it to each choice string in the panel item
until a match is found: that ordinal position is to become the panel item's
value. If no match is found, the panel item's value is left unchanged.

If a match is found, also initialize the internal notion of current print
parameters.
*/
{
int value;
char *entryvalue;

 entryvalue=&string[strlen(PrintEntryKeyword[field])+1];
 for (value=0; value<ItemsPerField(PRINTINGMENU,field); value++) {
  if (!(strcmp(entryvalue,
    (char *)xv_get(menu_field[PRINTINGMENU][field],PANEL_CHOICE_STRING,value)))) {
   xv_set(menu_field[PRINTINGMENU][field],PANEL_VALUE,value,NULL);
   switch (field) {
    case IMAGEMODE:
     SetCurrentImageMode(value);
     break;
    case IMAGEPRINTER:
     SetCurrentImagePrinter(value);
     break;
    case TEXTPRINTER:
     SetCurrentTextPrinter(value);
     break;
    case PRINTTOFILE:
     break;
    case ORIENTATION:
     break;
    case BACKGROUND:
     break;
    case NUMPERPAGE:
     break;
    case NUMLISTCOLS:
     break;
    case LASTPGFIRST:
     break;
   }
   return;
  }
 }
}

/********************  GetPrinterNameKeyword()  ********************************/
 
char *
GetPrinterNameKeyword(int printer)
/*
return the printer'th string from the set of PRINTCONFIGMENU menu keywords.
*/
{
  return (char *)GetPrinterName(printer);
}

/********************  GetPrinterNameValues()  ********************************/
 
char *
GetPrinterNameValues(int printer)  
/*
given which printer in PRINTCONFIGMENU menu, return a character version of its 
associated values.
*/
{
static char charvalue[MAXNAMELENGTH*NUMPRINTCONFIGSAVEFIELDS];

 (void)sprintf(charvalue,"%s %s %s"
  ,GetPrinterTypeName(GetPrinterType(printer))
  ,GetImageModeName(GetPrinterMode(printer))
  ,GetPrinterEnabled(printer)?ENABLEDNAME:DISABLEDNAME
  ); 
 return charvalue; 
} 

/********************  GetPlotspexParmValues()  ********************************/
 
/*ARGSUSED*/
char *
GetPlotspexParmValues(int which)  
/*
given which parameter in PLOTSPEXMENU menu, return a character version of its 
associated value.
*/
{
int value;
static char charvalue[MAX_FLNM_LNGTH];

 value=xv_get(menu_field[PLOTSPEXMENU][SELECTFOLDER],PANEL_VALUE);
 (void)sprintf(charvalue,"%s",xv_get(menu_field[PLOTSPEXMENU][SELECTFOLDER],
  PANEL_CHOICE_STRING,value));
 return strlen(charvalue)?charvalue:PLOTSPEXFOLDERPREFIX;
} 

/**********************  GetPrintEntryKeyword()  ****************************/

char *
GetPrintEntryKeyword(int entry)
/*
return the entry'th string from the set of PRINTINGMENU menu keywords.
*/
{
 return PrintEntryKeyword[entry];
}

/************************  GetPrintEntryValue()  ****************************/

char *
GetPrintEntryValue(int field)
/*
given the field in the PRINTINGMENU menu, return a character version of its value.
*/
{
int value;
static char charvalue[MAXPRINTERNAMELENGTH];

 value=xv_get(menu_field[PRINTINGMENU][field],PANEL_VALUE);
 (void)sprintf(charvalue,"%s",xv_get(menu_field[PRINTINGMENU][field],
  PANEL_CHOICE_STRING,value));
 return strlen(charvalue)?charvalue:"none";
}

/**********************  CreatePrintConfigMenus()  *****************************/

CreatePrintConfigMenus()
{
int item;

 if (UserPrintersMenu!=0)
  menu_destroy(UserPrintersMenu);
 if (DisabledPrintersMenu!=0)
  menu_destroy(DisabledPrintersMenu);
 if (EnabledPrintersMenu!=0)
  menu_destroy(EnabledPrintersMenu);
 UserPrintersMenu=xv_create((int)NULL,MENU,
  MENU_TITLE_ITEM, "Active Printer Names",
 NULL);
 EnabledPrintersMenu=xv_create((int)NULL,MENU,
  MENU_TITLE_ITEM, "Active Printer Names",
 NULL);
 DisabledPrintersMenu=xv_create((int)NULL,MENU,
  MENU_TITLE_ITEM, "Inactive Printer Names",
 NULL);
 for (item=0; item<ItemsPerField(PRINTCONFIGMENU,MODPRINTER); item++) 
  set_menu_field_string(PRINTCONFIGMENU,MODPRINTER,item,GetEnabledPrinterName(item));
 for (item=0; item<ItemsPerField(PRINTCONFIGMENU,DELPRINTER); item++) 
  set_menu_field_string(PRINTCONFIGMENU,DELPRINTER,item,GetEnabledPrinterName(item));
 for (item=0; item<ItemsPerField(PRINTCONFIGMENU,ADDPRINTER); item++) 
  set_menu_field_string(PRINTCONFIGMENU,ADDPRINTER,item,
  GetDisabledPrinterName(item));
 AttachPrintConfigMenus();
}

/*************************  GetCreateModeName()  ****************************/

char *
GetCreateModeName(int which)
/*
return the which'th name in list of CreateModeNames options
*/
{
 return CreateModeNames[which];
}

/*************************  GetShowSetupName()  ****************************/

char *
GetShowSetupName(int which)
/*
return the which'th name in list of ShowSetup options
*/
{
 return ShowSetupName[which];
}

/*****************************************************************************/

ItemIsSelectFolderButton(Panel_item item)
{
 return item==menu_field[PLOTSPEXMENU][SELECTFOLDER]?TRUE:FALSE;
}

/*************************  GetNeedFillDisplays()  ****************************/

GetNeedFillDisplays()
{
 return NeedFillDisplays;
}

/*************************  SetNeedFillDisplays()  ****************************/

SetNeedFillDisplays(int value)
{
 NeedFillDisplays=value;
}

/*************************  CreateFolderButtons()  ****************************/

CreateFolderButtons()
{
 set_menu_field_string(FOLDERSMENU,STATUSMSG,0,
  "Select Create Mode, Enter Name, and Click Apply To Execute");
 SetCurrentFolderMessage();
 set_menu_field_string(FOLDERSMENU,ENTERFOLDERNAME,0,"");
 set_menu_field_string(FOLDERSMENU,ENTERPASSWORD,0,"");
 if (xv_get(panel_menu[FOLDERSMENU],XV_SHOW))
  xv_set(panel_menu[FOLDERSMENU],
  PANEL_CARET_ITEM,menu_field[FOLDERSMENU][ENTERFOLDERNAME],NULL);
}

/****************************************************************************/

SetSavedValue(int value)
{
 SavedValue=value==ERROR?0:value;
}

/****************************************************************************/

GetSavedValue()
{
 return SavedValue;
}

/*************************  CreatePlotspexChoices()  ****************************/

CreatePlotspexChoices()
{
int session,field,item,setup;
static int firsttime=TRUE;
static char PanelListString[256];
static char DateSaved[64];
/*
remember value of the item being deleted so as to restore a "close" 
approximation upon re-creation
*/
 SetSavedValue(GetFieldValue(PLOTSPEXMENU,SETUPLIST));
 DestroyMenuFieldButton(PLOTSPEXMENU,SETUPLIST);
 CreateMenuFieldButton(PLOTSPEXMENU,SETUPLIST);
 if (num_valid_setups()) {
  for (item=0; item<ItemsPerField(PLOTSPEXMENU,SETUPLIST); item++) {
   setup=get_valid_setup(item);
/*
note that length to be used in LeftJustify() must match that used to read in 
the string, as done in GetSetupListTitle()
*/
   LeftJustify(GetSetupDescription(setup),PanelListString,
    GetSetupListTitleLength());
   LeftJustify(GetSubType(setup),&PanelListString[strlen(PanelListString)],16);
   (void)sprintf(DateSaved,"%s (Flt. %s)",GetSetupDateSaved(setup),
    GetSetupFlightSaved(setup));
   LeftJustify(DateSaved,&PanelListString[strlen(PanelListString)],23);
   set_menu_field_string(PLOTSPEXMENU,SETUPLIST,item,PanelListString);
  }
 } else {
  set_menu_field_string(PLOTSPEXMENU,SETUPLIST,0,"(No Setups)");
 }
 for (field=0; field<FieldsPerMenu(PLOTSPEXMENU); field++)
  SetMenuFieldAttrs(PLOTSPEXMENU,field);
 SizePanelMenu(PLOTSPEXMENU);
 if (GetSavedValue()>=ItemsPerField(PLOTSPEXMENU,SETUPLIST)) {
  SetSavedValue(ItemsPerField(PLOTSPEXMENU,SETUPLIST)-1);
 }
 UpdatePlotspexFields((char *)GetSetupListTitle(GetSavedValue()),
  GetSavedValue());
 if (!firsttime) {
/*
no action needed first time through...this is not an update, it is merely
an initialization.
*/
  if (GetResponseToNetRequest()) {
/*
this is in response to some other host updating its plotspex file, as opposed
to this host doing it. No add'l network notification needed.
*/
   SetResponseToNetRequest(FALSE);
  } else {
   char message[MAX_SOCKET_BUFF_LNGTH];
   if (GetFolderChanged()) {
/*
notify any other winds processes that it's time to update their memory map
notions of the plotspex file
*/
    time_to_log();
    (void)fprintf(stderr,"%s notifies %d other sessions with INITPLOTSPEXFILE\n",
     GetDisplayHost(),get_num_winds_sessions());
    (void)sprintf(message,"%d %s %s",
     INITPLOTSPEXFILE,GetDisplayHost(),GetPlotspexFilename());
    for (session=0; session<get_num_winds_sessions(); session++) {
     if (write_to_socket(INTERWINDS_TALK+session,message,strlen(message)) ==EBADWRITE) 
      perror("RemovePlotspexFolder write_to_socket");
    }
    SetFolderChanged(FALSE);
   }
  }
 } else {
  firsttime=FALSE;
 }
 if (GetNeedFillDisplays()) {
  SetNeedFillDisplays(FALSE);
  fill_displays(0);
 }
}

/********************  GetFolderChanged()  ******************************/

GetFolderChanged()
{
 return FolderChanged;
}

/********************  SetFolderChanged()  ******************************/

SetFolderChanged(value)
int value;
{
 FolderChanged=value;
}

/********************  SetPasswordGiven()  ******************************/

SetPasswordGiven(value)
int value;
{
 PasswordGiven=value;
}

/********************  GetPasswordGiven()  ******************************/

GetPasswordGiven()
{
 return PasswordGiven;
}

/********************  CreatePrintingChoices()  ******************************/

CreatePrintingChoices()
{
int item,savevalue;
char printername[MAXPRINTERNAMELENGTH];
char string[MAXPRINTERNAMELENGTH*2];

 savevalue=GetFieldValue(PRINTINGMENU,IMAGEPRINTER);
 (void)sprintf(printername,"%s",
  (char *)xv_get(menu_field[PRINTINGMENU][IMAGEPRINTER],
   PANEL_CHOICE_STRING,savevalue));
 DestroyMenuFieldButton(PRINTINGMENU,IMAGEPRINTER);
 CreateMenuFieldButton(PRINTINGMENU,IMAGEPRINTER);
 SetMenuFieldAttrs(PRINTINGMENU,IMAGEPRINTER);
 for (item=0; item<ItemsPerField(PRINTINGMENU,IMAGEPRINTER); item++) 
  set_menu_field_string(PRINTINGMENU,IMAGEPRINTER,item,GetImagePrinterName(item));
/***
 xv_set(menu_field[PRINTINGMENU][TEXTPRINTER],
         PANEL_CHOICE_STRINGS,(char *)NULL,NULL,
        NULL);
 for (item=0; item<ItemsPerField(PRINTINGMENU,TEXTPRINTER); item++) 
  set_menu_field_string(PRINTINGMENU,TEXTPRINTER,item,GetTextPrinterName(item));
***/
/*
reset value of printing menu buttons from saved names
*/
 (void)sprintf(string,"%s %s",PrintEntryKeyword[IMAGEPRINTER],printername);
 InitPrintFieldValue(IMAGEPRINTER,string);
}

/****************************  FILL_MENU()  **********************************/

fill_menu(int which)
/* fill all menu fields with appropriate strings
*/
{
int field,item;

/* 
get a rough idea of just how many fields are in the menu, before the
number of valid items are known.  "menu fields" reflect the various 
user-level fields. Set up save-file arrangements, as needed.
*/
 switch (which) {
 case FOLDERSMENU:
 case PLOTSPEXMENU:
 case PRINTCONFIGMENU:
 case PRINTINGMENU:
  num_menu_fields[which]=FieldsPerMenu(which);
  max_fields_per_page[which]=FieldsPerMenu(which);
  break;
 }
/*
create each field of the menu and initialize its choice strings
*/
  create_menu_field(which,NONE);
  switch (which) {
/*
create any PANEL or MENU menus that may need to be re-generated during run-time:
*/
   case PRINTINGMENU:
    CreatePrintingChoices();
    break;
   case PRINTCONFIGMENU:
    CreatePrintConfigMenus();
    break;
   case PLOTSPEXMENU:
    CreatePlotspexChoices();
    break;
   case FOLDERSMENU:
    CreateFolderButtons();
    break;
  }
  for (field=0; field<FieldsPerMenu(which); field++) {
   for (item=0; item<ItemsPerField(which,field); item++) {
/*
...and these only have calls made if they do NOT have menus expected to change
during run-time...:
*/
    switch (which) {
     case FOLDERSMENU:
      switch (field) {
       case SELECTCREATEMODE:
        set_menu_field_string(FOLDERSMENU,SELECTCREATEMODE,item,
         GetCreateModeName(item));
       case ENTERPASSWORD:
       case ENTERFOLDERNAME:
       case STATUSMSG:
       case CURRENTFOLDERMSG:
        break;
      }
      break;
     case PLOTSPEXMENU:
      switch (field) {
       case NUMSETUPSMSG:
        break;
       case SELECTFOLDER:
        break;
       case SHOWSETUP:
        set_menu_field_string(which,field,item,GetShowSetupName(item));
        break;
       case REINITPLOTSPEX:
        set_menu_field_string(which,field,item,"Re-init Folder");
        break;
       case DELETEFOLDER:
        set_menu_field_string(which,field,item,"Delete Folder");
        break;
       case DELETESPEC:
        set_menu_field_string(which,field,item,"Delete Setup");
        break;
       case ENABLEEDITS:
        break;
       case DISABLEEDITS:
        break;
       case SETUPLIST:
        break;
      }
      break;
     case PRINTCONFIGMENU:
      switch (field) {
       case ADDPRINTER:
       case DELPRINTER:
       case MODPRINTER:
        break;
       case CONFIGPENDING:
       case PRINTERNAME:
        break;
       case PRINTERTYPE:
        set_menu_field_string(which,field,item,GetPrinterTypeName(item));
        break;
       case PRINTERMODE:
        set_menu_field_string(which,field,item,GetImageModeName(item));
        break;
       case CONFIGAPPLY:
        break;
      }
      break;
     case PRINTINGMENU:
      switch (field) {
       case IMAGEPRINTER:
       case TEXTPRINTER:
        break;
       case PRINTTOFILE:
        set_menu_field_string(which,field,item, "Print To File");
        break;
       case IMAGEMODE:
        set_menu_field_string(which,field,item,GetImageModeName(item));
        break;
       case ORIENTATION:
        set_menu_field_string(which,field,item,GetOrientationName(item));
        break;
       case BACKGROUND:
        set_menu_field_string(which,field,item,GetBackgroundName(item));
        break;
       case NUMPERPAGE:
        set_menu_field_string(which,field,item,GetNumPerPageName(item));
        break;
       case NUMLISTCOLS:
        set_menu_field_string(which,field,item,GetNumListColName(item));
        break;
       case LASTPGFIRST:
        set_menu_field_string(which,field,item, "Last Page First");
        break;
     }
    }
   }
  }
/*
initialize the values of each field in various menus
*/
  switch (which) {
   case PRINTINGMENU:
    (void)sprintf(filename,USERINIT,UserHomeDir);
     RestoreEntriesFromFile(filename,NUMPRINTFIELDS,
     GetPrintEntryKeyword,InitPrintFieldValue,ErrorReport);
    break;
   case PLOTSPEXMENU:
    InitPlotspexFolderMenuValue();
    break;
   }
}

/***************************  FieldsPerMenu()  ********************************/

FieldsPerMenu(int which)
{
 switch (which) {
  case FOLDERSMENU:
   return NUMFOLDERSFIELDS;
  case PLOTSPEXMENU:
   return NUMPLOTSPEXFIELDS;
  case PRINTCONFIGMENU:
   return NUMPRINTCONFIGFIELDS ;
  case PRINTINGMENU:
   return NUMPRINTFIELDS ;
 }
 return 0;
}

/***************************  ItemsPerField()  ********************************/

ItemsPerField(int which,int field)
{
 switch (which){
  case FOLDERSMENU:
   switch (field) {
    case SELECTCREATEMODE:
     return NUMCREATEMODES;
    case ENTERPASSWORD:
    case ENTERFOLDERNAME:
     return 1;			/* simple text input item */
    case CURRENTFOLDERMSG:
    case STATUSMSG:
     return 1;			/* simple message item */
   }
   break;
  case PLOTSPEXMENU:
   switch (field) {
    case NUMSETUPSMSG:
     return 1;  /* simple message button */
    case SELECTFOLDER:
     return GetNumPlotspexFolders();
    case SHOWSETUP:
     return NUM_DISPLAY_QUADRANTS;
    case DISABLEEDITS:
    case ENABLEEDITS:
     return 1; /* simple operation and text buttons */
    case REINITPLOTSPEX:
    case DELETEFOLDER:
    case DELETESPEC:
     return 1;  /* simple operation button */
    case SETUPLIST:
     return num_valid_setups()?num_valid_setups():1;
   }
   break;
  case PRINTCONFIGMENU:
   switch (field) {
    case CONFIGPENDING:
     return 1;  /* name of current mode of pending operation */
    case PRINTERNAME:
     return 1;  /* name of printer currently being edited */
    case PRINTERTYPE:
     return NUMPRINTERTYPES;
    case PRINTERMODE:
     return NUMIMAGEMODES;
    case MODPRINTER:
     return GetNumEnabledPrinters();
    case DELPRINTER:
     return GetNumEnabledPrinters();
    case ADDPRINTER:
     return GetNumDisabledPrinters();
    case CONFIGAPPLY:
     return 1;	/* simple operation button */
   }
  case PRINTINGMENU:
   switch (field) {
    case PRINTTOFILE:
     return 1;  /* toggle on or off item */
    case ORIENTATION:
     return NUMORIENTATIONS;
    case BACKGROUND:
     return NUMBKGNDVALUES;
    case NUMPERPAGE:
     return NUMPERPGVALUES;
    case IMAGEMODE:
     return NUMIMAGEMODES;
    case NUMLISTCOLS:
     return NUMLISTCOLVALUES;
    case LASTPGFIRST:
     return 1;  /* toggle on or off item */
    case IMAGEPRINTER:
     return GetNumImagePrinters();
    case TEXTPRINTER:
     return GetNumTextPrinters();
   }
 }
 return 0;
}

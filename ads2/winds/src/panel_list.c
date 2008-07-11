/*
panel_list.c: provide widgets, operations and management for user selection
and browsing of a scrolling list of variable names/attributes/statistics/etc..
*/
#include <stdio.h>
#include <unistd.h>
#include <xview/xview.h>
#include <xview/panel.h>

#include <help.h>
#include <constants.h>
#include <display.h>
#include <errors.h>
#include <colors.h>
#include <proto.h>

/*
all the widgets and associated internal mgmt variables
*/
/*
parent panel, is it currently visible, its event window:
*/
static Panel ScrollPanel;
static int ScrollPanelShowing=FALSE;
static Window ScrollEventWindow;
/*
current list: title, info storage, extract variable name, number of entries,
number of selected entries, is it exclusive choice selection or not, is the
list filled from a text file, if so which one:
*/
static Panel_item PanelTitleItem;
char *GetPanelListTitle();
static char CurrentFilteredListTitle[256]="";
static char CustomListTitle[256]="";
static Panel_list_row_values *ScrollList=0;
char *GetScrollListVarname();
static int NumScrollListEntries=0;
static int ListFromVarFilter=TRUE;
static int ListFromTextFile=FALSE;
static int ListFromStrings=FALSE;
static char CurrentTextFile[256];
static int NeedListRefresh=FALSE;
/*
utility function buttons: dismiss menu, print list window, apply pending
selection, delete selection:
*/
static Panel_item DismissButton;
static Panel_item PrintWindowButton;
static FILE *PrintWindowFD;
static void (*CurrentHeaderPrintFunc)();
static Panel_item ApplyButton;
static Panel_item DeleteButton;
/*
list is "filtered" according to one of three domains: Type, Category, or
Keyword. User selects which domain is to be used and then may select the
specific type or category via button, or indicate specific keyword by typing
it in. Full list is then filtered by user indication. 

Here then are the components of the Type domain, to include the top level
button containing specific types (as per strings indicated) and the associated
functions that filter the full list according to type:
*/
static Panel_item TypeButton;
int  ShowTypeButton();
static char *TypeString[]={ "All","Raw","Derived","Analog","Digital",NULL };

int   SortByType(),
       SortByAll(),SortByRaw(),SortByDerived(),
       SortByAnalog(),SortByDigital();
int (*ShowTypeFunc[])()={
SortByAll,SortByRaw,SortByDerived,SortByAnalog,SortByDigital
};
/*
components of the Category domain, to include the top level button containing 
specific categories (as per strings obtained from the VarDB) and the associated
function that filters the full list according to a keyword match with
selected category against (the already known) variable category placement:
*/
int  ShowCategoryButton();
int   SortByCategory();
static Panel_item CategoryButton;
/*
components of the Keyword domain, to include the top level text input item 
(that gets desired keyword from user) and the associated function that filter 
the full list according to keyword match against variable names and titles:
*/
int  ShowKeywordButton();
Panel_setting SortByKeyword();
static Panel_item KeywordButton;
static char UserKeyword[256];
/*
top level domain selection widget, its associated strings and
functions that show appropriate 2nd level button set as per user selection:
*/
static Panel_item DomainButton;
int ShowDomain();
static char *DomainString[]={ "Type","Category","Keyword", NULL };

int (*ShowDomainFunc[])()={
 ShowTypeButton,ShowCategoryButton,ShowKeywordButton
};
/*
variable menu:
*/
static Panel_list_item VarPanelList;
static int InstallValue=ERROR;
static Panel_item HistoryButton;
static char HistoryVarname[NAMLEN];
static Panel_item TimeFrameMessage;
static char TimeFrameString[256];
static Panel_item StartTimeButton;
static Panel_item EndTimeButton;
void ChgHistoryTime();
static int NeedItemValueChg=FALSE;
static int NewValue;
static char *Strings=(char *)NULL;
static int NumStrings=0;
/*
statistics functions: save current selections to file, autoprint, autoprint
interval, when next flush will occur, select all in current list, reset
selections from file; menu of displayable stats flushes:
*/
static Panel_list_item StatsPanelList;
static Panel_item SaveSelectionsButton;
static Panel_item StatsAutoPrintButton;
static Panel_item StatsIntervalButton;
static Panel_item StatsFlushMenuButton;
static Panel_item NextFlushMessage;
static Panel_item SelectAllButton;
static Panel_item ResetFromFileButton;
static Menu StatsFlushMenu=0;
#define MAXFLUSHSAVES 32
static char FlushMenuString[MAXFLUSHSAVES][64];
static int *StatsSeln;
static char **StatsSelnNames;
static int NumStatsSelns=0;
/*
new setup menu functions: designate plot type, dep't vs. indep't variable
type, quadrant in which to install:
*/
static Panel_list_item NewSetupPanelList;
static Panel_item PlotTypeButton;
char *GetPlotTypeChoiceString();
static Panel_item VarTypeButton;
static Panel_item CurrentQuadButton;
static int CurrentSetupQuadrant=ERROR;
void SetPlotTypeProc();
void SetVarTypeProc();
void SetCurrQuadProc();
/*
attributes menu: button to call up vared process to change attributes:
*/
static Panel_list_item AttrsPanelList;
static Panel_item ChgAttrsButton;
void PrintAttributesHeader();
/*
keep track of current domain button, panel list, sort button, its value, and
associated sorting filter function:
*/
static Panel_item CurrentDomainButton;
static Panel_list_item CurrentPanelList;
static int CurrentSortButton=ERROR;
static int CurrentSortButtonValue=0;
static int (*CurrentFilter)();

/***********************  Parent Panel *************************************/

ShowScrollPanel(panellist,quadrant,event,title)
Panel_item panellist;
int quadrant;
Event *event;
char *title;
/*
make scroll panel visible: 
If this is a "change" operation, then there is a currently 
highlighted variable name; search for that variable name in the 
list and select it; if it is not in the list then select the first name in 
list that matches the highlighted name up to # of chars in that name (i.e.,
with DPTC highlighted but raw list showing, DPT would be selected in list);
if no matches or it is an "add" operation, select name at top of list instead.
Adjust overall layout as per current size of WINDS window. Set mouse at upper
left corner of panel, and show the panel. Change help images accordingly. 
Remember position of cursor when panel was invoked so it can be returned 
there on completion.
*/
{
/*
list may have been last filled from other than a filter; reset to most recent 
list obtained from filtering operation to preclude mismatch on next showing of, 
say, the variable selector menu (don't want history/stats flush/text file 
contents to still be showing).
*/
 SetListFromVarFilter(TRUE);
 FillScrollList(CurrentFilter,CurrentFilteredListTitle);
 SetItemExclusive(CurrentSortButton,TRUE);
 if (CurrentSortButton!=KeywordButton)
  SetButtonValue(CurrentSortButton,CurrentSortButtonValue);
/*
select variable entry as apropos
*/
 SetPanelSelections(panellist);
/*
layout is function of which quadrant
*/
 SetOverallLayout(panellist,quadrant);
 if (panellist!=NewSetupPanelList)
/*
new setup module calls this menu from an Xview menu, which does its own
cursor position mgmt; don't confuse things here by doing more of our own
*/
  (void)xv_set(ScrollPanel,
   WIN_MOUSE_XY,0,0, 
   NULL);
 ShowPanelItem(ScrollPanel);
 ShowPanelItem(panellist);
 SetPanelTitle(ScrollPanel,PanelTitleItem,title);
 ScrollPanelShowing=TRUE;
 if (panellist==StatsPanelList) {
/*
stats list has two keyboard input items; give it the keyboard focus and
place caret on first input item
*/
  win_set_kbd_focus(ScrollEventWindow,xv_get(ScrollEventWindow,XV_XID));
  xv_set(ScrollPanel,PANEL_CARET_ITEM,StatsIntervalButton,NULL);
 }
 if (panellist!=NewSetupPanelList)
/*
no need to do this for new setup, already done by Xview menu used to get it up
*/
  RememberCursorPosn(event_window(event),event_x(event),event_y(event));
 else
/*
position of certain buttons are a function of apply button width; thus, 
place them *after* this label has been updated, which occurs in function
SetPanelSelections()
*/
  PositionNewSetupButtons();
/*
reminder to re-highlight underlaying area after menu dismissal
*/
 SetNeedRehilite(quadrant,TRUE);
}

/**************************************************************************/

DismissScrollPanel()
/*
dismiss and hide the panel and call post-menu cleanup routines
*/
{
 MenuDismissed();
 HideScrollPanel();
 RemoveCurrentPanelMenu();
 if (CurrentPanelList==NewSetupPanelList) {
  fini_new_setup(GetCurrentSetupQuadrant());
 } else if (CurrentPanelList==StatsPanelList) {
/*
coordinate with Control Panel on value of button used to call up stats menu
*/
  SetStatsButtonValue(FALSE);
 }
 if (!GetListFromVarFilter()) {
  SetButtonValue(CurrentSortButton,CurrentSortButtonValue);
 }
}

/**************************************************************************/

DismissSansSelection()
/*
dismiss the panel w/o making a selection
*/
{
 SetSelectionMade(FALSE);
 DismissScrollPanel();
}

/**************************************************************************/

SetScrollPanelShowing(show)
int show;
{
 ScrollPanelShowing=show;
}

/**************************************************************************/

GetScrollPanelShowing(check)
int check;
/*
if the ScrollPanel is indeed up, and caller wishes it done (check is TRUE),
AND there is an item in need of adjustment as determined in its notify proc
(where it is unsafe to change it, and thus deferred to this mechanism), 
proceed to change that item's value.

...regardless, return TRUE if panel is currently visible, else return FALSE
*/
{
 static int first=TRUE;

 if (first && CurrentPanelList) {
  ShowPanelItem(CurrentPanelList);
  first=FALSE;
 }
 if (check) {
  if (GetNeedRefreshFolderList()) {
   RefreshFolderList();
   SetNeedRefreshFolderList(FALSE);
  }
  if (GetNeedInitPlotspex()) {
   SetNeedInitPlotspex(FALSE);
   SetNeedRefreshFolderList(TRUE);
   init_plotspex();
   if (ScrollPanelShowing)
    update_displays(GetWINDSElapsedTime());
  }
  if (ScrollPanelShowing) {
   if (CurrentPanelList==VarPanelList && !post_processing_mode()) {
   UpdateTimeFrameMessage();
   }
   if (GetNeedItemValueChg())
    ChgItemValue();
   if (NeedListRefresh)
    ShowSelectedItemPage();
  }
 }
 return ScrollPanelShowing;
}

/**************************************************************************/

HideScrollPanel()
/*
hide the panel
*/
{
 ScrollPanelShowing=FALSE;
 HidePanelItem(ScrollPanel);
}

/**************************************************************************/

GetPanelIsScrollPanel(panel)
Panel panel;
/*
if argument panel is the ScrollPanel, return TRUE; else, return FALSE
*/
{
 return panel==ScrollPanel;
}

/**************************************************************************/

ResizeScrollPanel(frame)
Frame frame;
/*
destroy and then recreate ScrollPanel, whose parent is frame. This will
facilitate resizing it, since it is assumed that new geometries and fonts
are already set.
*/
{
 int size=GetScrollPanelWidth();
 DestroyPanel(ScrollPanel);
 CreateScrollPanel(frame,size);
}

/**************************************************************************/

CreateScrollPanel(frame,width)
Frame frame;
int width;
/*
create and layout panel and its buttons; initialize choice strings for buttons;
set the "type" domain choices as initial filtering options and sort the list
sans any filter at all, i.e. show all variable names. Set up the event handler.
*/
{
 ScrollPanel=xv_create(frame,PANEL,
  XV_FONT, GetStandardFont(),
  PANEL_REPAINT_PROC, panel_repaint,
  XV_HELP_DATA,"WINDS:PANEL_LIST_HELP",
  XV_SHOW, FALSE,
 NULL);
 if (width!=ERROR)
/*
NOTE: KLUGE to handle mwm behavior anomalies:

on certain window mgrs, user-explicit placement at widget appearance time
changes (for some unknown reason) the width to an very small number. This
would cause the adjustment done by AdjustNrows (q.v.) to loop possible forever,
trying to divide buttons into enough rows to fit inside that prescribed width.
Preclude this anomaly as follows: if width argument given here is not ERROR, 
then it is a legitimate value from a previous iteration BEFORE this brain-dead
anomaly occurred. Use it instead of that brain-damaged width to remind 
everyone of where things ought to be.

This KLUGE has NOT been comprehensively tested to determine its effect in
other scenarios (7/18/96). My best guess however is that it will be benign.
Yeah, right.
*/
  xv_set(ScrollPanel,XV_WIDTH,width,NULL);

 ScrollEventWindow=canvas_paint_window(ScrollPanel);
 xv_set(ScrollEventWindow,
#include <event_attr.h>
  NULL);

 PanelTitleItem=xv_create(ScrollPanel,PANEL_MESSAGE,
  PANEL_LABEL_STRING,"No Title Yet",
 NULL);

 DismissButton=xv_create(ScrollPanel,PANEL_BUTTON,
  PANEL_NEXT_ROW, -1,
  PANEL_LABEL_STRING,"Dismiss",
  PANEL_NOTIFY_PROC, DismissSansSelection,
 NULL);
/*
position of PrintWindowButton varies during run-time
*/
 PrintWindowButton=xv_create(ScrollPanel,PANEL_BUTTON,
  PANEL_LABEL_STRING,"PrintWindow",
  PANEL_NOTIFY_PROC, PrintWindowProc,
 NULL);

 ChgAttrsButton=xv_create(ScrollPanel,PANEL_BUTTON,
  PANEL_LABEL_STRING,"ModifyVars/Attrs",
  PANEL_NOTIFY_PROC, ModifyAttributes,
 NULL);
 PositionToRightOfButton(ChgAttrsButton,DismissButton);

 ApplyButton=xv_create(ScrollPanel,PANEL_BUTTON,
  PANEL_LABEL_STRING,"Apply",
  PANEL_NOTIFY_PROC, InstallSelection,
 NULL);
 PositionToRightOfButton(ApplyButton,DismissButton);

 DeleteButton=xv_create(ScrollPanel,PANEL_BUTTON,
  PANEL_LABEL_STRING,"Delete",
  PANEL_NOTIFY_PROC, DeleteSelection,
 NULL);
 PositionToRightOfButton(DeleteButton,ApplyButton);
/*
Create and position "current level" objects in new setup panel
*/
 ResizeNewSetupButtons();

 HistoryButton=xv_create(ScrollPanel,PANEL_BUTTON,
  PANEL_LABEL_STRING,"ShowHistory",
  PANEL_NOTIFY_PROC, ShowHistory,
 NULL);
 PositionToRightOfButton(HistoryButton,DeleteButton);

 if (post_processing_mode())
  MakeTimeFrameButtons();
 else
  MakeTimeFrameMessage();

 SaveSelectionsButton=xv_create(ScrollPanel,PANEL_BUTTON,
  PANEL_LABEL_STRING,"Save",
  PANEL_NOTIFY_PROC, SaveSelnsToFile,
 NULL);
 PositionToRightOfButton(SaveSelectionsButton,DismissButton);

 StatsAutoPrintButton=xv_create(ScrollPanel,PANEL_TOGGLE,
  PANEL_CHOOSE_NONE,TRUE,
  PANEL_VALUE,-2,
  PANEL_CHOICE_STRINGS,"AutoPrt",NULL,
  PANEL_NOTIFY_PROC, ToggleStatsAutoPrint,
 NULL);
 PositionToRightOfButton(StatsAutoPrintButton,SaveSelectionsButton);

 StatsIntervalButton=xv_create(ScrollPanel,PANEL_TEXT,
  PANEL_LABEL_STRING,"Intrvl(sec)",
  PANEL_NOTIFY_PROC, ChgStatsInterval,
  PANEL_VALUE_DISPLAY_LENGTH, 4,
  PANEL_VALUE_STORED_LENGTH, 8,
  PANEL_VALUE,"",
 NULL);
 PositionToRightOfButton(StatsIntervalButton,StatsAutoPrintButton);
/*
must create menu first so it can be attached
*/
 InitStatsFlushMenu();
 StatsFlushMenuButton=xv_create(ScrollPanel,PANEL_BUTTON,
  PANEL_LABEL_STRING,"ShowStats",
  PANEL_ITEM_MENU,StatsFlushMenu,
  PANEL_INACTIVE,!GetNumStatsFlushMenuItems(),
 NULL);
 PositionToRightOfButton(StatsFlushMenuButton,StatsIntervalButton);

 DomainButton=xv_create(ScrollPanel,PANEL_CHOICE,
  PANEL_LABEL_STRING,"ShowBy:",
  PANEL_NOTIFY_PROC, ShowDomain,
  NULL);
 PositionBelowButton(DomainButton,DismissButton);
 SetDomainChoices();

 NextFlushMessage=xv_create(ScrollPanel,PANEL_MESSAGE,
 NULL);
 PositionToRightOfButton(NextFlushMessage,DomainButton);
 ChgFlushMessage(GetNextFlushString());

 SelectAllButton=xv_create(ScrollPanel,PANEL_BUTTON,
  PANEL_LABEL_STRING,"unknown",
  PANEL_NOTIFY_PROC, ToggleAllProc,
 NULL);

 ResetFromFileButton=xv_create(ScrollPanel,PANEL_BUTTON,
  PANEL_LABEL_STRING,"ResetFromFile",
  PANEL_NOTIFY_PROC, ResetFromFileProc,
 NULL);

 TypeButton=xv_create(ScrollPanel,PANEL_CHOICE,
  PANEL_NOTIFY_PROC, SortByType,
  PANEL_CHOOSE_NONE,TRUE,
  PANEL_VALUE,0,
  NULL);
 SetTypeChoices();
 PositionBelowButton(TypeButton,DomainButton);
 HidePanelItem(TypeButton);

 CategoryButton=xv_create(ScrollPanel,PANEL_CHOICE,
  PANEL_NOTIFY_PROC, SortByCategory,
  PANEL_CHOOSE_NONE,TRUE,
  PANEL_VALUE,-1,
  NULL);
 SetCategoryChoices();
 AdjustNrows(CategoryButton,GetScrollPanelWidth());
 PositionBelowButton(CategoryButton,DomainButton);
 HidePanelItem(CategoryButton);

 KeywordButton=xv_create(ScrollPanel,PANEL_TEXT,
  PANEL_NOTIFY_PROC, SortByKeyword,
  PANEL_VALUE,"",
  PANEL_LABEL_STRING,"EnterKeyword:",
  NULL);
 SetKeyboardButtonWidth();
 PositionBelowButton(KeywordButton,DomainButton);
 HidePanelItem(KeywordButton);

 VarPanelList=xv_create(ScrollPanel,PANEL_LIST,
  PANEL_NOTIFY_PROC,PanelListProc,
  PANEL_ITEM_COLOR,BLACK,
  XV_X,5,
  NULL);
 NewSetupPanelList=xv_create(ScrollPanel,PANEL_LIST,
  PANEL_NOTIFY_PROC,PanelListProc,
  XV_X,xv_get(VarPanelList,XV_X),
  PANEL_ITEM_COLOR,BLACK,
  NULL);
 AttrsPanelList=xv_create(ScrollPanel,PANEL_LIST,
  PANEL_NOTIFY_PROC,PanelListProc,
  XV_X,xv_get(VarPanelList,XV_X),
  PANEL_ITEM_COLOR,BLACK,
  NULL);
 StatsPanelList=xv_create(ScrollPanel,PANEL_LIST,
  PANEL_NOTIFY_PROC,PanelListProc,
  XV_X,xv_get(VarPanelList,XV_X),
  PANEL_ITEM_COLOR,BLACK,
  NULL);
/*
set it up so that the initial sort is by "all", with Type Domain button 
showing
*/
 HidePanelItem(AttrsPanelList);
 HidePanelItem(StatsPanelList);
 CurrentPanelList=VarPanelList;
 CurrentSortButton=CurrentDomainButton=TypeButton;
 CurrentSortButtonValue=0;
 SortByType(CurrentDomainButton,CurrentSortButtonValue);
 AdjustForDomainButton(CurrentDomainButton,FALSE);
 InitStatsPanelList(); 
/*
set up customized event handling
*/
 notify_interpose_event_func(ScrollPanel,ScrollPanelInterposer,NOTIFY_SAFE);
}

/**************************************************************************/

MakeTimeFrameButtons()
/*
for post-processing mode only: make buttons that give user the option of
selecting a time frame for variable history display
*/
{
 int minvalue=GetTimeInt(TRUE);
 int maxvalue=GetTimeInt(FALSE);

 if (maxvalue<minvalue) {
/*
we crossed midnite. Use continuous hours to help the math and convince the
panel item that the max really is bigger than the min.
*/
  maxvalue+=240000;
 }

 StartTimeButton=xv_create(ScrollPanel,PANEL_SLIDER,
  PANEL_LABEL_STRING, "Start:",
  PANEL_VALUE,minvalue,
  PANEL_NOTIFY_LEVEL,PANEL_DONE,
  PANEL_SHOW_RANGE,FALSE,
  PANEL_MIN_VALUE,minvalue,
  PANEL_MAX_VALUE,maxvalue,
  PANEL_NOTIFY_PROC,ChgHistoryTime,
  NULL);
  PositionToRightOfButton(StartTimeButton,HistoryButton);
  ShowPanelItem(HistoryButton);
 EndTimeButton=xv_create(ScrollPanel,PANEL_SLIDER,
  PANEL_LABEL_STRING, "End:  ",
  PANEL_VALUE,maxvalue,
  PANEL_NOTIFY_LEVEL,PANEL_DONE,
  PANEL_SHOW_RANGE,FALSE,
  PANEL_MIN_VALUE,minvalue,
  PANEL_MAX_VALUE,maxvalue,
  PANEL_NOTIFY_PROC,ChgHistoryTime,
  NULL);
  PositionBelowButton(EndTimeButton,StartTimeButton);
}

/**************************************************************************/

void
PanelListProc(item,string,client_data,op,event,row)
Panel_item item;
char *string;
caddr_t client_data;
Panel_list_op op;
Event *event;
int row;
/*
get value of variable name which has just been selected by user by clicking
on its name in panel. Update internal mgmt on selected var.
*/
{
 switch (op) {
  case PANEL_LIST_OP_DESELECT:
  case PANEL_LIST_OP_SELECT:
   SetSelectedVars(item,string,(int)client_data,op==PANEL_LIST_OP_SELECT);
   break;
 }
}

/*****************************************************************************/

void
PrintWindowProc(item,event)
Panel_item item;
Event *event;
/*
print out all currently listed items in panel list to a text file, and 
notify printing module to print this file
*/
{
int index;
char filename[256];

 (void)sprintf(filename,"%s/%s.%d",(char *)GetPrintWindowDir(),
  (char *)GetPrintWindowName(),getpid());
 for (index=0; index<NumScrollListEntries; index++) {
  if (index == 0 ) {
   PrintWindowFD=fopen(filename,"w");
   if (CurrentHeaderPrintFunc)
    (*CurrentHeaderPrintFunc)(PrintWindowFD);
  } else if (!(index % 53 )) {
   (void)fprintf(PrintWindowFD, "\f");
   if (CurrentHeaderPrintFunc)
    (*CurrentHeaderPrintFunc)(PrintWindowFD);
  }
  (void)fprintf(PrintWindowFD,"%s\n",ScrollList[index].string);
 }
 fflush(PrintWindowFD);
 fclose(PrintWindowFD);
 if (PrintTextFile(filename)==ERROR) {
  (void)fprintf(stderr,"WARNING: couldn't print text file\n");
 } 
}

/**************************************************************************/

char *
GetPrintWindowName()
/*
name of file used for temporary storage when printing current list window
*/
{
 return ".printwindow";
}

/**************************************************************************/

char *
GetPrintWindowDir()
/*
name of directory used for temporary storage when printing current list window 
*/
{
 static char filename[256];

 (void)sprintf(filename,"%s/%s",(char *)GetWindsHostsPath(),
  (char *)GetDisplayHost());
 return filename;
}

/**************************************************************************/

RemovePrintWindowFiles()
/*
remove any leftover print-window files from a previous session
*/
{
 FindFiles(GetPrintWindowName(),GetPrintWindowDir(),unlink,NULL,
  strlen((char *)GetPrintWindowName()));
}

/**************************************************************************/

SetDomainChoices()
/*
initialize the choice strings that indicate which domains are available
for filtering the list (e.g., type, category, keyword, etc.)
*/
{
 int opt;
 
 opt=0;
 while (DomainString[opt]) {
  xv_set(DomainButton,PANEL_CHOICE_STRING,opt,DomainString[opt],NULL);
  opt++;
 }
}

/**************************************************************************/

SetCategoryChoices()
/*
initialize the choice strings that indicate the filtering options available
within the (Bulletin 9) category domain
*/
{
 int opt;
 char *category;
 
 opt=0;
 category=(char *)GetFirstCategory();
 while ( (category) ) {
  xv_set(CategoryButton,PANEL_CHOICE_STRING,opt,category,NULL);
  category=(char *)GetNextCategory();
  opt++;
 }
}

/**************************************************************************/

SetTypeChoices()
/*
initialize the choice strings that indicate the filtering options available
within the type domain (e.g., raw, derived, analog, etc.)
*/
{
 int opt;

 opt=0;
 while (TypeString[opt]) {
  xv_set(TypeButton,PANEL_CHOICE_STRING,opt,TypeString[opt],NULL);
  opt++;
 }
}

/*************************************************************************/

SetListTitle(panellist,sorttype)
Panel_item panellist;
char *sorttype;
/*
set title on panellist as function of string sorttype
*/
{
 static char title[128];

 if (GetListFromVarFilter()) {
  (void)sprintf(CurrentFilteredListTitle,"%s",sorttype);
  (void)sprintf(title,"Variables: %s",sorttype);
 } else
  (void)sprintf(title,"%s",sorttype);
 xv_set(panellist,
  PANEL_LIST_TITLE,title,
  PANEL_CLIENT_DATA,ERROR,
  NULL);
}

/*************************************************************************/

SetCustomListTitle(title)
char *title;
{
 (void)sprintf(CustomListTitle,"%s",title);
}

/*************************************************************************/

char *
GetCustomListTitle()
{
 return CustomListTitle;
}

/*************************************************************************/

char *
GetPanelListTitle()
/*
return current title of current panel list
*/
{
 return
  (char *)xv_get(CurrentPanelList,PANEL_LIST_TITLE);
}

/*************************************************************************/

ShowCategoryButton()
/*
show the category domain options and hide all others
*/
{
 HidePanelItem(TypeButton);
 HidePanelItem(KeywordButton);
 AdjustForDomainButton(CategoryButton,FALSE);
}

/*************************************************************************/

ShowTypeButton()
/*
show the type domain options and hide all others
*/
{
 HidePanelItem(KeywordButton);
 HidePanelItem(CategoryButton);
 AdjustForDomainButton(TypeButton,FALSE);
}

/*************************************************************************/

SetDomainButtonValue(keyword)
char *keyword;
/*
set value of DomainButton as per a match (if any) with DomainString and
keyword
*/
{
 int string=0;

 while (DomainString[string]) {
  if (!strcasecmp(keyword,DomainString[string])) {
   SetButtonValue(DomainButton,string);
   return;
  }
  string++;
 }
}

/*************************************************************************/

SetTypeButtonValue(keyword)
char *keyword;
/*
set value of TypeButton as per a match (if any) with TypeString and
keyword
*/
{
 int string=0;

 while (TypeString[string]) {
  if (!strcasecmp(keyword,TypeString[string])) {
   SetButtonValue(TypeButton,string);
   if (CurrentSortButton==TypeButton)
    CurrentSortButtonValue=string;
   return;
  }
  string++;
 }
}

/*************************************************************************/

ShowKeywordButton()
/*
show the keyword domain input item and hide all other domain options
*/
{
 HidePanelItem(TypeButton);
 HidePanelItem(CategoryButton);
 AdjustForDomainButton(KeywordButton,TRUE);
 SetButtonMessage(KeywordButton,"");
 win_set_kbd_focus(ScrollEventWindow,xv_get(ScrollEventWindow,XV_XID));
 xv_set(ScrollPanel,PANEL_CARET_ITEM,KeywordButton,NULL);
}

/**************************************************************************/

ShowDomain(item,value)
Panel_item item;
int value;
/*
show domain options as indicated by value, as provided from user click on
desired domain in the domain button widget.
*/
{
 (*ShowDomainFunc[value])();
 HidePanelItem(CurrentPanelList);
 NeedListRefresh=TRUE;
}

/*************************************************************************/

AdjustForDomainButton(button,textitem)
Panel_item button;
int textitem;
/*
position indicated button directly below the Dismiss button; if the scrolling
list has been init'd, adjust its layout as function of this button so list is
directly below the button. 

Update internal notion of which button is currently in place above the list
and show the button.
*/
{
 PositionBelowButton(button,DomainButton);
 if (CurrentPanelList) {
  SetPanelListLayout(CurrentPanelList,button);
 }
 CurrentDomainButton=button;
 ShowPanelItem(button);
/*
if button is NOT a text input button and is NOT the current sort button, 
turn off any selected value 
*/
 if (button!=CurrentSortButton &&
     (button==TypeButton || button==CategoryButton))
  SetAllChoicesOff(button);
}

/*************************************************************************/

ActivateApplyButton(active)
int active;
/*
activate ApplyButton
*/
{
 SetItemInactive(ApplyButton,!active);
}

/*************************************************************************/

SetExclusiveChoices(panellist,exclusive,set)
Panel_item panellist;
int exclusive;
/*
set panellist to exclusivity as per exclusive
*/
{
 if (exclusive==ERROR) {
/*
no selections needed or allowed. Turn off any that are selected.
*/
  SetItemExclusive(panellist,FALSE);
  if (set)
   SetAllPanelItems(panellist,FALSE);
  else
   SetSelectedPanelItem(panellist,
    GetNextSelection(panellist,0,TRUE),FALSE);
  return;
 }
 if ( (GetItemExclusive(panellist) && exclusive==FALSE) ||
/*
panel is currently exclusive and non-exclusive is wanted
*/
      (!GetItemExclusive(panellist) && exclusive==TRUE) ) {
/*
panel is currently non-exclusive and exclusive is wanted
*/
  if (set)
   SetAllPanelItems(panellist,FALSE);
 }
 SetItemExclusive(panellist,exclusive);
/*
set panel selections now that exclusivity is determined
*/
 if (set)
  SetPanelSelections(panellist);
}

/**************************************************************************/

GetListFromVarFilter()
/*
return TRUE if current list is filled from a filter function
*/
{
 return ListFromVarFilter;
}

/**************************************************************************/

SetListFromVarFilter(set)
int set;
/*
set ListFromVarFilter as per set, i.e. whether current list is filled from 
a filter function
*/
{
 ListFromVarFilter=set;
 if (set) {
  SetListFromTextFile(FALSE);
  SetListFromStrings(FALSE);
 }
}

/**************************************************************************/

GetListFromTextFile()
/*
return TRUE if current list is filled from a text file
*/
{
 return ListFromTextFile;
}

/**************************************************************************/

SetListFromTextFile(set)
int set;
/*
set ListFromTextFile as per set, i.e. whether current list is filled from 
a text file
*/
{
 ListFromTextFile=set;
 if (set) {
  SetListFromVarFilter(FALSE);
  SetListFromStrings(FALSE);
 }
}

/**************************************************************************/

GetListFromStrings()
/*
return TRUE if current list is filled from Strings char array
*/
{
 return ListFromStrings;
}

/**************************************************************************/

SetListFromStrings(set)
int set;
/*
set parameters to indicate that the current list is filled (just about to be
filled) from the Strings[] char array
*/
{
 ListFromStrings=set;
 if (set) {
  SetListFromVarFilter(FALSE);
  SetListFromTextFile(FALSE);
 }
}

/**************************************************************************/

FillScrollListFromStrings(title)
char *title;
/*
fill the scrolling list values from the Strings[] array, saving the current fill
status for re-instatement if needed on menu dismissal
*/
{
 SetListFromStrings(TRUE);
 FillScrollList(NULL,title);
 CurrentSortButtonValue=GetButtonValue(CurrentSortButton);
 if ( (CurrentSortButton==TypeButton || CurrentSortButton==CategoryButton))
/*
if current sort button is NOT a text input button, turn off any selected value 
*/
  SetAllChoicesOff(CurrentSortButton);
}

/**************************************************************************/

FillScrollListFromTextFile(title)
char *title;
/*
fill the scrolling list values from a text file, saving the current fill
status for re-instatement if needed on menu dismissal
*/
{
 SetListFromTextFile(TRUE);
 FillScrollList(NULL,title);
 CurrentSortButtonValue=GetButtonValue(CurrentSortButton);
 if ( (CurrentSortButton==TypeButton || CurrentSortButton==CategoryButton))
/*
if current sort button is NOT a text input button, turn off any selected value 
*/
  SetAllChoicesOff(CurrentSortButton);
}

/**************************************************************************/

FillScrollList(filter,title)
int (*filter)();
char *title;
/*
allocate internal store of list entries space as per number of entries in Menu 
List. If scrolling list has previously been configured, delete all of its 
entries. In either event fill in the list using indicated filtering 
function and set layout of list on completion. Save value of current filter
function.
*/
{
 int entry,listsize,listfromtext,listfromstrings;
 char string[256];
 Xv_font font;

 if (GetListFromTextFile()) {
  ReadFileIntoArray(CurrentTextFile,NULL);
  listsize=GetNumFileEntries();
 } else if (GetListFromStrings()) {
  listsize=GetNumStrings();
 } else if (GetListFromVarFilter()) {
  listsize=GetNumMenuEntries();
 }
 if ( (ScrollList=(Panel_list_row_values *)realloc((void *)ScrollList,
  (size_t)(listsize*sizeof(Panel_list_row_values))))==NULL) {
  RequestAlertWhenSafe("WARNING: couldn't allocate enough memory to fill scrolling list. Please contact a WINDS expert about this!");
  ListLimits();
  return;
 }
 DeleteScrollLists(NumScrollListEntries);
 HidePanelItem(CurrentPanelList);
 NumScrollListEntries=0;
 font=GetStandardFont();
 listfromtext=GetListFromTextFile();
 listfromstrings=GetListFromStrings();
 for (entry=0; entry<listsize; entry++) {
  if (!filter || (filter && (*filter)(entry))) {
   if (listfromtext) {
    (void)sprintf(string,"%s",(char *)GetEntryString(entry));
   } else if (listfromstrings) {
    (void)sprintf(string,"%s",&Strings[entry*GetMaxLenStrings()]);
   } else if (GetListFromVarFilter()) {
    ScrollList[NumScrollListEntries].client_data=
    GetMenuListIndex(entry)|GetMenuListMode(entry);
    (void)sprintf(string,"%s                    ",
     (char *)GetMenuListName(entry));
    (void)sprintf(&string[NAMLEN+1],"%s",(char *)GetMenuListTitle(entry));
   }
   ScrollList[NumScrollListEntries].string=(char *)strdup((char *)string);
   ScrollList[NumScrollListEntries].font=font;
   ScrollList[NumScrollListEntries].glyph=XV_NULL;
   ScrollList[NumScrollListEntries].mask_glyph=XV_NULL;
   ScrollList[NumScrollListEntries].inactive=FALSE;
   ScrollList[NumScrollListEntries].selected=FALSE;
   NumScrollListEntries++;
  }
 }
 InstallScrollLists(NumScrollListEntries,title);
 if (GetListFromVarFilter()) {
  CurrentFilter=filter;
 }
 NeedListRefresh=TRUE;
}

/*************************************************************************/

ShowSelectedItemPage()
/*
set selection again in case previous list was on a page that does not exist
here, to be sure selected item will be visible to user
*/
{
 int selection;

 if (NeedListRefresh) {
  ShowPanelItem(CurrentPanelList);
  if (GetItemExclusive(CurrentPanelList)) {
   selection=GetNextSelection(CurrentPanelList,0,TRUE);
   if (selection!=ERROR)
    SetPanelSelection(CurrentPanelList,selection,TRUE);
  }
  NeedListRefresh=FALSE;
 }
}

/*************************************************************************/

SetEmptyList(panellist,title)
Panel_item panellist; 
char *title;
/*
inactivate panellist and give it a title reflecting that there are no
entries in it.
*/
{
 xv_set(panellist,
  PANEL_LIST_STRINGS,"(none)",NULL,
  PANEL_INACTIVE,TRUE,
  NULL);
 SetListTitle(panellist,title);
}

/*************************************************************************/

FillPanelList(panellist,title)
Panel_item panellist; 
char *title;
/*
fill panellist with values in ScrollList structures, set title to title,
set panellist layout, and set its selections if it is the current list
*/
{
 xv_set(panellist,
  PANEL_LIST_ROW_VALUES,0,ScrollList,NumScrollListEntries,
  PANEL_INACTIVE,FALSE,
  NULL);
 SetListTitle(panellist,title);
 SetPanelListLayout(panellist,CurrentDomainButton);
 if (panellist==CurrentPanelList)
  SetPanelSelections(panellist);
}

/*************************************************************************/

DeleteScrollLists(numentries)
int numentries;
/*
remove entries 0 through numentries in all panel lists
*/
{
 DeletePanelListRows(StatsPanelList,0,numentries);
 DeletePanelListRows(AttrsPanelList,0,numentries);
 DeletePanelListRows(VarPanelList,0,numentries);
 DeletePanelListRows(NewSetupPanelList,0,numentries);
}

/*************************************************************************/

InstallScrollLists(numentries,title)
int numentries;
char *title;
/*
if numentries is zero, set all lists to empty, else fill them as per ScrollList.
Set title to title. If current list is statistics menu, show the 
toggle-all and reset from file buttons and adjust toggle-button label as
per how many items are selected.
*/
{
 HidePanelItem(ApplyButton);
 HidePanelItem(HistoryButton);
 if (post_processing_mode()) {
  HidePanelItem(StartTimeButton);
  HidePanelItem(EndTimeButton);
 } else {
  HidePanelItem(TimeFrameMessage);
 }
 HidePanelItem(SelectAllButton);
 HidePanelItem(ResetFromFileButton);
 if (!numentries) {
  SetEmptyList(CurrentPanelList,title);
  if (CurrentPanelList!=NewSetupPanelList)
/*
New setup module maintains its own internal memory of selections made; don't
doubly manage it here.
*/
   SetItemInactive(ApplyButton,TRUE);
  if (CurrentPanelList==VarPanelList)
   SetItemInactive(HistoryButton,TRUE);
   if (post_processing_mode()) {
    SetItemInactive(StartTimeButton,TRUE);
    SetItemInactive(EndTimeButton,TRUE);
   }
 } else {
  FillPanelList(CurrentPanelList,title);
  if (CurrentPanelList!=NewSetupPanelList)
   SetItemInactive(ApplyButton,
    !GetListFromVarFilter() || !NumScrollListEntries);
  if (CurrentPanelList==VarPanelList) {
   SetItemInactive(HistoryButton,NumScrollListEntries==0);
   if (post_processing_mode()) {
    SetItemInactive(StartTimeButton,NumScrollListEntries==0);
    SetItemInactive(EndTimeButton,NumScrollListEntries==0);
   }
   if (GetListFromVarFilter())
    SetExclusiveChoices(VarPanelList,TRUE,FALSE);
  }
 }
 if (CurrentPanelList==StatsPanelList) {
  ShowPanelItem(SelectAllButton);
  ShowPanelItem(ResetFromFileButton);
  SetSelectAllButtonLabel();
 } else if (CurrentPanelList==AttrsPanelList) {
 } else if (CurrentPanelList==NewSetupPanelList) {
  ShowPanelItem(ApplyButton);
 } else 
 if (CurrentPanelList==VarPanelList) {
  ShowPanelItem(ApplyButton);
  ShowPanelItem(HistoryButton);
  if (post_processing_mode()) {
   ShowPanelItem(StartTimeButton);
   ShowPanelItem(EndTimeButton);
  } else {
   ShowPanelItem(TimeFrameMessage);
  }
 }
}

/************************  New Setup Menu List  ****************************/

ResizeNewSetupButtons()
/*
when panel buttons with PANEL_DISPLAY_LEVEL set to PANEL_CURRENT are rendered
through the xview menu system, available control of fonts is inadequate, 
resulting in item widths too small for the rendered strings (sometimes). 
Preclude that by re-creating all such buttons before each time they are 
displayed .
*/
{
 int value;
/*
drill: if button already exists, save its value and destroy it; (re)create it
and position it
*/
 if (CurrentQuadButton) {
  value=GetButtonValue(CurrentQuadButton);
  xv_destroy(CurrentQuadButton);
 }
 CurrentQuadButton=xv_create(ScrollPanel,PANEL_CHOICE,
  PANEL_NOTIFY_PROC, SetCurrQuadProc,
  PANEL_DISPLAY_LEVEL, PANEL_CURRENT,
  PANEL_CHOICE_STRINGS,"U.Left","U.Right","L.Right","L.Left",NULL,
  PANEL_VALUE,value,
  NULL);
 PositionToRightOfButton(CurrentQuadButton,ApplyButton);

 if (PlotTypeButton) {
  value=GetButtonValue(PlotTypeButton);
  xv_destroy(PlotTypeButton);
 }
 PlotTypeButton=xv_create(ScrollPanel,PANEL_CHOICE,
  PANEL_NOTIFY_PROC, SetPlotTypeProc,
  PANEL_DISPLAY_LEVEL, PANEL_CURRENT,
  NULL);
/*
need to get choice strings from new setup module
*/
 SetPlotTypeChoices(GetNumPlotTypeChoices(),GetPlotTypeChoiceString);
 xv_set(PlotTypeButton,
  PANEL_VALUE,value,
  NULL);
 PositionToRightOfButton(PlotTypeButton,CurrentQuadButton);

 if (VarTypeButton)
  xv_destroy(VarTypeButton);
/*
don't need to save value on VarTypeButton, choices are set by new setup module
dynamically as function of PlotTypeButton value...
*/
 VarTypeButton=xv_create(ScrollPanel,PANEL_CHOICE,
  PANEL_NOTIFY_PROC, SetVarTypeProc,
  PANEL_DISPLAY_LEVEL, PANEL_CURRENT,
  NULL);
 PositionToRightOfButton(VarTypeButton,PlotTypeButton);
}

/**************************************************************************/

PositionNewSetupButtons()
/*
lay out of various buttons in new setup module
*/
{
 PositionToRightOfButton(CurrentQuadButton,ApplyButton);
 PositionToRightOfButton(PlotTypeButton,CurrentQuadButton);
 PositionToRightOfButton(VarTypeButton,PlotTypeButton);
}

/**************************************************************************/

ShowNewSetupMenu(quadrant)
int quadrant;
/*
remove any other panel menu that is showing, if any, and do what needs to
be done to show the new setup menu, initially set to act on the quadrant
designated in arg quadrant.
*/
{
 CurrentPanelList=NewSetupPanelList;
 RemoveCurrentPanelMenu();
 ResizeNewSetupButtons();
 ShowPanelItem(ApplyButton);
/*
apply function is not available until user makes minimum number of choices
*/
 SetItemInactive(ApplyButton,TRUE);
 ShowPanelItem(PlotTypeButton);
 ShowPanelItem(VarTypeButton);
 ShowPanelItem(CurrentQuadButton);
/*
hide unneeded buttons
*/
 HidePanelItem(ChgAttrsButton);
 HidePanelItem(PrintWindowButton);
 HidePanelItem(DeleteButton);
 HidePanelItem(HistoryButton);
 if (post_processing_mode()) {
  HidePanelItem(StartTimeButton);
  HidePanelItem(EndTimeButton);
 } else {
  HidePanelItem(TimeFrameMessage);
 }
 HidePanelItem(SaveSelectionsButton);
 HidePanelItem(StatsAutoPrintButton);
 HidePanelItem(StatsIntervalButton);
 HidePanelItem(StatsFlushMenuButton);
 HidePanelItem(NextFlushMessage);
 HidePanelItem(SelectAllButton);
 HidePanelItem(ResetFromFileButton);
 HidePanelItem(AttrsPanelList);
 HidePanelItem(StatsPanelList);
 HidePanelItem(VarPanelList);
/*
non-exclusive choices
*/
 SetExclusiveChoices(NewSetupPanelList,FALSE,TRUE);
/*
lay it out in center of WINDS window
*/
 SetScrollPanelDims(2*GetBaseFrameWidth()/3);
/*
start with all items de-selected
*/
 SetAllPanelItems(NewSetupPanelList,FALSE);
 SetCurrentSetupQuadrant(quadrant);
/*
no need for Event *, event info has already remembered in new setup module
*/
 ShowScrollPanel(NewSetupPanelList,quadrant,(Event *)NULL,"New Setup Menu");
}

/**************************************************************************/

SetCurrentSetupQuadrant(quadrant)
int quadrant;
/*
manage which quadrant is currently deemed to be acted upon, i.e. if user 
elects to Apply, with new setup operations
*/
{
 CurrentSetupQuadrant=quadrant;
 SetButtonValue(CurrentQuadButton,quadrant);
}

/**************************************************************************/

GetCurrentSetupQuadrant()
/*
return which quadrant is currently deemed to be acted upon, i.e. if user 
elects to Apply, with new setup operations
*/
{
 return CurrentSetupQuadrant;
}

/*************************************************************************/

void
SetCurrQuadProc(item)
Panel_item item;
/*
actions to be taken when user changes current quadrant for new setup 
application: finish off old quadrant and initialize for new one
*/
{
 int OldQuadrant;

 OldQuadrant=GetCurrentSetupQuadrant();
 fini_new_setup(OldQuadrant);
 SetCurrentSetupQuadrant(GetButtonValue(item));
 InitNewSetupVars(FALSE);
}

/*************************************************************************/

void
SetVarTypeProc(item)
Panel_item item;
/*
manage new setup menu display as function of this variable type just chosen
by user
*/
{
 HidePanelItem(NewSetupPanelList);
 SetVarType(GetButtonValue(item));
 UpdateSelnsMade();
 SetAllPanelItems(NewSetupPanelList,FALSE);
 SetSelectionsFromList(NewSetupPanelList, GetNumNewSetupSelns(),GetNewSetupSeln);
 SetNewSetupChoicesExclusive();
 ShowPanelItem(NewSetupPanelList);
}

/*************************************************************************/

SetNewSetupChoicesExclusive()
/*
make new setup list choices non-exclusive, i.e. unless one variable has been
selected: at that point, exclusivity is function of the variable type in use
*/
{
 SetItemExclusive(NewSetupPanelList,FALSE);
 if (GetVarTypeIsExclusive() && GetNumNewSetupSelns()==1)
  SetItemExclusive(NewSetupPanelList,TRUE);
}

/**************************************************************************/

GetVarTypeButtonActive()
/*
return TRUE if VarTypeButton is active, else return FALSE
*/
{
 return GetButtonIsActive(VarTypeButton);
}

/*************************************************************************/

ShowVarTypeButton()
/*
show VarTypeButton
*/
{
 ShowPanelItem(VarTypeButton);
}

/*************************************************************************/

HideVarTypeButton()
/*
hide VarTypeButton
*/
{
 HidePanelItem(VarTypeButton);
}

/*************************************************************************/

ActivateVarTypeButton(active)
int active;
/*
activate VarTypeButton
*/
{
 SetItemInactive(VarTypeButton,!active);
}

/*************************************************************************/

ShowPlotTypeButton()
/*
show PlotTypeButton
*/
{
 ShowPanelItem(PlotTypeButton);
}

/*************************************************************************/

HidePlotTypeButton()
/*
hide PlotTypeButton
*/
{
 HidePanelItem(PlotTypeButton);
}

/*************************************************************************/

ShowCurrentQuadButton()
/*
show CurrentQuadButton
*/
{
 ShowPanelItem(CurrentQuadButton);
}

/*************************************************************************/

HideCurrentQuadButton()
/*
hide CurrentQuadButton
*/
{
 HidePanelItem(CurrentQuadButton);
}

/*************************************************************************/

SetVarTypeButtonValue(value)
int value;
/*
set value of VarTypeButton
*/
{
 SetButtonValue(VarTypeButton,value);
}

/*************************************************************************/

ClearVarTypeChoices()
/*
remove all choice strings from VarTypeButton
*/
{
 ClearChoiceStrings(VarTypeButton);
}

/**************************************************************************/

SetPlotTypeButtonValue(value)
int value;
/*
set value of PlotTypeButton
*/
{
 SetButtonValue(PlotTypeButton,value);
}

/*************************************************************************/

SetPlotTypeChoices(numchoices,ChoiceStringFunc)
int numchoices;
char *(*ChoiceStringFunc)();
/*
remove all extant choice strings, and then set numchoices choice strings
(as obtained from ChoiceStringFunc()) in PlotTypeButton
*/
{
 int choice;

 ClearChoiceStrings(PlotTypeButton);
 for (choice=0; choice<numchoices; choice++) {
  SetChoiceString(PlotTypeButton,choice,ChoiceStringFunc(choice));
 }
}

/*************************************************************************/

UpdateVarTypeButton(numchoice,string)
int numchoice;
char *string;
/*
set the numchoice'th choice string in VarTypeButton
*/
{
 if (VarTypeButton)
  SetChoiceString(VarTypeButton,numchoice,string);
}

/*************************************************************************/

void
SetPlotTypeProc(item)
Panel_item item;
/*
user has just selected a plot type; adjust new setup menu list accordingly
and notify new setup module to do whatever it needs to do. Initially set
choices to non-exclusive.
*/
{
int setup_type,quadrant;
 
 quadrant=GetCurrentSetupQuadrant();
 setup_type=GetButtonValue(item);
 HidePanelItem(NewSetupPanelList);
 SetAllPanelItems(NewSetupPanelList,FALSE);
 ConfigPlotType(setup_type,quadrant);
 SetNewSetupChoicesExclusive();
 ShowPanelItem(NewSetupPanelList);
}

/*************************  Variable Selector Menu  ***********************/

ShowVarMenu(quadrant,event)
int quadrant;
Event *event;
/*
remove any other panel menu that is showing, if any, and do what must be
done to show variable selector menu list.
*/
{
 CurrentPanelList=VarPanelList;
 RemoveCurrentPanelMenu();
 ShowPanelItem(ApplyButton);
 SetItemInactive(ApplyButton,NumScrollListEntries==0);
 ShowPanelItem(DeleteButton);
 ShowPanelItem(HistoryButton);
 if (post_processing_mode()) {
  ShowPanelItem(StartTimeButton);
  ShowPanelItem(EndTimeButton);
 } else {
  ShowPanelItem(TimeFrameMessage);
 }
 HidePanelItem(PlotTypeButton);
 HidePanelItem(VarTypeButton);
 HidePanelItem(ChgAttrsButton);
 HidePanelItem(PrintWindowButton);
 HidePanelItem(CurrentQuadButton);
 HidePanelItem(SaveSelectionsButton);
 HidePanelItem(StatsAutoPrintButton);
 HidePanelItem(StatsIntervalButton);
 HidePanelItem(StatsFlushMenuButton);
 HidePanelItem(NextFlushMessage);
 HidePanelItem(SelectAllButton);
 HidePanelItem(ResetFromFileButton);
 HidePanelItem(AttrsPanelList);
 HidePanelItem(StatsPanelList);
 HidePanelItem(NewSetupPanelList);
 SetExclusiveChoices(VarPanelList,TRUE,FALSE);
 SetScrollPanelDims(3*GetBaseFrameWidth()/4);
 ShowScrollPanel(VarPanelList,quadrant,event,"Variables Menu");
}

/**************************************************************************/

SetHistoryVarname(varname)
char *varname;
/*
remember which variable is a candidate for showing history
*/
{
 (void)sprintf(HistoryVarname,"%s",varname);
}

/**************************************************************************/

AdjustHistoryButtonValues(adjusted_item)
Panel_item adjusted_item;
/*
if the user has adjusted history time frame such that end time is sooner than
start time, reconcile things to make both times equal to the value of item
just adjusted.
*/
{
 Panel_item other_item;

 if (!StartTimeButton)
  return;

 if (adjusted_item==StartTimeButton)
  other_item=EndTimeButton;
 else
  other_item=StartTimeButton;

 if (GetButtonValue(StartTimeButton)>GetButtonValue(EndTimeButton))
  SetButtonValue(other_item,GetButtonValue(adjusted_item));
}

/**************************************************************************/

void 
ChgHistoryTime(item,value,event)
Panel_item item;
int value;
Event *event;
/*
notify procedure for history time frame selection button values: effectively
converts a 6-digit integer to an HHMMSS type number, i.e. maintaining 
sensible limits in each of the 3 pairs of numbers and adjusting adjacent-left
pairs as needed to convert a given value to the most reasonable HHMMSS
counterpart
*/
{
 int strip;
 static int lastvalue;

 NewValue=value;
/*
adjust seconds field
*/
 strip=StripValue(value,100); 
 if (strip>=60) 
  {
  if (value>lastvalue)
   NewValue=NewValue-strip+100;
  else
   NewValue=NewValue-strip+59;
  }
/*
adjust minutes field
*/
 strip=StripValue(NewValue,10000); 
 if (strip>=6000) 
  {
  if (value>lastvalue)
   NewValue=NewValue-strip+10000;
  else
   NewValue=NewValue-strip+5959;
  }
/*
hours field is left alone, governed already by min and max; at midnite, hours
value continues to 24 and beyond.
*/
 if (NewValue!=value)
/*
need an adjustment, but it's not safe to chg value within notify proc; delay 
until next catchalrm() time-out
*/
  SetNeedItemValueChg(item);
 else
  AdjustHistoryButtonValues(item);

 lastvalue=NewValue;
}

/**************************************************************************/

StripValue(value,filter)
int value,filter;
/*
return the n rightmost digits from value, where 10 to n'th power==filter
*/
{
 return value-(value/filter*filter);
}

/**************************************************************************/

ChgItemValue()
/*
change value of panel item NeedItemValueChg to NewValue
*/
{
 SetButtonValue(NeedItemValueChg,NewValue);
 AdjustHistoryButtonValues(NeedItemValueChg);
 NeedItemValueChg=FALSE;
}

/**************************************************************************/

GetNeedItemValueChg()
/*
return value of NeedItemValueChg, which is FALSE unless it represents a
Panel_item which needs an adjustment in PANEL_VALUE
*/
{
 return NeedItemValueChg;
}

/**************************************************************************/

SetNeedItemValueChg(item)
Panel_item item;
/*
set value of NeedItemValueChg, i.e. a Panel_item needing PANEL_VALUE adjustment,
to that Panel_item's handle (item).
*/
{
 NeedItemValueChg=item;
}

/**************************************************************************/

UpdateTimeFrameMessage()
/*
in real-time: update the start/end times of the time frame in shared 
memory available for variable history display
*/
{
 static int laststart=ERROR,lastend=ERROR;
 int startpoint,endpoint;

 startpoint=GetStartTimeIndex()%get_datablock_size()*3;
 endpoint=GetEndTimeIndex()%get_datablock_size()*3;
 if (laststart==startpoint && lastend==endpoint) 
/*
things haven't changed: don't bother to do anything
*/
  return;
 (void)sprintf(TimeFrameString,"(%02d:%02d:%02d-%02d:%02d:%02d)",
  GetHMS(startpoint),GetHMS(startpoint+1),GetHMS(startpoint+2),
  GetHMS(endpoint),GetHMS(endpoint+1),GetHMS(endpoint+2));
 SetButtonLabel(TimeFrameMessage,TimeFrameString);
  
 laststart=startpoint;
 lastend=endpoint; 
}

/**************************************************************************/

MakeTimeFrameMessage()
/*
make the real-time message item that shows user the time frame in
shared memory available for variable history display
*/
{
 TimeFrameMessage=xv_create(ScrollPanel,PANEL_MESSAGE,
  NULL);
 PositionToRightOfButton(TimeFrameMessage,HistoryButton);
 ShowPanelItem(HistoryButton);
}

/**************************************************************************/

GetHistoryStartHour()
/*
return current value of start hour in post-processing mode variable history
time-frame button 
*/
{
 return GetButtonValue(StartTimeButton)/10000;
}

/**************************************************************************/

GetHistoryStartMinute()
/*
return current value of start minute in post-processing mode variable history
time-frame button 
*/
{
 return StripValue(GetButtonValue(StartTimeButton),10000)/100;
}

/**************************************************************************/

GetHistoryStartSecond()
/*
return current value of start second in post-processing mode variable history
time-frame button 
*/
{
 return StripValue(GetButtonValue(StartTimeButton),100);
}

/**************************************************************************/

GetHistoryEndHour()
/*
return current value of end hour in post-processing mode variable history
time-frame button 
*/
{
 return GetButtonValue(EndTimeButton)/10000;
}

/**************************************************************************/

GetHistoryEndMinute()
/*
return current value of end minute in post-processing mode variable history
time-frame button 
*/
{
 return StripValue(GetButtonValue(EndTimeButton),10000)/100;
}

/**************************************************************************/

GetHistoryEndSecond()
/*
return current value of end second in post-processing mode variable history
time-frame button 
*/
{
 return StripValue(GetButtonValue(EndTimeButton),100);
}

/**************************************************************************/

GetMaxLenStrings()
/*
return the (currently hardwired) value for maximum length of the Strings[]
array
*/
{
 return 80;
}

/**************************************************************************/

SetNumStrings(num)
int num;
/*
set the number of elements in the Strings[] array that are to be currently
used as values for display in the scrolling list, to num
*/
{
 NumStrings=num;
}

/**************************************************************************/

GetNumStrings()
/*
return the number of elements in the Strings[] array that are to be currently
used as values for display in the scrolling list
*/
{
 return NumStrings;
}

/**************************************************************************/

GetHistoryToStrings(varname)
char *varname;
/*
in post-processing or real-time modes, get the history as indicated either
by time-frame buttons or the entire shared memory time frame, respectively,
for variable varname, and format it to the Strings[] array
*/
{
 static int init=FALSE;
 static char *StartDate,Month[16];
 static int StartMonth,StartDay,StartYear;
 static float *Values=0;
 static float *Times=0;
 static int Count,SameSize;
 int point,value,startbase,endbase,datastart,filestart;
 int startpoint,endpoint;
 int lenstring=GetMaxLenStrings();

 if (!init) {
  init=TRUE;
  if (post_processing_mode()) {
/*
use Unix basetime mechanisms to determine start basetime; open the netcdf file
and allocate the Times array as apropos, filling it in with values as obtained
from the netcdf file. Determine sizing requirement of Values array.
*/
   StartDate=(char *)strdup((char *)get_date_from_base(GetDataStartTime()));
   (void)sscanf(StartDate,"%03s-%02d-%02d",Month,&StartDay,&StartYear);
   StartMonth=GetMonthNum(Month);
   OpenNetCDFFile(GetNetCDFFilename(),TRUE);
   Count=GetDataEndTime()-GetDataStartTime()+1;
  } else {
/*
determine sizing requirement of Values array.
*/
   Count=get_time_frame_size();
  }
/*
allocate the Values array, which holds values for given variable, dealing with
memory allocation failure in hopefully a graceful way.
*/
  if ( (Values=(float *)realloc((void *)Values,(size_t)(Count*sizeof(float))))
   ==NULL) {
   RequestAlertWhenSafe("WARNING: couldn't allocate enough memory to complete history operation. Please contact a WINDS expert about this!");
   SetItemInactive(HistoryButton,TRUE);
   if (post_processing_mode()) {
    SetItemInactive(StartTimeButton,TRUE);
    SetItemInactive(EndTimeButton,TRUE);
   }
   return;
  }
/*
allocate the Strings array, which is formatted with names and values each time
user requests history on a variable, dealing with memory allocation failure in 
hopefully a graceful way.
*/
  if ( (Strings=(char *)realloc((void *)Strings,
   (size_t)((Count*lenstring)*sizeof(char)))) ==NULL) {
   RequestAlertWhenSafe("WARNING: couldn't allocate enough memory to complete history operation. Please contact a WINDS expert about this!");
   ListLimits();
   SetItemInactive(HistoryButton,TRUE);
   if (post_processing_mode()) {
    SetItemInactive(StartTimeButton,TRUE);
    SetItemInactive(EndTimeButton,TRUE);
   }
   return;
  }
 } else {
  if (!Values || !Strings)
/*
mem alloc'n possibly failed on initial processing attempt; do nothing
*/
   return;
 }
 if (post_processing_mode()) {
  (void)sprintf(TimeFrameString,"(%02d:%02d:%02d-%02d:%02d:%02d)",
   GetHistoryStartHour(),GetHistoryStartMinute(),GetHistoryStartSecond(),
   GetHistoryEndHour(),GetHistoryEndMinute(),GetHistoryEndSecond());
/*
determine start and end basetimes, from which the file position is calculated.
Adjust as necessary for crossing midnite, since the starting month/day/year
are not updated when that happens.
*/
  startbase=GetBaseFromMDYHMS(StartMonth,StartDay,StartYear,
   GetHistoryStartHour(),GetHistoryStartMinute(),GetHistoryStartSecond());
  endbase=GetBaseFromMDYHMS(StartMonth,StartDay,StartYear,
   GetHistoryEndHour(),GetHistoryEndMinute(),GetHistoryEndSecond());
  filestart=startbase-GetDataStartTime();
  if (endbase<startbase)
/*
we crossed midnite. Compensate.
*/
   endbase+=86400;
/*
indicate how many elements of the Strings[] array are relevant.
*/
  SameSize=FALSE;
  if (Count!=endbase-startbase+1)
   Count=endbase-startbase+1;
  else
/*
indicate re-sizing allocations for Times that it's not needed
*/
   SameSize=TRUE;
 } else {
/*
real-time mode: determine start index into data arrays (datastart) and adjusted
indices into time arrays (startpoint/endpoint). Indicate how many elements of 
the Strings[] array are relevant.
*/
  datastart=GetStartTimeIndex();
  startpoint=datastart%get_datablock_size();
  endpoint=GetEndTimeIndex()%get_datablock_size();
  Count=MIN(GetWINDSElapsedTime(),get_time_frame_size());
 }
 (void)fprintf(stderr,"Get history on %s %s\n",HistoryVarname,TimeFrameString);
 SetNumStrings(Count);
 if (post_processing_mode()) {
/*
re-size Times array if necessary (different Count than last time or
it hasn't been allocated at all yet). Then get Count values from netcdf file 
for HistoryVarname, starting at filestart, and fill the Values array 
accordingly
*/
  if (!SameSize || !Times) {
   Times=(float *)realloc((void *)Times,(size_t)(Count*sizeof(float)));
   GetNetCDFVarValues("time_offset",filestart,Count,Times);
  }
  GetNetCDFVarValues(HistoryVarname,filestart,Count,Values);
 } else {
/*
get Count values from shared memory for HistoryVarname, starting at datastart,
and fill the Values array accordingly
*/
  GetRealTimeVarValues(HistoryVarname,datastart,Count,Values);
 }
 for (value=0; value<Count; value++) {
/*
format each of Count elements of Strings array with times and values for
history on variable HistoryVarname
*/
  if (post_processing_mode()) {
   (void)sprintf(&Strings[value*lenstring],"%s %f",
    (char *)get_time_from_base(GetDataStartTime()+(int)Times[value]),
    Values[value]);
  } else {
    point=((startpoint+value)%get_datablock_size());
   (void)sprintf(&Strings[value*lenstring],"%02d:%02d:%02d %f",
    GetHMS(point*3),GetHMS(point*3+1),
    GetHMS(point*3+2),Values[value]);
  }
 }
}

/**************************************************************************/
SetPMS1DnetCDF()
/*
stub to facililate linking; this should be addressed correctly when there's
time
*/
{
}

/**************************************************************************/
void ShowHistory()
/*
show history on current history variable
*/
{
 char title[256];

 GetHistoryToStrings(HistoryVarname);
 CurrentHeaderPrintFunc=NULL; 
 (void)sprintf(title,"%s %s\n",HistoryVarname,TimeFrameString);
 SetCustomListTitle(title);
 FillScrollListFromStrings((char *)GetCustomListTitle());
 ShowTopOfMenu(VarPanelList);
 SetExclusiveChoices(VarPanelList,ERROR,FALSE);
}

/**************************************************************************/
void DeleteSelection()
/*
if internal mgmt indicates a variable is to be deleted, notify variable 
selector module by calling appropriate setup procedure to delete that selection.
In either event, dismiss the panel.
*/
{
 DeleteTrace();
 DismissScrollPanel();
}

/**************************************************************************/
void InstallSelection(item,event)
Panel_item item;
Event *event;
/*
if internal mgmt indicates a variable is selected, notify variable selector
module by calling appropriate setup procedure to install that selection.
In either event, dismiss the panel
*/
{
 if (CurrentPanelList==VarPanelList) {
  if (InstallValue!=ERROR) {
   SetSelectionMade(TRUE);
   InstallVariable(InstallValue);
   InstallValue=ERROR;
  }
 } else if (CurrentPanelList==NewSetupPanelList) {
  int quadrant,done;
  InstallNewSetup();
  done=TRUE;
  for (quadrant=0; quadrant<get_num_displays(); quadrant++) {
   if (GetNewSetupNeeded(quadrant)) {
    done=FALSE;
    break;
   }
  }
  if (!done) {
/*
go ahead and leave New Setup menu up after an install, to facilitate initiating
other new setups needed. Do what's needed to show quadrant just installed and
prepare for installing another one.
*/
   UpdateDisplay(CurrentSetupQuadrant);
   unhide_window(CurrentSetupQuadrant);
   ShowPanelItem(ScrollPanel);
   return;
  }
 }
 DismissScrollPanel();
}

/**************************************************************************/

SetDeleteButtonLabel()
/*
set label on delete button to reflect name of variable that will be
deleted
*/
{
 static char label[64];

 (void)sprintf(label,"Delete %s",(char *)GetHilitedVarName());
 xv_set(DeleteButton,
  PANEL_LABEL_STRING,label,
  NULL);
}

/**************************************************************************/

SetHistoryButtonLabel(varname)
char *varname;
/*
set label on HistoryButton as function of varname
*/
{
 static char label[64];

 (void)sprintf(label,"%s History",varname);
 SetPanelLabelString(HistoryButton,label);
}

/**************************************************************************/

SetApplyButtonLabel(change,target,newvar)
int change;
char *target, *newvar;
/*
set label on ApplyButton as function of change: if TRUE, reflect that the
operation is Change, else if FALSE: if newvar is TRUE, it's an Add operation;
else it's simply an Apply operation.
*/
{
 static char label[64];

 if (change) {
  if (target && newvar)
   (void)sprintf(label,"Chg %s to %s",target,newvar);
 } else {
  if (newvar)
   (void)sprintf(label,"Add %s",newvar);
  else
   (void)sprintf(label,"Apply");
 }
 SetPanelLabelString(ApplyButton,label);
}

/**************************************************************************/

UpdateButtonLabels(panellist,scrolllistposn)
Panel_item panellist;
int scrolllistposn;
/*
set labels on Apply and Delete buttons in panellist as function of type of
pending operation and hilited variable name selected for change in 
display module.
*/
{
 int changevar,showing_on_entry;
 char *hilitedvar;

 if (panellist!=CurrentPanelList)
/*
don't bother
*/
  return;

 if (scrolllistposn==ERROR) {
/*
simple Apply operation
*/
  SetApplyButtonLabel(FALSE,(char *)NULL,(char *)NULL);
  return;
 }
 hilitedvar=(char *)GetHilitedVarName();
 changevar=(hilitedvar && strlen(hilitedvar));
/*
start by hiding buttons that are about to be adjusted to
minimize irritating flashing
*/
 showing_on_entry=xv_get(ApplyButton,XV_SHOW);
 if (showing_on_entry) {
/*
only bother hiding, and subsequently re-showing, items if they are not 
already hidden; if they are, then some routine up the stack from here
has done it and is in charge of re-showing. Re-showing here will only
cause flashing, which is what we are trying to eliminate.
*/
  HidePanelItem(ApplyButton);
  HidePanelItem(HistoryButton);
  if (post_processing_mode()) {
   HidePanelItem(StartTimeButton);
   HidePanelItem(EndTimeButton);
  } else {
   HidePanelItem(TimeFrameMessage);
  }
  HidePanelItem(DeleteButton);
 }
 if (changevar) {
/*
Change operation 
*/
  SetHistoryVarname(GetScrollListVarname(scrolllistposn));
  SetApplyButtonLabel(TRUE,(char *)GetHilitedVarName(),
   GetScrollListVarname(scrolllistposn));
  SetHistoryButtonLabel(GetScrollListVarname(scrolllistposn));
  SetItemInactive(HistoryButton,NumScrollListEntries==0);
  if (post_processing_mode()) {
   SetItemInactive(StartTimeButton,NumScrollListEntries==0);
   SetItemInactive(EndTimeButton,NumScrollListEntries==0);
  }
  SetItemInactive(ApplyButton,NumScrollListEntries==0);
  PositionToRightOfButton(DeleteButton,ApplyButton);
  PositionToRightOfButton(HistoryButton,DeleteButton);
  if (post_processing_mode()) {
   PositionToRightOfButton(StartTimeButton,HistoryButton);
   PositionBelowButton(EndTimeButton,StartTimeButton);
  } else {
   PositionToRightOfButton(TimeFrameMessage,HistoryButton);
  }
/*
Delete button is active only if display module allows it and this is not
a vector component change or an independent variable
*/
  SetItemInactive(DeleteButton,!TraceIsDeletable(GetCurrentQuadrant()) || 
   GetChgVectorXComp() || GetChgVectorYComp() || 
   GetVarIsIndep(GetCurrentQuadrant()));
  SetDeleteButtonLabel();
 } else {
/*
Add operation
*/
  SetApplyButtonLabel(FALSE,(char *)NULL,GetScrollListVarname(scrolllistposn));
  SetHistoryButtonLabel(GetScrollListVarname(scrolllistposn));
  PositionToRightOfButton(DeleteButton,ApplyButton);
  PositionToRightOfButton(HistoryButton,DeleteButton);
  if (post_processing_mode()) {
   PositionToRightOfButton(StartTimeButton,HistoryButton);
   PositionBelowButton(EndTimeButton,StartTimeButton);
  } else {
   PositionToRightOfButton(TimeFrameMessage,HistoryButton);
  }
/*
no delete allowed here
*/
  SetItemInactive(DeleteButton,TRUE);
  SetItemInactive(HistoryButton,TRUE);
  if (post_processing_mode()) {
   SetItemInactive(StartTimeButton,TRUE);
   SetItemInactive(EndTimeButton,TRUE);
  }
 }
/*
re-show
*/
 if (showing_on_entry) {
/*
...i.e., if now is the right time
*/
  ShowPanelItem(HistoryButton);
  ShowPanelItem(ApplyButton);
  if (post_processing_mode()) {
   ShowPanelItem(StartTimeButton);
   ShowPanelItem(EndTimeButton);
  } else {
   ShowPanelItem(TimeFrameMessage);
  }
  ShowPanelItem(DeleteButton);
 }
}

/**********************  Statistics Menu List  ****************************/

ShowStatsMenu(which,event)
int which;
Event *event;
/* 
remove any other panel menu that is showing, if any, and do what must be
done to show statistics menu list.
*/ 
{
 CurrentPanelList=StatsPanelList;
 CreateStatsFlushMenu();
 RemoveCurrentPanelMenu();
 SetCurrentPanelMenu(which);
 HidePanelItem(ApplyButton);
 HidePanelItem(DeleteButton);
 HidePanelItem(HistoryButton);
 if (post_processing_mode()) {
  HidePanelItem(StartTimeButton);
  HidePanelItem(EndTimeButton);
 } else {
  HidePanelItem(TimeFrameMessage);
 }
 HidePanelItem(PlotTypeButton);
 HidePanelItem(ChgAttrsButton);
 ShowPanelItem(PrintWindowButton);
 PositionToRightOfButton(PrintWindowButton,StatsFlushMenuButton);
 PositionToRightOfButton(SelectAllButton,PrintWindowButton);
 PositionToRightOfButton(ResetFromFileButton,SelectAllButton);
 HidePanelItem(VarTypeButton);
 HidePanelItem(CurrentQuadButton);
 HidePanelItem(VarPanelList);
 HidePanelItem(AttrsPanelList);
 HidePanelItem(NewSetupPanelList);
 ShowPanelItem(SaveSelectionsButton);
 ShowPanelItem(StatsAutoPrintButton);
 ShowPanelItem(StatsIntervalButton);
 ShowPanelItem(StatsFlushMenuButton);
 ShowPanelItem(NextFlushMessage);
 ShowPanelItem(SelectAllButton);
 ShowPanelItem(ResetFromFileButton);
 SetExclusiveChoices(StatsPanelList,FALSE,TRUE);
 SetScrollPanelDims(GetBaseFrameWidth());
 ShowScrollPanel(StatsPanelList,ERROR,event,"Statistics Menu");
}
 
/**************************************************************************/

SetStatsSeln(which,set)
int which,set;
/*
set which'th member of statistics selection array to set. This array corresponds
one-to-one with top level MenuList and designates which variables are to be
output on next stats flush.
*/
{
 StatsSeln[which]=set;
}

/**************************************************************************/

char *
GetStatsSeln(which)
int which;
/*
return the variable name of the which'th selection saved for output in next 
stats flush, if indeed there are that many; if not, return NULL
*/
{
 int entry,found;

 for (found=0,entry=0; entry<GetNumMenuEntries(); entry++) {
  if (StatsSeln[entry]) {
   if ((found++)==which)
    return (char *)GetMenuListName(entry);
  }
 }
 return (char *)NULL;
}

/**************************************************************************/

InitStatsPanelList()
/*
allocate storage for internal mgmt of statistics output selections. Call
needed functions in statistics module for its indexing needs; init list
with selections for output as obtained from stats selections file; and
init the label on the toggle-all button as apropos.
*/
{
 int posn;

 StatsSeln=(int *)0;
 StatsSeln=(int *)realloc((void *)StatsSeln,
  (size_t)(GetNumMenuEntries()*sizeof(int)));
 StatsSelnNames=(char **)NULL;
 StatsSelnNames=(char **)realloc((void *)StatsSelnNames,
  (size_t)(GetNumMenuEntries()*sizeof(char *)));
 for (posn=0; posn<GetNumMenuEntries(); posn++) {
  StatsSelnNames[posn]=(char *)NULL;
  StatsSelnNames[posn]=(char *)realloc((void *)StatsSelnNames[posn],
   (size_t)NAMLEN);
  StatsSeln[posn]=0;
 }
 CreateStatsIndexArray();
 InitStatsSelnsFromFile();
 SetSelectAllButtonLabel();
}

/**************************************************************************/

char *
GetSelectAllButtonLabel()
/*
if all currently listed variables are selected for stats output, return 
the string that indicates next toggle will unselect all; else, return 
string that indicates next toggle will select all.
*/
{
 if (GetAllStatsSelected())
  return "DeselectAll";
 else
  return "SelectAll";
}

/**************************************************************************/

SetSelectAllButtonLabel()
/*
Set label on toggle button as function of whether all items are currently
selected or not; position and show it afterwards, in case its length increased
from label change and underlying right portion got hidden by neighboring 
button.
*/
{
 SetPanelLabelString(SelectAllButton,(char *)GetSelectAllButtonLabel());
 PositionToRightOfButton(ResetFromFileButton,SelectAllButton);
 ShowPanelItem(SelectAllButton);
}

/**************************************************************************/

GetAllStatsSelected()
/*
if all currently listed variables in list are selected, return TRUE; else,
return FALSE
*/
{
 if (GetNumItemsSelected(StatsPanelList)==NumScrollListEntries)
  return TRUE;
 else
  return FALSE;
}

/**************************************************************************/

void
ResetFromFileProc()
/*
init stats selections from file
*/
{
 InitStatsSelnsFromFile();
 SetSelectAllButtonLabel();
}

/**************************************************************************/

void
ToggleAllProc()
/*
toggle all currently listed variables either off or on, as function of 
whether all or currently selected or not, and update the toggle button
label accordingly to reflect which way next toggle will go.
*/
{
 SetAllStatsSelns(!GetAllStatsSelected());
 SetSelectAllButtonLabel();
}

/**************************************************************************/

SetAllStatsSelns(select)
/*
Set all currently listed statistics variables as per select, to include 
"entries" (internal mgmt) and "items" (graphical objects)
*/
{
 SelectAllStatsEntries(select);
 SetAllPanelItems(StatsPanelList,select);
}

/**************************************************************************/

SelectAllStatsEntries(select)
int select;
/*
Set the entries (internal mgmt) of all currently listed statistics variables 
as per select
*/
{
 int posn;

 for (posn=0; posn<NumScrollListEntries; posn++) {
  SetStatsEntry(GetScrollListVarname(posn),select,FALSE);
 }
}

/**************************************************************************/

SetStatsEntry(string,on,force)
char *string;
int on,force;
/*
set the statistic entry (internal mgmt) corresponding to string to on and
update counter of selections, only if it currently does not have the value
on OR force is TRUE.
*/
{
 int posn;

 if ( (posn=GetMenuListPosn(string))!=ERROR) {
  if (on) {
   if (!GetVarInStatsOutput(posn) || force) {
    SetStatsListSelected(posn,TRUE);
    NumStatsSelns++; 
   }
  } else {
   if (GetVarInStatsOutput(posn) || force) {
    SetStatsListSelected(posn,FALSE);
    NumStatsSelns--; 
   }
  }
 }
}

/**************************************************************************/

SetStatsListSelected(posn,on)
int posn,on;
/*
notify statistics module that variable in position posn (in the top level
MenuList) is to be toggled to value on (selection for output)
*/
{
 SetVarInStatsOutput(posn,on);
 StatsSeln[posn]=on;
}

/**************************************************************************/

SetAllOutputVars(set)
int set;
/*
set internal and graphical notion of all variables (in top level MenuList)
to set (selection for stats output)
*/
{
 int posn,num;

 num=GetNumMenuEntries(); 
 for (posn=0; posn<num; posn++)
  SetStatsSeln(posn,set);
}

/**************************************************************************/

InitStatsSelnsFromFile()
/*
hide stats panel to preclude flashing, while its list is initialized from
file. If no file exists, all currently listed items are to be selected for
output on next stats flush, else only those items listed in file. If file
exists but is empty, no items are selected. Re-show stats panel when done.
*/
{
 char *varname;

 HidePanelItem(StatsPanelList);
 SetAllPanelItems(StatsPanelList,FALSE);
 SetAllOutputVars(FALSE);
 NumStatsSelns=0;
 if (OpenStatsSelnsFile("r")==FALSE) {
  SetAllStatsSelns(TRUE);
 } else {
  while ((varname=(char *)GetNextStatsSeln())!=(char *)NULL) {
   SetStatsListSelected(GetMenuListPosn(varname),TRUE);
   SetStatsEntry(varname,TRUE,TRUE);
  }
  SetSelectionsFromList(StatsPanelList,NumStatsSelns,GetStatsSeln);
 } 
 ShowPanelItem(StatsPanelList);
}

/**************************************************************************/

void
ToggleStatsAutoPrint(item,value)
Panel_item item;
int value;
/*
notify stats module to reverse on/off-ness of autoprint operation
*/
{
  toggle_stats_printing();
}

/**************************************************************************/

ChgFlushMessage(string)
char *string;
/*
set next flush message to string
*/
{
 char message[128];

 (void)sprintf(message,"NextFlush: %s",string);
 SetPanelLabelString(NextFlushMessage,message);
}

/**************************************************************************/

ShowStatsFlush(menu,item)
Menu menu;
Menu_item item;
/*
display statistics flush as saved in file designated by selected item in
menu menu. Set choices to disabled and hide non-relevant buttons.
*/
{

 (void)sprintf(CurrentTextFile,"%s",GetStatsFilename(xv_get(item,MENU_VALUE)));
 HidePanelItem(StatsPanelList);
 CurrentHeaderPrintFunc=NULL; 
 SetCustomListTitle((char *)GetStatsFlushListTitle());
 FillScrollListFromTextFile((char *)GetCustomListTitle());
 SetExclusiveChoices(StatsPanelList,ERROR,FALSE);
 HidePanelItem(SelectAllButton);
 HidePanelItem(ResetFromFileButton);
 ShowPanelItem(StatsPanelList);
}

/**************************************************************************/

CreateStatsFlushMenu()
/*
if there are any items already created in StatsFlushMenu, save the assoc'd
values, and destroy StatsFlushMenu. In either event, proceed to create menu
StatsFlushMenu and fill it with nitems items/values as saved, if any; attach
it to its parent button StatsFlushMenuButton
*/
{
 int which,nitems;
 int values[MAXFLUSHSAVES];

 nitems=GetNumStatsFlushMenuItems();
 for (which=1; which<=nitems; which++) {
  values[which-1]=xv_get(xv_get(StatsFlushMenu,MENU_NTH_ITEM,which),
   MENU_VALUE);
 }
 if (StatsFlushMenu)
  xv_destroy_safe(StatsFlushMenu);
 StatsFlushMenu=xv_create((int)NULL,MENU,
 NULL);
 for (which=1; which<=nitems; which++) {
  xv_set(StatsFlushMenu,
   MENU_ITEM,
    MENU_STRING, FlushMenuString[which-1],
    MENU_VALUE, values[which-1],
    MENU_NOTIFY_PROC, ShowStatsFlush,
    0,
   NULL);
 }
 if (StatsFlushMenuButton)
  xv_set(StatsFlushMenuButton,
   PANEL_ITEM_MENU,StatsFlushMenu,
   NULL);
}

/**************************************************************************/

InitStatsFlushMenu()
/*
if StatsFlushMenu has not yet been created, remove all leftover stats flush
and print window files and create StatsFlushMenu
*/
{
 if (StatsFlushMenu)
  return;
 RemoveStatsFlushFiles();
 RemovePrintWindowFiles();
 CreateStatsFlushMenu();
}

/**************************************************************************/

RemoveStatsFlushFiles()
/*
remove any leftover statistics flush output files
*/
{
 FindFiles(GetStatsPrefix(),GetStatsFileDir(),unlink,NULL,
  strlen((char *)GetStatsPrefix()));
}

/**************************************************************************/

GetNumStatsFlushMenuItems()
/*
return number of items in StatsFlushMenu
*/
{
 if (StatsFlushMenu)
  return xv_get(StatsFlushMenu,MENU_NITEMS);
 else
  return 0;
}

/**************************************************************************/

ShiftMenuItems(suffix,starttime,endtime)
int suffix;
char *starttime,*endtime;
/*
manage strings in StatsFlushMenu to add a new one when the maximum
number of items are already there by removing the string at top of menu.
*/
{
 Menu_item item;
 int which,stringindex,value;
 int numitems=GetNumStatsFlushMenuItems();
/*
menu item numbering starts at 1
*/
 for (which=1; which<=numitems; which++) {
/*
...but index into associated strings starts at zero...
*/
  stringindex=which-1;
  item=xv_get(StatsFlushMenu,MENU_NTH_ITEM,which);
  if (which<numitems) {
/*
shift strings up one, get value of item with which it was associated
*/
   (void)sprintf(FlushMenuString[stringindex],"%s",FlushMenuString[which]);
   value=xv_get(xv_get(StatsFlushMenu,MENU_NTH_ITEM,which+1),MENU_VALUE);
  } else {
/*
init new string, assign argument suffix to value
*/
   (void)sprintf(FlushMenuString[stringindex],"%s to %s",starttime,endtime);
   value=suffix;
  }
/*
set the menu string and value
*/
  xv_set(item,
   MENU_STRING, FlushMenuString[stringindex],
   MENU_VALUE, value,
   NULL);
 }
}

/**************************************************************************/

AddToStatsFlushMenu(filenamesuffix,starttime,endtime)
int filenamesuffix;
char *starttime,*endtime;
/*
add a new item to the stats-flush menu, with string describing the start and
end times of flush period, and a value that points to disk file with stats
flush printout to be used for display if user selects this menu item.
*/
{
 int stringindex;
 int numitems=GetNumStatsFlushMenuItems();

 stringindex=numitems;
 if (numitems==MAXFLUSHSAVES) {
/*
need to make room for new menu item
*/
  ShiftMenuItems(filenamesuffix,starttime,endtime);
 } else {
  (void)sprintf(FlushMenuString[stringindex],"%s to %s",starttime,endtime);
  xv_set(StatsFlushMenu,
   MENU_ITEM,
    MENU_STRING, FlushMenuString[stringindex],
    MENU_VALUE, filenamesuffix,
    MENU_NOTIFY_PROC, ShowStatsFlush,
    0, 
   NULL);
 }
 if (xv_get(StatsFlushMenuButton,PANEL_INACTIVE)==TRUE)
/*
this menu button is inactivate until first flush is done
*/
  xv_set(StatsFlushMenuButton,PANEL_INACTIVE,FALSE,NULL);
}

/**************************************************************************/

Panel_setting
ChgStatsInterval(item)
Panel_item item;
/*
change interval between autoprints to value of keyboard text input item
*/
{
 char *interval;  
 int temp;

 if ( (interval=(char *)strdup((char *)GetButtonValue(item))) ) {
  if (sscanf(interval,"%d",&temp) != 1) {
   set_notice_needed(NON_NUMERIC_VALUE_ENTERED);
   SetStatsIntervalButtonValue(GetCurrentStatsInterval());
   return(PANEL_NEXT);
  }
 } else {
/* 
null string: take no action
*/
  SetStatsIntervalButtonValue(GetCurrentStatsInterval());
  return PANEL_NEXT;
 }
  if (!temp) {
   set_notice_needed(ZERO_UNREASONABLE);
   SetStatsIntervalButtonValue(GetCurrentStatsInterval());
  } else {
   if (temp != GetCurrentStatsInterval()) {
/* 
don't take any action if value entered is same as previous; this happens
when cursor is in control panel and user simply hits a <r>
*/
    SetCurrentStatsInterval(temp);
/* 
tell printer to dump current accumulations and restart with new interval
*/
    FlushStatsAccumulations();
   }
  }
  return(PANEL_NEXT);
}

/**************************************************************************/

SetStatsIntervalButtonValue(value)
int value;
/*
set value of the interval text input item to value
*/
{
char interval[64];

 (void)sprintf(interval,"%d",value);
 SetButtonMessage(StatsIntervalButton,interval);
}

/**************************************************************************/

void
SaveSelnsToFile(item,value)
Panel_item item;
int value;
/*
write out names of all variables currently selected for stats flush output
to file
*/
{
 int selected;

 for (selected=0; selected < NumStatsSelns; selected++) {
  (void)sprintf(StatsSelnNames[selected],"%s",
   (char *)GetStatsSeln(selected));
 }
 WriteStatsSelnsFile(StatsSelnNames,NumStatsSelns);
}

/*********************  Attributes Menu List  *****************************/

ShowAttrsMenu(which,event)
int which;
Event *event;
/*
remove any other panel menu that is showing, if any, and do what must be
done to show attributes menu list.
*/
{
 CurrentPanelList=AttrsPanelList;
 RemoveCurrentPanelMenu();
 SetCurrentPanelMenu(which);
/*
leave ChgAttrsButton out until there's time to get to it: need to get
categories and titles (and, might as well do ranges, etc.) all from
VarDB, eliminating bulletin-9 and plot-attrs.
*/
 /***Show***/HidePanelItem(ChgAttrsButton);
 ShowPanelItem(PrintWindowButton);
 PositionToRightOfButton(PrintWindowButton,DismissButton);
/*
if user elects to print window, this menu needs a header printed out first
*/
 CurrentHeaderPrintFunc=PrintAttributesHeader; 
 HidePanelItem(ApplyButton);
 HidePanelItem(DeleteButton);
 HidePanelItem(HistoryButton);
 if (post_processing_mode()) {
  HidePanelItem(StartTimeButton);
  HidePanelItem(EndTimeButton);
 } else {
  HidePanelItem(TimeFrameMessage);
 }
 HidePanelItem(PlotTypeButton);
 HidePanelItem(VarTypeButton);
 HidePanelItem(CurrentQuadButton);
 HidePanelItem(SaveSelectionsButton);
 HidePanelItem(StatsAutoPrintButton);
 HidePanelItem(StatsIntervalButton);
 HidePanelItem(StatsFlushMenuButton);
 HidePanelItem(NextFlushMessage);
 HidePanelItem(SelectAllButton);
 HidePanelItem(ResetFromFileButton);
 HidePanelItem(VarPanelList);
 HidePanelItem(NewSetupPanelList);
 HidePanelItem(StatsPanelList);
 SetExclusiveChoices(AttrsPanelList,ERROR,FALSE);
 SetScrollPanelDims(2*GetBaseFrameWidth()/3);
 ShowScrollPanel(AttrsPanelList,ERROR,event,"Attributes Menu");
}

/**************************************************************************/

void
PrintAttributesHeader(fd)
FILE *fd;
/*
information to be printed as header when user elects to print Attributes
list window 
*/
{
 (void)fprintf(fd,
  "PROJECT %s (%s) -- Aircraft %s -- Date %s\n%s\n\n%-16s%s\n%-16s%s\n\n",
  (char *)get_proj_number(),(char *)GetWINDSProjectName(),
  (char *)GetWINDSAircraftName(),(char *)get_date(),
  (char *)GetPanelListTitle(),
  "NAME","TITLE",
  "----","-----");
}

/**************************************************************************/

void
ModifyAttributes(item,event)
Panel_item item;
Event *event;
/*
bring up the process that allows users to change attributes; re-init all
lists to reflect any changes made.
*/
{
 FILE *pipe;
 char command[256];

 (void)sprintf(command,"%s %s",(char *)GetAttrModProgName(),
  (char *)GetVarAttrsFileName());
 pipe=popen(command,"r");
 (void)pclose(pipe);
 GetVarAttrs();
 InitMenuList();
 FillScrollList(CurrentFilter,CurrentFilteredListTitle);
}

/*********************  Item Selection Management  ***************************/

SetSelectedVars(panellist,string,value,on)
Panel_item panellist; 
char *string;
int value,on;
/*
manage behavior and internal info as per selected variable
*/
{
 static char varname[NAMLEN];
 int select;
 static int NoChoicesAllowed=FALSE;

 NoChoicesAllowed=FALSE;
 (void)sscanf(string,"%s",varname);
 select=GetScrollListPosn(varname);
 if (!GetListFromVarFilter()) {
  NoChoicesAllowed=TRUE;
 } else if (panellist==VarPanelList) {
/*
remember the value used to install the variable in plot display if user
indicates this is to be done
*/
  InstallValue=value;
  if (on)
   UpdateButtonLabels(panellist,select==ERROR?0:select);
 } else if (panellist==NewSetupPanelList) {
  if (on && 
   !GetVarTypeIsExclusive() && GetNumNewSetupSelns() >= GetMaxNewSetupSelns())
/*
unfortunately, the maximum # of selections is already made; unselect this item
*/
   SetPanelSelection(panellist,select,FALSE);
  else
   UpdateNewSetupSelns(varname,on);
 } else if (panellist==StatsPanelList) {
/*
save selection name to list of non-exclusive selections
*/
  SetStatsEntry(varname,on,FALSE);
  SetSelectAllButtonLabel();
 } else if (panellist==AttrsPanelList) {
   NoChoicesAllowed=TRUE;
 }
 if (NoChoicesAllowed) {
/*
turn off immediately any entry user has selected: no need or use for it here
*/
  select=GetNextSelection(panellist,0,TRUE);
  SetPanelSelection(panellist,select,FALSE);
 }
}

/**************************************************************************/

SetAllPanelItems(panellist,set)
Panel_item panellist; 
int set;
/*
if indeed it's not already done, set all items in panellist to set.
*/
{
 int item,numselected;

 numselected=GetNumItemsSelected(panellist);
 if ( (!set && !numselected) ||
      (set  && numselected==NumScrollListEntries))
  return;
/*
must make sure it is non-exclusive first before being able 
to clear or set all vars
*/
 SetItemExclusive(panellist,FALSE);
/*
hide and then re-show panel to avoid irritating flashing while setting items
*/
 HidePanelItem(panellist);
 for (item=0; item<NumScrollListEntries; item++)
  SetPanelSelection(panellist,item,set);
 ShowPanelItem(panellist);
}

/****************************************************************************/

GetScrollListPosn(varname)
char *varname;
/*
search current list for a match with varname. If found, return its position 
in that list, else return ERROR. Search is done with limited length, case
insensitive.
*/
{
 int indx;       

 if (!varname || !strlen(varname))
  return ERROR;
 indx = 0;
 while (indx < NumScrollListEntries) {
  if ( (strncasecmp((char *)ScrollList[indx].string,varname,strlen(varname)))==0){
   return(indx);
  } 
  indx++;
 }
 return(ERROR);
}

/**************************************************************************/

char *
GetScrollListVarname(posn)
int posn;
/*
extract the first word from the posn'th ScrollList string, presumed to
be the name of given variable
*/
{
 static char varname[NAMLEN];

 if (NumScrollListEntries) {
  (void)sscanf(ScrollList[posn].string,"%s",varname);
  return varname;
 } else {
  InstallValue=ERROR;
  return "???";
 }
}

/**************************************************************************/

SetPanelSelections(panellist)
Panel_item panellist;
/*
For Variable Selector Menu:

determine whether a variable name is currently highlighted (i.e., the 
operation is "change") by getting the current highlighted variable name.
If that name is empty (i.e., the operation must be "add"), select the name at 
top of panel list; else select highlighted name in list (if possible, as per
sort with which list is currently filtered...) and update internal mgmt of
selected variable.

For Attributes Menu:

Set no selections; just show top of menu

For Statistics Menu:

set selections from current list, show top of menu

For New Setup Menu:

set selections from current list, show top of menu, update button labels   
*/
{
 int select;

 if (!GetListFromVarFilter()) {
  ShowTopOfMenu(panellist);
  return;
 } else if (panellist==VarPanelList) {
/*
Var menu
*/
  select=GetScrollListPosn((char *)GetHilitedVarName());
  SetPanelSelection(panellist,select==ERROR?0:select,TRUE);
  UpdateButtonLabels(panellist,select==ERROR?0:select);
 } else if (panellist==AttrsPanelList ) {
/*
Attributes menu
*/
  ShowTopOfMenu(panellist);
 } else if (panellist==NewSetupPanelList ) {
/*
New Setup menu
*/
  SetSelectionsFromList(panellist,GetNumNewSetupSelns(),GetNewSetupSeln);
  ShowTopOfMenu(panellist);
  UpdateButtonLabels(panellist,ERROR);
 } else if (panellist==StatsPanelList) {
/*
Statistics menu
*/
  SetSelectionsFromList(panellist,NumStatsSelns,GetStatsSeln);
  ShowTopOfMenu(panellist);
 }
}

/****************************************************************************/

SetSelectionsFromList(panellist,numselns,selns)
Panel_item panellist;
int numselns;
char *(*selns)();
/*
set selected items in panellist as per the numselns selectons in char
array selns
*/
{
 int entry,select;

 if (numselns) {
/*
hide to avoid irritating flashing
*/
  HidePanelItem(panellist);
  for (entry=0; entry<numselns; entry++) {
   select=GetScrollListPosn(selns(entry));
   if (select!=ERROR)
    SetPanelSelection(panellist,select,TRUE);
  }
  if (panellist==CurrentPanelList)
   ShowPanelItem(panellist);
 }
}

/****************************************************************************/

SetPanelSelection(panellist,entry,select)
Panel_item panellist;
int entry,select;
/*
set selected value of the entry'th item in panellist to select
*/
{
 SetSelectedPanelItem(panellist,entry,select);
 if (panellist==VarPanelList && GetListFromVarFilter()) {
/*
remember select value for future installation needs
*/
  InstallValue=select?ScrollList[entry].client_data:ERROR;
  SetItemInactive(ApplyButton,NumScrollListEntries==0 || InstallValue==ERROR);
 }
}

/*********************  General Layout/Resizing  **************************/

SetOverallLayout(panellist,quadrant)
Panel_item panellist;
int quadrant;
/*
dimensions. Adjust # of rows in category widget to fit inside the width of 
panel; then adjust scrolling list dimensions and y-position, and finally
adjust width of keyboard text input item. 
*/
{
 AdjustNrows(CategoryButton,xv_get(ScrollPanel,XV_WIDTH));
 SetPanelListLayout(panellist,CurrentDomainButton);
 SetKeyboardButtonWidth();
}

/*************************************************************************/

GetScrollPanelWidth()
/*
return actual width of overall panel
*/
{
 if (ScrollPanel)
  return xv_get(ScrollPanel,XV_WIDTH);
 else
  return ERROR;
}

/*************************************************************************/

GetScrollPanelHeight()
/*
return actual height of overall panel
*/
{
 return xv_get(ScrollPanel,XV_HEIGHT);
}

/*************************************************************************/

GetNominalScrollPanelWidth()
/*
return nominal width of overall panel
*/
{
 return GetBaseFrameWidth()/2;
}

/*************************************************************************/

GetNominalScrollPanelHeight()
/*
return nominal height of overall panel
*/
{
 return 2*GetBaseFrameHeight()/3;
}

/*************************************************************************/

SetScrollPanelDims(width)
int width;
/* 
place scroll panel in middle of WINDS window, adjusting x-position for width
*/
{
 int xposn,yposn,height; 

 height=GetNominalScrollPanelHeight(); 
 yposn=(GetBaseFrameHeight()-GetNominalScrollPanelHeight())/2;
 xposn=(GetBaseFrameWidth()-width)/2;
/*
do it
*/
 xv_set(ScrollPanel,
  XV_FONT, GetStandardFont(),
  XV_X,xposn,
  XV_Y,yposn,
  XV_HEIGHT,height,
  XV_WIDTH,width,
  NULL);
}

/**************************************************************************/

GetScrollBarWidth()
/*
return the (currently hard-wired) pixel width of scroll bar
*/
{
 return 35;
}

/*************************************************************************/

SetPanelListLayout(panellist,button)
Panel_item panellist;
Panel_item button;
/*
position scrolling list portion of panel so that it is below the button
indicated; determine its width as per parent panel and scroll bar width; 
determine how many rows it should display as per parent panel height and
position. 
*/
{
 xv_set(panellist,
  XV_Y,GetBottomOfButton(button),
  PANEL_LIST_WIDTH,GetScrollPanelWidth()-GetScrollBarWidth(),
  PANEL_LIST_DISPLAY_ROWS,
   (GetScrollPanelHeight()-GetBottomOfButton(button))/
/*
do the math but make it fewer rows by 3 to account for title and allow some
margin
*/
   xv_get(panellist,PANEL_LIST_ROW_HEIGHT)-3,
   NULL);
}

/**************************************************************************/

GetCharWidthOfPanel()
/*
return how many characters can fit across the width of panel as function
of its current width and width of current font
*/
{
 return (xv_get(ScrollPanel,XV_WIDTH)/2-GetScrollBarWidth())/
  GetFontWidth(GetStandardFont());
}

/**************************************************************************/

SetKeyboardButtonWidth()
/*
adjust width of text input item as per width of parent panel
*/
{
 xv_set(KeywordButton,
  PANEL_VALUE_DISPLAY_LENGTH,GetCharWidthOfPanel(),
  PANEL_VALUE_STORED_LENGTH,GetCharWidthOfPanel()*2,
  NULL);
}

/**********************  Sorting/Filtering of List  **********************/

/**************************************************************************/

SortByCategory(Panel_item item, int value)
/*
set internal keyword as per string associated with selected category button
and fill scrolling list with all variables in that category. Update internal 
mgmt (and list highlighting, if possible) of selected variable. 
*/
{
 static char title[128];

 SetUserKeyword((char *)xv_get(CategoryButton,PANEL_CHOICE_STRING,value));
 (void)sprintf(title,"in category '%s'",(char *)GetUserKeyword());
 SetListFromVarFilter(TRUE);
 FillScrollList(VarIsInCategory,title);
 CurrentSortButton=item;
 CurrentSortButtonValue=value;
 SetCanHaveZeroChoices(item,FALSE);
}

/**************************************************************************/

VarIsInCategory(int entry)
/*
return TRUE if entry'th Menu List variable is in current category as set with
internal keyword routine
*/
{
 return (!strcmp((char *)GetUserKeyword(),(char *)GetMenuListCategory(entry)));
}

/**************************************************************************/

SortByType(Panel_item item, int value)
/*
set internal keyword as per type associated with selected type button
and fill scrolling list with all variables of that type. Update internal 
mgmt (and list highlighting, if possible) of selected variable. 
*/
{
 SetListFromVarFilter(TRUE);
 (*ShowTypeFunc[value])();
 CurrentSortButton=item;
 CurrentSortButtonValue=value;
 SetCanHaveZeroChoices(item,FALSE);
}

/*************************************************************************/

VarIsDerived(int entry)
/*
return TRUE if entry'th Menu List variable is DERIVED
*/
{
 return GetMenuListMode(entry)==DERIVED;
}

/****************************************************************************/

VarIsDigital(int entry)
/*
return TRUE if entry'th Menu List variable is digital input
*/
{
 char *type; 

 if (VarIsDerived(entry))
  return FALSE;
 GetType(GetMenuListName(entry),&type);
 return is_digital_type(type); 
}

/****************************************************************************/

VarIsAnalog(int entry)
/*
return TRUE if entry'th Menu List variable is analog input
*/
{
 char *type; 

 if (VarIsDerived(entry))
  return FALSE;
 GetType(GetMenuListName(entry),&type);
 return !(is_digital_type(type)); 
}
 
/*************************************************************************/

VarIsRaw(int entry)
/*
return TRUE if entry'th Menu List variable is RAW
*/
{
 return GetMenuListMode(entry)==RAW;
}

/*************************************************************************/

SortByAll()
/*
type domain option: show ALL variables
*/
{
 FillScrollList(NULL,"all");
}

/*************************************************************************/

SortByDerived()
/*
type domain option: show all DERIVED variables
*/
{
 FillScrollList(VarIsDerived,"derived");
}

/*************************************************************************/

SortByDigital()
/*
type domain option: show all digital input variables
*/
{
 FillScrollList(VarIsDigital,"digital");
}

/*************************************************************************/

SortByAnalog()
/*
type domain option: show all analog input variables
*/
{
 FillScrollList(VarIsAnalog,"analog");
}

/*************************************************************************/

SortByRaw()
/*
type domain option: show all RAW variables
*/
{
 FillScrollList(VarIsRaw,"raw");
}

/*************************************************************************/

Panel_setting SortByKeyword(Panel_item item, Event *event)
/*
fill scrolling list with all variables whose name and/or title match the
keyword as set by user input; an empty string will match ALL variables.
Update internal mgmt (and list highlighting, if possible) of selected variable.
*/
{
 char *keyword;
 static char title[128];
/*
since the text input button is not ever un-XV_SHOW'n, it will still detect
<r> events when keyword matching is not the user-designated domain. Catch
and reject such events except keyword matching is the current domain.
*/
 if (CurrentDomainButton!=KeywordButton)
  return PANEL_NEXT;

 if ( (keyword=(char *)strdup((char *)GetButtonValue(item))) ) {
  SetUserKeyword(keyword);
  (void)sprintf(title,"all matching '%s'",keyword);
  SetListFromVarFilter(TRUE);
  FillScrollList(VarHasKeyword,title);
 }
 CurrentSortButton=item;
 return PANEL_NEXT;
}

/*************************************************************************/

char *GetUserKeyword()
/*
return the keyword used to matches for various filtering operations
*/
{
 return UserKeyword;
}

/*************************************************************************/

SetUserKeyword(char *keyword)
/*
set the keyword used to matches for various filtering operations
*/
{
 (void)sprintf(UserKeyword,"%s",keyword);
}

/*************************************************************************/

VarHasKeyword(int entry)
/*
if the current keyword is contained (case-insensitive matching) in either
the entry'th Menu List name or title, return TRUE, else return FALSE.
*/
{
 return (StringInString(GetUserKeyword(),GetMenuListName(entry),FALSE) ||
  StringInString(GetUserKeyword(),(char *)GetMenuListTitle(entry),FALSE));
}


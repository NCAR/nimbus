/* panels.c -- general routines for panel interfaces
*/
#include <stdio.h>
#include <xview/xview.h>
#include <xview/panel.h>

#include <help.h>
#include <lengths.h>
#include <constants.h>
#include <macros.h>
#include <colors.h>
#include <proto.h>

static int current_panel=NONE;
static int panel_event_debug=FALSE;
static int Panel_event=ERROR;

#define PANELFONTHEIGHTFACTOR 15
#define PANELFONTWIDTHFACTOR 114

/*****************************************************************************/

GetStandardFontHeight()
{
 return GetBaseFrameHeight()/PANELFONTHEIGHTFACTOR;
}

/*****************************************************************************/

GetStandardFontWidth()
{
 return GetBaseFrameWidth()/PANELFONTWIDTHFACTOR;
}

/*****************************************************************************/

RefreshPanels()
{
 int quadrant;

 for (quadrant=0; quadrant<get_num_displays(); quadrant++) {
  RefreshOpPanel(quadrant);
 }
 RefreshControlPanel();
}

/***************************  SET_CURRENT_PANEL()  **************************/

set_current_panel(panel)
int panel;
{
 current_panel=panel;
}

/***************************  GET_CURRENT_PANEL()  **************************/

get_current_panel()
{
 return current_panel;
}

/*************************************************************************/

DrawPanelVerticalEdges(panel)
Panel panel;
{
 int width,height,color;

 color=BLACK;
 width=xv_get(panel,XV_WIDTH); 
 height=xv_get(panel,XV_HEIGHT);
/*
left vertical edge, single width defining line
*/
 (*ContextDrawLine[GetDrawType()])(xv_get(panel,XV_XID),
  0,0,0,height-1,color,1,LineSolid);
/*
right vertical edge, single width defining line
*/
 (*ContextDrawLine[GetDrawType()])(xv_get(panel,XV_XID),
  width-1,height-1,width-1,0,color,1,LineSolid);
}

/*************************************************************************/

DrawPanelBorders(panel)
Panel panel;
{
 int width,height,color;

 color=BLACK;
 width=xv_get(panel,XV_WIDTH); 
 height=xv_get(panel,XV_HEIGHT);
/*
left vertical edge + 1, double width defining line
*/
 (*ContextDrawLine[GetDrawType()])(xv_get(panel,XV_XID),
  1,0,1,height-1,color,2,LineSolid);
/*
right vertical edge - 1, double width defining line
*/
 (*ContextDrawLine[GetDrawType()])(xv_get(panel,XV_XID),
  width-1,height-1,width-1,0,color,2,LineSolid);
/*
top horizontal edge + 1, double width defining line
*/
 (*ContextDrawLine[GetDrawType()])(xv_get(panel,XV_XID),
  0,1,width-1,1,color,2,LineSolid);
/*
bottom horizontal edge - 1, double width defining line
*/
 (*ContextDrawLine[GetDrawType()])(xv_get(panel,XV_XID),
  0,height-1,width-1,height-1,color,2,LineSolid);
}

/***********************  PANEL_REPAINT()  *******************************/

/*ARGSUSED*/
panel_repaint(panel,pw)
Panel panel;
Xv_Window pw;
{
Rect *cursor_posn;
int left,top,width,height;
Event event;
/* 
get parameters describing panel rectangle
*/
 width=xv_get(panel,XV_WIDTH); height=xv_get(panel,XV_HEIGHT);
 left=xv_get(panel,XV_X); top=xv_get(panel,XV_Y);
 DrawPanelVerticalEdges(panel);
/* 
double width lines, vertical AND horizontal edges on selected objects
*/
 if (/***xv_get(panel,PANEL_CLIENT_DATA) == HELPIMAGE_STATS_MENU_AREA ||
     xv_get(panel,PANEL_CLIENT_DATA) == HELPIMAGE_ATTRS_MENU_AREA || ***/
     xv_get(panel,PANEL_CLIENT_DATA) == HELPIMAGE_PRINT_MENU_AREA ||
     xv_get(panel,PANEL_CLIENT_DATA) == HELPIMAGE_PRINTCONFIG_MENU_AREA ||
     xv_get(panel,PANEL_CLIENT_DATA) == HELPIMAGE_PLOTSPEX_MENU_AREA ||
     xv_get(panel,PANEL_CLIENT_DATA) == HELPIMAGE_FOLDERS_MENU_AREA ||
     xv_get(panel,PANEL_CLIENT_DATA) == HELPIMAGE_SETUP_AREA   ||
     GetPanelIsScrollPanel(panel))
 {
  DrawPanelBorders(panel);
 }
/* 
if cursor is over exposed section, call event_proc with a LOC_WINENTER event 
to facilitate proper updating of state variables
*/
 cursor_posn=(Rect *)xv_get(GetBaseFrame(),WIN_MOUSE_XY);
 if (within_boundaries(cursor_posn->r_left,cursor_posn->r_top, 
     left,top,left+width-1,top+height-1)) {
  event.ie_code=LOC_WINENTER;
  event.ie_flags=0;
  event.ie_shiftmask=0;
  event.ie_locx=cursor_posn->r_left; 
  event.ie_locy=cursor_posn->r_top; 
  event.action=ACTION_NULL_EVENT;
  event.ie_win=xv_get(panel,CANVAS_NTH_PAINT_WINDOW,0);
  event.ie_xevent=(union _XEvent *)malloc((unsigned)(sizeof (union _XEvent)));
  event.ie_xevent->xbutton.state=0;
  (void)event_proc(event.ie_win,&event);
  free((char *)event.ie_xevent); 
 }
}

/***********************  PANEL_EVENT_PROC()  *******************************/

void
panel_event_proc(Panel_item item, Event *event)

/* as of OW V-3, enter/exit/ascii events won't be detected in the event proc
installed for the panel paint window -- they will come here, to the panel_event
proc, only.  Send them to the event proc when received, since that's where all
the processing has already been coded.
*/
{
  void (*Event_proc)();
  Window window;
  Window EventWindow;


 if (GetScrollPanelShowing(FALSE))
  return;

 if (panel_event_debug )
  if (event_id(event)!=LOC_MOVE && event_id(event)!=32525)
   printf("panel_event: %d in item %d\n",event_id(event),item);

 window=xv_get(item,PANEL_PARENT_PANEL);
 Event_proc=(void (*)())xv_get(window,WIN_EVENT_PROC);
 set_panel_event(event_id(event));
 RememberCursorPosn(event->ie_win,event->ie_locx,event->ie_locy);
 if (event_is_ascii(event)) {
  (void)Event_proc(window,event);
  panel_default_handle_event(item,event);
 } else {
  switch (event_id(event)) {
   case LOC_WINENTER:
    EventWindow=canvas_paint_window(window);
    win_set_kbd_focus(EventWindow,xv_get(EventWindow,XV_XID));
   case ACTION_HELP:
   case EVENT_HELP_KEY:
   case LOC_WINEXIT:
    (void)Event_proc(window,event);
    break;
   case MS_MIDDLE:
    break;
   case MS_RIGHT:
    event->action=ACTION_SELECT;
    event->ie_code=MS_LEFT;
   case MS_LEFT:
    ConstructItemChoices(item);
   default:
    panel_default_handle_event(item,event);
    break;
  }
 }
}

/**********************  Event Processing  ********************************/

Notify_value
ScrollPanelInterposer(panel, event, arg, type)
Panel panel;
Event *event;
Notify_arg arg;
Notify_event_type type;
/*
manage help images on entry/exit to/from panel; convert mouse right button
events to left events, so as to disable scrolling list editing; manage
keyboard focus; and pass along events to "next event handler"
*/
{
 Window EventWindow;
 Panel_item item;
 
 item=xv_get(panel,PANEL_CURRENT_ITEM);
 switch (event_id(event)) {
  case LOC_WINEXIT:
   break;
  case LOC_WINENTER:
   SetMouseWindow(panel);
   chg_tb_help_image(CTL_AREA);
   EventWindow=canvas_paint_window(panel);
   win_set_kbd_focus(EventWindow,xv_get(EventWindow,XV_XID));
   break;
  case MS_RIGHT:
   event->action=ACTION_SELECT;
   event->ie_code=MS_LEFT;
  case MS_LEFT:
   if (ItemIsSelectFolderButton(item)) {
/*
re-fill menu of folder names before each display in case a networked session
has changed it or local session has deleted one of the folders in it.
*/
    SetPlotspexFolderMenu();
   }
   break;
  case EVENT_HELP_KEY:
   xv_help_show(panel,xv_get(panel,XV_HELP_DATA),event);
   break;
  default:
   break;
 }
 notify_next_event_func(panel,(Notify_event)event,arg,type);
}

/****************************************************************************/

ShowTopOfMenu(panellist)
Panel_item panellist;
/*
show the first page (top) of menu, coerced by selecting the first entry.
If indeed the first entry was already selected, leave it; else, unselect 
it to return things to where they should be.
*/
{
 int selected=xv_get(panellist,PANEL_LIST_SELECTED,0);
 
 SetPanelSelection(panellist,0,TRUE);
 if (!selected)
  SetPanelSelection(panellist,0,FALSE);
}

/**************************************************************************/

SetSelectedPanelItem(panellist,entry,select) 
Panel_item panellist;
int entry,select;
{
 xv_set(panellist,
  PANEL_LIST_SELECT,entry,select,
  NULL);
}

/**************************************************************************/

GetNextSelection(panellist,last,first)
Panel_item panellist;
int last,first;
/*
if arg first is set, return position of first selection in panellist; else,
return position of the first selection after position last.
*/
{
 if (first)
  return xv_get(panellist,PANEL_LIST_FIRST_SELECTED);
 else
  return xv_get(panellist,PANEL_LIST_NEXT_SELECTED,last);
}

/**************************************************************************/

GetNumItemsSelected(panellist)
Panel_item panellist; 
/*
return number of items selected in panellist
*/
{
 static int NumItemsSelected=0;
 int selected=GetNextSelection(panellist,0,TRUE);

 NumItemsSelected=0;
 while ( selected!=-1) {
  NumItemsSelected++;
  selected=GetNextSelection(panellist,selected,FALSE);
 }
 return NumItemsSelected;
}

/**************************************************************************/

SetPanelLabelString(button,string)
Panel_item button;
char *string;
{
 xv_set(button,
  PANEL_LABEL_STRING,string,
  NULL);
}

/**************************************************************************/

SetPanelTitle(panel,item,title)
Panel panel;
Panel_item item;
char *title;
/*
set value of panel label string as a title in center at top of list
*/
{
 xv_set(item,PANEL_LABEL_STRING,title,NULL);
 xv_set(item,
  XV_X, (int)(xv_get(panel,XV_WIDTH)/2)-
   (int)(xv_get(item,XV_WIDTH)/2),
  NULL);
}

/**************************************************************************/

PositionToRightOfButton(item,neighbor)
Panel_item item,neighbor;
{
 xv_set(item,
  XV_X,xv_get(neighbor,XV_X)+xv_get(neighbor,XV_WIDTH),
  XV_Y,xv_get(neighbor,XV_Y),
 NULL);
}

/*************************************************************************/

HidePanelItem(button)
Panel_item button;
/*
make button not visible 
*/
{
 xv_set(button,XV_SHOW,FALSE,NULL);
}

/*************************************************************************/

ShowPanelItem(Panel_item button)
/*
make button visible
*/
{
 xv_set(button,XV_SHOW,TRUE,NULL);
}

/**************************************************************************/

SetButtonMinValue(button,value)
Panel_item button;
int value;
{
 xv_set(button,PANEL_MIN_VALUE,value,NULL);
}

/**************************************************************************/

SetButtonMaxValue(button,value)
Panel_item button;
int value;
{
 xv_set(button,PANEL_MAX_VALUE,value,NULL);
}

/**************************************************************************/

GetButtonMinValue(button)
Panel_item button;
{
 return xv_get(button,PANEL_MIN_VALUE);
}

/**************************************************************************/

GetButtonMaxValue(button)
Panel_item button;
{
 return xv_get(button,PANEL_MAX_VALUE);
}

/**************************************************************************/

GetButtonValue(button)
Panel_item button;
{
 if (button)
  return xv_get(button,PANEL_VALUE);
 else
  return 0;
}

/**************************************************************************/

SetButtonColor(button,color)
Panel_item button;
int color;
{
 if (button)
  xv_set(button,PANEL_ITEM_COLOR,color,NULL);
}

/**************************************************************************/

SetButtonValue(button,value)
Panel_item button;
int value;
{
 if (button)
  xv_set(button,PANEL_VALUE,value,NULL);
}

/**************************************************************************/

SetButtonLabel(button,label)
Panel_item button;
char *label;
{
 xv_set(button,PANEL_LABEL_STRING,label,NULL);
}

/**************************************************************************/

SetButtonMessage(button,msg)
Panel_item button;
char *msg;
{
 xv_set(button,PANEL_VALUE,msg,NULL);
}

/**************************************************************************/

GetButtonIsActive(button)
Panel_item button;
{
 return !xv_get(button,PANEL_INACTIVE);
}

/**************************************************************************/

SetChoiceString(button,choice,string) 
Panel_item button; 
int choice;
char *string;
{
 xv_set(button,
  PANEL_CHOICE_STRING,choice,string,
  NULL);
}

/**************************************************************************/

ClearChoiceStrings(button) 
Panel_item button; 
{
 xv_set(button,
  PANEL_CHOICE_STRINGS,"",NULL,
  NULL);
}

/*************************************************************************/

SetItemInactive(item,inactive)
Panel_item item;
int inactive;
{
 xv_set(item,
  PANEL_INACTIVE,inactive,
  NULL);
}

/*************************************************************************/

DeletePanelListRows(panellist,start,number)
Panel_item panellist;
int start,number;
/*
delete number rows from panellist, starting at row start
*/
{
 if (!number)
  return;
 xv_set(panellist,
  PANEL_LIST_DELETE_ROWS, start, number,
  NULL);
}

/*************************************************************************/

PositionBelowButton(button,anchor)
Panel_item button,anchor;
/*
position given button directly below the indicated anchor button
*/
{
 xv_set(button,
  XV_Y,xv_get(anchor,XV_Y)+xv_get(anchor,XV_HEIGHT)+5,
  XV_X,xv_get(anchor,XV_X),
  NULL);
}

/*************************************************************************/

SetAllChoicesOff(item)
Panel_item item;
{
  SetCanHaveZeroChoices(item,TRUE);
  xv_set(item,
   PANEL_VALUE,-1,
   NULL);
}

/*************************************************************************/

GetItemExclusive(item)
Panel_item item;
{
 return xv_get(item,
  PANEL_CHOOSE_ONE);
}

/*************************************************************************/

SetItemExclusive(item,exclusive)
Panel_item item;
int exclusive;
{
 if (xv_get(item,PANEL_CHOOSE_ONE)!=exclusive)
/*
only change attribute if needed to avoid irritating flashing
*/
  xv_set(item,
   PANEL_CHOOSE_ONE,exclusive,
   NULL);
 if (GetCanHaveZeroChoices(item)==exclusive)
  SetCanHaveZeroChoices(item,!exclusive);
}

/*************************************************************************/

GetCanHaveZeroChoices(item)
Panel_item item;
{
 return xv_get(item,
  PANEL_CHOOSE_NONE);
}

/*************************************************************************/

SetCanHaveZeroChoices(item,allowed)
Panel_item item;
int allowed;
{
 xv_set(item,
  PANEL_CHOOSE_NONE,allowed,
  NULL);
}

/**************************************************************************/

DestroyPanel(panel)
Panel panel;
/*
destroy scroll panel
*/
{
 xv_destroy_safe(panel);
}

/**************************************************************************/

AdjustNrows(button,width)
Panel_item button;
int width;
/*
adjust the number of rows on what is presumably a PANEL_CHOICE button so that
that button's width is decreased as much as it takes to fit "nicely" within 
prescribed width. We start out with PANEL_CHOICE_NROWS value of 1, i.e. the
button has one row only, regardless of its current number of rows or of how 
current widths compare, so that in case of resizing WINDS larger we'll get the 
fewest possible number of rows for aesthetic reasons (that's the "nicely" part)
*/
{
 int nrows=1;

 do {
  xv_set(button,PANEL_CHOICE_NROWS,nrows++,NULL);
 } while (xv_get(button,XV_WIDTH)>width);
}

/*************************************************************************/

GetHeightOfButton(button)
Panel_item button;
/*
return height of given button
*/
{
 return xv_get(button,XV_HEIGHT);
}

/*************************************************************************/

GetBottomOfButton(button)
Panel_item button;
/*
return y-coordinate of bottom of given button
*/
{
 return xv_get(button,XV_Y)+xv_get(button,XV_HEIGHT);
}

/**************************  GET_PANEL_EVENT()  ******************************/

get_panel_event()
{
 return Panel_event;
}

/**************************  SET_PANEL_EVENT()  ******************************/

set_panel_event(id)
int id;
{
 Panel_event=id;
}

/***********************  PANEL_BACKGROUND_PROC()  ***************************/

void
panel_background_proc(panel, event)
Panel panel;
Event *event;
{
void (*Event_proc)();

 if (panel_event_debug )
  if (event_id(event)!=LOC_MOVE && event_id(event)!=32525)
   printf("panel bkgnd event: %d in panel %d\n",event_id(event),panel);
 Event_proc=(void (*)())xv_get(panel,WIN_EVENT_PROC);
 set_panel_event(event_id(event));
 switch (event_id(event)) {
  case ACTION_HELP:
  case EVENT_HELP_KEY:
  case LOC_WINENTER:
  case LOC_WINEXIT:
   (void)Event_proc(panel,event);
   break;
  case MS_RIGHT:
   event->action=ACTION_SELECT;
   event->ie_code=MS_LEFT;
  default:
   panel_default_handle_event(panel,event);
   break;
 }
}

/************************  EVENT_IN_CTL_PANEL()  ******************************/

event_in_ctl_panel()
{
 if (get_current_panel()==WTYPE_CONTROLPANEL)  
  return TRUE; 
 return FALSE;
}

/************************  EVENT_IN_OP_PANEL()  ******************************/

event_in_op_panel()
{
 if (get_current_panel()==HELPIMAGE_OP_PANEL_AREA)  
  return TRUE; 
 return FALSE;
}

/************************  EVENT_IN_SETUP_PANEL()  **************************/

event_in_setup_panel()
{
 if (get_current_panel()==HELPIMAGE_SETUP_AREA)  
  return TRUE; 
 return FALSE;
}

/************************  EVENT_IN_PANEL_MENU()  **************************/

event_in_panel_menu()
{
 if (/***get_current_panel()==HELPIMAGE_STATS_MENU_AREA ||
     get_current_panel()==HELPIMAGE_ATTRS_MENU_AREA || ***/
     get_current_panel()==HELPIMAGE_PRINT_MENU_AREA ||
     get_current_panel()==HELPIMAGE_PRINTCONFIG_MENU_AREA ||
     get_current_panel()==HELPIMAGE_PLOTSPEX_MENU_AREA ||
     get_current_panel()==HELPIMAGE_FOLDERS_MENU_AREA 
    )  
  return TRUE; 
 return FALSE;
}

/************************  EVENT_IN_PANEL_MENU()  **************************/

current_panel_menu_area()

/* following must match notion in panel_menu_area[], panel_menus.c 
*/ 
{
 switch (current_panel) {
/***
  case HELPIMAGE_STATS_MENU_AREA:
   return STATS_MENU_AREA;
  case HELPIMAGE_ATTRS_MENU_AREA:
   return ATTRS_MENU_AREA;
***/
  case HELPIMAGE_PRINT_MENU_AREA:
   return PRINTERS_MENU_AREA;
  case HELPIMAGE_PRINTCONFIG_MENU_AREA:
   return PRINTCONFIG_MENU_AREA;
  case HELPIMAGE_PLOTSPEX_MENU_AREA:
   return PLOTSPEX_MENU_AREA;
  case HELPIMAGE_FOLDERS_MENU_AREA:
   return FOLDERS_MENU_AREA;
 }
 return 0;
}

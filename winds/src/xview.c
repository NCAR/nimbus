/*
xview.c -- implementation for various X-window objects as per XView
*/
#include <stdio.h>
#include <xview/xview.h>
#include <xview/panel.h>
#include <xview/cms.h>

#include "proto.h"

#define MaxSubFrames 0
static Frame XviewBaseFrame=0;		/* base frame */
static Xv_Window XviewSubFrame[MaxSubFrames]=
{0,0};       /* popup frame */

/***********************************************************************/
 
CreateBaseFrame(char label[])
/*
create base frame with label, at (0,0) with color inheritance enabled
*/
{
 XviewBaseFrame = xv_create((int)NULL, FRAME,
	  FRAME_INHERIT_COLORS,TRUE,
          XV_LABEL, label,
          XV_X, 0, XV_Y, 0,
        NULL);
}

/***********************************************************************/
 
Frame
GetBaseFrame()
/*
return application base frame 
*/
{
 return XviewBaseFrame;
}

/***********************************************************************/
 
Display *
GetBaseFrameDisplay()
/*
return application base frame Display *
*/
{
 return (Display *)xv_get(XviewBaseFrame,XV_DISPLAY);
}

/***********************************************************************/
 
GetBaseFrameDamage()
/*
???
*/
{
 win_get_damage(XviewBaseFrame);
}

/***********************************************************************/
 
CreateSubFrame(int which, char label[])
/*
create which'th application sub frame with label as specified
*/
{
 XviewSubFrame[which] = xv_create (XviewBaseFrame, FRAME,
    FRAME_NO_CONFIRM, TRUE,
    FRAME_SHOW_LABEL, TRUE,
    FRAME_LABEL, label,
    NULL);
}

/***********************************************************************/
 
Frame
GetSubFrame(int which)
/*
return which'th application sub frame 
*/
{
 return XviewSubFrame[which];
}

/***********************************************************************/
 
CenterWindow(int centered, int border)
/*
center centered within border
*/
{ 
 xv_set (centered,
  XV_X, ((int)xv_get (border, XV_WIDTH) -
  (int)xv_get (centered, XV_WIDTH)) / 2,
  XV_Y, ((int)xv_get (border, XV_HEIGHT) -
  (int)xv_get (centered, XV_HEIGHT)) / 2,
  NULL);
}

/***********************************************************************/
 
Window
GetWindowXID(int window)
/*
return application base frame XID
*/
{
 return xv_get(window,XV_XID);
}

/***********************************************************************/
 
SetWindowX(int window,int xposn)
/*
set x-position of window
*/
{
 xv_set(window,XV_X,xposn,NULL);
}

/***********************************************************************/
 
SetWindowY(int window,int yposn)
/*
set y-position of window
*/
{
 xv_set(window,XV_Y,yposn,NULL);
}

/***********************************************************************/
 
GetWindowX(int window)
/*
return x-position of window
*/
{
 return xv_get(window,XV_X);
}

/***********************************************************************/
 
GetWindowY(int window)
/*
return y-position of window
*/
{
 return xv_get(window,XV_Y);
}

/***********************************************************************/
 
SetWindowHeight(int window, int height)
/*
set height of window
*/
{
 xv_set(window,XV_HEIGHT,height,NULL);
}

/***********************************************************************/
 
SetWindowWidth(int window, int width)
/*
set width of window
*/
{
 xv_set(window,XV_WIDTH,width,NULL);
}

/***********************************************************************/
 
GetWindowHeight(int window)
/*
return height of window
*/
{
 return xv_get(window,XV_HEIGHT);
}

/***********************************************************************/
 
GetWindowWidth(int window)
/*
return width of window
*/
{
 return xv_get(window,XV_WIDTH);
}

/***********************************************************************/
 
SizeWindowWidthToFitContents(int window)
/*
fit window around its contents in the width dimension
*/
{
 window_fit_width (window);
}

/***********************************************************************/
 
SizeWindowHeightToFitContents(int window)
/*
fit window around its contents in the height dimension
*/
{
 window_fit_height (window);
}

/***********************************************************************/
 
SizeWindowToFitContents(int window)
/*
fit window around its contents in both width and height dimensions
*/
{
 window_fit (window);
}

/***********************************************************************/
 
SetWindowVisible(int window, int show)
/*
show or hide window as per value of show
*/
{
 xv_set(window,XV_SHOW,show,NULL);
}

/***********************************************************************/
 
GetMousePosnY(int window)
/*
return y-position of cursor relative to window
*/
{
 return ((Rect *)xv_get(window,WIN_MOUSE_XY))->r_top;
}

/***********************************************************************/
 
GetMousePosnX(int window)
/*
return x-position of cursor relative to window
*/
{
 return ((Rect *)xv_get(window,WIN_MOUSE_XY))->r_left;
}

/***********************************************************************/
 
SetMousePosn(int window, int x, int y)
/*
set x and y-position of cursor relative to window
*/
{
 xv_set(window,WIN_MOUSE_XY,x,y,NULL);
}

/***********************************************************************/
 
SetWait3Func(Notify_value (*func)(), int pid)
/*
register func as the procedure to be called on change of run status for 
process with pid
*/
{
 notify_set_wait3_func(XviewBaseFrame,func,pid);
}

/***********************************************************************/

SetKeyboardFocus(int window)
{
 win_set_kbd_focus(window, xv_get(window, XV_XID));
}

/***********************************************************************/

#include <stdarg.h>

XSetAttr(int object, ...)
{
 va_list pvar;
 int attr,attrnum,CycleAttr,value,value2;
 int LastArgValid = TRUE;
 int IntCharCycle=FALSE,IntCycle = FALSE;
 int retval=0;
 int skip;
 char *string=(char *)NULL;
 Xv_font xvfont=(Xv_font)NULL;

 va_start(pvar, object);
 while (LastArgValid ) {
  skip=FALSE;
  if (!IntCycle && !IntCharCycle) {
   attr = va_arg(pvar, int);
  } else {
   attr=CycleAttr;
  }
  if (attr==(int)PANEL_CHOICE ||
   attr==(int) PANEL_TEXT ||
   attr==(int) PANEL_MESSAGE ||
   attr==(int) PANEL_BUTTON ||
/*** see compiler's pre-processed result: PANEL_CHOICE takes care of this one
   attr==(int) PANEL_TOGGLE ||
***/
   attr==(int) PANEL_LIST ||
   attr==(int) PANEL_SLIDER ||
   attr==(int) PANEL_NUMERIC_TEXT ||
   attr==(int) CMS ||
   attr==(int) MENU ||
   attr==(int) SERVER_IMAGE ||
   attr==(int) FRAME ||
   attr==(int) CANVAS ||
   attr==(int) PANEL
  ) {
   object=retval=XCreateObject(object,attr);
  } else {
   switch (attr) {
    case 0:
     LastArgValid=FALSE;
     va_end(pvar);
     return retval;
/*
set current object directly with this attribute, no further arguments needed
*/
    case WIN_SET_FOCUS:
     xv_set(object,attr,NULL);
     continue;
/*
*/
    case PANEL_LIST_DELETE_ROWS:
    case PANEL_LIST_SELECT:
     attrnum = va_arg(pvar, int);
     value = va_arg(pvar, int);
     xv_set(object,attr,attrnum,value,NULL);
     continue;
    case PANEL_LIST_STRING:
    case PANEL_CHOICE_STRING:
     attrnum = va_arg(pvar, int);
     string = va_arg(pvar, char *);
     xv_set(object,attr,attrnum,string,NULL);
     continue;
    case PANEL_CHOICE_STRINGS:
     attr = PANEL_CHOICE_STRING;
     if (!IntCharCycle) {
      IntCharCycle=TRUE;
      attrnum=0;
     }
     CycleAttr=PANEL_CHOICE_STRINGS;
     break;
    case XV_KEY_DATA:
     attr = va_arg(pvar, int);
     switch (attr) {
      case XV_HELP:
       string = va_arg(pvar, char *);
       xv_set(object,XV_HELP_DATA,string,NULL);
       skip=TRUE;
       break;
      default:
       (void)fprintf(stderr,"WARNING: unselected attr under XV_KEY_DATA\n");
       break;
     }
     break;
    case PANEL_LIST_STRINGS:
     attr = PANEL_LIST_STRING;
     if (!IntCharCycle) {
      IntCharCycle=TRUE;
      attrnum=0;
     }
     CycleAttr=PANEL_LIST_STRINGS;
     break;
    case WIN_CONSUME_EVENTS:
     attr = WIN_CONSUME_EVENT;
     IntCycle=TRUE;
     CycleAttr=WIN_CONSUME_EVENTS;
     break;
    case WIN_IGNORE_EVENTS:
     attr = WIN_IGNORE_EVENT;
     IntCycle=TRUE;
     CycleAttr=WIN_IGNORE_EVENTS;
     break;
/*
mouse x-y position: 2 int arguments to follow
*/
    case WIN_MOUSE_XY:
     value = va_arg(pvar, int);
     value2 = va_arg(pvar, int);
     xv_set(object,attr,value,value2,NULL);
     continue;
/*
set font
*/
    case XV_FONT:
     string=(char *)NULL;
     xvfont = va_arg(pvar, Xv_font);
     break;
/*
attribute is followed by one string argument
*/
    case PANEL_LIST_TITLE:
    case MENU_STRING:
    case XV_LABEL:
    case PANEL_LABEL_STRING:
     string = va_arg(pvar, char*);
     break;
/*
if attribute is not explicitly listed in this switch, treat it like it is
followed by one integer argument
*/
    default:
     string=(char *)NULL;
     value = va_arg(pvar, int);
     break;
   }
   if (!LastArgValid)
    break;
   if (!skip) {
    if (IntCycle) {
     value = va_arg(pvar, int);
     if (value)
      xv_set(object,attr,value,NULL);
     else
      IntCycle=FALSE;
    } else if (IntCharCycle) {
     string = va_arg(pvar, char *);
     if (string)
      xv_set(object,attr,attrnum++,string,NULL);
     else
      IntCharCycle=FALSE;
    } else {
     if (string) {
      xv_set(object,attr,string,NULL);
      string=(char *)NULL;
     } else if (xvfont) {
      xv_set(object,attr,xvfont,NULL);
      xvfont=(Xv_font)NULL;
     } else {
      xv_set(object,attr,value,NULL);
     }
    }
   }
  }
 }
 va_end(pvar);
 return retval;
}

/***********************************************************************/

XGetAttr(int object, ...)
{
 va_list pvar;
 int attr,attr2,value;

 va_start(pvar, object);
 attr = va_arg(pvar, int);
 switch (attr) {
  case PANEL_LIST_SELECTED:
  case PANEL_LIST_NEXT_SELECTED:
  case CANVAS_NTH_PAINT_WINDOW:
  case PANEL_CHOICE_STRING:
  case MENU_NTH_ITEM:
  case CMS_X_COLORS:
   attr2 = va_arg(pvar, int);
   value = xv_get(object,attr,attr2);
   break;
  default:
   value = xv_get(object,attr);
   break;
 }
 va_end(pvar);
 return value;
}

/***********************************************************************/

XCreateObject(int parent, void *objtype) 
{
 return xv_create(parent,objtype,NULL);
}


/* fixes.c -- handle fixed format window

   SCCS ID: 
@(#)fixes.c	1.1 11/14/90

*/
#include <xview/xview.h>
#include <stdio.h>
#include <string.h>

/****************************  WINDS INCLUDES  ******************************/

/*
layout
*/
#define FIXF_LABEL_GAP		2
#define MAX_FIXEES              20    /* max. # fixees, fixed_format window */
#define FIXES_LEFT_MARGIN  	40    /*left margin # pixels for 1st column */
#define FIXES_COLUMN_GAP   	24    /* pixels extra between columns  */
#define NUM_FIXED_ROWS 		10
#define FIXED_PRECISION          9
/*
FONTID: font ID
*/
#define FONTID(q) (GetFontID(GetFixedDisplayFont(q)))
/*
FONTWIDTH: width in pixels of text rectangle
*/
#define FONTWIDTH(q) (GetFontWidth(GetFixedDisplayFont(q)))
/*
FONTHEIGHT: height in pixels of text rectangle, inclusive of ascent and descent
*/
#define FONTHEIGHT(q) (GetFontHeight(GetFixedDisplayFont(q)))
/*
FONTDESCENT: height in pixels of descending portion of text rectangle
*/
#define FONTDESCENT(q) (GetFontDescent(GetFixedDisplayFont(q)))
/*
FONTASCENT: height in pixels of ascending portion of text rectangle
*/
#define FONTASCENT(q) (GetFontAscent(GetFixedDisplayFont(q)))
/*
number of font columns
*/
#define NUMFIXEDFONTCOLS	51
/*
number of font rows
*/
#define NUMFIXEDFONTROWS	18

#include <display.h>
#include <lengths.h>
#include <colors.h>
#include <constants.h>
#include <errors.h>
#include <help.h>
#include <ops.h>
#include <macros.h>
#include <proto.h>


/****************************  EXPORTABLE  ********************************/

/* number of fixees in plot window, their labels, and number of current fixee  
*/
int num_of_fixees[NUM_QUADRANTS];             
/* allow add'l for "Open slot" string */
char fixee_label[NUM_QUADRANTS][MAX_FIXEES][NAMLEN+3]; 
int fixee_number[NUM_QUADRANTS];              

int data_label_x[MAX_FIXEES][NUM_QUADRANTS];  /* x positions for data stamps */
int label_y[NUM_QUADRANTS][MAX_DISPLAY_VARS]; /* y positions of range_label's */
int add_fixee;             /* TRUE when event over open slot */
static char fixee_name[NUM_QUADRANTS][MAX_FIXEES][NAMLEN];  

/****************************************************************************/
 
GetFixedFontWidth(quadrant)
int quadrant;
{
 return (GetWINDSWindowWidth(quadrant)/NUMFIXEDFONTCOLS);
}
 
/****************************************************************************/
 
GetFixedFontHeight(quadrant)
int quadrant;
{
 return (GetWINDSWindowHeight(quadrant)/NUMFIXEDFONTROWS);
}

/*****************************************************************************/
 
GetFixedAreaTooSmall(quadrant)
int quadrant;
{
 return FALSE;
}

/******************************************************************/

GetMaxFixees()
{
 return MAX_FIXEES;
}

/******************************************************************/

char *
GetFixeeName(quadrant,which)
int quadrant,which;
{
 return fixee_name[quadrant][which]; 
}

/**************** SETUP_FIX()  ************************************/

setup_fix(quadrant) 
int quadrant;

/* set up a fix format column for each preselected variable  */

{ 
int choice;  /* ordinal number of current fixee being set up */ 
int total;


 for (choice=0; choice < MAX_FIXEES; choice++) {
  dep_ptr[quadrant][choice]=dep_attr_num[quadrant][choice]=ERROR;
 }

 update_mode[quadrant]=INITIAL;

 for(fixee_number[quadrant]=-1,choice=0,total=num_of_fixees[quadrant];
  choice < total;
  choice++) {

  fixee_number[quadrant]++;

 /* convert user choices into fix format columns  */

  if ( (indices(quadrant,usrvar[quadrant][choice])) == ERROR) { 

/* error condition  */
    set_notice_needed(VARIABLE_NOT_AVAILABLE);
    report(quadrant,"variable not available");
   --num_of_fixees[quadrant];
   --fixee_number[quadrant];
  } 
 } 

 if (!(num_of_fixees[quadrant])) {
/* simply return ERROR cond'n, caller should restore previous setup */
   report(quadrant,"bogus request; NO FIXEES !!");
   set_notice_needed(NO_LISTEES);
   return(ERROR);
 } 
 
 return(TRUE);
}


/*****************************  LIST_FIXED_LABELS()  *************************/


list_fixed_labels(quadrant)
int quadrant;

/* label columns with variable name and units

   on entry: dep_attr_num, window_height and window_width must be defined; 
*/

{
int k;


 label_y[quadrant][0]=(int) ((float)window_height[quadrant] / 
                      (float)NUM_FIXED_ROWS) - FONTHEIGHT(quadrant)/2;
 (*FillArea[GetDrawType()])(pw_xid[quadrant],0,0,window_width[quadrant],window_height[quadrant],0);

/* continue with each fixee name */

 (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,FOREGROUND,0,0,
  FONTID(quadrant));
 for (k=0; k<MAX_FIXEES; k++){
  if (k < num_of_fixees[quadrant]) {  /* total # chosen */
   (void)sprintf(unit_label[quadrant][k],"%s",
    &units[dep_attr_num[quadrant][k]*UNITSLEN]);
   (void)sprintf(fixee_label[quadrant][k],odd(k)?"%s":" %s",
    fixee_name[quadrant][k]);
  } else if (k<MAX_FIXEES) {  /* an open column  */
   (void)sprintf(fixee_label[quadrant][k],odd(k)?"Open slot":" Open slot");
  }
  label_x[quadrant][k]=odd(k)?window_width[quadrant]/2:0;
  if (k)
   label_y[quadrant][k]=label_y[quadrant][0]+
    (((int)(k/2)%NUM_FIXED_ROWS)* window_height[quadrant])/NUM_FIXED_ROWS;
  (*DrawText[GetDrawType()])(
   label_x[quadrant][k], 
   label_y[quadrant][k], 
   fixee_label[quadrant][k]);
  data_label_x[k][quadrant]=label_x[quadrant][k]+(NAMLEN+(odd(k)?0:1))*
  FONTWIDTH(quadrant);
 }

 return;
}


/************************  DRAW_HEADER()  ************************************/

draw_fixed_header(quadrant)
int quadrant;
/*  erase its background, draw the header with labels as per names and units 
    in plot_attr file 

    draw_fixed_header is called once on initial setup of window (on detection of
    SIGWINCH), and subsequently whenever user adds/changes/deletes a fixee in 
    the window 
*/

{

 if (!(num_of_fixees[quadrant]))  { /* might happen on bogus alternate request */
  return;
 }

 list_fixed_labels(quadrant);
 need_rehilite[quadrant]=TRUE;

}

/****************  EVENT_IN_CHG_FIXEE_AREA()  *******************************/

event_in_chg_fixee_area(x,y)
int x,y;
/* is the current canvas event in the area with the name labels?
   If so, set fixee_number to the value that points to the selected fixee
   in the fixee_label & label_x arrays, and return TRUE; else, return FALSE
*/
{
int RowGap;

 if (cursor_in_ctl_panel ||
     cursor_in_control_panel || 
     plot_type[current_quadrant]!=FIXED_FORMAT ||
     !label_y[current_quadrant][0])
  return(FALSE);

 add_fixee=FALSE;
 for (fixee_number[current_quadrant]=0; 
      fixee_number[current_quadrant] < MAX_FIXEES; 
      fixee_number[current_quadrant]++) {
/*
determine gap between rows, first time through
*/
   if (!fixee_number[current_quadrant])
    RowGap=label_y[current_quadrant][2]-label_y[current_quadrant][0]
     -FONTHEIGHT(current_quadrant);
/*
if cursor is at or below the y-position of fixee under examination ...
*/ 
   if (y <= label_y[current_quadrant][fixee_number[current_quadrant]]+RowGap/2){
    if ((!(odd(fixee_number[current_quadrant])) && 
     x < label_x[current_quadrant][fixee_number[current_quadrant]+1]-
/*
margin of 3 font characters to left of right columns labels
*/
     3*FONTWIDTH(current_quadrant))
     ||  ( odd(fixee_number[current_quadrant]) && 
      x >= label_x[current_quadrant][fixee_number[current_quadrant]]-
      3*FONTWIDTH(current_quadrant))) {
     if (fixee_number[current_quadrant] >= num_of_fixees[current_quadrant]) {
      add_fixee=TRUE;
      fixee_number[current_quadrant]=num_of_fixees[current_quadrant];
     } 
     return(TRUE);
    }
   } 
 /* 
allow chg_fixee_area to extend below lowest y-coord for labels 
*/
   if ((fixee_number[current_quadrant]==MAX_FIXEES-2 && 
        x<label_x[current_quadrant][MAX_FIXEES-1])
    || (fixee_number[current_quadrant]==MAX_FIXEES-1 && 
        x >= label_x[current_quadrant][MAX_FIXEES-1])) {
    if (fixee_number[current_quadrant] >= num_of_fixees[current_quadrant]) {
     add_fixee=TRUE;
     fixee_number[current_quadrant]=num_of_fixees[current_quadrant];
    }
    return(TRUE);
   }
  }
 return (FALSE);
 }

/********************  HIGHLIGHT_FIXEE_LABEL()  ******************************/

highlight_fixee_label(quadrant)
int quadrant;
/* reverse video the selected fixee_label
*/
{
/* highlight background */

 if (plot_type[quadrant]!=FIXED_FORMAT) 
  return;
 (*FillArea[GetDrawType()])(pw_xid[quadrant],
  label_x[quadrant][fixee_number[quadrant]]-
   (odd(fixee_number[quadrant])?
  FONTWIDTH(quadrant):0),
  label_y[quadrant][fixee_number[quadrant]]-FONTHEIGHT(quadrant),
  (NAMLEN+(fixee_number[quadrant]>=num_of_fixees[quadrant]?3:0)) 
    * 
  FONTWIDTH(quadrant),
  FONTHEIGHT(quadrant)+4*FIXF_LABEL_GAP,BLU);
 (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,REVERSE_VIDEO,0,0,
  FONTID(quadrant));
 (*DrawTtext[GetDrawType()])(
  label_x[quadrant][fixee_number[quadrant]],
  label_y[quadrant][fixee_number[quadrant]],
  fixee_label[quadrant][fixee_number[quadrant]]);
}


/**********************  CLEAR_FIXEE_LABEL()  ******************************/

clear_fixee_label(quadrant,mode,fixee_num)
int quadrant,mode,fixee_num;
/* restore the selected fixeee_label to normal video
*/
{
 if (fixee_num==NONE)
  return;

 if (plot_type[quadrant]!=FIXED_FORMAT) 
  return;
 (*FillArea[GetDrawType()])(pw_xid[quadrant],
           label_x[quadrant][fixee_num]-
           (odd(fixee_num)?FONTWIDTH(quadrant):0),
           label_y[quadrant][fixee_num]-FONTHEIGHT(quadrant),
  (NAMLEN+(fixee_num>=num_of_fixees[quadrant]?3:0)) * 
  FONTWIDTH(quadrant),
  FONTHEIGHT(quadrant)+4*FIXF_LABEL_GAP,0);
 if (mode == REFRESH) { 
  (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,FOREGROUND,0,0,
  FONTID(quadrant));
  (*DrawText[GetDrawType()])(
   label_x[quadrant][fixee_num],
   label_y[quadrant][fixee_num],
   fixee_label[quadrant][fixee_num]);
 }
}

/************************  CHG_FIXEE()  **********************************/

chg_fixee(event)
Event *event;
{
 ShowVarMenu(current_quadrant,event);
}
/****************************  FIXES()  *************************************/

fixes(quadrant) 
int quadrant;
{
extern int time_gone_by[]; 

 SetPointsDrawMode(quadrant,FALSE);
 switch (update_mode[quadrant]) {
  case NORMAL_UPDATE:
   break;
  case START_NOW:
  case INITIAL:
   SetNextDrawStart(quadrant,(time_gone_by[quadrant] % get_datablock_size()));
   (*FillArea[GetDrawType()])(pw_xid[quadrant],
     0,0,window_width[quadrant],window_height[quadrant],0);
/*** comment out 9/8/91 so that in analysis mode, frame jumps result in display
     of current second instead of blank data columns
   SetLastDrawEnd(quadrant,time_gone_by[quadrant]);
***/
   draw_fixed_header(quadrant);
   need_rehilite[quadrant]=TRUE;
   break;
  case RECONFIG:
  default:

/* when fixees are added or changed in analysis mode (or at any time when
   clock is not changing), need to trick more_points() into thinking there
   is a need to update the display
*/
   need_rehilite[quadrant]=TRUE;
   SetLastDrawEnd(quadrant,time_gone_by[quadrant]-1);
   draw_fixed_header(quadrant);
   break;
 }
 
 if (more_points(quadrant)) {/* new points since last update*/
  SetNextDrawStart(quadrant,
   ((time_gone_by[quadrant]-1) % get_datablock_size()));
  list_fixees(quadrant,GetNextDrawStart(quadrant));
 } 
 
 if (need_rehilite[quadrant]) {
  rehilite(quadrant);
 }
 update_mode[quadrant]=NORMAL_UPDATE;
}

/*****************************************************************************/

GetNumFixees(quadrant)
int quadrant;
{
 return num_of_fixees[quadrant];
}

/*****************************************************************************/

GetFixedDataLen()
{
 return FIXED_PRECISION;
}

/**********************  LIST_FIXEES()  **************************************/

list_fixees(quadrant,start)
int quadrant,start;
/* list values for start for each of num_fixees variables 
*/
{
int fixee;
char value[VALUELEN];
int RawStartBlock,DeriveStartBlock;
static int FormatSet=FALSE;
static char FixedFloatFormat[8],FixedOctalFormat[8],FixedHexFormat[8];

 if (!FormatSet) {
  (void)sprintf(FixedFloatFormat,"%%%df",GetFixedDataLen());
  (void)sprintf(FixedOctalFormat,"%%0%do",GetFixedDataLen());
  (void)sprintf(FixedHexFormat,"x%%0%dx",GetFixedDataLen()-1);
  FormatSet=TRUE;
 }
/*  
mod the point index to facilitate wraparound from the loop incrementing
*/
   DeriveStartBlock=(start%get_datablock_size())*GetNumDerivedNames();
   RawStartBlock=(start%get_datablock_size())*GetNumRawNames();
/* 
for each fixee ... 
*/
   for (fixee=0; fixee<num_of_fixees[quadrant]; fixee++) {
/* 
compute y-coordinates of next endpoint 
*/
    switch (GetDepVarMode(quadrant,fixee)) {
     case BITS:
      (void)sprintf(value,BaseIsHex()?FixedHexFormat:FixedOctalFormat,
       (GetBitDataValue(RawStartBlock+dep_ptr[quadrant][fixee])));
      break;
     case VOLTAGE:
      (void)sprintf(&value[0],FixedFloatFormat,
       ((float)GetVoltsDataValue(RawStartBlock+dep_ptr[quadrant][fixee])));
      break;
     case RAW:
      (void)sprintf(&value[0],FixedFloatFormat,
       ((float)GetRawDataValue(RawStartBlock+dep_ptr[quadrant][fixee])));
      break;
     case DERIVED:
      (void)sprintf(&value[0],FixedFloatFormat,
       ((float)GetDeriveDataValue(DeriveStartBlock+dep_ptr[quadrant][fixee])));
      break;
    }
    value[GetFixedDataLen()] = '\0';
/*
draw out the value 
*/
    (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,FOREGROUND,0,0,
     FONTID(quadrant)); 
    (*DrawText[GetDrawType()])(
     data_label_x[fixee][quadrant],
     label_y[quadrant][fixee],
     value);
    (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,FOREGROUND,0,0,
     FONTID(quadrant)); 
    (*DrawText[GetDrawType()])(data_label_x[fixee][quadrant]
     +(GetFixedDataLen()+1)*FONTWIDTH(quadrant),
     label_y[quadrant][fixee],
     unit_label[quadrant][fixee]);
   }  
}


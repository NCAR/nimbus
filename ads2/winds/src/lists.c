
/* lists.c -- handle lists to screen and printer

   SCCS ID: 
@(#)lists.c	4.2 10/27/89

*/
#include <xview/xview.h>
#include <stdio.h>
#include <string.h>

/*
layout
*/
#define DEFAULT_LIST_RATE 	1  /* default seconds between list updates */
#define LIST_HEADER_ROWS        3  /* listee labels, units, spacing */
#define MAX_LISTEES             24 /* max # of total listees */
#define DEFAULT_LISTEES    	6  /* # of listees per non-wide window  */
#define WIDE_LISTEES    	12 /* # of listees per wide window  */
#define TALL_LISTROWS        	30 /* number of rows in stack or full geom */
#define DEFAULT_LISTROWS        15 /* number of rows in quad or strip geom */
#define DEFAULT_LISTCOLS	51
#define WIDE_LISTCOLS		(DEFAULT_LISTCOLS*2)

#define LISTFONTID(q)		(GetFontID(GetListFont(q,TRUE)))
#define LISTFONTWIDTH(q) 	(GetFontWidth(GetListFont(q,TRUE)))
#define LISTFONTHEIGHT(q) 	(GetFontHeight(GetListFont(q,TRUE)))
#define LISTFONTDESCENT(q)	(GetFontDescent(GetListFont(q,TRUE)))

#define LISTROW_GAP		2	/* pixel gap between rows */
/*
height in pixels of each row, including gap, in window q
*/
#define ROW_HEIGHT(q)		(LISTFONTHEIGHT(q)+LISTROW_GAP)
/*
how many rows total, function of font height 
*/
#define LISTROWS(q)	((rheight[quadrant]-LISTFONTDESCENT(q))/ROW_HEIGHT(q))
/*
pixels as margin to left of time stamp column 
*/
#define LIST_LEFT_MARGIN 	10   
/*
# of digits to display for list values
*/
#define LIST_PRECISION 		6      
/*
width in pixels consumed by list values
*/
#define WIDTH_VALUE_CHARS(q) (listees_per_page[q]*LIST_PRECISION*LISTFONTWIDTH(q))
/*
left edge x-coord for start of list values area, just to right of time stamp
*/
#define XPOSN_VALUES_AREA(q) (LIST_LEFT_MARGIN+TIME_STAMP_LEN*LISTFONTWIDTH(q))
/*
width in pixels remaining to right of time stamp for list values
*/
#define VALUES_AREA_WIDTH(q) (GetWINDSWindowWidth(q)-XPOSN_VALUES_AREA(q))
/*
how much empty space (in characters, as per font width) is left after list 
values consume their area; divide this up as spacing between list value columns
*/
#define LISTCOL_GAP(q) ((VALUES_AREA_WIDTH(q)-WIDTH_VALUE_CHARS(q))/ \
			listees_per_page[q]/LISTFONTWIDTH(q))
/*
pixel width of each list value column, including gap between each one
*/
#define COLUMN_WIDTH(q)		((LIST_PRECISION+LISTCOL_GAP(q))*LISTFONTWIDTH(q))
/*
leftmost edge of 1st listee label hilite area
*/
#define FIRST_HILITE_X(q)    (XPOSN_VALUES_AREA(q)+(LISTCOL_GAP(q)*LISTFONTWIDTH(q))/2)
/* 
y-coord for list header 
*/
#define LISTROW(q,n) 		((n)*ROW_HEIGHT(q)) 
/*
find x-coord to center a string as function of x start point, string length, 
and whether font is the standard width or not
*/
#define STRWIDTH(q,len,std)	((len)*GetFontWidth(GetListFont(q,std)))
#define CENTERED_X(q,x0,len,std) ((x0)+(COLUMN_WIDTH(q)-STRWIDTH(q,len,std))/2)

/****************************  WINDS INCLUDES  ******************************/

#include <display.h>
#include <lengths.h>
#include <ops.h>
#include <constants.h>
#include <errors.h>
#include <colors.h>
#include <help.h>
#include <geom.h>
#include <macros.h>
#include <proto.h>


/***************************  GLOBAL WITHIN FILE  ****************************/

/* number of listees in plot window, their labels, and number of current 
  listee.  
*/
static char listee_name[NUM_QUADRANTS][MAX_LISTEES][NAMLEN];  
static int last_screen_row[NUM_QUADRANTS]=
{0,0,0,0,0};				/* which row of lists drawn last */
int num_of_listees[NUM_QUADRANTS];             
char unit_label[NUM_QUADRANTS][MAX_DISPLAY_VARS][UNITSLEN];
char listee_label[NUM_QUADRANTS][MAX_LISTEES][NAMLEN];   
int listee_number[NUM_QUADRANTS];              
int listees_per_page[NUM_QUADRANTS];              

int label_x[NUM_QUADRANTS][MAX_DISPLAY_VARS];/* x positions of range_label's  */
int add_listee;             /* TRUE when event over open slot */
int start_listee[NUM_DISPLAY_QUADRANTS]=  /* which list column to start with */
{0,0,0,0};
static int ListRate[NUM_DISPLAY_QUADRANTS]=
{DEFAULT_LIST_RATE,DEFAULT_LIST_RATE,DEFAULT_LIST_RATE,DEFAULT_LIST_RATE};

/********************************************************************/

GetDefaultListRate()
{
 return DEFAULT_LIST_RATE;
}

/********************************************************************/

GetListFontWidth(varnamefont,quadrant)
int varnamefont,quadrant;
{
int width,numcols;

 numcols=wide_geom(quadrant)?WIDE_LISTCOLS:DEFAULT_LISTCOLS;
 width=GetWINDSWindowWidth(quadrant)/(varnamefont?numcols:numcols*1.3);
 return width;
}

/********************************************************************/

GetListFontHeight(varnamefont,quadrant)
int varnamefont,quadrant;
/*
height is function of number of rows, which is function of whether window
is tall or not
*/
{
int height,numrows;

 numrows=(tall_geom(quadrant)?TALL_LISTROWS:DEFAULT_LISTROWS)+LIST_HEADER_ROWS;
 height= (GetWINDSWindowHeight(quadrant)
/*
deduct the space consumed by gaps between rows
*/
  -(numrows-1)*LISTROW_GAP)
/*
and determine font height by dividing remaining space by number of rows of
fonts
*/
  /(varnamefont?numrows:
/* 
kluge: add just a little to get smaller fonts for units labels 
*/
  numrows+3);
 return height;
}

/********************************************************************/

GetListAreaTooSmall(q)
int q;
/*
if the width needed to list all values is larger (or exactly the same...)
as the area available, return TRUE; else, return FALSE.
*/
{
 if (WIDTH_VALUE_CHARS(q) >= VALUES_AREA_WIDTH(q))
  return TRUE;
 else
  return FALSE;
}

/********************************************************************/

GetNumListees(quadrant)
int quadrant;
{
 return num_of_listees[quadrant];
}

/********************************************************************/

GetNumListRows(quadrant)
int quadrant;
{
 return LISTROWS(quadrant);
}

/********************************************************************/

GetListDataLen()
{
 return LIST_PRECISION;
}

/********************************************************************/

GetListLeftMargin()
{
 return LIST_LEFT_MARGIN;
}

/********************************************************************/

GetMaxListees()
{
 return MAX_LISTEES;
}

/********************************************************************/

GetMinListeesPerPage()
{
 return DEFAULT_LISTEES;
}

/********************************************************************/

char *
GetListeeName(quadrant,which)
int quadrant,which;
{
 return listee_name[quadrant][which];
}

/*****************************  GetListRate()  ******************************/

GetListRate(quadrant)
int quadrant;
{
 return ListRate[quadrant];
}

/*****************************  SetListRate()  ******************************/

SetListRate(quadrant,rate)
int quadrant,rate;
{
 ListRate[quadrant]=rate;
}

/*****************************  LISTS()  ***********************************/

lists(quadrant)
int quadrant;
{
extern int time_gone_by[]; 
int numpts;

 SetPointsDrawMode(quadrant,FALSE);
 switch (update_mode[quadrant]) { 
  case NORMAL_UPDATE:
   break; 
  case START_NOW:
  case INITIAL:
   SetNextDrawStart(quadrant,(time_gone_by[quadrant] % get_datablock_size()));
   SetLastDrawEnd(quadrant,time_gone_by[quadrant]);

/* reset next screen row to top
*/
   last_screen_row[quadrant]=0;
   need_rehilite[quadrant]=TRUE;

   (*FillArea[GetDrawType()])(pw_xid[quadrant],
    0,0,window_width[quadrant],window_height[quadrant],0);
   draw_list_header(quadrant);
   break;
  case RECONFIG:
  default: 

/* set controls to re-highlight object underneath cursor
*/
   need_rehilite[quadrant]=TRUE;

/* Clear window to prepare for new display here unless it's a
   simple change of listee column, in which case preserve other columns.
   Assumed new column already cleared previously.  This should clear the
   window and start next list row at top for change of display, change of
   geometry, and change of page, but NOT for change of listee column.
*/
   if (update_mode[quadrant]!=CHG_LISTEE && update_mode[quadrant]!=DESCRIPTION){
    (*FillArea[GetDrawType()])(pw_xid[quadrant],
     0,0,window_width[quadrant],window_height[quadrant],0);
    last_screen_row[quadrant]=0;
   }

/* redraw new list header
*/
   draw_list_header(quadrant);
   break; 
 } 

/* in case of geometry changes, update_mode is set to RECONFIG but then the
   REPAINT signal resets it to INITIAL.  Instead of mucking with the code
   that processes general REPAINT signals (refresh_window()), simply cover
   all cases here to redraw maximum history.
*/
 if ((update_mode[quadrant]==INITIAL 
  || update_mode[quadrant]==RECONFIG
  || update_mode[quadrant]==SET_LIST_RATE
  || update_mode[quadrant]==CHG_LISTEE)
  )
  SetPointsDrawMode(quadrant,TRUE);

 if ((numpts=more_points(quadrant))) {  

/* new points since last update: list them and adjust controls  
*/
  list_values(quadrant,GetNextDrawStart(quadrant),numpts);
  SetNextDrawStart(quadrant,(time_gone_by[quadrant] % get_datablock_size()));
 } 

 if (need_rehilite[quadrant]) {
  rehilite(quadrant);
 }
 update_mode[quadrant]=NORMAL_UPDATE;
}

/**********************  LIST_VALUES()  **************************************/

list_values(quadrant,start,numpts)
int quadrant,start,numpts;
/* list values from start for numpts points, for each of num_listees 
   variables 
*/
{
int end_pt,point,listee,screen_start;
int row, list_row;
char value[VALUELEN];
int hr, minut, sec;
int numpts_listed;
static char print_row[NUM_DISPLAY_QUADRANTS]
/*
following sizing for print_row should be determined at run-time, as a function
of LISTCOL_GAP(q); but take easy way out here and size it larger than it will
ever need to be, i.e. until value of MAX_LISTEES and WIDE_LISTEES begin to
approach each other...(since WIDE_LISTEES is max ever expected to be seen
on a page at any one time...). 
*/
      [(TIME_CHARS+1)+(LIST_PRECISION+1)*MAX_LISTEES];
int RawStartBlock,DeriveStartBlock;
static int FormatSet=FALSE;
static char ListFloatFormat[8],ListOctalFormat[8],ListHexFormat[8];

 if (!FormatSet) {
  (void)sprintf(ListFloatFormat,"%%%df",GetListDataLen());
  (void)sprintf(ListOctalFormat,"%%0%do",GetListDataLen());
  (void)sprintf(ListHexFormat,"x%%0%dx",GetListDataLen()-1);
  FormatSet=TRUE;
 }

/* for each new data point...  */

  numpts_listed=0;
  end_pt=start + numpts - 1;  /* here's the last point to be listed */

/* there's no need to list more than a full screen's worth of points;
   therefore, if there are more points than that (i.e., "start" is less than
   the first relevant start point), modify the start point to that point which 
   is relevant, i.e. one screen's worth of rows back
 */

  screen_start=MAX(start, end_pt-(LISTROWS(quadrant)*GetListRate(quadrant))+1);
  if (screen_start<start) {

/* sometimes -- DON'T ASK ME WHY -- the MAX() macro returns -12 as larger than
0; this occurs when start=0 and end_pt=1.  Could not duplicate in test code.
Strange. (7/25/90) 
*/
   (void)sprintf(errmsg,"WARNING: macro MAX() failed in list_values(); start=%d,screen_start=%d",start,screen_start);
   report(quadrant,errmsg);
   screen_start=start;
  }

  (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,FOREGROUND,BKGND,0,
     LISTFONTID(quadrant)); 
  for (list_row=last_screen_row[quadrant],point=screen_start;point<=end_pt;
       point++,list_row++) {

/*  mod the point index to facilitate wraparound from the loop incrementing
*/
   hr=GetHMS((point%get_datablock_size())*3);
   minut=GetHMS((point%get_datablock_size())*3 + 1);
   sec=GetHMS((point%get_datablock_size())*3 + 2);

   if (point%GetListRate(quadrant)) {

/* this point is NOT to be listed; decrement list row counter accordingly
*/
    list_row--;
   } else {
    numpts_listed++;
    row=list_row%LISTROWS(quadrant) + 1;
    if (inversed_row[quadrant] && 
        strlen(print_row[quadrant])){
/* 
re-normalize the previously inverse video'd data point
*/
     (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,FOREGROUND,BKGND,0,
      LISTFONTID(quadrant)); 
     (*DrawText[GetDrawType()])(
      GetListLeftMargin(),
      LISTROW(quadrant,LIST_HEADER_ROWS+((row-1)?(row-1)%LISTROWS(quadrant):LISTROWS(quadrant))),
      print_row[quadrant]);
     inversed_row[quadrant]=FALSE;
    }
    (void)sprintf(&print_row[quadrant][0],"\0");
    DeriveStartBlock=(point%get_datablock_size())*GetNumDerivedNames();
    RawStartBlock=(point%get_datablock_size())*GetNumRawNames();
    (void)sprintf(print_row[quadrant],"%02d:%02d:%02d",hr,minut,sec);

/* for each listee ... */

    for (listee=start_listee[quadrant]; 
         listee<MIN(num_of_listees[quadrant],
         start_listee[quadrant]+listees_per_page[quadrant]); 
         listee++) {
     int space;

/* compute y-coordinates of next endpoint */

     switch (GetDepVarMode(quadrant,listee)) {
      case BITS:
       (void)sprintf(value,BaseIsHex()?ListHexFormat:ListOctalFormat,
        GetBitDataValue(RawStartBlock+dep_ptr[quadrant][listee]));
       break;
      case VOLTAGE:
       (void)sprintf(value,ListFloatFormat,
        (float)GetVoltsDataValue(RawStartBlock+dep_ptr[quadrant][listee]));
       break;
      case RAW:
       (void)sprintf(value,ListFloatFormat,
        (float)GetRawDataValue(RawStartBlock+dep_ptr[quadrant][listee]));
       break;
      case DERIVED:
       (void)sprintf(value,ListFloatFormat,
        (float)GetDeriveDataValue(DeriveStartBlock+dep_ptr[quadrant][listee]));
       break;
     }
     value[GetListDataLen()] = '\0';
     for (space=0; space<LISTCOL_GAP(quadrant); space++)
      (void)strcat(print_row[quadrant]," ");
     (void)strcat(print_row[quadrant],value);
    }  /* end of loop on number of listee  */
/* 
inverse video current data point
*/
    (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,BKGND,FOREGROUND,0,
     LISTFONTID(quadrant));
    inversed_row[quadrant]=TRUE;
    (*DrawText[GetDrawType()])(
     GetListLeftMargin(),
     LISTROW(quadrant,LIST_HEADER_ROWS+row),
     print_row[quadrant]);
   } /* end of test on whether point should be listed */
  } /* end of loop on number of points  */
  last_screen_row[quadrant]+=numpts_listed;
}

/**************** SETUP_LISTS()  ************************************/

setup_lists(quadrant) 
int quadrant;

/* set up a list column for each preselected variable  */

{ 
int choice;  /* ordinal number of current listee being set up */ 
int total;


 for (choice=0; choice < MAX_LISTEES; choice++) {
  indep_ptr[quadrant]=dep_ptr[quadrant][choice]=dep_attr_num[quadrant][choice]=ERROR;
 }

 update_mode[quadrant]=INITIAL;
 start_listee[quadrant]=0;
 set_listees_per_page(quadrant);

 for(listee_number[quadrant]=-1,choice=0,total=num_of_listees[quadrant];
  choice < total;
  choice++) {

  listee_number[quadrant]++;

 /* convert user choices into list columns  */

  if ( (indices(quadrant,usrvar[quadrant][choice])) == ERROR) { 

/* error condition  */
    set_notice_needed(VARIABLE_NOT_AVAILABLE);
    report(quadrant,"variable not available");
   --num_of_listees[quadrant];
   --listee_number[quadrant];
  } 
 } 

 if (!(num_of_listees[quadrant])) {
/* simply return ERROR cond'n, caller should restore previous setup */
   report(quadrant,"bogus request; NO LISTEES !!");
   set_notice_needed(NO_LISTEES);
   return(ERROR);
 } 
 return(TRUE);
}

/*****************************  LIST_NAME_LABELS()  *************************/


list_listee_labels(quadrant)
int quadrant;

/* label columns with variable name and units

   on entry: dep_attr_num, window_height and window_width must be defined; 
   on exit: rheight and rtop are defined and all name labels are done
*/
{
int k;

/* define region dimensions */

 rtop[quadrant] = LISTROW(quadrant,LIST_HEADER_ROWS);
 rheight[quadrant] = window_height[quadrant] - rtop[quadrant];

/* start labelling with TIME */

 (*FillArea[GetDrawType()])(pw_xid[quadrant],
  0,0,window_width[quadrant],rtop[quadrant],0);
 (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,FOREGROUND,0,0,
  LISTFONTID(quadrant));
 (*DrawText[GetDrawType()])(
  LIST_LEFT_MARGIN,
  LISTROW(quadrant,1),
  "  TIME ");
/*
hours minutes seconds label
*/
 (*DrawText[GetDrawType()])(
  LIST_LEFT_MARGIN,
  LISTROW(quadrant,2),
   "hr mn sc");
/* 
column heading labels
*/
 for (k=0; k<MAX_LISTEES; k++){
  if (k < num_of_listees[quadrant]) {  
/* 
listee name
*/
   (void)sprintf(unit_label[quadrant][k],"%s",&units[dep_attr_num[quadrant][k]*UNITSLEN]);
   (void)sprintf(listee_label[quadrant][k],"%s",listee_name[quadrant][k]);
  } else if (k<MAX_LISTEES) {  
/* 
open slot label
*/
   (void)sprintf(listee_label[quadrant][k],"Open");
   (void)sprintf(unit_label[quadrant][k],"slot");
  }
/* 
do the labelling to window
*/
  if (k >= start_listee[quadrant] && 
      k < start_listee[quadrant] + listees_per_page[quadrant]) {
   int indx;
   indx=(k-start_listee[quadrant])%listees_per_page[quadrant]; 
   label_x[quadrant][indx]= 
    FIRST_HILITE_X(quadrant) + indx * COLUMN_WIDTH(quadrant);
/*
listee label
*/
   (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,FOREGROUND,0,0,
    GetFontID(GetListFont(quadrant,
/*
if length of string is less than nominal, can use default font
*/
     strlen(listee_label[quadrant][k])<=GetListDataLen())));
   (*DrawText[GetDrawType()])
    (CENTERED_X(quadrant,label_x[quadrant][indx],strlen(listee_label[quadrant][k]),
/*
if length of string is less than nominal, can use default font
*/
     (strlen(listee_label[quadrant][k])<=GetListDataLen())),
    LISTROW(quadrant,1),
    listee_label[quadrant][k]);
   (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,FOREGROUND,0,0,
    GetFontID(GetListFont(quadrant,
/*
if this an open slot, can use default font
*/
     k>= num_of_listees[quadrant])));
   (*DrawText[GetDrawType()])
    (CENTERED_X(quadrant,label_x[quadrant][indx],strlen(unit_label[quadrant][k]),
/*
if this an open slot, can use default font
*/
     (k>= num_of_listees[quadrant])),
    LISTROW(quadrant,2),
    unit_label[quadrant][k]);
  }
 }
 return;
}

/************************  DRAW_LIST_HEADER()  *******************************/

draw_list_header(quadrant)
int quadrant;

/*  erase background, draw the header with labels as per names and units in 
    plot_attr file 

    draw_list_header is called once on initial setup of window (on detection of
    SIGWINCH), and subsequently whenever user adds/changes/deletes a listee in 
    the window 
*/

{

 if (!(num_of_listees[quadrant]))  { /* might happen on bogus alternate request */
  return;
 }

 list_listee_labels(quadrant);
 need_rehilite[quadrant]=TRUE;

}

/***********************  NEXT_LIST_PAGE()  **********************************/

next_list_page(quadrant)
int quadrant;
{

/* change page only if there is a reason to --- i.e., if there is at least 
   one page worth of listees AND the maximum # of listees is not already 
   displayed (which it will be on Strip and Full-sized windows)
*/
 if (listees_per_page[quadrant]==MAX_LISTEES)

/* all listees already displayed on one page only
*/
  return;

 if (num_of_listees[quadrant]>=listees_per_page[quadrant]) {
  start_listee[quadrant]=(start_listee[quadrant]+listees_per_page[quadrant]);
  if (start_listee[quadrant]>num_of_listees[quadrant]
   || start_listee[quadrant]>=MAX_LISTEES)
/*
prevent unneeded page of all empty slots
*/
   start_listee[quadrant]=0;
  PreventWastedListArea(quadrant);
  update_mode[quadrant]=RECONFIG;
  inversed_row[quadrant]=FALSE;
 }
}

/****************  PreventWastedListArea()  *******************************/

PreventWastedListArea(quadrant)
int quadrant;
{
 if (start_listee[quadrant]+listees_per_page[quadrant]>MAX_LISTEES)
/*
prevent wide geometry with only leftmost slots filled, subsequent confusion
on right half when clicking for menu, wasted space
*/
  start_listee[quadrant]=MAX_LISTEES-listees_per_page[quadrant];
}

/****************  SET_LISTEES_PER_PAGE()  *******************************/

set_listees_per_page(quadrant)
int quadrant;
/* designate how many columns of listees will fit in each page of this quadrant 
   as function of its geometry, and how many pages are necessary to contain
   the maximum.  Initialize list page shown to first.
*/
{
 if (window_type[quadrant]!=DISPTYPE_LISTS)
  return;

 if (get_geom(quadrant)==GEOM_STRIP || get_geom(quadrant)==GEOM_FULL) {
  listees_per_page[quadrant]=WIDE_LISTEES;
  PreventWastedListArea(quadrant);
 } else {
  listees_per_page[quadrant]=DEFAULT_LISTEES;
 }
}

/****************  EVENT_IN_CHG_LISTEE_AREA()  *******************************/

event_in_chg_listee_area(x)
int x;
/* is the current canvas event in the area with the listee labels?
   If so, set listee_number to the value that points to the selected listee
   in the listee_label & label_x arrays, and return TRUE; else, return FALSE
*/
{

 if (cursor_in_ctl_panel ||
     cursor_in_control_panel || plot_type[current_quadrant]!=LISTS) 
  return(FALSE);
 if (x < FIRST_HILITE_X(current_quadrant))
  return (FALSE);

 if (!label_x[current_quadrant][0])
  return(FALSE);

 add_listee=FALSE;
 for (listee_number[current_quadrant]=start_listee[current_quadrant]; 
      listee_number[current_quadrant] < 
      start_listee[current_quadrant]+listees_per_page[current_quadrant]; 
      listee_number[current_quadrant]++) {
  if ( (x < label_x[current_quadrant]
      [(listee_number[current_quadrant]-start_listee[current_quadrant])
      %listees_per_page[current_quadrant]] + COLUMN_WIDTH(current_quadrant))
   ||

/* strip or full size window, cursor to right of last column
*/
     (listee_number[current_quadrant]==
      start_listee[current_quadrant]+listees_per_page[current_quadrant]-1 && 
      x > label_x[current_quadrant][
      (listee_number[current_quadrant]-start_listee[current_quadrant])
      %listees_per_page[current_quadrant]])
  ) {
   if (listee_number[current_quadrant] >= num_of_listees[current_quadrant]) {
    add_listee=TRUE;
    listee_number[current_quadrant]=num_of_listees[current_quadrant];
   } 
   return(TRUE);
  }
 }
 return (FALSE);
}

/********************  HIGHLIGHT_LISTEE_LABEL()  ******************************/

highlight_listee_label(quadrant)
int quadrant;

/* reverse video the selected listee_label
*/
{
int listee,column;

/* highlight background */

 listee=listee_number[quadrant];
 if (listee>=MAX_LISTEES)
  return;
 if (plot_type[quadrant]!=LISTS) 
  return;
 column=(listee-start_listee[quadrant])%listees_per_page[quadrant];
 (*FillArea[GetDrawType()])(pw_xid[quadrant],
  label_x[quadrant][column],0,
  COLUMN_WIDTH(quadrant), LISTROW(quadrant,LIST_HEADER_ROWS),
  BLU);
 (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,REVERSE_VIDEO,0,0,
  GetFontID(GetListFont(quadrant,
/*
if length of string is less than nominal, can use default font
*/
   strlen(listee_label[quadrant][listee])<=GetListDataLen())));
 (*DrawTtext[GetDrawType()])(
    label_x[quadrant][column]+(COLUMN_WIDTH(quadrant) -strlen(listee_label[quadrant][listee])
    * GetFontWidth(GetListFont(quadrant,
    (strlen(listee_label[quadrant][listee])<=GetListDataLen()))))/2,
    
  LISTROW(quadrant,1),listee_label[quadrant][listee]);
 (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,REVERSE_VIDEO,0,0,
  GetFontID(GetListFont(quadrant,
/*
if this is an open slot, use regular font, else use smaller one for units
*/
   listee>=num_of_listees[quadrant])));
 (*DrawTtext[GetDrawType()])(
    label_x[quadrant][column]+(COLUMN_WIDTH(quadrant)-strlen(unit_label[quadrant][listee])
    *GetFontWidth(GetListFont(quadrant,listee>= num_of_listees[quadrant])))/2,
    
  LISTROW(quadrant,2),
  unit_label[quadrant][listee]);
}


/**********************  CLEAR_COLUMN()  ************************************/

clear_column(quadrant,listee)
int quadrant,listee;
{
 if (plot_type[quadrant]!=LISTS) 
  return;
 if (listee>=MAX_LISTEES)
  return;
 listee-=start_listee[quadrant];
 (*FillArea[GetDrawType()])(pw_xid[quadrant],
  label_x[quadrant][listee%listees_per_page[quadrant]],
  rtop[quadrant],
  COLUMN_WIDTH(quadrant),rheight[quadrant],0);
}

/**********************  CLEAR_LISTEE_LABEL()  ******************************/

clear_listee_label(quadrant,mode,listee)
int quadrant,mode,listee;
/* restore the selected listee_label to normal video
*/
{
int column;

 if (listee==NONE)
  return;
 if (listee>=MAX_LISTEES)
  return;
 column=(listee-start_listee[quadrant])%listees_per_page[quadrant];
 if (plot_type[quadrant]!=LISTS) 
  return;
 (*FillArea[GetDrawType()])(pw_xid[quadrant],
  label_x[quadrant][column],0,
  COLUMN_WIDTH(quadrant),
  LISTROW(quadrant,LIST_HEADER_ROWS), 0);
 (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,FOREGROUND,0,0,
  GetFontID(GetListFont(quadrant,
/*
if length of string is less than nominal, can use default font
*/
   strlen(listee_label[quadrant][listee])<=GetListDataLen() 
        ))); 
 if (mode == REFRESH) { 
  (*DrawText[GetDrawType()])(
   CENTERED_X(quadrant,label_x[quadrant][column],strlen(listee_label[quadrant][listee]),
/*
if length of string is less than nominal, can use default font
*/
    (strlen(listee_label[quadrant][listee])<=GetListDataLen())),
   LISTROW(quadrant,1),
   listee_label[quadrant][listee]);
  (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,FOREGROUND,0,0,
   GetFontID(GetListFont(quadrant,
/*
if this an open slot, can use default font
*/
    listee>= num_of_listees[quadrant])));
  (*DrawText[GetDrawType()])(
   CENTERED_X(quadrant,label_x[quadrant][column],strlen(unit_label[quadrant][listee]),
/*
if length of string is less than nominal, can use default font
*/
    strlen(unit_label[quadrant][listee])<=GetListDataLen()),
  LISTROW(quadrant,2),
  unit_label[quadrant][listee]);
 }
}

/************************  CHG_LISTEE()  **********************************/

chg_listee(event)
Event *event;
{
 ShowVarMenu(current_quadrant,event);
}

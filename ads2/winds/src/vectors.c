/* vectors.c -- handle vector operations
*/
#include <xview/xview.h>
#include <stdio.h>
#include <math.h>

/****************************  WINDS INCLUDES  ******************************/

#include <xy_plt.h>
#include <vectors.h>
#include <colors.h>
#include <lengths.h>
#include <help.h>
#include <display.h>
#include <ops.h>
#include <macros.h>
#include <constants.h>
#include <proto.h>

/**************************  MODULE GLOBAL  ***********************************/

static int vector_intrvl[NUM_DISPLAY_QUADRANTS];
static int vector_max_scale[NUM_DISPLAY_QUADRANTS];
static char vector_x_comp[NUM_DISPLAY_QUADRANTS][NAMLEN],
     vector_y_comp[NUM_DISPLAY_QUADRANTS][NAMLEN];
static char vector_scale_label[NUM_DISPLAY_QUADRANTS][VECTOR_SCALE_TICKS][VALUELEN];
static char vector_units[NUM_DISPLAY_QUADRANTS][UNITSLEN];
static int vector_scale_value[NUM_DISPLAY_QUADRANTS][VECTOR_SCALE_TICKS];
static int font_height,
           font_width,
           text_left_x,
           tick_left_x,
           major_gap[NUM_DISPLAY_QUADRANTS],
           top_tick_y[NUM_DISPLAY_QUADRANTS];
static int Vector_color[NUM_DISPLAY_QUADRANTS]=
{CONTRAST_TRACE,CONTRAST_TRACE,CONTRAST_TRACE,CONTRAST_TRACE};
static int Vector_show[NUM_DISPLAY_QUADRANTS][MAX_TRACES]=
{TRUE,FALSE,FALSE,FALSE,
 TRUE,FALSE,FALSE,FALSE,
 TRUE,FALSE,FALSE,FALSE,
 TRUE,FALSE,FALSE,FALSE};


/****************************************************************************/

GetVectorFontHeight(int quadrant)
/*
return desired fontheight of vector fonts as per window height
*/
{
 return GetWINDSWindowHeight(quadrant)/24;
}

/****************************************************************************/

GetVectorFontWidth(int quadrant)
/*
return desired fontwidth of vector fonts as per window width
*/
{
 return GetWINDSWindowWidth(quadrant)/80;
}

/************************  VECTOR_INIT()  ***********************************/

vector_init(int quadrant)
{
int value,tick;


/* initialize scale values as function of max value
*/
  for (tick=0,value=0; tick<VECTOR_SCALE_TICKS; 
    tick++,value+=vector_max_scale[quadrant]/(VECTOR_SCALE_TICKS-1))
   vector_scale_value[quadrant][tick]=value;

/* initialize every time in case of changes to geometry
*/
  major_gap[quadrant]=(rbottom[quadrant]-rtop[quadrant])/(NUM_TIX-1);
  top_tick_y[quadrant]=rtop[quadrant]+major_gap[quadrant];
  tick_left_x=rright[quadrant];
  text_left_x=tick_left_x+VECTOR_TICK_LENGTH*2;
  font_height=GetFontHeight(GetVectorFont(quadrant));
  font_width=GetFontWidth(GetVectorFont(quadrant));

/* write scale labels from scale values
*/
  set_vector_scale_labels(quadrant);
}

/************************  SET_Y_COMPONENT()  ***************************/

set_y_component(int quadrant, char *y)
{
 (void)sprintf(vector_y_comp[quadrant],"%s",y);

/* set units as per y-component, assuming user has already or is about to
   set a x-component with matching units
*/
 set_vector_units(quadrant,y);
}

/************************  SET_X_COMPONENT()  ***************************/

set_x_component(int quadrant,char *x)
{
 (void)sprintf(vector_x_comp[quadrant],"%s",x);

/* set units as per x-component, assuming user has already or is about to
   set a y-component with matching units
*/
 set_vector_units(quadrant,x);
}

/**************************  GET_SHOW_VECTOR()  *****************************/

get_show_vector(int quadrant,int trace)
{
 return Vector_show[quadrant][trace];
}

/**************************  SET_SHOW_VECTOR()  *****************************/

set_show_vector(quadrant,show,trace,check)
int quadrant,show,trace,check;
{
 Vector_show[quadrant][trace]=show;
 if (check)
  CheckVectorButtonValue(quadrant,trace,show); 
}

/**************************  SET_VECTOR_COLOR()  *****************************/

set_vector_color(int quadrant,int value)
{
 Vector_color[quadrant]=value;
}

/**************************  GET_VECTOR_COLOR()  *****************************/

get_vector_color(int quadrant)
{
 return Vector_color[quadrant];
}

/************************  SET_VECTOR_UNITS()  ***************************/

set_vector_units(int quadrant,char *varname)
/* 
set units associated with track plot vectors as function of units assoc'd
with varname.  set_vector_units() is called in a fashion as to change the
vector units whenever either x- or y-component variable is changed.
*/
{
int index;

 if ((index=GetPlotAttrIndex(varname,TRUE))==ERROR) {
  (void)sprintf(vector_units[quadrant],"(unk)");
/*
DEFAULT varname not found ==> problem with indexing 
*/
  (void)fprintf(stderr,
   "WARNING: default vector units used for %s: unfound in GetPlotAttrIndex\n",
   varname);
 } else
  (void)sprintf(vector_units[quadrant],"(%s)",&units[index*UNITSLEN]);
 redraw_vector_scale_area(quadrant);
}

/************************  GET_Y_COMPONENT()  ***************************/

char *
get_y_component(int quadrant)
{
 return (char *)(vector_y_comp[quadrant]);
}

/************************  GET_X_COMPONENT()  ***************************/

char *
get_x_component(int quadrant)
{
 return (char *)(vector_x_comp[quadrant]);
}

/************************  SET_VECTOR_COMPONENTS()  ***************************/

set_vector_components(int quadrant, char *x, char *y)
{
 set_x_component(quadrant,x);
 set_y_component(quadrant,y);
}

/************************  SET_VECTOR_SCALE_LABELS()  ************************/

set_vector_scale_labels(int quadrant)
{
int tick;

 for (tick=VECTOR_SCALE_TICKS-1; tick>=0; tick--) {
  (void)sprintf(vector_scale_label[quadrant][VECTOR_SCALE_TICKS-1-tick],"%d",
                vector_scale_value[quadrant][tick]);
 }
}

/************************  DRAW_VECTOR_SCALE()  ***************************/

draw_vector_scale(int quadrant)
{
/* set some constant values
*/
 vector_init(quadrant);

/* draw descriptive text
*/
 draw_vector_legend(quadrant,NORMAL_VIDEO);

/* draw tick marks label with current vector scale
*/
 draw_vector_ticks(quadrant,NORMAL_VIDEO);
}

/***********************  DRAW_VECTOR_LEGEND()  ******************************/

draw_vector_legend(int quadrant, int mode)
{
  int legendcolor,width;
  int (**draw_func)();

  if (!major_gap[quadrant])
/*
all is not ready yet; try again later
*/
   return;

  legendcolor=mode==NORMAL_VIDEO?BLU:BKGND;
  draw_func=mode==NORMAL_VIDEO?DrawText:DrawTtext;
  (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,legendcolor,0,0,
   GetFontID(GetVectorFont(quadrant)));
  width=window_width[quadrant]-rright[quadrant]-VECTOR_TICK_LENGTH*2;
  if (mode==REVERSE_VIDEO)

/* then area is highlighted, and pending op is to chg scale: add "chg" to
   label
*/
   (*draw_func)(text_left_x+(width-font_width*strlen("chg"))/2,
    top_tick_y[quadrant]-(int)(3.7*font_height),"chg");

/* remainder of op label
*/
  (*draw_func)(text_left_x+(width-font_width*strlen("vector"))/2,
   top_tick_y[quadrant]-(int)(2.7*font_height),
   "vector");
  (*draw_func)(text_left_x+(width-font_width*strlen("scale"))/2,
   top_tick_y[quadrant]-(int)(1.7*font_height),"scale");
  (*draw_func)(text_left_x+(width-font_width*strlen(vector_units[quadrant]))/2,
   top_tick_y[quadrant]-(int)(0.7*font_height),vector_units[quadrant]);
}

/*****************************************************************************/

GetVectorLabelMargin(int quadrant)
/*
return pixel width needed to contain the vector scale textual info and 
scale object
*/
{
 return GetFontWidth(GetVectorFont(quadrant))*strlen(" vector ");
}

/*********************  DRAW_VECTOR_TICKS()  *********************************/

draw_vector_ticks(int quadrant,int mode)
{
int label,
    tickcolor,
    tick_y;
int (**draw_func)();

  if (!major_gap[quadrant])
/*
all is not ready yet; try again later
*/
   return;

  tickcolor=mode==NORMAL_VIDEO?BLU:BKGND;
  draw_func=mode==NORMAL_VIDEO?DrawText:DrawTtext;
  label=0;
  for (tick_y=top_tick_y[quadrant]; tick_y<=top_tick_y[quadrant]+major_gap[quadrant];
   tick_y=top_tick_y[quadrant]+(int)((label*major_gap[quadrant])/(VECTOR_SCALE_TICKS-1))) {
   (*ContextDrawLine[GetDrawType()])(pw_xid[quadrant],
    tick_left_x,tick_y,tick_left_x+VECTOR_TICK_LENGTH-1,tick_y,
    FOREGROUND,0,LineSolid);
   (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,tickcolor,0,0,
    GetFontID(GetVectorFont(quadrant)));
   (*draw_func)(text_left_x+font_width/2,tick_y+font_height/2,
    vector_scale_label[quadrant][label++]);
  }
}

/*********************  HIGHLIGHT_VECTOR_SCALE_AREA()  ***********************/

highlight_vector_scale_area(int quadrant)
{
  vector_init(quadrant);
  (*FillArea[GetDrawType()])(pw_xid[quadrant],
   VECTOR_SCALE_RECTANGLE(quadrant),
   BLU);
  draw_vector_legend(quadrant,REVERSE_VIDEO);
  draw_vector_ticks(quadrant,REVERSE_VIDEO);
}

/*********************  REDRAW_VECTOR_SCALE_AREA()  ***************************/

redraw_vector_scale_area(int quadrant)
{
  vector_init(quadrant);
  (*FillArea[GetDrawType()])(pw_xid[quadrant],
   VECTOR_SCALE_RECTANGLE(quadrant),
   0);
  draw_vector_legend(quadrant,NORMAL_VIDEO);
  draw_vector_ticks(quadrant,NORMAL_VIDEO);
}

/*********************  EVENT_IN_VECTOR_SCALE_AREA()  *************************/

event_in_vector_scale_area(int x,int y)
{
 if (plot_type[current_quadrant]!=TRACK_PLOT)
  return FALSE;

 if (x>=VECTOR_SCALE_AREA_XMIN(current_quadrant) && y>=VECTOR_SCALE_AREA_YTOP(current_quadrant) && y<=VECTOR_SCALE_AREA_YBOT(current_quadrant))
  return TRUE;
 
 return FALSE;
}

/***********************  SET_MAX_VECTOR_VALUE()  ****************************/

set_max_vector_value(int quadrant,int value)
{
int max_vector_val;
int clicks,direction;

 max_vector_val=MAX(vector_scale_value[quadrant][VECTOR_SCALE_TICKS-1],VECTOR_SCALE_TICKS-1);
 clicks=ABS(value-max_vector_val)/(VECTOR_SCALE_TICKS-1);
 direction=value>max_vector_val?UNZOOM_VECTOR:ZOOM_VECTOR;
 chg_vector_scale(quadrant,direction,clicks);
 
}

/************************  CHG_VECTOR_SCALE()  *******************************/

chg_vector_scale(int quadrant,int direction,int clicks)
{ 
int value;

  for (value=0; value<VECTOR_SCALE_TICKS; value++) {
   vector_scale_value[quadrant][value]+=direction==UNZOOM_VECTOR?clicks*value:
                                                   -1*clicks*value;
   vector_scale_value[quadrant][value]=MAX(vector_scale_value[quadrant][value],value);
  }
  vector_max_scale[quadrant]=vector_scale_value[quadrant][VECTOR_SCALE_TICKS-1];
  draw_bkgnd(quadrant);
}

/************************  SetVectorMaxScale()  ****************************/

SetVectorMaxScale(int quadrant,int value)
{
 vector_max_scale[quadrant]=value;
}

/************************  GetVectorMaxScale()  ****************************/

GetVectorMaxScale(int quadrant)
{
 return vector_max_scale[quadrant];
}

/************************  GET_VECTOR_INTERVAL()  ****************************/

get_vector_interval(int quadrant)
{
 return vector_intrvl[quadrant];
}

/************************  SET_VECTOR_INTERVAL()  ****************************/

set_vector_interval(int quadrant,int value)
{
 vector_intrvl[quadrant]=value;
}

/************************  VECTOR_TIME()  **********************************/

vector_time(int quadrant,int vectortime)
{
 if (!(vectortime % vector_intrvl[quadrant]))
  return TRUE;
 return FALSE;
}

/************************  VECTOR_SCALE()  ********************************/

float
vector_scale(int quadrant)
{
 return (float)major_gap[quadrant]/
        (float)vector_scale_value[quadrant][VECTOR_SCALE_TICKS-1];
}

/************************  DRAW_VECTOR()  **********************************/

draw_vector(quadrant,trace,derived_point,raw_point,xa,ya)
int quadrant,trace,derived_point,raw_point,xa,ya;
{
XPoint triangle[3];
double first_side_angle,
       second_side_angle,
       vector_angle,
       x_comp,y_comp;
int mode,arrow_length,xb,yb;

 arrow_length=VECTOR_ARROW_LENGTH;

/* compute parameters for vector and its arrowhead.  Turn vectors off if either
component is not found.
*/
 if ((mode=get_mode_by_name(vector_y_comp[quadrant]))==ERROR) {
  (void)fprintf(stderr,"WARNING: %s not found; turning vectors off, trace %d\n",
   vector_y_comp[quadrant],trace);
  set_show_vector(quadrant,FALSE,trace,TRUE);
  return;
 } 
 if (mode==DERIVED) {
  if ((y_comp=(double)get_derived_value(vector_y_comp[quadrant],derived_point))
   ==ERROR){
   return;
  }
 } else if((y_comp=(double)get_raw_value(vector_y_comp[quadrant],raw_point))
    ==ERROR) {
   return;
 } 

 if ((mode=get_mode_by_name(vector_x_comp[quadrant]))==ERROR) {
  (void)fprintf(stderr,"WARNING: %s not found; turning vectors off, trace %d\n",
   vector_y_comp[quadrant],trace);
  set_show_vector(quadrant,FALSE,trace,TRUE);
  return;
 }
 if (mode==DERIVED) {
  if ((x_comp=(double)get_derived_value(vector_x_comp[quadrant],derived_point))
    ==ERROR){
   return;
  }
 } else if((x_comp=(double)get_raw_value(vector_x_comp[quadrant],raw_point))
    ==ERROR) {
   return;
 } 
 if (x_comp)
  vector_angle=(atan(y_comp/x_comp)*FULL_CIRCLE)/(TWO_PI);
 else if (y_comp)
  vector_angle=y_comp>0?90.0:270.0;
 else 
/* 
x and y components are both zero; do not draw vector
*/
   return;
/* 
draw vector
*/
 xb=xa+vector_scale(quadrant)*x_comp;
 yb=ya-vector_scale(quadrant)*y_comp;
 (*DrawLine[GetDrawType()])(xa,ya,xb,yb);

/* adjust vector_angle to provide for full-circled angle so that arrowhead
   is pointing correctly
*/
 vector_angle+=x_comp<0 && y_comp>0?180.0:0;
 vector_angle+=x_comp<0 && y_comp<=0?180.0:0;
 vector_angle+=x_comp>0 && y_comp<0?360.0:0;

/* draw arrowhead
*/
 first_side_angle=(TWO_PI*(120.0+vector_angle))/FULL_CIRCLE;
 second_side_angle=(TWO_PI*(240.0+vector_angle))/FULL_CIRCLE;
 triangle[0].x=xb; 
 triangle[0].y=yb; 
 triangle[1].x=MIN(MAX_DRAW_COORDINATE,
  MAX(-MAX_DRAW_COORDINATE,xb+arrow_length*cos(first_side_angle))); 
 triangle[1].y=MIN(MAX_DRAW_COORDINATE,
  MAX(-MAX_DRAW_COORDINATE,yb-arrow_length*sin(first_side_angle))); 
 triangle[2].x=MIN(MAX_DRAW_COORDINATE,
  MAX(-MAX_DRAW_COORDINATE,xb+arrow_length*cos(second_side_angle))); 
 triangle[2].y=MIN(MAX_DRAW_COORDINATE,
  MAX(-MAX_DRAW_COORDINATE,yb-arrow_length*sin(second_side_angle))); 

/* fill in arrowhead polygon
*/
 (*DrawFilledPolygon[GetDrawType()])(triangle,3,Convex,CoordModeOrigin);
}

extern char *raw_list,*derive_list;

/*****************************************************************************/

ChgVectorXComponent(int selection) 
{
 switch (get_mode(&selection)) {
  case RAW:
   set_x_component(current_quadrant,&raw_list[NAMLEN*selection]);
   break;
  case DERIVED:
   set_x_component(current_quadrant,&derive_list[NAMLEN*selection]);
   break;
 }
 SetXComponentLabel(current_quadrant);
 set_show_vector(current_quadrant,GetBothComponentsSet(current_quadrant),0,TRUE);
}

/*****************************************************************************/

ChgVectorYComponent(int selection) 
{
 switch (get_mode(&selection)) {
  case RAW:
   set_y_component(current_quadrant,&raw_list[NAMLEN*selection]);
   break;
  case DERIVED:
   set_y_component(current_quadrant,&derive_list[NAMLEN*selection]);
   break;
 }
 SetYComponentLabel(current_quadrant);
 set_show_vector(current_quadrant,GetBothComponentsSet(current_quadrant),0,TRUE);
}

/*****************************************************************************/

GetBothComponentsSet(int quadrant)
{
 return (get_mode_by_name(get_x_component(quadrant))!=ERROR
     && get_mode_by_name(get_y_component(quadrant))!=ERROR);
}


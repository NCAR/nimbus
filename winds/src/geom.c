/* geom.c  -- manage shape and size of WINDS windows
*/
#include <stdio.h>
#include <xview/xview.h>
#include <xview/panel.h>

#include <constants.h>
#include <geom.h>
#include <display.h>
#include <op_panels.h>
#include <ops.h>
#include <proto.h>

/* available geometries
*/
static int geoms[NUM_GEOMS]=
{GEOM_QUAD,GEOM_STRIP,GEOM_STACK,GEOM_FULL};

static int window_hidden[NUM_QUADRANTS]=
{FALSE,FALSE,FALSE,FALSE,FALSE};

/* keep track of each window's geometry
*/
static int geometry[NUM_QUADRANTS];


/***************************  AVAIL_GEOMS()  *********************************/

avail_geoms(which)
int which;
/* return the which'th available geometry
*/
{
 return geoms[which];
}

/***************************  NUM_GEOMS()  ***********************************/

num_geoms()
/* return total number of available geometries
*/
{
 return NUM_GEOMS;
}

/***************************  TALL_GEOM()  ***********************************/

tall_geom(window)
int window;
/* return TRUE if geometry is a tall one, i.e. STACK or FULL
*/
{
 return (geometry[window]==GEOM_STACK || geometry[window]==GEOM_FULL); 
}

/***************************  WIDE_GEOM()  ***********************************/

wide_geom(window)
int window;
/* return TRUE if geometry is a wide one, i.e. STRIP or FULL
*/
{
 return (geometry[window]==GEOM_STRIP || geometry[window]==GEOM_FULL); 
}

/************************  GET_DEFAULT_GEOM()  ******************************/

get_default_geom()
{
 return GEOM_QUAD;
}

/***************************  GET_GEOM()  ***********************************/

get_geom(window)
int window;
/* return the geometry for window
*/
{
 return (geometry[window]); 
}

/***************************  SET_GEOM_PARMS()  ******************************/

set_geom_parms(window,geom)
int window,geom;
/* set parameters that control window's geometry
*/
{
 geometry[window]=geom;
 switch (geom) {
  case GEOM_QUAD:
   window_width[window]= control_width/2;
   window_height[window]=display_height/2-OP_PANEL_HEIGHT;
   break;
  case GEOM_STRIP:
   window_width[window]=control_width;
   window_height[window]=display_height/2-OP_PANEL_HEIGHT;
   break;
  case GEOM_STACK:
   window_width[window]=control_width/2;
   window_height[window]=display_height-OP_PANEL_HEIGHT;
   break;
  case GEOM_FULL:
   window_width[window]=control_width;
   window_height[window]=display_height-OP_PANEL_HEIGHT;
   break;
 }
 if (window<NUM_DISPLAY_QUADRANTS) {
  window_x[window]=geom_posn_x(window,geom);
  window_y[window]=geom_posn_y(window,geom);
 }
}

/***************************  GEOM_POSN_X()  *********************************/

geom_posn_x(window,geom)
int window,geom;
/* based on window position and its geometry, return the x-coordinate for that
   window's position
*/
{
 if (window==UPPER_LEFT || window==LOWER_LEFT || 
     geom==GEOM_STRIP || geom==GEOM_FULL)
  return 0;
 return (GetBaseFrameWidth()/2);
}

/***************************  GEOM_POSN_Y()  *********************************/

geom_posn_y(window,geom)
int window,geom;
/* based on window position and its geometry, return the y-coordinate for that
   window's position
*/
{
 if (window==UPPER_LEFT || window==UPPER_RIGHT || 
     geom==GEOM_STACK || geom==GEOM_FULL)
  return OP_PANEL_HEIGHT;
 return (display_height/2+OP_PANEL_HEIGHT);
}

/***************************  SET_GEOM()  ************************************/

set_geom(window,geom)
int window,geom;
/* take all steps to set window to given geometry: set parameters, chg graphical
   objects
*/
{
 set_geom_parms(window,geom);
 set_geometry(window);
}

/***************************  UNHIDE_WINDOW()  ******************************/

unhide_window(window)
int window;
/* re-show a window that was previously hidden -- windows are hidden to
   prevent confusion during geometry changes
*/
{
 SetWINDSShowWindow(window,TRUE);
 set_op_geom(window);
}

/***************************  HIDDEN_WINDOW()  ******************************/

hidden_window(window)
int window;
/* if this window has been hidden, return TRUE and reset boolean; else,
   return FALSE.
*/
{
 if (window_hidden[window]) {
  window_hidden[window]=FALSE;
  return TRUE;
 }
 return FALSE;
}

/***************************  SET_GEOMETRY()  ********************************/

set_geometry(window)
int window;
/* set window's graphical objects size and position as function of current
   geometry parameters, set flag to initiate redraw, put window on top of
   display stack
*/
{
/* hide window to prevent confusion between events and geometry before
   everything is done.  E.g., if a lower right strip is showing when it's  
   geometry is changed to stack, function event_in_add_trace_area() will
   (often) return TRUE due to cursor position when the re-sized window
   appears below the cursor.  This causes a trace label of "Add Trace" to
   appear and then disappear when everything gets synced, leaving a blanked
   strip across the display area.  Hiding the window until everything is
   synced prevents this, and probably numerous other as yet un-noticed
   glitches.  (The syncing is completed in update_displays(), who asks if
   window is hidden and re-shows it if needed.)
*/
 SetWINDSShowWindow(window,FALSE);

/* set flag to notify update_displays() that an unhide is needed
*/
 window_hidden[window]=TRUE;
/* 
now it's safe to change the shape of window
*/
 SetCanvasDimensions(window,
	window_x[window], window_y[window],
	window_width[window], window_height[window]);
/* 
list windows need to know how many columns are now displayable as result
of size change
*/
 set_listees_per_page(window);

/* protection: only if this is a normal display window, set update_mode[] 
   ( calib window calls this sometimes...)
*/
 if (window<NUM_DISPLAY_QUADRANTS)
  update_mode[window]=RECONFIG;
/*
pms2d doesn't use update_mode[]; force it here
*/
 if (pms2d_window_type(window))
  (void)refresh_window(INITIAL,window);
}

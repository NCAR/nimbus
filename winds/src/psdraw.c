/* psdraw.c  --  PostScript drawing routines interface
*/

#include <xview/xview.h>
#include <X11/Xlib.h>

/************************  WINDS INCLUDE FILES  *****************************/

#include <display.h>

/*****************************  EXPORTABLE **********************************/


/***************************  IMPORTED **************************************/


/***************************  GLOBAL TO MODULE  *****************************/


/***************************  FLUSH_DISPLAY()  ********************************/

PSFlushDisplay()
{
 return;
}

/***************************  INIT_DRAW()  ************************************/

PSInitDraw()
/* perform initialization to enable drawing routines functionality/objects
*/
{
 SetDrawType(PS);
 return;
}

/************************  INIT_MEM_DRAW()  **********************************/

/*ARGSUSED*/
PSInitMemDraw(image,width,height,depth)
Server_image *image;
int width,height,depth;
/* create new memory image, destroying old one if it exists and has different
dimensions than requested for new one.
*/
{
 return;
}

/***************************  SET_HILITE_COLORS()  ****************************/

/*ARGSUSED*/
PSSetHiliteColors(c1,c2)
int c1,c2;
/* set GC so that colors c1 and c2 may be drawn in XOR fashion
*/
{
 return;
}

/*****************************  SET_CONTEXT()  *******************************/

/*ARGSUSED*/
PSSetContext(xid,function,color,width,style,fid)
Drawable xid;
int color,width,style;
int fid;
/* set the graphics context for ensuing graphics operation(s)
*/
{
 return;
}

/***************************  SET_CLIPPING()  ********************************/

/*ARGSUSED*/
PSSetClipping(x,y,width,height)
int x,y,width,height;
{
 return;
}

/***************************  UNSET_CLIPPING()  ******************************/

PSUnsetClipping()
{
 return;
}

/***************************  GET_Y_OFFSET()  *********************************/

PSGetYOffset()
{
 return;
}

/***************************  GET_X_OFFSET()  *********************************/

PSGetXOffset()
{
 return;
}

/***************************  SET_OFFSET()  ***********************************/

/*ARGSUSED*/
PSSetOffset(x_offset,y_offset)
int x_offset,y_offset;
{
 return;
}

/***************************  FILL_AREA()  ***********************************/

/*ARGSUSED*/
PSFillArea(xid,x,y,width,height,color)
Drawable xid;
int x,y,width,height,color;
{
 return;
}

/********************  CONTEXT_DRAW_LINE()  **********************************/

/*ARGSUSED*/
PSContextDrawLine(xid,x0,y0,x1,y1,color,width,style)
Drawable xid;
int x0,y0,x1,y1,color,width,style;
{
 return;
}

/**********************  DRAW_FILLED_POLYGON()  ******************************/

/*ARGSUSED*/
PSDrawFilledPolygon(points,npoints,shape,mode)
XPoint *points;
int npoints,shape,mode;
{
 return;
}

/**********************  DRAW_CIRCLE()  **************************************/

/*ARGSUSED*/
PSDrawCircle(x,y,diameter)
int x,y,diameter;
{
 return;
}

/**********************  DRAW_LINE()  ****************************************/

/*ARGSUSED*/
PSDrawLine(xa,ya,xb,yb)
int xa,ya,xb,yb;
{
 return;
}

/**********************  LIMIT_COORDS() *************************************/

/*ARGSUSED*/
PSLimitCoords(xa,ya,xb,yb)
int *xa,*xb,*ya,*yb;
{
 return;
}

/**********************  DRAW_DASH_LINE() *************************************/

/*ARGSUSED*/
PSDrawDashLine(xid, x0, y0, x1, y1, dash_offset, dash_list, dash_list_len)
Drawable xid;				/* where to draw */
int x0, y0, x1, y1;			/* start/end coordinates */
int dash_offset;			/* line phase, usually 0 */
unsigned char *dash_list;		/* dash pattern information */
int dash_list_len;			/* length of dash_list */

/* Draws a patterned line.  Prior to using this routine, PSSetContext must be
   called setting the width and color of the line.  Also the line style must
   have been set to LineOnOffDash.
*/
{
 return;
}
/*********************  CONTEXT_DRAW_POLYLINE()  ******************************/

/*ARGSUSED*/
PSContextDrawPolyline(xid,numpts,list,color,width)
Drawable xid;
XPoint *list;
int numpts,color,width;
{
 return;
}

/**********************  DRAW_POLYLINE()  **************************************/

/*ARGSUSED*/
PSDrawPolyline(numpts,list)
int numpts;
XPoint *list;
{
 return;
}


/*********************  DRAW_POLYPOINT()  ************************************/

/*ARGSUSED*/
PSDrawPolypoint(numpts,list)
int numpts;
XPoint *list;
{
 return;
}

/***************************  RASTER_OP()  ************************************/

/*ARGSUSED*/
PSRasterOp(src_xid,source_x,source_y,dest_xid,dest_x,dest_y,
          dest_width,dest_height,op)
Drawable src_xid,dest_xid;
int source_x,source_y,dest_x,dest_y,dest_width,dest_height;
int op;
{
 return;
}

/***************************  MONO_RASTER_OP()  ******************************/

/*ARGSUSED*/
PSMonoRasterOp(src_xid,source_x,source_y,dest_xid,dest_x,dest_y,
               dest_width,dest_height,op,color)
Drawable src_xid,dest_xid;
int source_x,source_y,dest_x,dest_y,dest_width,dest_height,color;
int op;
/* copy from 1-bit deep pixmap to 8-bit deep image
*/
{
 return;
}

/**************************** DESTROY_IMAGE()  *******************************/

PSDestroyImage()
{
 return;
}

/****************************  GET_IMAGE()  **********************************/

/*ARGSUSED*/
char *
PSGetImage(xid,source_x,source_y,width,height)
Drawable xid;
int source_x,source_y,width,height;
{
 return;
}

/****************************  DRAW_TEXT()  **********************************/

/*ARGSUSED*/
PSDrawText(x,y,string)
int x,y;
char *string;
{
 return;
}

/****************************  DRAW_TTEXT()  **********************************/

/*ARGSUSED*/
PSDrawTtext(x,y,string)
int x,y;
char *string;
{
 return;
}

/**********************  GET_COLOR_MAP()  ************************************/

PSGetColorMap()
{
 return;
}

/**********************  SET_COLOR_MAP()  ************************************/

/*ARGSUSED*/
PSSetColorMap(which)
char *which;
{
 return;
}

/**********************  SETUP_COLORS()  ************************************/

PSSetupColors()
/* set up the canvas' colormap */
{
 return;
}

/* xdraw.c  --  xlib drawing routines interface
*/

#include <xview/xview.h>
#include <xview/canvas.h>
#include <xview/panel.h>
#include <xview/font.h>
#include <xview/cms.h>
#include <xview/svrimage.h>
#include <X11/Xlib.h>

#include <string.h>

/************************  WINDS INCLUDE FILES  *****************************/

#include <constants.h>
#include <colors.h>
#include <display.h>
#include <proto.h>

/*****************************  EXPORTABLE **********************************/

Display *win_display; 

/***
Window x_window,
       x_root_window;
***/

Cms cms;

XColor *xcolors;

int index_offset,      /* add to index into xcolors to get desired pixel */
    cms_size;


/***************************  GLOBAL TO MODULE  *****************************/

static Drawable drawable;
static GC gc;
static XGCValues gc_val; 
static char *cmsname="winds_colormap";
static int X_offset,
           Y_offset;
static int ColorMap;
static Xv_singlecolor *CmsColor;
static Xv_singlecolor BlueMap[] = {
/*
blue color map
*/
        { 128,    255,    255 },                /* BKGND (LT BLU) */
        { 0,      0,      255 },                /* BLU   */
        { 0,      128,     0 },                 /* GRN   */
        { 255,    0,    0 },                    /* RED   */
        { 178,    0,     178 },                 /* VIO   */
        { 255,    128,    0 },                  /* BRN   */
        { 0,     255,    0 },                   /* LTG   */
        { 128,     0,    178 },                 /* CYN   */
        { 255,     0,    255 },                 /* PUR   */
        { 0,     128,    128 },                 /* IND   */
        { 250,    130,    80 },                 /* ORG   */
        { 128,     128,    128 },                /* G3 */
        { 128,     128,    128 },               /* G4 */
        { 160,     160,    160 },               /* G5    */
        { 255,     255,    255 },               /* WHT   */
        { 0,     0,    0 },                     /* BLK   */
};
static Xv_singlecolor BlackMap[] = {
/*
black color map
*/
        { 0,     0,    0 },                     /* BKGND (BLK)   */
        { 128,   255,      255 },               /* BLU (LT BLU)   */
        { 0,    255,    0},                     /* GRN */
        { 255,    0,    0 },                    /* RED   */
        { 255,    255,     0 },                 /* VIO   (YELLOW) */
        { 255,    128,    0 },                  /* BRN   */
        { 32,    178,    170},                  /* LTG  (lightseagreen) */
        { 250,   250,    210 },                 /* CYN (lightgoldenrodyellow) */
        { 255,     0,    255 },                 /* PUR   */
        { 255,     165,    0 },                 /* IND (ORANGE)  */
        { 250,    130,    80 },                 /* ORG   */
        { 128,     128,    128 },                /* G3 */
        { 255,     255,    255 },               /* G4  (WHT)  */
        { 160,     160,    160 },               /* G5  */
        { 0,     0,    0 },               	/* WHT   */
 	{ 128,    255,    255 },                /* BLU (LT BLU) */
};

/***************************  FLUSH_DISPLAY()  ********************************/

X11FlushDisplay()
{
 XFlush(win_display);
}

/***************************  INIT_DRAW()  ************************************/

X11InitDraw()
/* perform initialization to enable drawing routines functionality/objects
*/
{
/* visible window drawing
*/
  win_display=(Display *)xv_get(GetBaseFrame(),XV_DISPLAY);
  gc=DefaultGC(win_display,DefaultScreen(win_display));

}

/************************  INIT_MEM_DRAW()  **********************************/

X11InitMemDraw(image,width,height,depth)
Server_image *image;
int width,height,depth;
/* create new memory image, destroying old one if it exists and has different
dimensions than requested for new one.
*/
{
static int last_image=NONE,last_width=NONE,last_height=NONE,last_depth=NONE;

/* no action needed unless at least one dimension is different
*/
  win_display=(Display *)xv_get(GetBaseFrame(),XV_DISPLAY);
  depth = XDefaultDepth(win_display, DefaultScreen(win_display));

  if (*image && *image==last_image && 
       width==last_width && height==last_height && depth==last_depth)
   return;

  if (*image)
   xv_destroy_safe(*image);

  *image=(Server_image)xv_create((int)NULL, SERVER_IMAGE,
        XV_WIDTH,               width,
        XV_HEIGHT,              height,
        SERVER_IMAGE_DEPTH,     depth,
        SERVER_IMAGE_COLORMAP,  cmsname,
        NULL);
  last_image=*image;  last_width=width;  last_height=height; last_depth=depth;
}

/***************************  SET_HILITE_COLORS()  ****************************/

X11SetHiliteColors(c1,c2)
int c1,c2;
/* set GC so that colors c1 and c2 may be drawn in XOR fashion
*/
{
 gc_val.foreground=xcolors[c1+index_offset].pixel
                  ^xcolors[c2+index_offset].pixel;
 XChangeGC(win_display,gc,
  GCFunction|GCForeground|GCBackground|GCLineWidth|GCLineStyle,
  &gc_val);
}

/*****************************  SET_CONTEXT()  *******************************/

X11SetContext(xid,function,color,width,style,fid)
Drawable xid;
int function,color,width,style, fid;
/* set the graphics context for ensuing graphics operation(s)
*/
{
 gc_val.function=function;
 gc_val.foreground=xcolors[color+index_offset].pixel;
 gc_val.line_width=width;
 gc_val.line_style=style;
 if (fid) {
/* if this is for a font, or caller wishes to force setting of background, 
   width doubles as the background color.  Set fid to TRUE in latter case
   if font is not being used.
*/
  gc_val.background = xcolors[width+index_offset].pixel;
  if (fid!=TRUE)
   XSetFont(win_display,gc,(Font)fid);
 }
 XChangeGC(win_display,gc,
  GCFunction|GCForeground|GCBackground|GCLineWidth|GCLineStyle,
  &gc_val);
 drawable = xid;
}

/***************************  SET_CLIPPING()  ********************************/

X11SetClipping(x,y,width,height)
int x,y,width,height;
{
XRectangle rectangle;

 rectangle.x=x;
 rectangle.y=y;
 rectangle.width=width;
 rectangle.height=height;
 XSetClipRectangles(win_display,gc,0,0,&rectangle,1,Unsorted);
}

/***************************  UNSET_CLIPPING()  ******************************/

X11UnsetClipping()
{
 XSetClipMask(win_display,gc,None);
}

/***************************  GET_Y_OFFSET()  *********************************/

X11GetYOffset()
{
 return Y_offset;
}

/***************************  GET_X_OFFSET()  *********************************/

X11GetXOffset()
{
 return X_offset;
}

/***************************  SET_OFFSET()  ***********************************/

X11SetOffset(x_offset,y_offset)
int x_offset,y_offset;
{
 X_offset=x_offset;
 Y_offset=y_offset;
}

/***************************  FILL_AREA()  ***********************************/

X11FillArea(xid,x,y,width,height,color)
Drawable xid;
int x,y,color;
unsigned int width,height;
{

 X11SetContext(xid,GXcopy,color,0,0,0);
 XFillRectangle(win_display,drawable,gc,x,y,width,height);

}

/********************  CONTEXT_DRAW_LINE()  **********************************/

X11ContextDrawLine(xid,x0,y0,x1,y1,color,width,style)
Drawable xid;
int x0,y0,x1,y1,color,width,style;
{
 X11SetContext(xid,GXcopy,color,width,style,0);
 X11DrawLine(x0,y0,x1,y1); 
}

/**********************  DRAW_FILLED_POLYGON()  ******************************/

X11DrawFilledPolygon(points,npoints,shape,mode)
XPoint *points;
int npoints,shape,mode;
{
 XFillPolygon(win_display,drawable,gc,points,npoints,shape,mode);
}

/**********************  DRAW_CIRCLE()  **************************************/

X11DrawCircle(x,y,diameter)
int x,y;
unsigned int diameter;
{
 XFillArc(win_display,drawable,gc,(int)(x-diameter/2),(int)(y-diameter/2),diameter,diameter,0,360*64);
}

/**********************  DRAW_LINE()  ****************************************/

X11DrawLine(xa,ya,xb,yb)
int xa,ya,xb,yb;
{
 X11LimitCoords(&xa,&ya,&xb,&yb);
 XDrawLine(win_display,drawable,gc,xa,ya,xb,yb); 
}

/**********************  LIMIT_COORDS() *************************************/

X11LimitCoords(xa,ya,xb,yb)
int *xa,*xb,*ya,*yb;
{
 *xa=MIN(MAX_DRAW_COORDINATE,*xa); *xb=MIN(MAX_DRAW_COORDINATE,*xb);
 *ya=MIN(MAX_DRAW_COORDINATE,*ya); *yb=MIN(MAX_DRAW_COORDINATE,*yb);
 *xa=MAX(-MAX_DRAW_COORDINATE,*xa); *xb=MAX(-MAX_DRAW_COORDINATE,*xb);
 *ya=MAX(-MAX_DRAW_COORDINATE,*ya); *yb=MAX(-MAX_DRAW_COORDINATE,*yb);
}

/**********************  DRAW_DASH_LINE() *************************************/

X11DrawDashLine(xid, x0, y0, x1, y1, dash_offset, dash_list, dash_list_len)
Drawable xid;				/* where to draw */
int x0, y0, x1, y1;			/* start/end coordinates */
int dash_offset;			/* line phase, usually 0 */
char dash_list[];		/* dash pattern information */
int dash_list_len;			/* length of dash_list */

/* Draws a patterned line.  Prior to using this routine, X11SetContext must be
   called setting the width and color of the line.  Also the line style must
   have been set to LineOnOffDash.
*/
{
  XSetDashes (win_display, gc, dash_offset, dash_list, dash_list_len);  
  X11LimitCoords(&x0,&y0,&x1,&y1);
  XDrawLine (win_display, xid, gc, x0, y0, x1, y1); 
}
/*********************  CONTEXT_DRAW_POLYLINE()  ******************************/

X11ContextDrawPolyline(xid,numpts,list,color,width)
Drawable xid;
XPoint *list;
int numpts,color,width;
{

 X11SetContext(xid,GXcopy,color,width,LineSolid,0);
 if (numpts)
  XDrawLines(win_display,drawable,gc,list,numpts,CoordModeOrigin);
}

/**********************  DRAW_POLYLINE()  **************************************/

X11DrawPolyline(numpts,list)
int numpts;
XPoint *list;
{
 if (numpts)
  XDrawLines(win_display,drawable,gc,list,numpts,CoordModeOrigin);
}


/*********************  DRAW_POLYPOINT()  ************************************/

X11DrawPolypoint(numpts,list)
int numpts;
XPoint *list;
{
 if (numpts)
  XDrawPoints(win_display,drawable,gc,list,numpts,CoordModeOrigin);
}

/***************************  RASTER_OP()  ************************************/

X11RasterOp(src_xid,source_x,source_y,dest_xid,dest_x,dest_y,
          dest_width,dest_height,op)
Drawable src_xid,dest_xid;
int source_x,source_y,dest_x,dest_y;
unsigned int dest_width,dest_height;
int op;
{

 X11SetContext(dest_xid,op,0,0,0,0);
 XCopyArea(win_display,
          src_xid,dest_xid,
          gc,source_x,source_y,dest_width,dest_height,dest_x,dest_y);

}

/***************************  MONO_RASTER_OP()  ******************************/

X11MonoRasterOp(src_xid,source_x,source_y,dest_xid,dest_x,dest_y,
               dest_width,dest_height,op,color)
Drawable src_xid,dest_xid;
int source_x,source_y,dest_x,dest_y,color;
unsigned int dest_width,dest_height;
int op;
/* copy from 1-bit deep pixmap to 8-bit deep image
*/
{
 X11SetContext(dest_xid,op,color,WHT,0,TRUE);
 XCopyPlane(win_display,src_xid,dest_xid,
          gc,source_x,source_y,dest_width,dest_height,dest_x,dest_y,
          (unsigned long)1);
}

static XImage *image;

/**************************** DESTROY_IMAGE()  *******************************/

X11DestroyImage()
{
 (void)XDestroyImage(image);
}

/****************************  GET_IMAGE()  **********************************/

char *
X11GetImage(xid,source_x,source_y,width,height)
Drawable xid;
int source_x,source_y;
unsigned int width,height;
{

 image=XGetImage(win_display,xid,
         source_x,source_y,width,height,(unsigned long)AllPlanes,ZPixmap);         
 return((char *)image->data);
}

/****************************  DRAW_TEXT()  **********************************/

X11DrawText(x,y,string)
int x,y;
char *string;
{
 XDrawImageString(win_display,drawable,gc,x,y,string,strlen(string));
}

/****************************  DRAW_TTEXT()  **********************************/

X11DrawTtext(x,y,string)
int x,y;
char *string;
{
 XDrawString(win_display,drawable,gc,x,y,string,strlen(string));
}

/**********************  GET_COLOR_MAP()  ************************************/

X11GetColorMap()
{
 return ColorMap;
}

/**********************  SET_COLOR_MAP()  ************************************/

X11SetColorMap(which)
char *which;
{
 if (!strcasecmp(which,LTBLU_STRING)) {
  CmsColor=&BlueMap[0];
  ColorMap=LTBLU_MAP;
 } else 
 if (!strcasecmp(which,BLACK_STRING)) {
  CmsColor=&BlackMap[0];
  ColorMap=BLACK_MAP;
 } 
}

/**********************  SETUP_COLORS()  ************************************/

X11SetupColors()
/* set up the canvas' colormap */
{

 cms = xv_create((int)NULL, CMS, 
              CMS_NAME, cmsname,
              CMS_CONTROL_CMS, TRUE,
              CMS_SIZE,     CMS_CONTROL_COLORS + NUMCOLORS,
              CMS_TYPE, XV_STATIC_CMS,
              CMS_COLORS,   CmsColor,
        NULL); 
 cms_size=(unsigned long)xv_get(cms, CMS_SIZE);
 if (xcolors)
  free((char *)xcolors);
 xcolors=(XColor *)malloc((unsigned)(cms_size*sizeof(XColor)));
 xcolors=(XColor *)xv_get(cms,CMS_X_COLORS,xcolors);
 index_offset=cms_size-NUMCOLORS;
}

RepaintPanel(panel,width,height,color)
Panel panel;
int width,height,color;
{
static XGCValues values;
static GC panelgc;
static Display *paneldisplay;
/***
 X11FillArea(xv_get(panel,XV_XID),0,0,width,height,PANELCOLOR);
***/
 values.foreground=xcolors[color+index_offset].pixel;
 values.background=xcolors[color+index_offset].pixel;
 values.graphics_exposures=FALSE;
 if (!paneldisplay) {
  values.plane_mask=1L;
  paneldisplay=(Display *)xv_get(panel,XV_DISPLAY);
 }
 panelgc=XCreateGC(paneldisplay,xv_get(panel,XV_XID),
  GCGraphicsExposures|/***GCForeground|GCBackground|***/GCPlaneMask,&values);
 XFillRectangle(paneldisplay,xv_get(panel,XV_XID),panelgc,0,0,width,height);
}


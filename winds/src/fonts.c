/*
fonts.c
*/

#include <stdio.h>
#include <xview/xview.h>
#include <xview/font.h>

#include <display.h>
#include <help.h>
#include <constants.h>
#include <proto.h>

/*
parameters describing fonts
*/

#define MINFONTHEIGHT 11
#define MINFONTWIDTH 6
#define MAXFONTHEIGHT 21
#define MAXFONTWIDTH 11

#define NUMFONTS 7
#define BOLD10L "-b&h-lucidatypewriter-bold-r-*-*-*-100-*-*-*-*-*-*" /*11x6*/
#define BOLD10C   "-adobe-courier-bold-r-*-*-*-100-*-*-*-*-*-*"      /*12x6*/
#define BOLD12L "-b&h-lucidatypewriter-bold-r-*-*-*-120-*-*-*-*-*-*" /*13x7*/
#define BOLD14L "-b&h-lucidatypewriter-bold-r-*-*-*-140-*-*-*-*-*-*" /*15x9*/
#define BOLD18C   "-adobe-courier-bold-r-*-*-*-180-*-*-*-*-*-*"      /*18x11*/
#define BOLD18L "-b&h-lucidatypewriter-bold-r-*-*-*-180-*-*-*-*-*-*" /*19x11*/
#define MED18L "-b&h-lucidatypewriter-medium-r-*-*-*-180-*-*-*-*-*-*"/*21x11*/

static char FontName[NUMFONTS][128]={
BOLD10L, BOLD10C, BOLD12L, BOLD14L, BOLD18C, BOLD18L, MED18L
};
static Xv_font WindsFont[NUMFONTS];

static int debugfonts=0;

static Xv_font              /* fonts for various windows */
	PMS1dDisplayFont[NUM_QUADRANTS],
	PMS2dDisplayFont[NUM_QUADRANTS],
  	DefaultListFont[NUM_QUADRANTS],
  	SmallListFont[NUM_QUADRANTS],
  	FixedDisplayFont[NUM_QUADRANTS],
	TimeStampFont[NUM_QUADRANTS],
        RangeLabelFont[NUM_QUADRANTS],
        DefaultRangeLabelFont[NUM_QUADRANTS],
        SmallRangeLabelFont[NUM_QUADRANTS],
	TimeFrameFont[NUM_QUADRANTS],
        TraceLabelFont[NUM_QUADRANTS],
	LandmarkFont[NUM_QUADRANTS],
	StandardFont,
 	CalibDisplayFont,
        ImageHeaderFont,
	VectorFont[NUM_QUADRANTS];
 
/*****************************************************************************/

SetWINDSFonts(frame)
Frame frame;
{
int font;

 for (font=0; font<NUMFONTS; font++ ) {
  SetWINDSFont(frame,&WindsFont[font],FontName[font]);
 }
 SetWINDSFont(frame,&ImageHeaderFont,BOLD14L);

 SetStandardFont();
}

/*****************************************************************************/

SetStandardFont()
{
 StandardFont=WindsFont[FindFont(GetStandardFontHeight(),
  GetStandardFontWidth())];
}

/*****************************************************************************/

GetMinFontHeight()
{
 return MINFONTHEIGHT;
}

/*****************************************************************************/

GetMinFontWidth()
{
 return MINFONTWIDTH;
}

/*****************************************************************************/

FindFont(height,width)
int height,width;
{
int font,found,hdiff,wdiff,olddiff,newdiff;

 olddiff=32768;
 found=ERROR;
 height=MIN(height,MAXFONTHEIGHT);
 width=MIN(width,MAXFONTWIDTH);
 height=MAX(height,MINFONTHEIGHT);
 width=MAX(width,MINFONTWIDTH);
 if (debugfonts)
  printf("\n\nDEBUG: looking for font with height %d and width %d...\n",
  height,width);
 for (font=0; font<NUMFONTS; font++ ) {
  hdiff=height-GetFontHeight(WindsFont[font]);
  wdiff=width-GetFontWidth(WindsFont[font]);
  if (debugfonts)
   printf("  DEBUG: font with height %d and width %d ",
    GetFontHeight(WindsFont[font]),GetFontWidth(WindsFont[font]));
  if (!hdiff && !wdiff) {
   found=font;
   if (debugfonts)
    printf("is an exact match\n");
   break;
  }
  if (hdiff>=0 && wdiff >=0) {
   newdiff=(hdiff*hdiff)+(wdiff*wdiff);
   if (debugfonts)
    printf("has total diff of %d\n",newdiff);
   if ( newdiff < olddiff ) {
    found=font;
    olddiff=newdiff;
   }
  } else {
   if (debugfonts)
    printf("has 1 dimension too large\n");
  }
 }
 if (found==ERROR)
/*
if all fonts here are too large, choose the smallest one
*/
  found=0;
 if (debugfonts)
  printf("DEBUG: choosing font with height %d and width %d\n",
    GetFontHeight(WindsFont[found]),GetFontWidth(WindsFont[found])); 
 return found;
}

/*****************************************************************************/

Xv_font
GetWINDSFont(which)
int which;
{
 return WindsFont[which];
}

/*****************************************************************************/

SetWINDSFont(frame,font,name)
Frame frame;
Xv_font *font;
char    name[];
{
if (!(*font = (Xv_font)xv_find(frame,FONT,
                FONT_NAME, name,
              NULL))) {
   *font=(Xv_font)xv_get(frame,XV_FONT);
   (void)fprintf(stderr,"WARNING: using default font instead of font %s",name);
  }
}

/*****************************************************************************/

SetDisplayFont(quadrant)
int quadrant;
{
 switch (GetWINDSWindowType(quadrant)) {
  case DISPTYPE_XYPLOT:
   SetTimeStampFont(quadrant,FindFont(GetTimeStampFontHeight(quadrant),
                             GetTimeStampFontWidth(quadrant)));
   SetVectorFont(quadrant,FindFont(GetVectorFontHeight(quadrant),
                             GetVectorFontWidth(quadrant)));
  case WTYPE_SKEWT:
   SetTimeFrameFont(quadrant,FindFont(GetTimeFrameFontHeight(quadrant),
                             GetTimeFrameFontWidth(quadrant)));
   SetRangeLabelFonts(quadrant,FindFont(GetRangeLabelFontHeight(quadrant),
                               GetRangeLabelFontWidth(quadrant)));
   SetTraceLabelFont(quadrant,FindFont(GetTraceLabelFontHeight(quadrant),
                              GetTraceLabelFontWidth(quadrant)));
   break;
  case DISPTYPE_PMS1D:
   Set1dDisplayFont(quadrant,FindFont(Get1dFontHeight(quadrant),
                             Get1dFontWidth(quadrant)));
   break;
  case DISPTYPE_PMS2D:
   Set2dDisplayFont(quadrant,FindFont(Get2dFontHeight(quadrant),
                             Get2dFontWidth(quadrant)));
   break;
  case DISPTYPE_LISTS:
   SetDefaultListFont(quadrant,FindFont(GetListFontHeight(TRUE,quadrant),
                               GetListFontWidth(TRUE,quadrant)));
   SetSmallListFont(quadrant,FindFont(GetListFontHeight(FALSE,quadrant),
                               GetListFontWidth(FALSE,quadrant)));
   break;
  case DISPTYPE_FIXEDFORMAT:
   SetFixedDisplayFont(quadrant,FindFont(GetFixedFontHeight(quadrant),
                             GetFixedFontWidth(quadrant)));
   break;
  case DISPTYPE_ETC:
   break;
  case WTYPE_CALIBDISP:
   break;
 }
}

/*****************************************************************************/
/*****************************************************************************/
/***********************   XY DISPLAY FONTS  *********************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

/*****************************************************************************/

Xv_font
GetRangeLabelFont(quadrant)
int quadrant;
{
 return RangeLabelFont[quadrant];
}

/*****************************************************************************/

Xv_font
GetTraceLabelFont(quadrant)
int quadrant;
{
 return TraceLabelFont[quadrant];
}

/*****************************************************************************/

Xv_font
GetTimeFrameFont(quadrant)
int quadrant;
{
 return TimeFrameFont[quadrant];
}

/*****************************************************************************/

Xv_font
GetTimeStampFont(quadrant)
int quadrant;
{
 return TimeStampFont[quadrant];
}

/*****************************************************************************/

SetTraceLabelFont(quadrant,which)
int quadrant,which;
{
 TraceLabelFont[quadrant]=WindsFont[which];
 SetLandmarkFont(quadrant,which?which-1:0);
}

/*****************************************************************************/

SetLandmarkFont(quadrant,which)
int quadrant,which;
{
 LandmarkFont[quadrant]=WindsFont[which];
}

/*****************************************************************************/

Xv_font
GetLandmarkFont(quadrant)
{
 return LandmarkFont[quadrant];
}

/*****************************************************************************/

SetTimeFrameFont(quadrant,which)
int quadrant,which;
{
 TimeFrameFont[quadrant]=WindsFont[which];
}

/*****************************************************************************/

SetTimeStampFont(quadrant,which)
int quadrant,which;
{
 TimeStampFont[quadrant] = WindsFont[which];
}

/*****************************************************************************/

SetRangeLabelFont(quadrant,defaultsize)
int quadrant;
int defaultsize;
{
 if (defaultsize)
  RangeLabelFont[quadrant]=DefaultRangeLabelFont[quadrant];
 else
  RangeLabelFont[quadrant]=SmallRangeLabelFont[quadrant];
}

/*****************************************************************************/

SetSmallRangeLabelFont(quadrant,which)
int quadrant,which;
{
 SmallRangeLabelFont[quadrant]=WindsFont[which];
}

/*****************************************************************************/

SetDefaultRangeLabelFont(quadrant,which)
int quadrant,which;
{
 DefaultRangeLabelFont[quadrant]=WindsFont[which];
}

/*****************************************************************************/

Xv_font
GetSmallRangeLabelFont(quadrant)
int quadrant;
{
 return SmallRangeLabelFont[quadrant];
}

/*****************************************************************************/

Xv_font
GetDefaultRangeLabelFont(quadrant)
int quadrant;
{
 return DefaultRangeLabelFont[quadrant];
}

/*****************************************************************************/

SetRangeLabelFonts(quadrant,which)
int quadrant,which;
{
 DefaultRangeLabelFont[quadrant]=WindsFont[which];
 SmallRangeLabelFont[quadrant]=WindsFont[
  FindFont(GetFontHeight(WindsFont[which])-1,GetFontWidth(WindsFont[which])-1)];
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/**********************   FIXED DISPLAY FONTS  *******************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

/*****************************************************************************/

SetFixedDisplayFont(quadrant,which)
int quadrant,which;
{
 FixedDisplayFont[quadrant]=WindsFont[which];
}

/*****************************************************************************/

Xv_font
GetFixedDisplayFont(quadrant)
int quadrant;
{
 return FixedDisplayFont[quadrant];
}

/*****************************************************************************/
/*****************************************************************************/
/**********************   LIST DISPLAY FONTS  ********************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

/*****************************************************************************/

Xv_font
GetListFont(quadrant,defaultsize)
int quadrant,defaultsize;
{
 return (defaultsize?GetDefaultListFont(quadrant):GetSmallListFont(quadrant));
}

/*****************************************************************************/

SetDefaultListFont(quadrant,which)
int quadrant,which;
{
 DefaultListFont[quadrant]=WindsFont[which];
}

/*****************************************************************************/

SetSmallListFont(quadrant,which)
int quadrant,which;
{
 SmallListFont[quadrant]=WindsFont[which];
}

/*****************************************************************************/

GetDefaultListFont(quadrant)
int quadrant;
{
 return DefaultListFont[quadrant];
}

/*****************************************************************************/

GetSmallListFont(quadrant)
int quadrant;
{
 return SmallListFont[quadrant];
}

/*****************************************************************************/
/*****************************************************************************/
/***********************  PMS1D DISPLAY FONTS  *******************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

Set1dDisplayFont(quadrant,which)
int quadrant,which;
{
 PMS1dDisplayFont[quadrant]=WindsFont[which];
}

/*****************************************************************************/

Xv_font
Get1dDisplayFont(quadrant)
int quadrant;
{
 return PMS1dDisplayFont[quadrant];
}

/*****************************************************************************/
/*****************************************************************************/
/***********************  PMS2D DISPLAY FONTS  *******************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

Set2dDisplayFont(quadrant,which)
int quadrant,which;
{
 PMS2dDisplayFont[quadrant]=WindsFont[which];
}

/*****************************************************************************/

Xv_font
Get2dDisplayFont(quadrant)
int quadrant;
{
 return PMS2dDisplayFont[quadrant];
}

/*****************************************************************************/
/*****************************************************************************/
/*********************  VECTOR FONTS  ****************************************/
/*****************************************************************************/
/*****************************************************************************/

/*****************************************************************************/

SetVectorFont(quadrant,which)
int quadrant,which;
{
 VectorFont[quadrant]=WindsFont[which];
}

/*****************************************************************************/

Xv_font
GetVectorFont(quadrant)
int quadrant;
{
 return VectorFont[quadrant];
}

/*****************************************************************************/

Xv_font
GetAlertFont() 
{
 return StandardFont;
}

/*****************************************************************************/

SetCalibDisplayFont() 
{
 CalibDisplayFont=StandardFont;
 SetTraceLabelFont(PLOTS,FindFont(GetTraceLabelFontHeight(PLOTS),
  GetTraceLabelFontWidth(PLOTS)));
 SetTimeStampFont(PLOTS,FindFont(GetTimeStampFontHeight(PLOTS),
  GetTimeStampFontWidth(PLOTS)));
 SetRangeLabelFonts(PLOTS,FindFont(GetRangeLabelFontHeight(PLOTS),
  GetRangeLabelFontWidth(PLOTS)));
 SetTimeFrameFont(PLOTS,FindFont(GetTimeFrameFontHeight(PLOTS),
  GetTimeFrameFontWidth(PLOTS)));
}

/*****************************************************************************/

Xv_font
GetCalibDisplayFont() 
{
 return CalibDisplayFont;
}

/*****************************************************************************/

Xv_font
GetAdscmdFont() 
{
 return StandardFont;
}

/*****************************************************************************/

Xv_font
GetStandardFont()
{
 return StandardFont;
}

/*****************************************************************************/

Xv_font
GetOpPanelFont()
{
 return StandardFont;
}

/*****************************************************************************/

Xv_font
GetControlPanelFont()
{
 return StandardFont;
}

/*****************************************************************************/

Xv_font
GetImageHeaderFont()
{
 return ImageHeaderFont;
}

/*****************************************************************************/

Xv_font
GetCopyrightFont()
{
 return WindsFont[NUMFONTS-1];
}

/*****************************************************************************/

static XFontStruct *fontinfo;      

XFontStruct *
GetFontInfoStruct(font)
Xv_font font;
{
 if (!font)
  return (XFontStruct *)NULL;

 fontinfo=(XFontStruct *)xv_get(font,FONT_INFO);
 return fontinfo;
}

/*****************************************************************************/

GetFontDescent(font)
Xv_font font;
{
 if (!font)
  return 0;
 return (int)((GetFontInfoStruct(font))->descent);
}

/*****************************************************************************/

GetFontAscent(font)
Xv_font font;
{
 if (!font)
  return 0;
 return (int)((GetFontInfoStruct(font))->ascent);
}

/*****************************************************************************/

GetFontID(font)
Xv_font font;
{
 if (!font)
  return 0;
 return (int)((GetFontInfoStruct(font))->fid);
}

/*****************************************************************************/

GetFontWidth(font)
Xv_font font;
{
 if (!font)
  return 0;
 return (int)((GetFontInfoStruct(font))->max_bounds.width);
}

/*****************************************************************************/

GetFontHeight(font)
Xv_font font;
{
 if (!font)
  return 0;
 return (int)
 ((GetFontInfoStruct(font))->descent+(GetFontInfoStruct(font))->ascent);
}

/*****************************************************************************/

GetFontBaseline(font)
Xv_font font;
{
 if (!font)
  return 0;
 return (int)((GetFontInfoStruct(font))->ascent);
}

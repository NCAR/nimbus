/* help.c -- create and manipulate the help image for button help
*/ 

#include <stdio.h>
#include <xview/xview.h>
#include <xview/canvas.h>
#include <xview/svrimage.h>

/****************************  WINDS INCLUDES  ******************************/

#include <display.h>
#include <help.h>
#include <gticons.h>
#include <colors.h>
#include <constants.h>
#include <macros.h>
#include <op_panels.h>
#include <errors.h>
#include <network.h>
#include <proto.h>

/*************************  MODULE SPECIFIC  **********************************/

static Server_image HelpImages[NUM_AREAS];   /* image for help images*/ 
static Drawable HelpImagesXID[NUM_AREAS];   /* drawable for help image */
static Server_image LeftOpImage[NUM_VAROPTS*2];/*image for left-op help images*/
static Drawable LeftOpImageXID[NUM_VAROPTS*2];/* drawable, left-op help image*/
static int LastHelpImage=NO_AREA;	/* last help image */
static Server_image ExtraHeightImage;


/***************************  make_help()  *********************************/


make_help()
/* 
make the help images
*/
{
#define make_image(image) (Server_image)xv_create((int)NULL,SERVER_IMAGE, \
                         XV_WIDTH,ICON_WIDTH_, \
                         XV_HEIGHT,ICON_HEIGHT_, \
                         SERVER_IMAGE_BITS, image, \
                NULL)

 Server_image pick_panel_item_image=make_image(pick_panel_item); 
 Server_image pick_text_item_image=make_image(pick_text_item); 
 Server_image select_dismiss_image=make_image(select_dismiss); 
 Server_image click_to_zoom_image=make_image(click_to_zoom); 
 Server_image click_to_unzoom_image=make_image(click_to_unzoom); 
 Server_image chg_full_menu_image=make_image(chg_full_menu); 
 Server_image delete_hilited_image=make_image(delete_hilited); 
 Server_image add_full_menu_image=make_image(add_full_menu); 
 Server_image blank_image=make_image(blank); 
 Server_image lmark_delete_image=make_image(lmark_delete); 
 ExtraHeightImage=make_image(extendor);

 MakeLeftOpImages();
 pw_xid[HELP_IMAGE]=
  (Drawable)xv_get(canvas_paint_window(canvas[HELP_IMAGE]),XV_XID);

/* make image by calling make_image() with arguments of area_name, image
   corresponding to LEFT, MIDDLE, and RIGHT buttons
*/
  make_images(CHG_TRACE_AREA,
               blank_image,
               chg_full_menu_image,
               chg_full_menu_image);

  make_images(CHG_LISTEE_AREA,
               blank_image,
               chg_full_menu_image,
               chg_full_menu_image);

  make_images(CHG_FIXEE_AREA,
               blank_image,
               chg_full_menu_image,
               chg_full_menu_image);

  make_images(CHG_INDEP_AREA,
               blank_image,
               chg_full_menu_image,
               chg_full_menu_image);

  make_images(ADD_TRACE_AREA,
               blank_image,
               add_full_menu_image,
               add_full_menu_image);

  make_images(CHG_Y_SCALE_AREA,
               blank_image,
               click_to_zoom_image,
               click_to_unzoom_image);

  make_images(CHG_TIME_SCALE_AREA,
               LeftOpImage[OP_DEFRNG],
               click_to_zoom_image,
               click_to_unzoom_image);

  make_images(CHG_X_SCALE_AREA,
               blank_image,
               click_to_zoom_image,
               click_to_unzoom_image);

  make_images(CHG_VECTOR_SCALE_AREA,
               blank_image,
               click_to_zoom_image,
               click_to_unzoom_image);

  make_images(PLOT_FRAME_AREA,
               blank_image,
               blank_image,
               blank_image);

  make_images(LANDMARK_OPS_AREA,
               blank_image,
               blank_image,
               lmark_delete_image);

  make_images(NO_AREA,
               blank_image,
               blank_image,
               blank_image);

  make_images(CTL_AREA,
               pick_panel_item_image,
               blank_image,
               pick_panel_item_image);

  make_images(PMS1D_ACCUM_AREA,
               blank_image,
               click_to_zoom_image,
               click_to_unzoom_image);

  make_images(PMS1D_YMAX_AREA,
               blank_image,
               click_to_zoom_image,
               click_to_unzoom_image);

  make_images(PMS1D_YMIN_AREA,
               blank_image,
               click_to_zoom_image,
               click_to_unzoom_image);

  make_images(PMS2D_AREA,
               blank_image,
               blank_image,
               blank_image);

  make_images(ADSPOP_AREA,
               pick_panel_item_image,
               blank_image,
               pick_panel_item_image);

  make_images(PRINTERS_MENU_AREA,
               pick_panel_item_image,
               blank_image,
               pick_panel_item_image);

  make_images(FOLDERS_MENU_AREA,
               pick_panel_item_image,
               blank_image,
               pick_panel_item_image);

  make_images(PLOTSPEX_MENU_AREA,
               pick_panel_item_image,
               blank_image,
               pick_panel_item_image);

  make_images(PRINTCONFIG_MENU_AREA,
               pick_panel_item_image,
               blank_image,
               pick_panel_item_image);

  make_images(NEW_SETUP_AREA,
               pick_panel_item_image,
               blank_image,
               pick_panel_item_image);

  make_images(OP_PANEL_AREA,
               pick_panel_item_image,
               blank_image,
               pick_panel_item_image);

  make_images(FLTFRAME_AREA,
               blank_image,
               blank_image,
               blank_image);

  make_images(CALIB_SETPT_AREA,
               delete_hilited_image,
               chg_full_menu_image,
               chg_full_menu_image);

  make_images(CALIB_DIALOGUE_AREA,
               pick_text_item_image,
               blank_image,
               pick_text_item_image);

  make_images(CALIB_PLOT_AREA,
               blank_image,
               chg_full_menu_image,
               chg_full_menu_image);

  make_images(MENU_AREA,
               blank_image,
               blank_image,
               select_dismiss_image);

  make_images(NOTICE_AREA,
               select_dismiss_image,
               blank_image,
               blank_image);

  (*RasterOp[GetDrawType()])(HelpImagesXID[NO_AREA],0,0,
           pw_xid[HELP_IMAGE],0,0,ICON_WIDTH_*NUM_BUTTONS,BUTTON_HEIGHT,
           GXcopy);
}

/********************  MakeLeftOpImages()  **********************************/

MakeLeftOpImages()
{
int area;
  LeftOpImage[OP_TOGGLE_FIX]=make_image(solo_toggle_fix); 
  LeftOpImage[OP_TOGGLE_MODE]=
/*
no mode toggle offered in analysis mode
*/
   post_processing_mode()?make_image(blank):make_image(solo_toggle_mode);
  LeftOpImage[OP_DEFRNG]=make_image(solo_defrng); 
  LeftOpImage[NUM_VAROPTS+OP_TOGGLE_FIX]=make_image(global_toggle_fix); 
  LeftOpImage[NUM_VAROPTS+OP_TOGGLE_MODE]=
   post_processing_mode()?make_image(blank):make_image(global_toggle_mode);
  LeftOpImage[NUM_VAROPTS+OP_DEFRNG]=make_image(global_defrng); 
  for (area=0; area < NUM_VAROPTS*2; area++) {
   LeftOpImageXID[area]=(Drawable)xv_get(LeftOpImage[area],XV_XID);
  }
}

/**************************  MAKE_IMAGES()  **********************************/

make_images(area,p1,p2,p3)
int area;
Server_image p1,p2,p3;
{
 Drawable p1_xid,p2_xid,p3_xid,ExtraHeightImageXID;
 extern Display *win_display;

 if (!(HelpImages[area] = (Server_image)xv_create((int)NULL, SERVER_IMAGE,
        XV_WIDTH,               ICON_WIDTH_*NUM_BUTTONS,
        XV_HEIGHT,              BUTTON_HEIGHT,
        SERVER_IMAGE_DEPTH,     XDefaultDepth(win_display, XDefaultScreen(win_display)),
//        SERVER_IMAGE_DEPTH,     16,
        NULL) )) {
  (void)fprintf(stderr,"FATAL: can't create memory image in make_images()\n");
  RequestAlertNow("FATAL: can't create memory image...contact WINDS expert with this information!");
  winds_cleanup(ERROR);
 }
 HelpImagesXID[area]=(Drawable)xv_get(HelpImages[area],XV_XID);
 p1_xid=(Drawable)xv_get(p1,XV_XID);
 p2_xid=(Drawable)xv_get(p2,XV_XID);
 p3_xid=(Drawable)xv_get(p3,XV_XID);
 ExtraHeightImageXID=(Drawable)xv_get(ExtraHeightImage,XV_XID);

 (*FillArea[GetDrawType()])(HelpImagesXID[area],0,0,ICON_WIDTH_*NUM_BUTTONS,
		BUTTON_HEIGHT, WHT);
 (*MonoRasterOp[GetDrawType()])(p1_xid,0,0,
  HelpImagesXID[area],0,0,ICON_WIDTH_,ICON_HEIGHT_,GXcopy,BLU);
 (*MonoRasterOp[GetDrawType()])(ExtraHeightImageXID,0,0,
  HelpImagesXID[area],0,ICON_HEIGHT_,ICON_WIDTH_,EXTENSION,GXcopy,BLU);

 (*MonoRasterOp[GetDrawType()])(p2_xid,0,0,
  HelpImagesXID[area],ICON_WIDTH_,0,ICON_WIDTH_,ICON_HEIGHT_,GXcopy,BLU);
 (*MonoRasterOp[GetDrawType()])(ExtraHeightImageXID,0,0,
  HelpImagesXID[area],ICON_WIDTH_,ICON_HEIGHT_,ICON_WIDTH_,EXTENSION,GXcopy,BLU);

 (*MonoRasterOp[GetDrawType()])(p3_xid,0,0,
  HelpImagesXID[area],ICON_WIDTH_*2,0,ICON_WIDTH_,ICON_HEIGHT_,GXcopy,BLU);
 (*MonoRasterOp[GetDrawType()])(ExtraHeightImageXID,0,0,
  HelpImagesXID[area],ICON_WIDTH_*2,ICON_HEIGHT_,ICON_WIDTH_,EXTENSION,GXcopy,BLU);
}

/**************************  UPDATE_LEFT_OP() ********************************/

update_leftop(quadrant,hilited)
int quadrant,hilited;
{
 if (GetScrollPanelShowing(FALSE))
  return;
 (*MonoRasterOp[GetDrawType()])
  (LeftOpImageXID[get_hilited_op(quadrant)+(hilited?0:NUM_VAROPTS)],
  0,0,pw_xid[HELP_IMAGE],0,0,ICON_WIDTH_,ICON_HEIGHT,
  GXcopy,BLU);
}

/*************************  CHG_TB_HELP_IMAGE()  ***************************/

/*ARGSUSED*/
chg_tb_help_image(area)
int area;
/* change the image being displayed in help_image to the image associated 
   with area
*/
{

 if (!pw_xid[HELP_IMAGE])
/* 
ACTION_NULL_EVENT triggers call here before all objects exist
*/
  return;

 if (alert_up) {

/* sometimes volunteer events occur just after a menu or popup goes up, that 
   result in a help image request which should be ignored.  If caller knows,
   however, that one of these is TRUE but wishes the call to eventually be
   applied as a help image update, caller should instead presently call 
   chg_help_images(HELPIMAGE_PENDING_IMAGE,<type>).  When the variable becomes FALSE 
   and it is 'safe', a call to chg_help_images(HELPIMAGE_LAST_IMAGE,0) will 
   complete the update with the last <type> sent with HELPIMAGE_PENDING_IMAGE. 
*/
  return;
 }
/*
protect help image when scroll panel is showing: mechanism to force help image
update in highlight_area() will override the just updated help image,
incorrectly
*/ 
 if (GetScrollPanelShowing(FALSE) && area !=CTL_AREA)
  return;

 (*RasterOp[GetDrawType()])(HelpImagesXID[area],0,0,
           pw_xid[HELP_IMAGE],0,0,ICON_WIDTH_*NUM_BUTTONS,BUTTON_HEIGHT,
           GXcopy);

/* left button op can be set by user on certain window types. Op executes
   individually for highlighted traces, globally when nothing is highlighted.
   Update help image accordingly.
*/

/* save last area moved to in case of repaint needed and for frame label
   changes on statistics setup mods */
  if (normal_tb_area(area))
   LastHelpImage=area;
  set_ctl_frame_label(area);
}

/*****************************  SET_NEXT_TB_IMAGE()  **************************/

set_next_tb_image(image)
int image;
{
 LastHelpImage=image;
}

/*****************************  LAST_TB_IMAGE()  **************************/

last_help_image()
{
 return LastHelpImage;
}

/* menus.c -- create and manage menus for WINDS
*/
#include <stdio.h>
#include <xview/xview.h>

/****************************  WINDS INCLUDES  ******************************/

#include <display.h>
#include <help.h>
#include <lengths.h>
#include <plotspex.h>
#include <ops.h>
#include <xview.h>
#include <files.h>
#include <constants.h>
#include <geom.h>
#include <errors.h>
#include <proto.h>
#include <pms1d.h>

/*************************  EXPORTABLE  *************************************/

/*
following menus are attached to menu buttons in op_panels.c:
*/
Menu XySubtypeMenu;               /* possible types of xy plots */
Menu Pms2dSubtypeMenu;               /* possible types of pms2d plots */
Menu EtcSubtypeMenu;               /* possible types of etcetera plots */
Menu PlotspecMenu[MAX_DISPLAY_TYPES]; /* menus under DispTypeString buttons */
/* 
strings enumerating types of possible displays, used by op_panels.c: 
*/
char DispTypeString[MAX_DISPLAY_TYPES][MAX_TYPE_LNGTH]= {
"xy-plot","1d","2d","list","fixed","etc"
};
/* 
TRUE when any menu dismissed WITH selection made 
*/
static int SelectionMade=FALSE; 

/***************************  GLOBAL WITHIN FILE  ****************************/

static Menu XyPlotspecMenu[NUM_XY_TYPES];  /* plotspex xy-type choices */
static Menu Pms2dPlotspecMenu[NUM_2D_TYPES];  /* plotspex 2d-type choices */
static Menu EtcPlotspecMenu[NUM_ETC_TYPES];  /* video, etc. choices */
static int menu_x,menu_y;	/* position and window where menu is invoked */
static Xv_Window menu_win;

/*****************************************************************************/

RememberCursorPosn(window,x,y)
Xv_Window window;
int x,y;
{
 menu_win=window;
 menu_x=x;
 menu_y=y;
}

/*****************************************************************************/

RestoreCursorPosn()
{
 (void)xv_set(menu_win,WIN_MOUSE_XY,menu_x,menu_y,NULL);
}

/****************************************************************************/
 
SetSelectionMade(selected)
int selected;
{
 SelectionMade=selected;
}

/*********************  NoSelectionMade()  **********************************/
 
void
NoSelectionMade()
/* 
provided for non-leaf menu items -- prevents default menu item selection
on pullright menus when user dismisses menu without fully extending pullright
cascade and making a selection.  
*/
{
 if (!cursor_in_ctl_panel && !cursor_in_control_panel) {
/***
  if (update_mode[current_quadrant]==ADD_TRACE) {
   switch (window_type[current_quadrant]) {
    case DISPTYPE_LISTS:
     --num_of_listees[current_quadrant];
     break;
    case DISPTYPE_FIXEDFORMAT:
     --num_of_fixees[current_quadrant];
     break;
    case DISPTYPE_XYPLOT:
     --num_of_traces[current_quadrant];
     break;
   }
  }
***/
  update_mode[current_quadrant]=NORMAL_UPDATE;
 }
}

/***********************  MenuDismissed()  **********************************/
 
void
MenuDismissed()
/* 
called after dismissal of any menu, with selection made or not.  Reset
cursor to original position and reset pertinent parameters.
*/
{
 if (!SelectionMade) {
  NoSelectionMade();
 }
 RestoreCursorPosn();
/* 
assume no selection made until next MENU_NOTIFY_PROC is called
*/
 SelectionMade=FALSE;
 SetChgVectorXComp(FALSE);
 SetChgVectorYComp(FALSE);
}

/*************************  GET_ENTRY()  ********************************/
 
/*ARGSUSED*/
void
get_entry(menu,item)
Menu menu;
Menu_item item;
{
int entry;
int new_type;

  SelectionMade=TRUE;
  entry=(int)xv_get(item,MENU_VALUE);

  new_type=xv_get(menu,MENU_CLIENT_DATA);

   switch (new_type) {
    case DISPTYPE_LISTS:  
    case DISPTYPE_FIXEDFORMAT:  
    case DISPTYPE_XYPLOT:
     config_display(current_quadrant,DISPTYPE_XYPLOT,entry);
     break;
    case DISPTYPE_PMS1D:
     config_display(current_quadrant,DISPTYPE_PMS1D,entry);
     break;
    case DISPTYPE_PMS2D:
     config_display(current_quadrant,DISPTYPE_PMS2D,entry);
     break;
    case DISPTYPE_ETC:
     config_display(current_quadrant,DISPTYPE_ETC,entry);
     break;
    default:
     (void)fprintf(stderr,"WARNING: new_type %d not in switch\n",new_type);
     return;
   }
   if (NewSetup(entry) && (
    new_type==DISPTYPE_PMS2D 
    || new_type==DISPTYPE_PMS1D
    )){
/*
user requests new setup via menu for 1d or 2d: no further steps needed, so
set and display default description
*/
    set_description(current_quadrant,"(new setup - untitled)");
    show_op_msg(current_quadrant,(char *)get_description(current_quadrant));
   }
 set_next_tb_image(window_type[current_quadrant]);
}

/*************************  MAKE_ALT_MENU()  ********************************/

make_alt_menu()
{
int proc_num;       /* used as index into plotspec[] entries during
                     processing loop */
int disptype,subtype,item;      /* other loop indices  */
static char XySubtypeString[NUM_XY_TYPES][MAX_TYPE_LNGTH]= {
/* subtypes of possible xy plots -- these MUST be in same order as their
   associated #define's in $WINDS/include/help.h
*/
"time series","track plot","skew-t/log-p","sounding","x-y (non-time)"
};
static char Pms2dSubtypeString[NUM_2D_TYPES][MAX_TYPE_LNGTH]= {
/* subtypes of possible 2d plots -- these MUST be in same order as their
   associated #define's in $WINDS/include/help.h
*/
"standard","grey scale","HVPS"
};
static char EtcSubtypeString[NUM_ETC_TYPES][MAX_TYPE_LNGTH]= {
/* subtypes of possible etcetera plots -- these MUST be in same order as their
   associated #define's in $WINDS/include/help.h
*/
"video"
};
/* 
following leaf_menu_ objects used to determine which font to use for leaf menus
*/
struct menu_info {
 int num_items;
 int max_chars;
};
struct menus_info {
 struct menu_info xy_plot_leaf[NUM_XY_TYPES]; 
 struct menu_info pms2d_plot_leaf[NUM_2D_TYPES]; 
 struct menu_info etc_plot_leaf[NUM_ETC_TYPES]; 
 struct menu_info non_xy_leaf[MAX_DISPLAY_TYPES];
} leaf_menu_sizer;

int length;

  destroy_alt_menus();

/* initialize information about # and length of items in each leaf menu
*/
  for (subtype=0; subtype < NUM_XY_TYPES; subtype++) {
   leaf_menu_sizer.xy_plot_leaf[subtype].num_items=
   leaf_menu_sizer.xy_plot_leaf[subtype].max_chars=0;
  }
  for (subtype=0; subtype < NUM_ETC_TYPES; subtype++) {
   leaf_menu_sizer.etc_plot_leaf[subtype].num_items=
   leaf_menu_sizer.etc_plot_leaf[subtype].max_chars=0;
  }
  for (subtype=0; subtype < NUM_2D_TYPES; subtype++) {
   leaf_menu_sizer.pms2d_plot_leaf[subtype].num_items=
   leaf_menu_sizer.pms2d_plot_leaf[subtype].max_chars=0;
  }
  for (subtype=0; subtype < get_num_display_types(); subtype++) {
   leaf_menu_sizer.non_xy_leaf[subtype].num_items=
   leaf_menu_sizer.non_xy_leaf[subtype].max_chars=0;
  }

/* make leaf menus: start with just an inactive top item (to prevent default
selection) and "new setup" option
*/
 for (subtype=0; subtype < NUM_XY_TYPES; subtype++) {
/* 
leaf menus for xy plot subtypes
*/
  XyPlotspecMenu[subtype] = xv_create( (int)NULL,MENU,
#include <menu_attr.h>
	      		MENU_NOTIFY_PROC, NoSelectionMade,
			MENU_TITLE_ITEM, XySubtypeString[subtype],
			MENU_CLIENT_DATA,DISPTYPE_XYPLOT,
                        MENU_ITEM,
                         MENU_STRING, "new setup",
                         MENU_VALUE, NEW_SETUP | subtype, 
			 MENU_NOTIFY_PROC, get_entry,
                        0,
                       NULL);
  SetFontForMenu(XyPlotspecMenu[subtype]);
 }
 for (subtype=0; subtype < NUM_ETC_TYPES; subtype++) {
/* 
leaf menus for etc subtypes
*/
  EtcPlotspecMenu[subtype] = xv_create((int) NULL,MENU,
#include <menu_attr.h>
	      		MENU_NOTIFY_PROC, NoSelectionMade,
			MENU_TITLE_ITEM, EtcSubtypeString[subtype],
			MENU_CLIENT_DATA,DISPTYPE_ETC,
                        MENU_ITEM,
                         MENU_STRING, "new setup",
/*
for now, video in post-processing mode is not available
	    	  	 MENU_INACTIVE,(subtype==VIDEO && post_processing_mode()),
re-activated 6/22/94...why not? */
                         MENU_VALUE, NEW_SETUP | subtype, 
			 MENU_NOTIFY_PROC, get_entry,
                        0,
                       NULL);
  SetFontForMenu(EtcPlotspecMenu[subtype]);
 }

 for (subtype=0; subtype < NUM_2D_TYPES; subtype++) {
/* 
leaf menus for 2d plot subtypes
*/
  Pms2dPlotspecMenu[subtype] = xv_create((int) NULL,MENU,
#include <menu_attr.h>
	      		MENU_NOTIFY_PROC, NoSelectionMade,
			MENU_TITLE_ITEM, Pms2dSubtypeString[subtype],
			MENU_CLIENT_DATA,DISPTYPE_PMS2D,
                        MENU_ITEM,
                         MENU_STRING, "new setup",
	    		 MENU_INACTIVE,(!NumberPMS2Dprobes(subtype)),
                         MENU_VALUE, NEW_SETUP | subtype, 
			 MENU_NOTIFY_PROC, get_entry,
                        0,
                       NULL);
  SetFontForMenu(Pms2dPlotspecMenu[subtype]);
 }

 for (disptype=0; disptype < get_num_display_types(); disptype++) {
/* 
leaf menus for all disptype other than xy-plot disptype
*/
  if (!MenuWithSubTypes(disptype)) {

/* hack needed: DISPTYPE_LISTS and DISPTYPE_FIXEDFORMAT are xy-plot 
   subtypes, but offered to user as separate window disptypes; adjust here to get 
   the indices right
*/
   subtype=disptype==DISPTYPE_LISTS?LISTS:
    (disptype==DISPTYPE_FIXEDFORMAT?FIXED_FORMAT:0/*** 0 because, so far, 
     only one DISPTYPE_ETC subtype***/);
/* 
all disptypes but those with subtypes, i.e. pms1d, lists, etc. 
*/
   PlotspecMenu[disptype] = xv_create((int) NULL,MENU,
#include <menu_attr.h>
	      		MENU_NOTIFY_PROC, NoSelectionMade,
                        MENU_TITLE_ITEM,DispTypeString[disptype],
			MENU_CLIENT_DATA,disptype,
                        MENU_ITEM,
                         MENU_STRING, "new setup",
/***
	    		 MENU_INACTIVE,(!NumberPMS1Dprobes()),
***/
                         MENU_VALUE, NEW_SETUP | subtype,
			 MENU_NOTIFY_PROC, get_entry,
                        0,
                       NULL);
   SetFontForMenu(PlotspecMenu[disptype]);
  } else if (disptype==DISPTYPE_XYPLOT) {
/* xy-plot categories (e.g., TRACK_PLOT, etc.) 
*/
   XySubtypeMenu = xv_create((int)  NULL,MENU,
#include <menu_attr.h>
	      		MENU_NOTIFY_PROC, NoSelectionMade,
			MENU_TITLE_ITEM, "x-y plot types",
                       NULL);
   SetFontForMenu(XySubtypeMenu);
  } else if (disptype==DISPTYPE_ETC) {
   EtcSubtypeMenu = xv_create((int)  NULL,MENU,
#include <menu_attr.h>
	      		MENU_NOTIFY_PROC, NoSelectionMade,
			MENU_TITLE_ITEM, "etcetera displays",
                       NULL);
   SetFontForMenu(EtcSubtypeMenu);
  } else if (disptype==DISPTYPE_PMS2D) {
   Pms2dSubtypeMenu = xv_create((int)  NULL,MENU,
#include <menu_attr.h>
	      		MENU_NOTIFY_PROC, NoSelectionMade,
			MENU_TITLE_ITEM, "pms2d plot types",
                       NULL);
   SetFontForMenu(Pms2dSubtypeMenu);
  }
 }
/* 
look at each shared memory structure plotspec[] entry 
*/
 proc_num=0; 
 while (proc_num<num_of_setups() &&
  plotspec && strlen(plotspec[proc_num].disptype)) {
  if (get_plotspec_validity(proc_num)) {
/* 
determine what display is being requested -- NOTE: these strings MUST match
the notion held by the project setup process, which creates the plotspex
file that was used to initialize this shared memory struct plotspec
*/
   if (!(strcmp(plotspec[proc_num].disptype,"xy_plt"))) {
    disptype=DISPTYPE_XYPLOT;
/* 
determine subtype of display requested
*/
    if (!(strcmp(plotspec[proc_num].subtype,"time_series"))) {
     subtype=TIME_SERIES;
    } else if (!(strcmp(plotspec[proc_num].subtype,"track_plot"))) {
     subtype=TRACK_PLOT;
    } else if (!(strcmp(plotspec[proc_num].subtype,"skew_t"))) {
     subtype=SKEW_T;
    } else if (!(strcmp(plotspec[proc_num].subtype,"sounding"))) {
     subtype=SOUNDING;
    } else if (!(strcmp(plotspec[proc_num].subtype,"xy"))) {
     subtype=XY;
    } else if (!(strcmp(plotspec[proc_num].subtype,"list_display"))) {
/* 
LISTS and FIXED_FORMAT are exceptions, in that they are handled by the xy 
plot display, but are listed in different menus. So, change the disptype
here as well.
*/
     subtype=LISTS;
     disptype=DISPTYPE_LISTS;
    } else if (!(strcmp(plotspec[proc_num].subtype,"fixed_display"))) {
     subtype=FIXED_FORMAT;
     disptype=DISPTYPE_FIXEDFORMAT;
    } 
/* 
set menu item with the plotspex entry
*/
    if (subtype!=LISTS && subtype !=FIXED_FORMAT) {
     menu_set(XyPlotspecMenu[subtype],
      MENU_ITEM,
       MENU_STRING, plotspec[proc_num].desc,
       MENU_VALUE, proc_num+1, 
       MENU_NOTIFY_PROC, get_entry,
      0,
     NULL);
/* update info about # and longest string in this leaf menu
*/
     length=strlen(plotspec[proc_num].desc);
     if (length > leaf_menu_sizer.xy_plot_leaf[subtype].max_chars)
      leaf_menu_sizer.xy_plot_leaf[subtype].max_chars=length;
     leaf_menu_sizer.xy_plot_leaf[subtype].num_items++;
    }
/* 
it's not handled by xy-plot display: 
*/
   } else if (!(strcmp(plotspec[proc_num].disptype,"pms1d"))) {
    disptype=DISPTYPE_PMS1D;
   } else if (!(strcmp(plotspec[proc_num].disptype,"etcetera"))) {
    disptype=DISPTYPE_ETC;
    if (!(strcmp(plotspec[proc_num].subtype,"video"))) {
     subtype=VIDEO;
    }
    menu_set(EtcPlotspecMenu[subtype],
     MENU_ITEM,
      MENU_STRING, plotspec[proc_num].desc,
      MENU_VALUE, proc_num+1, 
      MENU_NOTIFY_PROC, get_entry,
     0,
    NULL);
    length=strlen(plotspec[proc_num].desc);
    if (length > leaf_menu_sizer.etc_plot_leaf[subtype].max_chars)
     leaf_menu_sizer.etc_plot_leaf[subtype].max_chars=length;
    leaf_menu_sizer.etc_plot_leaf[subtype].num_items++;
   } else if (!(strcmp(plotspec[proc_num].disptype,"pms2d"))) {
    disptype=DISPTYPE_PMS2D;
/* 
set menu item with the plotspex entry
*/
    if (!(strcmp(plotspec[proc_num].subtype,"standard"))) {
     subtype=PMS2D_STANDARD;
    } else if (!(strcmp(plotspec[proc_num].subtype,"greyscale"))) {
     subtype=PMS2D_GREYSCALE;
    } else if (!(strcmp(plotspec[proc_num].subtype,"hvps"))) {
     subtype=PMS2D_HVPS;
    }
    menu_set(Pms2dPlotspecMenu[subtype],
     MENU_ITEM,
      MENU_STRING, plotspec[proc_num].desc,
      MENU_VALUE, proc_num+1, 
      MENU_NOTIFY_PROC, get_entry,
     0,
    NULL);
    length=strlen(plotspec[proc_num].desc);
    if (length > leaf_menu_sizer.pms2d_plot_leaf[subtype].max_chars)
     leaf_menu_sizer.pms2d_plot_leaf[subtype].max_chars=length;
    leaf_menu_sizer.pms2d_plot_leaf[subtype].num_items++;
   } 
/* 
set menu item with the plotspex entry
*/
   if (!MenuWithSubTypes(disptype)) {
/* 
update info about # and longest string in this leaf menu
*/
    length=strlen(plotspec[proc_num].desc);
    if (length > leaf_menu_sizer.non_xy_leaf[disptype].max_chars)
     leaf_menu_sizer.non_xy_leaf[disptype].max_chars=
      MAX(length,strlen(plotspec[proc_num].desc));
 
    menu_set(PlotspecMenu[disptype],
      MENU_ITEM,
       MENU_STRING, plotspec[proc_num].desc,
       MENU_VALUE, proc_num+1, /* MENU_VALUE should correspond to entry of
                                    argument file assoc'd with this plotspec,
                                    each of which is one more than index into
                                    plotspec[] struct */
       MENU_NOTIFY_PROC, get_entry,
      0,
     NULL);
   }
  }
  ++proc_num;
 }
/* 
construct top-level menus and add pull-rights created above
*/
 for (item=0; item < NUM_XY_TYPES; item++)
  menu_set(XySubtypeMenu,
            MENU_ITEM,
		MENU_STRING, XySubtypeString[item],
            	MENU_PULLRIGHT, XyPlotspecMenu[item],
	    NULL,
           NULL);
 for (item=0; item < NUM_2D_TYPES; item++)
  menu_set(Pms2dSubtypeMenu,
            MENU_ITEM,
		MENU_STRING, Pms2dSubtypeString[item],
            	MENU_PULLRIGHT, Pms2dPlotspecMenu[item],
	    NULL,
           NULL);
 for (item=0; item < NUM_ETC_TYPES; item++) {
  menu_set(EtcSubtypeMenu,
            MENU_ITEM,
		MENU_STRING, EtcSubtypeString[item],
            	MENU_PULLRIGHT, EtcPlotspecMenu[item],
	    NULL,
           NULL);
 }
}

/*****************************************************************************/

SetFontForMenu(menu)
Menu menu;
{
 (void)menu_set(menu,
  XV_FONT, GetStandardFont(),
 NULL);
}

/************************ MenuWithSubTypes()  ********************************/

MenuWithSubTypes(disptype)
int disptype;
{
  return (disptype == DISPTYPE_XYPLOT ||
          disptype == DISPTYPE_PMS2D ||
          disptype == DISPTYPE_ETC);
}

/*****************************************************************************/

DestroyMenu(menu)
Menu menu;
{
 if (menu)
  xv_destroy_safe(menu);
}

/***************************  DESTROY_ALT_MENUS()  ***************************/

destroy_alt_menus()
{
int type;
 for (type=0; type<NUM_XY_TYPES; type++) {
  DestroyMenu(XyPlotspecMenu[type]); 
 }
 for (type=0; type<NUM_2D_TYPES; type++) {
  DestroyMenu(Pms2dPlotspecMenu[type]); 
 }
 for (type=0; type<NUM_ETC_TYPES; type++) {
  DestroyMenu(EtcPlotspecMenu[type]); 
 }
 DestroyMenu(XySubtypeMenu);
 DestroyMenu(Pms2dSubtypeMenu);
 DestroyMenu(EtcSubtypeMenu);
 for (type=0; type<get_num_display_types(); type++) {
  if (type != DISPTYPE_XYPLOT) {
   DestroyMenu(PlotspecMenu[type]); 
  }
 }
}

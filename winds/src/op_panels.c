/* op_panels.c  -- create and manage the operation panels associated with
   each display quadrant
*/

#include <stdio.h>
#include <xview/xview.h>
#include <xview/panel.h>

/**************************  WINDS INCLUDES  *********************************/

#include <display.h>
#include <op_panels.h>
#include <help.h>
#include <ops.h>
#include <lengths.h>
#include <constants.h>
#include <xview.h>
#include <network.h>
#include <plotspex.h>
#include <xy_plt.h>
#include <printers.h>
#include <pms1d.h>
#include <proto.h>

/****************************  EXPORTABLE  **********************************/

Panel op_panel[NUM_DISPLAY_QUADRANTS]={0,0,0,0};

/************************  GLOBAL WITHIN MODULE  *****************************/

static Panel_item genop_choices[NUM_DISPLAY_QUADRANTS],
		  spex_button[NUM_DISPLAY_QUADRANTS][MAX_DISPLAY_TYPES],
		  hilited_ops[NUM_DISPLAY_QUADRANTS],
		  geom_button[NUM_DISPLAY_QUADRANTS],
		  indepvar_button[NUM_DISPLAY_QUADRANTS],
		  scale_button[NUM_DISPLAY_QUADRANTS],
		  scale_type_button[NUM_DISPLAY_QUADRANTS],
		  vector_button[NUM_DISPLAY_QUADRANTS],
		  vector_comps[NUM_DISPLAY_QUADRANTS],
		  vector_colors[NUM_DISPLAY_QUADRANTS],
		  VectorsOnOff[NUM_DISPLAY_QUADRANTS][MAX_TRACES],
		  lmrk_button[NUM_DISPLAY_QUADRANTS],
		  trk_type_item[NUM_DISPLAY_QUADRANTS],
		  geom_ops[NUM_DISPLAY_QUADRANTS],
		  lmrk_ops[NUM_DISPLAY_QUADRANTS],
                  probe_1d_ops[NUM_DISPLAY_QUADRANTS],
                  probe_1d_data[NUM_1D_DATATYPE_PARMS][NUM_DISPLAY_QUADRANTS],
                  probe_1d_scales[NUM_1D_SCALES_PARMS][NUM_DISPLAY_QUADRANTS],
                  probe_2d_probenames[NUM_DISPLAY_QUADRANTS],
		  history_ops[NUM_DISPLAY_QUADRANTS],
		  op_msg[NUM_DISPLAY_QUADRANTS];
static int current_panel[NUM_DISPLAY_QUADRANTS]=	{NONE,NONE,NONE,NONE},
           last_panel[NUM_DISPLAY_QUADRANTS]=		{NONE,NONE,NONE,NONE},
           SaveGenopValue[NUM_DISPLAY_QUADRANTS]=	{0,0,0,0},
           hilite_op[NUM_DISPLAY_QUADRANTS]=		{0,0,0,0},
           probe_val[MAX_DISPLAYED_1D_PROBES]=		{1,2,4,8,16,32},
           last_pms1d_val[NUM_DISPLAY_QUADRANTS]=	{0,0,0,0},
           last_pms2d_val[NUM_DISPLAY_QUADRANTS]=	{0,0,0,0},
           IsolateTraces[NUM_DISPLAY_QUADRANTS]=      {FALSE,FALSE,FALSE,FALSE},
           ScaleType[NUM_DISPLAY_QUADRANTS]= 
             {NORMAL_SCALE,NORMAL_SCALE,NORMAL_SCALE,NORMAL_SCALE},
           last_cnts_value[NUM_DISPLAY_QUADRANTS],
           last_liters_value[NUM_DISPLAY_QUADRANTS],
           last_ytype_value[NUM_DISPLAY_QUADRANTS],
           last_xtype_value[NUM_DISPLAY_QUADRANTS];
/***
           image_type=OP_COLORPRINT;
***/

static int ChgVectorXComp=FALSE,
           ChgVectorYComp=FALSE;
static int IntervalButtonXPosn=0;

/*********************** PANEL ITEM NOTIFY PROCEDURES *************************/

/**************************  HISTORY_PROC()  *********************************/

void
history_proc(item,value,event)
Panel_item item;
int value;
Event *event;
{
int quadrant;

 quadrant=xv_get(item,PANEL_CLIENT_DATA);

/***
 if (!frozen[quadrant]) {
***/

  if (value==OP_SHOW_HIST) {
   report(quadrant,"refresh");
   switch (window_type[quadrant]) {
    case WTYPE_CALIBDISP:
     break;
    default:
     if (refresh_window(INITIAL,quadrant))
      set_force_autoscale(quadrant,TRUE);
     break;
    }
  } else {

/* clear history
*/
   report(quadrant,"refresh w/o history");
   switch (window_type[quadrant]) {
    case WTYPE_CALIBDISP:
    case DISPTYPE_PMS1D:
    case DISPTYPE_LISTS:
    case DISPTYPE_FIXEDFORMAT:
    case DISPTYPE_ETC:
    case DISPTYPE_PMS2D:
     break;
    default:
     (void)refresh_window(START_NOW,quadrant);
     break;
    }
  }
/***
 }
***/
 xv_set(history_ops[quadrant],
 	PANEL_VALUE,0,
 	NULL);
}

/**************************  PROBE_1D_PROC()  *********************************/

void
probe_1d_proc(item,event)
Panel_item item;
Event *event;
{
int quadrant,total_value,value,probe;

 quadrant=xv_get(item,PANEL_CLIENT_DATA);
 total_value=xv_get(item,PANEL_VALUE);
 value=total_value ^ last_pms1d_val[quadrant];
 for (probe=0; probe < NumberPMS1Dprobes(); probe++) {
  if (value==probe_val[probe])
   TogglePMS1Dprobe(quadrant,probe);
 }
 last_pms1d_val[quadrant]=total_value;
}

/************************  PROBE_1D_CNTS_PROC()  ******************************/

void
probe_1d_cnts_proc(item,event)
Panel_item item;
Event *event;
{
int value,quadrant;

 quadrant=xv_get(item,PANEL_CLIENT_DATA);
 value=xv_get(item,PANEL_VALUE);
 if (value!=last_cnts_value[quadrant]) {
  TogglePMS1Ddatatype(quadrant);
  last_cnts_value[quadrant]=value;
  if (!isQuadrantCounts(quadrant))
   xv_set(probe_1d_data[PMS1D_DISP_LITERS][quadrant],XV_SHOW,TRUE,NULL);
  else {

/* on counts data-type, don't offer liters and ensure x-axis scale is set to
   linear
*/
   if (last_xtype_value[quadrant]!=0) {
    last_xtype_value[quadrant]=0;
    xv_set(probe_1d_scales[PMS1D_DISP_XTYPE][quadrant],
     PANEL_VALUE,last_xtype_value[quadrant],
    NULL);
   }
   xv_set(probe_1d_data[PMS1D_DISP_LITERS][quadrant],XV_SHOW,FALSE,NULL);
  }
 }
}

/**************************  PROBE_1D_LITERS_PROC()  ***************************/

void
probe_1d_liters_proc(item,event)
Panel_item item;
Event *event;
{
int value,quadrant;

 quadrant=xv_get(item,PANEL_CLIENT_DATA);
 value=xv_get(item,PANEL_VALUE);
 if (value!=last_liters_value[quadrant]) {
  TogglePMS1Dvolume(quadrant);
  last_liters_value[quadrant]=value;
 }
}

/**************************  PROBE_1D_XTYPE_PROC()  ***************************/

void
probe_1d_xtype_proc(item,event)
Panel_item item;
Event *event;
{
int value,quadrant;

 quadrant=xv_get(item,PANEL_CLIENT_DATA);
 value=xv_get(item,PANEL_VALUE);
 if (value!=last_xtype_value[quadrant]) {
  TogglePMS1DxaxisType(quadrant);
  last_xtype_value[quadrant]=value;
 }
}

/************************  PROBE_1D_YTYPE_PROC()  *****************************/

void
probe_1d_ytype_proc(item,event)
Panel_item item;
Event *event;
{
int value,quadrant;

 quadrant=xv_get(item,PANEL_CLIENT_DATA);
 value=xv_get(item,PANEL_VALUE);
 if (value!=last_ytype_value[quadrant]) {
  TogglePMS1DyaxisType(quadrant);
  last_ytype_value[quadrant]=value;
 }
}

/**************************  HILITED_PROC()  *********************************/

void
hilited_proc(item,event)
Panel_item item;
Event *event;
{
int quadrant;
int value;

 quadrant=xv_get(item,PANEL_CLIENT_DATA);
 value=xv_get(item,PANEL_VALUE);
 if (post_processing_mode()) {
/*
mode toggling not offered in analysis mode.  As per value assignments in 
../include/op_panels.h, adjust value here to "skip" that non-existent button
*/
  if (value>=OP_TOGGLE_MODE)
   value++;
 }
 hilite_op[quadrant]=value;
}

/************************  LMRK_OPS_PROC()  *******************************/

void
lmrk_ops_proc(item,value,event)
Panel_item item;
int value;
Event *event;
{
int quadrant;
int current_value;
static int last_value[NUM_DISPLAY_QUADRANTS];

 quadrant=xv_get(item,PANEL_CLIENT_DATA);
 current_value=value ^ last_value[quadrant];
 switch (current_value) {
  case LMRK_ADD:
   show_oppanel(OP_ETC_ADD_LMRK,quadrant); 
   xv_set(lmrk_ops[quadrant],PANEL_VALUE,last_value[quadrant],NULL);
   break;
  case LMRK_SHOW:
   toggle_show_hidden(quadrant);
   break;
 }
 last_value[quadrant]=xv_get(item,PANEL_VALUE);
}

/************************  GEOM_OPS_PROC()  *******************************/

void
geom_ops_proc(item,value,event)
Panel_item item;
int value;
Event *event;
{
int quadrant;

 quadrant=xv_get(item,PANEL_CLIENT_DATA);
 set_geom(quadrant,value);
 SetDisplayFont(quadrant);
}

/************************  TRK_TYPE_BUTTON_PROC()  ***************************/

void
trk_type_item_proc(item,event)
Panel_item item;
Event *event;
{
int value/***,quadrant***/;

/***
 quadrant=xv_get(item,PANEL_CLIENT_DATA);
***/
 value=xv_get(item,PANEL_VALUE);
 if (value==TRK_POSITION)
  printf("position track plot type\n");
 else
  printf("distance track plot type\n");
}

/************************  LMRK_BUTTON_PROC()  *******************************/

void
lmrk_button_proc(item,event)
Panel_item item;
Event *event;
{
int quadrant;

 quadrant=xv_get(item,PANEL_CLIENT_DATA);
 show_oppanel(OP_ETC_LMRKS,quadrant);
 xv_set(lmrk_button[quadrant],PANEL_VALUE,0,NULL);
}

/****************************************************************************/

GetChgVectorYComp()
{
 return ChgVectorYComp;
}

/****************************************************************************/

GetChgVectorXComp()
{
 return ChgVectorXComp;
}

/****************************************************************************/

SetChgVectorXComp(set)
int set;
{
 ChgVectorXComp=set;
}

/****************************************************************************/

SetChgVectorYComp(set)
int set;
{
 ChgVectorYComp=set;
}

/***************************  Y_COMP_PROC()  ********************************/

void
y_comp_proc(quadrant,event)
int quadrant;
Event *event;
{
 RememberCursorPosn(event->ie_win,event->ie_locx,event->ie_locy);
 SetChgVectorYComp(TRUE);
 SetHilitedVarName((char *)get_y_component(quadrant));
 ShowVarMenu(quadrant,event);
}

/***************************  X_COMP_PROC()  ********************************/

void
x_comp_proc(quadrant,event)
int quadrant;
Event *event;
{
 RememberCursorPosn(event->ie_win,event->ie_locx,event->ie_locy);
 SetChgVectorXComp(TRUE);
 SetHilitedVarName((char *)get_x_component(quadrant));
 ShowVarMenu(quadrant,event);
}

/************************  VECTOR_BUTTON_PROC()  *****************************/

void
vector_button_proc(item,event)
Panel_item item;
Event *event;
{
int quadrant;

 quadrant=xv_get(item,PANEL_CLIENT_DATA);
 show_oppanel(OP_ETC_VECTOR,quadrant);
 xv_set(vector_button[quadrant],PANEL_VALUE,0,NULL);
}

/************************  SCALE_BUTTON_PROC()  *******************************/

void
scale_button_proc(item,event)
Panel_item item;
Event *event;
{
int quadrant;

 quadrant=xv_get(item,PANEL_CLIENT_DATA);
 show_oppanel(OP_ETC_SCALES,quadrant);
 xv_set(scale_button[quadrant],PANEL_VALUE,0,NULL);
}

/************************  SCALE_TYPE_BUTTON_PROC()  **************************/

void
scale_type_button_proc(item,value,event)
Panel_item item;
Event *event;
{
int quadrant;

 quadrant=xv_get(item,PANEL_CLIENT_DATA);
 if (value==get_scale_type(quadrant))
  return;
 set_scale_type(quadrant,value);
 switch (value) {
 case NORMAL_SCALE:
  if (get_same_scale(quadrant))
   set_same_scale(quadrant,FALSE);
  if (get_isolate_traces(quadrant))
   set_isolate_traces(quadrant,FALSE);
  break;
 case SAME_SCALE:
  set_same_scale(quadrant,TRUE);
  break;
 case ISOLATE_SCALE:
  set_isolate_traces(quadrant,TRUE);
  break;
 }
}

/************************  INDEPVAR_BUTTON_PROC()  *****************************/

void
indepvar_button_proc(item,value,event)
Panel_item item;
int value;
Event *event;
{
int quadrant;

 quadrant=xv_get(item,PANEL_CLIENT_DATA);
 if (value==get_indepvar_yaxis(quadrant))
  return;
 set_indepvar_yaxis(quadrant,!get_indepvar_yaxis(quadrant));
 if (refresh_window(INITIAL,quadrant))
  set_force_autoscale(quadrant,TRUE); 
}

/************************  GEOM_BUTTON_PROC()  *******************************/

void
geom_button_proc(item,event)
Panel_item item;
Event *event;
{
int quadrant;

 quadrant=xv_get(item,PANEL_CLIENT_DATA);
 show_oppanel(OP_ETC_GEOM,quadrant);
 xv_set(geom_button[quadrant],PANEL_VALUE,0,NULL);
}

/**************************  PROBE_2D_PROC()  *********************************/

void
probe_2d_proc(item,value,event)
Panel_item item;
int value;
Event *event;
{
int quadrant,total_value,clicked_button,probe;

 quadrant=xv_get(item,PANEL_CLIENT_DATA);
 total_value=xv_get(item,PANEL_VALUE);
 clicked_button=total_value ^ last_pms2d_val[quadrant];
 for (probe=0; probe<num_pms2d_probes(quadrant); probe++) {
  if (clicked_button==probe_val[probe]) {
/*
this probe corresponds to the probe button clicked
*/
   select_2d_probe(quadrant,probe);
   if (pms2d_probe_selected(quadrant,probe)) {
/*
the probe is to be displayed
*/
    xv_set(probe_2d_probenames[quadrant],
     PANEL_VALUE,last_pms2d_val[quadrant]|probe_val[probe],
    NULL);
    last_pms2d_val[quadrant]|=probe_val[probe];
   } else {
/*
the probe is NOT to be displayed
*/
    xv_set(probe_2d_probenames[quadrant],
     PANEL_VALUE,last_pms2d_val[quadrant]& ~(probe_val[probe]),
    NULL);
   }
  }
 }
 last_pms2d_val[quadrant]=xv_get(probe_2d_probenames[quadrant],PANEL_VALUE);
}

/****************************************************************************/

GetIntervalButtonXPosn()
{
 return IntervalButtonXPosn;
}

/****************************************************************************/

SetIntervalButtonXPosn(x)
int x;
{
 IntervalButtonXPosn=x;
}

/****************************************************************************/

/*
panel choice buttons are set up as "On", "Off", so they have values,
respectively, of 0 and 1. Define this here to make code more
understandable.
*/
#define VectorsOn 1
#define VectorsOff 0

/****************************************************************************/

CheckVectorButtonValue(quadrant,trace,show) 
int quadrant,trace,show;
{
 if (trace>=num_of_traces[quadrant])
  SetButtonValue(VectorsOnOff[quadrant][trace],VectorsOff);
 else
  SetButtonValue(VectorsOnOff[quadrant][trace],show?VectorsOn:VectorsOff);
}

/****************************************************************************/

VectorsOnOffProc(item,value,event)
Panel_item item;
int value;
Event *event;
{
int quadrant,trace;

 quadrant=xv_get(item,PANEL_CLIENT_DATA);
 if (!GetBothComponentsSet(quadrant)) {
  SetButtonValue((Panel_item)VectorsOnOff[quadrant],VectorsOff);
  return;
 }
 for (trace=0; trace<MAX_TRACES; trace++)
  if (VectorsOnOff[quadrant][trace]==item)
   break;
/*
set vectors to show and nudge update mode to cause a refresh
*/
 set_show_vector(quadrant,value==VectorsOn,trace,FALSE);
 if (trace<num_of_traces[quadrant])
  update_mode[quadrant]=CHG_VECTOR_COMP;
/*
for some reason, CHOICE items that use PANEL_NONE for PANEL_DISPLAY_LEVEL
cause items to their right to get washed out after this notify proc. Correct
this with a simple re-show here
*/
 ShowTextItem(quadrant);
 RestoreCursorPosn();
}

/****************************************************************************/

ConstructItemChoices(item) 
Panel_item item;
{
/***
 int trace,quadrant;
 char string[NAMLEN*2+5];

 quadrant=xv_get(item,PANEL_CLIENT_DATA);
 switch (item) {
  case VectorsOnOff[quadrant]:
   ClearChoiceStrings(item);
   for (trace=0; trace<num_of_traces[quadrant]; trace++) {
    (void)sprintf(string,"%s vs. %s",
     GetTraceName(quadrant,trace),GetTraceName(quadrant,INDEPVAR));
    SetChoiceString(item,trace,string);
   }
   break;
  default:
   break;
 }
***/
}

/**************************  COLORS_PROC()  *********************************/

void
colors_proc(item,value,event)
Panel_item item;
int value;
Event *event;
{
int quadrant,trace;

 quadrant=xv_get(item,PANEL_CLIENT_DATA);
/*
for some reason, CHOICE items that use PANEL_NONE for PANEL_DISPLAY_LEVEL
cause items to their right to get washed out after this notify proc. Correct
this with a simple re-show here
*/
 for (trace=0; trace<MAX_TRACES; trace++)
  ShowPanelItem(VectorsOnOff[quadrant][trace]);
 ShowTextItem(quadrant);
 if (value==get_vector_color(quadrant)) {
  RestoreCursorPosn();
  return;
 }
 set_vector_color(quadrant,value);
 draw_bkgnd(quadrant);
 update_mode[quadrant]=CHG_VECTOR_SCALE;
 need_rehilite[quadrant]=TRUE;
 RestoreCursorPosn();
}

/**************************  COMPS_PROC()  *********************************/

void
comps_proc(item,value,event)
Panel_item item;
int value;
Event *event;
{
int quadrant,trace;

 quadrant=xv_get(item,PANEL_CLIENT_DATA);
/*
for some reason, CHOICE items that use PANEL_NONE for PANEL_DISPLAY_LEVEL
cause items to their right to get washed out after this notify proc. Correct
this with a simple re-show here
*/
 for (trace=0; trace<MAX_TRACES; trace++)
  ShowPanelItem(VectorsOnOff[quadrant][trace]);
 ShowPanelItem(vector_colors[quadrant]);
 ShowTextItem(quadrant);
 if (value==X_COMP)
  x_comp_proc(quadrant,event);
 else
  y_comp_proc(quadrant,event);
}

/**************************  PRINTER_PROC()  *********************************/

void
printer_proc(item,value)
Panel_item item;
int value;
{
int quadrant;

 quadrant=xv_get(item,PANEL_CLIENT_DATA);
 if (GetCurrentImageMode()==COLORPRINT && 
     GetCurrentImagePrinterMode()!=COLORPRINT) {
/*
only color mode printers can handle color print requests; other matches are OK, 
i.e.  bw and grey requests can be handled by any other mode of printer
*/
  show_op_msg(quadrant,PRINT_MISMATCH_MSG);
  return;
 }

 if (value==BWPRINT)
  switch (window_type[quadrant]) {
   case DISPTYPE_LISTS:
    report(quadrant,"list print");
    take_snapshot[quadrant]=TRUE;
/*    toggle_print_lists(quadrant); */
    break;
   case WTYPE_CALIBDISP:
    report(quadrant,"print calibration entries");
    print_entry();
    break;
   case DISPTYPE_ETC:
    fprintf(stderr,"Print B/W Video\n");
   default:
    show_op_msg(quadrant,"    ");
    report(quadrant,"snapshot");
    take_snapshot[quadrant]=TRUE;
    break;
   }

 else

/* color or greyscale print
*/
  switch (window_type[quadrant]) {
   case DISPTYPE_LISTS:
    break;
   case WTYPE_CALIBDISP:
    take_color_snapshot[PLOTS]=TRUE;
    report(PLOTS,"color snapshot");
    break;
   case DISPTYPE_ETC:
    fprintf(stderr,"Print Color/GreyScale Video\n");
   default:
    show_op_msg(quadrant,"    ");
    take_color_snapshot[quadrant]=TRUE;
    report(quadrant,"color snapshot");
    break;
   }
}

/***************************  GENOP_PROC()  ********************************/

void
genop_proc(item,total_value,event)
Panel_item item;
int total_value;
Event *event;
{
/* 'EXCLUSIVE' button:
   this button is to stay lit, and it overrides any other buttons that are
   similarly 'nested menu' types.  Set flag indicating need for restore of
   any status buttons hiliting since we just cleared them here.
*/
#define SET_EXCLUSIVE  total_value=current_value; \
     		       set_genop_value(quadrant,total_value); \
     		       need_status=TRUE;
/* 'BLIP' button:
   this button is NOT to stay lit, since it simply executes and is done; reset
   total_value to the last one seen and set lit buttons to that value.  There
   is no need to set flag for restoring status buttons, since they were 
   preserved in the last total just used to set the hilited value.
*/
#define BLIP_OP set_genop_value(quadrant,SaveGenopValue[quadrant]);

int quadrant;
int need_status,current_value;

 quadrant=xv_get(item,PANEL_CLIENT_DATA);
 need_status=FALSE;

/* isolate the choice just selected
*/
/***
printf("enter genop proc: total val: %d, last total val: %d\n",
total_value,SaveGenopValue[quadrant]);
***/
 current_value=total_value ^ SaveGenopValue[quadrant];
/***
printf("current val: %d\n",current_value);
***/

/* execute indicated operation. NOTE that, although the code here uses 
   #defines that seem to indicate a given operation, the CHOICE_STRINGS for
   each window_type may vary, and often the action taken is a function of
   that window_type.
*/
 switch (current_value) {
  case OP_CHG_DISP:
   SET_EXCLUSIVE
   if (GetCurrentFolderDeleted()) {
    save_genop_value(quadrant,current_value);
    show_op_msg(quadrant,FOLDERDELETED);
   } else {
    show_oppanel(OP_CHG_DISP,quadrant);
   }
   break;
  case OP_OP_MSG:
   show_oppanel(OP_OP_MSG,quadrant);
   break;
  case OP_PRINT:
   switch (window_type[quadrant]) {
    case DISPTYPE_LISTS:
     if (get_lstat_port()==ERROR) {
/*
since show_op_msg() recurses into this proc, do some juggling on local values
so exclusive or's work right to land in the OP_OP_MSG switch.  Calls to 
show_op_msg() outside this proc don't need to do this since values are where
they should be to begin with... 
*/
      SET_EXCLUSIVE
      save_genop_value(quadrant,current_value);
      show_op_msg(quadrant,NO_PRINTER_MSG);
     } else {
      printer_proc(item,BWPRINT);
     }
     break;
    case DISPTYPE_FIXEDFORMAT:
     SET_EXCLUSIVE
     if (get_bw_image_port()==ERROR) {
      save_genop_value(quadrant,current_value);
      show_op_msg(quadrant,NO_PRINTER_MSG);
     } else {
      printer_proc(item,BWPRINT);
     }
     break;
    default:
     SET_EXCLUSIVE
     if (!GetNumImagePrinters()) {
      save_genop_value(quadrant,current_value);
      show_op_msg(quadrant,NO_PRINTER_MSG);
     } else {
      printer_proc(item,GetCurrentImageMode());
     }
     break;
    }
   break;
  case OP_SAVE:
   switch (window_type[quadrant]) {
    case WTYPE_CALIBDISP:
     break;
    default:
     SET_EXCLUSIVE
     if (GetCurrentFolderDeleted()) {
      save_genop_value(quadrant,current_value);
      show_op_msg(quadrant,FOLDERDELETED);
     } else if (!GetFolderEditsEnabled()) {
      save_genop_value(quadrant,current_value);
      show_op_msg(quadrant,EDITSDISABLED);
     } else if (num_of_setups() == get_maxsetups()) {
      save_genop_value(quadrant,current_value);
      show_op_msg(quadrant,NO_MORE_SETUPS);
     } else {
      show_oppanel(OP_SAVE,quadrant);
     }
     break;
    }
   break;
  case OP_FREEZE:
   switch (window_type[quadrant]) {
    case WTYPE_CALIBDISP:
     break;
    case DISPTYPE_ETC:
     (void)fprintf(stderr,"Freeze video\n");
#ifdef PARALLAX
     RequestAlertNow("Sorry...the Freeze operation on video has been experiencing some problems, so for the time being has been disabled...");
/***
     SetVideoLive(GetQuadrantVideoSource(quadrant),!GetVideoLive(GetQuadrantVideoSource(quadrant)));
     freeze(quadrant);
***/
#endif
     break;
    default:
     report(quadrant,"freeze");
     freeze(quadrant);

/* take the easy way out: to accommodate analysis mode,
   simply refresh history when a window gets unfrozen
*/
     if (!frozen[quadrant] && (GetInputMode()==RAWFILE_NETCDF ||
          post_processing_mode())) {
      draw_bkgnd(quadrant);
      update_mode[quadrant]=INITIAL;
      need_rehilite[quadrant]=TRUE;
      set_force_autoscale(quadrant,TRUE); 
     }
     break;
   }
   break;
  case OP_HISTORY:
   switch (window_type[quadrant]) {
    case DISPTYPE_LISTS:
     BLIP_OP
     next_list_page(quadrant);
     break;
    case DISPTYPE_FIXEDFORMAT:
     SET_EXCLUSIVE
     show_oppanel(OP_ETC,quadrant);
     break;
    case DISPTYPE_PMS1D:
     SET_EXCLUSIVE
     show_oppanel(OP_PROBES,quadrant);
     break;
    case DISPTYPE_ETC:
     SET_EXCLUSIVE
fprintf(stderr,"Chg video channel\n");
     break;
    case DISPTYPE_PMS2D:
     SET_EXCLUSIVE
     if (!num_pms2d_probes(quadrant)) {
      save_genop_value(quadrant,current_value);
      show_op_msg(quadrant,NO_2D_PROBES);
     } else {
      show_oppanel(OP_PROBES,quadrant);
     }
     break;
    default:
     SET_EXCLUSIVE
     show_oppanel(OP_HISTORY,quadrant);
     break;
   }
   break;
  case OP_HILITED_VAR:
   switch (window_type[quadrant]) {
    case DISPTYPE_PMS1D:
     SET_EXCLUSIVE
     show_oppanel(OP_PMS1D_DATATYPE,quadrant);
     break;
    case DISPTYPE_ETC:
     SET_EXCLUSIVE
fprintf(stderr,"Video etc operations\n");
     break;
    case DISPTYPE_PMS2D:
     BLIP_OP
     select_2d_rate(quadrant);
     break;
    case WTYPE_SKEWT:
     SET_EXCLUSIVE
     show_oppanel(OP_SKEW_RANGES,quadrant);
     break;
    case DISPTYPE_LISTS:
     SET_EXCLUSIVE
     show_oppanel(OP_LIST_RATE,quadrant);
     break;
    case DISPTYPE_XYPLOT:
     SET_EXCLUSIVE
     show_oppanel(OP_HILITED_VAR,quadrant);
     break;
    default:
     break;
   }
   break;
  case OP_AUTOSCL:
   switch (window_type[quadrant]) {
    case DISPTYPE_XYPLOT:
     report(quadrant,"toggle autoscale");
     toggle_autoscale(quadrant);
     break;
    case WTYPE_SKEWT:
    case DISPTYPE_LISTS:
     SET_EXCLUSIVE
     show_oppanel(OP_ETC,quadrant);
     break;
    case DISPTYPE_PMS2D:
     SET_EXCLUSIVE
     show_oppanel(OP_ETC,quadrant);
     break;
    case DISPTYPE_PMS1D:
     SET_EXCLUSIVE
     show_oppanel(OP_PMS1D_SCALES,quadrant);
     break;
    default:
     BLIP_OP
     break;
   }
   break;
  case OP_ETC:
   SET_EXCLUSIVE
   show_oppanel(OP_ETC,quadrant);
   break;
 }
/* 
restore status buttons hiliting if so indicated
*/
 set_genop_hiliting(need_status,quadrant,total_value);
}

/**************************  CHGDISP_PROC()  *********************************/

void
chgdisp_proc(item,event)
Panel_item item;
Event *event;
{
int quadrant;

 quadrant=xv_get(item,PANEL_CLIENT_DATA);
 RememberCursorPosn(event->ie_win,event->ie_locx,event->ie_locy);
}

/**************************  SET_1D_PROBE_BUTTONS()  ***********************/

set_1d_probe_buttons(quadrant)
int quadrant;
{
int probe;

 if (!probe_1d_ops[quadrant])
  return;

 for (probe=0; probe<NumberPMS1Dprobes(); probe++)
  xv_set(probe_1d_ops[quadrant],
	PANEL_CHOICE_STRING,probe,(char *)GetPMS1DprobeName(probe),
	NULL);
 xv_set(probe_1d_ops[quadrant],
    PANEL_VALUE,0,
    NULL);
 for (probe=0; probe<NumberPMS1Dprobes(); probe++) {
  if (isPMS1DprobeSelected(quadrant,probe)) {  
   xv_set(probe_1d_ops[quadrant],
    PANEL_VALUE,xv_get(probe_1d_ops[quadrant],PANEL_VALUE)|probe_val[probe],
    NULL);
  }
 }

 last_pms1d_val[quadrant]=xv_get(probe_1d_ops[quadrant],PANEL_VALUE,NULL);
}

/**************************  SET_2D_PROBE_BUTTONS()  *************************/

set_2d_probe_buttons(quadrant)
int quadrant;
{
int probe;

 if (!probe_2d_probenames[quadrant])
  return;

 xv_set(probe_2d_probenames[quadrant],
/*
don't use NULL string to avoid abw errors in Purify when this object has 
a PANEL_VALUE set a little bit later on. Instead, set to blanks and use
y-position to effectively hide it if there are no probes.
*/
	 PANEL_CHOICE_STRINGS," ",NULL,
	NULL);

 if (!num_pms2d_probes(quadrant)) 
/*
hide the buttons since there are no choices anyway
*/
  xv_set(probe_2d_probenames[quadrant],
   XV_Y,VAROP_ROWY+GetOpPanelHeight(),
  NULL);
 else {
  xv_set(probe_2d_probenames[quadrant],
   XV_Y,VAROP_ROWY,
  NULL);
  for (probe=0; probe<num_pms2d_probes(quadrant); probe++) {
   xv_set(probe_2d_probenames[quadrant],
  	   PANEL_CHOICE_STRING,probe,(char *)get_2d_probename(quadrant,probe),
  	  NULL);
  }
 }
 set_2d_probe_button_vals(quadrant);
}

/**********************  SET_2D_PROBE_BUTTON_VALS()  ************************/

set_2d_probe_button_vals(quadrant)
int quadrant;
{
int probe;

 xv_set(probe_2d_probenames[quadrant],
    PANEL_VALUE,0,
    NULL);
 for (probe=0; probe<num_pms2d_probes(quadrant); probe++) {
  if (pms2d_probe_selected(quadrant,probe)) {  
   xv_set(probe_2d_probenames[quadrant],
    PANEL_VALUE,xv_get(probe_2d_probenames[quadrant],PANEL_VALUE)|probe_val[probe],
    NULL);
  }
 }
 last_pms2d_val[quadrant]=xv_get(probe_2d_probenames[quadrant],PANEL_VALUE,NULL);
}

/*********************  SET_INDEPVAR_BUTTON()  ***************************/

set_indepvar_button(quadrant,value)
int quadrant,value;
{
 if (!indepvar_button[quadrant])
  return;
 xv_set(indepvar_button[quadrant],PANEL_VALUE,value,NULL);
}

/*********************  SET_SCALE_TYPE_BUTTON()  ***************************/

set_scale_type_button(quadrant)
int quadrant;
{
 if (!scale_type_button[quadrant])
  return;
 xv_set(scale_type_button[quadrant],PANEL_VALUE,get_scale_type(quadrant),NULL);
}

/*********************SET_VECTOR_COLOR_BUTTON()  *****************************/

set_vector_color_button(quadrant,value)
int quadrant,value;
{
 if (vector_colors[quadrant])
  xv_set(vector_colors[quadrant],PANEL_VALUE,value,NULL);
}

/************************  SET_GENOP_BUTTONS()  ******************************/

set_genop_buttons(quadrant)
int quadrant;
{

 if (!genop_choices[quadrant])
  return;

 set_geom_buttons(quadrant);
 switch (window_type[quadrant]) {
  case DISPTYPE_XYPLOT:
   xv_set(genop_choices[quadrant],
 	PANEL_CHOICE_STRINGS, GENOPS_XYPLOT, NULL,
	NULL);
   set_hilite_buttons(quadrant);
   if (plot_type[quadrant]==TRACK_PLOT) 
    set_vector_items(quadrant);
   break;
  case DISPTYPE_PMS1D:
   xv_set(genop_choices[quadrant],
 	PANEL_CHOICE_STRINGS, GENOPS_PMS1D, NULL,
	NULL);
   set_1d_probe_buttons(quadrant);
   set_1d_probe_disp_parms(quadrant);
   break;
  case DISPTYPE_PMS2D:
   xv_set(genop_choices[quadrant],
 	PANEL_CHOICE_STRINGS, GENOPS_PMS2D, NULL,
	NULL);
   set_2d_probe_buttons(quadrant);
   break;
  case DISPTYPE_LISTS:
   xv_set(genop_choices[quadrant],
 	PANEL_CHOICE_STRINGS, GENOPS_LISTS, NULL,
	NULL);
   set_hilite_buttons(quadrant);
   break;
  case DISPTYPE_FIXEDFORMAT:
   xv_set(genop_choices[quadrant],
 	PANEL_CHOICE_STRINGS, GENOPS_FIXED, NULL,
	NULL);
   break;
  case DISPTYPE_ETC:
   xv_set(genop_choices[quadrant],
 	PANEL_CHOICE_STRINGS, GENOPS_ETC, NULL,
	NULL);
   break;
  case WTYPE_SKEWT:
   xv_set(genop_choices[quadrant],
 	PANEL_CHOICE_STRINGS, GENOPS_SKEWT, NULL,
	NULL);
   break;
  default:

/* likely, no plotspex entry for this quadrant; setup as new setup window
   defaulted to xy-plot
*/
   window_type[quadrant]=DISPTYPE_XYPLOT;
   plot_type[quadrant]=TIME_SERIES;
   set_genop_buttons(quadrant);
   break;
 }
 set_genop_hiliting(TRUE,quadrant,get_genop_value(quadrant));
}

/************************  SET_HILITE_BUTTONS()  ******************************/

set_hilite_buttons(quadrant)
int quadrant;
{
 if (!hilited_ops[quadrant])
  return;

 switch (window_type[quadrant]) {
  case DISPTYPE_XYPLOT:
   hilite_op[quadrant]=OP_TOGGLE_FIX;
   if (plot_type[quadrant]==TRACK_PLOT) {
    xv_set(hilited_ops[quadrant],
	PANEL_VALUE,hilite_op[quadrant],
	NULL);
    if (post_processing_mode())
      xv_set(hilited_ops[quadrant],
	PANEL_CHOICE_STRINGS,
         ANALYSIS_HILITE_OPS,NULL,
	NULL);
     else
      xv_set(hilited_ops[quadrant],
	PANEL_CHOICE_STRINGS,
         HILITE_OPS,NULL,
	NULL);
   } else {
    xv_set(hilited_ops[quadrant],
	PANEL_VALUE,hilite_op[quadrant],
	NULL);
    if (post_processing_mode())
      xv_set(hilited_ops[quadrant],
	PANEL_CHOICE_STRINGS,
         ANALYSIS_HILITE_OPS,NULL,
	NULL);
     else
      xv_set(hilited_ops[quadrant],
	PANEL_CHOICE_STRINGS,
         HILITE_OPS,NULL,
	NULL);
   }
   break;
  case DISPTYPE_LISTS:
   hilite_op[quadrant]=OP_TOGGLE_MODE;
   break;
  case WTYPE_SKEWT:
  case DISPTYPE_FIXEDFORMAT:
  case DISPTYPE_PMS2D:
  case DISPTYPE_PMS1D:
  case DISPTYPE_ETC:
   break;
 }
}

/***************************  SET_GEOM_BUTTONS()  ****************************/

set_geom_buttons(quadrant)
int quadrant;
{

 if (!geom_ops[quadrant])
  return;

 xv_set(geom_ops[quadrant],PANEL_VALUE,get_geom(quadrant),NULL);

}

/**************************  SET_1D_PROBE_DISP_PARMS()  ***********************/

set_1d_probe_disp_parms(quadrant)
int quadrant;
{
int parm;

 last_cnts_value[quadrant]=isQuadrantCounts(quadrant)?0:1;
 last_liters_value[quadrant]= isQuadrantYliters(quadrant)?0:1;
 last_ytype_value[quadrant]= isQuadrantYlinear(quadrant)?0:1;
 last_xtype_value[quadrant]= isQuadrantXlinear(quadrant)?0:1;

 for (parm=0; parm<NUM_1D_DATATYPE_PARMS; parm++) {
  if (!probe_1d_data[parm][quadrant])
   return;
  switch (parm) {
   case PMS1D_DISP_CNTS:
    xv_set(probe_1d_data[PMS1D_DISP_CNTS][quadrant],
     PANEL_LABEL_STRING,DATATYPE_LABEL,
     PANEL_CHOICE_STRINGS,DATATYPE_STRING,NULL,
     PANEL_VALUE,last_cnts_value[quadrant],
     XV_X,0,
     NULL);
   case PMS1D_DISP_LITERS:
    xv_set(probe_1d_data[PMS1D_DISP_LITERS][quadrant],
     PANEL_LABEL_STRING,UNITS_LABEL,
     PANEL_CHOICE_STRINGS,UNITS_STRINGS,NULL,
     PANEL_VALUE,last_liters_value[quadrant],
     XV_X,
	 xv_get(probe_1d_data[PMS1D_DISP_CNTS][quadrant],XV_X)+
	 xv_get(probe_1d_data[PMS1D_DISP_CNTS][quadrant],XV_WIDTH)+
         OP_ITEMS_XGAP,
     NULL);
    break;
  }
 }
 for (parm=0; parm<NUM_1D_SCALES_PARMS; parm++) {
  if (!probe_1d_scales[parm][quadrant])
   return;
  switch (parm) {
   case PMS1D_DISP_XTYPE:
    xv_set(probe_1d_scales[PMS1D_DISP_XTYPE][quadrant],
     PANEL_LABEL_STRING,XAXIS_LABEL,
     PANEL_CHOICE_STRINGS,XAXIS_STRINGS,NULL,
     PANEL_VALUE,last_xtype_value[quadrant],
     XV_X,0,
     NULL);
    break;
   case PMS1D_DISP_YTYPE:
    xv_set(probe_1d_scales[PMS1D_DISP_YTYPE][quadrant],
     PANEL_LABEL_STRING,YAXIS_LABEL,
     PANEL_CHOICE_STRINGS,YAXIS_STRINGS,NULL,
     PANEL_VALUE,last_ytype_value[quadrant],
     XV_X,
	 xv_get(probe_1d_scales[PMS1D_DISP_XTYPE][quadrant],XV_X)+
	 xv_get(probe_1d_scales[PMS1D_DISP_XTYPE][quadrant],XV_WIDTH)+
         OP_ITEMS_XGAP,
     NULL);
    break;
  }
 }
}

/************************* SET()/GET() PROCEDURES *****************************/

/**************************  SET_ISOLATE_TRACES()  ****************************/

set_isolate_traces(quadrant,value)
int quadrant,value;
{
 if (plot_type[quadrant]==SKEW_T ) {
  IsolateTraces[quadrant]=FALSE;
  return;
 }
 IsolateTraces[quadrant]=value;
 if (value && get_same_scale(quadrant))
/*
same scale and isolation are to be mutually exclusive operations
*/
  set_same_scale(quadrant,FALSE);

 set_ScaleType(quadrant);
 set_scale_type_button(quadrant);
 (void)refresh_window(INITIAL,quadrant);
}

/**************************  GET_ISOLATE_TRACES()  ****************************/

get_isolate_traces(quadrant)
int quadrant;
{
 return IsolateTraces[quadrant];
}

/**************************  GET_LAST_PANEL_SHOWN()  ****************************/

get_last_panel_shown(quadrant)
int quadrant;
{
 return last_panel[quadrant];
}

/**************************  SET_SCALETYPE()  ********************************/

set_ScaleType(quadrant)
{
 if (get_isolate_traces(quadrant))
  set_scale_type(quadrant,ISOLATE_SCALE);
 else if (get_same_scale(quadrant))
  set_scale_type(quadrant,SAME_SCALE);
 else 
  set_scale_type(quadrant,NORMAL_SCALE);
}

/**************************  SET_SCALE_TYPE()  ********************************/

set_scale_type(quadrant,value)
{
 ScaleType[quadrant]=value;
}

/**************************  GET_SCALE_TYPE()  ********************************/

get_scale_type(quadrant)
{
 return ScaleType[quadrant];
}

/**************************  GET_HILITED_OP()  ******************************/

get_hilited_op(quadrant)
int quadrant;
{
 return hilite_op[quadrant];
}

/*************************  GET_GENOP_VALUE()  ********************************/

get_genop_value(quadrant)
int quadrant;
{
 if (genop_choices[quadrant])
  return xv_get(genop_choices[quadrant],
 	PANEL_VALUE,
 	NULL);
 return 0;
}

/*************************  SET_GENOP_VALUE()  ********************************/

set_genop_value(quadrant,value)
int quadrant,value;
{
 if (genop_choices[quadrant])
  xv_set(genop_choices[quadrant],
 	PANEL_VALUE,value,
 	NULL);
 save_genop_value(quadrant,value);
}

/************************  SET_VECTOR_ITEMS()  ******************************/

set_vector_items(quadrant)
int quadrant;
{
  set_comp_label(quadrant,X_COMP);
  set_comp_label(quadrant,Y_COMP);
}


/*****************************************************************************/

RefreshOpPanel(quadrant)
int quadrant;
{
  xv_set(op_panel[quadrant],
   XV_SHOW,FALSE,
   NULL);
  xv_set(op_panel[quadrant],
   XV_SHOW,TRUE,
   NULL);
}

/************************************************************************/

SetOpPanelGeom(window)
int window;
{
 if (!op_panel[window])
  return;
 xv_set(op_panel[window],
  XV_X, window_x[window], XV_Y, window_y[window]-GetOpPanelHeight(),
  XV_WIDTH, window_width[window], 
  XV_HEIGHT, GetOpPanelHeight(),
  NULL);
}

/************************************************************************/

GetOpPanelRowsHeight()
{
 if (!op_panel[0])
  return 0;
 return 2*xv_get(genop_choices[0],XV_HEIGHT)+
  GetFontHeight(GetOpPanelFont());
}

/************************************************************************/

GetOpPanelHeight()
{
 return OP_PANEL_HEIGHT;
}

/***************************  SET_OP_GEOM()  ****************************/

set_op_geom(window)
int window;
{
 if (!op_panel[window])
  return;
 SetOpPanelGeom(window);
 if (window<NUM_DISPLAY_QUADRANTS)
  xv_set(op_panel[window],XV_SHOW,TRUE,NULL);
 set_geom_buttons(window);
}

/**************************  SET_COMP_LABEL()  *******************************/

set_comp_label(quadrant,component)
int quadrant,component;
{
  char	comp_string[NAMLEN*3], *p;
printf("In set_comp_label\n");
  if (!vector_comps[quadrant])
    return;

  /* feedback on components' names: "none" if given component isn't in dataset
   */
  if (component == X_COMP) {
    p = get_x_component(quadrant);
    sprintf(comp_string, "%s (%s)", COMP_X_STRING,
	get_mode_by_name(p) == ERROR ? "none" : p);
  } else {
    p = get_y_component(quadrant);
    sprintf(comp_string, "%s (%s)", COMP_Y_STRING,
	get_mode_by_name(p) == ERROR ? "none" : p);
  }
/*
printf("  going in [%s]....\n", comp_string);fflush(stdout);
  xv_set(vector_comps[quadrant],
	PANEL_CHOICE_STRING, component == X_COMP ? 0 : 1, comp_string, NULL);
*/
printf("we're out\n");
}

/************************  SET_GENOP_HILITING()  *****************************/

set_genop_hiliting(status_needed,quadrant,value)
int status_needed,quadrant,value;

/* if status_needed is TRUE, assign the genop buttons to value OR'd with
   current status variables, as appropriate to window type.  Generally
   called when a SET_EXCLUSIVE gen-op is invoked.
*/
{
 if (status_needed)
  set_genop_value(quadrant,
    ((frozen[quadrant]?OP_FREEZE:0) | 
    (window_type[quadrant]==DISPTYPE_LISTS && list_printing[quadrant]?OP_PRINT:0) | 
    (window_type[quadrant]==DISPTYPE_XYPLOT && autoscale[quadrant]?OP_AUTOSCL:0) | 
    value));
 save_genop_value(quadrant,get_genop_value(quadrant));
}

/************************* MISCELLANEOUS PROCEDURES *****************************/

/**************************  EXEC_HILITED_OP()  ******************************/

exec_hilite_op(int quadrant, Event *event)
{

 switch (window_type[quadrant]) {
  case DISPTYPE_FIXEDFORMAT:
  case WTYPE_SKEWT:
   break;
  case DISPTYPE_LISTS:
   if (!post_processing_mode())
    toggle_mode(quadrant);
   break;
  case WTYPE_CALIBDISP:
   delete_hilited_entry();
   need_rehilite[STATS]=TRUE;
   break;
  case DISPTYPE_XYPLOT:
   if (event_in_chg_time_scale_area(event_y(event))) {
    reset_plot_attr(quadrant);
   } else {
    switch (hilite_op[quadrant]) {
     case OP_TOGGLE_FIX:
      toggle_fix_float(quadrant);
      break;
     case OP_TOGGLE_MODE:
      toggle_mode(quadrant);
      break;
     case OP_DEFRNG:
      reset_plot_attr(quadrant);
      break;
    }
   }
 }
}

/************************  CLEAR_GENOP_STATUS()  *****************************/

clear_genop_status(quadrant)
int quadrant;

/* reset all status buttons as per default for window type and, in some cases 
   (e.g., autoscale) the value of given status parameter.  Generally called
   when given window's plotspec is changed.
*/
{
 if (!genop_choices[quadrant])
  return;
 set_genop_value(quadrant,

/* assuming freeze and list printing are off until turned on manually 
*/
    ~OP_FREEZE & ~OP_PRINT & get_genop_value(quadrant));
 if (window_type[quadrant]==DISPTYPE_XYPLOT)

/* autoscale may be on or off 
*/
  set_genop_value(quadrant,
    autoscale[quadrant]?(get_genop_value(quadrant) | OP_AUTOSCL):
                        (get_genop_value(quadrant) & ~OP_AUTOSCL));
 else

/* autoscale button is possibly assigned to different function; clear it
*/
  set_genop_value(quadrant,(get_genop_value(quadrant) & ~OP_AUTOSCL));

 save_genop_value(quadrant,get_genop_value(quadrant));

}

/********************** SAVE_GENOP_VALUE() ************************************/

save_genop_value(quadrant,value)
int quadrant,value;
{
 SaveGenopValue[quadrant]=value;
}

/*********************** HIDE/SHOW PROCEDURES *********************************/

/************************  SHOW_LMARK_PANEL()  ********************************/

show_lmrk_panel(quadrant,show)
int quadrant,show;
{
 if (!lmrk_ops[quadrant])
  return;
 xv_set(lmrk_ops[quadrant],XV_SHOW,show,NULL);
}

/************************  SHOW_GEOM_PANEL()  *********************************/

show_geom_panel(quadrant,show)
int quadrant,show;
{
 if (!geom_ops[quadrant])
  return;
 xv_set(geom_ops[quadrant],XV_SHOW,show,NULL);
}

/************************  SHOW_SCALES_PANEL()  *******************************/

show_scales_panel(quadrant,show)
int quadrant,show;
{
 if (scale_type_button[quadrant]) {
  set_scale_type_button(quadrant);
  xv_set(scale_type_button[quadrant],XV_SHOW,show,NULL);
 }
 if (plot_type[quadrant]!=TIME_SERIES) {
  if (scale_type_button[quadrant] && indepvar_button[quadrant])
   xv_set(indepvar_button[quadrant],XV_SHOW,show,
    XV_X, 
     xv_get(scale_type_button[quadrant],XV_X)+
     xv_get(scale_type_button[quadrant],XV_WIDTH)+
    OP_ITEMS_XGAP,
    NULL);
 }
}

/**************************  SHOW_VECTOR_OPS()  *********************************/

show_vector_ops(quadrant,show)
int quadrant,show;
{
static char VectorInterval[VALUELEN*2];
int trace;
 
 (void)sprintf(&VectorInterval[0],"%d",get_vector_interval(quadrant));
 if (show)
  show_text_op(quadrant,INTERVAL,VectorInterval);
 xv_set(vector_comps[quadrant],XV_SHOW,show,NULL);
 xv_set(vector_colors[quadrant],XV_SHOW,show,NULL);
 for (trace=0; trace<MAX_TRACES; trace++)
  xv_set(VectorsOnOff[quadrant][trace],XV_SHOW,show,NULL);
}

/**************************  SHOW_ETC_PANEL()  *********************************/

show_etc_panel(quadrant,show)
int quadrant,show;
{

   xv_set(geom_button[quadrant],XV_SHOW,show,NULL);
   switch (window_type[quadrant]) {
    case DISPTYPE_XYPLOT:
     switch (plot_type[quadrant]) {
      case SOUNDING:
      case XY:
      case TIME_SERIES:
       xv_set(scale_button[quadrant],XV_SHOW,show,
	XV_X, 
	 xv_get(geom_button[quadrant],XV_X)+
	 xv_get(geom_button[quadrant],XV_WIDTH)+
         OP_ITEMS_XGAP,
	NULL);
       break;
      case TRACK_PLOT:
       xv_set(vector_button[quadrant],XV_SHOW,show,NULL);
       xv_set(lmrk_button[quadrant],XV_SHOW,show,NULL);
       xv_set(scale_button[quadrant],XV_SHOW,show,
	XV_X, 
	 xv_get(lmrk_button[quadrant],XV_X)+
	 xv_get(lmrk_button[quadrant],XV_WIDTH)+
         OP_ITEMS_XGAP,
	NULL);
/***
       xv_set(trk_type_item[quadrant],XV_SHOW,show,NULL);
***/
       break;
     }
     break;
    case DISPTYPE_PMS1D:
     break;
    case DISPTYPE_PMS2D:
     break;
    case DISPTYPE_LISTS:
     break;
    case DISPTYPE_FIXEDFORMAT:
     break;
    case DISPTYPE_ETC:
     break;
    case WTYPE_CALIBDISP:
     break;
    case WTYPE_SKEWT:
     break;
   }
}

/**************************  HIDE_OPPANEL()  *********************************/

hide_oppanel(quadrant)
int quadrant;
{
int type;

 switch (current_panel[quadrant]) {
  case OP_SAVE:
   hide_text_op(quadrant);
   break;
  case OP_FREEZE:
   break;
  case OP_CHG_DISP:
   for (type=0; type<get_num_display_types(); type++) {
     xv_set(spex_button[quadrant][type],XV_SHOW,FALSE,NULL);
   }
   break;
  case OP_HISTORY:
   switch (window_type[quadrant]) {
    case DISPTYPE_PMS1D:
     xv_set(probe_1d_ops[quadrant],XV_SHOW,FALSE,NULL);
     break;
    case DISPTYPE_PMS2D:
     xv_set(probe_2d_probenames[quadrant],XV_SHOW,FALSE,NULL);
     break;
    case DISPTYPE_ETC:
     break;
    default:
     xv_set(history_ops[quadrant],XV_SHOW,FALSE,NULL);
   }
   break;
  case OP_HILITED_VAR:
   switch (window_type[quadrant]) {
    case DISPTYPE_ETC:
     break;
    case DISPTYPE_PMS1D:
     xv_set(probe_1d_data[PMS1D_DISP_LITERS][quadrant],XV_SHOW,FALSE,NULL);
     xv_set(probe_1d_data[PMS1D_DISP_CNTS][quadrant],XV_SHOW,FALSE,NULL);
     break;
    case WTYPE_SKEWT:
    case DISPTYPE_LISTS:
     hide_text_op(quadrant);
     break;
    default:
     xv_set(hilited_ops[quadrant],XV_SHOW,FALSE,NULL);
   }
   break;
  case OP_PMS1D_SCALES:
   xv_set(probe_1d_scales[PMS1D_DISP_XTYPE][quadrant],XV_SHOW,FALSE,NULL);
   xv_set(probe_1d_scales[PMS1D_DISP_YTYPE][quadrant],XV_SHOW,FALSE,NULL);
   break;
  case OP_ETC:
   show_etc_panel(quadrant,FALSE);
   break;
  case OP_ETC_VECTOR:
   hide_text_op(quadrant);
   show_vector_ops(quadrant,FALSE);
   break;
  case OP_ETC_SCALES:
   show_scales_panel(quadrant,FALSE);
   break;
  case OP_ETC_GEOM:
   show_geom_panel(quadrant,FALSE);
   break;
  case OP_ETC_LMRKS:
   show_lmrk_panel(quadrant,FALSE);
   break;
  case OP_ETC_ADD_LMRK:
   hide_text_op(quadrant);
   break;
  case OP_OP_MSG:
   xv_set(op_msg[quadrant],XV_SHOW,FALSE,NULL);
   break;
 }
 last_panel[quadrant]=current_panel[quadrant];
}

/********************  DispSubMenusShowing()  *********************************/

DispSubMenusShowing(quadrant)  
int quadrant;
{
 return current_panel[quadrant]==OP_CHG_DISP;
}

/***********************  ActivateOpPanels()  ******************************/

ActivateOpPanels(active)
int active;
{
int quadrant;
 
 for (quadrant=0; quadrant<get_num_displays(); quadrant++)
  xv_set(genop_choices[quadrant],PANEL_INACTIVE,!active,NULL);
}

/**************************  SHOW_OPPANEL()  *********************************/

show_oppanel(which,quadrant)
int which,quadrant;
{
int type;
char string[VALUELEN*2];

 hide_oppanel(quadrant);

 current_panel[quadrant]=which;
 switch (which) {
  case OP_SAVE:
   show_text_op(quadrant,DESCRIPTION,(char *)get_description(quadrant));
   break;
  case OP_FREEZE:
   break;
  case OP_CHG_DISP:
   for (type=0; type<get_num_display_types(); type++) {
     xv_set(spex_button[quadrant][type],XV_SHOW,TRUE,NULL);
   }
   break;
  case OP_HISTORY:
/* 
same as OP_PROBES
*/
   switch (window_type[quadrant]) {
    case DISPTYPE_ETC:
     break;
    case DISPTYPE_PMS1D:
     xv_set(probe_1d_ops[quadrant],XV_SHOW,TRUE,NULL);
     break;
    case DISPTYPE_PMS2D:
     xv_set(probe_2d_probenames[quadrant],XV_SHOW,TRUE,NULL);
     break;
    default:
     xv_set(history_ops[quadrant],XV_SHOW,TRUE,NULL);
   }
   break;
  case OP_HILITED_VAR:

/* same as OP_PMS1D_DATATYPE, OP_LIST_RATE, and OP_SKEW_RANGES
*/
   if (window_type[quadrant]==DISPTYPE_PMS1D) {
    xv_set(probe_1d_data[PMS1D_DISP_CNTS][quadrant],XV_SHOW,TRUE,NULL);
    if (!isQuadrantCounts(quadrant))
     xv_set(probe_1d_data[PMS1D_DISP_LITERS][quadrant],XV_SHOW,TRUE,NULL);
   } else if (window_type[quadrant]==WTYPE_SKEWT) {
    (void)sprintf(&string[0],"%d %d",
     (int)skew_minpress[quadrant],(int)skew_mintemp[quadrant]);
    show_text_op(quadrant,SKEWT_RANGES,string);
   } else if (window_type[quadrant]==DISPTYPE_LISTS) {
    (void)sprintf(&string[0],"%d",GetListRate(quadrant));
    show_text_op(quadrant,SET_LIST_RATE,string);
    break;
   } else {
    xv_set(hilited_ops[quadrant],XV_SHOW,TRUE,NULL);
   }
   break;
  case OP_PMS1D_SCALES:
   if (!isQuadrantCounts(quadrant)) 

/* disallow changes to log/linear on x-axis if datatype is counts
*/
    xv_set(probe_1d_scales[PMS1D_DISP_XTYPE][quadrant],XV_SHOW,TRUE,NULL);
   xv_set(probe_1d_scales[PMS1D_DISP_YTYPE][quadrant],XV_SHOW,TRUE,NULL);
   break;
  case OP_ETC:
   show_etc_panel(quadrant,TRUE);
   break;
  case OP_ETC_VECTOR:
   show_vector_ops(quadrant,TRUE);
   break;
  case OP_ETC_SCALES:
   show_scales_panel(quadrant,TRUE);
   break;
  case OP_ETC_GEOM:
   show_geom_panel(quadrant,TRUE);
   break;
  case OP_ETC_LMRKS:
   show_lmrk_panel(quadrant,TRUE);
   break;
  case OP_ETC_ADD_LMRK:
   show_text_op(quadrant,ADD_LANDMARK,"");
   break;
  case OP_OP_MSG:
   xv_set(op_msg[quadrant],XV_SHOW,TRUE,NULL);
   break;
 }
}

/**************************  SHOW_OP_MSG()  *********************************/

show_op_msg(quadrant,msg)
int quadrant;
char *msg;
{
char *Msg;

 if (quadrant >= NUM_DISPLAY_QUADRANTS)
/* 
no op-panels for calib plot windows, etc.
*/
  return;
 if (!op_msg[quadrant])
/*
message widget not created yet
*/
  return;
 Msg=(char *)malloc((unsigned int)(strlen(msg)+4));
 (void)sprintf(Msg," %s",msg);
 xv_set(op_msg[quadrant],
	PANEL_LABEL_STRING,Msg,
	NULL);
/***
printf("SHOW OP MSG: call genop proc with %d ^ %d = %d\n",
get_genop_value(quadrant),OP_OP_MSG,get_genop_value(quadrant)^OP_OP_MSG);
***/
 genop_proc(op_msg[quadrant],get_genop_value(quadrant)^OP_OP_MSG,(Event *)0);
 free(Msg);
}

/**************************** MAKE PROCEDURES *******************************/

/***************************  MAKE_OP_PANELS()  ****************************/

make_op_panels()
{
int quadrant,op_quadrant;

 for (quadrant=0; quadrant<get_num_displays(); quadrant++) {
  if (op_panel[quadrant])
   xv_destroy_safe(op_panel[quadrant]);
  op_panel[quadrant]=xv_create(GetBaseFrame(),PANEL,
	 XV_HELP_DATA,"WINDS:OP_PANEL_HELP",
         XV_WIDTH, window_width[quadrant],
	 PANEL_REPAINT_PROC, panel_repaint,
         PANEL_EVENT_PROC, panel_event_proc,
         PANEL_BACKGROUND_PROC, panel_background_proc,
	 PANEL_ACCEPT_KEYSTROKE, TRUE,
         XV_FONT, GetOpPanelFont(),
	 PANEL_ITEM_X_GAP, OP_ITEMS_XGAP,
         NULL);
  set_op_geom(quadrant);
  op_quadrant=quadrant+FIRST_OP_QUADRANT;
  paint_window[op_quadrant]=canvas_paint_window(op_panel[quadrant]);
  xv_set(paint_window[op_quadrant],
	  PANEL_CLIENT_DATA,HELPIMAGE_OP_PANEL_AREA,
#include <event_attr.h>
          WIN_EVENT_PROC, event_proc,
         NULL);
  make_genop_buttons(quadrant);
  make_chg_disp(quadrant);
  AttachPlotspecMenus(quadrant);
  make_message(quadrant);
  make_history(quadrant);
  make_hilited(quadrant);
  make_geom_ops(quadrant);
  make_scale_ops(quadrant);
  make_vector_ops(quadrant);
  make_lmrk_ops(quadrant);
/***
  make_trk_type_item(quadrant);
***/
  make_1d_probe_ops(quadrant);
  make_1d_disp_parms(quadrant);
  make_2d_probe_ops(quadrant);
  make_text_item(quadrant);
 }
}

/*********************  MAKE_GENOP_BUTTONS()  ********************************/

make_genop_buttons(quadrant)
int quadrant;
{

 genop_choices[quadrant]=xv_create(op_panel[quadrant],PANEL_TOGGLE,
	PANEL_VALUE,0,
	PANEL_EVENT_PROC, panel_event_proc,
	PANEL_NOTIFY_PROC,genop_proc,
        PANEL_CLIENT_DATA,quadrant,
	XV_X,1,
	NULL);
 set_genop_buttons(quadrant);
}

/**************************  MAKE_1D_PROBE_OPS()  *****************************/

make_1d_probe_ops(quadrant)
int quadrant;
{
  probe_1d_ops[quadrant]=xv_create(op_panel[quadrant],PANEL_TOGGLE,
        PANEL_CLIENT_DATA,quadrant,
	PANEL_EVENT_PROC, panel_event_proc,
	PANEL_NOTIFY_PROC,probe_1d_proc,
	XV_X,0,
	XV_Y, VAROP_ROWY,
	XV_SHOW,FALSE,
	NULL);
 set_1d_probe_buttons(quadrant);
}

/**************************  MAKE_1D_DISP_PARMS()  ***************************/

make_1d_disp_parms(quadrant)
int quadrant;
{
int parm;

 for (parm=0; parm<NUM_1D_DATATYPE_PARMS; parm++) 
  probe_1d_data[parm][quadrant]=
        xv_create(op_panel[quadrant],PANEL_CHOICE,
        PANEL_CLIENT_DATA,quadrant,
	PANEL_EVENT_PROC, panel_event_proc,
	PANEL_NOTIFY_PROC,parm==PMS1D_DISP_CNTS?
         probe_1d_cnts_proc:probe_1d_liters_proc,
	XV_Y, VAROP_ROWY,
	XV_SHOW,FALSE,
	NULL);
 for (parm=0; parm<NUM_1D_SCALES_PARMS; parm++)
  probe_1d_scales[parm][quadrant]=
        xv_create(op_panel[quadrant],PANEL_CHOICE,
        PANEL_CLIENT_DATA,quadrant,
	PANEL_EVENT_PROC, panel_event_proc,
	PANEL_NOTIFY_PROC,parm==PMS1D_DISP_XTYPE?
         probe_1d_xtype_proc:probe_1d_ytype_proc,
	XV_Y, VAROP_ROWY,
	XV_SHOW,FALSE,
	NULL);
 set_1d_probe_disp_parms(quadrant);
}
/**************************  MAKE_2D_PROBE_OPS()  *****************************/

make_2d_probe_ops(quadrant)
int quadrant;
{
  probe_2d_probenames[quadrant]=xv_create(op_panel[quadrant],PANEL_TOGGLE,
        PANEL_CLIENT_DATA,quadrant,
	PANEL_EVENT_PROC, panel_event_proc,
	PANEL_NOTIFY_PROC,probe_2d_proc,
	XV_X,0,
	XV_Y, VAROP_ROWY,
	XV_SHOW,FALSE,
	NULL);
 set_2d_probe_buttons(quadrant);
}

/**************************  MAKE_HILITED()  *********************************/

make_hilited(quadrant)
int quadrant;
{
  hilited_ops[quadrant]=xv_create(op_panel[quadrant],PANEL_CHOICE,
        PANEL_CLIENT_DATA,quadrant,
	PANEL_EVENT_PROC, panel_event_proc,
	PANEL_NOTIFY_PROC,hilited_proc,
	XV_X,0,
	XV_Y, VAROP_ROWY,
	XV_SHOW,FALSE,
	NULL);
 set_hilite_buttons(quadrant);
}

/**************************  MAKE_TRK_TYPE_ITEM()  ****************************/

make_trk_type_item(quadrant)
int quadrant;
{
  trk_type_item[quadrant]=xv_create(op_panel[quadrant],PANEL_CHOICE,
	PANEL_VALUE,0,
        PANEL_CLIENT_DATA,quadrant,
	PANEL_EVENT_PROC, panel_event_proc,
	PANEL_NOTIFY_PROC,trk_type_item_proc,
	PANEL_CHOICE_STRINGS,TRK_TYPE_STRINGS,NULL,
	XV_X,0,
	XV_Y, VAROP_ROWY,
	XV_SHOW,FALSE,
	NULL);
}

/**************************  MAKE_LMRK_OPS()  *********************************/

make_lmrk_ops(quadrant)
int quadrant;
{
char lmrk_string[NAMLEN*2];

  lmrk_button[quadrant]=xv_create(op_panel[quadrant],PANEL_TOGGLE,
	PANEL_VALUE,0,
        PANEL_CLIENT_DATA,quadrant,
	PANEL_EVENT_PROC, panel_event_proc,
	PANEL_NOTIFY_PROC,lmrk_button_proc,
	PANEL_CHOICE_STRINGS,LMRK_STRING,NULL,
	XV_X, 
	 xv_get(vector_button[quadrant],XV_X)+
	 xv_get(vector_button[quadrant],XV_WIDTH)+
         OP_ITEMS_XGAP,
	XV_Y, VAROP_ROWY,
	XV_SHOW,FALSE,
	NULL);
  (void)sprintf(&lmrk_string[0],"%s:",LMRK_STRING);
  lmrk_ops[quadrant]=xv_create(op_panel[quadrant],PANEL_TOGGLE,
	PANEL_VALUE,0,
        PANEL_CLIENT_DATA,quadrant,
	PANEL_EVENT_PROC, panel_event_proc,
	PANEL_NOTIFY_PROC,lmrk_ops_proc,
	PANEL_LABEL_STRING,lmrk_string,
	PANEL_CHOICE_STRINGS,LMRK_OPS,NULL,
	XV_X,0,
	XV_Y, VAROP_ROWY,
	XV_SHOW,FALSE,
	NULL);
}

/**************************  MAKE_VECTOR_OPS()  ******************************/

make_vector_ops(quadrant)
int quadrant;
{
 int trace;
 static char number[MAX_TRACES][8];

 vector_button[quadrant]=xv_create(op_panel[quadrant],PANEL_TOGGLE,
	PANEL_VALUE,0,
        PANEL_CLIENT_DATA,quadrant,
	PANEL_EVENT_PROC, panel_event_proc,
	PANEL_NOTIFY_PROC,vector_button_proc,
	PANEL_CHOICE_STRINGS,VECTOR_STRING,NULL,
	XV_X, 
	 xv_get(geom_button[quadrant],XV_X)+
	 xv_get(geom_button[quadrant],XV_WIDTH)+
         OP_ITEMS_XGAP,
	XV_Y, VAROP_ROWY,
	XV_SHOW,FALSE,
	NULL);
printf("Vector_comps created\n");
  vector_comps[quadrant]=xv_create(op_panel[quadrant],PANEL_CHOICE,
	PANEL_VALUE,0,
        PANEL_CLIENT_DATA,quadrant,
	PANEL_EVENT_PROC, panel_event_proc,
	PANEL_NOTIFY_PROC,comps_proc,
	PANEL_LABEL_STRING,"Components:",
        PANEL_DISPLAY_LEVEL,PANEL_NONE,
	PANEL_CHOICE_STRINGS,"X","Y",NULL,
	XV_X, X_COMP_X,
	XV_Y, VAROP_ROWY,
	XV_SHOW,FALSE,
	NULL);
  vector_colors[quadrant]=xv_create(op_panel[quadrant],PANEL_CHOICE,
	PANEL_VALUE,get_vector_color(quadrant),
        PANEL_CLIENT_DATA,quadrant,
	PANEL_EVENT_PROC, panel_event_proc,
	PANEL_NOTIFY_PROC,colors_proc,
        PANEL_DISPLAY_LEVEL,PANEL_NONE,
	PANEL_LABEL_STRING,"Colors:",
	PANEL_CHOICE_STRINGS,"SameAsTrace","ContrastTrace",NULL,
	XV_X, 
	 xv_get(vector_comps[quadrant],XV_X)+
	 xv_get(vector_comps[quadrant],XV_WIDTH)+
         GetFontWidth(GetOpPanelFont()),
	XV_Y, VAROP_ROWY,
	XV_SHOW,FALSE,
	NULL);
  for (trace=0; trace<MAX_TRACES; trace++) {
/*
we're using separate items for each trace here instead of using a single
non-exclusive choice item because extracting the value from the latter is
more work than I want to do right now.
*/
   VectorsOnOff[quadrant][trace]=xv_create(op_panel[quadrant],PANEL_CHOICE,
        PANEL_CHOOSE_ONE,FALSE,
        PANEL_CLIENT_DATA,quadrant,
        PANEL_EVENT_PROC, panel_event_proc,
	PANEL_NOTIFY_PROC,VectorsOnOffProc,
/*
string y-position on PANEL_LABEL_STRING for CHOICE items w/PANEL_NONE are
different than those with default PANEL_CURRENT (PANEL_DISPLAY_LEVEL); 
account for aesthetics here with an approximation to make it look nicer
*/
        XV_Y, VAROP_ROWY-GetFontAscent(GetOpPanelFont())/3,
        XV_SHOW,FALSE,
        NULL);
   (void)sprintf(number[trace],"%d",trace+1);
   if (trace==0)
    xv_set(VectorsOnOff[quadrant][0],
	PANEL_LABEL_STRING,"On/Off:",
        NULL);
   xv_set(VectorsOnOff[quadrant][trace],
        XV_X,
         trace==0?(xv_get(vector_colors[quadrant],XV_X)+
         xv_get(vector_colors[quadrant],XV_WIDTH)+
          GetFontWidth(GetOpPanelFont())):
         (xv_get(VectorsOnOff[quadrant][trace-1],XV_X)+
         xv_get(VectorsOnOff[quadrant][trace-1],XV_WIDTH)+
         OP_ITEMS_XGAP),
        PANEL_CHOICE_STRINGS,number[trace],NULL,
	PANEL_VALUE,get_show_vector(quadrant,trace)?VectorsOn:VectorsOff,
        NULL);
  }
  SetIntervalButtonXPosn(xv_get(VectorsOnOff[quadrant][MAX_TRACES-1],XV_X)+
         xv_get(VectorsOnOff[quadrant][MAX_TRACES-1],XV_WIDTH)+
         OP_ITEMS_XGAP);
  set_comp_label(quadrant,X_COMP);
  set_comp_label(quadrant,Y_COMP);
}

/*****************************************************************************/

SetYComponentLabel(quadrant)
int quadrant;
{
 set_comp_label(quadrant,Y_COMP);
}

/*****************************************************************************/

SetXComponentLabel(quadrant)
int quadrant;
{
 set_comp_label(quadrant,X_COMP);
}

/**************************  MAKE_SCALE_OPS()  *******************************/

make_scale_ops(quadrant)
int quadrant;
{

  scale_button[quadrant]=xv_create(op_panel[quadrant],PANEL_TOGGLE,
	PANEL_VALUE,0,
        PANEL_CLIENT_DATA,quadrant,
	PANEL_EVENT_PROC, panel_event_proc,
	PANEL_NOTIFY_PROC,scale_button_proc,
	PANEL_CHOICE_STRINGS,SCALE_STRING,NULL,
	XV_Y, VAROP_ROWY,
	XV_SHOW,FALSE,
	NULL);
  scale_type_button[quadrant]=xv_create(op_panel[quadrant],PANEL_CHOICE,
	PANEL_VALUE,NORMAL_SCALE,
        PANEL_CLIENT_DATA,quadrant,
	PANEL_EVENT_PROC, panel_event_proc,
	PANEL_NOTIFY_PROC,scale_type_button_proc,
	PANEL_CHOICE_STRINGS,SCALE_TYPE_STRING,NULL,
	XV_Y, VAROP_ROWY,
	XV_X, 0,
	XV_SHOW,FALSE,
	NULL);
  indepvar_button[quadrant]=xv_create(op_panel[quadrant],PANEL_CHOICE,
	PANEL_VALUE,get_indepvar_yaxis(quadrant),
        PANEL_CLIENT_DATA,quadrant,
	PANEL_EVENT_PROC, panel_event_proc,
	PANEL_NOTIFY_PROC,indepvar_button_proc,
	PANEL_LABEL_STRING,INDEPVAR_LABEL_STRING,
	PANEL_CHOICE_STRINGS,INDEPVAR_STRING,NULL,
	XV_Y, VAROP_ROWY,
	XV_SHOW,FALSE,
	NULL);
}

/**************************  MAKE_GEOM_OPS()  *********************************/

make_geom_ops(quadrant)
int quadrant;
{
char geom_string[NAMLEN*2];

  geom_button[quadrant]=xv_create(op_panel[quadrant],PANEL_TOGGLE,
	PANEL_VALUE,0,
        PANEL_CLIENT_DATA,quadrant,
	PANEL_EVENT_PROC, panel_event_proc,
	PANEL_NOTIFY_PROC,geom_button_proc,
	PANEL_CHOICE_STRINGS,GEOM_STRING,NULL,
	XV_X, GEOM_X,
	XV_Y, VAROP_ROWY,
	XV_SHOW,FALSE,
	NULL);
  (void)sprintf(&geom_string[0],"%s:",GEOM_STRING);
  geom_ops[quadrant]=xv_create(op_panel[quadrant],PANEL_CHOICE,
	PANEL_VALUE,get_geom(quadrant),
        PANEL_CLIENT_DATA,quadrant,
	PANEL_EVENT_PROC, panel_event_proc,
	PANEL_NOTIFY_PROC,geom_ops_proc,
	PANEL_LABEL_STRING,geom_string,
	PANEL_CHOICE_STRINGS,GEOM_OPS,NULL,
	XV_X,GEOM_X,
	XV_Y, VAROP_ROWY,
	XV_SHOW,FALSE,
	NULL);
}

/**************************  MAKE_HISTORY()  *********************************/

make_history(quadrant)
int quadrant;
{
   history_ops[quadrant]=xv_create(op_panel[quadrant],PANEL_TOGGLE,
        PANEL_CLIENT_DATA,quadrant,
	PANEL_EVENT_PROC, panel_event_proc,
	PANEL_NOTIFY_PROC,history_proc,
	PANEL_CHOICE_STRINGS,HISTORY_OPS,NULL,
	XV_X,0,
	XV_Y, VAROP_ROWY,
	XV_SHOW,FALSE,
	NULL);
}

/**************************  MAKE_PRINTER()  *********************************/

make_message(quadrant)
int quadrant;
{

/* create message object
*/
    op_msg[quadrant]=xv_create(op_panel[quadrant],PANEL_MESSAGE,
	PANEL_CLIENT_DATA,quadrant,
	XV_X,0,
	XV_Y, VAROP_ROWY,
	XV_SHOW,FALSE,
	NULL);
}

/**************************  MAKE_CHGDISP()  *********************************/

make_chg_disp(quadrant)
int quadrant;
{
int type;

 for (type=0; type<MAX_DISPLAY_TYPES; type++) {
  spex_button[quadrant][type]=xv_create(op_panel[quadrant],PANEL_BUTTON,
   PANEL_CLIENT_DATA,quadrant,
   PANEL_EVENT_PROC, panel_event_proc,
   PANEL_NOTIFY_PROC,chgdisp_proc,
   PANEL_LABEL_STRING,DispTypeString[type],
   XV_SHOW,FALSE,
   NULL);
 }
}

/*****************************************************************************/

AttachPlotspecMenus(quadrant)
int quadrant;
{
int type;

  for (type=0; type<MAX_DISPLAY_TYPES; type++) {
   switch (type) {
    case DISPTYPE_XYPLOT:
     xv_set(spex_button[quadrant][0],
/* 
set 1st item so that adequate margin is kept between menus that appear and
the right and left edges of display screen.  If menus come up too close to
display screen edges, they will not stay up!  Thus, the menu buttons 
themselves must be positioned so as to preclude this...
*/
	XV_X, quadrant==UPPER_LEFT||quadrant==LOWER_LEFT?SPEX_BTN_LEFTX:
            SPEX_BTN_RIGHTX,
        XV_Y, VAROP_ROWY,
	PANEL_ITEM_MENU,XySubtypeMenu,
     NULL);
     break;
    case DISPTYPE_ETC:
     xv_set(spex_button[quadrant][type],
	PANEL_ITEM_MENU,EtcSubtypeMenu,
     NULL);
     break;
    case DISPTYPE_PMS2D:
     xv_set(spex_button[quadrant][type],
	PANEL_ITEM_MENU,Pms2dSubtypeMenu,
     NULL);
     break;
    default:
     xv_set(spex_button[quadrant][type],
	PANEL_ITEM_MENU,PlotspecMenu[type],
     NULL);
     break;
   }
   if (type)
    PositionToRightOfButton(spex_button[quadrant][type],
     spex_button[quadrant][type-1]);
  }
}

/******************************************************************************/

ShowDispSubMenus(quadrant)
int quadrant;
{
 show_oppanel(OP_CHG_DISP,quadrant);
}

/************************  HideDispSubMenus()  ********************************/

HideDispSubMenus()
/*
if the Disp menu sub-category type menu is showing in any of the setup windows,
hide it
*/
{
int quadrant;

 for (quadrant=0; quadrant<NUM_DISPLAY_QUADRANTS; quadrant++)  {
  if (DispSubMenusShowing(quadrant)) {
   hide_oppanel(quadrant);
   set_genop_value(quadrant,0);
   set_genop_hiliting(TRUE,quadrant,get_genop_value(quadrant));
  }
 }
}

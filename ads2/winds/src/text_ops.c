/* text_ops.c -- manage text item notification
*/

#include <stdio.h>
#include <xview/xview.h>
#include <xview/panel.h>

/**************************  WINDS INCLUDES  *********************************/

#include <constants.h>
#include <op_panels.h>
#include <display.h>
#include <lengths.h>
#include <xview.h>
#include <ops.h>
#include <plotspex.h>
#include <help.h>
#include <errors.h>
#include <xy_plt.h>
#include <macros.h>
#include <geom.h>
#include <proto.h>

/************************  GLOBAL WITHIN MODULE  *****************************/

static Panel_item text_object[NUM_DISPLAY_QUADRANTS];
static Panel_item display_feedback[NUM_DISPLAY_QUADRANTS];
static int text_op[NUM_DISPLAY_QUADRANTS];
static int text_op_underway[NUM_DISPLAY_QUADRANTS]={FALSE,FALSE,FALSE,FALSE};
static int text_object_up[NUM_DISPLAY_QUADRANTS]={FALSE,FALSE,FALSE,FALSE};
static int save_opvalue[NUM_DISPLAY_QUADRANTS];
static int save_textop[NUM_DISPLAY_QUADRANTS];

/**************************  GET_VALUES()  *********************************/

set_numeric_val(s,op,quadrant)
char *s;
int op,quadrant;
/* string s should not be any longer than MAX_DESC_LNGTH
*/
{
double bot,top_,bot_frac,top_frac,range;
double get_avg();
char *decimal;
int int_bot, int_top, int_range;  
int trace,nvals;
int lat_deg,lat_min,lat_sec,lon_deg,lon_min,lon_sec;
char label[LMRK_LBL_LNGTH];
char temp_string[MAX_DESC_LNGTH];

 trace=trace_number[quadrant];
 switch (op) {
   case PMS1D_MINY:
    nvals=1;
    if (sscanf(s,"%lf",&range) != nvals)
     return NON_NUMERIC_VALUE_ENTERED;
    minyrange(quadrant,s);
    break;
   case PMS1D_MAXY:
    nvals=1;
    if (sscanf(s,"%lf",&range) != nvals)
     return NON_NUMERIC_VALUE_ENTERED;
    maxyrange(quadrant,s);
    break;
   case PMS1D_PERIOD:
    nvals=1;
    if (sscanf(s,"%d",&int_range) != nvals)
     return (NON_NUMERIC_VALUE_ENTERED);
    set_period(quadrant,s);
    break;
   case SET_LIST_RATE:
    nvals=1;
    if (sscanf(s,"%d",&int_range) != nvals)
     return (NON_NUMERIC_VALUE_ENTERED);
    else if (int_range<=0)
     return LESS_OR_EQUAL_ZERO;
    else {
     SetListRate(quadrant,int_range);
    }
    break;
   case CHG_VECTOR_SCALE:
    nvals=1;
    if (sscanf(s,"%ld",&int_range) != nvals)
     return NON_NUMERIC_VALUE_ENTERED;
    else if (!int_range)
     return ZERO_UNREASONABLE;
    else
     set_max_vector_value(quadrant,int_range);
    break;
   case INTERVAL:
    nvals=1;
    if (sscanf(s,"%ld",&int_range) != nvals)
     return NON_NUMERIC_VALUE_ENTERED;
    else if (!int_range)
     return ZERO_UNREASONABLE;
    else
     set_vector_interval(quadrant,int_range);
    break;
   case SKEWT_RANGES:
    nvals=2;

/* read in minimum pressure, minimum temperature
*/
    if ( (sscanf(s,"%lf %lf",&bot,&top_) != nvals)) {
     return ONLY_ONE_VALUE_ENTERED;
    } 
    if (!bot)  

/* log scale -- zero won't cut it 
*/
     return ZERO_UNREASONABLE;

    skew_minpress[quadrant]=bot; skew_mintemp[quadrant]=top_;

/* set scales, done
*/
    set_dep_range(quadrant,CUSTOM);
    set_indep_range(quadrant,CUSTOM);
    draw_bkgnd(quadrant);
    break;
   case ADD_LANDMARK:

/* can enter various # of numbers for lat/lon: 
    deg.frac deg.frac / deg mm.frac deg mm.frac /
    deg deg / deg min deg min / deg min sec deg min sec - ergo, including label,
    number of possible entries is 3, 5, or 7
*/
    nvals=sscanf(s,"%s %d %d %d %d %d %d",&temp_string[0],
          &lat_deg,&lat_min,&lat_sec,&lon_deg,&lon_min,&lon_sec);
    decimal=strchr(&s[strlen(temp_string)],'.');
    if (decimal) {
     nvals=sscanf(s,"%s %lf %lf %lf %lf",
        &temp_string[0],&bot,&top_,&bot_frac,&top_frac);
     switch (nvals) {
     case 3:
      break;
     case 5:
      bot+=top_/60.0;
      top_=bot_frac+top_frac/60.0;
      break;
     default:
      return ONLY_ONE_VALUE_ENTERED;
     }
    } else {
     switch (nvals) {
     case 3:
      bot=(double)lat_deg; 
      top_=(double)lat_min;
      break;
     case 5:
      bot=ABS((double)lat_deg) + (double)(lat_min)/60.0; 
      top_=ABS((double)lat_sec) + (double)(lon_deg)/60.0;;
      bot=lat_deg<0?-bot:bot;
      top_=lat_sec<0?-top_:top_;
      break;
     case 7:
      bot=ABS((double)lat_deg)+(double)(lat_min)/60.0+(double)(lat_sec)/3600.0; 
      top_=ABS((double)lon_deg)+(double)(lon_min)/60.0 
       +(double)(lon_sec)/3600.0;
      bot=lat_deg<0?-bot:bot;
      top_=lon_deg<0?-top_:top_;
      break;
     default:
      return ONLY_ONE_VALUE_ENTERED;
     }
    }
    (void)strncpy(label,temp_string,LMRK_LBL_LNGTH-1);
    label[LMRK_LBL_LNGTH-1]='\0';
    add_landmark(quadrant,bot,top_,label);
    break;
   case CHG_FIXED_SCALE:
    nvals=2;
    if (trace == INDEPVAR 
       || dep_mode[quadrant][trace] != BITS) {
     if ( (sscanf(s,"%lf%lf",&bot,&top_) != nvals) &&
          (sscanf(s,"%lf,%lf",&bot,&top_) != nvals) )
      return ONLY_ONE_VALUE_ENTERED;
    } else { 

/* it's a dependent var in bits mode -- try to cover all reasonable 
   possibilities of  user input in hex or octal modes 
*/
     if ((sscanf(s,(hex?"%x%x":"%o%o"),&int_bot,&int_top)!=nvals)&&
      (sscanf(s,(hex?"%x,%x":"%o,%o"),&int_bot,&int_top)!=nvals)&& 
      (sscanf(s,(hex?"x%x,x%x":"0%o,0%o"),&int_bot,&int_top)!=nvals)&&
      (sscanf(s,(hex?"x%xx%x":"0%o0%o"),&int_bot,&int_top)!=nvals))
       return ONLY_ONE_VALUE_ENTERED;
    }    

/* check for input of zero -- not a good choice 
*/
    if ( (trace == INDEPVAR || 
          dep_mode[quadrant][trace] != BITS)?(top_-bot)
          :(int_top-int_bot)) {      
     if (trace != INDEPVAR) {

      dep_invert_scale[quadrant][dep_attr_num[quadrant][trace]]=
       (dep_mode[quadrant][trace] != BITS)?
       (bot>top_):(int_bot>int_top);

/* DON'T make labels 'nice' here -- this is at user discretion 
*/
      set_fixed_dep_range(quadrant,trace,
       (dep_mode[quadrant][trace]==BITS)?(double)int_bot:bot,
       (dep_mode[quadrant][trace]==BITS)?(double)int_top:top_,
       dep_attr_num[quadrant][trace]);

/* repeat for all traces if same scale is in effect -- assumed only possible
   if Same_Scale button already on, which makes all dependent traces FIXED
*/
      if (get_same_scale(quadrant))
       set_scales_same(quadrant,bot,top_);
     } else {
      indep_invert_scale[quadrant][indep_attr_num[quadrant]]=bot>top_;
      set_fixed_indep_range(quadrant,bot,top_,indep_attr_num[quadrant]);
     }
     draw_bkgnd(quadrant);
    } else {
     return ZERO_UNREASONABLE;
    }
    break;
  case CHG_TIME_SCALE:
  case CHG_FLOATING_SCALE:
    nvals=1;
    if (trace == INDEPVAR || 
        dep_mode[quadrant][trace] != BITS) {
     if (sscanf(s,"%lf",&range) != nvals)
      return NON_NUMERIC_VALUE_ENTERED;
    } else { 

/* it's a dependent var in bits mode -- try to cover all the possible user 
   entries in hex or octal modes 
*/
     if ((sscanf(s,(hex?"%x":"%o"),&int_range)!=nvals) &&
         (sscanf(s,(hex?"x%x":"0%o"),&int_range)!=nvals))
      return NON_NUMERIC_VALUE_ENTERED;
    }
    if ( (trace == INDEPVAR 
          || dep_mode[quadrant][trace] != BITS)?(range):
           (int_range)) {
     if (trace != INDEPVAR) {
      set_float_dep_range(quadrant,trace,
       get_avg(quadrant,seconds_to_avg[quadrant],
         dep_ptr[quadrant][trace],dep_mode[quadrant][trace]), 
       (dep_mode[quadrant][trace]==BITS)?
         (double)(ABS(int_range)):ABS(range),
       USER_ATTR,dep_attr_num[quadrant][trace]);
      draw_bkgnd(quadrant);
     } else {
      if (plot_type[quadrant]!=TIME_SERIES)
       set_float_indep_range(quadrant,
        get_avg(quadrant,seconds_to_avg[quadrant],
         indep_ptr[quadrant],indep_mode[quadrant]),
        ABS(range),USER_ATTR,indep_attr_num[quadrant]);
      else
       set_time_range(quadrant,(int)(ABS(range)));
      draw_bkgnd(quadrant);
     }
    } else {
     return ZERO_UNREASONABLE;
    }
    break;
 }

/* this return value, implying success, assumes that none of the error
   returns above (defined in errors.h) are equal to 0
*/
 return 0;
}

/**************************  STOP_TEXT_OP()  *********************************/

stop_text_op(quadrant)
int quadrant;
{
/* hide any text object except ones that are brought up via panel item buttons
*/
 if (quadrant > NUM_DISPLAY_QUADRANTS)
  return;

 if (text_object_up[quadrant]  && !text_op_from_button(text_op[quadrant])) {
  hide_text_op(quadrant);

/* restore the last panel shown, if any
*/
  show_oppanel(get_last_panel_shown(quadrant),quadrant);
  if (!text_op_from_button(save_textop[quadrant])) {
   set_genop_value(quadrant,save_opvalue[quadrant]);
   save_genop_value(quadrant,get_genop_value(quadrant));
  }
 }
}

/**************************  START_TEXT_OP()  *********************************/

start_text_op(quadrant,area)
int quadrant,area;
{
 switch (area) {
  case NO_AREA:
  case PLOT_FRAME_AREA:
  case ADSPOP_AREA:
  case FLTFRAME_AREA:
  case CALIB_SETPT_AREA:
  case PMS2D_AREA:
  case CALIB_DIALOGUE_AREA:
  case CALIB_PLOT_AREA:
  case MENU_AREA:
  case NOTICE_AREA:
  case NEW_SETUP_AREA:
  case OP_PANEL_AREA:
   break;
  case CHG_VECTOR_SCALE_AREA:
   show_text_op(quadrant,CHG_VECTOR_SCALE,"");
   break;
  case CHG_TRACE_AREA:
   show_text_op(quadrant,CHG_TRACE,"");
   break;
  case CHG_INDEP_AREA:
   show_text_op(quadrant,CHG_INDEPVAR,"");
   break;
  case ADD_TRACE_AREA:
   show_text_op(quadrant,ADD_TRACE,"");
   break;
  case CHG_Y_SCALE_AREA:
   switch (plot_type[quadrant]) {
    case SKEW_T:
    case LISTS:
    case FIXED_FORMAT:
     return;
    default:
     if (get_indepvar_yaxis(quadrant))
      show_text_op(quadrant,
      (indep_rng_typ[quadrant]==FIXED?CHG_FIXED_SCALE:
       CHG_FLOATING_SCALE),"");
     else
      show_text_op(quadrant,
      (dep_rng_typ[quadrant][trace_number[quadrant]]==FIXED?CHG_FIXED_SCALE:
       CHG_FLOATING_SCALE),"");
   }
   break;
  case CHG_X_SCALE_AREA:
   switch (plot_type[quadrant]) {
    case SKEW_T:
    case LISTS:
    case FIXED_FORMAT:
     return;
    default:
     show_text_op(quadrant,
      (trace_number[quadrant]==INDEPVAR?
       (indep_rng_typ[quadrant]==FIXED?CHG_FIXED_SCALE:CHG_FLOATING_SCALE):
       (dep_rng_typ[quadrant][trace_number[quadrant]]==FIXED?CHG_FIXED_SCALE:
        CHG_FLOATING_SCALE)
      ),"");
   }
   break;
  case CHG_TIME_SCALE_AREA:
   show_text_op(quadrant,CHG_TIME_SCALE,"");
   break;
  case PMS1D_ACCUM_AREA:
   show_text_op(quadrant,PMS1D_PERIOD,"");
   break;
  case PMS1D_YMIN_AREA:
   show_text_op(quadrant,PMS1D_MINY,"");
   break;
  case PMS1D_YMAX_AREA:
   show_text_op(quadrant,PMS1D_MAXY,"");
   break;
  case CHG_LISTEE_AREA:
   show_text_op(quadrant,CHG_LISTEE,"");
   break;
  case CTL_AREA:
   break;
  case CHG_FIXEE_AREA:
   show_text_op(quadrant,CHG_FIXEE,"");
   break;
 }
}

/*************************  LABEL_STRING()  **********************************/

char *
label_string(quadrant)
int quadrant;
{
static char string[MAX_DESC_LNGTH*2];

 switch (text_op[quadrant]) {
  case ADD_LANDMARK:
   (void)sprintf(string,"Label Lat Lon <d [m [s]]>:");
   break;
  case DESCRIPTION:
   (void)sprintf(string,"Save setup as:");
   break;
  case CHG_LISTEE:
   (void)sprintf(string,"Chg %s:",
    (char *)GetListeeName(quadrant,listee_number[quadrant]));
   break;
  case CHG_FIXEE:
   (void)sprintf(string,"Chg %s:",
    (char *)GetFixeeName(quadrant,fixee_number[quadrant]));
   break;
  case CHG_TRACE:
   (void)sprintf(string,"Chg %s:",
    trace_name[quadrant][trace_number[quadrant]]);
   break;
  case ADD_TRACE:
   (void)sprintf(string,"Add trace:");
   break;
  case CHG_INDEPVAR:
   (void)sprintf(string,"Chg %s:",
    (char *)GetPlotAttrName(indep_attr_num[quadrant]));
   break;
  case CHG_TIME_SCALE:
   (void)sprintf(string,"Time range [seconds]:");
   break;
  case CHG_FIXED_SCALE:
   (void)sprintf(string,"FIXED range [min max]:");
   break;
  case CHG_FLOATING_SCALE:
   (void)sprintf(string,"FLOATING range [+ or -]:");
   break;
  case CHG_VECTOR_SCALE:
   (void)sprintf(string,"Vector scale max:");
   break;
  case PMS1D_PERIOD:
   (void)sprintf(string,"Accum'n Period:");
   break;
  case PMS1D_MINY:
   (void)sprintf(string,"Y-minimum:");
   break;
  case PMS1D_MAXY:
   (void)sprintf(string,"Y-maximum:");
   break;
  case SKEWT_RANGES:
   (void)sprintf(string,"Minimums [press temp]:");
   break;
  case INTERVAL:
   (void)sprintf(string,INTRVL_STRING);
   break;
  case SET_LIST_RATE:
   (void)sprintf(string,"List Updates (sec):");
   break;
 }
 return string;
}

/**************************  TEXT_PROC()  *********************************/
 
/*ARGSUSED*/
Panel_setting
text_proc(item,event)
Panel_item item;
Event *event;
{
int quadrant;
char kbd_input1[MAX_DESC_LNGTH];  /* user keyboard input value(s)  */
char *kbd_input;  /* nice-ified string */
int retval;
 
 quadrant=xv_get(item,PANEL_CLIENT_DATA);
 (void)strncpy(kbd_input1,(char *) xv_get(item,PANEL_VALUE),MAX_DESC_LNGTH-1);
 if (!(strlen(kbd_input1)))  { 

/* null string: take no action 
*/
   return PANEL_NONE;
 }

/* trim leading and trailing blanks and adjust text item value accordingly
*/
 kbd_input=&kbd_input1[strspn(kbd_input1," ")];
 (void)TrimTrailingBlanks(kbd_input);
 xv_set(text_object[quadrant],
        PANEL_VALUE,kbd_input,
        NULL);
 
 update_mode[quadrant]=text_op[quadrant];
 switch (text_op[quadrant]) {
  case PMS1D_MINY:
  case PMS1D_MAXY:
  case PMS1D_PERIOD:
  case CHG_VECTOR_SCALE:
  case CHG_FIXED_SCALE:
  case CHG_TIME_SCALE:
  case CHG_FLOATING_SCALE:
  case SKEWT_RANGES:
  case INTERVAL:
  case ADD_LANDMARK:
  case SET_LIST_RATE:
   if ( (retval=set_numeric_val(kbd_input,text_op[quadrant],quadrant)) ) {
    set_notice_needed(retval);
    update_mode[quadrant]=NORMAL_UPDATE;
   } else
    need_rehilite[quadrant]=TRUE;
   break;
  case DESCRIPTION:
   if (GetCurrentFolderDeleted()) {
    show_op_msg(quadrant,FOLDERDELETED);
    return PANEL_NONE;
   }
   if (!GetFolderEditsEnabled()) {
    show_op_msg(quadrant,EDITSDISABLED);
    return PANEL_NONE;
   }
   if (num_of_setups() == get_maxsetups()) {
    show_op_msg(quadrant,NO_MORE_SETUPS);
    return PANEL_NONE;
   }
   if (
    (strstr(kbd_input,FIRST_WORD)!=NULL) 
    || (strstr(kbd_input,ARGSWORD)!=NULL) 
    || (strstr(kbd_input,ENDARGSWORD)!=NULL) 
    || (strstr(kbd_input,PASSWORDENTRY)!=NULL) 
    || (strstr(kbd_input,DESCRIBEWORD)!=NULL) 
   ) {
    (void)sprintf(error_info,"SORRY: You are not allowed to use the following strings in your description: '%s', '%s', '%s', '%s'. Please enter a description without any of these.\n",FIRST_WORD,ARGSWORD,ENDARGSWORD,DESCRIBEWORD,PASSWORDENTRY);
    set_notice_needed(INVALID_DESCRIPTION);
   } else {
    SetNeedSaveSetup(quadrant,TRUE);
    (void)strncpy(description,kbd_input,MAX_DESC_LNGTH-1);
    if (window_type[quadrant]==DISPTYPE_XYPLOT)
     update_mode[quadrant]=NORMAL_UPDATE;
   }
   break; 
  case ADD_TRACE:
  case CHG_INDEPVAR:
  case CHG_TRACE:
  case CHG_LISTEE:
  case CHG_FIXEE:

/* accept only first NAMLEN-1 characters; truncate name at first blank space
*/
   kbd_input[NAMLEN-1]='\0';
   if (nindex(kbd_input,' '))
    (void)sprintf(&kbd_input[0],"%s",substring(kbd_input,' '));
    
   if (text_op[quadrant]==ADD_TRACE)
    num_of_traces[quadrant]++;
   if (text_op[quadrant]==CHG_INDEPVAR)
    trace_number[quadrant]=INDEPVAR;
   if (indices(quadrant,kbd_input)==ERROR) {
    set_notice_needed(VARIABLE_NOT_AVAILABLE);
    report(quadrant,"variable not available");
    update_mode[quadrant]=NORMAL_UPDATE;
    if (text_op[quadrant]==ADD_TRACE)
     num_of_traces[quadrant]--;
   } else {
    need_rehilite[quadrant]=TRUE;
    if (text_op[quadrant]==CHG_LISTEE)
     if (list_printing[quadrant])
      print_header(quadrant);
   }
   break;

 } 

/* update text object's label and clear value unless it makes sense to leave 
   it up
*/
 if (text_op[quadrant]!=SKEWT_RANGES 
     && text_op[quadrant]!=INTERVAL 
     && text_op[quadrant]!=DESCRIPTION 
     && text_op[quadrant]!=SET_LIST_RATE 
    )
  xv_set(text_object[quadrant],
        PANEL_VALUE,"",
        PANEL_LABEL_STRING,label_string(quadrant),
        NULL);
 text_op_underway[quadrant]=FALSE;
 return PANEL_NONE;
}

/*************************  display_saved_feedback  ***************************/

display_saved_feedback(quadrant,display)
int quadrant,display;
{
 xv_set(display_feedback[quadrant],
	 XV_SHOW,display,
         XV_X, xv_get(text_object[quadrant],XV_WIDTH)+GetFontWidth(GetOpPanelFont())*2,
	NULL);
}

/*************************  TEXT_OP_FROM_BUTTON()  ***************************/

text_op_from_button(op)
int op;
/*
return TRUE if op is one that display a text item from user pressing an
op-panel button, as opposed to starting to type in a highlighted area
*/
{
  return (op == DESCRIPTION ||
  op == INTERVAL ||
  op == SKEWT_RANGES ||
  op == SET_LIST_RATE ||
  op == ADD_LANDMARK);
}

/*****************************************************************************/

ShowTextItem(quadrant)
int quadrant;
{
 ShowPanelItem(text_object[quadrant]);
}

/*************************  SHOW_TEXT_OP()  **********************************/

show_text_op(quadrant,op,string)
int quadrant,op;
char *string;
/*
call with either an empty string, if called after user strikes a key that is
to be the first input character, e.g. with cursor highlighting the change-trace
area, or with the string desired shown on appearance after user request the 
input item from some other button, e.g. List Update Rate.
*/
{

 if (text_op_underway[quadrant] && text_op[quadrant]==op)
  return;

 if (!text_op_from_button(op)) {
/*
only those text items invoked via user keystrokes (as first input char.) 
should save previous value of op panel for future restores
*/
  save_opvalue[quadrant]=get_genop_value(quadrant);
  save_textop[quadrant]=op;
 }
 hide_oppanel(quadrant);
 text_op[quadrant]=op;

 if (op==INTERVAL)
  xv_set(text_object[quadrant],
        PANEL_VALUE_DISPLAY_LENGTH, INTRVL_TEXT_LNGTH,
        PANEL_VALUE_STORED_LENGTH, INTRVL_TEXT_LNGTH,
	XV_X,GetIntervalButtonXPosn(),
        NULL);
 else if (op==DESCRIPTION)
  xv_set(text_object[quadrant],

/* Offset total by 3 to account for added space at end of description string, 
   which is needed to keep words separated in final version.
*/
        PANEL_VALUE_DISPLAY_LENGTH, MAX_DESC_LNGTH-3,
        PANEL_VALUE_STORED_LENGTH, MAX_DESC_LNGTH-3,
	XV_X,NORMAL_TEXT_X,
        NULL);
 else
  xv_set(text_object[quadrant],
        PANEL_VALUE_DISPLAY_LENGTH, NORMAL_TEXT_LNGTH,
        PANEL_VALUE_STORED_LENGTH, NORMAL_TEXT_LNGTH,
	XV_X,NORMAL_TEXT_X,
        NULL);

  if (!text_op_from_button(op)) {
/* 
for appropriate text operations, clear any MENU type
genop_choice buttons
*/
   set_genop_value(quadrant,0);
   set_genop_hiliting(TRUE,quadrant,0);
 }
 xv_set(text_object[quadrant],
        XV_Y,VAROP_ROWY,
        PANEL_LABEL_STRING,label_string(quadrant),
        PANEL_VALUE, " ",
        NULL);
 UpdateTextObject(quadrant,string);
 text_op_underway[quadrant]=TRUE;
 text_object_up[quadrant]=TRUE;

/* get keyboard focus to this object
*/
 set_kbd_focus_window(paint_window[quadrant+FIRST_OP_QUADRANT]);
}

/***********************  UpdateTextObject()  **********************************/

UpdateTextObject(quadrant,string)
int quadrant;
char *string;
{
 if (strlen(string))
  xv_set(text_object[quadrant],
        PANEL_VALUE,string,
        NULL);
}

/*************************  HIDE_TEXT_OP()  **********************************/

hide_text_op(quadrant)
int quadrant;
{
 xv_set(text_object[quadrant],
        PANEL_VALUE,"",
        XV_Y, VAROP_ROWY+OP_PANEL_HEIGHT,
        NULL);
 text_op_underway[quadrant]=FALSE;
 text_op[quadrant]=ERROR;
 text_object_up[quadrant]=FALSE;
}


/**************************  MAKE_TEXT_ITEM()  *******************************/

make_text_item(quadrant)
int quadrant;
{
   text_object[quadrant]=xv_create(op_panel[quadrant],PANEL_TEXT,
        PANEL_NOTIFY_LEVEL, PANEL_SPECIFIED,
        PANEL_NOTIFY_STRING,NOTIFY_STRING,
        PANEL_CLIENT_DATA,quadrant,
        PANEL_EVENT_PROC, panel_event_proc,
        PANEL_NOTIFY_PROC,text_proc,

/* setting value to something other than empty string prevents the 
   discolored garbage caret on first appearance
*/
        PANEL_VALUE,"",

/* Offset total by 3 to account for added space at end of description string, 
   which is needed to keep words separated in final version.
*/
        PANEL_VALUE_DISPLAY_LENGTH, MAX_DESC_LNGTH-3,
        PANEL_VALUE_STORED_LENGTH, MAX_DESC_LNGTH-3,
        XV_X,0,

/* hide text item by position instead of use of XV_SHOW, since latter
   interferes with setting keyboard focus properly.  Show text item later
   by moving its position again.
*/
        XV_Y, VAROP_ROWY+OP_PANEL_HEIGHT,
        NULL);
   display_feedback[quadrant]=xv_create(op_panel[quadrant],PANEL_MESSAGE,
        XV_SHOW,FALSE,
        XV_Y, VAROP_ROWY,
        PANEL_LABEL_STRING," (Saved)",
        NULL);
}

/**************************************************************************/
 
MakeTextItem(panel,string,minimum,maximum,defvalue,length,proc)
Panel panel;
char *string;
int minimum,maximum,defvalue,length;
void (*proc)();
/*
make numeric text item, Assign it minimum and maximum input value limits and
defvalue initial value.
*/
{
Panel_item item;
 
 item=xv_create(panel,PANEL_NUMERIC_TEXT,
        PANEL_LABEL_STRING,string,
        PANEL_MIN_VALUE,minimum,
        PANEL_MAX_VALUE,maximum,
        PANEL_VALUE,defvalue,
        PANEL_NOTIFY_PROC,proc,
        PANEL_VALUE_DISPLAY_LENGTH,length,
        NULL);
 return item;
}


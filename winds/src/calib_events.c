#include <stdio.h>
#include <string.h>
#include <xview/xview.h>
#include <xview/panel.h>


/****************************  WINDS INCLUDES  ******************************/

#include <constants.h>
#include <lengths.h>
#include <xy_plt.h>
#include <errors.h>
#include <xview.h>
#include <calib.h>
#include <shmem.h>
#include <tasks.h>
#include <ops.h>
#include <files.h>
#include <macros.h>
#include <display.h>
#include <plotspex.h>
#include <proto.h>

/*************************  EXPORTABLE  ***************************************/

int need_calib_attr;		/* TRUE when invalid input entered in popup */
int data_indx;              /* pointer determined by parameter name */
char varname[NAMLEN];         /* current variable being plotted */
float set_point;              /* y-axis set point value */
char set_pt_string[VALUELEN];
int num_sex;      /* number of seconds used to calibrate */
char num_sex_string[VALUELEN];
struct hist *current;        /* pointer to current history structure */
char new_units[UNITSLEN];
float bottom, top;
struct hist *search(), *add_member();


/**********************  SET_SET_POINT()  ************************************/

set_set_point()
{
float test_float;

   if ( sscanf(
    (char *)XGetAttr (set_pt,PANEL_VALUE),
    "%f",
    &test_float) 
    != 1) {

/* error input; restore previous value and notify user of boo-boo
*/
    (void)XSetAttr (set_pt,PANEL_VALUE,set_pt_string,NULL);
    set_notice_needed(NON_NUMERIC_VALUE_ENTERED);
   } else {

/* good input; store tested value in float and character forms, truncate
   trailing zeroes of character form
*/
    set_point=test_float;
    (void)sprintf(set_pt_string,"%f",set_point);
    (void)trim_trailing_digits(set_pt_string,LBL_PRECISION,TRUE);
   }
}

/************************  GET_INPUT_FUNC()  *********************************/

/*ARGSUSED*/
Panel_setting
get_input_func(item, event)
Panel_item item;
Event *event;
/* called on <r> over each panel item; process according to PANEL_CLIENT_DATA
*/
{
int which,test_int;

 switch (which=(int)(XGetAttr (item,PANEL_CLIENT_DATA))) {
  case XMINIMUM:

/* set minimum and range, so that next time this variable is called, the 
   same range will apply.
*/
   (void)sprintf(&default_mins[indep_attr_num[PLOTS]*VALUELEN],"%s",
    (char *)XGetAttr(item,PANEL_VALUE));
   (void)sscanf(&default_mins[indep_attr_num[PLOTS]*VALUELEN],"%lf",
    &indep_min[PLOTS]);
   (void)sprintf(&default_range[indep_attr_num[PLOTS]*VALUELEN],"%f",
    ABS(indep_max[PLOTS]-indep_min[PLOTS])); 

/* set scales, update the set point, and set flag to redraw the plot next
   time-out
*/
   indep_invert_scale[PLOTS][indep_attr_num[PLOTS]]=
    indep_min[PLOTS]>indep_max[PLOTS];
   set_indep_range(PLOTS,DEFAULT);
   update_setpt(indep_min[PLOTS]);
   update_mode[PLOTS]=INITIAL;
   break;
  case XMAXIMUM:
   (void)sscanf((char *)XGetAttr(item,PANEL_VALUE),"%lf",
    &indep_max[PLOTS]);
   (void)sprintf(&default_range[indep_attr_num[PLOTS]*VALUELEN],"%f",
    ABS(indep_max[PLOTS]-indep_min[PLOTS])); 
   indep_invert_scale[PLOTS][indep_attr_num[PLOTS]]=
    indep_min[PLOTS]>indep_max[PLOTS];
   set_indep_range(PLOTS,DEFAULT);
   update_mode[PLOTS]=INITIAL;
   break;
  case VAR_UNITS:
   (void)sprintf(&units[indep_attr_num[PLOTS]*UNITSLEN],"%s",
    (char *)XGetAttr(item,PANEL_VALUE));
   draw_bkgnd(PLOTS);
   break;
  case SENSOR_TYPE:
  case SENSOR_SN:
  case COND_SN: 
  case AMP_GAIN: 
  case OFF_SET: 
  case AMP_CUT_OFF: 
  case ADS_ADDRESS: 
  case TECHNICIAN: 
   update_member(which,current,(char *)XGetAttr(item,PANEL_VALUE),0.0);
   break;
  case SET_POINT:
   set_set_point();
   return (PANEL_NONE);    /* don't advance caret since this item will be 
                              changed repeatedly for each variable */
  case ACCUM_INTRVL:
   if (sscanf((char *)XGetAttr (item,PANEL_VALUE),"%d",&test_int) != 1) {
    (void)XSetAttr(num_sx,PANEL_VALUE,num_sex_string,NULL);
    set_notice_needed(NON_NUMERIC_VALUE_ENTERED);
   } 
   if (!test_int) {
    (void)XSetAttr (num_sx,PANEL_VALUE,num_sex_string,NULL);
    set_notice_needed(ZERO_UNREASONABLE);
   } else {
    num_sex=test_int;
    (void)sprintf(num_sex_string,"%d",num_sex);
   }
   break;
  default:
   break;
 } 
 return(PANEL_NEXT);
}

/************************ CALIB_PROC ************************************/

void calib_proc(event)
Event *event;
/* called on MS_RIGHT over any calib window
*/
{
 SortByAnalog();
 ShowTypeButton();
 SetDomainButtonValue("type");
 SetTypeButtonValue("analog");
 ShowVarMenu(current_quadrant,event);
}

/***************************************************************************/

InstallCalibSelection(index)
int index;
{
 SetSelectionMade(TRUE);
 menu_selected=TRUE;
 need_calib_attr=FALSE;
 SetCalibTrace(index);
}

/******************************************************************************/

SetCalibTrace(index)
int index;
{
char description[MAX_DESC_LNGTH];

 if (get_mode(&index)!=RAW)  /* not in raw_list: do naught */
  return(FALSE);
 (void)sprintf(varname,"%s",&raw_list[index*NAMLEN]);
 data_indx=index;
/* initialize usrvar and indepvar_name for call to setup_plots()  */
 (void)strcpy(usrvar[PLOTS][0],varname);
 (void)strcpy(indepvar_name[PLOTS],varname);
 if (!(current = search(varname))) /* get structure for current var */
  current = add_member(varname);
 setup_plots(PLOTS);
 (void)sprintf(description,"Calib'n of %s",varname);
 set_description(PLOTS,description);
 update_dialogue(current);
/*
see update_setpt commentary for explanation for casting set_point to double
*/
 update_setpt((double)set_point);
}

/**********************  UPDATE_SETPT()  *************************************/

update_setpt(value)
/*
for some reason...declaring value as a float (and using %f in the sprintf
to follow) causes a misunderstanding...value is viewed as a double in this
routine whether I declare it a float or not. So, to keep everybody working
together, declare value a double and go to %lf.
*/
double value;
{
 (void)sprintf(set_pt_string,"%lf",value);
 (void)trim_trailing_digits(set_pt_string,LBL_PRECISION,TRUE);
 (void)XSetAttr (set_pt,PANEL_VALUE,set_pt_string,NULL);
}

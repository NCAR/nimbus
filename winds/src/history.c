#include <stdio.h>
#include <xview/xview.h>
#include <xview/panel.h>


/****************************  WINDS INCLUDES  ******************************/

#include <constants.h>
#include <lengths.h>
#include <calib.h>
#include <help.h>
#include <files.h>
#include <socket.h>
#include <display.h>
#include <header.h>
#include <xy_plt.h>
#include <proto.h>

/****************************  EXPORTABLE  ********************************/

struct hist *save_start,
            *save_end;
FILE *WINDSfopen();
static FILE *history_file;
static char calib_history_flnm[MAX_FLNM_LNGTH]; 


/**********************  OPEN_HISTORY_FILE()  *******************************/

FILE *
open_history_file(mode)
char *mode;
{
 (void)sprintf(&calib_history_flnm[0],HISTORY,ProjectDirectory,proj_number);
 history_file = WINDSfopen(calib_history_flnm,mode);
 return history_file;
}

/******************************  GET_HISTORY()  ******************************/

get_history()
/* read in the history file
*/
{
struct hist *list;

 if (save_start!=NULL)

/* preserve history list if already gotten once
*/
  return;

 save_start=save_end=NULL;
 if ( (history_file=open_history_file("r"))==NULL)
  return;
 list = (struct hist *)(malloc((unsigned int) sizeof(struct hist)));
 update_member(VARNAME,list,"",0.0); 
 update_member(SENSOR_TYPE,list,"",0.0); 
 update_member(SENSOR_SN,list,"",0.0);
 update_member(COND_SN,list,"",0.0);
 update_member(AMP_GAIN,list,"",0.0);
 update_member(OFF_SET,list,"",0.0);
 update_member(AMP_CUT_OFF,list,"",0.0);
 update_member(ADS_ADDRESS,list,"",0.0);
 update_member(TECHNICIAN,list,"",0.0);
 update_member(C0,list,"",0.0);
 update_member(C1,list,"",1.0);
 update_member(C2,list,"",0.0);
 list->valid_coefs=FALSE;
 list->next=NULL;
 save_start = list;
 while(fread((char *)(list),sizeof(struct hist),1,history_file)!=(size_t)NULL) {
  save_end = list;
  init_reserved_space(list);
  list->next = (struct hist *)(malloc(sizeof(struct hist)));
  list = list->next;
 }
 if (save_end)
  save_end->next = NULL;

 (void)WINDSfclose(history_file);
}

/******************************  SEARCH()  ******************************/

struct hist *search(name)
char *name;
/* search "linked list" of history file entries for match with string name 
*/
{
struct hist *linked_list;

 linked_list = save_start;
 while (linked_list) {
  if( !(strcasecmp(linked_list->calib_name,name))) 
   return(linked_list);
  else  
   linked_list = linked_list->next;
 }
 return(NULL);
}

/******************************  UPDATE_DIALOGUE()  **************************/


update_dialogue(member)
struct hist *member;
/* update information displayed by panel "dialogue". */
{
char string[VALUELEN];
extern Panel dialogue;
extern Panel_item var_name,                      
	   xminimum,
	   xmaximum,
	   var_units,
           sensor_type,
           sensor_sn,
           cond_sn,
           amp_gain,
	   off_set,
           amp_cutoff,
           ads_addr,
           tech;
 
 (void)xv_set(xminimum,PANEL_VALUE,&default_mins[indep_attr_num[PLOTS]*VALUELEN],NULL);
 (void)sprintf(string,"%f",indep_max[PLOTS]);
 (void)trim_trailing_digits(string,LBL_PRECISION,TRUE);
 (void)xv_set(xmaximum,PANEL_VALUE,string,NULL);
 (void)xv_set(var_units,PANEL_VALUE,&units[indep_attr_num[PLOTS]*UNITSLEN],NULL);

 (void)xv_set(var_name,PANEL_LABEL_STRING,member->calib_name,NULL);
 (void)xv_set (sensor_type,PANEL_VALUE,member->sensor_type,NULL);
 (void)xv_set (sensor_sn,PANEL_VALUE,member->sensor_sn,NULL);
 (void)xv_set(cond_sn,PANEL_VALUE,member->cond_sn,NULL);
 (void)xv_set (amp_gain,PANEL_VALUE,member->amp_gain,NULL);
 (void)xv_set(off_set,PANEL_VALUE,member->off_set,NULL);
 (void)xv_set (amp_cutoff,PANEL_VALUE,member->amp_cut_off,NULL);
 (void)xv_set (ads_addr,PANEL_VALUE,member->ads_address,NULL);
 (void)xv_set (tech,PANEL_VALUE,member->technician,NULL);

/* start with first text item */
 (void)xv_set (dialogue,PANEL_CARET_ITEM,sensor_type,NULL);
}

/******************************  UPDATE_HISTFILE()  *************************/

update_histfile()
/* update the history file on disk with the entire contents of list pointed
   to by save_start
*/

{
struct hist *list;

 if ( (history_file = open_history_file("w")) == NULL) {
  time_to_log();
  (void)fprintf(stderr,"FATAL: unable to open a writable %s\n",
   calib_history_flnm);
  RequestAlertNow("FATAL: can't open writable calib history file...contact WINDS expert with this information!");
  winds_cleanup(ERROR);
 }
 list = save_start;
 while(list) {
  (void)fwrite((char *)(list),sizeof(struct hist),1,history_file);
  list = list->next;
 }
 (void)WINDSfclose(history_file);

/* if current attributes are default, add entry in calib attributes file
   on their behalf
*/
 if (need_calib_attr) {
  add_entry_to_file(
   PROJ_CALIB_ATTR,varname,&units[indep_attr_num[PLOTS]*UNITSLEN],
   &default_range[indep_attr_num[PLOTS]*VALUELEN],
   &default_mins[indep_attr_num[PLOTS]*VALUELEN]);
  need_calib_attr=FALSE;
 }
}

/******************************  ADD_MEMBER()  *******************************/

struct hist *add_member(name)
char *name;
/* allocate a new member to list pointed to by save_start and return a pointer
   to that member, which is initialized with default values
*/
{ 
 if (save_end) {
  save_end->next = (struct hist *)(malloc(sizeof(struct hist)));
  save_end=save_end->next;
 } else {
  save_start=save_end = (struct hist *)(malloc(sizeof(struct hist)));
 }
 update_member(VARNAME,save_end,name,0.0); 
 update_member(SENSOR_TYPE,save_end,"",0.0); 
 update_member(SENSOR_SN,save_end,"",0.0);
 update_member(COND_SN,save_end,"",0.0);
 update_member(AMP_GAIN,save_end,"",0.0);
 update_member(OFF_SET,save_end,"no",0.0);
 update_member(AMP_CUT_OFF,save_end,"",0.0);
 update_member(ADS_ADDRESS,save_end,"",0.0);
 update_member(TECHNICIAN,save_end,"",0.0);
 update_member(C0,save_end,"",0.0);
 update_member(C1,save_end,"",1.0);
 update_member(C2,save_end,"",0.0);
 save_end->valid_coefs=FALSE;
 init_reserved_space(save_end);
 save_end->next=NULL;
 return(save_end);
}

/************************  INIT_RESERVED_SPACE()  ****************************/

init_reserved_space(list)
struct hist *list;
{
long byte;
 for (byte=0; byte<RESERVED; byte++)
  list->reserved_space[byte]='\0';
}

/******************************  UPDATE_MEMBER()  ****************************/

update_member(which,member,string,coef)
int which;
struct hist *member;
char *string;
float coef;
/* update the which'th field of structure pointed to by member with either
   character constant string or float value coef.  
*/
{
if(!(member)) return;
 switch (which) {
  case VARNAME:
   (void)sprintf(member->calib_name,"%s",string);
   break;
  case SENSOR_TYPE:
   (void)sprintf(member->sensor_type,"%s",string);
   break;
  case SENSOR_SN:
   (void)sprintf(member->sensor_sn,"%s",string);
   break;
  case COND_SN:
   (void)sprintf(member->cond_sn,"%s",string);
   break;
  case AMP_GAIN:
   (void)sprintf(member->amp_gain,"%s",string);
   break;
  case OFF_SET:
   (void)sprintf(member->off_set,"%s",string);
   break;
  case AMP_CUT_OFF:
   (void)sprintf(member->amp_cut_off,"%s",string);
   break;
  case ADS_ADDRESS:
   (void)sprintf(member->ads_address,"%s",string);
   break;
  case TECHNICIAN: 
   (void)sprintf(member->technician,"%s",string);
   break;
  case C0:
   member->coef_0=coef;
   break;
  case C1:
   member->coef_1=coef;
   break;
  case C2:
   member->coef_2=coef;
   break;
 }
}
/********************* PRINT_HISTORY **********************/
print_history(int diskfile)
{
  struct hist summary;
  FILE	*ascii_history, *outFP;

  if (!diskfile && get_lstat_port()==ERROR) 
    return;

  if ((history_file = open_history_file("r")) == NULL) {
    fprintf(stderr,"WARNING: print_history can't open readable %s\n",
	calib_history_flnm);
    RequestAlertNow("WARNING: History file for this project was not found.");
    return;
    }

  if (diskfile)
    outFP = WINDSfopen("/home/local/proj/defaults/history.text", "w");
  else
    outFP = WINDSfopen("historyPrint", "w+");

  fprintf(outFP, "\n\n\n\nProject %s (%s)      %s      %s\nVARIABLE NAME      SN           TYPE        COEF_0       COEF_1       COEF_2 \n\n",
	(char *)get_proj_number(),(char *)GetWINDSProjectName(),
	(char *)GetWINDSAircraftName(),(char *)get_date());

  while (fread((char *)&summary, sizeof(struct hist), 1, history_file)
						!= (size_t)NULL)
    {
    fprintf(outFP, "%-13s %6s %16s %12.4f %12.4f %12.4f\n",
		summary.calib_name,summary.sensor_sn,summary.sensor_type,
		summary.coef_0,summary.coef_1,summary.coef_2);
    }

  (void)WINDSfclose(history_file);
  (void)WINDSfclose(outFP);

  system("/home/user/ads/my_lp -h -r -Plp historyPrint");
}

 
/* stats.c -- manage statistics setup parameters and output

   SCCS ID:
@(#)stats.c	1.1 11/14/90

*/

#include <stdio.h>
#include <xview/xview.h>
#include <xview/canvas.h>

/****************************  WINDS INCLUDES  ******************************/

#include <proto.h>
#include <constants.h>
#include <lengths.h>
#include <shmem.h>
#include <calib.h>
#include <colors.h>
#include <xy_plt.h>
#include <macros.h>
#include <display.h>
#include <socket.h>

/****************************  EXPORTABLE  ********************************/

int setpts_width, 			/* width of window */
    setpts_height;			/* height of window */


/***************************  GLOBAL WITHIN FILE  ****************************/

static double volt_sum,                  	/* statistics stuff */
       volt_sumsq,
       volt_minimum,
       volt_maximum;
static double value_sum,
       value_sumsq,
       value_minimum,
       value_maximum;
double sqrt();
static float r,erms,emax;

static struct set_stats *page_top=NULL,		/* top of entries displayed */
                        *last_setpt=NULL,	/* last entry in list */
                        *first_setpt=NULL,  	/* top entry in list */
			*free_setpt,		/* free memory pointers */
			*next_setpt;		
struct set_stats *hilited_setpt=NULL, 		/* highlighted entry */
                 *find_setpt(); 		/* func to find entry in list */
static int current_yrow;			/* top of next entry */
static struct coordinates *last_coord,  	/* 1st coord in replot list */
                          *first_coord;		/* last coord in replot list */

/*************************  INIT_ACCUMS()  ************************************/

init_accums()
/* initialize statistics accumulations and coordinate list
*/
{
struct coordinates *next_coord,*free_coord;	/* free memory pointers */

 volt_sum= 0;
 volt_sumsq= 0;
 volt_maximum= -99999.;
 volt_minimum= 99999.;
 value_sum= 0;
 value_sumsq= 0;
 value_maximum= -99999.;
 value_minimum= 99999.;
 if (first_coord) {
  free_coord=first_coord;
  next_coord=first_coord->next;
  while (free_coord) {
   free((char *)free_coord);
   free_coord=next_coord;
   if (next_coord)
    next_coord=free_coord->next;
  }
 }
 first_coord=NULL;
 last_coord=NULL;
}

/***************************************************************************/

SetSetptsDims()
{
 setpts_width=(int)(xv_get(canvas[STATS],XV_WIDTH));
 setpts_height=(int)(xv_get(canvas[STATS],XV_HEIGHT));
}

/***************************  INIT_STATS()  ********************************/

init_stats()
/* initialize statistics window
*/
{
 SetSetptsDims();
 new_list();
 stats_header();
}

/*************************  SUM_STATS()  **************************************/

sum_stats(indx)
int indx;
/* accumulate values for later statistics analysis
*/
{
  double volt_arg;
  double value_arg;

  volt_arg=voltsdata[indx**len_raw+data_indx];
  volt_sum+= volt_arg;
  volt_sumsq+= volt_arg*volt_arg;
  volt_maximum=MAX(volt_arg,volt_maximum);
  volt_minimum=MIN(volt_arg,volt_minimum);

  if (current->valid_coefs) { /* there may be no history for this variable */
    value_arg=current->coef_0 + current->coef_1 * volt_arg +
         current->coef_2 * volt_arg * volt_arg;
    value_sum+= value_arg;
    value_sumsq+= value_arg*value_arg;
    value_maximum=MAX(value_arg,value_maximum);
    value_minimum=MIN(value_arg,value_minimum);
    }

}
 
/************************  LIST_SETPTS()  *********************************/

list_setpts()
/* debugging aid-- list set points
*/
{
struct set_stats *top;
 top=first_setpt;
 while (top) {
  printf("%f %f\n",top->set,top->volt_avg);
  top=top->next;
 }
}

/************************  ADD_SETPT()  *********************************/

add_setpt()
/* add new set point to list
*/
{
struct coordinates *fill_coord,*next_coord;	
float count,mean,arg,sigma;

 if (!last_setpt) {
  last_setpt=(struct set_stats *)(malloc(sizeof(struct set_stats)));
  page_top=first_setpt=last_setpt;
  last_setpt->previous=NULL;
 } else {
  last_setpt->next=(struct set_stats *)(malloc(sizeof(struct set_stats)));
  last_setpt->next->previous=last_setpt;
  last_setpt=last_setpt->next;
 }
 count = (float)num_sex;
 mean = volt_sum/count;
 arg = ABS((volt_sumsq-volt_sum*volt_sum/count)/(count-1.));
 sigma = sqrt((double)arg);
 last_setpt->volt_avg=mean;
 last_setpt->volt_sigma=sigma;
 last_setpt->volt_maximum=volt_maximum;
 last_setpt->volt_minimum=volt_minimum;

 if (current->valid_coefs) { /* there may be no history for this variable */
  count = (float)num_sex;
  mean = value_sum/count;
  arg = ABS((value_sumsq-value_sum*value_sum/count)/(count-1.));
  sigma = sqrt((double)arg);
  last_setpt->value_avg=mean;
  last_setpt->value_sigma=sigma;
  last_setpt->value_maximum=value_maximum;
  last_setpt->value_minimum=value_minimum;
 }
 last_setpt->set=set_point;
 last_setpt->next=NULL;
/*
transfer coordinate values from coord list (starting at first_coord)
to setpoint struct
*/
 next_coord=first_coord;
 while (next_coord) {
  if (next_coord==first_coord) {
/*
allocate initial segment of setpoint struct's list of coords, set fill pointer
to its coord struct
*/
   last_setpt->coords=
    (struct coordinates *)(malloc(sizeof(struct coordinates)));
   fill_coord=last_setpt->coords;
  } else {
/* 
allocate a new segment, adjust fill pointer
*/
   fill_coord->next=
    (struct coordinates *)(malloc(sizeof(struct coordinates)));
   fill_coord=fill_coord->next;
  }
/*
give fill pointer the values, in effect making the transfer
*/
  fill_coord->x=next_coord->x;
  fill_coord->y=next_coord->y;
  next_coord=next_coord->next;
 }
 fill_coord->next=NULL;
}

/***************************  STATOUT()  ***********************************/

statout()
/* create new set point entry and list it to setpts window
*/
{

 if (!(menu_selected)) 
  return;

 add_setpt();

 if (current_yrow + ENTRY_HEIGHT >= MAX_Y){
/* make room on display by scrolling up and listing new point; note that if 
   the display has already been scrolled down, the new entry will NOT be 
   written to the screen but the screen will scroll up anyway as feedback to 
   user
 */
  (void)scroll_up();
/* the list has been scrolled up, so hilight the entry beneath the cursor if
   one was highlighted before the relist and there is one beneath the cursor
*/
  if (hilited_setpt && hilited_setpt->next)
   highlight_setpt_label(hilited_setpt->next->y_bottom);
 } else { 
  (void)print_set_stat(last_setpt,FOREGROUND,current_yrow);
  current_yrow += ENTRY_HEIGHT;
  last_setpt->y_bottom=current_yrow;
 }
}


/**********************  PRINT_SET_STATS()  *********************************/

print_set_stat(member,op,top_y)
struct set_stats *member;
int op;     /* either PIX_SRC or PIX_SRC|PIX_COLOR() */
int top_y;  /* value of y-coordinate of top of entry 'box' */
/* if member is not NULL and top_y is within bounds -- i.e., there is room on
   the display screen, write out a display entry with its values
*/
{
  static char string[128];
 
  if (!member) 
   return(FALSE);

  if (top_y + ENTRY_HEIGHT >= MAX_Y)
   return(FALSE);

  (void)sprintf (string,"%9.2f%9.3f%9.3f%11.2f%11.2f",
   (float)member->set,(float)member->volt_avg,(float)member->volt_sigma,
   (float)member->volt_maximum,(float)member->volt_minimum);
  if (op==FOREGROUND) {
   (*SetContext[GetDrawType()])(pw_xid[STATS],GXcopy,BLACK,0,0,
    GetFontID(GetCalibDisplayFont()));  
   (*DrawText[GetDrawType()])(HDR_X,top_y+SET_GAP,string);
  } else {
   (*SetContext[GetDrawType()])(pw_xid[STATS],GXcopy,REVERSE_VIDEO,0,0,
    GetFontID(GetCalibDisplayFont()));  
   (*DrawTtext[GetDrawType()])(HDR_X,top_y+SET_GAP,string);
  }
  if (current->valid_coefs) {
   (void)sprintf (string,"%18.3f%9.3f%11.2f%11.2f",
   (float)member->value_avg,(float)member->value_sigma,
   (float)member->value_maximum,(float)member->value_minimum);
  } else
   (void)sprintf (string,"               (No Previous Coefficients Available)");
  if (op==FOREGROUND) {
   (*SetContext[GetDrawType()])(pw_xid[STATS],GXcopy,BLACK,0,0,
    GetFontID(GetCalibDisplayFont()));  
   (*DrawText[GetDrawType()])(HDR_X,top_y+ENTRY_HEIGHT,string);
  } else {
   (*SetContext[GetDrawType()])(pw_xid[STATS],GXcopy,REVERSE_VIDEO,0,0,
    GetFontID(GetCalibDisplayFont()));  
   (*DrawTtext[GetDrawType()])(HDR_X,top_y+ENTRY_HEIGHT,string);
   }
  return(TRUE);
}

/*************************  HIGHLIGHT_SETPT_LABEL()  *************************/
 
highlight_setpt_label(ybot)
int ybot;
/* highlight the set point entry whose y_bottom coordinate matches ybot
*/
{

 hilited_setpt=find_setpt(ybot);
 if (!hilited_setpt)
  return;
 (*FillArea[GetDrawType()])(pw_xid[STATS],0, ybot-(ENTRY_HEIGHT)+DESC,
  setpts_width,ENTRY_HEIGHT,1);

 (void)print_set_stat(hilited_setpt,REVERSE_VIDEO,ybot-(ENTRY_HEIGHT));
}
 
/****************************  CLEAR_SETPT_LABEL()  **************************/
 
clear_setpt_label(ybot)
int ybot;
/* clear the set point entry whose y_bottom coordinate matches ybot
*/
{
 (*FillArea[GetDrawType()])(pw_xid[STATS],0,ybot-(ENTRY_HEIGHT)+DESC,
  setpts_width,ENTRY_HEIGHT, 0);
 (void)print_set_stat(find_setpt(ybot),FOREGROUND,ybot-(ENTRY_HEIGHT));
 hilited_setpt=NULL;
}
 
/****************************  FIND_SETPT()  **********************************/

struct set_stats 
*find_setpt(ybottom)
int ybottom;
/* find the set point entry that is visible on screen with screen y-coordinate
   of ybottom
*/
{
static struct set_stats *setpt_number;

 setpt_number=page_top;
 while (setpt_number) {
  if (ybottom == setpt_number->y_bottom )
   return(setpt_number);
  setpt_number=setpt_number->next;
 }
 return(NULL);
}

/*************************  EVENT_IN_STATS_WINDOW ()  *************************/ 
event_in_setpts_window(quadrant)
int quadrant;
{
 if (!canvas[quadrant])
  return FALSE;
 return (quadrant == STATS);
}

/*************************  CURRENT_SETPT()  *********************************/

current_setpt(y)
int y;
/* return the y_bottom coordinate of currently highlighted set point; if none,
   return 0, a coordinate not useable as that y_bottom.
*/
{
static struct set_stats *setpt_number;

 hilited_setpt=NULL;
 setpt_number=page_top;
 while (setpt_number) {
  if (y > HEADER_BOTTOM && y <= setpt_number->y_bottom ) {
   hilited_setpt=setpt_number;
   return(setpt_number->y_bottom);
  }
  setpt_number=setpt_number->next;
 }
 return(0);
} 

/************************  NEW_LIST()  **************************************/

new_list()
/* initialize a new set point list 
*/
{
 if (first_setpt) {
  free_setpt=first_setpt;
  next_setpt=first_setpt->next;
 }
 while (free_setpt) {
  free((char *)free_setpt);
  free_setpt=next_setpt;
  if (next_setpt)
   next_setpt=free_setpt->next;
 }
 page_top=last_setpt=first_setpt=NULL; 
 clear_setpts_window();
 clear_calcoes();
}


/************************  CLEAR_CALCOES()  **********************************/

clear_calcoes()
{
 (*FillArea[GetDrawType()])(pw_xid[STATS],0,setpts_height-CALCOE_HT,setpts_width,CALCOE_HT,0);
}

/*******************  CLEAR_STATS_WINDOW()  **********************************/

clear_setpts_window()
/* clear out all set point entries
*/
{
 current_yrow=HEADER_BOTTOM;
 (*FillArea[GetDrawType()])(pw_xid[STATS],0,HEADER_BOTTOM,setpts_width,setpts_height-HEADER_BOTTOM-CALCOE_HT, 0);
}


/***********************  STATS_HEADER()  ***********************************/

stats_header()
/* write out the header in the setpts window
*/
{
 (*FillArea[GetDrawType()])(pw_xid[STATS],0,0,setpts_width,HEADER_BOTTOM,
  0);
 (*SetContext[GetDrawType()])(pw_xid[STATS],GXcopy,BLACK,0,0,
    GetFontID(GetCalibDisplayFont()));  
 (*DrawText[GetDrawType()])(HDR_X,HDR_Y2,
  "SET POINT    VOLTS    SIGMA    MAXIMUM    MINIMUM");
 (*DrawText[GetDrawType()])(HDR_X,HEADER_BOTTOM,
  "             VALUE    SIGMA    MAXIMUM    MINIMUM");
}

/**************************  RELIST_POINTS()  *********************************/

relist_points()
/* called when a new point is to be listed and either maximum # of points have 
   been listed or a point has been deleted
*/
{
 static struct set_stats *list;

  list=page_top;
  clear_setpts_window();
  while (print_set_stat(list,FOREGROUND,current_yrow)) {
/* reset y-coords for relisted entries */
   current_yrow += ENTRY_HEIGHT;
   list->y_bottom=current_yrow;
   list=list->next;
  } 
}

/***************************  REGRESS_STATS()  ******************************/

double volt[MAXSIZE];
float point[MAXSIZE];
int npts,iorder;

regress_stats()
/* calculate and display calibration coefficients as function of set point
   and voltage averages
*/
{ 
  float det,detn;
  float cals[MAXSIZE];
  struct set_stats *ptr;
  char string[50];

/* Stuff stat info from linked list into arrays 'volt' and 'point'. */

 if (!current)
   return;

  npts=0;
  ptr = first_setpt;
  while (ptr) {
	volt[npts] = ptr->volt_avg;
	point[npts++] = ptr->set;
	ptr = ptr->next;
	}
  iorder = 3;  /* 2nd order coefficients are all that are ever used */
  leastsq(volt,point,iorder,npts,&detn,&det,cals);
  coeff(cals,iorder,volt,point,npts,&r,&erms,&emax);

  current->valid_coefs=TRUE;
  current->coef_0=cals[0];
  current->coef_1=cals[1];
  current->coef_2=cals[2];

/* display calcoes in plots and setpts windows */

  (*SetContext[GetDrawType()])(pw_xid[STATS],GXcopy,BLACK,0,0,
    GetFontID(GetCalibDisplayFont()));  
  (*DrawText[GetDrawType()])(HDR_X,MAX_Y+ROW_GAP,"CALIBRATION COEFFICIENTS:");
  (void)sprintf(string,"C0: %9.4f C1: %9.4f C2: %9.4f",current->coef_0,
		current->coef_1,current->coef_2);
  (*DrawText[GetDrawType()])(HDR_X,MAX_Y+2*ROW_GAP+GetFontHeight(GetCalibDisplayFont()),string);
  (*DrawText[GetDrawType()])(HDR_X,MAX_Y+3*ROW_GAP+2*GetFontHeight(GetCalibDisplayFont()),"ERROR STATISTICS:");
  (void)sprintf(string,"R2: %9.4f RMS: %9.4f MAX: %9.4f",r,erms,emax);
  (*DrawText[GetDrawType()])(HDR_X,MAX_Y+4*ROW_GAP+3*GetFontHeight(GetCalibDisplayFont()),string);

  redraw_calib_curve(); /* clear out any previous fitted curve */
  fitted_curve(PLOTS,cals[0],cals[1],cals[2]);

  print_entry(); /* Prints summary page with stats on last variable calibrated */

}

/***************************  SCROLL_UP()  **********************************/

scroll_up()
/* scroll stats window list up
*/
{
 if (page_top && page_top!=last_setpt) {
  page_top=page_top->next;
  relist_points();
  return(TRUE);
 }
 return(FALSE);
}

/***************************  SCROLL_DOWN()  **********************************/

scroll_down()
/* scroll stats window list down
*/
{
 if (page_top && page_top->previous){
  page_top=page_top->previous;
  relist_points();
  return(TRUE);
 }
 return(FALSE);
}

/*************************  START_CALIB_CURVE()  ***************************/

start_calib_curve()
{
 if (init_calib_curve())
  need_calib_curve=num_sex;
}

/*************************  REDO_LAST_ENTRY()  ***************************/

redo_last_entry()
/* delete the last entry in the list of set points, and re-do it
*/
{
 if (delete_setpt(last_setpt)) {
  if (init_calib_curve()) {
   need_calib_curve=num_sex;
   redraw_calib_curve();
   need_redo_last=TRUE;
  }
 }
}

/*************************  DELETE_HILITED_ENTRY()  ***************************/

delete_hilited_entry()
/* delete the currently highlighted set point entry
*/
{
 if (delete_setpt(hilited_setpt)) {
  redraw_calib_curve();
  relist_points();
 }
}

/****************************  DELETE_SETPT()  ********************************/

delete_setpt(dead_meat)
struct set_stats *dead_meat;
/* remove the set point entry pointed to by dead_meat from the list of
   entries
*/
{

 if (!dead_meat)
  return(FALSE);

/* reroute pointers to detour around deletee */

 if (dead_meat->previous)
  dead_meat->previous->next=dead_meat->next;
 if (dead_meat->next)
  dead_meat->next->previous=dead_meat->previous;

/* protect special pointers if their members are marked for death */

 if (first_setpt==dead_meat) {  /* start of list */
  first_setpt=first_setpt->next;
  if (first_setpt)
   first_setpt->previous=NULL;
 } 

 if (last_setpt==dead_meat) {  /* end of list */
  last_setpt=last_setpt->previous;
  if (last_setpt)
   last_setpt->next=NULL;
 }

/* if there are any scrolled off top entries, bring last one back to screen */

 if (page_top->previous)
  page_top=page_top->previous;
 else

/* move top of display page down one if first entry is on death row */

  if (page_top==dead_meat) 
   page_top=page_top->next;

/* kill it */

  free((char *)(dead_meat));
  return(TRUE);
}

/******************************  SAVE_COORDS()  ******************************/

save_coords(x,y)
int x,y;
/* save the device coordinates for all set point vs. voltages for current
   set point being calibrated, in case any set points are deleted and a 
   subsequent redraw is necessary.
*/
{
 if (!last_coord) {
  last_coord=(struct coordinates *)(malloc(sizeof(struct coordinates)));
  first_coord=last_coord;
 } else {
  last_coord->next=(struct coordinates *)(malloc(sizeof(struct coordinates)));
  last_coord=last_coord->next;
 }
 last_coord->next=NULL;
 last_coord->x=x;
 last_coord->y=y;
}

/******************************  REDRAW_CALIB_CURVE() *************************/

redraw_calib_curve()
/* clear out previous set point scatter plot and redraw with what is probably
   a new list of coordinates (after a deletion) or simply to refresh the
   plot frame prior to a fitted curve drawing
*/
{
static struct set_stats *list;
static struct coordinates *list_coords;

 clear_plot_frame(PLOTS);
 list=first_setpt;
 while (list) {
  list_coords=list->coords;
  while (list_coords) {
/***
printf("draw x at %d %d for setpt %f\n",list_coords->x,list_coords->y,list->value_avg);
***/
   draw_x(PLOTS,list_coords->x,list_coords->y,BLU,GXcopy,2);
   list_coords=list_coords->next;
  }
  list=list->next; 
 }
}

/******************************  PRINT_ENTRY() *************************/

print_entry()
{
  struct set_stats *print_it;
  FILE	*fp;

  if ((fp = fopen("regressPrint", "w+")) == NULL)
    perror("calib: print_entry: open failed");

  if (!current || get_lstat_port()==ERROR)
    return;

  fprintf(fp, "\n\n\n\n---------------------------------------------------------------------------\n");

  fprintf(fp,"Project %s (%s)      %s      %s\n",
	(char *)get_proj_number(),(char *)GetWINDSProjectName(),
	(char *)GetWINDSAircraftName(),(char *)get_date());

  fprintf(fp, "  PARAMETER CALIBRATED: %s           SENSOR TYPE: %s\n",
	current->calib_name,current->sensor_type);

  fprintf(fp,
	"  SENSOR SERIAL NUMBER: %s         CONDITIONER SERIAL NUMBER: %s \n",
	current->sensor_sn,current->cond_sn);

  fprintf(fp,
	"  AMPLIFIER GAIN: %s                   AMPLIFIER CUTOFF FREQUENCY: %s \n",
	current->amp_gain,current->amp_cut_off);

  fprintf(fp,
	"  ADS ADDRESS: %s                      SUMMATION INTERVAL: %d \n",
	current->ads_address,num_sex);

  fprintf(fp, "  CALIBRATION DONE BY: %s \n",current->technician);

  fprintf(fp, "---------------------------------------------------------------------------\n\n\n");

  fprintf(fp, "SET POINT    VOLTS    SIGMA    MAXIMUM    MINIMUM\n");
  fprintf(fp, "             VALUE    SIGMA    MAXIMUM    MINIMUM\n\n");

  for (print_it = first_setpt; print_it; print_it = print_it->next)
    {
    fprintf(fp,"%9.2f%9.3f%9.3f%11.2f%11.2f\n",
    (float)print_it->set,(float)print_it->volt_avg,(float)print_it->volt_sigma,
    (float)print_it->volt_maximum,(float)print_it->volt_minimum);

    if (current->valid_coefs) {
      fprintf(fp,"%18.3f%9.3f%11.2f%11.2f\n\n",
		(float)print_it->value_avg,(float)print_it->value_sigma,
		(float)print_it->value_maximum,(float)print_it->value_minimum);
      fprintf(fp, "  NO PREVIOUS COEFFICIENTS WERE AVAILABLE  \n\n");
      }
    }

  fprintf(fp,
	"\n  NEW CALIBRATION COEFFICIENTS:\n    C0: %9.4f C1: %9.4f C2: %9.4f\n\n",
	(float)current->coef_0,(float)current->coef_1,(float)current->coef_2);

  fprintf(fp, "  ERROR STATISTICS:\n    R2: %9.4f RMS: %8.4f MAX: %8.4f\n\f", r,erms,emax);

  fclose(fp);
  system("/home/users/ads/my_lp -h -r -Plp regressPrint");

}

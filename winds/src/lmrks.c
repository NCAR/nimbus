/* lmrks.c -- draw in landmarks as designated in database
*/
#include <stdio.h>
#include <xview/xview.h>

#include <lengths.h>
#include <constants.h>
#include <track.h>
#include <macros.h>
#include <display.h>
#include <colors.h>
#include <help.h>
#include <proto.h>

static float *landmark_data;
static char *landmark_label,
            *Landmark_filename;
static int total_landmarks;
static int *hidden_lmark;
static FILE *append_lmrk;
static int hilited_lmark=ERROR;
static int current_lmark=ERROR;
static int showing_deleted[NUM_DISPLAY_QUADRANTS]=
{FALSE,FALSE,FALSE,FALSE};
static int dir,ascent,descent;
static XCharStruct overall;

struct bounds  {
 int x0;
 int y0;
 int x1;
 int y1;
} *lmark_area[NUM_DISPLAY_QUADRANTS];

/************************  INIT_LANDMARK_DATA()  *****************************/

init_landmark_data(landmark_filename)
char *landmark_filename;
{
FILE *WINDSfopen(),*landmarks;
float value;
char label[LMRK_LBL_LNGTH];
char temp_string[LMRK_LBL_LNGTH*2];
int data_pts;
int quadrant,dummy;

 if (!Landmark_filename)
  Landmark_filename=(char *)strdup(landmark_filename);
 if ( (landmarks=WINDSfopen(landmark_filename,"r")) == NULL) {
  (void)fprintf(stderr,"no landmark data present\n");
  return;
 }
/* 
size data and label arrays
*/
 if (landmark_data) {
  (void)free((char *)landmark_data);
  landmark_data=0;
 }
 if (landmark_label) {
  (void)free((char *)landmark_label);
  landmark_label=0;
 }
 if (hidden_lmark) {
  (void)free((char *)hidden_lmark);
  hidden_lmark=0;
 }
 for (quadrant=0; quadrant<get_num_displays(); quadrant++)
  if (lmark_area[quadrant]) {
   (void)free((char *)lmark_area[quadrant]);
  lmark_area[quadrant]=0;
 }
/* 
read through file to determine size of data array needed and indexing
information
*/
 data_pts=0;
 while (fscanf(landmarks,"%f",&value)!=EOF) {
  (void)fscanf(landmarks,"%f %s %d",&value,&temp_string[0],&dummy);
  (void)strncpy(label,temp_string,LMRK_LBL_LNGTH-1);
  label[LMRK_LBL_LNGTH-1]='\0'; 
  data_pts+=PTS_PER_COORD;
 }
/* 
reset file to start
*/
 (void)WINDSfclose(landmarks);
/*
allocate as needed
*/
 if (data_pts) {
  landmark_data=(float *)malloc((unsigned int)data_pts*sizeof(float));
  landmark_label=(char *)malloc((unsigned int)
            (data_pts/PTS_PER_COORD)*sizeof(char)*LMRK_LBL_LNGTH);
  for (quadrant=0; quadrant<get_num_displays(); quadrant++) {
   lmark_area[quadrant]=(struct bounds *)malloc((unsigned int)
            (data_pts/PTS_PER_COORD)*sizeof(struct bounds));
  }
  hidden_lmark=(int *)malloc((unsigned int)(data_pts/PTS_PER_COORD)*sizeof(int));
/* 
read in data to permanent array
*/
  total_landmarks=0;
  landmarks=WINDSfopen(landmark_filename,"r");
  while (fscanf(landmarks,"%f",&value)!=EOF) {
   landmark_data[total_landmarks*PTS_PER_COORD+LAT_COORD]=value;
   (void)fscanf(landmarks,"%f %s %d",
    &landmark_data[total_landmarks*PTS_PER_COORD+LON_COORD],
    &landmark_label[total_landmarks*LMRK_LBL_LNGTH],
    &hidden_lmark[total_landmarks]
   );
   total_landmarks++;
  }
  (void)WINDSfclose(landmarks);
 }
}

/************************  SAVE_LMARK_SETUP()  *****************************/

save_lmark_setup()
{
FILE *WINDSfopen(),*landmarks;
int lmark;

 landmarks=WINDSfopen(Landmark_filename,"w");
 for (lmark=0; lmark<total_landmarks; lmark++) {
  (void)fprintf(landmarks,"%f %f %s %d\n",
   landmark_data[lmark*PTS_PER_COORD+LAT_COORD],
   landmark_data[lmark*PTS_PER_COORD+LON_COORD],
   &landmark_label[lmark*LMRK_LBL_LNGTH],
   hidden_lmark[lmark]
  );
 }
 (void)WINDSfclose(landmarks);
}

/************************  ADD_LANDMARK()  ********************************/

add_landmark(quadrant,lat,lon,label)
int quadrant;
double lat,lon;
char *label;
{
int x,y;
FILE *WINDSfopen();
/* add new landmark to file
*/
 if ( (append_lmrk=WINDSfopen(Landmark_filename,"a")) == NULL) {
  time_to_log();
  (void)fprintf(stderr,"WARNING: can't append to landmark file\n");
  return;
 }
 (void)fprintf(append_lmrk,"%f %f %s %d\n",lat,lon,label,FALSE);
 (void)WINDSfclose(append_lmrk);
/* 
add new landmark to memory store 
*/
 init_landmark_data(Landmark_filename);
/* 
draw new landmark to display
*/
 y=get_y_coord(quadrant,0,lat);
 x=get_x_coord(quadrant,lon);
 draw_landmark_and_label(quadrant,GXxor,
  rleft[quadrant]+x,rtop[quadrant]+y,label,
  LMARK_COLOR,BKGND);
 XTextExtents((XFontStruct *)
  GetFontInfoStruct(GetLandmarkFont(quadrant)),
  &landmark_label[(total_landmarks-1)*LMRK_LBL_LNGTH],
  strlen(&landmark_label[(total_landmarks-1)*LMRK_LBL_LNGTH]),
  &dir,&ascent,&descent,&overall);
 set_landmark_area(quadrant,total_landmarks-1,
  rleft[quadrant]+x-LMRK_LNGTH,rtop[quadrant]+y-ascent,
  2*LMRK_LNGTH+overall.width,ascent);
}

/************************  REFRESH_LANDMARKS()  ******************************/

refresh_landmarks(quadrant)
int quadrant;
{
int landmark;

 for (landmark=0; landmark<total_landmarks; landmark++) {
  if (!hidden_lmark[landmark] || showing_deleted[quadrant]) {
   draw_landmark_and_label(quadrant,GXxor,
    lmark_area[quadrant][landmark].x0+LMRK_LNGTH,
    lmark_area[quadrant][landmark].y1,
    &landmark_label[landmark*LMRK_LBL_LNGTH],
    hidden_lmark[landmark]?HIDDEN_COLOR:LMARK_COLOR,BKGND); 
  }
 }
}

/************************  DRAW_LANDMARKS()  ********************************/

draw_landmarks(quadrant)
int quadrant;
{
int x,y,landmark;
 
 for (landmark=0; landmark<total_landmarks; landmark++) {

/* assuming data is in (lat,lon) order, and that latitude is on the y-axis
*/
  y=get_y_coord(quadrant,0,landmark_data[landmark*PTS_PER_COORD+LAT_COORD]);
  x=get_x_coord(quadrant,landmark_data[landmark*PTS_PER_COORD+LON_COORD]);
  if (!hidden_lmark[landmark] || showing_deleted[quadrant]) {
   draw_landmark_and_label(quadrant,GXxor,
    x,y,&landmark_label[landmark*LMRK_LBL_LNGTH],
    hilited_lmark==landmark?HILITED_LMARK:
     (hidden_lmark[landmark]?HIDDEN_COLOR:LMARK_COLOR),BKGND); 
  }

/* want to set landmark area boundaries regardless of showing or not, in
   case of request later for showing hidden landmarks
*/
  XTextExtents((XFontStruct *)
  GetFontInfoStruct(GetLandmarkFont(quadrant)),
  &landmark_label[landmark*LMRK_LBL_LNGTH],
   strlen(&landmark_label[landmark*LMRK_LBL_LNGTH]),&dir,&ascent,&descent,
   &overall);
  set_landmark_area(quadrant,landmark,
   rleft[quadrant]+x-LMRK_LNGTH,rtop[quadrant]+y-ascent,
   2*LMRK_LNGTH+overall.width,ascent);
 }
}

/******************  DRAW_LANDMARK_AND_LABEL()  ******************************/

draw_landmark_and_label(quadrant,op,x,y,label,foreground_color,bkgnd_color)
int quadrant,x,y,foreground_color,bkgnd_color;
char *label;
{

/* even though clipping is set here, there is an Xview anomaly which must be
   protected against: if DrawTtext() is called with y=-1 (when drawing into
   a Server image), a column of volunteer garbage strings are drawn at the
   same x-coord.  Ergo, don't do anything when y is less than zero.
*/
 if (y<0)
  return;
 
 set_landmark_context(quadrant,op,foreground_color,bkgnd_color);
 (*DrawLine[GetDrawType()])(x-LMRK_LNGTH/2,y-LMRK_LNGTH/2,x+LMRK_LNGTH/2,y+LMRK_LNGTH/2);
 (*DrawLine[GetDrawType()])(x-LMRK_LNGTH/2,y+LMRK_LNGTH/2,x+LMRK_LNGTH/2,y-LMRK_LNGTH/2);
 (*DrawTtext[GetDrawType()])(x+LMRK_LNGTH,y,label);
 (*UnsetClipping[GetDrawType()])();
}

/**********************  SET_LANDMARK_AREA()  ********************************/

set_landmark_area(quadrant,which,x,y,width,height)
int quadrant,which,x,y,width,height;
{
 lmark_area[quadrant][which].x0=x;
 lmark_area[quadrant][which].y0=y;
 lmark_area[quadrant][which].x1=x+width;
 lmark_area[quadrant][which].y1=y+height;
}

/**********************  TOGGLE_SHOW_HIDDEN()  *****************************/

toggle_show_hidden(quadrant)
int quadrant;
{
 if (showing_deleted[quadrant]) {
  hide_deleted_lmrks(quadrant);
  refresh_landmarks(quadrant);
  refresh_boundaries(quadrant);
 } else
  show_deleted_lmrks(quadrant);
}

/**********************  SHOW_DELETED_LMRKS()  *******************************/

show_deleted_lmrks(quadrant)
int quadrant;
{
int lmark;

 for (lmark=0; lmark<total_landmarks; lmark++) {
  if (hidden_lmark[lmark]) {
   draw_landmark_and_label(quadrant,GXxor,
    lmark_area[quadrant][lmark].x0+LMRK_LNGTH,
    lmark_area[quadrant][lmark].y1,
    &landmark_label[lmark*LMRK_LBL_LNGTH],
    HIDDEN_COLOR,BKGND);
  }
 }
 showing_deleted[quadrant]=TRUE;
}

/**********************  HIDE_DELETED_LMRKS()  *******************************/

hide_deleted_lmrks(quadrant)
int quadrant;
{
int lmark;

 for (lmark=0; lmark<total_landmarks; lmark++) {
  draw_landmark_and_label(quadrant,GXxor,
   lmark_area[quadrant][lmark].x0+LMRK_LNGTH,
   lmark_area[quadrant][lmark].y1,
   &landmark_label[lmark*LMRK_LBL_LNGTH],
   hidden_lmark[lmark]?HIDDEN_COLOR:LMARK_COLOR,BKGND);
 }
 showing_deleted[quadrant]=FALSE;
}

/**********************  UPDATE_HILITED_LANDMARK()  **************************/

update_hilited_landmark(quadrant,x,y)
int quadrant,x,y;
{
int lmark;

/* check first if this is in the same area as any previously hilited landmark;
   if so, do nothing, else clear that hilite.  Return TRUE if a landmark is
   hilited, else return FALSE.
*/
 if (hilited_lmark!=ERROR && 
     within_boundaries(x,y,
      rleft[quadrant],rtop[quadrant],rright[quadrant],rbottom[quadrant]) &&
     within_boundaries(x,y,
      lmark_area[quadrant][hilited_lmark].x0,
      lmark_area[quadrant][hilited_lmark].y0,
      lmark_area[quadrant][hilited_lmark].x1,
      lmark_area[quadrant][hilited_lmark].y1)) 
  return hilited_lmark!=ERROR;
 else
  clear_hilited_landmark(quadrant);

/* check if cursor is in any other landmark's area
*/
 current_lmark=ERROR;
 for (lmark=0; lmark<total_landmarks; lmark++) {
  if (within_boundaries(x,y,
       rleft[quadrant],rtop[quadrant],rright[quadrant],rbottom[quadrant]) &&
      within_boundaries(x,y,
       lmark_area[quadrant][lmark].x0,
       lmark_area[quadrant][lmark].y0,
       lmark_area[quadrant][lmark].x1,
       lmark_area[quadrant][lmark].y1)) {

/* got one: verify not deleted, or, if it is, that deleted landmarks are 
   showing
*/
   if ((!hidden_lmark[lmark] || showing_deleted[quadrant])) {

/* hilite new one.
*/
    draw_landmark_and_label(quadrant,GXxor,
     lmark_area[quadrant][lmark].x0+LMRK_LNGTH,
     lmark_area[quadrant][lmark].y1,
     &landmark_label[lmark*LMRK_LBL_LNGTH],
     HILITED_LMARK,hidden_lmark[lmark]?HIDDEN_COLOR:LMARK_COLOR);
    current_lmark=hilited_lmark=lmark;
    chg_tb_help_image(LANDMARK_OPS_AREA);
    update_leftop(quadrant,FALSE);
    return TRUE;
   } 
  } 
 }
 return hilited_lmark!=ERROR;
}

/************************  CLEAR_HILITED_LANDMARK()  **************************/

clear_hilited_landmark(quadrant)
int quadrant;
{
 if (hilited_lmark!=ERROR && 
        (!hidden_lmark[hilited_lmark] || showing_deleted[quadrant])
    ) {
  draw_landmark_and_label(quadrant,GXxor,
   lmark_area[quadrant][hilited_lmark].x0+LMRK_LNGTH,
   lmark_area[quadrant][hilited_lmark].y1,
   &landmark_label[hilited_lmark*LMRK_LBL_LNGTH],
   HILITED_LMARK,hidden_lmark[hilited_lmark]?HIDDEN_COLOR:LMARK_COLOR);
  hilited_lmark=ERROR;
  chg_tb_help_image(PLOT_FRAME_AREA);
  update_leftop(quadrant,FALSE);
 }
}

/************************   HIDE_LANDMARK()  ********************************/

hide_landmark(quadrant)
int quadrant;

/* hide designated landmark in current quadrant only; it remains visible in
   other quadrants (if already showing) until that window is modified.  This
   allows users ability to vary landmark mappings from window to window.
*/
{
 if (current_lmark==ERROR) 
  return;

/* first undraw any hilited color to restore original color.  Even cursor is 
   over a landmark, it may not be highlighted since restoring hidden landmarks
   turns them GREEN and then deleting them again (w/o moving cursor) turns
   them back to RED.
*/
 if (hilited_lmark!=ERROR)
  draw_landmark_and_label(quadrant,GXxor,
   lmark_area[quadrant][hilited_lmark].x0+LMRK_LNGTH,
   lmark_area[quadrant][hilited_lmark].y1,
   &landmark_label[hilited_lmark*LMRK_LBL_LNGTH],
   HILITED_LMARK,hidden_lmark[hilited_lmark]?HIDDEN_COLOR:LMARK_COLOR);

 if (hidden_lmark[current_lmark]) {

/* restore a hidden landmark: first, hide it to get to square one
*/
  draw_landmark_and_label(quadrant,GXxor,
   lmark_area[quadrant][current_lmark].x0+LMRK_LNGTH,
   lmark_area[quadrant][current_lmark].y1,
   &landmark_label[current_lmark*LMRK_LBL_LNGTH],
   HIDDEN_COLOR,BKGND);

/* then, draw it in normal color (not hilite color, even though cursor is 
   still over it, to provide feedback to user that it was restored)
*/
  draw_landmark_and_label(quadrant,GXxor,
   lmark_area[quadrant][current_lmark].x0+LMRK_LNGTH,
   lmark_area[quadrant][current_lmark].y1,
   &landmark_label[current_lmark*LMRK_LBL_LNGTH],
   LMARK_COLOR,BKGND);
 } else if (showing_deleted[quadrant]) {

/* a non-hidden landmark is to be hidden while hidden landmarks are showing; 
   instead of simply disappearing, it goes to red.
*/
  draw_landmark_and_label(quadrant,GXxor,
   lmark_area[quadrant][current_lmark].x0+LMRK_LNGTH,
   lmark_area[quadrant][current_lmark].y1,
   &landmark_label[current_lmark*LMRK_LBL_LNGTH],
   LMARK_COLOR,BKGND);
  draw_landmark_and_label(quadrant,GXxor,
   lmark_area[quadrant][current_lmark].x0+LMRK_LNGTH,
   lmark_area[quadrant][current_lmark].y1,
   &landmark_label[current_lmark*LMRK_LBL_LNGTH],
   HIDDEN_COLOR,BKGND);
 } else {

/* hide a normally showing landmark; it simply disappears
*/
  draw_landmark_and_label(quadrant,GXxor,
   lmark_area[quadrant][current_lmark].x0+LMRK_LNGTH,
   lmark_area[quadrant][current_lmark].y1,
   &landmark_label[current_lmark*LMRK_LBL_LNGTH],
   LMARK_COLOR,BKGND);
 }
 
/* toggle hidden status of current landmark; in any case, no landmark is 
   actually highlighted anymore, so adjust that parameter.
*/
 hidden_lmark[current_lmark]=!hidden_lmark[current_lmark];
 hilited_lmark=ERROR;

 if ((hidden_lmark[current_lmark] && !showing_deleted[quadrant])) 

/* set current landmark to ERROR so that next entry to this routine will 
   simply return with no action.  This prevents user from hiding a landmark
   (while not showing hidden ones) and then clicking RIGHT again w/o moving
   cursor to unhide it -- can cause color confusion.
*/
  current_lmark=ERROR;

/* save new hide-show configuration
*/
 save_lmark_setup();
}

/*
winds.c -

get and set operations on winds parameters
*/

#include <xview/xview.h>
#include <xview/canvas.h>
#include <xview/panel.h>

#include <constants.h>
#include <display.h>
#include <files.h>
#include <raf.h>
#include <lengths.h>
#include <proto.h>

Canvas canvas[NUM_WINDOWS];             /* data display canvasses */
int window_type[NUM_WINDOWS];           /* window types */

static char WINDSProjectName[32];

/*****************************************************************************/

CheckArrayBound(client,arraysize,arraytypesize,index)
char *client;
int arraysize;
int arraytypesize;
int index;
{
 if (index<0 || index > ((arraysize/arraytypesize)-1)) {
  fprintf(stderr, "WARNING: out of bounds index in %s: %d\n", client, index);
  return ERROR;
 }
 return OK;
}

/*****************************************************************************/

Canvas
GetWINDSCanvas(quadrant)
int quadrant;
{
 if (CheckArrayBound("GetWINDSCanvas",sizeof(canvas),sizeof(Canvas),quadrant)
  ==ERROR) {
  return (Canvas)ERROR;
 }
 return (Canvas)canvas[quadrant];
}

/*****************************************************************************/

SetWINDSShowWindow(quadrant,show)
int quadrant,show;
{
 if (CheckArrayBound("SetWINDSShowWindow (canvas)",
  sizeof(canvas),sizeof(Canvas),quadrant)==ERROR) {
  return;
 }
 if (canvas[quadrant])
  xv_set(canvas[quadrant],XV_SHOW,show,NULL);
 if (CheckArrayBound("SetWINDSShowWindow (op_panel)",
  sizeof(op_panel),sizeof(Panel),quadrant)==ERROR) {
  return;
 }
 if (op_panel[quadrant])
  xv_set(op_panel[quadrant],XV_SHOW,show,NULL);
}

/*****************************************************************************/

SetWINDSSnapshot(quadrant,snap)
int quadrant;
{
 if (CheckArrayBound("SetWINDSSnapshot",sizeof(take_snapshot),sizeof(int),
  quadrant)==ERROR) {
  return;
 }
 take_snapshot[quadrant]=snap;
}

/*****************************************************************************/

SetWINDSColorSnapshot(quadrant,snap)
int quadrant;
{
 if (CheckArrayBound("SetWINDSColorSnapshot",sizeof(take_color_snapshot),
  sizeof(int),quadrant)==ERROR) {
  return;
 }
 take_color_snapshot[quadrant]=snap;
}

/*****************************************************************************/

GetWINDSWindowWidth(quadrant)
int quadrant;
{
 if (CheckArrayBound("GetWINDSWindowWidth",sizeof(window_width),
  sizeof(int),quadrant)==ERROR) {
  return ERROR;
 }
 return window_width[quadrant];
}

/*****************************************************************************/

GetWINDSWindowHeight(quadrant)
int quadrant;
{
 if (CheckArrayBound("GetWINDSWindowHeight",sizeof(window_height),
  sizeof(int),quadrant)==ERROR) {
  return ERROR;
 }
 return window_height[quadrant];
}

/*****************************************************************************/

GetWINDSWindowX(quadrant)
int quadrant;
{
 if (CheckArrayBound("GetWINDSWindowX",sizeof(window_x),
  sizeof(int),quadrant)==ERROR) {
  return ERROR;
 }
 return window_x[quadrant];
}

/*****************************************************************************/

GetWINDSWindowY(quadrant)
int quadrant;
{
 if (CheckArrayBound("GetWINDSWindowY",sizeof(window_y),
  sizeof(int),quadrant)==ERROR) {
  return ERROR;
 }
 return window_y[quadrant];
}

/*****************************************************************************/

GetWINDSWindowType(quadrant)
int quadrant;
{
 if (CheckArrayBound("GetWINDSWindowType",sizeof(window_type),
  sizeof(int),quadrant)==ERROR) {
  return ERROR;
 }
 return window_type[quadrant];
}

/*****************************************************************************/

char *
GetWINDSProjectName()
{
static int GotName=FALSE;
static char *ProjNum;

 if (!GotName || 
/*
project name has yet to be initialized
*/
  (ProjNum && strcmp(ProjNum,(char *)get_proj_number()))) {
/* 
project name & number init'd but user may have since changed to 
different project 
*/
  char filename[MAX_FLNM_LNGTH];
  FILE *fd;
  if (ProjNum)
   (void)free(ProjNum);
  ProjNum=(char *)strdup((char *)get_proj_number());
  (void)sprintf(filename,PROJECTNAME,ProjectDirectory,ProjNum);
  if ( (fd=(FILE *)WINDSfopen(filename,"r"))==NULL) {
   (void)sprintf(WINDSProjectName,"(No Name)");
  } else {
   (void)fscanf(fd,"%s",WINDSProjectName);  
   WINDSfclose(fd);
  }
  GotName=TRUE;
 }
 return WINDSProjectName;
}

/***********************  GetWINDSAircraftName()  ****************************/

char *
GetWINDSAircraftName()
{
char *prnum;

 prnum=(char *)strdup((char *)get_proj_number());
 switch (prnum[0]) {
  case PROJ_ELECTRA:
   return ELECTRA_NAME;
  case PROJ_KINGAIR:
   return KINGAIR_NAME;
  case PROJ_SABRELINER:
   return SABRELINER_NAME;
  case PROJ_C130:
   return C130_NAME;
  case PROJ_SAILPLANE:
   return SAILPLANE_NAME;
 }
 return "Unknown Aircraft";
}

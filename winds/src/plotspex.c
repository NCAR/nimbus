/* plotspex.c -- manage the display setup array and menus
*/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/file.h>
#include <dirent.h>
#include <xview/xview.h>
#include <xview/canvas.h>

/***********************  WINDS INCLUDES  ************************************/

#include <xy_plt.h>
#include <lengths.h>
#include <plotspex.h>
#include <files.h>
#include <constants.h>
#include <errors.h>
#include <display.h>
#include <help.h>
#include <colors.h>
#include <ops.h>
#include <header.h>
#include <proto.h>
#include "pms1d.h"
#include <pms2d.h>
#include <vectors.h>
#include <macros.h>
#include <geom.h>
#include <op_panels.h>

/**************************  EXPORTABLE  **************************************/

struct alt *plotspec=0;   /* list of alternate setups */

/* autocale parameters: how close to max or min to allow before auto-adjusting 
   the range, how frequently to check for autoscale need, and number of seconds 
   to use in averaging when determining new midpoint for an auto-scaled redraw 
   of a plot.
*/
double tolerance_factor[NUM_QUADRANTS];   
int clip_chk_interval[NUM_QUADRANTS];     
int seconds_to_avg[NUM_QUADRANTS];        

int xaxis_seconds[NUM_QUADRANTS]; /* length in minutes of time axis */

int plot_type[NUM_QUADRANTS]
/* which type of xy-display */
={TIME_SERIES,TIME_SERIES,TIME_SERIES,TIME_SERIES,TIME_SERIES}; 

int autoscale[NUM_QUADRANTS]=
{FALSE,FALSE,FALSE,FALSE,FALSE};  /* whether to check for clipping or not */

/* minimum pressure, temperature expected for skew-t plots 
*/
double skew_minpress[NUM_QUADRANTS];      
double skew_mintemp[NUM_QUADRANTS];       

int IndepvarYaxis[NUM_QUADRANTS]=
{FALSE,FALSE,FALSE,FALSE,FALSE}; /* indep't var normally on x-axis */
/* dependent variable choices  */
char usrvar[NUM_QUADRANTS][MAX_DISPLAY_VARS][NAMLEN];   
char indepvar_name[NUM_QUADRANTS][NAMLEN];   /* independent variable name  */
char description[MAX_DESC_LNGTH];  /* user description for new setup */
int overwrite;				/* TRUE when user wishes to overwrite
					duplicate plotspex description entry */

/**************************  GLOBAL IN FILE  **********************************/

static int DeletePlotspec=FALSE;
static char CurrentDesc[NUM_QUADRANTS][MAX_DESC_LNGTH];
static char PlotspexFilename[MAX_FLNM_LNGTH]; /* filename made from host name */
static int NumSetups;                	/* total entries in plotspex  */
static int MaxSetups=DEFAULT_MAX_SETUPS; /* maximum # of plotspex allowed */
static int *ValidSetup;			/* indices of valid plotspec entries */
static int NumValidSetups;		/* # of valid plotspec entries */
static FILE *SetupFile;               /* handle for plotspex file */
static int SetupFileLock;		/* advisory lock descriptor */
static int NeedInitPlotspex;		/* flag set to indicate need to re-read						in the current plotspex file */
/* 
identification for current plotspex file folder 
*/
static char CurrentPlotspexFolderName[MAXFOLDERNAMELENGTH*2]=""; 
static int NeedSaveSetup[NUM_QUADRANTS]=
{FALSE,FALSE,FALSE,FALSE,FALSE};
static int VideoShowing[NUM_DISPLAY_QUADRANTS]=
{FALSE,FALSE,FALSE,FALSE};
static int QuadrantVideoSource[NUM_DISPLAY_QUADRANTS]=
{0,0,0,0};

static int NeedPlotspexPassword=FALSE;
static char PlotspexFolderName[MAXPLOTSPEXFOLDERS][MAXFOLDERNAMELENGTH*2];
static int NumPlotspexFolders=0;
static int FolderIsProtected=FALSE;
static char FolderPassword[MAXPASSWORDLENGTH];
static char PlotspexParmKeyword[NUMPLOTSPEXSAVEFIELDS][MAXKEYWORDLENGTH]=
{FOLDERNAMEKEYWORD};


/********************  GetQuadrantVideoSource()  ********************************/

GetQuadrantVideoSource(quadrant)
int quadrant;
{
 return QuadrantVideoSource[quadrant];
}


/********************  SetQuadrantVideoSource()  ********************************/

SetQuadrantVideoSource(quadrant,videosource)
int quadrant,videosource;
{
 QuadrantVideoSource[quadrant]=videosource;
}

/**********************  GetVideoShowing()  ***********************************/

GetVideoShowing(quadrant)
int quadrant;
{
 return VideoShowing[quadrant];
}

/**********************  SetVideoShowing()  ***********************************/

SetVideoShowing(quadrant,showing)
int quadrant,showing;
{
 VideoShowing[quadrant]=showing;
}


/**********************  GetPlotspexParmKeyword() *******************************/

char *
GetPlotspexParmKeyword(which)
int which;
{
 return PlotspexParmKeyword[which];
}

/**********************  GetNeedSaveSetup() *******************************/

GetNeedSaveSetup(quadrant)
{
 return NeedSaveSetup[quadrant];
}

/**********************  SetNeedSaveSetup() *******************************/

SetNeedSaveSetup(quadrant,value)
int quadrant,value;
{
 NeedSaveSetup[quadrant]=value;
}

/**********************  GetNeedInitPlotspex() *******************************/

GetNeedInitPlotspex()
{
 return NeedInitPlotspex;
}

/**********************  SetNeedInitPlotspex() *******************************/

SetNeedInitPlotspex(value)
int value;
{
 NeedInitPlotspex=value;
}

/****************  SetCurrentPlotspexFolderName() *******************************/

SetCurrentPlotspexFolderName(name)
char *name;
{
 (void)sprintf(CurrentPlotspexFolderName,"%s",name);
}

/***************  GetCurrentPlotspexFolderName() *******************************/

char *
GetCurrentPlotspexFolderName()
{
 return CurrentPlotspexFolderName?
  (strlen(CurrentPlotspexFolderName)?CurrentPlotspexFolderName:
  ""):"";
}

/*******************  PlotspexFolderIsDefault() *******************************/

PlotspexFolderIsDefault(name) 
char *name;
{
 return !(strcmp(name,"default"));
}

/**********************  SetPlotspexFilename() *******************************/

SetPlotspexFilename()
{
 (void)sprintf(PlotspexFilename,PLOTSPEX,ProjectDirectory,proj_number,
  PLOTSPEXFOLDERPREFIX,PlotspexFolderIsDefault(GetCurrentPlotspexFolderName())?
   "":GetCurrentPlotspexFolderName());
}

/**********************  GetPlotspexFilename() *******************************/

char *
GetPlotspexFilename()
{
 return PlotspexFilename;
}

/*******************  SetPlotspexFolderName() *******************************/

SetPlotspexFolderName(which,name)
int which;
char *name;
{
 if (which < MAXPLOTSPEXFOLDERS) {
  (void)strncpy(PlotspexFolderName[which],name,
   strlen(PLOTSPEXFOLDERPREFIX)+MAXFOLDERNAMELENGTH);
  if (which==GetNumPlotspexFolders())
   SetNumPlotspexFolders(which+1);
 } else {
  (void)fprintf(stderr,"WARNING: Folder name %s not set; too many folders\n",
   name);
 }
}

/*******************  SetPlotspexFolderNames() *******************************/

SetPlotspexFolderNames()
{
char dir_name[MAX_FLNM_LNGTH];
DIR *dirp;
struct dirent *dp;

 (void)sprintf(dir_name, "%s/%s", ProjectDirectory,get_proj_number());
 dirp = opendir(dir_name);
 if (dirp==NULL) {
  (void)fprintf(stderr,"WARNING: directory open failed; errno: %d\n",errno);
 }
/*
provide default folder
*/
 (void)sprintf(PlotspexFolderName[0],PLOTSPEXFOLDERPREFIX);
 SetNumPlotspexFolders(1);
/*
get any others
*/
 errno=0;
 for (dp = readdir(dirp);dp != NULL && GetNumPlotspexFolders()<MAXPLOTSPEXFOLDERS;
      dp=readdir(dirp)) {
  if (!strncmp((char*)dp->d_name,PLOTSPEXFOLDERPREFIX,strlen(PLOTSPEXFOLDERPREFIX)) 
/*
found a matching filename...
*/
     && strlen((char*)dp->d_name)>strlen(PLOTSPEXFOLDERPREFIX)) {
/*
...that has a name other than the default name
*/
   SetPlotspexFolderName(GetNumPlotspexFolders(),dp->d_name);
  } 
 }
 closedir(dirp);
 if (GetNumPlotspexFolders()==1)
  (void)fprintf(stderr,"No custom folders found\n");
}

/*******************  SetNumPlotspexFolders() *******************************/

SetNumPlotspexFolders(value)
int value;
{
 NumPlotspexFolders=value;
}

/*******************  GetNumPlotspexFolders() *******************************/

GetNumPlotspexFolders()
{
 return NumPlotspexFolders;
}

/*******************  GetPlotspexFolderName() *******************************/

char *
GetPlotspexFolderName(which)
int which;
{
 if (which < MAXPLOTSPEXFOLDERS) {
  if (!(strcmp(PlotspexFolderName[which],PLOTSPEXFOLDERPREFIX)))
   return "default";
  else
   return &PlotspexFolderName[which][strlen(PLOTSPEXFOLDERPREFIX)];
 } else {
  (void)fprintf(stderr,"WARNING: Folder name not returned; too many folders\n");  return NULL;
 }
}

/**********************  GetPms2dDispType() *******************************/

GetPms2dDispType(quadrant)
int quadrant;
{
 return get_2d_display_type(quadrant);
}

/**********************  GET_INDEPVAR_YAXIS() *******************************/

get_indepvar_yaxis(quadrant)
int quadrant;
{
 return IndepvarYaxis[quadrant];
}

/**********************  SET_INDEPVAR_YAXIS() *******************************/

set_indepvar_yaxis(quadrant,value)
int quadrant,value;
{
 IndepvarYaxis[quadrant]=value;
 set_indepvar_button(quadrant,value);
}

/**********************  GET_MAXSETUPS() *******************************/

get_maxsetups()
{
 return MaxSetups;
}

/**********************  SET_MAXSETUPS() *******************************/

set_maxsetups(value)
int value;
{
 MaxSetups=value;
}

/**********************  GET_PLOTSPEC_VALIDITY() *******************************/

get_plotspec_validity(entry)
{
 return plotspec[entry].valid;
}

/**********************  SET_PLOTSPEC_VALIDITY() *******************************/

set_plotspec_validity(entry,value)
int entry,value;
{
 plotspec[entry].valid=value;
}

/*************************  GET_VALID_SETUPS()  ****************************/

get_valid_setup(int which)
{
 return ValidSetup[which];
}

/*************************  NUM_OF_SETUPS()  *********************************/

int 
num_of_setups()
{
 return NumSetups;
}

/*************************  NUM_VALID_SETUPS()  ****************************/

num_valid_setups()
{
 return NumValidSetups;
}

/*********************  GetSubTypeFromString()  *******************************/

GetSubTypeFromString(int which)
{
 if (!(strcmp(plotspec[which].subtype,"time_series"))) 
  return TIME_SERIES;
 else if (!(strcmp(plotspec[which].subtype,"track_plot"))) 
  return TRACK_PLOT;
 else if (!(strcmp(plotspec[which].subtype,"sounding"))) 
  return SOUNDING;
 else if (!(strcmp(plotspec[which].subtype,"xy"))) 
  return XY;
 else if (!(strcmp(plotspec[which].subtype,"skew_t"))) 
  return SKEW_T;
 else if (!(strcmp(plotspec[which].subtype,"list_display"))) 
  return LISTS;
 else if (!(strcmp(plotspec[which].subtype,"fixed_display"))) 
  return FIXED_FORMAT;
 else if (!(strcmp(plotspec[which].subtype,"standard"))) 
  return PMS2D_STANDARD;
 else if (!(strcmp(plotspec[which].subtype,"greyscale"))) 
  return PMS2D_GREYSCALE;
 else if (!(strcmp(plotspec[which].subtype,"hvps"))) 
  return PMS2D_HVPS;
 else 
  return ERROR;
}

/************************  GetDispType()  *******************************/

GetDispType(which)
int which;
{
 if (!(strcmp(plotspec[which].disptype,"xy_plt")))  {
  if (!(strcmp(plotspec[which].subtype,"list_display")))  {
   return DISPTYPE_LISTS;
  } else if (!(strcmp(plotspec[which].subtype,"fixed_display")))  {
   return DISPTYPE_FIXEDFORMAT;
  } else  {
   return DISPTYPE_XYPLOT;
  }
 } else if (!(strcmp(plotspec[which].disptype,"pms1d")))  {
  return DISPTYPE_PMS1D;
 } else if (!(strcmp(plotspec[which].disptype,"pms2d")))  {
  return DISPTYPE_PMS2D;
 } else if (!(strcmp(plotspec[which].disptype,"etcetera")))  {
  return DISPTYPE_ETC;
 } else {
  return ERROR;
 }
}

/************************  GetSubType()  *******************************/

char *
GetSubType(int which)
{
 switch (GetDispType(which)) {
  case DISPTYPE_LISTS:
  case DISPTYPE_FIXEDFORMAT:
  case DISPTYPE_XYPLOT:
   switch (GetSubTypeFromString(which)) {
    case TIME_SERIES:
     return "Time Series";
    case TRACK_PLOT:
     return "Track Plot";
    case SKEW_T:
     return "Skew-T";
    case SOUNDING:
     return "Sounding";
    case XY:
     return "XY";
    case LISTS:
     return "List";
    case FIXED_FORMAT:
     return "Fixed Format";
    default:
     return "Unknown";
   }
  case DISPTYPE_ETC:
   return "Video";
  case DISPTYPE_PMS1D:
   return "Pms1d";
  case DISPTYPE_PMS2D:
   switch (GetSubTypeFromString(which)) {
    case PMS2D_STANDARD:
     return "Pms2d Std";
    case PMS2D_GREYSCALE:
     return "Pms2d Greyscale";
    case PMS2D_HVPS:
     return "Pms2d HVPS";
    default:
     return "Unknown";
   }
 }
 return "Unknown";
}

/*********************  GetSetupFlightSaved()  ****************************/

char *
GetSetupFlightSaved(int which)
/*
return the which'th plotspec flightsaved
*/
{
 return plotspec[which].flightsaved;
}

/*********************  SetSetupFlightSaved()  ****************************/

SetSetupFlightSaved(int which, char *flt)
/*
set the value of flight number during session in which this setup was saved
*/
{
 (void)sprintf(plotspec[which].flightsaved,"%s",flt);
}

/*********************  SetSetupDateSaved()  ****************************/

SetSetupDateSaved(which,date)
int which;
char *date;
/*
set the value of date during session in which this setup was saved
*/
{
 if (!date)
  (void)sprintf(plotspec[which].datesaved,"(sometime before %s)",
   (char *)GetCurrentDate());
 else
  (void)sprintf(plotspec[which].datesaved,"%s",date);
}

/*********************  GetSetupDateSaved()  ****************************/

char *
GetSetupDateSaved(int which)
/*
return the which'th plotspec datesaved
*/
{
 return plotspec[which].datesaved;
}

/*********************  GetSetupDescription()  ****************************/

char *
GetSetupDescription(which)
/*
return the which'th plotspec description
*/
{
 return (char *)TrimTrailingBlanks(plotspec[which].desc);
}

/*************************  GET_DESCRIPTION()  *******************************/

char *
get_description(int quadrant)
/* 
return current description of quadrant
*/
{
 return CurrentDesc[quadrant];
}

/*************************  SET_DESCRIPTION()  *******************************/

set_description(quadrant,string)
int quadrant;
char *string;
/* 
maintain current description per quadrant
*/
{
 (void)strcpy(CurrentDesc[quadrant],(char *)TrimTrailingBlanks(string));
}


/**********************  PMS1D_WINDOW_TYPE()  ******************************/

pms1d_window_type(quadrant)
int quadrant;
/* return TRUE if this is a pms1d-window type
*/
{
 return (window_type[quadrant]==DISPTYPE_PMS1D);
}

/**********************  PMS2D_WINDOW_TYPE()  ******************************/

pms2d_window_type(quadrant)
int quadrant;
/* return TRUE if this is a pms2d-window type
*/
{
 return (window_type[quadrant]==DISPTYPE_PMS2D);
}

/**********************  XY_WINDOW_TYPE()  **********************************/

xy_window_type(quadrant)
int quadrant;
/* return TRUE if this is an xy-window type
*/
{
 return (window_type[quadrant]==DISPTYPE_XYPLOT
        || window_type[quadrant]==DISPTYPE_LISTS
        || window_type[quadrant]==DISPTYPE_FIXEDFORMAT
        || window_type[quadrant]==WTYPE_SKEWT);
}

/****************************************************************************/

ListWindowType(quadrant)
int quadrant;
{
 return (window_type[quadrant]==DISPTYPE_LISTS);
}

/**********************  HILITE_WINDOW_TYPE()  ******************************/

hilite_window_type(quadrant)
int quadrant;
/* return TRUE if this is a window type that uses hiliting as cursor moves
   around
*/
{
 return (window_type[quadrant]==DISPTYPE_PMS1D || 
         xy_window_type(quadrant)          ||         
         quadrant==STATS
        );
}

/**********************  CALIB_WINDOW_TYPE()  ******************************/

calib_window_type(quadrant)
int quadrant;
/* return TRUE if this is a window type that uses hiliting as cursor moves
   around
*/
{
 return (quadrant==STATS || quadrant==PLOTS || quadrant==DIALOGUE);
}

/*************************  SET_VALID_SETUPS()  ****************************/

set_valid_setups()
{
int setup,valid;
static int alloc=FALSE;

 if (!alloc) {
  ValidSetup=(int *)malloc((unsigned int)get_maxsetups()*sizeof(int));
  alloc=TRUE;
 }
 for (valid=setup=0; setup<num_of_setups(); setup++)
  if (get_plotspec_validity(setup))
   ValidSetup[valid++]=setup;
 NumValidSetups=valid;
}

/************************  SET_DEFAULTS()  **********************************/

set_defaults(quadrant)
int quadrant;
{
/* set defaults */

 tolerance_factor[quadrant]=DEFAULT_TOLERANCE_FACTOR;
 clip_chk_interval[quadrant]=DEFAULT_CLIP_CHK_INTERVAL;
 seconds_to_avg[quadrant]=DEFAULT_SEX_2_AVG;
 xaxis_seconds[quadrant]=DEFAULT_XAXIS_SECONDS;
 plot_type[quadrant]=TIME_SERIES;
 num_of_traces[quadrant]=0;
 num_of_listees[quadrant]=0;
 num_of_fixees[quadrant]=0;
 skew_minpress[quadrant]=DEFAULT_MINPRESS;
 skew_mintemp[quadrant]=DEFAULT_MINTEMP;
 set_indepvar_yaxis(quadrant,FALSE);
 set_isolate_traces(quadrant,FALSE);
 update_mode[quadrant]=RECONFIG;
 set_vector_color_button(quadrant,CONTRAST_TRACE);
 set_vector_interval(quadrant,VECTOR_DEFAULT_INTRVL);
 SetVectorMaxScale(quadrant,VECTOR_DEFAULT_SCALE_MAX);
 autoscale[quadrant]=FALSE;
 plot_setup[quadrant]=FALSE;
 set_geom(quadrant,get_default_geom());
 SetListRate(quadrant,GetDefaultListRate());
 set_same_scale(quadrant,FALSE);
}

/*************************  FIND_PLOTSPEC_ENTRY()  ***************************/

find_plotspec_entry(fp,entry)
FILE **fp;
int entry;
/*
search file handled with fp for the entry'th entry.  Scan in strings in the
entry until reach start of argument list, as indicated with keyword "ARGS"
*/
{
int entry_number;
char arg[MAX_ARG_LNGTH];
 
/* read in the arguments for designated spec */

 entry_number=0;
 while (fscanf(*fp,"%s",arg) !=EOF) {
  if (!(strncmp(arg,FIRST_WORD,strlen(FIRST_WORD)))) {
   if ((++entry_number)==entry) {
    do {
     (void)fscanf(*fp,"%s",arg);
    } while (strcmp(arg,"ARGS"));
    break;
   }
  }
 }
}

/***********************  SET_ARGUMENT_MODE()  **********************************/

set_argument_mode(arg)
char *arg;
/*
set the mode as function of string value; assume the argument 
is an argument value, i.e. NOT a keyword, until proven otherwise.

Keywords are defined in ../include/plotspex.h
*/
{
static int mode;

  mode=ARGUMENT_VALUE;
  if (!(strcmp(arg,"DESCRIPTION"))) {
   mode=DESCRIBE;
  } else if (!(strcmp(arg,"FLIGHTSAVED"))) {
   mode=FLIGHTSAVED;
  } else if (!(strcmp(arg,"DATESAVED"))) {
   mode=DATESAVED;
  } else if (!(strcmp(arg,"ARGS"))) {
   mode=ARGS;
  } else if (!(strcmp(arg,"TRACES"))) {
   mode=TRACES;
  } else if (!(strcmp(arg,"CLIPPING"))) {
   mode=CLIPPING;
  } else if (!(strcmp(arg,"TYPE"))) {
   mode=TYPE;
  } else if (!(strcmp(arg,"INDEP_VAR"))) {
   mode=INDEP_VAR;
  } else if (!(strcmp(arg,"TIME_RANGE"))) {
   mode=TIME_RANGE;
  } else if (!(strcmp(arg,"INDEPVAR_YAXIS"))) {
   mode=INDEPVAR_YAXIS;
  } else if (!(strcmp(arg,"ISOLATE_TRACES"))) {
   mode=ISOLATE_TRACES;
  } else if (!(strcmp(arg,"LISTEES"))) {
   mode=LISTEES;
  } else if (!(strcmp(arg,"FIXEES"))) {
   mode=FIXEES;
  } else if (!(strcmp(arg,"BYPASS"))) {
   mode=BYPASS;
  } else if (!(strcmp(arg,"SKEW_CORNER"))) {
   mode=SKEW_CORNER;
  } else if (!(strcmp(arg,"VECTORS"))) {
   mode=VECTORS;
  } else if (!(strcmp(arg,"VECTORS_CONTRAST"))) {
   mode=VECTORS_CONTRAST;
  } else if (!(strcmp(arg,"VECTORS_ON"))) {
   mode=VECTORS_ON;
  } else if (!(strcmp(arg,"DEP_RANGE"))) {
   mode=DEP_RANGE;
  } else if (!(strcmp(arg,"SAME_SCALE"))) {
   mode=SCALES_SAME;
  } else if (!(strcmp(arg,"INDEP_RANGE"))) {
   mode=INDEP_RANGE;
  } else if (!(strcmp(arg,"DEP_INVRT_SCL"))) {
   mode=DEP_INVRT_SCL;
  } else if (!(strcmp(arg,"INDEP_INVRT_SCL"))) {
   mode=INDEP_INVRT_SCL;
  } else if (!(strcmp(arg,"GEOMETRY"))) {
   mode=GEOMETRY;
  } else if (!(strcmp(arg,"LIST_RATE"))) {
   mode=LIST_RATE;
  } else if (!(strcmp(arg,"END_ARGS"))) {
   mode=END_ARGS;
  } else if (!(strcmp(arg,"NPLPROBES"))) {
   mode=NPLPROBES;
  } else if (!(strcmp(arg,"PLPROBES"))) {
   mode=PLPROBES;
  } else if (!(strcmp(arg,"CHANNEL"))) {
   mode=CHANNEL;
  } else if (!(strcmp(arg,"DISP_RATE"))) {
   mode=DISP_RATE;
  } 
  return mode;
}

/***************************  INIT_PLOTSPEX()  *******************************/
 
init_plotspex()
/* re-initialize the plotspex menus with newly created plotspex file info
*/
{
int quadrant;

 destroy_alt_menus();
 ReadInPlotspex();
 make_alt_menu();
 for (quadrant=0; quadrant<get_num_displays(); quadrant++)
  AttachPlotspecMenus(quadrant);
 CreatePlotspexChoices();
}

/*********************  GetDescriptionFromFile()  ******************************/

char *
GetDescriptionFromFile(File,skipleadingspaces)
FILE *File;
int skipleadingspaces;
{
static char descrn[MAX_DESC_LNGTH*2];
int chr,desclength,start;
char c[1],*enddescptr;
/* 
File pointer is assumed to be at some point after the keyword for the
plotspec description.

Read in a maximum length description, a character at a time. Refrain from using
%s since there may be imbedded duplicate spaces. Don't want to use fgets either
because that limits us as to knowing ahead of time what the max length of an
entry has to be. Return the description, sans leading and/or trailing spaces, 
and return the file pointer to the end of that description in the file.

Will read in description until either maximum length is read or keyword "ARGS"
is encountered. Will skip leading spaces only if skipleadingspaces is TRUE.
Want this TRUE if caller has read in only the DESCRIPTION keyword, but if caller
has read in any amount beyond this (as is true in ReadInPlotspec()), want to
retain possible imbedded duplicate spaces, so set skipleadingspaces FALSE.
*/
 if (skipleadingspaces) {
  do {
   (void)fscanf(File,"%c",c);
  } while (isspace(c[0]));
  descrn[0]=c[0];
  start=1;
 } else
  start=0;
 for (chr=start; chr<MAX_DESC_LNGTH+strlen("ARGS")-start; chr++) {
  (void)fscanf(File,"%c",&descrn[chr]);
 }
 descrn[MAX_DESC_LNGTH+strlen("ARGS")-start]='\0';
 enddescptr=(char *)strstr(descrn,"ARGS");
 desclength=strlen(descrn)-(enddescptr?strlen(enddescptr):0);
 (void)fseek(File,(long)(-(enddescptr?strlen(enddescptr):0)),1);
/*
cut off extra non-description characters
*/
 descrn[desclength]='\0';
 return (char *)TrimTrailingBlanks(descrn);
}

/**********************  GetFolderIsProtected() *******************************/

GetFolderIsProtected()
{
 return FolderIsProtected;
}

/**********************  GetFolderPassword() *******************************/

char *
GetFolderPassword()
{
 return FolderPassword;
}

/**********************  SetFolderPassword() *******************************/

SetFolderPassword(password)
char *password;
/*
set on read in of plotspex folder, as function of password-entry field value.
Read-in occurs on initial startup, whenever a setup is saved or deleted, and
whenever the folder is changed to a different one.
*/
{
 if (strcmp(password,NOPASSWORD)) {
  (void)sprintf(FolderPassword,"%s",password);
  SetFolderIsProtected(TRUE);
 } else {
  SetFolderIsProtected(FALSE);
 }
}

/**********************  SetFolderIsProtected() *******************************/

SetFolderIsProtected(value)
int value;
{
 FolderIsProtected=value;
}

/**********************  ReadInPlotspex() *******************************/

ReadInPlotspex()
/* read info in PLOTSPEX file and create entries in plotspec structure
   as requested there.  Each requested setup entry must end with the
   string "END_ARGS".  
*/
{
FILE *plotspex,                    /* displays setup file */
     *WINDSfopen();
char entry_num[MAX_ARG_LNGTH],     /* "Entry_n" string in plotspex */
     disptype[MAX_ARG_LNGTH],      /* type of display window */
     *tempdesc,                	   /* temporary storage for description */
     arg[MAX_ARG_LNGTH];           /* other setup argument(s) */
int skip_entry,                    /* */
    dummy,                    	   /* */
    mode,                          /* type of argument being read */
    no_more_args,                  /* no more arguments ! */
    current_setup;                 /* current plotspex entry number */
static int vectorarg=0;
int entryispassword;
int passwordreadin;
 
/* 
derived list of names must be initialized before successfully calling this
routine - ostensibly done by WaitOnVariableLists() before this call is made

open and read in setup file into plotspec structure 
*/
NumSetups=0;
SetPlotspexFilename();
if (!PlotspexFolderIsDefault(GetCurrentPlotspexFolderName()) 
 && !FileExists(GetPlotspexFilename())) {
 (void)fprintf(stderr,
  "WARNING: custom plotspex folder %s is missing; using default\n",
  GetCurrentPlotspexFolderName());
 SetCurrentPlotspexFolderName("default");
 SetPlotspexFilename();
} 
if ( (plotspex=WINDSfopen(PlotspexFilename,"r")) == NULL) {
 (void)fprintf(stderr,"WARNING: no plotspex file created for project %s!!!\n",
   proj_number);
 set_valid_setups();
 return;
} 
SetFolderPassword(NOPASSWORD);
entryispassword=FALSE;
passwordreadin=FALSE;
while (fscanf(plotspex,"%s %s",entry_num,disptype) != EOF) {
 if (!NumSetups) {
/*
first time through, malloc a chunk of plotspec structures
*/
  if (plotspec)
   free((struct alt *)plotspec);
  plotspec=(struct alt *)malloc((unsigned int)SETUP_BLOCKSIZE*sizeof(struct alt));
 } else if (!((NumSetups+1)%SETUP_BLOCKSIZE) &&
   (NumSetups+1)<=get_maxsetups()) {
/*
and just before (NumSetups+1) every SETUP_BLOCKSIZE'th time, re-allocate for 
more space -- allow extra +1 since we are null terminating the last struct 
below, to allow correct check in loop in make_alt_menus() as it reads the array
of structs, until null termination
*/
  plotspec=(struct alt *)
    realloc((void *)plotspec,
     (size_t)(((NumSetups+1)+SETUP_BLOCKSIZE)*sizeof(struct alt)));
 }
 if (!(strcmp(entry_num,PASSWORDENTRY))) {
  entryispassword=TRUE;
  SetFolderPassword(disptype);
  passwordreadin=TRUE;
 } else {
/* 
stick setup name into struct -- 
*/
  current_setup=++NumSetups-1;
  (void)sprintf(plotspec[current_setup].disptype,"%s",disptype);
  (void)sprintf(plotspec[NumSetups].disptype,"\0"); /* null terminate */
  SetSetupFlightSaved(current_setup,"unknown");
  SetSetupDateSaved(current_setup,(char *)NULL);
 }

 if (!entryispassword && NumSetups > get_maxsetups() ) {
/*
don't exceed the specified limit
*/
  NumSetups--;
  set_notice_needed(TOO_MANY_SETUPS);
  time_to_log();
  (void)fprintf(stderr,
   "WARNING: attempt to exceed max. plotspex entries (%d)\n",get_maxsetups());
  set_valid_setups();
  (void)WINDSfclose(plotspex);
  if (!passwordreadin) {
/*
must read the rest of the file until a password, if any, is seen
*/
   if (ReadFileIntoEntryArray(PlotspexFilename,ErrorReport)!=ERROR) {
    int entry,posn;
    if ((entry=FindEntryWithString(PASSWORDENTRY))!=ERROR) {
     posn=0;
/*
here we believe that the password is the 2nd word in the password entry string
*/
     (void)GetNextString(&posn,(char *)GetEntryString(entry));
     SetFolderPassword((char *)GetNextString(&posn,GetEntryString(entry)));
    }
   } 
  }
  return;
 } 
/* deal with further arguments */

 no_more_args=FALSE;
 skip_entry=FALSE;
 do {
  (void)fscanf(plotspex,"%s",arg);
   if ((dummy=set_argument_mode(arg))!=ARGUMENT_VALUE) {
/*
got a keyword
*/
    mode=dummy;
    if (mode==END_ARGS) 
     no_more_args=TRUE;
   } else switch (mode) {
/*
got an argument value for previous keyword category
*/
     case DESCRIBE:
      vectorarg=0;
      tempdesc=(char *)GetDescriptionFromFile(plotspex,FALSE);
      if (tempdesc)
/*
since we are telling GetDescriptionFromFile() NOT to skip leading spaces,
don't put a space between two strings that are used to reconstruct the
description, GetDescriptionFromFile()'s value will do that for us.
*/
       (void)sprintf(plotspec[current_setup].desc,"%s%s",arg,tempdesc);
      else
       (void)sprintf(plotspec[current_setup].desc,"%s",arg);
/*
currently, post processing mode does not honor video or pms2d probe displays.
Invalidate these setups to preclude alerts during setup cycling.
*/
      if ((post_processing_mode() &&
          (
/*** re-activated 6/22/94...why not?
           !(strcmp(plotspec[current_setup].disptype,"etcetera")) ||
***/
           !(strcmp(plotspec[current_setup].disptype,"pms2d"))))  ||
           (!strcmp(plotspec[current_setup].disptype,"pms1d")
             && !NumberPMS1Dprobes())
	) {
       skip_entry=TRUE;
       (void)sprintf(arg,"video/pms2d in pp mode, or pms1d w/o any probes");
      }
      break;
     case FLIGHTSAVED:
      SetSetupFlightSaved(current_setup,arg);
      break;
     case DATESAVED:
      SetSetupDateSaved(current_setup,arg);
      break;
     case TYPE:
      (void)sprintf(plotspec[current_setup].subtype,"%s",arg);
      if (!strcmp(plotspec[current_setup].disptype,"pms2d"))
       if (!NumberPMS2Dprobes(GetSubTypeFromString(current_setup))) {
        skip_entry=TRUE;
        (void)sprintf(arg,"pms2d w/o any probes");
       }
      break;
/*
verify that each variable name listed in current entry is actually in the current 
dataset; if not, skip this entry.
*/
     case TRACES:
     case INDEP_VAR:
     case LISTEES:
     case FIXEES:
      if (GetVarListIndex(arg,TRUE)==ERROR) {
       skip_entry=TRUE;
      }
      break;
     case VECTORS:
      if (++vectorarg==3 || vectorarg==4)
/*
examine only the 3rd and 4th vector mode arguments -- x and y components
*/
       if (GetVarListIndex(arg,TRUE)==ERROR) {
        skip_entry=TRUE;
       }
      break;
     default:
      break;
   }
 } while (!no_more_args && !skip_entry);
 if (skip_entry) {
/***
  (void)fprintf(stderr,"WARNING: discarding plotspec entry %s due to argument %s\n",
   plotspec[current_setup].desc,arg);
***/
  set_plotspec_validity(current_setup,FALSE);
  do {
   (void)fscanf(plotspex,"%s",arg);
  } while (set_argument_mode(arg)!=END_ARGS);
 } else {
  if (entryispassword) {
   entryispassword=FALSE;
  } else {
   set_plotspec_validity(current_setup,TRUE);
  }
 }
} 
(void)WINDSfclose(plotspex);
set_valid_setups();
return;
}

/*************************  SET_XY_WINDOW_TYPE()  ****************************/

set_window_type(quadrant,type)
int quadrant;

{
switch (type) {
 case DISPTYPE_XYPLOT:
 case DISPTYPE_LISTS:
 case DISPTYPE_FIXEDFORMAT:
 case WTYPE_CALIBDISP:
 case WTYPE_SKEWT:

 switch (plot_type[quadrant]) {
  case TIME_SERIES:
  case TRACK_PLOT:
  case SOUNDING:
  case XY:
   window_type[quadrant]=DISPTYPE_XYPLOT;
   break;
  case SKEW_T:
   window_type[quadrant]=WTYPE_SKEWT;
   break;
  case LISTS:
   window_type[quadrant]=DISPTYPE_LISTS;
   break;
  case FIXED_FORMAT:
   window_type[quadrant]=DISPTYPE_FIXEDFORMAT;
   break;
  case CALIBRATION:
   window_type[quadrant]=WTYPE_CALIBDISP;
   break;
 }
 break;
 
 case DISPTYPE_ETC:
  window_type[quadrant]=DISPTYPE_ETC;
  break;
 case DISPTYPE_PMS1D:
  window_type[quadrant]=DISPTYPE_PMS1D;
  break;
 case DISPTYPE_PMS2D:
  switch (GetPms2dDispType(quadrant)) {
   case PMS2D_STANDARD:
   case PMS2D_GREYSCALE:
   case PMS2D_HVPS:
    window_type[quadrant]=DISPTYPE_PMS2D;
  }
}
}

/*************************  FILL_DISPLAYS()  *********************************/

fill_displays(direction)
int direction;
/* 
replace current WINDS displays with get_num_displays() setups, as defined
in plotspex file (stored in plotspec[] array) going either forward or back
in file, as defined by direction.
*/
{
int plotspec_entry,quadrant;
int num_displays,displays_filled;
static int index=0;          /* index into array of valid setups */
static int filldebug=FALSE;  /* turn me on in the dbxtool to get details */
 
 num_displays=get_num_displays();

 if (!direction) {
/*
we are at startup time -- special conditions
*/
  plotspec_entry=get_valid_setup(0);
  if (filldebug)
   (void)fprintf(stderr,"STARTUP: first valid setup is %s\n",
    plotspec[plotspec_entry].desc);
/*
re-init index since fill_displays() may be called later on with arg of 0
*/
  index=0;
 } else {
/*
change starting index as per direction of cycling desired
*/
  index=index+(direction>0?1:-1)*num_displays;
  if (filldebug)
   (void)fprintf(stderr,"request cycle starting from %d\n",index);
  if (index>=num_valid_setups()) {
   index=0;
   if (filldebug)
    (void)fprintf(stderr,"That's too high; reset index to 0\n");
  }
  if (index<0) {
/*
going backwards from first subset; determine start index as function of number
available, so as to manage positioning consistently
*/
   if (num_displays>1)
    index=(((num_valid_setups()-1)/num_displays)*num_displays);
   else
    index=num_valid_setups()-1;
   if (filldebug)
    (void)fprintf(stderr,"That's too low; reset index to %d\n",index);
  } 
  plotspec_entry=get_valid_setup(index);
  if (filldebug)
   (void)fprintf(stderr,"CYCLE: first valid setup is %s\n",
    plotspec[plotspec_entry].desc);
 }

 displays_filled=0;
 do {
/*
fill each quadrant with either a valid setup or a new setup window
*/
  quadrant=displays_filled; 
  if (!plotspec) {
/*
no plotspex file found; use new setups
*/
   if (filldebug)
    (void)fprintf(stderr,"No plotspex found; assign quadrant %d to new setup\n",
    quadrant);
   SetNewSetupNeeded(quadrant,TRUE);
   FillQuadrant(quadrant,PUR);
   displays_filled++;    
  } else if (plotspec_entry > num_of_setups()-1) {
/*
at end of plotspex file; use new setups if at startup, else let it ride so as
to maintain cycle positioning management.
*/
   if (filldebug)
    (void)fprintf(stderr,"At end of %d plotspex file entries\n",
    num_of_setups());
   if (!direction) {
    SetNewSetupNeeded(quadrant,TRUE);
    FillQuadrant(quadrant,PUR);
    if (filldebug)
     (void)fprintf(stderr,"...assign new setup to quadrant %d\n",quadrant);
   } else {
    if (filldebug)
     (void)fprintf(stderr," do nothing for quadrant %d\n",quadrant);
   }
   displays_filled++;
  } else {
   do {
    if (get_plotspec_validity(plotspec_entry)) {
/*
use this one for a quadrant fill.  
*/
     if (!(strcmp(plotspec[plotspec_entry].disptype,"xy_plt"))) {
      config_display(quadrant,DISPTYPE_XYPLOT,plotspec_entry+1);
      displays_filled++;
     } else if (!(strcmp(plotspec[plotspec_entry].disptype,"pms1d"))) {
      config_display(quadrant,DISPTYPE_PMS1D,plotspec_entry+1);
      displays_filled++;
     } else if (!(strcmp(plotspec[plotspec_entry].disptype,"etcetera"))) {
      config_display(quadrant,DISPTYPE_ETC,plotspec_entry+1);
      displays_filled++;
     } else if (!(strcmp(plotspec[plotspec_entry].disptype,"pms2d"))) {
      config_display(quadrant,DISPTYPE_PMS2D,plotspec_entry+1);
      displays_filled++;
     } else {
/*
whoops...
*/
      time_to_log();
      (void)fprintf(stderr,
       "WARNING: invalid type in fill_displays(): %d, in %s.  NOTIFY WINDS EXPERT!!\n",
        plotspec[plotspec_entry].disptype,plotspec[plotspec_entry].desc);
     }
    } else {
/*
invalid plotspec for this dataset...ignore
*/
      if (filldebug)
       (void)fprintf(stderr,
       "skip plotspec entry: %s\n",plotspec[plotspec_entry].desc);
    }
/*
continue while entry just examined is invalid AND we are not at EOF yet
*/
   } while (!get_plotspec_validity(plotspec_entry++) &&
             plotspec_entry < num_of_setups());
  }
 } while (displays_filled<num_displays);
/*
fini initialization
*/
 window_type[HELP_IMAGE]=WTYPE_CONTROLPANEL;
 window_type[TIME_AND_PROJECT]=WTYPE_CONTROLPANEL;
 window_type[STATS]=WTYPE_CALIBDISP;
 window_type[PLOTS]=WTYPE_CALIBDISP;
 window_type[DIALOGUE]=WTYPE_CALIBDISP;
}

/*****************************  CONFIG_DISPLAY()  ***************************/

config_display(quadrant,disptype,entry)
int quadrant,disptype,entry;
/* read argument file for further args */
{
 int NeedCheckOnVideo;

 if (!NewSetup(entry)) {
  hide_oppanel(quadrant);
  set_genop_value(quadrant,0);
  set_genop_hiliting(TRUE,quadrant,get_genop_value(quadrant));
 }
 NeedCheckOnVideo=FALSE;
if (
    disptype==DISPTYPE_XYPLOT 
 || disptype==DISPTYPE_LISTS 
 || disptype==DISPTYPE_FIXEDFORMAT
) {
 if (get_xy_args(quadrant,entry)) {
  NeedCheckOnVideo=TRUE;
  if (!NewSetup(entry))
   SetNewSetupNeeded(quadrant,FALSE);
 }
} else if (disptype==DISPTYPE_PMS1D) {
 if (get_1d_args(quadrant,entry)) {
  init_state_parms(quadrant,DISPTYPE_PMS1D);
  (*FillArea[GetDrawType()])(pw_xid[quadrant],0,0,
    window_width[quadrant],window_height[quadrant],0);
  PlotPMS1Dbackground(quadrant);
  NeedCheckOnVideo=TRUE;
  SetNewSetupNeeded(quadrant,FALSE);
 } else {
  set_notice_needed(NO_PMS1D_DATA);
  return;
 }
} else if (disptype==DISPTYPE_ETC ) {
 if (get_etc_args(quadrant,entry)) {
/*
so far only VIDEO is available as an etc disptype
*/
  set_geom(quadrant,get_default_geom());
  init_state_parms(quadrant,DISPTYPE_ETC);
  SetNewSetupNeeded(quadrant,FALSE);
 } else {
  return;
 }
} else if (disptype==DISPTYPE_PMS2D ) {
 if (get_2d_args(quadrant,entry)) {
  set_2d_cursor(quadrant);
  NeedCheckOnVideo=TRUE;
  show_setup(quadrant);
  SetNewSetupNeeded(quadrant,FALSE);
 } else {
  return;
 }
}
#ifdef PARALLAX
if (NeedCheckOnVideo && GetVideoShowing(quadrant)) {
 DestroyVideo(GetQuadrantVideoSource(quadrant));
 SetVideoShowing(quadrant,FALSE);
}
#endif
set_cursor_tracking(quadrant);
set_genop_buttons(quadrant);
clear_genop_status(quadrant);
if (!NewSetup(entry)) {
/*
user has configured a previously saved setup into this window; set and
display its description
*/
 set_description(quadrant,plotspec[entry-1].desc);
 show_op_msg(quadrant,(char *)get_description(quadrant));
 time_to_log();
 (void)fprintf(stderr,"Fill quadrant %d with '%s'\n",
  quadrant,(char *)get_description(quadrant));
}
return;
}

/**************************  GET_ETC_ARGS()  **********************************/

get_etc_args(quadrant,entry)
int quadrant, entry;
{
int process=ERROR,mode,temp,videosource=0;
FILE *arg_file;
char arg[MAX_ARG_LNGTH];

 if (NewSetup(entry)) { 
  if (StartUpVideo(quadrant,videosource)) {
   set_description(quadrant,"(new setup - untitled)");
   show_op_msg(quadrant,(char *)get_description(quadrant));
   return TRUE;
  } else
   return FALSE;
 } 
 if ((arg_file=WINDSfopen(GetPlotspexFilename(),"r")) == NULL) {
  time_to_log();
  (void)fprintf(stderr,"WARNING: no plotspex file created for project %s!\n",
   proj_number);
  return FALSE;
 } 
 find_plotspec_entry(&arg_file,entry);
 while (fscanf(arg_file,"%s",arg) !=EOF) {
   if ((temp=set_argument_mode(arg))!=ARGUMENT_VALUE) {
    mode=temp;
    if (mode==END_ARGS)  {
     break;
    }
   } else switch (mode) {
    case TYPE:
    if (!(strcmp(arg,"video"))) {
     process=VIDEO;
    }
    case CHANNEL:
     (void)sscanf(arg,"%d",&videosource);
     break;
   }
 } 
 (void)WINDSfclose(arg_file);
 switch (process) {
  case VIDEO:
   return StartUpVideo(quadrant,videosource);
 }
}

/*************************** StartUpVideo()  **********************************/

StartUpVideo(quadrant,videosource)
int quadrant,videosource;
{
#ifdef PARALLAX
 SetQuadrantVideoSource(quadrant,videosource);
 if (GetVideoShowing(quadrant)) {
  DestroyVideo(GetQuadrantVideoSource(quadrant));
  SetVideoShowing(quadrant,FALSE);
 }
 if (!CreateVideo(canvas[quadrant],videosource)) {
  CheckVideoStatus();
  show_op_msg(quadrant,(char *)get_description(quadrant));
  (void)sprintf(error_info,"Request for video display could not be honored, for following reason: %s",(char *)GetVideoErrorMsg());
  set_notice_needed(INVALID_DESCRIPTION);
  return FALSE;
 } else {
  SetVideoShowing(quadrant,TRUE);
  return TRUE;
 }
#else
 show_op_msg(quadrant,(char *)get_description(quadrant));
 (void)sprintf(error_info,"Request for video display could not be honored: embedded WINDS video software is not available. Video can be independently accessed by NetVUE, if it is installed (see OW menu).");
 set_notice_needed(INVALID_DESCRIPTION);
 return FALSE;
#endif
}

/**************************  GET_2D_ARGS()  **********************************/

get_2d_args(quadrant,entry)
int quadrant,entry;
{
FILE *arg_file;
char arg[MAX_ARG_LNGTH];
int geometry,num,mode,temp;

 if (post_processing_mode()) {
  (void)sprintf(error_info,
  "No 2d data available in post-processing mode; cannot display pms2d window.");
  set_notice_needed(NO_PMS2D_DATA);
  return FALSE;
 }
 if (NewSetup(entry)) { 
  set_geom(quadrant,get_default_geom());
  init_pms2d_parms(quadrant,GetEntryNumber(entry));
  return TRUE;
 } 
 if ((arg_file=WINDSfopen(GetPlotspexFilename(),"r")) == NULL) {
  time_to_log();
  (void)fprintf(stderr,"WARNING: no plotspex file created for project %s!\n",
   proj_number);
  return FALSE;
 } 

 find_plotspec_entry(&arg_file,entry);
 while (fscanf(arg_file,"%s",arg) !=EOF) {
   if ((temp=set_argument_mode(arg))!=ARGUMENT_VALUE) {
    mode=temp;
    if (mode==END_ARGS)  {
     (void)WINDSfclose(arg_file);
     return TRUE;
    }
   } else switch (mode) {
    case TYPE:
     if (!(strcmp(arg,"standard"))) 
      init_pms2d_parms(quadrant,PMS2D_STANDARD);
     else if (!(strcmp(arg,"greyscale"))) 
      init_pms2d_parms(quadrant,PMS2D_GREYSCALE);
     else if (!(strcmp(arg,"hvps"))) 
      init_pms2d_parms(quadrant,PMS2D_HVPS);
     else
      (void)fprintf(stderr,"WARNING: invalid pms2d-type: %s\n",arg);
     break;
    case DISP_RATE:
     (void)sscanf(arg,"%d",&disp_rate[quadrant]);
     break;
    case GEOMETRY:
     (void)sscanf(arg,"%d",&geometry);
     set_geom(quadrant,geometry);
     break;
    case NPLPROBES:
/*
here we assume that TYPE has already been set
*/
     (void)sscanf(arg,"%d",&num);
     SetNum2dProbes(quadrant,num);
     break;
    case PLPROBES:
     select_2d_probe(quadrant,get_2d_prb_index(quadrant,arg));
     break;
   }
 } 
 (void)WINDSfclose(arg_file);
 return TRUE;
}

/**************************  GET_XY_ARGS()  **********************************/

get_xy_args(quadrant,entry)
int quadrant,entry;

{
FILE *arg_file;
char arg[MAX_ARG_LNGTH];
static int mode=ERROR;
int temp,trace,geometry,list_rate;
int attrnum;
double plus_or_minus,min,max;

 if (NewSetup(entry)) { /* set a flag and informational variables, to be
                          handled in timer handler */
  initiate_new_setup(quadrant,GetEntryNumber(entry));
  return TRUE;
 } else if (entry==CALIB_SETUP) {
  plot_type[quadrant]=CALIBRATION;
  init_state_parms(quadrant,DISPTYPE_XYPLOT);
  init_display_parms(quadrant);
  return TRUE;
 } else
  set_defaults(quadrant);

 if ((arg_file=WINDSfopen(GetPlotspexFilename(),"r")) == NULL) {
  time_to_log();
  (void)fprintf(stderr,"WARNING: no plotspex file created for project %s!!!\n",
   proj_number);
  return FALSE;
 } 

 find_plotspec_entry(&arg_file,entry);
 while (fscanf(arg_file,"%s",arg) !=EOF) {
   if ((temp=set_argument_mode(arg))!=ARGUMENT_VALUE) {
    mode=temp;
    if (mode==END_ARGS)  {
     (void)WINDSfclose(arg_file);
     return TRUE;
    }
   } else switch (mode) {
    case TRACES:
     (void)strcpy(usrvar[quadrant][num_of_traces[quadrant]],arg);
     if (++num_of_traces[quadrant]>MAX_TRACES) {
      num_of_traces[quadrant]=MAX_TRACES;
      mode=BYPASS;
     }
     break;
    case LISTEES:
     (void)strcpy(usrvar[quadrant][num_of_listees[quadrant]],arg);
     if (++num_of_listees[quadrant]>GetMaxListees()) {
      num_of_listees[quadrant]=GetMaxListees();
      mode=BYPASS;
     }
     break;
    case FIXEES:
     (void)strcpy(usrvar[quadrant][num_of_fixees[quadrant]],arg);
     if (++num_of_fixees[quadrant]>GetMaxFixees()) {
      num_of_fixees[quadrant]=GetMaxFixees();
      mode=BYPASS;
     }
     break;
    case CLIPPING:
     (void)sscanf(arg,"%lf",&tolerance_factor[quadrant]);
     (void)fscanf(arg_file,"%d %d",
      &clip_chk_interval[quadrant],&seconds_to_avg[quadrant]);

/* values of -1 -1 -1 imply no autoscaling 
*/
     if (tolerance_factor[quadrant] == ERROR ||
         clip_chk_interval[quadrant] == ERROR ||
         seconds_to_avg[quadrant] == ERROR ) {
      seconds_to_avg[quadrant]=DEFAULT_SEX_2_AVG;
      tolerance_factor[quadrant]=DEFAULT_TOLERANCE_FACTOR;
      clip_chk_interval[quadrant]=DEFAULT_CLIP_CHK_INTERVAL;
      autoscale[quadrant]=FALSE;
     } else
      autoscale[quadrant]=TRUE;
     break;
    case GEOMETRY:
     (void)sscanf(arg,"%d",&geometry);
     set_geom(quadrant,geometry);
     break;
    case LIST_RATE:
     (void)sscanf(arg,"%d",&list_rate);
     SetListRate(quadrant,list_rate);
     break;
    case INDEP_VAR:
     (void)strcpy(indepvar_name[quadrant],arg);
     break;
    case INDEPVAR_YAXIS:
     set_indepvar_yaxis(quadrant,atoi(arg));
     break;
    case SKEW_CORNER:
     (void)sscanf(arg,"%lf",&skew_minpress[quadrant]);
     (void)fscanf(arg_file,"%lf",&skew_mintemp[quadrant]);
     break;
    case VECTORS:
     set_vector_interval(quadrant,atoi(arg));
     (void)fscanf(arg_file,"%d",&temp);
     SetVectorMaxScale(quadrant,temp);
     (void)fscanf(arg_file,"%s",arg);
     set_x_component(quadrant,arg);
     (void)fscanf(arg_file,"%s",arg);
     set_y_component(quadrant,arg);
     break;
    case VECTORS_ON:
     for (trace=0; trace<num_of_traces[quadrant]; trace++) {
      if (trace==0)
       (void)sscanf(arg,"%d",&temp);
      else
       (void)fscanf(arg_file,"%d",&temp);
      set_show_vector(quadrant,temp,trace,TRUE);
     }
     break;
    case VECTORS_CONTRAST:
     (void)sscanf(arg,"%d",&temp);
     set_vector_color(quadrant,temp);
     set_vector_color_button(quadrant,temp);
     break;
    case ISOLATE_TRACES:
     (void)sscanf(arg,"%d",&temp);
     set_isolate_traces(quadrant,atoi(arg));
     break;
    case SCALES_SAME:
     (void)sscanf(arg,"%d",&temp);
     set_need_same_scale(quadrant,temp);
     break;
    case TIME_RANGE:
     (void)sscanf(arg,"%d",&xaxis_seconds[quadrant]);
     break;
    case INDEP_INVRT_SCL:
     if ( (attrnum=GetPlotAttrIndex(indepvar_name[quadrant],FALSE))!=ERROR) 
      (void)sscanf(arg,"%d",&indep_invert_scale[quadrant][attrnum]);
     else {
/*
varname not found ==> problem with indexing 
*/
      (void)fprintf(stderr,
       "WARNING: INDEP_INVRT_SCL unset; %s unfound in GetPlotAttrIndex\n",
       indepvar_name[quadrant]);
     }
     break;
    case DEP_INVRT_SCL:
     for (trace=0; trace<num_of_traces[quadrant]; trace++) {
      if ( (attrnum=GetPlotAttrIndex(usrvar[quadrant][trace],FALSE))!=ERROR) {
       if (trace==0)
        (void)sscanf(arg,"%d",&dep_invert_scale[quadrant][attrnum]);
       else
        (void)fscanf(arg_file,"%d",&dep_invert_scale[quadrant][attrnum]);
      } else {
/*
varname not found ==> problem with indexing 
*/
       (void)fprintf(stderr,
        "WARNING: DEP_INVRT_SCL unset; %s unfound in GetPlotAttrIndex\n",
        usrvar[quadrant][trace]);
/*
must, however, consume file input to stay on track
*/
       if (trace)
        (void)fscanf(arg_file,"%d",&temp);
      }
     }
     break;
    case DEP_RANGE:
     for (trace=0; trace<num_of_traces[quadrant]; trace++) {
      if (trace==0)
       (void)sscanf(arg,"%d",&dep_rng_typ[quadrant][0]);
      else
       (void)fscanf(arg_file,"%d",&dep_rng_typ[quadrant][trace]);
      attrnum=GetPlotAttrIndex(usrvar[quadrant][trace],FALSE);
      if (dep_rng_typ[quadrant][trace]==FLOATING) {
       (void)fscanf(arg_file,"%lf",&plus_or_minus);
       if (attrnum!=ERROR) {
        (void)sprintf(&custom_mins[quadrant][attrnum*VALUELEN],"0.0");
        (void)sprintf(&custom_range[quadrant][attrnum*VALUELEN],"%lf",
         -2.0*plus_or_minus);
       } else {
/*
varname not found ==> problem with indexing 
*/
        (void)fprintf(stderr,
         "WARNING: custom range unset for %s: unfound in GetPlotAttrIndex\n",
         usrvar[quadrant][trace]);
       }
      } else {
       (void)fscanf(arg_file,"%lf %lf",&min,&max);
       if (attrnum!=ERROR) {
        (void)sprintf(&custom_mins[quadrant][attrnum*VALUELEN],"%lf",min);
        (void)sprintf(&custom_range[quadrant][attrnum*VALUELEN],"%lf",
         ABS(max-min));
       } else {
/*
varname not found ==> problem with indexing 
*/
        (void)fprintf(stderr,
         "WARNING: custom range unset for %s: unfound in GetPlotAttrIndex\n",
         usrvar[quadrant][trace]);
       }
      }
     }
     break;
    case INDEP_RANGE:
     (void)sscanf(arg,"%d",&indep_rng_typ[quadrant]);
     attrnum=GetPlotAttrIndex(indepvar_name[quadrant],FALSE);
     if (indep_rng_typ[quadrant]==FLOATING) {
       (void)fscanf(arg_file,"%lf",&plus_or_minus);
       if (attrnum!=ERROR) {
        (void)sprintf(&custom_mins[quadrant][attrnum*VALUELEN],"0.0");
        (void)sprintf(&custom_range[quadrant][attrnum*VALUELEN],"%lf",
         -2.0*plus_or_minus);
       } else {
/*
varname not found ==> problem with indexing 
*/
        (void)fprintf(stderr,
         "WARNING: custom range unset for %s: unfound in GetPlotAttrIndex\n",
         indepvar_name[quadrant]);
       }
     } else {
       (void)fscanf(arg_file,"%lf %lf",&min,&max);
       if (attrnum!=ERROR) {
        (void)sprintf(&custom_mins[quadrant][attrnum*VALUELEN],"%lf",min);
        (void)sprintf(&custom_range[quadrant][attrnum*VALUELEN],"%lf",
         ABS(max-min));
       } else {
/*
varname not found ==> problem with indexing 
*/
        (void)fprintf(stderr,
         "WARNING: custom range unset for %s: unfound in GetPlotAttrIndex\n",
         indepvar_name[quadrant]);
       }
     }
     break;
    case TYPE:
     if (!(strcmp(arg,"time_series"))) {
      plot_type[quadrant]=TIME_SERIES;
     } else if (!(strcmp(arg,"track_plot"))) {
      plot_type[quadrant]=TRACK_PLOT;
     } else if (!(strcmp(arg,"skew_t"))) {
      plot_type[quadrant]=SKEW_T;
      autoscale[quadrant]=FALSE;
      set_indepvar_yaxis(quadrant,TRUE);
     } else if (!(strcmp(arg,"sounding"))) {
      plot_type[quadrant]=SOUNDING;
      set_indepvar_yaxis(quadrant,TRUE);
      set_isolate_traces(quadrant,TRUE);
     } else if (!(strcmp(arg,"xy"))) {
      plot_type[quadrant]=XY;
     } else if (!(strcmp(arg,"list_display"))) {
      plot_type[quadrant]=LISTS;
     } else if (!(strcmp(arg,"fixed_display"))) {
      plot_type[quadrant]=FIXED_FORMAT;
     }
     init_state_parms(quadrant,DISPTYPE_XYPLOT);
     init_display_parms(quadrant);
     break;
    case BYPASS: /* a maximum has been exceeded; accept no more args until
                    next valid keyword is detected */
     break;
   }
 } 
 (void)WINDSfclose(arg_file);
 return TRUE;
}

/**********************  SetDeletePlotspec()  **********************************/

SetDeletePlotspec(value)
int value;
{
 DeletePlotspec=value;
}

/**********************  GetDeletePlotspec()  **********************************/

GetDeletePlotspec()
{
 return DeletePlotspec;
}

/******************  GetPendingDescription()  **********************************/

char *
GetPendingDescription()
{
 return description;
}

/******************  SetPendingDescription()  **********************************/

SetPendingDescription(string)
char *string;
{
 (void)sprintf(description,"%s",(char *)TrimTrailingBlanks(string));
}

/***********************  SetNeedPlotspexPassword()  *************************/

SetNeedPlotspexPassword(value)
int value;
{
 NeedPlotspexPassword=value;
}

/***********************  GetNeedPlotspexPassword()  *************************/

GetNeedPlotspexPassword()
{
 return NeedPlotspexPassword;
}

/***********************  GetPlotspexPassword()  *************************/

GetPlotspexPassword()
{
 return TRUE;
}


/****************************  SAVE_SETUP()  **********************************/

save_setup(quadrant)
int quadrant;
{
char *s1,*string;               /* strings in plotspex filename */
int current_entry;  		/* number of last entry in file */

int overwrite_entry;		/* TRUE when user request to overwrite an 
				entry with duplicate description */
int EntryStart,			/* starting point in string of current entry */
    start,			/* index into string being saved */
    start_next_entry,		/* place in string where entry after duplicate
				entry begins */
    got_next_entry;		/* TRUE when start_next_entry has been 
				determined */
/*
check for need for password
*/
if (GetNeedPlotspexPassword()) {
 if (GetPlotspexPassword()==FALSE) {
  return;
 }
}
/*
if this func called with ERROR argument, it means we need to DELETE the setup
described by GetPendingDescription()
*/
if (quadrant==ERROR) {
 SetDeletePlotspec(TRUE);
 quadrant=0;
}
/* 
open plotspex file, apply advisory lock
*/
overwrite_entry=ERROR;
start=current_entry=0;
if ( (SetupFileLock=open(GetPlotspexFilename(),O_WRONLY)) == ERROR) {
 report(quadrant,"WARNING: no setup file lock created !!!");
}
if ( (LockFile(SetupFileLock,0))==ERROR) 
 report(quadrant,"WARNING: file lock failed for setup file!!!");
/* 
get length of entire file, create string to hold it
*/
string=(char *)malloc((unsigned int)lseek(SetupFileLock,(off_t)0,SEEK_END)
/*
plus one for null terminator at end
*/
 +1
/*
plus one for each line feed added, one at end of as many as MaxSetups lines 
*/
 + get_maxsetups());
/* 
open file and start reading it in
*/
if ( (SetupFile=WINDSfopen(GetPlotspexFilename(),"r")) == NULL) {
 report(quadrant,"WARNING: no setup file created !!!");
} else {
 while (fscanf(SetupFile,"%s",&string[start]) != EOF) {
  if (!(strncmp(&string[start],FIRST_WORD,strlen(FIRST_WORD))))  {
/* 
this is the start of a new entry
*/
   current_entry++;
   EntryStart=start;
  }
  if (!(strcmp(&string[start],"DESCRIPTION"))) {
   char *tempdesc;
/* 
compare just read-in description with new one
*/
   if (!strcmp(GetPendingDescription(),
/*
tell GetDescriptionFromFile() to skip leading spaces in description
*/
       (tempdesc=(char *)GetDescriptionFromFile(SetupFile,TRUE)))) {
/*
we found a matching description in the file, as given in global char. array
description. Assume not to overwrite or delete it until instructed to do so by 
user.
*/
    overwrite=FALSE;
/*
call error proc directly instead of setting flag for time-out routine, since
we are looking for user response here immediately
*/
    notify_user_error(
     GetDeletePlotspec()?REMOVE_PLOTSPEX_ENTRY:DUPLICATE_PLOTSPEX_ENTRY);
    if (overwrite) {
/* 
user acknowledged a go-ahead on overwrite or delete
*/
     if (GetDeletePlotspec()) {
/*
we are in the process of deletion, not saving...adjust accordingly
Value of overwrite_entry is last character in string that will be written
to file, assuming enter_new_plotspec() will finish it off.
*/
      overwrite_entry=EntryStart;
     } else {
/*
at this point, value of start points to string just before keyword DESCRIPTION
appears. Adjust overwrite_entry to accommodate the keyword, a space, the 
description, and one more space:
*/
      overwrite_entry=start+strlen(tempdesc)+strlen(" DESCRIPTION ");
     }
     got_next_entry=FALSE;
    } else {
/* 
user gave it up: clean up and return
*/
     unlock_spexfiles(string);
     SetDeletePlotspec(FALSE);
     return;
    }
   }
/*
copy description back to ongoing string
*/
   (void)sprintf(&string[strlen(string)]," %s ",tempdesc);
  }
  if (!(strcmp(&string[start],"END_ARGS"))) {
/* 
then this is start of at least the 2nd entry; precede it with a new-line
*/
   (void)sprintf(&string[strlen(string)],"\n");
   if (overwrite_entry!=ERROR && !got_next_entry) {
/* 
there is a pending overwrite entry; establish start point in ongoing string
of where to resume when re-writing all of it back out to file
*/
    got_next_entry=TRUE;
    start_next_entry=strlen(string);
   }
  } else {
/*
in between words, add a space
*/
   (void)sprintf(&string[strlen(string)]," ");
  }
/*
keep track of next point in string to write to
*/
  start=strlen(string);
 }
}
(void)WINDSfclose(SetupFile);
if (overwrite_entry!=ERROR) {
/* 
overwrite entry with same description
*/
 if ( (SetupFile=WINDSfopen(GetPlotspexFilename(),"w")) == NULL) {
  report(quadrant,"WARNING: couldn't open plotspex file for writing");
  unlock_spexfiles(string);
  SetDeletePlotspec(FALSE);
  return;
 }
/* 
create and write out the first segment before the overwrite target
*/
 s1=(char *)malloc((unsigned int)(overwrite_entry+1));
 (void)strncpy(s1,string,overwrite_entry);
 (void)sprintf(&s1[overwrite_entry],"\0");
 (void)fprintf(SetupFile,"%s",s1);
 (void)free(s1);
/* 
now either write out the new entry or ignore it, effectively deleting it
*/
 time_to_log();
 if (GetDeletePlotspec()) {
  (void)fprintf(stderr,"Delete setup '%s'\n",
   (char *)GetPendingDescription());
 } else {
  (void)fprintf(stderr,"Overwrite setup '%s' with new parameters\n",
   (char *)get_description(quadrant));
  enter_new_plotspec(quadrant,current_entry+1,FALSE);
 }
/* 
if this was not the last one, write out the remainder of the ongoing string, i.e.
the remaining plotspec entries
*/
 if (start_next_entry<=strlen(string))
  (void)fprintf(SetupFile,"%s",&string[start_next_entry]);
} else { 
 if (GetDeletePlotspec()) {
/*
a possible timing scenario allows session A to delete a setup before session B
is aware of it and has already requested to delete the same one; in which case,
session B will NOT find it, the value of overwrite_entry will be ERROR and the
process will come here thinking it's time to save that setup! Preclude that
possibility with a check here. On next time cycle through catchalrm, session B
WILL receive notification of folder change and will update its notion of the 
plotspex file.
*/
  report(quadrant,"WARNING: couldn't find setup to be deleted");
  SetDeletePlotspec(FALSE);
  unlock_spexfiles(string);
  return;
 }
/* 
append new entry with unique description
*/
 if ( (SetupFile=WINDSfopen(GetPlotspexFilename(),"a+")) == NULL) {
  report(quadrant,"WARNING: couldn't open plotspex file for appending");
  unlock_spexfiles(string);
  return;
 }
/* 
update op-panel text item's notion of this window's current description
*/
 set_description(quadrant,description);
 time_to_log();
 (void)fprintf(stderr,"Save new setup '%s'\n",
  (char *)get_description(quadrant));
 enter_new_plotspec(quadrant,current_entry+1,TRUE);
}

if (!GetDeletePlotspec())
 display_saved_feedback(quadrant,TRUE);
else
 SetDeletePlotspec(FALSE);
unlock_spexfiles(string);
SetNeedInitPlotspex(TRUE);
SetFolderChanged(TRUE);
}

/*************************  UNLOCK_SPEXFILES()  ******************************/

unlock_spexfiles(s)
char *s;
/* 
cleanup malloc()'s, close files and release advisory lock
*/
{
 free((char *)s);
 (void)WINDSfclose(SetupFile);
 (void)UnlockFile(SetupFileLock,0);
 (void)close(SetupFileLock);
}

/**************************  ENTER_NEW_PLOTSPEC()  ****************************/

enter_new_plotspec(quadrant,next_entry,print_desc)
int quadrant,next_entry;
{
int attrnum,indx;                      	/* loop index */

/* write out new plotspex entry based on process' config variables */

 switch (window_type[quadrant]) {

  case DISPTYPE_ETC:
   if (print_desc)
/* 
description
*/
    (void)fprintf(SetupFile,
    "%s%d etcetera DESCRIPTION %s ",FIRST_WORD,next_entry,description);
/* 
geometry, display window type, dependent axis variable, type of variables
*/
   (void)fprintf(SetupFile,"ARGS DATESAVED %s FLIGHTSAVED %s TYPE video CHANNEL %d ",
    GetCurrentDate(),GetCurrentFlight(),GetQuadrantVideoSource(quadrant));
   break;
  case DISPTYPE_XYPLOT:
  case DISPTYPE_LISTS:
  case DISPTYPE_FIXEDFORMAT:
  case WTYPE_SKEWT:
   if (print_desc)
/* 
description
*/
    (void)fprintf(SetupFile,
    "%s%d xy_plt DESCRIPTION %s ",FIRST_WORD,next_entry,description);
/* 
geometry, display window type, dependent axis variable, type of variables
*/
   (void)fprintf(SetupFile,"ARGS DATESAVED %s FLIGHTSAVED %s GEOMETRY %d TYPE ",
    GetCurrentDate(),GetCurrentFlight(),get_geom(quadrant));
   switch (plot_type[quadrant]) {
    case TIME_SERIES:
     (void)fprintf(SetupFile,"time_series TIME_RANGE %d TRACES ",xaxis_seconds[quadrant]);
     break;
    case TRACK_PLOT:
     (void)fprintf(SetupFile,"track_plot INDEP_VAR %s TRACES ",indepvar_name[quadrant]);
     break;
    case SKEW_T:
     (void)fprintf(SetupFile,"skew_t INDEP_VAR %s TRACES ",indepvar_name[quadrant]);
     break;
    case SOUNDING:
     (void)fprintf(SetupFile,"sounding INDEP_VAR %s TRACES ",indepvar_name[quadrant]);
     break;
    case XY:
     (void)fprintf(SetupFile,"xy INDEP_VAR %s TRACES ",indepvar_name[quadrant]);
     break;
    case LISTS:
     (void)fprintf(SetupFile,"list_display LISTEES ");
     break;
    case FIXED_FORMAT:
     (void)fprintf(SetupFile,"fixed_display FIXEES ");
     break;
   }
   switch (plot_type[quadrant]) {
    case TRACK_PLOT:
    case TIME_SERIES:
    case SKEW_T:
    case SOUNDING:
    case XY:

/* variable names
*/
     for (indx=0; indx<num_of_traces[quadrant]; indx++)
      (void)fprintf(SetupFile,"%s ",trace_name[quadrant][indx]);
     if (plot_type[quadrant]==SKEW_T) {
/* 
define ranges
*/
      (void)fprintf(SetupFile," SKEW_CORNER %lf %lf ",
       skew_minpress[quadrant],skew_mintemp[quadrant]);
     } else {
      (void)fprintf(SetupFile," DEP_RANGE ");
      for (indx=0; indx<num_of_traces[quadrant]; indx++) {
       (void)fprintf(SetupFile,"%d ",dep_rng_typ[quadrant][indx]);
       if (dep_rng_typ[quadrant][indx]==FLOATING) {
        (void)fprintf(SetupFile,"%lf ",
         dep_intrvl[quadrant][indx]*((NUM_TIX-1)/2));
       } else {
        (void)fprintf(SetupFile,"%lf %lf ",
         dep_min[quadrant][indx],dep_max[quadrant][indx]);
       }
      }
      if (plot_type[quadrant]!=TIME_SERIES) {
/* 
independent variable inverted scale
*/
       attrnum=GetPlotAttrIndex(indepvar_name[quadrant],FALSE);
       (void)fprintf(SetupFile," INDEP_INVRT_SCL %d INDEP_RANGE %d ",
         attrnum!=ERROR?indep_invert_scale[quadrant][attrnum]:0,
         indep_rng_typ[quadrant]);
       if (attrnum==ERROR) {
/*
varname not found ==> problem with indexing 
*/
        (void)fprintf(stderr,
         "WARNING: INDEP_INVRT_SCL unsaved; %s: no find in GetPlotAttrIndex\n",
         indepvar_name[quadrant]);
       }
       if (indep_rng_typ[quadrant]==FLOATING) {
        (void)fprintf(SetupFile,"%lf ",
         indep_intrvl[quadrant]*((NUM_TIX-1)/2));
       } else {
        (void)fprintf(SetupFile,"%lf %lf ",
         indep_min[quadrant],indep_max[quadrant]);
       }
/*
independent var on y-axis
*/
       (void)fprintf(SetupFile," INDEPVAR_YAXIS %d ",
        get_indepvar_yaxis(quadrant));
       if (plot_type[quadrant]==TRACK_PLOT) {
        int vectors_on;
/* 
vectors on/off
*/
        vectors_on=FALSE;
        (void)fprintf(SetupFile," VECTORS_ON ");
        for (indx=0; indx<num_of_traces[quadrant]; indx++) {
         if (get_show_vector(quadrant,indx)) {
          (void)fprintf(SetupFile,"%d ",TRUE);
          vectors_on=TRUE;
         } else {
          (void)fprintf(SetupFile,"%d ",FALSE);
         }
        }
/*
other vector parameters
*/
        (void)fprintf(SetupFile," VECTORS %d ",get_vector_interval(quadrant));
        (void)fprintf(SetupFile,"%d ",GetVectorMaxScale(quadrant));
        (void)fprintf(SetupFile,"%s %s ",
         (char *)get_x_component(quadrant),
         (char *)get_y_component(quadrant));
/* 
vectors colors
*/
        (void)fprintf(SetupFile," VECTORS_CONTRAST ");
        if (get_vector_color(quadrant)==CONTRAST_TRACE)
         (void)fprintf(SetupFile,"%d ",TRUE);
        else
         (void)fprintf(SetupFile,"%d ",FALSE);
       }
      }
/* 
dependent variable inverted scale
*/
      (void)fprintf(SetupFile," DEP_INVRT_SCL ");
      for (indx=0; indx<num_of_traces[quadrant]; indx++) {
       attrnum=GetPlotAttrIndex(usrvar[quadrant][indx],FALSE);
       if (attrnum!=ERROR) {
        if (dep_invert_scale[quadrant][attrnum] !=1 && 
            dep_invert_scale[quadrant][attrnum] != 0) {
/*
there's a bug somewhere munging value of dep_invert_scale[quadrant][attrnum];
catch it for now and force a reasonable value for the variable, which usually
will be zero. Previous lack of checking for attrnum==ERROR was possibly the
culprit.
*/
         (void)fprintf(stderr,
          "SAVE_SETUP WARNING: dep_invert_scale=%d, quadrant %d, attrnum %d\n",
	   dep_invert_scale[quadrant][attrnum],quadrant,attrnum);
         (void)fprintf(stderr,"  ...changing it to 0\n");
	 dep_invert_scale[quadrant][attrnum]=0;
        }
       (void)fprintf(SetupFile,"%d ",dep_invert_scale[quadrant][attrnum]);
       } else {
/*
varname not found ==> problem with indexing 
*/
        (void)fprintf(stderr,
         "WARNING: DEP_INVRT_SCL unsaved; %s: unfound in GetPlotAttrIndex\n",
         usrvar[quadrant][indx]);
        (void)fprintf(SetupFile,"0 ");
       }
      }
/* 
autoscale
*/
      (void)fprintf(SetupFile," CLIPPING ");
      if (autoscale[quadrant])
       (void)fprintf(SetupFile,"%lf %d %d ",
        tolerance_factor[quadrant],clip_chk_interval[quadrant],
        seconds_to_avg[quadrant]);
      else
       (void)fprintf(SetupFile,"-1 -1 -1 ");
/* 
same scale
*/
      (void)fprintf(SetupFile," SAME_SCALE ");
      if (get_same_scale(quadrant))
       (void)fprintf(SetupFile,"%d ",TRUE);
      else
       (void)fprintf(SetupFile,"%d ",FALSE);
/* 
isolation of traces
*/
      (void)fprintf(SetupFile," ISOLATE_TRACES ");
      if (get_isolate_traces(quadrant))
       (void)fprintf(SetupFile,"%d ",TRUE);
      else
       (void)fprintf(SetupFile,"%d ",FALSE);
    }
    break;
   case LISTS:
/* 
variable names
*/
    for (indx=0; indx<num_of_listees[quadrant]; indx++)
     (void)fprintf(SetupFile,"%s ",(char *)GetListeeName(quadrant,indx));
    (void)fprintf(SetupFile," LIST_RATE %d ",GetListRate(quadrant));
    break;
   case FIXED_FORMAT:

/* variable names
*/
    for (indx=0; indx<num_of_fixees[quadrant]; indx++)
     (void)fprintf(SetupFile,"%s ",(char *)GetFixeeName(quadrant,indx));
    break;
   }
   break;

  case DISPTYPE_PMS1D:

   if (print_desc)
/* 
description
*/
    (void)fprintf(SetupFile,
    "%s%d pms1d DESCRIPTION %s ",FIRST_WORD,next_entry,description);
/* 
geometry
*/
   (void)fprintf(SetupFile,"ARGS DATESAVED %s FLIGHTSAVED %s GEOMETRY %d ",
    GetCurrentDate(),GetCurrentFlight(),get_geom(quadrant));
   (void)fprintf(SetupFile,
    " NPLPROBES %d PLPROBES ",PMS1Dsetup[quadrant].nprobes);
    for (indx=0; indx<PMS1Dsetup[quadrant].nprobes; indx++)
     (void)fprintf(SetupFile,"%s ",PMS1Dprobe[PMS1Dsetup[quadrant].probenum[indx]].name);
   (void)fprintf(SetupFile,
    " DATATYPE %d  YSCALE %d  XTYPE %d  YTYPE %d  YMIN %f  YMAX %f  SUM_PERIOD %d ",
    PMS1Dsetup[quadrant].type,  PMS1Dsetup[quadrant].volume,
    PMS1Dsetup[quadrant].xtype, PMS1Dsetup[quadrant].ytype,
    PMS1Dsetup[quadrant].ymin,  PMS1Dsetup[quadrant].ymax,
    PMS1Dsetup[quadrant].sum_period);
    break;

  case DISPTYPE_PMS2D:
   
   if (print_desc)
/* 
description
*/
    (void)fprintf(SetupFile,
    "%s%d pms2d DESCRIPTION %s ",FIRST_WORD,next_entry,description);
/* 
geometry, 2d window type
*/
   (void)fprintf(SetupFile,"ARGS DATESAVED %s FLIGHTSAVED %s GEOMETRY %d TYPE ",
    GetCurrentDate(),GetCurrentFlight(),get_geom(quadrant));
   (void)fprintf(SetupFile, 
    "%s NPLPROBES %d ",
           GetPms2dDispType(quadrant)==PMS2D_STANDARD?"standard":
            (GetPms2dDispType(quadrant)==PMS2D_GREYSCALE?"greyscale":"hvps"),
           num_pms2d_probes(quadrant));
   if (num_pms2d_probes(quadrant)) {
    (void)fprintf(SetupFile," PLPROBES ");
    for (indx=0; indx<num_pms2d_probes(quadrant); indx++) {
     if (pms2d_probe_selected(quadrant,indx))
      (void)fprintf(SetupFile,"%s ",(char *)get_2d_probename(quadrant,indx));
    }
   }
   (void)fprintf(SetupFile," DISP_RATE %d ",disp_rate[quadrant]);
 }
 (void)fprintf(SetupFile,"END_ARGS\n");
}

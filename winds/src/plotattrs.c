#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <X11/Xlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <display.h>
#include <files.h>
#include <constants.h>
#include <help.h>
#include <lengths.h>
#include <xy_plt.h>
#include <errors.h>
#include <plotattrs.h>
#include <proto.h>
#include <vardb.h>

char *custom_range[NUM_QUADRANTS],*custom_mins[NUM_QUADRANTS];

static int num_attr;			/* #nentries in plot_attr file */

int *dep_invert_scale[NUM_QUADRANTS];
int *indep_invert_scale[NUM_QUADRANTS];

static char *attrname;                 /* plot_attr file fields */
static int SizeOfAttrDatabase;
char *units,
     *default_range,
     *default_mins;

/* position of [in]dependent variable names within plot_attr file 
*/
int indep_attr_num[NUM_QUADRANTS];
int dep_attr_num[NUM_QUADRANTS][MAX_DISPLAY_VARS];

struct varattrs {
 char VarName[VARDB_NL];
 char Units[VARDB_UL];
 char Title[VARDB_TL];
 int RangeIsFixed;
 int RangeIsFloat;
 double PlusOrMinus;
 double TotalRange;
 double RangeMinimum;
 double RangeMaximum;
};

struct varattrs *VarAttrs;

/**************************   GetPlotAttrName()  *******************************/

char *
GetPlotAttrName(index)
int index;
/*
names as listed in plot_attr file
*/
{
 return &attrname[index*VARDB_NL];
}

/**************************  GetVarListName ()  *******************************/

char *
GetVarListName(index)
int index;
/*
names as listed by raw and derived variable lists
*/
{
static char varname[NAMLEN];

 if (index<*len_raw) {
  (void)sprintf(varname,"%s",&raw_list[NAMLEN*index]);
 } else if (index<(*len_raw+*len_derive)) {
  (void)sprintf(varname,"%s",&derive_list[NAMLEN*(index-*len_raw)]);
/*
provide for BITS and VOLTAGE with use of VarDB only
*/
 } else if (GetVarDataBase() && index==*len_raw+*len_derive) {
  (void)sprintf(varname,"BITS");
 } else if (GetVarDataBase() && index==*len_raw+*len_derive+1) {
  (void)sprintf(varname,"VOLTAGE");
 } else {
  return NULL;
 }
/***
 (void)sprintf(varname,"%s",(index<*len_raw?&raw_list[NAMLEN*index]:
  &derive_list[NAMLEN*(index-*len_raw)]));
***/
 return varname;
}

/****************************   GetVarAttrName()  *******************************/

char *
GetVarAttrName(index)
int index;
/*
names listed either from raw/derived lists and filled in from VarDB, or, if
VarDB not around, from plot-attrs file and filled in from that and bulletin-9
*/
{
 return VarAttrs[index].VarName;
}

/**************************   GET_VARTITLE()  ********************************/
 
char *
get_vartitle(index)
int index;
{
 if (!GetVarDataBase()){
/*
in this and other funcs using plot-attrs database, adjust index to ordinal
position in that plot-attrs array, assuming calls made here using varlist
indexing
*/
  if ((index=GetPlotAttrIndex(GetVarListName(index),FALSE))==ERROR) 
  return "No Title";
 }
 return VarAttrs[index].Title;
}

/**************************   GET_VARUNITS()  ********************************/
 
char *
get_varunits(index)
int index;
{
 if (!GetVarDataBase()){
  if ((index=GetPlotAttrIndex(GetVarListName(index),TRUE))==ERROR) {
   return "unk";
  } 
 }
 return &units[index*UNITSLEN];
}

/**********************   GET_VAR_RNG_IS_FIXED()  ****************************/
 
int
get_var_rng_is_fixed(index)
int index;
{
 if (!GetVarDataBase()) {
  if ((index=GetPlotAttrIndex(GetVarListName(index),TRUE))==ERROR) {
   return FALSE;
  } 
 }
 return VarAttrs[index].RangeIsFixed;
}

/**************************   GET_VAR_RNGMIN()  *****************************/
 
double
get_var_rngmin(index)
int index;
{
 if (!GetVarDataBase()) {
  if ((index=GetPlotAttrIndex(GetVarListName(index),TRUE))==ERROR) {
   return 0.0;
  } 
 }
 return VarAttrs[index].RangeMinimum;
}

/**************************   GET_VAR_RNGMAX()  *****************************/
 
double
get_var_rngmax(index)
int index;
{
 if (!GetVarDataBase()) {
  if ((index=GetPlotAttrIndex(GetVarListName(index),TRUE))==ERROR) {
   return 0.0;
  } 
 }
 return VarAttrs[index].RangeMaximum;
}

/***********************   GetTotalRange()  *************************/
 
double
GetTotalRange(index)
int index;
{
 if (!GetVarDataBase()) {
  if ((index=GetPlotAttrIndex(GetVarListName(index),TRUE))==ERROR) {
   return 200.0;
  } 
 }
 return VarAttrs[index].TotalRange;
}

/***********************   GET_VAR_PLUS_OR_MINUS()  *************************/
 
double
get_var_plus_or_minus(index)
int index;
{
 if (!GetVarDataBase()) {
  if ((index=GetPlotAttrIndex(GetVarListName(index),TRUE))==ERROR) {
   return 100.0;
  } 
 }
 return VarAttrs[index].PlusOrMinus;
}

/****************************  GET_INDEP_ATTR_NUM()  *************************/ 

get_indep_attr_num(quadrant) 
int quadrant;
{
 return indep_attr_num[quadrant];
}
 
/****************************  SET_INDEP_ATTR_NUM()  *************************/ 

set_indep_attr_num(quadrant,value) 
int quadrant,value;
{
 indep_attr_num[quadrant]=value;
}

/****************************  GET_DEP_ATTR_NUM()  ****************************/

get_dep_attr_num(quadrant,which)
int quadrant,which;
{
 return dep_attr_num[quadrant][which];
}

/****************************  SET_DEP_ATTR_NUM()  ****************************/

set_dep_attr_num(quadrant,which,value)
int quadrant,which,value;
{
 dep_attr_num[quadrant][which]=value;
}

/*************************  INSTALL_NEW_ATTR()  ******************************/

install_new_attr(plottype,new_name,new_units,new_range,new_minimum)
int plottype;
char *new_name,*new_units,*new_range,*new_minimum;

/* add a new entry to memory and disk versions of plot attributes, as 
   specified with name, units, range, and minimum.
*/
{
int quadrant;

/* ensure maximum won't be exceeded
*/
 if (num_attr==MAX_ATTR) {
  (void)fprintf(stderr,"WARNING: can't add new default attribute for %s: increase MAX_ATTR in plotattrs.h, or reduce size of attributes file\n",new_name);
  (void)sprintf(error_info,"%s.  Contact a WINDS expert about this alert, and ignore any other alerts immediately following.",errmsg);
  set_notice_needed(TOO_MANY_PLOTATTRS);
  return ERROR;
 }

 if (plottype!=CALIBRATION)
  add_entry_to_file(PROJ_ATTR,new_name,new_units,new_range,new_minimum);
 else
  need_calib_attr=TRUE;

 (void)sprintf(&attrname[num_attr*VARDB_NL],"%s",new_name);
 (void)sprintf(&units[num_attr*UNITSLEN],"%s",new_units);
 (void)sprintf(&default_range[num_attr*VALUELEN],"%s",new_range);
 (void)sprintf(&default_mins[num_attr*VALUELEN],"%s",new_minimum);
 for (quadrant=0; quadrant<get_num_displays(); quadrant++) {
  (void)sprintf(&custom_range[quadrant][num_attr*VALUELEN],"%s",new_range);
  (void)sprintf(&custom_mins[quadrant][num_attr*VALUELEN],"%s",new_minimum);
 }

/* return position of new entry
*/
 return ++num_attr-1;
}

/****************************  GET_PLOT_ATTR()  ******************************/

get_plot_attr(quadrant,mode,varname)
int quadrant,mode;
char varname[];
/* given the quadrant, mode, and name of variable, attempt to get its 
   plot attribute index # in plot-attrs array.  Perform error checking; 
   prompt for new entry values if plot_type is CALIBRATION; look for DEFAULT 
   entry if no value found; return ERROR if no DEFAULT entry found.
*/ 
{
int attr_index;
static double range;

  if ( (attr_index= GetPlotAttrIndex(mode==VOLTAGE?"VOLTAGE":
                                  (mode==BITS?"BITS":varname),TRUE)) == ERROR) {
   return(ERROR);
/***
   attr_index=install_new_attr(plot_type[quadrant],
    varname,&units[attr_index*UNITSLEN],
    &default_range[attr_index*VALUELEN],
    &default_mins[attr_index*VALUELEN]);
***/
  }
  (void)sscanf(&default_range[attr_index*VALUELEN],"%lf",&range);
  if (!range) {
/*
this is not a good thing at all
*/
   (void)sprintf(error_info,"WARNING: a range of ZERO has been requested for %s. This is likely due to an erroneous entry in the Plot Attributes File, and is possibly not the only entry with that problem. You are advised to examine and edit that file to correct this problem. In the meantime, a default (or a previously customized) range will be used for display of that variable.", &attrname[attr_index*VARDB_NL]);
   (void)fprintf(stderr,"INVALID Plot Attribute entry for %s: range is zero\n",
    &attrname[attr_index*VARDB_NL]);
   set_notice_needed(INVALID_PLOTATTR);
/*
check whether a custom range (either explicitly by user IN THIS QUADRANT ONLY, 
or via a saved plotspex entry) has already been set; if so, use it, else use a 
default range
*/
   (void)sscanf(&custom_range[quadrant][attr_index*VALUELEN],"%lf",&range);
   if (!range) {
    if ( (attr_index=GetPlotAttrIndex("DEFAULT",FALSE))==ERROR) {
     return(ERROR);
    }
   }
  }  
  return(attr_index);
}

/****************************  ADD_ENTRY_TO_FILE()  **************************/

add_entry_to_file(filepath,name,Units,range,minimum)
char *filepath,*name,*Units,*range,*minimum;
{
FILE *new_plot_attr;
static char filename[MAX_FLNM_LNGTH];
int new_plot_attr_lock;

/* open plot attributes file in append mode 
*/
 (void)sprintf(filename,filepath,ProjectDirectory,proj_number);

 if ( (new_plot_attr=WINDSfopen(filename,"a+")) == NULL ) {
  (void)WINDSfclose(new_plot_attr);
  time_to_log();
  (void)fprintf(stderr,"FATAL: unable to open attr file %s \n",filename);
  RequestAlertNow("FATAL: couldn't open plot attr file...contact WINDS expert with this information!");
  winds_cleanup(ERROR);
 }

/* provide exclusive access
*/
 if ( (new_plot_attr_lock=open(filename,O_WRONLY)) == ERROR) {
  (void)fprintf(stderr,"WARNING: no new-plot-attr file lock created !!!\n");
 }
 if (LockFile(new_plot_attr_lock,0)==ERROR)
  (void)fprintf(stderr,"WARNING: file lock failed for new_plot_attr !!!\n");

/* add the new entry at end of file
*/
 (void)fprintf(new_plot_attr,ATTRS_FORMAT,
  name,Units,range,minimum);
 (void)fprintf(new_plot_attr,"\n");
 (void)fflush(new_plot_attr);

/* release lock
*/
 (void)UnlockFile(new_plot_attr_lock,0);
 (void)close(new_plot_attr_lock);
 (void)WINDSfclose(new_plot_attr);
}

/****************************  chg_plot_attrs()  ****************************/
 
chg_plot_attrs(type)
int type;
{
int j,quadrant;
int num_attrs;                 /* number of entries in plot_attr file */
/* 
get default plot attributes file read in
*/
 num_attrs=get_plot_attrs(type);
/*
reset customized ranges to newly read in defaults
*/ 
 for (j=0; j<num_attrs; j++) {
  for (quadrant=0; quadrant<NUM_QUADRANTS; quadrant++) {
   (void)sprintf(&custom_range[quadrant][j*VALUELEN],
    "%s",&default_range[j*VALUELEN]);
   (void)sprintf(&custom_mins[quadrant][j*VALUELEN],
    "%s",&default_mins[j*VALUELEN]);
  }
 }
}

/*************************  GET_PLOT_ATTRS()  ********************************/

get_plot_attrs(type)
int type;
/* read in plot attributes from filename implied from type. If project-specific attributes
file not there, make a copy of it from master list. Return number of specifications read in.
*/
{
static char filename[MAX_FLNM_LNGTH];  /* filename constructed from proj. # */
static char def_name[MAX_FLNM_LNGTH];  /* filename constructed from proj. # */
FILE *fd;
static int allocated=FALSE;
int quadrant;

if (!GetVarDataBase() || type==CALIBRATION) {
/*
for now, use old attrs file for CALIBRATION displays
*/
 switch (type) {
 case CALIBRATION:
  (void)sprintf(filename,PROJ_CALIB_ATTR,ProjectDirectory,proj_number);
  break;
 case TIME_SERIES:
  (void)sprintf(filename,PROJ_ATTR,ProjectDirectory,proj_number);
  break;
 }

 if ((fd=WINDSfopen(filename,"r")) == NULL) {
  (void)WINDSfclose(fd);
  (void)fprintf(stderr,"No proj-specific plot_attrs; using master\n");
  switch (type) {
  case CALIBRATION:
   (void)sprintf(def_name,CALIB_ATTR,ProjectDirectory);
   break;
  case TIME_SERIES:
   (void)sprintf(def_name,PLOT_ATTR,ProjectDirectory);
   break;
  }
  if ((fd=WINDSfopen(def_name,"r")) == NULL) {
   (void)fprintf(stderr,"No master plot_attrs; exiting...\n");
   RequestAlertNow("FATAL: no master plot attr file...contact WINDS expert with this information!");
   winds_cleanup(ERROR);
  }
 }
}
/* 
allocate the space if not already done
*/
 if (!allocated) {
  if (!GetVarDataBase() || type==CALIBRATION) 
   SizeOfAttrDatabase=MAX_ATTR;
  else
   SizeOfAttrDatabase=(*len_raw+*len_derive+2 /* BITS and VOLTAGE */);
  VarAttrs=(struct varattrs *)(calloc((unsigned)SizeOfAttrDatabase,
   sizeof(struct varattrs)));
  attrname=(char *)(calloc((unsigned)SizeOfAttrDatabase*VARDB_NL,
   sizeof(char)));
  units=(char *)(calloc((unsigned)SizeOfAttrDatabase*UNITSLEN,
   sizeof(char)));
  default_range=(char *)(calloc((unsigned)SizeOfAttrDatabase*VALUELEN,
   sizeof(char)));
  default_mins=(char *)(calloc((unsigned)SizeOfAttrDatabase*VALUELEN,
   sizeof(char)));
  for (quadrant=0; quadrant<NUM_QUADRANTS; quadrant++) {
   custom_range[quadrant]=(char *)(calloc(SizeOfAttrDatabase*VALUELEN,
    sizeof(char)));
   custom_mins[quadrant]=(char *)(calloc(SizeOfAttrDatabase*VALUELEN,
    sizeof(char)));
  }
  for (quadrant=0; quadrant<NUM_QUADRANTS; quadrant++) {
   dep_invert_scale[quadrant]=(int *)
    (calloc((unsigned)SizeOfAttrDatabase,sizeof(int)));
   indep_invert_scale[quadrant]=(int *)
    (calloc((unsigned)SizeOfAttrDatabase,sizeof(int)));
  }
  allocated=TRUE;
 } else {
/*
the calloc() call inits these to zeroes, but init again on subsequent calls
since this function may be called again when user toggles between normal 
displays and the calibration windows. Each of these display window types uses
a different plot_attr database.
*/
  for (quadrant=0; quadrant<NUM_QUADRANTS; quadrant++) {
   for (num_attr=0; num_attr<SizeOfAttrDatabase; num_attr++) {
    dep_invert_scale[quadrant][num_attr]=FALSE;
    indep_invert_scale[quadrant][num_attr]=FALSE;
   }
  }
 }
/* 
read 'em in
*/
if (!GetVarDataBase() || type==CALIBRATION) {
 (void)fprintf(stderr,
  "WINDS: either we're setting up CALIB attrs, or no VarDB file found \n");
 for (num_attr=0; 
  fscanf(fd,ATTRS_FORMAT,
  &attrname[num_attr*VARDB_NL],&units[num_attr*UNITSLEN],
  &default_range[num_attr*VALUELEN],
  &default_mins[num_attr*VALUELEN]) 
  != EOF; num_attr++) {
  if (num_attr == MAX_ATTR) {
   time_to_log();
   (void)fprintf(stderr,
    "FATAL: increase MAX_ATTR in files.h, or reduce size of plot_attr file\n");
   (void)sprintf(error_info,"FATAL: increase Maximum Attributes value, or reduce size of Plot Attributes file.  Contact a WINDS expert about this alert, and ignore any other alerts immediately following.");
   notify_user_error(TOO_MANY_PLOTATTRS);
   winds_cleanup(ERROR);
  }
  (void)sprintf(VarAttrs[num_attr].VarName,"%s",
   (char *)&attrname[num_attr*VARDB_NL]);
  (void)sprintf(VarAttrs[num_attr].Units,"%s",
   (char *)&units[num_attr*UNITSLEN]);
/*
start with no title for now; on creation of bulletin 9 category menu, available
titles will override this
*/
  (void)sprintf(VarAttrs[num_attr].Title,"No Title");
  (void)sscanf(&default_range[num_attr*VALUELEN],"%lf",
   &VarAttrs[num_attr].TotalRange);
/*
major kluge that should be done away with: negative range ==> floating type
in the plot-attrs file
*/
  VarAttrs[num_attr].RangeIsFloat=VarAttrs[num_attr].TotalRange<0;
  VarAttrs[num_attr].RangeIsFixed=!VarAttrs[num_attr].RangeIsFloat;
  if (VarAttrs[num_attr].RangeIsFloat) {
   VarAttrs[num_attr].RangeMinimum=0.00;
   VarAttrs[num_attr].RangeMaximum=0.00;
   VarAttrs[num_attr].PlusOrMinus=ABS(VarAttrs[num_attr].TotalRange/2.0);
/*
correct appearance of range for user's sake
*/
   VarAttrs[num_attr].TotalRange*=-1;
  } else {
   VarAttrs[num_attr].PlusOrMinus=0.00;
   (void)sscanf(&default_mins[num_attr*VALUELEN],"%lf",
    &VarAttrs[num_attr].RangeMinimum);
   VarAttrs[num_attr].RangeMaximum=VarAttrs[num_attr].RangeMinimum+
    VarAttrs[num_attr].TotalRange;
  }
 }
 (void)WINDSfclose(fd);
} else {
 char *varname;
 int index;
 (void)fprintf(stderr,"WINDS: VarDB file found and in use\n");
 for (num_attr=0; num_attr<SizeOfAttrDatabase; num_attr++) {
  index=num_attr*VALUELEN;
  varname=(char *)GetVarListName(num_attr);
/*
maintain attrname for now while supporting plot_attrs and bulletin.9
*/
  (void)sprintf(&attrname[num_attr*VARDB_NL],"%s",varname);
  (void)sprintf(VarAttrs[num_attr].VarName,"%s",varname);
/*
it is noted for now that UNITSLEN is 13 while VARDB_UL is 16. This could 
cause an overwrite situation in following call, but I'm deferring on this
until removing plot_attrs entirely, soon.
*/
  (void)sprintf(&units[num_attr*UNITSLEN],"%s",(char *)VarDB_GetUnits(varname));
  (void)sprintf(VarAttrs[num_attr].Units,"%s",(char *)VarDB_GetUnits(varname));
  (void)sprintf(VarAttrs[num_attr].Title,"%s",(char *)VarDB_GetTitle(varname));
  if (VarDB_isRangeFloating(varname)) {
   VarAttrs[num_attr].RangeIsFloat=TRUE;
   VarAttrs[num_attr].RangeIsFixed=FALSE;
   VarAttrs[num_attr].RangeMinimum=0.00;
   VarAttrs[num_attr].RangeMaximum=0.00;
   VarAttrs[num_attr].TotalRange=(double)ABS(VarDB_GetFloatRange(varname));
   VarAttrs[num_attr].PlusOrMinus=VarAttrs[num_attr].TotalRange/2.0;
   (void)sprintf(&default_range[index],"%lf", -1*VarAttrs[num_attr].TotalRange);
   (void)sprintf(&default_mins[index],"0.00");
  } else {
   VarAttrs[num_attr].RangeIsFloat=FALSE;
   VarAttrs[num_attr].PlusOrMinus=0.00;
   VarAttrs[num_attr].RangeIsFixed=TRUE;
   VarAttrs[num_attr].RangeMinimum=(double)VarDB_GetFixedRangeLower(varname);
   VarAttrs[num_attr].RangeMaximum=(double)VarDB_GetFixedRangeUpper(varname);
   VarAttrs[num_attr].TotalRange=ABS(
    VarAttrs[num_attr].RangeMaximum-VarAttrs[num_attr].RangeMinimum);
   (void)sprintf(&default_range[index],"%lf",VarAttrs[num_attr].TotalRange);
   (void)sprintf(&default_mins[index],"%lf",VarAttrs[num_attr].RangeMinimum);
  }
 }
} 
 return(num_attr);
}

/***********************  GetSizeOfAttrDatabase()  **************************/

GetSizeOfAttrDatabase()
{ 
 return SizeOfAttrDatabase;
}

/***********************   GetVarListIndex()  *******************************/

GetVarListIndex(varname,OrWithMode)
char *varname;
int OrWithMode;
/* 
search menu list character array for a match with varname. If found,
return its position in appropriate array (OR'd with a value indicating RAW or
DERIVED if OrWithMode is TRUE); else put out a message and return ERROR
*/
{
 int indx;            /* utility indexing   */

 indx = 0;
 while (indx < GetNumMenuEntries()) {
  if ( (strcasecmp((char *)GetMenuListName(indx),varname) )==0)   {
   if (OrWithMode)
    return(GetMenuListMode(indx)|GetMenuListIndex(indx));
   return(GetMenuListIndex(indx));
  }
  indx++;
 }
 return(ERROR);
}

/*********************  GetPlotAttrIndex()  ******************************/
 
GetPlotAttrIndex(varname,checkfordefault)
char *varname;   /* variable name requested */
int checkfordefault;	/* check for "DEFAULT" entry if varname not found */
/*  
given variable name varname, return its ordinal position within the
PROJ_ATTR file . If varname is not in that file AND checkfordefault is TRUE, 
search for "DEFAULT" entry; if still no luck, return ERROR.

Array attrname has the names in the plot_attr file on entry.
*/
{
int i;   /* indexing througn attrname array  */
 
 for (i=0; i<num_attr; i++) {
  if  ((strcasecmp(&attrname[i*VARDB_NL],varname)) == 0) {
   return i;
  }
 } 
 if (!checkfordefault)
  return ERROR;
/*
check for entry named "DEFAULT"
*/
 for (i=0; i<num_attr; i++) {
  if  ((strcasecmp(&attrname[i*VARDB_NL],"DEFAULT")) == 0) {
   time_to_log();
   (void)fprintf(stderr,"using DEFAULT attributes for %s\n",varname);
   return(i);
  }
 } 
/*
whoops
*/
 (void)fprintf(stderr,"WHOOPS: no plot specifications found for DEFAULT\n");
 return ERROR;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

static struct cat *cat_list, *category;
static int num_of_cats;
static struct parameter *bulletin_9;

/**********************  GET_NUM_OF_CATEGORIES()  ****************************/

get_num_of_categories()
{
 return num_of_cats;
}

/**********************  FIRST_CATEGORY()  *********************************/

struct cat *
first_category()
{
 return cat_list;
}

/***************************************************************************/

char *
GetNextCategory()
{
 category=category->next;
 if (category)
  return category->name;
 else
  return NULL;
}

/***************************************************************************/

char *
GetFirstCategory()
{
 category=cat_list;
 return category->name;
}

/**********************  GET_BULLETIN_9()  *********************************/

get_bulletin_9() 
/* read in the bulletin 9 database file
*/
{
 static struct parameter *variable;     /* variables listed in bulletin.9  */
 FILE *fp;                  /* read bulletin.9 file  */
 char record[CATEGORY_LENGTH];       /* record read from bulletin.9  */
 char filename[MAX_FLNM_LNGTH]; 
 char title[MAX_TITLE_LENGTH]; 
 char Units[UNITSLEN]; 
 char name[NAMLEN]; 
 float range,range_min;


 (void)sprintf(&filename[0],PROJ_BULLETIN9,ProjectDirectory,proj_number);
 if ( (fp = WINDSfopen(filename,"r")) == NULL) {
  (void)WINDSfclose(fp);
  (void)fprintf(stderr,
   "Warning: no project-specific bulletin.9 -- use system file instead\n"); 
  (void)sprintf(&filename[0],BULLETIN9,ProjectDirectory);
  if ( (fp = WINDSfopen(filename,"r")) == NULL) {
   (void)WINDSfclose(fp);
   perror("FATAL: WINDSfopen of system file bulletin.9"); 
   RequestAlertNow("FATAL: can't open bulletin 9...contact WINDS expert with this information!");
   winds_cleanup(ERROR);
  }
 }

 variable=NULL;
 category=NULL;
 num_of_cats=0;

 while ( fgets(record,CATEGORY_LENGTH,fp) != NULL) {
  if (category_type(record)) {  /* this is a category  */
   num_of_cats++;
   if (category) {              /* add new category to list */
    if (!category->next)
     category->next=(struct cat *)(malloc((unsigned)sizeof(struct cat)));
    category=category->next;
   } else {             /* start list of categories  */          
    category=(struct cat *)(malloc(sizeof(struct cat)));
    cat_list=category;
   }

/* initialize this category structure  */

   (void)strcpy(category->name , record);
   category->number_of_vars=0;
   category->next=NULL;
  } else {            
/* 
variable name, units, title 
*/
   char format[32];
   (void)sprintf(format,"%%%dc%%s%%s%%f%%f",MAX_TITLE_LENGTH);
   if ( (sscanf(record,format,
    title,Units,name,&range,&range_min)) == NUMBULLETIN9FIELDS) {
/*
install title
*/
    if (!GetVarDataBase()) {
     int index;
     title[MAX_TITLE_LENGTH]='\0';
/*
it might be in bulletin.9 but not in raw/derived list
*/
     if ((index=GetVarListIndex(name,FALSE))!=ERROR)
      (void)sprintf(VarAttrs[
       !GetVarDataBase()?GetPlotAttrIndex(name,FALSE):index].Title,
       "%s",title);
    }
/*
valid variable entry -- else, maybe just a line feed trying to get in on the act
*/
    if (variable) {    /* add new variable to list */
     if (!variable->next)
      variable->next=(struct parameter *)(malloc(sizeof(struct parameter)));
     variable=variable->next;
    } else {             
     variable=(struct parameter *)(malloc(sizeof(struct parameter)));
     bulletin_9=variable;
    }
    (void)sscanf(name,"%s",variable->name);
    variable->category_number=num_of_cats-1;
    variable->next=NULL;
    SetMenuListCategory(name,category->name);

/* this category has one more variable in it */

    category->number_of_vars++;
   }
  }
 }
 (void)WINDSfclose(fp);
}

/************************  CATEGORY_TYPE()  *********************************/
 
int category_type(s)
char *s;
/* determine whether this character string meets criterion for being regarded
   as a category; note the assumption being made here: if the word 'VARIABLES'
   is in this string, it is a category.  This must be adhered to in the file
   /home/winds/sys/bulletin.9
*/
{
int start_pos;
char testword[80];
 
   for (start_pos=0; sscanf(&s[start_pos],"%s",testword) != EOF;
        start_pos += strlen(testword))  {
/************************  ASSUMPTION  ***************************************/
/* see above intro to this function for explanation  */
    if ( (strcmp(testword,"VARIABLES")) == 0){
/************************  END ASSUMPTION  ***********************************/
     s[start_pos+1]='\0';
     return(TRUE);
    }
   }
   return(FALSE);
} 

/**************************   TOP_OF_BULLETIN9()  ****************************/

struct parameter *
top_of_bulletin9()
{
 return bulletin_9;
}

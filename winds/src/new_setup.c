/* new_setup.c  -- create and manage the new setup panel used during a session
   to create a new plot setup.
*/

#include <stdio.h>
#include <xview/xview.h>
#include <xview/panel.h>

/**************************  WINDS INCLUDES  *********************************/

#include <display.h>
#include <lengths.h>
#include <help.h>
#include <plotspex.h>
#include <constants.h>
#include <xy_plt.h>
#include <new_setup.h>
#include <op_panels.h>
#include <panel_menus.h>
#include <proto.h>

/************************  GLOBAL WITHIN MODULE  *****************************/

static char DepVarSeln[MAX_DISPLAY_VARS][NAMLEN];
static char IndepVarSeln[NAMLEN];
static Need_new_setup[NUM_DISPLAY_QUADRANTS]={FALSE,FALSE,FALSE,FALSE};
static int PendingPlotType,
           VariableType,    
           NewSetupInProgress=FALSE,
           NumNewSetupSelns[SETUP_MAX_VARTYPES];
static char VarTypeChoiceString[SETUP_MAX_VARTYPES][64]={"",""};
static char PlotTypeChoiceString[TOTAL_SETUP_TYPES][64]={
"Time","Track","Skew-t","Sound'g",
"XY","List","Fixed","Pms1D","2D-std",
"2D-Grey","2D-HVPS"};
static int num_vartypes[NUM_SETUP_TYPES]=
{TIME_SERIES_VARTYPES,TRKPLT_VARTYPES,SKEWT_VARTYPES,SOUNDING_VARTYPES,
XY_VARTYPES,LIST_VARTYPES,FIXED_VARTYPES};
static int MaxNewSetupSelns[NUM_SETUP_TYPES][SETUP_MAX_VARTYPES]=
{MAX_TRACES,0,				/* TIME_SERIES */
 MAX_TRACES,MAX_INDEPVARS,		/* TRACK_PLOT */
 MAX_SKEWT_TRACES,MAX_INDEPVARS,	/* SKEW_T */
 MAX_TRACES,MAX_INDEPVARS,		/* SOUNDING */
 MAX_TRACES,MAX_INDEPVARS,		/* XY */
 0,0,					/* LISTS */
 0,0};					/* FIXED_FORMAT */

/*****************************************************************************/

SetNewSetupNeeded(quadrant,value)
int quadrant,value;
{
 Need_new_setup[quadrant]=value;
}

/***************************************************************************/

GetNewSetupNeeded(quadrant)
int quadrant;
{
 return Need_new_setup[quadrant];
}

/*************************************************************************/

ConfigPlotType(plottype,quadrant)
int plottype,quadrant;
{
 if (plottype==SETUP_1D) {
  DismissScrollPanel();
  config_display(quadrant,DISPTYPE_PMS1D,NEW_SETUP);
  SetNewSetupNeeded(quadrant,FALSE);
 } else if (plottype==SETUP_2D_STD) {
  DismissScrollPanel();
  config_display(quadrant,DISPTYPE_PMS2D,NEW_SETUP|PMS2D_STANDARD);
  SetNewSetupNeeded(quadrant,FALSE);
 } else if (plottype==SETUP_2D_HVPS) {
  DismissScrollPanel();
  config_display(quadrant,DISPTYPE_PMS2D,NEW_SETUP|PMS2D_HVPS);
  SetNewSetupNeeded(quadrant,FALSE);
 } else if (plottype==SETUP_2D_GREY) {
  DismissScrollPanel();
  config_display(quadrant,DISPTYPE_PMS2D,NEW_SETUP|PMS2D_GREYSCALE);
  SetNewSetupNeeded(quadrant,FALSE);
 } else {
  PendingPlotType=plottype;
  AdjustVarTypes();
  InitNewSetupVars(TRUE);
  AdjustForPlotType();
/*
make sure we return here to avoid setting a default description for all other
plottype's below
*/
  return;
 }
/*
1d and 2d setup types are ready to go with no further adieu; set and 
display descriptions (not done in config_display because NewSetup(entry)
is true there, and want to avoid doing it then because user may cancel
a pending new setup for other setup types, e.g. time series, and in that
case we want to see the title for the retained setup.)
*/
 set_description(quadrant,"(new setup - untitled)");
 show_op_msg(quadrant,(char *)get_description(quadrant));
}

/************************************************************************/

SetVarType(type)
int type;
{
 VariableType=type;
}

/************************  InstallNewSetup()  ****************************/

void
InstallNewSetup()
{
int quadrant,seln;

 quadrant=GetCurrentSetupQuadrant();
 (void)sprintf(indepvar_name[quadrant],"%s",IndepVarSeln);
 for (seln=0; seln<GetTotalDepvars(); seln++ )
  (void)sprintf(usrvar[quadrant][seln],"%s",DepVarSeln[seln]);  
 if (got_minimal_setup()) {
  set_defaults(quadrant);
  plot_type[quadrant]=PendingPlotType;
  init_state_parms(quadrant,DISPTYPE_XYPLOT);
  init_display_parms(quadrant);
  SetInstallNewSetupVars(quadrant);
  fini_new_setup(quadrant);
  set_genop_buttons(quadrant);
  clear_genop_status(quadrant);
/*
set and display a default description
*/
  set_description(quadrant,"(new setup - untitled)");
  show_op_msg(quadrant,(char *)get_description(quadrant));
  SetNewSetupNeeded(quadrant,FALSE);
 } 
}

/***************************  FINI_NEW_SETUP()  *******************************/

fini_new_setup(quadrant)
int quadrant;
{
  need_rehilite[quadrant]=TRUE;
  NewSetupInProgress=FALSE;
  set_next_tb_image(window_type[quadrant]);
  set_current_panel(NONE);

/* since a LOC_WINEXIT is not generated for the new-setup panel when the user 
   clicks Apply or Cancel  -- it is simply hidden -- explicitly reset the
   boolean that tracks cursor relation to the new setup panel (this is normally
   done by the LOC_WINEXIT test in event_proc())
*/
/***
  cursor_in_setup_panel=FALSE;
***/
}


/*****************************************************************************/

SetInstallNewSetupVars(quadrant)
int quadrant;
{
 switch (plot_type[quadrant]) {
  case TIME_SERIES:
   num_of_traces[quadrant]=GetTotalDepvars();
   break;
  case TRACK_PLOT:
   num_of_traces[quadrant]=GetTotalDepvars();
   break;
  case SKEW_T:
   num_of_traces[quadrant]=GetTotalDepvars();
   autoscale[quadrant]=FALSE;
   set_indepvar_yaxis(quadrant,TRUE);
   break;
  case SOUNDING:
   num_of_traces[quadrant]=GetTotalDepvars();
   set_indepvar_yaxis(quadrant,TRUE);
   set_isolate_traces(quadrant,TRUE);
   break;
  case XY:
   num_of_traces[quadrant]=GetTotalDepvars();
   break;
  case LISTS:
   num_of_listees[quadrant]=GetTotalDepvars();
   break;
  case FIXED_FORMAT:
   num_of_fixees[quadrant]=GetTotalDepvars();
   break;
 }
}

/**************************************************************************/

GetNumNewSetupSelns()
{
 return NumNewSetupSelns[VariableType];
}

/**************************************************************************/

GetMaxNewSetupSelns()
{
 return MaxNewSetupSelns[PendingPlotType][VariableType];
}

/**************************************************************************/

TransferQuadSpecificVars(old,new)
int old,new;
{
}

/**************************************************************************/

UpdateNewSetupSelns(varname,selected)
char *varname;
int selected;
{
int seln,selns;

 selns=NumNewSetupSelns[VariableType];
 if (selected) {
/* 
add this one to selection list...
*/
  if (GetVartypeIsIndep()) {
   if (strcmp(IndepVarSeln,varname)) {
/*
...if it's not already there...
*/
    (void)sprintf(IndepVarSeln,"%s",varname);
    ++NumNewSetupSelns[SETUP_INDEPVAR];
   }
  } else {
   if (strcmp(DepVarSeln[selns],varname)) {
    (void)sprintf(DepVarSeln[selns],"%s",varname);
    ++NumNewSetupSelns[SETUP_DEPVAR];
   }
  }
 } else {
/* 
this item is de-selected
*/
  if (selns)
   --NumNewSetupSelns[VariableType];
  if (GetVartypeIsIndep()) {
   (void)sprintf(IndepVarSeln,"\0");
  } else {
   for (seln=0; seln<selns; seln++) {
    if (!strcmp(DepVarSeln[seln],varname)) {
     if (selns>1)
      (void)sprintf(DepVarSeln[seln],"%s",DepVarSeln[selns-1]);
     (void)sprintf(DepVarSeln[selns-1],"\0");
     break;
    }
   }
  }
 }
/* 
update message about how many selections are made in this variable type, and
activate/inactivate apply button accordingly
*/
 UpdateSelnsMade();
}

/*****************************************************************************/

UpdateSelnsMade()
/*
update message about how many selections are made in this variable type
*/
{
 UpdateVarTypeChoices();
 ActivateApplyButton(got_minimal_setup());
 SetNewSetupChoicesExclusive();
}

/*****************************************************************************/

got_minimal_setup()
{
int var_type;

 for (var_type=0; var_type<num_vartypes[PendingPlotType]; var_type++) {
  if (!(NumNewSetupSelns[var_type]))
   return FALSE;
 }
 return TRUE;
}

/*****************************************************************************/

GetTotalDepvars()
{
 return NumNewSetupSelns[SETUP_DEPVAR];
}

/****************************************************************************/

char *
GetNewSetupSeln(which)
int which;
{
 if (GetVartypeIsIndep()) {
  return IndepVarSeln;
 } else {
  return DepVarSeln[which];
 }
}

/************************* **************************************************/

GetVartypeIsIndep()
/* 
return TRUE if the setup type's variable type is an independent variable, 
else FALSE
*/
{
 return(VariableType==SETUP_INDEPVAR);
}

/*****************************************************************************/

AdjustForPlotType()
{
static int MaxVarsInit=FALSE;

 if (!MaxVarsInit) {
/*
determine various elements of MaxNewSetupSelns at run-time
*/
  MaxNewSetupSelns[LISTS][0]=GetMaxListees();
  MaxNewSetupSelns[FIXED_FORMAT][0]=GetMaxFixees();
  MaxVarsInit=TRUE;
 }
 UpdateSelnsMade();
 SetPlotTypeButtonValue(PendingPlotType);
}

/*****************************************************************************/

InitNewSetupVars(initselns)
int initselns;
{
 int var_type,seln;

 NewSetupInProgress=TRUE;
 if (initselns) {
  for (var_type=0; var_type<num_vartypes[PendingPlotType]; var_type++) {
   if (var_type==SETUP_DEPVAR) {
    for (seln=0; seln<NumNewSetupSelns[var_type]; seln++) {
     (void)sprintf(DepVarSeln[seln],"\0");
    }
   } else {
    (void)sprintf(IndepVarSeln,"\0");
   }
   NumNewSetupSelns[var_type]=0;
  }
 }
}

/*****************************************************************************/

initiate_new_setup(quadrant,plottype)
int quadrant,plottype;
{
 SetVarType(SETUP_DEPVAR);
 UpdateSelnsMade();
 ConfigPlotType(plottype,quadrant);
 InitNewSetupVars(TRUE);
 ShowNewSetupMenu(quadrant);
/*
must do this one again after showing menu, since menu was re-created to 
reconcile fonts with item widths...
*/
 AdjustVarTypes();
 AdjustForPlotType();
}

/*****************************************************************************/

GetPendingPlotType()
{
 return (PendingPlotType);
}

/*************************************************************************/
 
char * 
GetVarTypeChoiceString(numchoice)
int numchoice;
{
 return VarTypeChoiceString[numchoice];
}
 
/*************************************************************************/

UpdateVarTypeChoices()
{
 UpdateVarTypeChoice(SETUP_DEPVAR);
 UpdateVarTypeChoice(SETUP_INDEPVAR);
}

/*************************************************************************/

UpdateVarTypeChoice(which)
int which;
{
 UpdateVarTypeChoiceString(which);
 UpdateVarTypeButton(which,GetVarTypeChoiceString(which));
}

/*************************************************************************/

SetVarTypeChoice(numchoice,choice)
int numchoice;
char *choice;
{
 (void)sprintf(VarTypeChoiceString[numchoice],"%s",choice);
}

/*****************************************************************************/

UpdateVarTypeChoiceString(which)
int which;
{
 char savestring[64];

 (void)sscanf(VarTypeChoiceString[which],"%s",savestring);
 (void)sprintf(VarTypeChoiceString[which],"%s %d/%d",savestring,
  NumNewSetupSelns[which],MaxNewSetupSelns[PendingPlotType][which]);
}

/*****************************************************************************/

AdjustVarTypes()
{
 SetVarType(SETUP_DEPVAR);
 SetNewSetupChoicesExclusive();
 switch(PendingPlotType) {
  case LISTS:
  case FIXED_FORMAT:
  case TIME_SERIES:
   SetVarTypeChoice(0,"Dep't");
   SetVarTypeChoice(1,"");
   ActivateVarTypeButton(FALSE);
   break;
  case TRACK_PLOT:
   SetVarTypeChoice(0,"Lat.");
   SetVarTypeChoice(1,"Long.");
   ActivateVarTypeButton(TRUE);
   break;
  case SKEW_T:
   SetVarTypeChoice(0,"Dep't");
   SetVarTypeChoice(1,"Indep't");
   ActivateVarTypeButton(TRUE);
   break;
  case SOUNDING:
  case XY:
   SetVarTypeChoice(0,"Dep't");
   SetVarTypeChoice(1,"Indep't");
   ActivateVarTypeButton(TRUE);
   break;
 }
 ClearVarTypeChoices();
 UpdateSelnsMade();
 SetVarTypeButtonValue(SETUP_DEPVAR);
}

/*****************************************************************************/

GetNumPlotTypeChoices()
{
 return TOTAL_SETUP_TYPES;
}

/*****************************************************************************/

char *
GetPlotTypeChoiceString(which)
{
 return PlotTypeChoiceString[which];
}
 
/*****************************************************************************/

GetVarTypeIsExclusive()
{
 return (GetVartypeIsIndep());
}

/*****************************************************************************/

GetNewSetupInProgress()
{
 return NewSetupInProgress;
}


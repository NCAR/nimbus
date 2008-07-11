/* timer.c -- provide for one-second time-outs and subsequent display
   updating, etc.
*/

#include <stdio.h>
#include <time.h>
#include <xview/xview.h>
#include <xview/canvas.h>

/****************************  WINDS INCLUDES  ******************************/

#include <shmem.h>
#include <colors.h>
#include <display.h>
#include <timer.h>
#include <lengths.h>
#include <constants.h>
#include <help.h>
#include <xy_plt.h>
#include <errors.h>
#include <ops.h>
#include <socket.h>
#include <printers.h>
#include <proto.h>
#include <pms1d.h>
#include <network.h>
#include <plotspex.h>
#include <raf.h>
#include <files.h>


/****************************  EXPORTABLE  ********************************/

int plot_setup[NUM_DISPLAY_QUADRANTS]=
{FALSE,FALSE,FALSE,FALSE};	/* whether or not setup done for given xy-type
				   plot window */
int ElapsedTime,		/* save value of shared mem pointer, which
				   may change in the current time-out period
				   of processing */
    current_hr,
    current_minute,
    current_sec; 		/* current hour, minute, second */
int local_hr[NUM_QUADRANTS],
    local_minute[NUM_QUADRANTS],
    local_sec[NUM_QUADRANTS]; /* local hour, minute, second -- may vary from
                		 current time due to frozen quadrant */
int start_hour,
    start_minute,
    start_second;		/* start hour, minute and second */
int end_hour,
    end_minute,
    end_second;			/* end hour, minute and second */
int need_calib_curve=FALSE,
    need_redo_last=FALSE,
    take_snapshot[NUM_QUADRANTS],
    take_color_snapshot[NUM_QUADRANTS],
    time_gone_by[NUM_QUADRANTS]; /* save value of *elapsed_time since last update */ 
char current_time[TIME_CHARS];  /* string with current time written in it */
int update_current_time=FALSE;  /* set TRUE to force update of current time */

/***************************  GLOBAL WITHIN FILE  ****************************/

static Notify_value catchalrm(Notify_client client, int which);


static int start_time_init;
static int WINDSisStarted=FALSE;
static int Recycle=FALSE;
static char fltno_string[2*NAMLEN]="0";
static char *FlightNumber=NULL;
static int last_hr, last_min, last_sec;
static int MaxLenTimeWindowString=0;

/******************************  ResetCurrentTime()  ************************/

ResetCurrentTime()
{
 start_time_init=FALSE;
 if (GetInputMode()==DISK_DATA 
  || GetInputMode()==RAWFILE_NETCDF 
  || post_processing_mode()
  ) {
  *elapsed_time=0;
 }
}

/******************************  GetWINDSisStarted()  ************************/

GetWINDSisStarted()
{
 return WINDSisStarted;
}

/******************************  SetWINDSisStarted()  ************************/

SetWINDSisStarted(value)
int value;
{
 WINDSisStarted=value;
}

/******************************  DISABLE_TIMER()  ***************************/

disable_timer(_frame)
Frame _frame;
{
 (void) notify_set_itimer_func(_frame,catchalrm,
        ITIMER_REAL,ITIMER_NULL,ITIMER_NULL);
 return;
}

/*******************************  ENABLE_TIMER()  ***************************/

enable_timer(_frame)
Frame _frame;
{
static struct itimerval go_timer= {  /* internal display update timer  */
 UPDATE_SEX, UPDATE_USEX, UPDATE_SEX, UPDATE_USEX };

/************** Utilize notifier hack to avoid stopped timer bug *************/
 struct  timeval c_tv;
        c_tv = go_timer.it_value;
        c_tv.tv_sec *= 2;
        notify_set_signal_check(c_tv);

 (void) notify_set_itimer_func(_frame,catchalrm,
        ITIMER_REAL,&go_timer,ITIMER_NULL);

}

/************************  CHECK_FOR_REQUESTS()  ******************************/

/************************  CATCHALRM()  **********************************/

/*ARGSUSED*/
static Notify_value
catchalrm(client,which)
Notify_client client;
int which;
{
 static int need_hide_copyrite;
 static int menus_made;
 static int monitor_checked=FALSE;
 int curr_sec;

 if (Recycle) {

/* implement this setting with a ctl-panel button; must first re-do searches
   of netconfig file to look in a memory struct instead of opening/closing the
   file, so that changes via panel-menu don't need to modify the file itself
   to facilitate a project/fltno restart.  Provide a "Save" button on this
   menu to allow file updates, and user start of next session as per current 
   change.  NOTE that this also demands rethinking the netconfig file's
   role in this -- concurrent sessions can modify it w/different values!  File
   lock control is NOT the answer here, since you don't want the file locked
   for an entire session nor does it provide any protection to lock it only
   during write operations.  This ties in with user-specific plotspex files 
   -- at startup, user provides login or some session id, which is used to
   retrieve id-specific plotspex AND netconfig files.  Using login "user" is
   no good here either, since many people will be using this one.  Other
   project files needing this flexibility include: config, depend.tbl?, 
   landmarks, plot_attr, user.names?....This obviously calls for a file-mgmt
   module that reads files into given structs, updates them on save commands,
   names them according to user-session-ids, etc.
*/
  recycle();
  Recycle=FALSE;
 }

 if (!GetWINDSisStarted()) {
  winds_startup();
  DrawControlPanelPnum();
  DrawControlPanelFltno();
  SetWINDSisStarted(TRUE);
  need_hide_copyrite=TRUE;
  last_sec=ERROR;
  menus_made=FALSE;
  SetNeedStatsInit(TRUE);
  start_time_init=FALSE;
 }
/*
check for user-induced window size changes. Want to do this before updating
displays so new geometry is in place for redraws.
*/
 ResizeWindows();
 if (elapsed_time && *elapsed_time) {  /* protect against SEGV  */
  ElapsedTime=curr_sec=*elapsed_time;
  current_hr=hhmmss[((curr_sec-1)%get_datablock_size())*3];
  current_minute=hhmmss[1 + ((curr_sec-1)%get_datablock_size())*3];
  current_sec=hhmmss[2 + ((curr_sec-1)%get_datablock_size())*3];

/* wait, on startup, for some meaningful value for hh:mm:ss before proceeding
...this, unfortunately, means the first second of a flight that starts at
exactly midnight will be skipped.  So sue me.
*/
  if (!start_time_init && !current_hr && !current_minute && !current_sec)
   return NOTIFY_DONE;
  if (!TimeIsSane(current_hr,current_minute,current_sec))
/* 
funky times sometimes sent out from ADS -- ignore this second entirely
*/
   return NOTIFY_DONE;
  if (update_current_time || curr_sec != last_sec ) {
/* 
by now, we know that times are non-zero, that they are not equal to last one
seen, and that they are not 'funky' -- ergo, go ahead and do some work
*/
   update_current_time=FALSE;
   set_current_time_string(current_hr,current_minute,current_sec);
   last_sec=curr_sec;

/* update control panel time display
*/
   set_time(current_time);
  }
  if (!start_time_init)
   init_start_time();
  end_hour=current_hr;
  end_minute=current_minute;
  end_second=current_sec;
  UpdateSessionTimeFrame(start_hour,start_minute,start_second,
   end_hour,end_minute,end_second);

 } else {
/* 
no elapsed time yet; done for now, unless first time through analysis mode;
then, request input to fill in first frame with start of file
*/
/***
  fprintf(stderr,"DEBUG: no data yet\n");
***/
  if (post_processing_mode() && elapsed_time && !ElapsedTime) {
   if (last_sec==ERROR) {
    send_input_info(REQ_SET_START_TIME,0,"start time request",
                    TRUE);
   }
  }
  return NOTIFY_DONE;
 }
/*
check for notifications from other winds processes
*/
 CheckForNetworkMessages();
/*
check if time is up for display of feedback message in Control Panel
*/
 CheckForTimeToClearMessage();
/*
verify that monitor is running once every few minutes. Don't check unless
time is updating, i.e. curr_sec != last_sec, to preclude the possibility
(odds of 1 in 300) that updating has stopped at a point when ElapsedTime is 
some multiple of 300: this will cause VerifyMonitor to fire off about
10 times/second, bringing the CPU down to its knees. There is no downside
of NOT checking the monitor since the first time through, curr_sec will
indeed NOT equal last_sec and the verification will execute. The only
other (remote) possibility would be that the monitor process dies after
this verification, during the span in which time here is not updating and
ElapsedTime % 300 is true; then, no re-start. 
*/
 if (!post_processing_mode()  &&
     (!monitor_checked || !(ElapsedTime%300))) {
  VerifyMonitor(FALSE);
  monitor_checked=TRUE;
 }

 if (calib_running) {

  time_gone_by[PLOTS] = curr_sec;
  if (GetScrollPanelShowing(TRUE) || alert_up )
/* don't change displays below a menu, alert, or adscmd popup to preclude damage
   and need for subsequent refresh.  Don't allow update of displays during
   these times to prevent resetting of update_mode while popup is being
   used.
*/
   return(NOTIFY_DONE);

  if (need_calib_curve) {
   need_calib_curve-=calib_curve(set_point);

/* wait until calib_curve() is done
*/
   if (!need_calib_curve) {
    statout();
    need_rehilite[STATS]=TRUE;
    if (need_redo_last) {
     relist_points();
     need_redo_last=FALSE;
    }
   }
  }

  if (update_mode[PLOTS]==INITIAL) {
   update_mode[PLOTS]=NORMAL_UPDATE;
   init_stats();
   init_plots();
   if (menu_selected)
    draw_bkgnd(PLOTS);
  }
  if (take_color_snapshot[PLOTS]) {
   (void)color_snapshot(PLOTS);
   take_color_snapshot[PLOTS]=FALSE;
  }
  if (take_snapshot[PLOTS]) {
   (void)snapshot(PLOTS);
   take_snapshot[PLOTS]=FALSE;
  }
  if (need_rehilite[STATS])
   rehilite(STATS);

  update_voltage(PLOTS);
  return(NOTIFY_DONE);

 } else {

/* make xy menus first time through,  AFTER getting plotspex and fonts, and
   after input has init'd global name lists.  Making the xy-menus before
   this time results in brain-damaged fonts.
*/
  if (!menus_made) {
   MakeMenusAndPanels();
   menus_made=TRUE;
  }
/* 
check for notices needed
*/
  notify_user_error(get_notice_needed());

/* initialize statistics interval first time through -- IF there is a printer
   for lists and stats
*/
  if (GetNeedStatsInit()) {
   init_lstats(current_hr,current_minute,current_sec);
   SetNeedStatsInit(FALSE);
  }
/* 
print out next second of data if warranted, accumulate and possibly
print out statistics
*/
  lstat_update(curr_sec);
  
/* send out any special data values, if requested
*/
  if (flat_panel_data()) {
   send_flat_panel_data(curr_sec-1);
  }
  if (AsciiData()) {
   SendAsciiData(curr_sec-1,current_hr,current_minute,current_sec);
  }
  if (GetScrollPanelShowing(TRUE) || alert_up )
/* 
don't change displays below a menu, alert, or adscmd popup to preclude damage
and need for subsequent refresh.  Don't allow update of displays during
these times to prevent resetting of update_mode while popup is being
used.
*/
   return(NOTIFY_DONE);
/* 
stop clock and update displays
*/
  disable_timer(client);
  update_displays(curr_sec);
  enable_timer(client);
/* 
hide copyright if showing
*/
  if (need_hide_copyrite) {
   hide_copyright();
   need_hide_copyrite=FALSE;
  }
  return(NOTIFY_DONE);
 }
}

/*****************************************************************************/

ResizeMenusAndPanels()
{
 SetStandardFont();
 ResizeControlPanel();
 SetControlPanelDims();
 MakeMenusAndPanels();
 RefreshPanels();
}

/*****************************************************************************/

MakeMenusAndPanels()
{
 int quadrant;

 make_winds_menus();
/* 
for some reason, making the op-panels before this time results in
brain-damaged layout.  DON'T ask me why.  Must make menus before op-panels 
so Disp buttons have menu to attach. 
*/
 make_op_panels();
 for (quadrant=0; quadrant<get_num_displays(); quadrant++) 
  show_op_msg(quadrant,(char *)get_description(quadrant));
 make_panel_menus(GetBaseFrame(),control_width,display_height);
}

/************************  UPDATE_DISPLAYS()  ********************************/

update_displays(time_elapsed)
int time_elapsed;
{
int quadrant;
static int disp_feedback[NUM_DISPLAY_QUADRANTS]=
{0,0,0,0}; 

 for (quadrant=0; quadrant<get_num_displays(); quadrant++) {
 
/* 
don't update times if frozen
*/
  if (!frozen[quadrant]) {
   time_gone_by[quadrant] = time_elapsed-start_point[quadrant];
   local_sec[quadrant]=current_sec;
   local_minute[quadrant]=current_minute;
   local_hr[quadrant]=current_hr;
  } 
/*
check for need to save any setups
*/
  if (GetNeedSaveSetup(quadrant)) {
   SetNeedSaveSetup(quadrant,FALSE);
   save_setup(quadrant);
/*
display "Saved" feedback for 3 seconds
*/
   disp_feedback[quadrant]=3;
  } else if (disp_feedback[quadrant]) {
   disp_feedback[quadrant]--;
   if (!disp_feedback[quadrant]) {
    display_saved_feedback(quadrant,FALSE);
   }
  }
/* don't update display if a multiple click is in progress
*/
  if (num_clix[quadrant]) {
   return;
  }
  CheckForMultiPrints(quadrant);
  if (take_snapshot[quadrant]) {
    take_snapshot[quadrant]=FALSE;
    (void)snapshot(quadrant);
  }
  if (take_color_snapshot[quadrant]) {
   if (window_type[quadrant]==DISPTYPE_FIXEDFORMAT)
 
/* no need for wasting time on color for fixed format window: simply print
   it in black and white
*/
    (void)snapshot(quadrant);
   else
    (void)color_snapshot(quadrant);
   take_color_snapshot[quadrant]=FALSE;
  }
  if (GetNewSetupNeeded(quadrant)) {
   ShowNewSetupHintsToUser(quadrant);
  }
  UpdateAndShowDisplay(quadrant);
 }
 AdjustForNextMultiPrint();
}

/*************************************************************************/

UpdateAndShowDisplay(quadrant)
int quadrant;
{
 UpdateDisplay(quadrant);
   
/* 
now that displays are updated, it's safe to show any windows that were
previously hidden to prevent confusion, particularly when geometries are
changed.
*/
 if (hidden_window(quadrant)) {
  unhide_window(quadrant);
/*
if a PanelMenu was showing, it may have just gotten hidden; undo this
*/
  ShowCurrentPanelMenu();
 }
}

/*************************************************************************/

ShowNewSetupHintsToUser(quadrant)
int quadrant;
{
 static char hint[]="Setup needed: select a flashing button...";

 ShowDispSubMenus(quadrant);
 (*SetContext[GetDrawType()])(pw_xid[quadrant],GXcopy,BKGND,0,0,
  GetFontID(GetControlPanelFont()));
 (void)(*DrawTtext[GetDrawType()])(
  GetWINDSWindowWidth(quadrant)/2-
  (strlen(hint)*GetFontWidth(GetControlPanelFont()))/2,
  GetWINDSWindowHeight(quadrant)/2,hint);
}

/***************************************************************************/

FillQuadrant(quadrant,color)
{ 
 (*FillArea[GetDrawType()])(pw_xid[quadrant],0,0,
    GetWINDSWindowWidth(quadrant),GetWINDSWindowHeight(quadrant),color);
} 
 
/*************************************************************************/

GetAreaTooSmall(quadrant) 
int quadrant;
{
 switch (window_type[quadrant]) {
  case DISPTYPE_LISTS:
   return GetListAreaTooSmall(quadrant);
  case DISPTYPE_FIXEDFORMAT:
   return GetFixedAreaTooSmall(quadrant);
  case DISPTYPE_PMS1D:
   return Get1dAreaTooSmall(quadrant);
  case DISPTYPE_PMS2D:
   return Get2dAreaTooSmall(quadrant);
  case DISPTYPE_XYPLOT:
   return GetXYAreaTooSmall(quadrant);
/***
   switch (plot_type[quadrant]) {
    case TIME_SERIES:
     return FALSE;
    case TRACK_PLOT:
     return FALSE;
    case SOUNDING:
     return FALSE;
    case XY:
     return FALSE;
   }
***/
  case WTYPE_SKEWT:
   return FALSE;
 }
}

/**********************  UpdateDisplay()  ********************************/

UpdateDisplay(quadrant)
int quadrant;
{
  if (GetAreaTooSmall(quadrant)) 
/*
if a user resize has produced a window that will cause a crash, freeze, 
endless loop, or some other problem, don't go out of your way to allow
that to happen...
*/
   return;
  if (GetNewSetupNeeded(quadrant)) 
   return;
  switch (window_type[quadrant]) {
   case DISPTYPE_PMS1D:
    update_pms1d_display(quadrant,ElapsedTime);
    break;
   case DISPTYPE_PMS2D:
    update_pms2d_display(quadrant);
    break;
   case DISPTYPE_XYPLOT:
   case WTYPE_SKEWT:
   case DISPTYPE_LISTS:
   case DISPTYPE_FIXEDFORMAT:
    update_xy_display(quadrant);
    break;
  }
}

/**********************  UPDATE_XY_DISPLAYS()  ********************************/

update_xy_display(quadrant)
int quadrant;
{

int var;
int last_var;

static int last_time_chkd[NUM_DISPLAY_QUADRANTS]=
{0,0,0,0};
static int last_hex_status=TRUE; /* assuming that hex also starts out TRUE */
static int redisplay_needed[NUM_DISPLAY_QUADRANTS]=
{FALSE,FALSE,FALSE,FALSE},
           autoscale_done[NUM_DISPLAY_QUADRANTS]=
{FALSE,FALSE,FALSE,FALSE};

 if (!plot_setup[quadrant]) {
  setup_xyplot(quadrant);
  update_mode[quadrant]=RECONFIG;
  plot_setup[quadrant]=TRUE;
 }

 if (GetNewSetupInProgress()) {

/* user has initiated a new setup 
*/
   return;
 }

 switch (plot_type[quadrant]) {
  case TIME_SERIES:
  case TRACK_PLOT:
  case SKEW_T:
  case SOUNDING:
  case XY:
   if (!num_of_traces[quadrant])
    return;
   break;
  case LISTS:
   if (!num_of_listees[quadrant])
    return;
   break;
  case FIXED_FORMAT:
   if (!num_of_fixees[quadrant])
    return;
   break;
 }

 autoscale_done[quadrant]=FALSE;
 redisplay_needed[quadrant]=FALSE;
 
/* check for need for autoscale */
 
 if (plot_type[quadrant]!= LISTS && plot_type[quadrant]!=FIXED_FORMAT &&
     (force_autoscale(quadrant) || 
     (autoscale[quadrant] && !frozen[quadrant] &&
     (start_point[quadrant]+time_gone_by[quadrant]) > 
      (last_time_chkd[quadrant] + clip_chk_interval[quadrant])))) {
  last_time_chkd[quadrant] = start_point[quadrant]+time_gone_by[quadrant];
  autoscale_done[quadrant]=auto_scale_if_needed(quadrant);
  set_force_autoscale(quadrant,FALSE);
 }
 
/* if someone else has changed the global setting for octal vs. hex printing
   AND autoscaling has not occurred, redraw the range labels (autoscale will
   have already done this)
*/

 if (plot_type[quadrant] != FIXED_FORMAT && last_hex_status != hex) {
  last_hex_status=hex;

/* only redraw if one of the variables is in BITS mode */

  last_var=(plot_type[quadrant]==LISTS)?num_of_listees[quadrant]:num_of_traces[quadrant];
  for (var=0; var<last_var;var++) {
   if (dep_mode[quadrant][var]==BITS) {
    redisplay_needed[quadrant]=TRUE;
    break;
   }
  } 
  if (redisplay_needed[quadrant]) {
   if (plot_type[quadrant] != LISTS) {
    if (!autoscale_done[quadrant]) {
     draw_bkgnd(quadrant);

/* since range labels may be wider or narrower, this effectively changes the 
x scale */

     update_mode[quadrant]=CHG_X_SCALE;  
     need_rehilite[quadrant]=TRUE;
    }
   } else { /* signal printer to change base */
    if (list_printing[quadrant])
     print_header(quadrant);
   }
  } 
 } 

/* do the display update -- check for repaint procedure called first.  If
   window is frozen, check whether user has made a display change that will
   call for updating of display data 
*/

 if ((frozen[quadrant]  && update_mode[quadrant]!=NORMAL_UPDATE) 
  || (!frozen[quadrant] && repaint_set[quadrant]))
  (*display_func[plot_type[quadrant]])(quadrant);

 return;
}

/***********************  UPDATE_PMS1D_DISPLAY()  ****************************/
update_pms1d_display(quadrant, time_elapsed)
int	quadrant;
int	time_elapsed;
{
	static int	last_time_chkd[NUM_DISPLAY_QUADRANTS] = {0, 0, 0, 0};
 
	if (update_mode[quadrant] == INITIAL ||
	    update_mode[quadrant] == RECONFIG)
		{
		(*FillArea[GetDrawType()])(pw_xid[quadrant],
				0,
				0,
				window_width[quadrant],
				window_height[quadrant],
				0);

		PlotPMS1Dbackground(quadrant);
		adjust_1d_times(quadrant,time_elapsed);
		}
 
	if (!frozen[quadrant] &&
	    time_gone_by[quadrant] > last_time_chkd[quadrant])
		get_pms1d_data(quadrant);

	if (update_mode[quadrant] == RECONFIG || (!frozen[quadrant] &&
	    time_gone_by[quadrant] > last_time_chkd[quadrant]))
		PlotPMS1Ddata(quadrant, time_elapsed);

	last_time_chkd[quadrant] = time_gone_by[quadrant];

	if (need_rehilite[quadrant])
		rehilite(quadrant);

	update_mode[quadrant] = NORMAL_UPDATE;

}	/* END UPDATE_PMS1D_DISPLAY */

/***********************  GET_PMS1D_DATA()  *********************************/
get_pms1d_data(quadrant)
int	quadrant;
{
  int		i, j, probenum, newRange;
  int		rePlotBackground = FALSE;
  float		SCALE;
  extern int	*len_derive;
  double	get_derived_value();


  /* Check to see if the FSSP range has changed and if so, redraw the
   * background.
   */
  for (i = 0; i < PMS1Dsetup[quadrant].nprobes; ++i)
    {
    probenum = PMS1Dsetup[quadrant].probenum[i];

    if (PMS1Dprobe[probenum].type != FSSP)
      continue;

    newRange = get_derived_value(PMS1Dprobe[probenum].rangeName,
      (int)((ElapsedTime-1) % get_datablock_size()) * *len_derive);

    if (newRange != PMS1Dprobe[probenum].range)
      {
      update_mode[quadrant] = RECONFIG;
      rePlotBackground = TRUE;
      PMS1Dprobe[probenum].range = newRange;
      }
    }

  if (rePlotBackground)
    {
    (*FillArea[GetDrawType()])(pw_xid[quadrant],
		0,
		0,
		window_width[quadrant],
		window_height[quadrant],
		0);

    PlotPMS1Dbackground(quadrant);
    }

  if (isQuadrantCounts(quadrant))
    {
    for (i = 0; i < PMS1Dsetup[quadrant].nprobes; ++i)
      {
      probenum = PMS1Dsetup[quadrant].probenum[i];

      for (j = 0; j < PMS1Dprobe[probenum].nbins; ++j)
        PMS1Daccum[quadrant].data[i][j] += PMS1Dprobe[probenum].actual[j];
      }
    }
  else
    {
    for (i = 0; i < PMS1Dsetup[quadrant].nprobes; ++i)
      {
      probenum = PMS1Dsetup[quadrant].probenum[i];

      SCALE = 1.0;

      if (isQuadrantYliters(quadrant))
        {
        if (PMS1Dprobe[probenum].type == FSSP ||
	    PMS1Dprobe[probenum].type == ASAS ||
	    PMS1Dprobe[probenum].type == F300)
           SCALE = 1000.0;
        }
      else
        {
        if (PMS1Dprobe[probenum].type == X200 ||
	    PMS1Dprobe[probenum].type == Y200 ||
	    PMS1Dprobe[probenum].type == X260)
           SCALE = (1.0 / 1000.0);
        }

      for (j = 0; j < PMS1Dprobe[probenum].nbins; ++j)
        PMS1Daccum[quadrant].data[i][j]
		+= PMS1Dprobe[probenum].conc[j] / PMS1Dprobe[probenum].width[j]
		* SCALE / PMS1Dsetup[quadrant].sum_period;
      }
    }

}	/* END GET_PMS1D_DATA */

/***********************  INIT_START_TIME()  ********************************/
init_start_time() /* get start time of current WINDS session */
{
 static int attempt=0;
 int hour,minute,second;

 /* wait for the first valid second
  */
 if (!post_processing_mode()) {
  hour=current_hr;
  minute=current_minute;
  second=current_sec;
 } else {
  hour=hhmmss[0+(attempt%get_datablock_size())*3];
  minute=hhmmss[1+(attempt%get_datablock_size())*3];
  second=hhmmss[2+(attempt%get_datablock_size())*3];
 }
 if (hour!=0 || minute!=0 || second!=0) {
/*
got a valid second: initialize start time
*/
  start_hour=hour;
  start_minute=minute;
  start_second=second;
  (void)fprintf(stderr,"init start time: %d:%d:%d\n",
   start_hour,start_minute,start_second);
  start_time_init=TRUE;
 } else if (post_processing_mode()) {
/*
in real-time, each second sent is a new one anyway, so just test each one;
in post-processing mode, bump index to effectively look at next second next
time through. This has the effect, in post-processing mode, of never being
able to start a dataset at midnite, since this looks just like 3 zeroes.
No big deal, we'll just lose that startup second.
*/
  attempt++;
 }
}

/**************************  TimeIsSane()  *******************************/

TimeIsSane(hour,minute,second)
int hour,minute,second;
{
/* 
sanity checks
*/
 if (hour<0 || minute <0 || second <0 || hour>23 || minute>59 || second >59) {
  time_to_log();
/*
this funky time sometimes happens if a WINDS session is abnormally terminated in
devctl mode, then OW is recycled with new WINDS session in an analysis mode 
that needs a larger DERIVE_KEY segment than the previous.  Don't know why, it
seems new session doesn't realize that it needs to re-set the size in getmem().
Simply restart WINDS to remedy, resetting input doesn't help since you need
to adjust shared memory sizes.  For some other unknown reason, "unclean" 
termination of WINDS *without* recycling OW does not confuse the shared mem
mgmt, it knows it must clean up old segments first and then resize for new.
This behavior has been verified on the RDI laptop only, so far (8/6/92).
*/
  (void)fprintf(stderr,"funky time some time after %d:%d:%d --> %d:%d:%d\n",
   last_hr,last_min,last_sec,hour,minute,second);
  return FALSE;
 } 
 return TRUE;
}

/********************  SET_CURRENT_TIME_STRING()  ****************************/

set_current_time_string(hour,minute,second)
int hour,minute,second;
{

 (void)sprintf(current_time,"%02d:%02d:%02d",hour,minute,second);
  last_hr=hour; last_min=minute; last_sec=second;
}

/******************************  SET_TIME()  **********************************/

set_time(newtime)
char *newtime;
{
 DrawControlPanelTime(newtime);
}

/******************************************************************************/

SetMaxLenTimeWindowString()
{
 MaxLenTimeWindowString=10;
 MaxLenTimeWindowString=MAX(MaxLenTimeWindowString,strlen(project));
 MaxLenTimeWindowString=MAX(MaxLenTimeWindowString,strlen(fltno_string));
}

/******************************************************************************/

GetMaxLenTimeWindowString()
{
 return MaxLenTimeWindowString;
}

/******************************************************************************/

DrawControlPanelTime(newtime)
char *newtime;
{
 if (strlen(newtime) > time_length) {
  time_to_log();
  (void)fprintf(stderr,"funky time: %s\n",newtime);
  return;
 }
 (*SetContext[GetDrawType()])(pw_xid[TIME_AND_PROJECT],GXcopy,BLU,WHT,0,
  GetFontID(GetControlPanelFont()));
 (void)(*DrawText[GetDrawType()])((GetTimeWindowWidth()
  -time_length*GetFontWidth(GetControlPanelFont()))/2,
  xv_get(canvas[TIME_AND_PROJECT],XV_HEIGHT)/4-
  GetFontDescent(GetControlPanelFont()),newtime);
}

/**********************  DrawControlPanelDate()  *****************************/

DrawControlPanelDate(date_string)
char *date_string;
/* change the date display in control panel.  No erasure of previous string is
   done, since it is assumed that all date strings sent are of form "mm/dd/yy"
   and will simply overwrite the previous string.
*/
{
int left_margin,top_margin;

 (*SetContext[GetDrawType()])(pw_xid[TIME_AND_PROJECT],GXcopy,BLU,WHT,0,
  GetFontID(GetControlPanelFont()));
 left_margin=(GetTimeWindowWidth()
  -strlen(date_string)*GetFontWidth(GetControlPanelFont()))/2;
 top_margin=2*(xv_get(canvas[TIME_AND_PROJECT],XV_HEIGHT)/4-
  GetFontDescent(GetControlPanelFont()));
 (void)(*DrawText[GetDrawType()])(left_margin,top_margin,date_string);
}

/***********************  DrawControlPanelPnum()  ****************************/

DrawControlPanelPnum()
{
int left_margin,top_margin;

 (*SetContext[GetDrawType()])(pw_xid[TIME_AND_PROJECT],GXcopy,BLU,WHT,0,
  GetFontID(GetControlPanelFont()));
 left_margin=(GetTimeWindowWidth()
  -proj_length*GetFontWidth(GetControlPanelFont()))/2;
 top_margin=3*(xv_get(canvas[TIME_AND_PROJECT],XV_HEIGHT)/4-
  GetFontDescent(GetControlPanelFont()));
 (void)(*DrawText[GetDrawType()])(left_margin,top_margin,project);
}

/***********************  DrawControlPanelFltno()  ***************************/

DrawControlPanelFltno()
{
int left_margin,font_baseline;

 if (!GotFlightNumber())
  return;
 font_baseline=4*(xv_get(canvas[TIME_AND_PROJECT],XV_HEIGHT)/4-
  GetFontDescent(GetControlPanelFont()));
/*
clear out old flight # in case new one is a shorter string, don't want to
leave behind old flight number in background
*/
 (*FillArea[GetDrawType()])(pw_xid[TIME_AND_PROJECT],
  0,font_baseline-GetFontAscent(GetControlPanelFont()),
  GetTimeWindowWidth(),GetFontHeight(GetControlPanelFont()),WHT);
 (*SetContext[GetDrawType()])(pw_xid[TIME_AND_PROJECT],GXcopy,BLU,WHT,0,
  GetFontID(GetControlPanelFont()));
 left_margin=(GetTimeWindowWidth()
  -strlen(fltno_string)*GetFontWidth(GetControlPanelFont()))/2;
 (void)(*DrawText[GetDrawType()])(left_margin,font_baseline,fltno_string);
}

/***************************************************************************/

SetFlightNumber(fltno)
char *fltno;
{
 FlightNumber=(char *)strdup(fltno);
}

/***************************************************************************/

char *
GetFlightNum()
{
 return FlightNumber;
}

/******************************  GET_FLTNO()  ******************************/

char *
get_fltno()
{
 return fltno_string;
}

/******************************  SET_FLTNO()  ******************************/

set_fltno(fltno)
char *fltno;
{
 (void)sprintf(fltno_string,"Flt %s",fltno);
 SetFlightNumber(fltno);
}

/***********************  GotFlightNumber()  ***************************/

GotFlightNumber()
{
 return (FlightNumber!=NULL);
}

/***********************  UpdateSessionTimeFrame()  ***************************/

UpdateSessionTimeFrame(Starthour,Startminute,Startsecond,
Endhour,Endminute,Endsecond)
int Starthour,Startminute,Startsecond,Endhour,Endminute,Endsecond;
{
FILE *session;
char filename[MAX_FLNM_LNGTH];

 (void)sprintf(filename,SESSIONFILE,ProjectDirectory,GetDisplayHost());
 if ( (session=WINDSfopen(filename,"w"))==NULL) {
  return;
 }
 (void)fprintf(session,"%s.%s.%s.%d:%d:%d-to-%d:%d:%d",
  get_month(),get_day(),get_year(),
  Starthour,Startminute,Startsecond,Endhour,Endminute,Endsecond);
 (void)WINDSfclose(session);
}

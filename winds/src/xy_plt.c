/*
    xy_plt.c -- manage and establish xy-plot module

   SCCS ID: 
@(#)xy_plt.c	4.2 10/27/89

*/

#include <xview/xview.h>
#include <sys/time.h>
#include <stdio.h>

/****************************  WINDS INCLUDES  ******************************/

#include <xy_plt.h>
#include <display.h>
#include <constants.h>
#include <help.h>
#include <lengths.h>
#include <files.h>
#include <ops.h>
#include <plotattrs.h>
#include <proto.h>

/************************  SETUP_XYPLOT()  **********************************/

setup_xyplot(quadrant)
int quadrant;
{
 switch (plot_type[quadrant]) {
  case TIME_SERIES:
  case TRACK_PLOT:
  case SKEW_T:
  case SOUNDING:
  case XY:
   setup_plots(quadrant); /* get plots set up */
   break;
  case LISTS:
   (void)setup_lists(quadrant); 
   break;
  case FIXED_FORMAT:
   (void)setup_fix(quadrant);
   break;
  case CALIBRATION:
   break; 
 }
}

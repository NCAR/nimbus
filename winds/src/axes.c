/* axes.c -- deal with axes on plots 

   SCCS ID: 
@(#)axes.c	4.2 10/27/89

*/

/****************************  WINDS INCLUDES  ******************************/

#include <xy_plt.h>
#include <display.h>
#include <lengths.h>
#include <constants.h>
#include <help.h>
#include <ops.h>
#include <macros.h>
#include "proto.h"


/**************************  EXPORTABLE  *************************************/

double time_mults[]={  /* acceptable time range tick mark intervals -- anywhere
                          from 1 second to 2 hours (total range of 8 hours) */
1, 2, 4, 8, 15, 20, 30, 60, 120, 240, 300, 480, 600, 900, 1200, 1800,
2400, 3600, 5400, 7200, 10800, 14400, 18000, 21600, 25200, 28800
};

/************************  GLOBAL WITHIN FILE  *****************************/

static double mults[]={  /* acceptable tick mark intervals  */
0.00005, 0.0001, 0.0002, 0.00025, 0.0005, 
0.001, 0.002, 0.0025, 0.005, 0.01, 0.02, 0.025, 0.05, 0.1, 0.2, 0.25, 0.5, 
1.0, 2.0, 2.5, 5.0, 10.0, 20.0, 25.0, 50.0, 100.0, 200.0, 250.0, 500.0, 
1000.0, 2000.0, 2500.0, 5000.0, 10000.0, 20000.0, 40000.0
};
static double octal_mults[]={  /* acceptable octal tick mark intervals  */
01, 02, 04, 010, 020, 040, 0100, 0200, 0400, 01000, 02000, 04000,
010000, 020000, 040000,  0100000, 0200000
};
static double hex_mults[]={  /* acceptable hex tick mark intervals  */
0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800,
0x1000, 0x2000, 0x4000, 0x8000, 0x10000
};


/*******************  ACCEPTABLE_DECIMAL_INTERVAL()  *************************/

double acceptable_decimal_interval(double x)
/* given x, a 'ballpark' tick mark interval, return its closest counterpart
   in the set of acceptable intervals
*/
{
double up,down;
int j;
short inverted;

 inverted=FALSE;
 if (x < 0) {
  inverted=TRUE;
  x = ABS(x);
 }

 for (j=0; j<NM_MULTS; j++) {
  if (x==mults[j]) {  /* direct hit */
   return(inverted?-mults[j]:mults[j]);
  } else {            /* x is between two members of set  */
   if ( (j<NM_MULTS-1) && (x>mults[j]) && (x<mults[j+1]) ) {
/* return the member which is closest to x  */
    down=x-mults[j];
    up=mults[j+1]-x;
    return (down>=up? (inverted?-mults[j+1]:mults[j+1]):
                      (inverted?-mults[j]:mults[j]));
   } else 
/* x is less than any of the members of acceptable set; return the smallest
   one
*/
    if (x<mults[0])
     return(inverted?-mults[0]:mults[0]); 
 }
 }
/* x is not between any of the members of acceptable set; return the largest
   one
*/
 return(inverted?-mults[NM_MULTS-1]:mults[NM_MULTS-1]);
}

/************************  ACCEPTABLE_HEX_INTERVAL()  ***********************/

double acceptable_hex_interval(double x)
/* given x, a 'ballpark' tick mark interval, return its closest counterpart
   in the set of acceptable intervals
*/
{
double up,down;
int j;
short inverted;

 inverted=FALSE;
 if (x < 0) {
  inverted=TRUE;
  x = ABS(x);
 }

 for (j=0; j<NM_HEX_MULTS; j++) {
  if (x==hex_mults[j]) {  /* direct hit */
   return(inverted?-hex_mults[j]:hex_mults[j]);
  } else {            /* x is between two members of set  */
   if ( (j<NM_HEX_MULTS-1) && (x>hex_mults[j]) && (x<hex_mults[j+1]) ) {
/* return the member which is closest to x  */
    down=x-hex_mults[j];
    up=hex_mults[j+1]-x;
    return (down>=up? (inverted?-hex_mults[j+1]:hex_mults[j+1]):
                      (inverted?-hex_mults[j]:hex_mults[j]));
   } else 
/* x is less than any of the members of acceptable set; return the smallest
   one
*/
    if (x<hex_mults[0])
     return(inverted?-hex_mults[0]:hex_mults[0]); 
 }
 }
/* x is not between any of the members of acceptable set; return the largest
   one
*/
 return(inverted?-hex_mults[NM_HEX_MULTS-1]:hex_mults[NM_HEX_MULTS-1]);
}


/************************  ACCEPTABLE_OCTAL_INTERVAL()  **********************/

double acceptable_octal_interval(double x)
/* given x, a 'ballpark' tick mark interval, return its closest counterpart
   in the set of acceptable intervals
*/
{
double up,down;
int j;
short inverted;

 inverted=FALSE;
 if (x < 0) {
  inverted=TRUE;
  x = ABS(x);
 }

 for (j=0; j<NM_OCTAL_MULTS; j++) {
  if (x==octal_mults[j]) {  /* direct hit */
   return(inverted?-octal_mults[j]:octal_mults[j]);
  } else {            /* x is between two members of set  */
   if ( (j<NM_OCTAL_MULTS-1) && (x>octal_mults[j]) && (x<octal_mults[j+1]) ) {
/* return the member which is closest to x  */
    down=x-octal_mults[j];
    up=octal_mults[j+1]-x;
    return (down>=up? (inverted?-octal_mults[j+1]:octal_mults[j+1]):
                      (inverted?-octal_mults[j]:octal_mults[j]));
   } else 
/* x is less than any of the members of acceptable set; return the smallest
   one
*/
    if (x<octal_mults[0])
     return(inverted?-octal_mults[0]:octal_mults[0]); 
 }
 }
/* x is not between any of the members of acceptable set; return the largest
   one
*/
 return(inverted?-octal_mults[NM_OCTAL_MULTS-1]:octal_mults[NM_OCTAL_MULTS-1]);
}

/*******************  ACCEPTABLE_TIME_INTERVAL()  *************************/

double acceptable_time_interval(double x)
/* given x, a 'ballpark' tick mark interval, return its closest counterpart
   in the set of acceptable intervals
*/
{
double up,down;
int j;

 x = ABS(x);

 for (j=0; j<NM_TIME_MULTS; j++) {
  if (x==time_mults[j]) {  /* direct hit */
   return(time_mults[j]);
  } else {            /* x is between two members of set  */
   if ( (j<NM_TIME_MULTS-1) && (x>time_mults[j]) && (x<time_mults[j+1]) ) {
/* return the member which is closest to x  */
    down=x-time_mults[j];
    up=time_mults[j+1]-x;
    return (down>=up? (time_mults[j+1]):(time_mults[j]));
   } else 
/* x is less than any of the members of acceptable set; return the smallest
   one
*/
    if (x<time_mults[0])
     return(time_mults[0]); 
 }
 }
/* x is not between any of the members of acceptable set; return the largest
   one
*/
 return(time_mults[NM_TIME_MULTS-1]);
}

/*************************  ACCEPTABLE_INTERVAL()  ****************************/

double acceptable_interval(double x, int mode)
{
 switch (mode) {
  case BITS:
   if (hex)
    return(acceptable_octal_interval(x));
   else
    return(acceptable_hex_interval(x));
  case TIME_AXIS:
   return(acceptable_time_interval(x));
  default:
   return(acceptable_decimal_interval(x));
 }
}

/************************  ACCEPTABLE_MIDPT()  ****************************/

double acceptable_midpt(double value, double intrvl)
/* given a data value, return the closest multiple of the interval
*/
{
 return ((double) ( (int)(value/intrvl + (value<0?-0.5:0.5))) * intrvl);
}

/**************************** GET_TIX()  ************************************/

int get_rnglbls(int quadrant, int trace)
/* 
set the character labels dep_rnglbl and associated character lengths 
for each tick mark label, and set the maximum length max_len[] for each set
of such range labels

on exit: dep_rnglbl[], max_dep_len[], indep_rnglbl[], and max_indep_len[]
         are initialized
*/
{
int j;
int num_tix;

 if (trace != INDEPVAR) { 
/* 
get dependent variable range labels 
*/
  max_dep_len[quadrant][trace]=0;
  num_tix=plot_type[quadrant]==SKEW_T?skew_x_tix[quadrant]:NUM_TIX;
  for (j=0; j<num_tix; j++) {
   if (dep_intrvl[quadrant][trace] >= 1 || dep_intrvl[quadrant][trace]<=-1) {
/* 
intervals greater than 1 or less than -1: no need for fractional parts 
*/
    if (dep_mode[quadrant][trace]==BITS) {
     (void)sprintf(dep_rnglbl[quadrant][trace][j],((hex)?"x%05x":"%06o"),
      (int)(dep_max[quadrant][trace]-j*dep_intrvl[quadrant][trace]));
    } else {
     (void)sprintf(dep_rnglbl[quadrant][trace][j],"%lf",
     (plot_type[quadrant]==SKEW_T?
     (dep_min[quadrant][trace]+j*dep_intrvl[quadrant][trace]):
     (dep_max[quadrant][trace]-j*dep_intrvl[quadrant][trace])));
     trim_trailing_digits(dep_rnglbl[quadrant][trace][j],LBL_PRECISION,FALSE);
    }
    max_dep_len[quadrant][trace]=(dep_lbl_len[quadrant][trace][j]=
     strlen(dep_rnglbl[quadrant][trace][j]))>max_dep_len[quadrant][trace]?
     dep_lbl_len[quadrant][trace][j]:max_dep_len[quadrant][trace];
   } else {
/* 
interval less than 1 or -1: retain fractional parts 
*/
    (void)sprintf(dep_rnglbl[quadrant][trace][j],"%lf",
     dep_max[quadrant][trace]-j*dep_intrvl[quadrant][trace]);
    max_dep_len[quadrant][trace]=
     (dep_lbl_len[quadrant][trace][j]=
     trim_trailing_digits(dep_rnglbl[quadrant][trace][j],LBL_PRECISION,TRUE))>
     max_dep_len[quadrant][trace]?
     dep_lbl_len[quadrant][trace][j]:max_dep_len[quadrant][trace];
   }
  }
 } else { 
/* 
get independent variable range labels 
*/
  num_tix=plot_type[quadrant]==SKEW_T?MAX_Y_TIX:NUM_TIX;
  max_indep_len[quadrant]=0;
  for (j=0; j<num_tix; j++) {
   if (indep_intrvl[quadrant] >= 1 || indep_intrvl[quadrant]<=-1) {
/* 
intervals greater than 1 or less than -1: no need for fractional parts 
*/
    if (indep_mode[quadrant]==BITS) {
     (void)sprintf(indep_rnglbl[quadrant][j],((hex)?"x%05x":"%06o"),
      (int)(indep_min[quadrant]+j*indep_intrvl[quadrant]));
    } else {
     (void)sprintf(indep_rnglbl[quadrant][j],"%lf",
      ((plot_type[quadrant]==SKEW_T?
/* 
though the skew-t plot starts at 1050 mb., make first label 1000.0 even
just to make things pretty. Also note that scale is inverted for skew-t,
so multiply interval by -1 to decrement with increasing y-coordinates
*/
     1000.0:indep_min[quadrant])+j*
      (plot_type[quadrant]==SKEW_T?-indep_intrvl[quadrant]:
      indep_intrvl[quadrant])));
     trim_trailing_digits(indep_rnglbl[quadrant][j],LBL_PRECISION,FALSE);
    }
    indep_lbl_len[quadrant][j]=strlen(indep_rnglbl[quadrant][j]);
    max_indep_len[quadrant]=indep_lbl_len[quadrant][j]>
     max_indep_len[quadrant]?indep_lbl_len[quadrant][j]:
     max_indep_len[quadrant];
   } else {
/* 
interval less than 1 or -1: retain fractional parts 
*/
    (void)sprintf(indep_rnglbl[quadrant][j],"%lf",
     indep_min[quadrant]+j*indep_intrvl[quadrant]);
    max_indep_len[quadrant]=(indep_lbl_len[quadrant][j]=
     trim_trailing_digits(indep_rnglbl[quadrant][j],LBL_PRECISION,TRUE))>
     max_indep_len[quadrant]?indep_lbl_len[quadrant][j]:max_indep_len[quadrant];
   }
  }
 }
}

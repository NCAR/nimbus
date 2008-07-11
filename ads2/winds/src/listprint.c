#include <stdio.h>

#include "printers.h"
#include "geom.h"
#include "ops.h"
#include "lengths.h"
#include "socket.h"
#include "display.h"
#include "constants.h"
#include "proto.h"

/* 
TRUE if header needed on printout 
*/
static int HeaderNeeded;                      
static int PrintMode=INITIAL;
/* 
cumulative total number of points on last entry to print_update() 
*/
static int LastTotal=0;   
/* 
how many seconds to NOT print lists to allow catchup at printer 
*/
static int WaitFor=0;     
/* 
keep track of need for form feeds 
*/
static int RowsPrinted=0; 
/* 
which quadrant, if any, is printing lists 
*/
static int QuadPrinting=NONE;                 
/*
*/
static int ColumnStart[MAX_DISPLAY_VARS];
/* 
socket comm'n buffer
*/
char sock_buff[MAX_SOCKET_BUFF_LNGTH];
/*
get data values
*/
float GetDeriveDataValue(),GetRawDataValue(),GetVoltsDataValue();

FILE *listFP, *WINDSfopen();  /* Print file */

/*****************************************************************************/

SetQuadPrinting(int quadrant)
{
  QuadPrinting = quadrant;

  if (quadrant == NONE)
    {
    if (listFP)
      {
      fclose(listFP);
      listFP = NULL;
      system("/home/users/ads/my_lp -h -r -Plp listPrint");
      }
    }
  else
    {
    listFP = WINDSfopen("listPrint", "w+");
    }

}

/*****************************************************************************/

GetQuadPrinting()
{
 return QuadPrinting;
}

/*****************************************************************************/

SetPrintMode(mode)
int mode;
{
 PrintMode=mode;
}

/*****************************************************************************/

SetHeaderNeeded(needed)
int needed;
{
 HeaderNeeded=needed;
}

/*****************************************************************************/

init_printer_parms()
/* 
initialize printer parameters
*/
{
 HeaderNeeded=FALSE;
}

/*****************************  LSTAT_UPDATE()  *****************************/

lstat_update(time_elapsed)
int time_elapsed;
{
static int numpts;
static int start_pt=0;          /* next value for index into shared mem */

 if (PrintMode==INITIAL) {
  start_pt = time_elapsed % get_datablock_size();
  LastTotal=time_elapsed;
 } 

 if ((numpts=new_printer_points(time_elapsed))) {  
/* 
new points since last update  
*/
  get_values(start_pt,numpts,time_elapsed);
 } else {   /*  no new points since last update  */
  PrintMode=NORMAL_UPDATE;
  return;
 }

 start_pt = time_elapsed % get_datablock_size();
 PrintMode=NORMAL_UPDATE;
}

/**********************  GET_VALUES()  **************************************/

get_values(start,num_points,time_elapsed)
int start,num_points,time_elapsed;
/* list values from start for num_points points, for each of num_listees 
   variables.  As well, accumulate statistics values whether printer is
   running or not. 
*/
{
int end_pt,point;
static int index, hr, minut, sec;
int StatsRowsPrinted;

/* for each new data point...  */

   end_pt=start + num_points;  /* here's the last point to be listed */

   for (point=start;point<end_pt;point++){

    index=point%get_datablock_size();

    hr=GetHMS(index*3);
    minut=GetHMS(index*3 + 1);
    sec=GetHMS(index*3 + 2);

    if (WaitFor)

/* count down in real-time seconds to wait for printer to finish with either
   a stats printout or an image before allowing lists to continue print,
   thus preventing a backlog that might not ever get caught up.
*/
     WaitFor--;
    else {

/* if lists are ongoing, print header if needed & then list current value(s) 
*/
     if (QuadPrinting!=NONE) {
      if (HeaderNeeded || RowsPrinted >= GetNumPrintRows())
       print_header(QuadPrinting);
      if (!(time_elapsed%GetListRate(QuadPrinting))) {
       print_value(QuadPrinting,index,hr,minut,sec);
       RowsPrinted++;
      }
     }
    }
/* 
is it stats flush time and stats is printing?  if so, a header will be 
needed for restart of lists
*/
     if ((StatsRowsPrinted=StatsIsFlushed(hr,minut,sec))) {
/* 
printer may have a difficult time catching up with ongoing lists after 
stats output has been printed.  Relieve it of this burden by telling lists 
to minimize catchup.
*/
      if (GetStatsAutoPrintEnabled()) {
       SetWaitTimeForImage(STATSPRINT,StatsRowsPrinted);
       HeaderNeeded=TRUE;
      }
     }
     AccumulateStats(index);
   }  /* for # of points */
}

/************************  WAIT_FOR_PRINTER()  *******************************/

SetWaitTimeForImage(type,geom)
int type,geom;
/* designate the waiting period in ongoing lists during a 'catchup' period 
   -- i.e., when an image or stats has printed out while list printing is
   ongoing, the lists will need to 'catch up' with real time.  That period
   defaults to the current ongoing interval (for now, the only option is
   one per second) unless SetWaitTimeForImage() is called with desired value.
*/
{
int rows;

 if (post_processing_mode()) 

/* don't bother with wait times -- clock is probably not running anyway
*/
  return;

 if (GetCurrentImagePrinterType()==PAINTJET &&
     get_lstat_port()==PJETSOCKET && GetNeedPauseOnLists()) {
/* 
only the color printer doubling as list/stats printer needs a wait interval
*/
  switch (type) {
   case STATSPRINT:
/*
special case: calls with STATSPRINT as 1st argument use the geometry argument
as the # of rows printed; in general case, the geometry determines how much
time will be needed to print the image, but the stats case depends on how
many rows of stats are printed.
*/
    rows=geom;
    WaitFor+=(int)(rows*STATS_ROW_TIME);
    break;
   case BWPRINT:
   case GREYPRINT:
    switch (geom) {
     case GEOM_QUAD:
      WaitFor+=PJ_QUAD_BW_TIME;
      break;
     case GEOM_STRIP:
      WaitFor+=PJ_STRIP_BW_TIME;
      break;
     case GEOM_STACK:
      WaitFor+=PJ_STACK_BW_TIME;
      break;
     case GEOM_FULL:
      WaitFor+=PJ_FULL_BW_TIME;
      break;
    }
    break;
   case COLORPRINT:
    switch (geom) {
     case GEOM_QUAD:
      WaitFor+=PJ_QUAD_COLOR_TIME;
      break;
     case GEOM_STRIP:
      WaitFor+=PJ_STRIP_COLOR_TIME;
      break;
     case GEOM_STACK:
      WaitFor+=PJ_STACK_COLOR_TIME;
      break;
     case GEOM_FULL:
      WaitFor+=PJ_FULL_COLOR_TIME;
      break;
    }
    break;
  }
 } else {
  WaitFor=0;
 }
}

/**********************  PRINT_VALUE()  **************************************/

print_value(quadrant,index,hr,minut,sec)
int quadrant,index;
int hr,minut,sec;
/*
write out all values for current list parameters for a given second of data
*/
{
static char time_label[TIME_CHARS];
static char value[VALUELEN];
int listee;
static int FormatSet=FALSE;
static char ListFloatFormat[8],ListOctalFormat[8],ListHexFormat[8];

 if (get_lstat_port()==ERROR)
  return;

 if (!FormatSet) {
  (void)sprintf(ListFloatFormat,"%%%df",GetListDataLen());
  (void)sprintf(ListOctalFormat,"%%0%do",GetListDataLen());
  (void)sprintf(ListHexFormat,"x%%0%dx",GetListDataLen()-1);
  FormatSet=TRUE;
 }

 (void)sprintf(time_label,"%02d:%02d:%02d ",hr,minut,sec);
/*
see explanation in print_header() for following padding, etc.
*/
 PadWithBlanks(sock_buff,MAX_SOCKET_BUFF_LNGTH);
 (void)sprintf(&sock_buff[0],"%s ",time_label);
 sock_buff[strlen(sock_buff)]=' ';
/*
get current data value for each listee
*/
 for (listee=0; listee<GetNumListees(quadrant); listee++) {
  switch (GetDepVarMode(quadrant,listee)) {
   case BITS:
    (void)sprintf(value,BaseIsHex()?ListHexFormat:ListOctalFormat,
     (GetBitDataValue(index*GetNumRawNames()+dep_ptr[quadrant][listee])));
    break;
   case VOLTAGE:
    (void)sprintf(value,ListFloatFormat,
     (GetVoltsDataValue(index*GetNumRawNames()+dep_ptr[quadrant][listee])));
    break;
   case RAW:
    (void)sprintf(value,ListFloatFormat,
     (GetRawDataValue(index*GetNumRawNames()+dep_ptr[quadrant][listee])));
    break;
   case DERIVED:
    (void)sprintf(value,ListFloatFormat,
     (GetDeriveDataValue(index*GetNumDerivedNames()+dep_ptr[quadrant][listee])));
    break;
  }
  value[GetListDataLen()] = '\0';
  (void)sprintf(&sock_buff[ColumnStart[listee]+
/*
start at last ending position
*/
   (strlen((char *)GetListeeName(quadrant,listee))>GetListDataLen()?
/*
adjust to right justify data values
*/
    strlen((char *)GetListeeName(quadrant,listee))-GetListDataLen():0)],
/*
append space, unless this is last name, in which case do a line feed
*/
   listee<GetNumListees(quadrant)-1?"%s ":"%s\n",value);
  if (listee<GetNumListees(quadrant)-1)
/*
replace null terminator with space unless this is the last name
*/
   sock_buff[strlen(sock_buff)]=' ';
 }  

  /* write out to socket for printing
   */
  fprintf(listFP, sock_buff);

// if (write_to_socket(get_lstat_port(),sock_buff,strlen(sock_buff)) == EBADWRITE)
//  perror("list write_to_socket");
}

/************************  PRINT_HEADER()  ************************************/

print_header(quadrant)
int quadrant;
/* print new header on hardcopy list 

   Do a form feed to minimize confusion between interspersed process printouts. 
   This condition is met whenever list printing is toggled on.  Also do form
   feed whenver header changes (different listees) within same process.

*/

{
static int k;
static int first_entry=TRUE;
static char *year,*month,*day;
char *get_year(),*get_month(),*get_day();
int buff_index;

 if (get_lstat_port()==ERROR)
  return;

 if (first_entry) {
/*
determine date first time around
*/
  first_entry=FALSE;
  month=get_month();
  year=get_year();
  day=get_day();
 }

  list_listee_labels(quadrant);
  buff_index=0;
  RowsPrinted=0;

  fprintf(listFP, "\n\n%s  FLT # %s  DATE %s %s %s \n",
	project,get_fltno(),month,day,year);
  fprintf(listFP, "Listing on %d parameters\n", GetNumListees(quadrant));

/*
we are going to be replacing the null terminator from subsequent sprintf()'s
with a space, to facilitate positioning of names in the sock_buff buffer
w/o padding with spaces one at a time. To keep things cleaned up, then, we
must pad the ENTIRE buffer with white space before beginning.
*/
  PadWithBlanks(sock_buff,MAX_SOCKET_BUFF_LNGTH);
  strcpy(sock_buff, "  TIME   ");
  buff_index = strlen(sock_buff);
  sock_buff[buff_index]=' ';

  /* format the buffer with the variable names
   */
  for (k=0; k<GetNumListees(quadrant); k++) {
   if (strlen((char *)GetListeeName(quadrant,k))>GetListDataLen())
/*
start at last ending position
*/
    sprintf(&sock_buff[buff_index],
/*
append space, unless this is last name, in which case do a line feed
*/
     k < GetNumListees(quadrant)-1 ? "%s " : "%s\n",
      (char *)GetListeeName(quadrant,k));
   else
    sprintf(&sock_buff[
/*
adjust position of name in buffer so that it is right justified
*/
     buff_index+GetListDataLen()-strlen((char *)GetListeeName(quadrant,k))],
     k < GetNumListees(quadrant)-1 ? "%s " : "%s\n",
      (char *)GetListeeName(quadrant,k));
/*
keep track of each names starting column index for future reference in
positioning units and data values in a right justified manner
*/
   ColumnStart[k]=buff_index;
   buff_index=strlen(sock_buff);
   if (k<GetNumListees(quadrant)-1)
/*
replace null terminator with space unless this is the last name
*/
    sock_buff[buff_index]=' ';
  }

  /* write out to socket for printing
   */
  fprintf(listFP, sock_buff);


  /* write out the units - same drill as above for names
   */
  PadWithBlanks(sock_buff,MAX_SOCKET_BUFF_LNGTH);
  strcpy(sock_buff, "hr mn sc ");
  sock_buff[strlen(sock_buff)]=' ';
  for (k=0; k<GetNumListees(quadrant); k++) {
   sprintf(&sock_buff[ColumnStart[k]+
    (strlen((char *)GetListeeName(quadrant,k)) > GetListDataLen() ?
     strlen((char *)GetListeeName(quadrant,k)) - 
     strlen(&units[dep_attr_num[quadrant][k]*UNITSLEN]) :
      GetListDataLen()-strlen(&units[dep_attr_num[quadrant][k]*UNITSLEN]))],
    k < GetNumListees(quadrant)-1 ? "%s" : "%s\n",
    &units[dep_attr_num[quadrant][k]*UNITSLEN]);
   if (k<GetNumListees(quadrant)-1) 
    sock_buff[strlen(sock_buff)]=' ';
  }

  /* write out to socket for printing
   */
  fprintf(listFP, sock_buff);

  RowsPrinted+=NUM_HEADER_ROWS;
  HeaderNeeded=FALSE;
}

/**************************  NEW_PRINTER_POINTS()  ***************************/

new_printer_points(time_elapsed)
int time_elapsed;
/* determine the start point in shared memory for the next drawing cycle
   in time_series(), as a function of mode and time_gone_by
*/
{
int num_points,return_val;

/* normal update -- from point after last point read */

    if ((num_points=time_elapsed - LastTotal)) {
     LastTotal=time_elapsed;
     if (num_points>get_time_frame_size()) 
      num_points=get_time_frame_size();
     return_val=num_points;
    } else {
     return_val=0;
    }
  return(return_val);
}

/* 
   flat_panel.c -- manage data sent out over serial line in general case and
   for special case of parcel tracking processing. 
*/

#include <stdio.h>

#include <printers.h>
#include <socket.h>
#include <lengths.h>
#include <shmem.h>
#include <display.h>
#include <constants.h>
#include <flat_panel.h>
#include <macros.h>
#include <proto.h>

static int flat_panel_data_enabled;   /* TRUE only if various data to be sent 
					out serial line to flat panel display */
static num_flat_panel_parms=0;

/* entries in FLAT_PANEL_PARMS file are assumed entered in following order:

description	user_label	parameter	units	format

... none longer than NAMLEN characters.  The description field is to be left
undisturbed.  Field "format" is to be entered in C float format sense.
*/
static char flat_panel_desc[MAX_FLAT_PANEL_PARMS][NAMLEN*2];
static char flat_panel_label[MAX_FLAT_PANEL_PARMS][NAMLEN*2];
static char flat_panel_parm[MAX_FLAT_PANEL_PARMS][NAMLEN*2];
static char flat_panel_units[MAX_FLAT_PANEL_PARMS][NAMLEN*2];
static char format[MAX_FLAT_PANEL_PARMS][NAMLEN*2];
static char lcd_home[HOME_LNGTH] = {0x11,0x02,0x12,0};
static char flip_home[HOME_LNGTH] = {0x0E, 0x12, 0x16, 0};
static char *home_cmd;

/* special pointer parms for parcel tracking
*/
static int bearing[MAX_PTRS],
           distance[MAX_PTRS],
           altitude[MAX_PTRS];
static int time_parm=ERROR;

/**************************  GET_FLAT_PANEL_PARMS()  *************************/

get_flat_panel_parms(filename)
char *filename;
{
 FILE *WINDSfopen(),*flat_panel_file;
 int parm;

/* flag special parcel tracking parms in case this is a general display setup
   without parcel tracking
*/
 for (parm=0; parm<MAX_PTRS; parm++)
  bearing[parm]=distance[parm]=altitude[parm]=ERROR;

 if ( (flat_panel_file=WINDSfopen(filename,"r")) == NULL) {
  (void)fprintf(stderr,"WARNING: no flat_panel data present\n");
  flat_panel_data_enabled=FALSE;
  return;
 }

 while (fscanf(flat_panel_file,
  "%s %s %s %s %s",
  &flat_panel_desc[num_flat_panel_parms][0],
  &flat_panel_label[num_flat_panel_parms][0],
  &flat_panel_parm[num_flat_panel_parms][0],
  &flat_panel_units[num_flat_panel_parms][0],
  &format[num_flat_panel_parms][0]
  )!=EOF) {
/*
 determine whether parcel tracking is to be enabled
*/
   if (!strcmp(flat_panel_desc[num_flat_panel_parms],BEARING1))
    bearing[0]=num_flat_panel_parms;
   else if (!strcmp(flat_panel_desc[num_flat_panel_parms],BEARING2))
    bearing[1]=num_flat_panel_parms;
   else if (!strcmp(flat_panel_desc[num_flat_panel_parms],ALTITUDE1))
    altitude[0]=num_flat_panel_parms;
   else if (!strcmp(flat_panel_desc[num_flat_panel_parms],ALTITUDE2))
    altitude[1]=num_flat_panel_parms;
   else if (!strcmp(flat_panel_desc[num_flat_panel_parms],DISTANCE1))
    distance[0]=num_flat_panel_parms;
   else if (!strcmp(flat_panel_desc[num_flat_panel_parms],DISTANCE2))
    distance[1]=num_flat_panel_parms;
   else if (!strcmp(flat_panel_desc[num_flat_panel_parms],TIME_STRING))
    time_parm=num_flat_panel_parms;
   flat_panel_label[num_flat_panel_parms][NAMLEN-1]='\0';
   flat_panel_parm[num_flat_panel_parms][NAMLEN-1]='\0';
   flat_panel_units[num_flat_panel_parms][NAMLEN-1]='\0';
   num_flat_panel_parms++;
 }
 (void)WINDSfclose(flat_panel_file);

/* set hardware specific parameters
*/
 if (lcd_display())
  home_cmd=lcd_home;
 else if (flip_display())
  home_cmd=flip_home;
 else
  (void)fprintf(stderr,"WARNING: neither LCD nor FLIP is requested!");
}

/**************************  NUM_FLAT_PANEL_VALUES()  ************************/

num_flat_panel_values()
{
/* with parcel data enabled, there are three components of the pointer that
   have alternates, totalling 11 possible parameters; otherwise, maximum 8
*/
 return ParclDataToFlatPanel()?num_flat_panel_parms:MIN(num_flat_panel_parms,FP_ROWS*2);
}

/**************************  ParclDataToFlatPanel()  **************************/

ParclDataToFlatPanel()
{
int pointer;

 if (!flat_panel_data_enabled)
  return FALSE;

 for (pointer=0; pointer<MAX_PTRS; pointer++)
  if (bearing[pointer]==ERROR || distance[pointer]==ERROR ||
      altitude[pointer]==ERROR)
   return FALSE;
  return TRUE;
}

/**************************  SET_FLAT_PANEL_FLAG()  **************************/

set_flat_panel_flag(filename)
char *filename;
{
 if ((flat_panel_data_enabled=flat_panel_display_data_needed())) {
  get_flat_panel_parms(filename);
 }
}

/*****************************  FLAT_PANEL_DATA()  ***************************/

flat_panel_data()
{
 return flat_panel_data_enabled;
}

/*******************   FORMAT_BUFFER()  ***********************************/

format_buffer(buff,value,point,mode)
char *buff;
int value,point,mode;

/* manage positioning of label-value pairs so that: for each different
   point, labels are lined up left-justified and values are lined up right-
   justified; values are output in user-specified format; if magnitude of
   value exceeds format capacity, maintain the overall length of printed
   value (and thus the columns stay lined up) by truncating from the right
   end of value; maintain logical row lengths of FP_ROW_LNGTH, assuming
   2 label-value pairs per row.  Avoid line feeds on LCD display; these 
   confuse that device.

   Caller sends a character buffer "buff", index into entries "value",
   index into data array(s) "point", and raw or derived indicator "mode".
   
   Returned is buff, in format as described above.
*/
{
char temp[FP_ROW_LNGTH];
static int index,next_print;
static int last_point=-1;
static int extra_chars;
int value_length,indx;
int time_format;
int save;

 if (point!=last_point) {

/* 1st time through with new point
*/
  extra_chars=index=next_print=0;
  last_point=point;
  (void)sprintf(&buff[0],"\0");
 }

/* set flag if value is current time
*/
 time_format=(value==time_parm);

/* space between left half's value and right half label
*/
 if (!time_format)
  (void)sprintf(&buff[index],(next_print%2)?" %s":"%s",
  &flat_panel_label[value][0]);
 else
  (void)sprintf(&buff[index]," ");

/***
printf("after label: %s, %d\n",buff,strlen(buff));
***/

/* fill rest of current half of row with spaces
*/
  for (save=0,indx=strlen(buff); 
       save<(FP_ROW_LNGTH/2)-
       (time_format?strlen(current_time):strlen(flat_panel_label[value]))

/* left half of row uses 10 spaces; right half has one less due to space after
   left half's value
*/
       -(next_print%2)?1:0; 
       save++,indx++)
   (void)sprintf(&buff[indx]," ");
/***
printf("after padding: %s, %d\n",buff,strlen(buff));
***/

/* get value printed in format specified
*/
 switch (mode) {
  case TIME_AXIS:
   (void)sprintf(&temp[0],"%s",current_time);
   break;
  case DERIVED:
   (void)sprintf(&temp[0],format[value],
    get_derived_value(flat_panel_parm[value],(point%get_datablock_size())**len_derive));
   break;
  case RAW:
   (void)sprintf(&temp[0],format[value],
    get_raw_value(flat_panel_parm[value],(point%get_datablock_size())**len_raw));
   break;
 }

/* determine how many characters of value to copy, find correct starting point 
   in buffer, insert value
*/
 if (time_format)
  value_length=strlen(current_time);
 else 
  (void)sscanf(format[value],"%%%d",&value_length);

 ++next_print;
 indx=next_print*(FP_ROW_LNGTH/2)-value_length+extra_chars;
 (void)strncpy(&buff[indx],temp,value_length);
 (void)sprintf(&buff[indx+value_length],"\0");

/***
printf("after adding value: %s, %d\n",buff,strlen(buff));
***/

/* right most column on flip display; write a carriage return except on very
   last one
*/
  if (flip_display() && !(next_print % 2) && value<num_flat_panel_values()-1) {
   (void)sprintf(&buff[strlen(buff)],"\r\n");
   extra_chars+=2;
/***
printf("after adding line feeds: %s, %d\n",buff,strlen(buff));
***/
  }

/* update index for next write
*/
 index=strlen(buff);

}

/**************************  SEND_FLAT_PANEL_DATA()  *************************/

send_flat_panel_data(point)
int point;
{
int value,mode;
static char buff[FP_ROW_LNGTH*(FP_ROWS+1)];
static int last_point=-1;

 if (point==last_point)
  return;

 value=0;
 for (value=0; value<num_flat_panel_values(); value++) {
  if (value==time_parm) {

   format_buffer(buff,value,point,TIME_AXIS);

  } else if ((mode=get_mode_by_name(flat_panel_parm[value]))!=ERROR) {
   if (mode==DERIVED) {

    if (value==distance[0] || value==distance[1]) {

/* for distance, altitude, and bearing, output only the designated pair
*/
     if (value==distance[get_current_parcel()]) {
      format_buffer(buff,value,point,DERIVED);
     }
    } else if (value==bearing[0] || value==bearing[1]) {
     if (value==bearing[get_current_parcel()]) {
      format_buffer(buff,value,point,DERIVED);
     } 
    } else if (value==altitude[0] || value==altitude[1]) {
     if (value==altitude[get_current_parcel()]) {
      format_buffer(buff,value,point,DERIVED);
     }
    } else {

/* other than distance, altitude, or bearing: output for either RAW or DERIVED
*/
     format_buffer(buff,value,point,DERIVED);
    }
   } else {
    format_buffer(buff,value,point,RAW);
   }
  }
 }
 if (write_to_socket(FLAT_PANEL_DATA,&home_cmd[0],strlen(&home_cmd[0])) 
   ==EBADWRITE)
  perror("flat_panel home cmd write_to_socket");
 if (serial_feed_debug())
  (void)fprintf(stderr,"FLAT_PANEL: \n%s\n",buff);
 if (write_to_socket(FLAT_PANEL_DATA,&buff[0],strlen(&buff[0])) ==EBADWRITE)
  perror("flat_panel data write_to_socket");
 last_point=point;
}

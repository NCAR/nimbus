/* config.c -- manage global attributes as defined in project-specific 
   config file
*/
#include <stdio.h>
#include <lengths.h>
#include <config.h>
#include <files.h>
#include <constants.h>
#include <display.h>
#include <proto.h>


/****************************  GET_NUM_DISPLAY_TYPES()  **********************/

get_num_display_types()
{
 return MAX_DISPLAY_TYPES;
}

/****************************  GET_CONFIG()  *********************************/

get_config()
{
 char charval1[MAX_ARG_LNGTH], charval2[MAX_ARG_LNGTH];
 char arg[MAX_ARG_LNGTH];
 int quadrant,mode;
 int intval1;

/* 
start with default settings and initializations
*/
 for (quadrant=0; quadrant<get_num_displays(); quadrant++)
  set_defaults(quadrant);

 if (open_config_file()==ERROR)
  return;
/* 
read in over-riding setup parameters
*/
 while (fscanf(config_file,"%s",&arg[0])!=EOF) {
  mode=ERROR;
  if (!strcmp(arg,VECTORS_KEYWD)) {
   mode=VECTORS;
  } else if (!strcmp(arg,SHMEM_KEYWD)) {
   mode=SHMEM_SEX;
  } else if (!strcmp(arg,MAXSETUPS_KEYWD)) {
   mode=MAXSETUPS;
  } 
  switch (mode) {
   case MAXSETUPS:
    (void)fscanf(config_file,"%d",&intval1);
    set_maxsetups(intval1);
    break;
   case SHMEM_SEX:
    (void)fscanf(config_file,"%d",&intval1);
/* 
do NOT set shared memory limit here, assuming that it is already done
with an explicit call to get_config_parm(SHMEM_SEX) in both display and input
processes. That value may have already been adjusted at that time to 
compensate for inadequately sized system shared memory segments, so we don't
want to read the value back in here and undo that adjustment!
     set_shmem_size(intval1);
*/
    break;
   case VECTORS:
    (void)fscanf(config_file,"%s %s",charval1,charval2);
/* 
truncate names as needed, ensure capitalization
*/
    charval1[NAMLEN-1]='\0'; charval2[NAMLEN-1]='\0';
    for (quadrant=0; quadrant<get_num_displays(); quadrant++)
     set_vector_components(quadrant,charval1,charval2);
    break;
   case ERROR:
    break;
  }
 }
 (void)WINDSfclose(config_file);
 return;
}

/* config_mgmt.c -- manage global attributes as defined in project-specific 
   config file
*/
#include <stdio.h>
#include <lengths.h>
#include <config.h>
#include <files.h>
#include <constants.h>
#include <display.h>
#include <proto.h>

static char arg[MAX_ARG_LNGTH];
static int size;

FILE	*config_file;

/****************************  OPEN_CONFIG_FILE()  **************************/

open_config_file()
{
 char filename[MAX_FLNM_LNGTH];

/* open configuration or defaults file
*/
  (void)sprintf(&filename[0],CONFIG_FILE,ProjectDirectory,proj_number);
  if ( (config_file=WINDSfopen(filename,"r")) == NULL) {
   (void)WINDSfclose(config_file);
   (void)fprintf(stderr,"no config file present -- using defaults\n");
   (void)sprintf(&filename[0],DEF_CONFIG_FILE,ProjectDirectory);
   if ( (config_file=WINDSfopen(filename,"r")) == NULL) {
    (void)fprintf(stderr,"no default config file present -- big trouble\n");
    return(ERROR);
   }
  }
  return TRUE;
}

/**************************  GET_CONFIG_PARM() ********************************/

int get_config_parm(parm)
int parm;
{
 if (open_config_file()==ERROR)
  return ERROR;

 while (fscanf(config_file,"%s",&arg[0])!=EOF) {
  switch (parm) {
   case SHMEM_SEX:
    if (!strcmp(arg,SHMEM_KEYWD)) {
     (void)fscanf(config_file,"%d",&size);
     (void)WINDSfclose(config_file);
     return size;
    }
    break;
  }
 }
 (void)WINDSfclose(config_file);
 return ERROR;
}


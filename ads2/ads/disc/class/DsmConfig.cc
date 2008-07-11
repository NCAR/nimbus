/* DsmConfig.cc
   Reads dsm configuration information from the config file.

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <DsmConfig.h>
 
/******************************************************************************
** Public Functions
******************************************************************************/

DsmConfig::DsmConfig ()

// Constructor.  Reads in the config file information.
{
/* If executing on a DSM, read the user configuration jumper.  If the network
   is not enabled, then don't attempt to read the configuration file.  Instead
   just run with the current configuration in battery backed sram.
*/
#ifdef VXWORKS
  if (MCC_USER_CFG & MCC_USER_CFG_NONET) {
    stand_alone = TRUE;
    firstDsm();
    fprintf (stderr, "\nThe network user configuration jumper is disabled.\n");
    fprintf (stderr, "Operating in the standalone mode.\n");
    taskDelay (sysClkRateGet() * 5);
    return;
  }
#endif
 
// Open and read the dsm config file.  
  if (openConfig() == OK) {
    readHeaderName();
    readLabelLines();
    readConfigLines();
    closeConfig();
  }
#ifndef VXWORKS
  else {
    exit (ERROR);		// fatal error in the non-DSM environment
  }
#endif

  stand_alone = FALSE;
  firstDsm();
}
/*****************************************************************************/

int DsmConfig::nextDsm ()
 
// Selects the next config file entry.
{
  if ((dsm_idx + 1) < dsm_cnt) {
    dsm_idx++;
    return TRUE;
  }
  return FALSE;
}
/*****************************************************************************/
 
int DsmConfig::selectByLocn (char *locn)
 
// Selects a config entry by the matching location.
{
  int idx;

  for (idx = 0; (idx < dsm_cnt) && strcmp (locations[idx], locn); idx++);

  if (idx < dsm_cnt) { 
    dsm_idx = idx;
    return TRUE;
  }
  fprintf (stderr, "DsmConfig: Unknown dsm location --> %s.\n", locn);
  return FALSE;
}
/*****************************************************************************/
 
int DsmConfig::selectByHost (char *host)
 
// Selects a config entry by the matching hostname.
{
  int idx;

  for (idx = 0; (idx < dsm_cnt) && strcmp (host_names[idx], host); idx++)
    printf("%s %s\n", host_names[idx], host);
 
  if (idx < dsm_cnt) { 
    dsm_idx = idx;
    return TRUE;
  }

  fprintf (stderr, "DsmConfig: Unknown host --> %s.\n", host);
  return FALSE;
}
/*****************************************************************************/
 
int DsmConfig::selectByIndex (int index)
 
// Selects a config entry by the index.
{
  if ((index >= 0) && (index < dsm_cnt)) {
    dsm_idx = index;
    return TRUE;
  }
 
  fprintf (stderr, "DsmConfig: Invalid index --> %d.\n", index);
  return FALSE;
}
/******************************************************************************
** Private Functions
******************************************************************************/
 
int DsmConfig::openConfig ()
 
// Open the config file.
{
  static char tstr[80], t1str[80], *p;

// Open the dsmconfig file.
#ifdef VXWORKS
extern BOOT_PARAMS sysBootParams;

  (void)sprintf(tstr, "%s/hosts/%s/dsmconfig", 
                getenv("PROJ_DIR"), sysBootParams.hostName);

#else
  p = getenv("HOST");
  strcpy(t1str, p);
  if ((p = strchr(t1str, '.')))
    *p = '\0';

  (void)sprintf(tstr, "%s/hosts/%s/dsmconfig", getenv("PROJ_DIR"), t1str);
#endif
 
  printf("config file = [%s]\n", tstr);

  if ((fp = fopen (tstr, "r")) == NULL) {
    perror ("DsmConfig: fopen");
    return ERROR;
  }

  return OK;
}

/*****************************************************************************/
void DsmConfig::readHeaderName ()
 
// Reads the header path name from the the config file.
{
  if (!(int)fgets (main_header_name, DSM_MAX_LINE, fp)) {
    perror ("DsmConfig, read main header name:");
    exit (ERROR);
  }

// Discard the eol carriage return.
  main_header_name[strlen (main_header_name) - 1] = '\0';

//printf ("header name = %s.\n", main_header_name);
}
/*****************************************************************************/

void DsmConfig::readLabelLines ()
 
// Reads past the label fields in the file.
{
  char tstr[DSM_MAX_LINE];

// Read the blank line.
  if (!(int)fgets (tstr, DSM_MAX_LINE, fp)) {
    perror ("DsmConfig, read blank line:");
    exit (ERROR);
  }

// Read the labels.
  if (!(int)fgets (tstr, DSM_MAX_LINE, fp)) {
    perror ("DsmConfig, read label fields:");
    exit (ERROR);
  }
 
// Read the underlines.
  if (!(int)fgets (tstr, DSM_MAX_LINE, fp)) {
    perror ("DsmConfig, read underline fields:");
    exit (ERROR);
  }
}
/*****************************************************************************/
 
void DsmConfig::readConfigLines ()
 
// Reads the lines from the config file.
{
  char tstr1[8];
  char tstr2[8];
  char tstr3[8];
  char tstr4[8];

  for (dsm_cnt = 0; (dsm_cnt < MAX_DSM) && 
       (fscanf (fp, "%s %s %d %s %s %s %s", locations[dsm_cnt], 
       host_names[dsm_cnt], &dsm_ports[dsm_cnt], tstr1, tstr2, tstr3,tstr4) 
       != EOF);
       dsm_cnt++) {
       
//  printf ("locn[%1d] = %s\n", dsm_cnt, locations[dsm_cnt]);
//  printf ("host[%1d] = %s\n", dsm_cnt, host_names[dsm_cnt]);

    if (!strcmp (tstr1, DSM_CONFIG_YES_STR))
      time_master[dsm_cnt] = TRUE;
    else
      time_master[dsm_cnt] = FALSE;

    if (!strcmp (tstr2, DSM_CONFIG_YES_STR))
      local_record[dsm_cnt] = TRUE;
    else
      local_record[dsm_cnt] = FALSE;

    if (!strcmp (tstr3, DSM_CONFIG_YES_STR))
      nats_[dsm_cnt] = TRUE;
    else
      nats_[dsm_cnt] = FALSE;

    if (!strcmp (tstr4, DSM_CONFIG_YES_STR))
      serial_time[dsm_cnt] = TRUE;
    else
      serial_time[dsm_cnt] = FALSE;

// Build the dsm and ctl header names by appending the location to the 
// main header name
    strcpy (dsm_header_name[dsm_cnt], main_header_name);
    strcat (dsm_header_name[dsm_cnt], ".");
    strcat (dsm_header_name[dsm_cnt], locations[dsm_cnt]); 
  }

}
/*****************************************************************************/

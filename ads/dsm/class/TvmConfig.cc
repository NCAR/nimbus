/* TvmConfig.cc
   Reads tvm display configuration information from the tvmconfig file.

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <TvmConfig.h>
 
/******************************************************************************
** Public Functions
******************************************************************************/

TvmConfig::TvmConfig (char *host, char *locn)

// Constructor.  
{

  tvm_idx = 0;

// Open and read the dsm config file.  
  if (openConfig (host) == OK) {
    tvm_cnt = 0;
    readLabelLines();
    readConfigLines (locn);
    closeConfig();
  }

// If the config file cannot be opened, then operate in the standalone mode 
// using the existing sram setup.
}
/*****************************************************************************/
 
int TvmConfig::nextVar ()
 
// Selects the next tvmconfig file entry for this dsm.
{
  if ((tvm_idx + 1) < tvm_cnt) {
    tvm_idx++;
    return TRUE;
  }
  return FALSE;
}
/*****************************************************************************/
 
int TvmConfig::selectByName (char *name)
 
// Selects a config entry by the matching variable name.
{
  int idx;

  for (idx = 0; (idx < tvm_cnt) && strcmp (var_name[idx], name); idx++);

  if (idx < tvm_cnt) { 
    tvm_idx = idx;
    return TRUE;
  }
  fprintf (stderr, "TvmConfig: Unknown variable name, %s.\n", name);
  return FALSE;
}
/******************************************************************************
** Private Functions
******************************************************************************/
 
int TvmConfig::openConfig (char *host)
 
// Open the config file.
{
  static char tstr[80];

// Open the dsmconfig file.
  (void)sprintf(tstr, "%s/hosts/%s/tvmconfig", getenv("PROJ_DIR"), host);
 
  if (!(int)(fp = fopen (tstr, "r"))) {
    perror ("TvmConfig: fopen");
    return ERROR;
  }
  return OK;
}
/*****************************************************************************/
 
void TvmConfig::readLabelLines ()
 
// Reads past the label fields in the file.
{
  str12 tstr;

// Read the labels.
  if (fscanf (fp, "%s %s %s %s %s", tstr, tstr, tstr, tstr, tstr) == EOF) { 
    fprintf (stderr, 
      "Error reading the label fields from the tvmconfig file.\n");
    exit (ERROR);
  }

// Read the underlines.
  if (fscanf (fp, "%s %s %s %s %s", tstr, tstr, tstr, tstr, tstr) == EOF) { 
    fprintf (stderr, 
      "Error reading the underline fields from the tvmconfig file.\n");
    exit (ERROR);
  }
}
/*****************************************************************************/
 
void TvmConfig::readConfigLines (char *locn)
 
// Reads the lines from the config file.
{
  str12 location;
  int stat;
/**
  printf ("location display_type var_name display_number display_entry\n");
**/
  for (tvm_cnt = 0; (tvm_cnt < TVM_FIXED_FMT_TOTAL_ENTRIES) && 
       ((stat = fscanf (fp, "%s %s %s %d %d", location, display_type[tvm_cnt], 
        var_name[tvm_cnt], &display_number[tvm_cnt], &display_entry[tvm_cnt])) 
        != EOF);) {

// Check if this entry is for this dsm location. Keep it if it is.
    if (!strcmp (locn, location)) {
//      printf ("%s %s %s %d %d\n", location, display_type[tvm_cnt], 
//        var_name[tvm_cnt], display_number[tvm_cnt], display_entry[tvm_cnt]);
      tvm_cnt++;
    }
  }
}
/*****************************************************************************/

/* VarConfig.cc
   Reads derived variable configuration information from the varconfig file.


   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <VarConfig.h>
 
/******************************************************************************
** Public Functions
******************************************************************************/

VarConfig::VarConfig (const char *host, const char *locn)

// Constructor.  
{

  var_cnt = 0;
  var_idx = 0;
  dep_idx = 0;
  memset (dep_cnt, 0, sizeof(dep_cnt));

// Open and read the dsm config file.  
  openConfig (host);
  readLabelLines();
  readConfigLines (locn);
  closeConfig();
  
  firstVar();
}
/*****************************************************************************/
 
int VarConfig::nextVar ()
 
// Selects the next variable entry for this dsm.
{
  if ((var_idx + 1) < var_cnt) {
    var_idx++;
    firstDepend();
    return TRUE;
  }
  return FALSE;
}
/*****************************************************************************/
 
int VarConfig::nextDepend ()
 
// Selects the next dependency variable for the current entry.
{
  if ((dep_idx + 1) < dep_cnt[var_idx]) {
    dep_idx++;
    return TRUE;
  }
  return FALSE;
}
/*****************************************************************************/
 
int VarConfig::selectByName (const char *name)
 
// Selects an entry by matching the variable name.
{
  int i;

  for (i = 0; (i < var_cnt) && strcmp (var_name[i], name); i++);

  if (i < var_cnt) { 
    var_idx = i;
    firstDepend();
    return TRUE;
  }
  fprintf (stderr, "VarConfig: Unknown variable name, %s.\n", name);
  return FALSE;
}
/******************************************************************************
** Private Functions
******************************************************************************/
 
void VarConfig::openConfig (const char *host)
 
// Open the config file.
{
  char tstr[80], *p;

// Open the dsmconfig file.
  if ((p = strchr(host, '.')))
    *p = '\0';

// Open the dsmconfig file.
  (void)sprintf(tstr, "%s/hosts/%s/varconfig", getenv("PROJ_DIR"), host);
 
  if (!(int)(fp = fopen (tstr, "r"))) {
    perror ("VarConfig: fopen");
    exit (ERROR);
  }
}
/*****************************************************************************/
 
void VarConfig::readLabelLines ()
 
// Reads past the label fields in the file.
{
  char tstr[VAR_MAX_LINE];

// Read the labels.
  if (!(int)fgets (tstr, VAR_MAX_LINE, fp)) { 
    perror ("VarConfig, read label fields:");
    exit (ERROR);
  }

// Read the underlines.
  if (!(int)fgets (tstr, VAR_MAX_LINE, fp)) { 
    perror ("VarConfig, read underline fields:");
    exit (ERROR);
  }
}
/*****************************************************************************/
 
void VarConfig::readConfigLines (const char *locn)
 
// Reads the lines from the config file.
{
  str12 loc;
  char tstr[VAR_MAX_LINE];
  int len;

// Read file lines into tstr.
  for (var_cnt = 0; (var_cnt < VAR_MAX_ENTRIES) && 
       (int)fgets (tstr, VAR_MAX_LINE, fp);) { 

// Read the fields from the line.
    len = sscanf (tstr, "%s %s %s %s %s %s", loc, var_name[var_cnt], 
                 dep_name[var_cnt][0], dep_name[var_cnt][1], 
                 dep_name[var_cnt][2], dep_name[var_cnt][3]);

// Check if this entry is for this dsm location. Keep it if it is.
    if (!strcmp (locn, loc)) {
      dep_cnt[var_cnt] = len - 2;	// reads minus loc and name 
      var_cnt++;
    }
  }
}
/*****************************************************************************/

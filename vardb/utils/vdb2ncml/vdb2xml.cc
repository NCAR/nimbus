/*
-------------------------------------------------------------------------
OBJECT NAME:	vdb2ncml.cc

FULL NAME:	Convert VarDB to NcML.

DESCRIPTION:	Converts VarDB, Categories, StandardNames, DerivedNames,
		DependTable, ModVars and AircraftSpecs to NcML.

COPYRIGHT:	University Corporation for Atmospheric Research, 2006
-------------------------------------------------------------------------
*/

#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netcdf.h>
#include <libgen.h>

#include <raf/vardb.h>

#include <cstdio>
#include <sys/types.h>

#include <netcdf.h>
#include <raf/portable.h>


static char defaultProjDir[1000];
static char projDir[1000];

static const char COMMENT = '#';
static const float fill_value = -32767.0;

extern long	VarDB_nRecords;

/* -------------------------------------------------------------------- */
void checkModVars(int ncid, int varID, const char *varName)
{
  char fileName[500], buffer[1000], name[100];
  strcpy(fileName, defaultProjDir);
  strcat(fileName, "ModVars");

  FILE *fp;
  if ((fp = fopen(fileName, "r")) == NULL)
  {
    return;
  }

  float vals[2];
  while (fgets(buffer, BUFSIZ, fp))
  {
    if (buffer[0] == COMMENT)
      continue;

    sscanf(buffer, "%s %f %f", name, &vals[0], &vals[1]);

    if (strcmp(name, varName) == 0)
      nc_put_att_float(ncid, varID, "modulus_range", NC_FLOAT, 2, vals);
  }

  fclose(fp);
}

/* -------------------------------------------------------------------- */
void checkDerivedNames(int ncid, int varID, const char *varName)
{
  char fileName[500], buffer[1000], *p;
  strcpy(fileName, defaultProjDir);
  strcat(fileName, "DerivedNames");

  FILE *fp;
  if ((fp = fopen(fileName, "r")) == NULL)
  {
    return;
  }

  while (fgets(buffer, BUFSIZ, fp))
  {
    if (buffer[0] == COMMENT)
      continue;

    p = strtok(buffer, " \t");

    if (strcmp(p, varName) == 0)
    {
      p = strtok(NULL, "\n");
      while (isspace(*p)) ++p;
      nc_put_att_text(ncid, varID, "derive", strlen(p)+1, p);
    }
  }

  fclose(fp);
}

/* -------------------------------------------------------------------- */
void checkDependencies(int ncid, int varID, const char *varName)
{
  char fileName[500], buffer[1000], *p;
  strcpy(fileName, projDir);
  strcat(fileName, "DependTable");

  FILE *fp;
  if ((fp = fopen(fileName, "r")) == NULL)
  {
    return;
  }

  while (fgets(buffer, BUFSIZ, fp))
  {
    if (buffer[0] == COMMENT)
      continue;

    p = strtok(buffer, " \t\n");

    if (p && strcmp(p, varName) == 0)
    {
      if ( (p = strtok(NULL, "\n")) )
      {
        while (isspace(*p)) ++p;
        nc_put_att_text(ncid, varID, "Dependencies", strlen(p)+1, p);
      }
      else
        nc_put_att_text(ncid, varID, "Dependencies", 1, "");
    }
  }

  fclose(fp);
}

/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  int	i = 1, ncid, timeDim, varID, noDim;
  char	outFile[512];
  const char *p;

  if (argc < 2)
  {
    fprintf(stderr, "Usage: vdbdump [-a] [proj_num | VarDB_filename]\n");
    return(1);
  }

//===================================
//================================
//====================
FILE* vdb;
vdb=fopen("VDB.xml","a");
fprintf(vdb,"  <variableCatalog>\n");
//================================
//====================
//=============================


  if (InitializeVarDB(argv[i]) == ERR)
  {
    fprintf(stderr, "vdb2ncml: Initialize failure.\n");
    return(1);
  }

  strcpy(defaultProjDir, getenv("PROJ_DIR"));
  strcat(defaultProjDir, "/Configuration/raf/");

  if (strchr(argv[i], '/'))
  {
    strcpy(projDir, argv[i]);
    dirname(projDir);
    strcat(projDir, "/");
  }
  else
    projDir[0] = '\0';

  strcpy(outFile, argv[i]);
  strcat(outFile, ".nc");

  if (nc_create(outFile, 0, &ncid) != NC_NOERR)
  {
    fprintf(stderr, "vdb2ncml: Can't create output file %s.\n", outFile);
    return(1);
  }


  // Time dimension.  Do we have to have a dimension?
  nc_def_dim(ncid, "Time", NC_UNLIMITED, &timeDim);


  // Time variable.
  nc_def_var(ncid, "Time", NC_INT, 1, &timeDim, &varID);
  p = "time of measurement";
  nc_put_att_text(ncid, varID, "long_name", strlen(p)+1, p);
  p = "seconds since yyyy-mm-dd hh:mm:ss +0000";
  nc_put_att_text(ncid, varID, "units", strlen(p)+1, p);
  p = "seconds since %F %T %z";
  nc_put_att_text(ncid, varID, "strptime_format", strlen(p)+1, p);

  // AircraftSpecs - boom_len.
  float boom_len = 5.18; // C-130
  nc_def_var(ncid, "N130AR", NC_INT, 0, 0, &varID);
  nc_put_att_float(ncid, varID, "boom_length", NC_FLOAT, 1, &boom_len);
  p = "TASHC MR THETA THETAE RHUM ATTACK SSLIP ATX DPXC EDPC PALT PSX PSXC QCX QCXC TASX TTX XMACH2 THETAV ZERO ONE PALTF IAS";
  nc_put_att_text(ncid, varID, "always_derive", strlen(p)+1, p);

  boom_len = 4.42; // GV
  nc_def_var(ncid, "N677F", NC_INT, 0, 0, &varID);
  nc_put_att_float(ncid, varID, "boom_length", NC_FLOAT, 1, &boom_len);
  p = "TASHC MR THETA THETAE RHUM ATTACK SSLIP ATX DPXC EDPC PALT PSX PSXC QCX QCXC TASX TTX XMACH2 THETAV ZERO ONE PALTF IAS";
  nc_put_att_text(ncid, varID, "always_derive", strlen(p)+1, p);

  boom_len = 4.21; // NRL-P3
  nc_def_var(ncid, "NRL-P3", NC_INT, 0, 0, &varID);
  nc_put_att_float(ncid, varID, "boom_length", NC_FLOAT, 1, &boom_len);
  p = "TASHC MR THETA THETAE RHUM ATTACK SSLIP ATX DPXC EDPC PALT PSX PSXC QCX QCXC TASX TTX XMACH2 THETAV ZERO ONE PALTF";
  nc_put_att_text(ncid, varID, "always_derive", strlen(p)+1, p);

  boom_len = 4.52; // Electra
  nc_def_var(ncid, "N308D", NC_INT, 0, 0, &varID);
  nc_put_att_float(ncid, varID, "boom_length", NC_FLOAT, 1, &boom_len);
  p = "TASHC MR THETA THETAE RHUM ATTACK SSLIP ATX DPXC EDPC PALT PSX PSXC QCX QCXC TASX TTX XMACH2 THETAV ZERO ONE PALTF";
  nc_put_att_text(ncid, varID, "always_derive", strlen(p)+1, p);


  for (i = 0; i < VarDB_nRecords; ++i)
  {
    struct var_v2 * vp = &((struct var_v2 *)VarDB)[i];

    if (isdigit(vp->Name[0]))
    {
      continue;
    }



//==============================================================================================================================
////==============================================================================================================================
////==============================================================================================================================
////==================================Julian Quick ===============================================================================
////===============================VDB 2 XML Additions============================================================================
////==============================================================================================================================

fprintf(vdb,"    <variable name=\"%s\">\n",vp->Name);
fprintf(vdb,"      <units>%s</units>\n",vp->Units);
fprintf(vdb,"      <long_name>%s</long_name>\n",vp->Title);
if (ntohl(vp->is_analog)!=0)
{
  fprintf(vdb,"      <is_analog>true</is_analog>\n");
  fprintf(vdb,"      <voltage_range>%d %d</voltage_range>\n",ntohl(vp->voltageRange[0]),ntohl(vp->voltageRange[1]));
  fprintf(vdb,"      <default_sample_rate>%d</default_sample_rate>\n",ntohl(vp->defaultSampleRate));
}else
{
  fprintf(vdb,"      <is_analog>false</is_analog>\n");
}
if(vp->MaxLimit-vp->MinLimit!=0)
{
  fprintf(vdb,"      <min_limit>%f</min_limit>\n",ntohf(vp->MinLimit));
  fprintf(vdb,"      <max_limit>%f</max_limit>\n",ntohf(vp->MaxLimit));
}
fprintf(vdb,"      <category>%s</category>\n",VarDB_GetCategoryName(vp->Name));
fprintf(vdb,"      <standard_name>%d</standard_name>\n",ntohl(vp->standard_name));
if(ntohl(vp->reference)!=0)
{
  fprintf(vdb,"      <reference>true</reference>\n");
}else
{
  fprintf(vdb,"      <reference>false</reference>\n");
}
fprintf(vdb,"    </variable>\n");
////==============================================================================================================================
////==============================================================================================================================
////==============================================================================================================================
////==============================================================================================================================
////==============================================================================================================================
////==============================================================================================================================


    nc_def_var(ncid, vp->Name, NC_FLOAT, 1, &timeDim, &varID);

    nc_put_att_float(ncid, varID, "_FillValue", NC_FLOAT, 1, &fill_value);

    p = vp->Units;
    if (strcmp(p, "mb") == 0 || strcmp(p, "mbar") == 0)
      p = "hPa";
    nc_put_att_text(ncid, varID, "units", strlen(p)+1, p);

    p = vp->Title;
    nc_put_att_text(ncid, varID, "long_name", strlen(p)+1, p);

    p = VarDB_GetCategoryName(vp->Name);
    if (strcmp(p, "None"))
      nc_put_att_text(ncid, varID, "Category", strlen(p)+1, p);
    p = VarDB_GetStandardNameName(vp->Name);
    if (strcmp(p, "None"))
      nc_put_att_text(ncid, varID, "standard_name", strlen(p)+1, p);

    if (fabs(VarDB_GetMinLimit(vp->Name)) + fabs(VarDB_GetMaxLimit(vp->Name)) > 0.0001)
    {
      float   range[2];

      range[0] = VarDB_GetMinLimit(vp->Name);
      range[1] = VarDB_GetMaxLimit(vp->Name);
      nc_put_att_float(ncid, varID, "valid_range", NC_FLOAT, 2, range);
    }


    checkModVars(ncid, varID, vp->Name);
    checkDerivedNames(ncid, varID, vp->Name);
    checkDependencies(ncid, varID, vp->Name);

// What about Spikes & Lags?  Put in aircraft specific?
// What about default global_attrs; coordinates, etc.
  }









  ReleaseVarDB();
  nc_close(ncid);
//=============
//=========================
fprintf(vdb,"  </variableCatalog>\n");
fprintf(vdb,"  <Dictionary>\n");
fprintf(vdb,"    <definition name=\"units\">Units which signal is reported in</definition>\n");
fprintf(vdb,"    <definition name=\"alternativeUnits\">We can put a definition here when we're ready!</definition>\n");
fprintf(vdb,"    <definition name=\"long_name\">Description of signal</definition>\n");
fprintf(vdb,"    <definition name=\"is_analog\">Boolean describing if signal is analog. True means the signal is analog.</definition>\n");
fprintf(vdb,"    <definition name=\"voltage_range\">Minimum and maximum voltage [insert voltage units here]</definition>\n");
fprintf(vdb,"    <definition name=\"default_sample_rate\">Signal's default sample rate</definition>\n");
fprintf(vdb,"    <definition name=\"min_limit\">Minimum possible value signal can drop to</definition>\n");
fprintf(vdb,"    <definition name=\"max_limit\">Maximum possible value signal can drop to</definition>\n");
fprintf(vdb,"    <definition name=\"category\">Used to group signals</definition>\n");
fprintf(vdb,"    <definition name=\"modulus_range\">We can put a definition here!</definition>\n");
fprintf(vdb,"    <definition name=\"derive\">We can put a definition here!</definition>\n");
fprintf(vdb,"    <definition name=\"dependencies\">We can put a definition here!</definition>\n");
fprintf(vdb,"    <definition name=\"static_lag\">We can put a definition here!</definition>\n");
fprintf(vdb,"    <definition name=\"spike_slope\">We can put a definition here!</definition>\n");
fprintf(vdb,"    <definition name=\"standard_name\">We can put a definition here!</definition>\n");
fprintf(vdb,"    <definition name=\"reference\">We can put a definition here!</definition>\n");
fprintf(vdb,"  </Dictionary>\n");
fprintf(vdb,"</VarDB>");
fclose(vdb);
//==============================
//==============
  return(0);

}	/* END MAIN */

/* END VDB2NCML.CC */

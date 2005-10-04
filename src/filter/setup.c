/*
-------------------------------------------------------------------------
OBJECT NAME:	setup.c

FULL NAME:	Command CallBacks

ENTRY POINTS:	LoadSetup_OK()
		SaveSetup_OK()
		Set_SetupFileName()

STATIC FNS:	none

DESCRIPTION:	Contains functions for Reading/Writing Setup Files.
		window.

INPUT:			

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	cb_main.c (LoadSetup(), SaveSetup()).

COPYRIGHT:	University Corporation for Atmospheric Research, 1995-2005
-------------------------------------------------------------------------
*/

#include <sys/param.h>
#include <unistd.h>
#include <Xm/ToggleB.h>
#include "nimbus.h"
#include "decode.h"
#include "gui.h"
#include "injectsd.h"

static char	SetupFileName[MAXPATHLEN];

extern char *dataQuality[];
extern SyntheticData sd;

/* -------------------------------------------------------------------- */
void Set_SetupFileName(char s[])
{
  strcpy(SetupFileName, s);
}
 
/* END SET_SETUPFILENAME */

/* -------------------------------------------------------------------- */
void LoadSetup_OK(Widget w, XtPointer client, XmFileSelectionBoxCallbackStruct *call)
{
  int	indx;
  FILE	*fp;
  char	*file, *target;
  bool	ProductionSetup;

  ProductionSetup = w ? false : true;

  if (ProductionSetup)
    file = buffer;
  else
    if (cfg.Interactive())
      ExtractFileName(call->value, &file);
    else
      file = SetupFileName;

  if (strlen(file) < 2)
    return;

  if ((fp = fopen(file, "r")) == NULL)
    if (!ProductionSetup)
      {
      sprintf(buffer, "LoadSetup: Can't open %s.", file);
      HandleError(buffer);
      return;
      }
    else
      return;

  if (cfg.Interactive() && !ProductionSetup)
    FileCancel((Widget)NULL, (XtPointer)NULL, (XtPointer)NULL);


  sprintf(&buffer[500], "Loading setup file: %s\n", file);
  LogMessage(&buffer[500]);


  /* Get Project Number.
   */
  fgets(buffer, 128, fp);

  if (atoi(strchr(buffer, '=')+1) != atoi(cfg.ProjectNumber().c_str()))
    {
    fclose(fp);
    HandleError("Invalid file, project number does not match.");
    return;
    }


  fgets(buffer, 128, fp);

  if (!ProductionSetup || cfg.ProductionRun())
    switch (atoi(strchr(buffer, '=')+1))
      {
      case Config::HighRate:
        SetHighRate(NULL, NULL, NULL);
        break;
      case Config::SampleRate:
        SetSampleRate(NULL, NULL, NULL);
        break;
      case Config::LowRate:
        SetLowRate(NULL, NULL, NULL);
        break;
      }

  while (fgets(buffer, 2048, fp))
    {
    if (buffer[0] == COMMENT || buffer[0] == '\n')
      continue;

    target = strtok(buffer, "=");

    if (strcmp(target, "SDI") == 0)
      {
      target = strtok(NULL, " \t");

      if ((indx = SearchTable(sdi, target)) == ERR)
        {
        char	tmp[64];

        sprintf(tmp, "LoadSetup: can't find %s.\n", target);
        LogMessage(tmp);
        continue;
        }

      if (!ProductionSetup || cfg.ProductionRun())
        sdi[indx]->Dirty = true;


      while ( (target = strtok(NULL, " \t")) )
        {
        if (strncmp(target, "O=", 2) == 0)
          if (!ProductionSetup || cfg.ProductionRun())
            sdi[indx]->Output = atoi(&target[2]);

        if (strncmp(target, "OR=", 3) == 0)
          if (!ProductionSetup || cfg.ProductionRun())
            sdi[indx]->OutputRate = atoi(&target[3]);

        if (strncmp(target, "SL=", 3) == 0)
          sdi[indx]->StaticLag = atoi(&target[3]);

        if (strncmp(target, "SS=", 3) == 0)
          sdi[indx]->SpikeSlope = (NR_TYPE)atof(&target[3]);

        if (strncmp(target, "DQ=", 3) == 0)
          sdi[indx]->DataQuality = SearchDataQuality(&target[3]);

        if (strncmp(target, "nCOEF=", 6) == 0)
          {
          size_t order = atoi(strchr(target, '=')+1);

          sdi[indx]->cof.clear();
          for (size_t i = 0; i < order; ++i)
            sdi[indx]->cof.push_back((float)atof(strtok(NULL, " \t")));
          }
        }
      }
    else
    if (strcmp(target, "RAW") == 0)
      {
      target = strtok(NULL, " \t");

      if ((indx = SearchTable(raw, target)) == ERR)
        {
        char	tmp[64];

        sprintf(tmp, "LoadSetup: can't find %s.\n", target);
        LogMessage(tmp);
        continue;
        }

      if (!ProductionSetup || cfg.ProductionRun())
        raw[indx]->Dirty = true;


      while ( (target = strtok(NULL, " \t")) )
        {
        if (strncmp(target, "O=", 2) == 0)
          if (!ProductionSetup || cfg.ProductionRun())
            raw[indx]->Output = atoi(&target[2]);

        if (strncmp(target, "OR=", 3) == 0)
          if (!ProductionSetup || cfg.ProductionRun())
            raw[indx]->OutputRate = atoi(&target[3]);

        if (strncmp(target, "DQ=", 3) == 0)
          raw[indx]->DataQuality = SearchDataQuality(&target[3]);

        if (strncmp(target, "SL=", 3) == 0)
          raw[indx]->StaticLag = atoi(&target[3]);

        if (strncmp(target, "SS=", 3) == 0)
          raw[indx]->SpikeSlope = (NR_TYPE)atof(&target[3]);

        if (strncmp(target, "nCOEF=", 6) == 0)
          {
          size_t order = atoi(strchr(target, '=')+1);

          raw[indx]->cof.clear();
          for (size_t i = 0; i < order; ++i)
            raw[indx]->cof[i] = (float)atof(strtok(NULL, " \t"));
          }
        }
      }
    else
    if (strcmp(target, "DERIVED") == 0)
      {
      target = strtok(NULL, " \t");

      if ((indx = SearchTable(derived, target)) == ERR)
        {
        char	tmp[64];

        sprintf(tmp, "LoadSetup: can't find %s.\n", target);
        LogMessage(tmp);
        continue;
        }

      if (!ProductionSetup || cfg.ProductionRun())
        derived[indx]->Dirty = true;

      while ( (target = strtok(NULL, " \t")) )
        {
        if (strncmp(target, "O=", 2) == 0)
          if (!ProductionSetup || cfg.ProductionRun())
            derived[indx]->Output = atoi(&target[2]);

        if (strncmp(target, "OR=", 3) == 0)
          if (!ProductionSetup || cfg.ProductionRun())
            derived[indx]->OutputRate = atoi(&target[3]);

        if (strncmp(target, "DQ=", 3) == 0)
          derived[indx]->DataQuality = SearchDataQuality(&target[3]);

        if (strncmp(target, "nDEP=", 5) == 0)
          {
          derived[indx]->ndep = atoi(strchr(target, '=')+1);

          for (size_t i = 0; i < derived[indx]->ndep; ++i)
            {
            strcpy(derived[indx]->depend[i], strtok(NULL, " \t\n"));
            DependIndexLookup(derived[indx], i);
            }
          }
        }
      }
    else
    if (strcmp(target, "DEFAULT") == 0)
      {
      char	def_name[20];
      NR_TYPE	f[128];

      strcpy(def_name, strtok(NULL, " \t"));

      size_t n = atoi(strtok(NULL, " \t"));

      for (size_t i = 0; i < n; ++i)
        f[i] = (NR_TYPE)atof(strtok(NULL, " \t"));

      SetDefaultsValue(def_name, f);
      }
    else
      {
      char msg[200];
      sprintf(msg, "Invalid keyword [%s] found in setup file, ignoring.\n",
	target);
      LogMessage(msg);
      continue;
      }
    }

  fclose(fp);

  FillListWidget();

}	/* END LOADSETUP_OK */

/* -------------------------------------------------------------------- */
void LoadSynthetic_OK(Widget w, XtPointer client, XmFileSelectionBoxCallbackStruct *call)
{
  char *file;
  ExtractFileName(call->value, &file); 

  if (strlen(file) == 0 || access(file, R_OK) == ERR)
    {
    HandleError("Non-existent input file.");
    return;
    }

  sd.InitSynth(file);
  FileCancel((Widget)NULL, (XtPointer)NULL, (XtPointer)NULL);

}

/* -------------------------------------------------------------------- */
void SaveSetup_OK(Widget w, XtPointer client, XmFileSelectionBoxCallbackStruct *call)
{
  FILE	*fp;
  char	*file;

  if (w)
    ExtractFileName(call->value, &file);
  else
    file = buffer;

  if ((fp = fopen(file, "w+")) == NULL)
    {
    sprintf(&buffer[500], "Can't create %s.", file);
    HandleError(&buffer[500]);
    return;
    }

  if (w)
    FileCancel((Widget)NULL, (XtPointer)NULL, (XtPointer)NULL);


  fprintf(fp, "ProjNum=%s\n", cfg.ProjectNumber().c_str());
  fprintf(fp, "PRate=%d\n", (int)cfg.ProcessingRate());

  for (size_t i = 0; i < sdi.size(); ++i)
    if (sdi[i]->Dirty)
      {
      fprintf(fp, "SDI=%s O=%d ", sdi[i]->name, sdi[i]->Output);

      if (sdi[i]->StaticLag != 0)
        fprintf(fp, "SL=%d ", sdi[i]->StaticLag);

      if (sdi[i]->SpikeSlope != 0.0)
        fprintf(fp, "SS=%e ", sdi[i]->SpikeSlope);

      fprintf(fp, "DQ=%s OR=%d nCOEF=%d",
		sdi[i]->DataQuality, sdi[i]->OutputRate, sdi[i]->cof.size());

      for (size_t j = 0; j < sdi[i]->cof.size(); ++j)
        fprintf(fp, " %e", sdi[i]->cof[j]);

      fprintf(fp, "\n");
      }

  for (size_t i = 0; i < raw.size(); ++i)
    if (raw[i]->Dirty)
      {
      fprintf(fp, "RAW=%s O=%d ", raw[i]->name, raw[i]->Output);

      if (raw[i]->StaticLag != 0)
        fprintf(fp, "SL=%d ", raw[i]->StaticLag);

      if (raw[i]->SpikeSlope != 0.0)
        fprintf(fp, "SS=%e ", raw[i]->SpikeSlope);

      fprintf(fp, "DQ=%s OR=%d nCOEF=%d", 
		raw[i]->DataQuality, raw[i]->OutputRate, raw[i]->cof.size());

      for (size_t j = 0; j < raw[i]->cof.size(); ++j)
        fprintf(fp, " %e", raw[i]->cof[j]);

      fprintf(fp, "\n");
      }

  for (size_t i = 0; i < derived.size(); ++i)
    if (derived[i]->Dirty)
      {
      fprintf(fp, "DERIVED=%s O=%d DQ=%s OR=%d nDEP=%d",
				derived[i]->name,
				derived[i]->Output,
				derived[i]->DataQuality,
				derived[i]->OutputRate,
				derived[i]->ndep);

      for (size_t j = 0; j < derived[i]->ndep; ++j)
        fprintf(fp, " %s", derived[i]->depend[j]);

      fprintf(fp, "\n");
      }

  SaveDefaults(fp);

  fclose(fp);

}	/* END SAVESETUP_OK */

/* END SETUP.C */

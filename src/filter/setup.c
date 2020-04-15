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

void ProcessSetupLineRAW(RAWTBL *, char *, bool);
void ProcessSetupLineDERIVED(DERTBL *, char *, bool);

static char	SetupFileName[MAXPATHLEN];

extern SyntheticData sd;

/* -------------------------------------------------------------------- */
void Set_SetupFileName(char s[])
{
  strcpy(SetupFileName, s);
}

/* -------------------------------------------------------------------- */
void LoadSetup_OK(Widget w, XtPointer client, XmFileSelectionBoxCallbackStruct *call)
{
  int	indx;
  FILE	*fp;
  char	*file, *target, line[256];
  bool	ProductionSetup;

  ProductionSetup = w ? false : true;

  if (ProductionSetup)
    file = buffer;
  else
  {
    if (cfg.Interactive())
      ExtractFileName(call->value, &file);
    else
      file = SetupFileName;
  }

  if (strlen(file) < 2)
    return;

  if ((fp = fopen(file, "r")) == NULL)
  {
    if (!ProductionSetup)
    {
      sprintf(buffer, "LoadSetup: Can't open %s.", file);
      HandleError(buffer);
      return;
    }
    else
      return;
  }

  if (cfg.Interactive() && !ProductionSetup)
    FileCancel((Widget)NULL, (XtPointer)NULL, (XtPointer)NULL);


  sprintf(&buffer[500], "Loading setup file: %s\n", file);
  LogMessage(&buffer[500]);


  /* Get Project Number.
   */
  {
  char name[256];
  fgets(buffer, 128, fp);
  strcpy(name, strchr(buffer, '=')+1);
  name[strlen(name)-1] = '\0';

  if (cfg.ProjectNumber().compare(name))
  {
    fclose(fp);
    HandleError("Invalid file, project number does not match.");
    return;
  }
  }


  fgets(buffer, 128, fp);

  if (!ProductionSetup)
  {
    long value = atoi(strchr(buffer, '=')+1);
    switch (value)
    {
      case 25:
      case 50:
        SetHighRate(NULL, (void*)value, NULL);
        break;
      case Config::SampleRate:
        SetSampleRate(NULL, NULL, NULL);
        break;
      case Config::LowRate:
        SetLowRate(NULL, NULL, NULL);
       break;
    }
  }

  while (fgets(buffer, 2048, fp))
  {
    if (buffer[0] == COMMENT || buffer[0] == '\n')
      continue;

    strcpy(line, buffer);
    target = strtok(line, "=");

    if (InterpKey.compare(target) == 0)
    {
      target = strtok(NULL, " \t\n");
      if (Interp_Linear.compare(target) == 0)
        cfg.SetInterpolationType(Config::Linear);
      if (Interp_Cubic.compare(target) == 0)
        cfg.SetInterpolationType(Config::CubicSpline);
      if (Interp_Akima.compare(target) == 0)
        cfg.SetInterpolationType(Config::AkimaSpline);
    }
    else
    if (TwoD_AreaRatioKey.compare(target) == 0)
    {
      target = strtok(NULL, " \t\n");
      cfg.SetTwoDAreaRejectRatio(atof(target));
    }
    else
    if (TwoD_MethodKey.compare(target) == 0)
    {
      target = strtok(NULL, " \t\n");
      if (TwoD_MethodCenterIn.compare(target) == 0)
        cfg.SetTwoDProcessingMethod(Config::Center_In);
      if (TwoD_MethodRecon.compare(target) == 0)
        cfg.SetTwoDProcessingMethod(Config::Reconstruction);
    }
    else
    // SDI is ADS2 only.  It was Analog and Digital Counters.  All fall under RAW now.
    if (strcmp(target, "SDI") == 0 || strcmp(target, "RAW") == 0)
    {
      target = strtok(NULL, " \t");

      if ((indx = SearchTable(raw, target)) == ERR)
      {
        char	tmp[64];

        sprintf(tmp, "LoadSetup: can't find %s.\n", target);
        LogMessage(tmp);
        continue;
      }

      ProcessSetupLineRAW(raw[indx], target, ProductionSetup);
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

      ProcessSetupLineDERIVED(derived[indx], target, ProductionSetup);
    }
    else
    if (strcmp(target, "PROBE") == 0)
    {
      char suffix[64];
      target = strtok(NULL, " \t");
      strcpy(suffix, target);

      for (size_t i = 0; i < raw.size(); ++i)
      {
        if ( strstr(raw[i]->name, suffix) )
        {
          strcpy(line, buffer);
          target = strtok(line, "=");
          target = strtok(NULL, " \t");

          ProcessSetupLineRAW(raw[i], target, ProductionSetup);
        }
      }

      for (size_t i = 0; i < derived.size(); ++i)
      {
        if ( strstr(derived[i]->name, suffix) )
        {
          strcpy(line, buffer);
          target = strtok(line, "=");
          target = strtok(NULL, " \t");

          ProcessSetupLineDERIVED(derived[i], target, ProductionSetup);
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


  if (cfg.InterpolationType() != cfg.DefaultInterpolationType())
  {
    fprintf(fp, "%s=", InterpKey.c_str());
    if (cfg.InterpolationType() == Config::CubicSpline)
      fprintf(fp, "%s", Interp_Cubic.c_str());
    else
    if (cfg.InterpolationType() == Config::AkimaSpline)
      fprintf(fp, "%s", Interp_Akima.c_str());
    fprintf(fp, "\n");
  }

  if (cfg.TwoDAreaRejectRatio() != cfg.DefaultPMS2DAreaRatioReject())
    fprintf(fp, "%s=%.2f\n", TwoD_AreaRatioKey.c_str(), cfg.TwoDAreaRejectRatio());

  if (cfg.TwoDProcessingMethod() != cfg.DefaultPMS2DProcessingMethod())
  {
    fprintf(fp, "%s=", TwoD_MethodKey.c_str());
    if (cfg.TwoDProcessingMethod() == Config::Center_In)
      fprintf(fp, "%s", TwoD_MethodCenterIn.c_str());
    else
    if (cfg.TwoDProcessingMethod() == Config::Reconstruction)
      fprintf(fp, "%s", TwoD_MethodRecon.c_str());
    fprintf(fp, "\n");
  }


  for (size_t i = 0; i < raw.size(); ++i)
  {
    if (raw[i]->Dirty)
    {
      fprintf(fp, "RAW=%s O=%d ", raw[i]->name, raw[i]->Output);

      if (raw[i]->StaticLag != 0)
        fprintf(fp, "SL=%d ", raw[i]->StaticLag);

      if (raw[i]->SpikeSlope != 0.0)
        fprintf(fp, "SS=%e ", raw[i]->SpikeSlope);

      fprintf(fp, "DQ=%s OR=%zu",
		raw[i]->DataQuality, raw[i]->OutputRate);

      if (cfg.CalibrationsAppliedBy() == Config::NIMBUS || !cfg.ProductionRun())
      {
        fprintf(fp, " nCOEF=%zu", raw[i]->cof.size());

        for (size_t j = 0; j < raw[i]->cof.size(); ++j)
          fprintf(fp, " %e", raw[i]->cof[j]);
      }

      fprintf(fp, "\n");
    }
    else
    if (raw[i]->Output == false)
      fprintf(fp, "RAW=%s O=%d\n", raw[i]->name, raw[i]->Output);
  }

  for (size_t i = 0; i < derived.size(); ++i)
  {
    if (derived[i]->Dirty)
    {
      fprintf(fp, "DERIVED=%s O=%d DQ=%s OR=%zu nDEP=%zu",
				derived[i]->name,
				derived[i]->Output,
				derived[i]->DataQuality,
				derived[i]->OutputRate,
				derived[i]->nDependencies);

      for (size_t j = 0; j < derived[i]->nDependencies; ++j)
        fprintf(fp, " %s", derived[i]->depend[j]);

      fprintf(fp, "\n");
    }
    else
    if (derived[i]->Output == false)
      fprintf(fp, "DERIVED=%s O=%d\n", derived[i]->name, derived[i]->Output);
  }


  SaveDefaults(fp);

  fclose(fp);

}	/* END SAVESETUP_OK */

void ProcessSetupLineRAW(RAWTBL *rp, char *target, bool ProductionSetup)
{
  if (!ProductionSetup || cfg.ProductionRun())
    rp->Dirty = true;

  while ( (target = strtok(NULL, " \t")) )
  {
    if (strncmp(target, "O=", 2) == 0)
      if (!ProductionSetup || cfg.ProductionRun())
        rp->Output = atoi(&target[2]);

    if (strncmp(target, "OR=", 3) == 0)
      if (!ProductionSetup || cfg.ProductionRun())
        rp->OutputRate = atoi(&target[3]);

    if (strncmp(target, "DQ=", 3) == 0)
      rp->DataQuality = SearchDataQuality(&target[3]);

    if (strncmp(target, "SL=", 3) == 0)
      rp->StaticLag = atoi(&target[3]);

    if (strncmp(target, "SS=", 3) == 0)
      rp->SpikeSlope = (NR_TYPE)atof(&target[3]);

    if (strncmp(target, "nCOEF=", 6) == 0)
    {
      if (cfg.CalibrationsAppliedBy() == Config::NIMBUS || !cfg.ProductionRun())
      {
        size_t order = atoi(strchr(target, '=')+1);

        rp->cof.clear();
        for (size_t i = 0; i < order; ++i)
          rp->cof.push_back((float)atof(strtok(NULL, " \t")));
      }
    }
  }
}

void ProcessSetupLineDERIVED(DERTBL *dp, char *target, bool ProductionSetup)
{
  if (!ProductionSetup || cfg.ProductionRun())
    dp->Dirty = true;

  while ( (target = strtok(NULL, " \t")) )
  {
    if (strncmp(target, "O=", 2) == 0)
      if (!ProductionSetup || cfg.ProductionRun())
        dp->Output = atoi(&target[2]);

    if (strncmp(target, "OR=", 3) == 0)
      if (!ProductionSetup || cfg.ProductionRun())
        dp->OutputRate = atoi(&target[3]);

    if (strncmp(target, "DQ=", 3) == 0)
      dp->DataQuality = SearchDataQuality(&target[3]);

    if (strncmp(target, "nDEP=", 5) == 0)
    {
      dp->nDependencies = atoi(strchr(target, '=')+1);

      for (size_t i = 0; i < dp->nDependencies; ++i)
      {
        strcpy(dp->depend[i], strtok(NULL, " \t\n"));
        DependIndexLookup(dp, i, false);
      }
    }
  }
}

/* END SETUP.C */

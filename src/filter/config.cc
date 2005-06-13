/*
-------------------------------------------------------------------------
OBJECT NAME:	config.cc

FULL NAME:	Initialize

COPYRIGHT:	University Corporation for Atmospheric Research, 2005
-------------------------------------------------------------------------
*/

#include "nimbus.h"

/* -------------------------------------------------------------------- */
Config::Config()
{
  SetInteractive(true);
  SetProductionRun(false);
  SetDespiking(true);
  SetTimeShifting(true);
  SetQCenabled(false);
  SetProcessingMode(PostProcessing);
  SetAsyncFileEnabled(false);
  SetLoadProductionSetup(true);
  SetHoneyWellCleanup(true);
  SetInertialShift(true);

  SetProcessingRate(LowRate);
  SetInterpolationType(Linear);

  SetADSVersion(ADS_2);
}

/* END CONFIG.CC */

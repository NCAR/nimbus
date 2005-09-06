/*
-------------------------------------------------------------------------
OBJECT NAME:	config.cc

COPYRIGHT:	University Corporation for Atmospheric Research, 2005
-------------------------------------------------------------------------
*/

#include "config.h"

/* -------------------------------------------------------------------- */
Config::Config()
{
  SetInteractive(true);
  SetProductionRun(false);
  SetDespiking(true);
  SetTimeShifting(true);
  SetQCenabled(false);
  SetProcessingMode(PostProcessing);
  SetOutputNetCDF(true);
  SetOutputSQL(true);
  SetTransmitToGround(false);
  SetAsyncFileEnabled(false);
  SetLoadProductionSetup(true);
  SetHoneyWellCleanup(true);
  SetInertialShift(true);

  SetProcessingRate(LowRate);
  SetInterpolationType(Linear);

  SetADSVersion(ADS_2);
}

// END CONFIG.CC

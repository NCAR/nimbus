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
  SetQCenabled(false);		// Currently not implemented.
  SetProcessingMode(PostProcessing);
  SetOutputNetCDF(true);
  SetOutputSQL(false);		// Currently real-time only.
  SetTransmitToGround(false);	// real-time only.
  SetAsyncFileEnabled(false);
  SetLoadProductionSetup(true);
  SetHoneyWellCleanup(true);
  SetInertialShift(true);

  SetProcessingRate(LowRate);
  SetHRTRate(TwentyFive);
  SetInterpolationType(Linear);

  SetDespikeReporting(true);
  SetLagErrorReporting(false);

  SetADSVersion(ADS_2);
}

// END CONFIG.CC

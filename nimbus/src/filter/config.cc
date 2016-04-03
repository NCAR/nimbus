/*
-------------------------------------------------------------------------
OBJECT NAME:	config.cc

COPYRIGHT:	University Corporation for Atmospheric Research, 2005-2011
-------------------------------------------------------------------------
*/

#include "config.h"

#include <cstdio>

const Config::interpolationType Config::_defaultInterp = Config::Linear;
const Config::pms2dProcessing Config::_defaultPMS2DProcessingMethod = Center_In;
const float Config::_defaultPMS2DAreaRatioReject = 0.1;

/* -------------------------------------------------------------------- */
Config::Config() : _adsFileExtension(".ads")
{
  SetInteractive(true);
  SetProductionRun(false);
  SetDespiking(true);
  SetTimeShifting(true);
  SetBlankoutVariables(true);
  SetQCenabled(false);		// Currently not implemented.
  SetProcessingMode(PostProcessing);
  SetOutputNetCDF(true);
  SetOutputSQL(false);		// Currently real-time only.
  SetTransmitToGround(false);	// real-time only.
  SetGroundFeedDataRate(5);	// 5 second default
  SetCalibrationsToBeAppliedBy(NIMBUS);
  SetAsyncFileEnabled(false);
  SetLoadProductionSetup(true);
  SetHoneyWellCleanup(true);
  SetInertialShift(true);

  // Enabled by default, even though they only take effect in real-time.
  SetEnableBroadcast(true);
  SetWarnTimeLags(true);

  // These correspond to defaults originally from psql.cc.
  SetAnalyzeInterval(1500);
  SetVacuumInterval(1000);

  SetCreateKMLFile(true);
  SetCreateNavFile(false);

  SetProcessingRate(LowRate);
  SetHRTRate(TwentyFive);

  SetDespikeReporting(true);
  SetLagErrorReporting(false);

  SetADSVersion(ADS_2);
  SetGroundFeedType(UDP);

  SetInterpolationType(_defaultInterp);
  SetTwoDProcessingMethod(_defaultPMS2DProcessingMethod);
  SetTwoDAreaRejectRatio(_defaultPMS2DAreaRatioReject);
}

std::string
Config::AircraftString()
{
   if (Aircraft() == Config::HIAPER)
      return "GV";
   if (Aircraft() == Config::NRL_P3)
      return "NRLP3";
   if (Aircraft() == Config::C130)
      return "C130";

  fprintf(stderr, "config.cc: No aircraft set for AircraftString()\n");
  return "";
}

Config::interpolationType
Config::DefaultInterpolationType()
{
  return _defaultInterp;
}

Config::pms2dProcessing
Config::DefaultPMS2DProcessingMethod()
{
  return _defaultPMS2DProcessingMethod;
}

float
Config::DefaultPMS2DAreaRatioReject()
{
  return _defaultPMS2DAreaRatioReject;
}

void
Config::SetFlightDate(const std::string s)
{
  _flightDate = s;

  // In 2013 we finally migrated to nidas cal files.
  if (s.substr(6).compare("2013") >= 0)
    SetCalibrationsToBeAppliedBy(NIDAS);
}



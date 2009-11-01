/*
-------------------------------------------------------------------------
OBJECT NAME:	config.cc

COPYRIGHT:	University Corporation for Atmospheric Research, 2005
-------------------------------------------------------------------------
*/

#include "config.h"

const Config::interpolationType Config::_defaultInterp = Config::Linear;
const Config::pms2dProcessing Config::_defaultPMS2DProcessingMethod = Center_In;
const float Config::_defaultPMS2DAreaRatioReject = 0.5;

/* -------------------------------------------------------------------- */
Config::Config() : _adsFileExtension(".ads")
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
  SetGroundFeedDataRate(5);	// 5 second default
  SetAsyncFileEnabled(false);
  SetLoadProductionSetup(true);
  SetHoneyWellCleanup(true);
  SetInertialShift(true);

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

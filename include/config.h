/*
-------------------------------------------------------------------------
OBJECT NAME:	config.h

DESCRIPTION:	Header File declaring Variable and associated processing
		functions.

NOTE:		ProcessingRate vs. HRT_Rate, there used to only be 1 HRT,
		25Hz.  I couldn't easily roll the new 50Hz into
		ProcessingRate, hence the new member HRTRate().
-------------------------------------------------------------------------
*/

#ifndef _congig_h_
#define _congig_h_

#include <string>

class Config
{
public:
  enum aircraft
  {
	ELECTRA=308, KINGAIR=312, SABRELINER=307, C130=130, SAILPLANE=9929,
	NOAA_G4=49, HIAPER=677, NRL_P3=303, B57=357, TECHS=300, TADS=600
  };

  enum ADSVersion	{ ADS_2, ADS_3 };
  enum processingMode	{ PostProcessing, RealTime };
  enum interpolationType { Linear=0, CubicSpline, AkimaSpline };
  enum processingRate	{ SampleRate=0, LowRate=1, HighRate=25 };
  enum hrtRate		{ TwentyFive=25, Fifty=50, OneHundred=100 };
  enum groundFeedType	{ LDM, UDP };
  enum calibrationMode	{ NIDAS, NIMBUS };

  enum pms2dProcessing	{ Entire_In=0, Center_In, Reconstruction };

  Config();

  bool Interactive() const		{ return _interactive; }
  bool Despiking() const		{ return _despiking; }
  bool TimeShifting() const		{ return _timeShifting; }
  bool BlankoutVariables() const	{ return _blankout; }
  bool QCenabled() const		{ return _qcEnabled; }
  bool ProductionRun() const		{ return _productionRun; }
  bool WarnTimeLags() const		{ return _warnTimeLags; }
  bool OutputNetCDF() const		{ return _outputNetCDF; }
  bool OutputSQL() const		{ return _outputSQL; }
  bool TransmitToGround() const		{ return _transmitToGround; }
  bool EnableBroadcast() const		{ return _enableBroadcast; }
  int GroundFeedDataRate() const	{ return _transmitRate; }
  bool AsyncFileEnabled() const		{ return _asyncFileEnabled; }
  bool LoadProductionSetup() const	{ return _loadProductionSetup; }
  bool HoneyWellCleanup() const		{ return _honeywellCleanup; }
  bool InertialShift() const		{ return _inertialShift; }

  bool CreateKMLFile() const		{ return _googleEarth; }
  bool CreateNavFile() const		{ return _iwgadts; }

  bool DespikeReporting() const		{ return _despikeReporting; }
  bool LagReporting() const		{ return _lagReporting; }

  bool isADS2() const			{ return _adsVersion == ADS_2; }
  bool isADS3() const			{ return _adsVersion == ADS_3; }

  aircraft Aircraft()			{ return _aircraft; }
  std::string AircraftString();
  processingMode ProcessingMode() const	{ return _mode; }
  processingRate ProcessingRate() const	{ return _processingRate; }
  hrtRate HRTRate() const		{ return _hrtRate; }
  interpolationType InterpolationType() const { return _interpType; }
  groundFeedType GroundFeedType()	const { return _groundFeedType; }
  calibrationMode CalibrationsAppliedBy() const { return _calibrations; }

  pms2dProcessing TwoDProcessingMethod() const { return _pms2dProcessing; }
  float TwoDAreaRejectRatio() const	{ return _twoDrejectRatio; }

  size_t AnalyzeInterval() const        { return _analyzeInterval; }
  size_t VacuumInterval() const         { return _vacuumInterval; }

  void SetInteractive(bool state)	{ _interactive = state; }
  void SetProductionRun(bool state)	{ _productionRun = state; }
  void SetWarnTimeLags(bool state)	{ _warnTimeLags = state; }
  void SetDespiking(bool state)		{ _despiking = state; }
  void SetTimeShifting(bool state)	{ _timeShifting = state; }
  void SetBlankoutVariables(bool state)	{ _blankout = state; }
  void SetQCenabled(bool state)		{ _qcEnabled = state; }
  void SetOutputNetCDF(bool state)	{ _outputNetCDF = state; }
  void SetOutputSQL(bool state)		{ _outputSQL = state; }
  void SetTransmitToGround(bool state)	{ _transmitToGround = state; }
  void SetEnableBroadcast(bool state)	{ _enableBroadcast = state; }
  void SetGroundFeedDataRate(int rate)	{ _transmitRate = rate; }
  void SetAsyncFileEnabled(bool state)	{ _asyncFileEnabled = state; }
  void SetLoadProductionSetup(bool state) { _loadProductionSetup = state; }
  void SetHoneyWellCleanup(bool state)	{ _honeywellCleanup = state; }
  void SetInertialShift(bool state)	{ _inertialShift = state; }

  void SetCreateKMLFile(bool state)	{ _googleEarth = state; }
  void SetCreateNavFile(bool state)	{ _iwgadts = state; }

  void SetAircraft(aircraft ac)		{ _aircraft = ac; }
  void SetAircraft(int ac)		{ _aircraft = (aircraft)ac; }
  void SetProcessingMode(processingMode state)		{ _mode = state; }
  void SetProcessingRate(processingRate pr)		{ _processingRate = pr; }
  void SetHRTRate(hrtRate rate)				{ _hrtRate = rate; }
  void SetInterpolationType(interpolationType it)	{ _interpType = it; }
  void SetGroundFeedType(groundFeedType type)		{ _groundFeedType = type; }
  void SetCalibrationsToBeAppliedBy(calibrationMode c)	{ _calibrations = c; }

  void SetDespikeReporting(bool state)	{ _despikeReporting = state; }
  void SetLagErrorReporting(bool state)	{ _lagReporting = state; }

  void SetADSVersion(ADSVersion nv) { _adsVersion = nv; }

  void SetAnalyzeInterval(size_t ai)              { _analyzeInterval = ai; }
  void SetVacuumInterval(size_t vi)               { _vacuumInterval = vi; }

  const std::string& ProjectDirectory() const	{ return _projectDirectory; }
  const std::string& ProjectName() const	{ return _projectName; }
  const std::string& ProjectNumber() const	{ return _projectNumber; }
  const std::string& TailNumber() const		{ return _tailNumber; }
  const std::string& NIDASrevision() const	{ return _nidasRevision; }
  const std::string& ProjectDirectoryRevision() const	{ return _projDirRevision; }
  const std::string& FlightNumber() const	{ return _flightNumber; }
  const std::string& FlightDate() const		{ return _flightDate; }
  const std::string& ADSfileExtension() const	{ return _adsFileExtension; }
  const std::string& Checksum() const		{ return _checksum; }

  void SetProjectDirectory(const std::string s)	{ _projectDirectory = s; }
  void SetProjectName(const std::string s)	{ _projectName = s; }
  void SetProjectNumber(const std::string s)	{ _projectNumber = s; }
  void SetProjectDirectoryRevision(const std::string s)	{ _projDirRevision = s; }
  void SetTailNumber(const std::string s)	{ _tailNumber = s; }
  void SetFlightNumber(const std::string s)	{ _flightNumber = s; }
  void SetFlightDate(const std::string s);
  void SetNIDASrevision(const std::string s)	{ _nidasRevision = s; }
  void SetChecksum(const std::string s)		{ _checksum = s; }

  std::string CoordinateLatitude() const        { return _coordLAT; }
  std::string CoordinateLongitude() const       { return _coordLON; }
  std::string CoordinateAltitude() const        { return _coordALT; }
  std::string CoordinateTime() const            { return _coordTime; }

  std::string WindFieldVariables() const
  { return _windWS + " " + _windWD + " " + _windWI; }

  void SetCoordLAT(const std::string s)		{ _coordLAT = s; }
  void SetCoordLON(const std::string s)		{ _coordLON = s; }
  void SetCoordALT(const std::string s)		{ _coordALT = s; }
  void SetCoordTime(const std::string s)	{ _coordTime = s; }
  void SetWindSpeed(const std::string s)	{ _windWS = s; }
  void SetWindDirection(const std::string s)	{ _windWD = s; }
  void SetWindVertical(const std::string s)	{ _windWI = s; }
  void SetTwoDProcessingMethod(pms2dProcessing p) { _pms2dProcessing = p; }
  void SetTwoDAreaRejectRatio(float f)		{ _twoDrejectRatio = f; }

  interpolationType DefaultInterpolationType();
  pms2dProcessing DefaultPMS2DProcessingMethod();
  float DefaultPMS2DAreaRatioReject();

  void SetSorterLength(int seconds) { _sorterLength = seconds; }
  int GetSorterLength() { return _sorterLength; }


private:
  bool _interactive;
  bool _productionRun;
  bool _warnTimeLags;
  bool _despiking;
  bool _timeShifting;
  bool _blankout;
  bool _qcEnabled;
  bool _outputNetCDF;
  bool _outputSQL;
  bool _transmitToGround;
  bool _enableBroadcast;
  bool _asyncFileEnabled;
  bool _loadProductionSetup;
  bool _honeywellCleanup;
  bool _inertialShift;

  bool _googleEarth;
  bool _iwgadts;

  bool _despikeReporting;
  bool _lagReporting;

  int _transmitRate;

  size_t _analyzeInterval;
  size_t _vacuumInterval;

  aircraft _aircraft;
  ADSVersion _adsVersion;
  processingMode _mode;
  processingRate _processingRate;
  hrtRate _hrtRate;
  interpolationType _interpType;
  groundFeedType _groundFeedType;
  calibrationMode _calibrations;	// cals to be applied by nimbus or nidas.

  std::string _projectDirectory;

  // Some meta-data.
  std::string _projectName;
  std::string _projectNumber;
  std::string _tailNumber;
  std::string _flightNumber;
  std::string _flightDate;
  std::string _nidasRevision;
  std::string _projDirRevision;
  std::string _adsFileExtension;
  std::string _checksum;

  std::string _coordLAT;
  std::string _coordLON;
  std::string _coordALT;
  std::string _coordTime;
  std::string _windWS;	// Wind Speed variable
  std::string _windWD;	// Wind Direction variable
  std::string _windWI;	// Vertical Wind variable


  pms2dProcessing _pms2dProcessing;

  /* % of pixels which must be shaded inside particle bounding box.
   */
  float _twoDrejectRatio;

  // Length in seconds of the nidas processed sample sorter.
  int _sorterLength;

  static const interpolationType _defaultInterp;
  static const pms2dProcessing _defaultPMS2DProcessingMethod;
  static const float _defaultPMS2DAreaRatioReject;
};

#endif

// END CONFIG.H

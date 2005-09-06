/*
-------------------------------------------------------------------------
OBJECT NAME:	config.h

DESCRIPTION:	Header File declaring Variable and associated processing
		functions.
-------------------------------------------------------------------------
*/

#ifndef _congig_h_
#define _congig_h_

class Config
{
public:
  enum ADSVersion	{ ADS_2, ADS_3 };
  enum ProcessingMode	{ PostProcessing, RealTime };
  enum interpolationType { Linear=0, CubicSpline, AkimaSpline };
  enum processingRate	{ SampleRate=0, LowRate=1, HighRate=25 };

  Config();

  bool Interactive() const		{ return _interactive; }
  bool Despiking() const		{ return _despiking; }
  bool TimeShifting() const		{ return _timeShifting; }
  bool QCenabled() const		{ return _qcEnabled; }
  bool ProcessingMode() const		{ return _mode; }
  bool ProductionRun() const		{ return _productionRun; }
  bool OutputNetCDF() const		{ return _outputNetCDF; }
  bool OutputSQL() const		{ return _outputSQL; }
  bool TransmitToGround() const		{ return _transmitToGround; }
  bool AsyncFileEnabled() const		{ return _asyncFileEnabled; }
  bool LoadProductionSetup() const	{ return _loadProductionSetup; }
  bool HoneyWellCleanup() const		{ return _honeywellCleanup; }
  bool InertialShift() const		{ return _inertialShift; }

  bool isADS2() const			{ return _adsVersion == ADS_2; }
  bool isADS3() const			{ return _adsVersion == ADS_3; }

  processingRate ProcessingRate() const	{ return _processingRate; }
  interpolationType InterpolationType() const	{ return _interpType; }

  void SetInteractive(bool state)	{ _interactive = state; }
  void SetProductionRun(bool state)	{ _productionRun = state; }
  void SetDespiking(bool state)		{ _despiking = state; }
  void SetTimeShifting(bool state)	{ _timeShifting = state; }
  void SetQCenabled(bool state)		{ _qcEnabled = state; }
  void SetProcessingMode(bool state)	{ _mode = state; }
  void SetOutputNetCDF(bool state)	{ _outputNetCDF = state; }
  void SetOutputSQL(bool state)		{ _outputSQL = state; }
  void SetTransmitToGround(bool state)	{ _transmitToGround = state; }
  void SetAsyncFileEnabled(bool state)	{ _asyncFileEnabled = state; }
  void SetLoadProductionSetup(bool state) { _loadProductionSetup = state; }
  void SetHoneyWellCleanup(bool state)	{ _honeywellCleanup = state; }
  void SetInertialShift(bool state)	{ _inertialShift = state; }

  void SetProcessingRate(processingRate pr) { _processingRate = pr; }
  void SetInterpolationType(interpolationType it) { _interpType = it; }

  void SetADSVersion(ADSVersion nv) { _adsVersion = nv; }


private:
  bool _interactive;
  bool _productionRun;
  bool _despiking;
  bool _timeShifting;
  bool _qcEnabled;
  bool _mode;
  bool _outputNetCDF;
  bool _outputSQL;
  bool _transmitToGround;
  bool _asyncFileEnabled;
  bool _loadProductionSetup;
  bool _honeywellCleanup;
  bool _inertialShift;

  ADSVersion _adsVersion;
  processingRate _processingRate;
  interpolationType _interpType;
};

#endif

// END CONFIG.H

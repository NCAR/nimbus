#include "calibrator.h"

using namespace std;

Calibrator::Calibrator(std::vector<float> cnd0, std::vector<float> gof, std::vector<float> gec1, std::vector<float> gec2) 
{
  _CND0 = cnd0;
  _GOF = gof;
  _GEC1 = gec1;
  _GEC2 = gec2;

  for (int i=0; i<NUM_CHANNELS; i++) _Gain.push_back(0.0);

  float _AA = -244.3364635;
  float _Bb = 0.462418;
  float _cC = 0.0000588;
  float _DD = -0.000000013;
  float _Wtg = 0.1;

  return;
}

std::vector<float> Calibrator::Calibrate(std::vector<int> ScanCounts, std::vector<int> PltWireCnts)
{
  std::vector<float> Gnd;
  std::vector<float> Geqn;
  for (int i=0; i<NUM_CHANNELS; i++) 
  {
    Gnd.push_back(0.0);
    Geqn.push_back(0.0);
  }

  int TargIndx = 10;   	// Index into ScanCounts for the target w/ND on
  int TargNDIndx = 11;  // Index into ScanCounts for the target w/ND off
  for (int i=0; i<NUM_CHANNELS; i++)
  {
    Gnd[i] = ScanCounts[TargIndx]-ScanCounts[TargNDIndx]/_CND0[i];
    TargIndx = TargIndx+12;
    TargNDIndx = TargNDIndx+12;
  }

  // Temperature of the mixer
  float TMix;
  int   TMC = PltWireCnts[4]; // Counts for Tmix

  TMix = _AA + _Bb*TMC + _cC*TMC*TMC + _DD*TMC*TMC*TMC;

  // Initialize the gain
  for (int i=0; i<NUM_CHANNELS; i++)
  {
    if (_Gain[i] == 0.0) {
      _Gain[i] = TMix*_GEC2[i];
    } else {
      _Gain[i] = (_Gain[i] + Gnd[i] * _Wtg)/ (1+_Wtg);
    }
  }
}


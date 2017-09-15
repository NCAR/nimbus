#include "calibrator.h"
#include <iostream>

using namespace std;

Calibrator::Calibrator(std::vector<float> cnd0, std::vector<float> gof, std::vector<float> gec1, std::vector<float> gec2) 
{
  _CND0 = cnd0;
  _GOF = gof;
  _GEC1 = gec1;
  _GEC2 = gec2;

  // Create a gain vector with all elements set to zero.
  for (int i=0; i<NUM_CHANNELS; i++) _Gain.push_back(0.0);

  // Set constants for platinum wire gain equation for temperature of target. 
  // These should remain constant as long as physical target doesn't change.
  // If target is replaced, these may change.
  _AA = -244.3364635;
  _Bb = 0.462418;
  _cC = 0.0000588;
  _DD = -0.000000013;
  _Wtg = 0.1;

  // the angle associated with the horizontal scan
  _LocHor = 5; 

  return;
}

std::vector<float> Calibrator::Calibrate(std::vector<int> ScanCounts, std::vector<int> PltWireCnts, std::vector<int> TargetCnts, float OAT)
{
  // The equations in this routine come from MTPGainEquation.docx (in svn)
  std::vector<float> ScanBTs;

  std::vector<float> Gnd;
  // Create a Gnd vector with all elements set to zero.
  for (int i=0; i<NUM_CHANNELS; i++) Gnd.push_back(0.0);

  int TargIndx = 0;   	// Index into ScanCounts for the target w/ND on
  int TargNDIndx = 3;  // Index into ScanCounts for the target w/ND off
  for (int i=0; i<NUM_CHANNELS; i++)
  {
    Gnd[i] = (TargetCnts[TargIndx]-TargetCnts[TargNDIndx])/_CND0[i];
    TargIndx = TargIndx+1;
    TargNDIndx = TargNDIndx+1;
  }

  // Counts for TMix
  float r = 350.0+(250.0*(PltWireCnts[4]-PltWireCnts[0])/(PltWireCnts[7]-PltWireCnts[0])); 
  
  // Temperature of the mixer (TMix, also known as Tifa)
  float TMix = _AA +_Bb*r + _cC*r*r + _DD*r*r*r;

  // g() is initialized to a value at the beginning of a flight and then is adjusted on
  // a scan by scan basis in an iterative fashion.  
  for (int i=0; i<NUM_CHANNELS; i++)
  {
    if (_Gain[i] == 0.0) {
      // Initialize the gain
      _Gain[i] = (TMix-_GOF[0])*_GEC2[i] + _GEC1[i];
    } else {
      _Gain[i] = (_Gain[i] + Gnd[i] * _Wtg)/ (1+_Wtg);
    }
    cout << "Gain["<<i<<"]="<<_Gain[i]<<"\n";
  }

  cout << "Calculate Tb...\n";
  // Calculate Brightness Temperatures
  for (int i=0; i<NUM_CHANNELS;i++)
  {
    for (int j=0;j<NUM_SCAN_ANGLES;j++)
    {
        ScanBTs.push_back(OAT + (ScanCounts[i*10+j] - ScanCounts[i*10+_LocHor]) / _Gain[i]);
    }
  }

  return ScanBTs;
}

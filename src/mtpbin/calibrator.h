/*! \file calibrator.h
 *  \brief This class encapsulates the calibration of the MTP scans.  
 ** 
 **
 ** The Microwave Temperature Profiler (MTP) instrument build by JPL for
 ** NCAR as part of the set of instruments known as HAIS (build specifically
 ** in support of the Haiper GV aircraft) came with processing software 
 ** written in Visual Basic 6.0. 
 **
 ** Copyright 2015-2016 University Corporation for Atmospheric Research
 **  - VB6 and Algorithm Copyright MJ Mahoney, NASA Jet Propulsion Laboratory
 **
 */
#ifndef _CALIB_h
#define _CALIB_h

#include "mtp.h"
#include <vector>
#include <string> 


/*! \class Calibrator
 *
 * This class encapsulates the calibration of the MTP scans.  It makes use of 
 * constants that are written into the .REF file for the flight in question.
 * I am unclear as to when that file gets written.
 *
 * When the MTP instrument completes a scan of the atmosphere the scan counts
 * are converted to Brightness Temperatures using this class.
 * 
 */
class Calibrator
{

public:

  /**
   * Constructor
   * @param cnd0 std::vector<float> is the 'Cnd0' values from the REF file.  
   * I believe that these are the "temperatures" of the target in each channel
   * in degrees C when the noise diode is on.  Comments in the VB code call
   * it the Noise Diode Temperature Fit Offset value.
   * @param gof std::vector<float> is the 'GOF' values from the REF file.
   * I believe that these are an offset for the temperature of the mixer, one
   * for each channel.  Comments in the code say they are Gain Equation Offsets
   * but their use belies that.
   * @param gec1 std::vector<float> is the 'GEC(chan,1)' vector from the REF file
   * called Gain Equation Coefficients, they do seem to be used in that way
   * There is one value per channel, this one is the offset coefficient.
   * @param gec2 std::vector<float> is the 'GEC(chan,2)' vector from the REF file
   * called Gain Equation Coefficients, this one is the linear coefficient
   * applied to the difference of the Mixer temperature minus the GOF above.
   */
  Calibrator(std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>);

  /**
   * Calibrate method
   * @param ScanCounts std::vector<int> is the vector of counts produced by the MTP
   * instrument for this scan. This vector should be of length 36 - three
   * points for each angle (one per channel) and six points for the target -
   * three per channel for the noise diode turned on and for it turned off.
   * @param PltWireCnts is the platinum wire counts.
   * @param TargetCnts is the counts from the target. First 3 are with ND on, next 3
   * off.
   * @param OAT is outside air temperature from the A line of the RAW MTP file (Kelvin).
   *
   * @returns a std::vector<float> of Brightness Temperatures for the scan. 
   * There are 30 elements to the vector, 3 values at each of the 10 angles
   * of the MTP's scan.
   */
  std::vector<float> Calibrate(std::vector<int>, std::vector<int>, std::vector<int>,float);
  

private: 

  ///< For storing the rolling gain value - one value per channel
  std::vector<float> _Gain;

  ///< Constants for platinum wire gain equation for temperature of target
  float _AA;
  float _Bb;
  float _cC;
  float _DD;
  float _Wtg;

  ///< The scan angle associated with the horizontal scan.
  int _LocHor;

  ///< Elements that are pulled from the flight's REF file.  
  ///< e.g. "c:\MTP\Data\NGV\HIPPO-5\20110822\NG20110822.REF"
  std::vector<float> _CND0;
  std::vector<float> _GOF;
  std::vector<float> _GEC1;
  std::vector<float> _GEC2;
  
};

#endif

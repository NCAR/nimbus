/*
 *************************************************************************** 
 ** rcf_set.h
 ** 
 ** The Microwave Temperature Profiler (MTP) instrument build by JPL for
 ** NCAR as part of the set of instruments known as HAIS (build specifically
 ** in support of the Haiper GV aircraft) came with processing software 
 ** written in Visual Basic 6.0. 
 **
 ** A set of Retrieval Coefficient Files (RCFs) are currently written 
 ** by the VB program RCCalc which takes RAOB data and converts it into 
 ** Templates that describe what that RAOB profile would look like to the MTP 
 ** instrument if the instrument was used to measure the atmosphere described 
 ** by the profile. 
 **
 ** When the MTP instrument completes a scan of the atmosphere the scan counts
 ** are converted to Brightness Temperatures, using these the "best match" 
 ** RCF is found and it's corresponding Retrieval Coefficients are used
 ** to determine the atmospheric temperature profile.
 ** 
 ** This class encapsulates the whole set of RCFs for a project and provides
 ** a method for determining which of the RCFs is the best match of a given
 ** set of brightness temperatures (scan) taken at a particular elevation.
 **
 ** Copyright 2015-2016 University Corporation for Atmospheric Research
 **  - VB6 and Algorithm Copyright MJ Mahoney, NASA Jet Propulsion Laboratory
 **
*/
#ifndef _RCF_SET_h
#define _RCF_SET_h

#include <vector>
#include <string> 
#include "rcf.h"

class RetrievalCoefficientFileSet  
{

public:

  std::vector<RetrievalCoefficientFile> getRCFVector() {return _RCFs;}

  /**
   * Constructor
   * @param std::string is directory name containing the RCF files to be 
   * pulled into the set.
   */
  RetrievalCoefficientFileSet(const std::string);

  /**
   * Set the Flight Levels in KM.  
   * @param[in] is an array of floating point values indicating the km
   * above sea level of each flight level in each retrieval coefficient file.
   * @param[in] is the size fo the vector
   * Flight levels should be ordered in decreasing altitude.
   * @returns true if levels successfully set, false if not.
   * Assumes that all RCFs in the set have the same flight levels - 
   * historically that has been the case, but it may not have to be.
   */
  bool setFlightLevelsKm(float[], int);

  /**
   * Get the Retrieval Coefficient Set from the "best" template
   * weighted according to the flight level of the aircraft and given
   * a set of Scan Brightness Temperatures
   * @param[in] is an array of Scan Brightness Temperatures and is expected
   *  to be NUM_OBSVBLSs in length.
   * @param[in] is the altitude of the aircraft in km at the time that the 
   *  Scan was taken.
   * @pram [in] is an Brightness Temperature Bias provided by user
   * @return is the weighted average set of Model Brightness Temperatures, 
   *  RMS values, and Retrieval Coefficients from the template that best 
   *  matches the input Scan Brightness Temperatures as observed from 
   *  the input flight altitude. 
   */
  RC_Set_1FL getBestWeightedRCSet(float*, float, float);

private: 

  std::string _RCFDir;
  std::vector<RetrievalCoefficientFile> _RCFs;
  
};

#endif

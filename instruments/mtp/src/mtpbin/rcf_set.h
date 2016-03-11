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
  /**
   * Constructor
   * @param std::string is directory name containing the RCF files to be 
   * pulled into the set.
   */
  RetrievalCoefficientFileSet(const std::string);

  //std::string getId() {return _RCFId;}
  //std::string getId() ;

  /**
   * Get the weighted average Retrieval Coefficient Set
   * @param PAltKm pressure altitude in KM at which to weight the elements 
   * of the set: the observables and rmms vectors as well as the retrieval 
   * coefficient matrices from flight levels above and below PAltKm are 
   * averaged with a weight factor based on nearness of PAltkm to the 
   * pressure altitude of the flight level as described in the RCF header.
   */
  //RC_Set_1FL getRCAvgWt(float);

  /**
   * Set the Flight Levels in KM.  
   * @param is a vector of floating point values indicating the km
   * above sea level of each flight level in the retrieval coefficient file.
   * Flight levels should be ordered in decreasing altitude.
   * @returns true if levels successfully set, false if not.
   */
  //bool setFlightLevelsKm(float[], int);

private: 

  std::string _RCFDir;
  std::vector<RetrievalCoefficientFile> _RCFs;
  //std::vector<RC_Set_1FL> _FlRcSetVec;
  
};

#endif

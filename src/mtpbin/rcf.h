/*
 *************************************************************************** 
 ** rcf.h
 ** 
 ** The Microwave Temperature Profiler (MTP) instrument build by JPL for
 ** NCAR as part of the set of instruments known as HAIS (build specifically
 ** in support of the Haiper GV aircraft) came with processing software 
 ** written in Visual Basic 6.0. 
 **
 ** Copyright 2015 University Corporation for Atmospheric Research
 **  - VB6 and Algorithm Copyright MJ Mahoney, NASA Jet Propulsion Laborator
 **
 ** Retrieval Coeeficient Files (RCFs) are currently written by the VB program
 ** RCCalc which takes RAOB data and converts it into Templates that describe
 ** what that RAOB profile would look like to the MTP instrument if the 
 ** instrument was used to measure the atmosphere described by the profile. 
 ** Since the profile would appear differerntly to the MTP depending upon 
 ** the altitude at which the instrument was performing the measurement, the
 ** RCF files have multiple flight levels containing expected brightness 
 ** temperatures (observables), assocciated rms values and the retrieval
 ** coefficients that would allow one to convert from the brightness 
 ** temperatures to the profile temperatures.
 ** 
 ** This class provides for reading in an RCF, storing and providing access to
 ** its data.
 **
*/
#ifndef _RCF_h
#define _RCF_h

#include <vector>
#include <string> 
#include "rcf_structs.h"

class RetrievalCoefficientFile 
{

public:
  RetrievalCoefficientFile(std::string);

  //std::string getId() {return _RCFId;}
  std::string getId() ;

  /**
   * Get the weighted average Retrieval Coefficient Set
   * @param PAlt pressure altitude at which to weight the elements of the set.
   * The observables and rmms vectors as well as the retrieval coefficient
   * matrices from flight levels above and below PAlt are averaged with a 
   * weight factor based on nearness of PAlt to the pressure altitude of 
   * the flight level.
   */
  RC_Set_1FL getRCAvgWt(float);


private: 

  // Convert the elements of the Union used to read a flight level 
  // Retrieval Coefficient set into a more usable form.
  void FlUn2FlRcSet(My_RC_FL_Un, RC_Set_1FL*);

  std::string _RCFFileName;
  std::string _RCFId;

  std::vector<My_RC_FL_Un> flightLevelRCInfoVec;
  std::vector<RC_Set_1FL> FlRcSetVec;
  
};

#endif

/*! \file rcf.h
 *  \brief This class provides for reading in an RCF, storing and providing
 *  access to its data.
 ** 
 ** The Microwave Temperature Profiler (MTP) instrument build by JPL for
 ** NCAR as part of the set of instruments known as HAIS (build specifically
 ** in support of the Haiper GV aircraft) came with processing software 
 ** written in Visual Basic 6.0. 
 **
 ** Copyright 2015 University Corporation for Atmospheric Research
 **  - VB6 and Algorithm Copyright MJ Mahoney, NASA Jet Propulsion Laborator
 **
*/
#ifndef _RCF_h
#define _RCF_h

#include <vector>
#include <string> 
#include "mtp_rcf_structs.h"

/*! \class RetrievalCoefficientFile
 *
 * This class provides for reading in an RCF, storing and providing access to
 * its data.
 *
 * Retrieval Coefficient Files (RCFs) are currently written by the VB program
 * RCCalc which takes RAOB data and converts it into Templates that describe
 * what that RAOB profile would look like to the MTP instrument if the 
 * instrument was used to measure the atmosphere described by the profile. 
 *
 * Since the profile would appear differently to the MTP depending upon 
 * the altitude at which the instrument was performing the measurement, the
 * RCF files have multiple flight levels containing expected brightness 
 * temperatures (observables), assocciated rms values and the retrieval
 * coefficients that would allow one to convert from the brightness 
 * temperatures to the profile temperatures.
 * 
 */
class RetrievalCoefficientFile 
{

public:
  RetrievalCoefficientFile(std::string);
  RetrievalCoefficientFile();

  std::string getId()        {return _RCFId;}
  std::string getFileName()  {return _RCFFileName;}
  RCF_HDR     getRCF_HDR()   {return _RCFHdr;}
  ///< When using getRCF_HDR, be advised that char arrays have no endstring!
  std::vector<RC_Set_1FL> 
              getFL_RC_Vec() {return _FlRcSetVec;}
  bool isValid();

  /**
   * Get the weighted average Retrieval Coefficient Set
   * @param PAltKm pressure altitude in KM at which to weight the elements 
   * of the set: the observables and rmms vectors as well as the retrieval 
   * coefficient matrices from flight levels above and below PAltKm are 
   * averaged with a weight factor based on nearness of PAltkm to the 
   * pressure altitude of the flight level as described in the RCF header.
   */
  RC_Set_1FL getRCAvgWt(float);

  /**
   * Set the Flight Levels in KM.  
   * @param FlightLevels is a vector of floating point values indicating the km
   * above sea level of each flight level in the retrieval coefficient file.
   * Flight levels should be ordered in decreasing altitude.
   * @param Len is the length of the FlightLevels vector
   * @returns true if levels successfully set, false if not.
   */
  bool setFlightLevelsKm(float[], int);

private: 

  /**
   * Convert the elements of the Union used to read a flight level 
   * Retrieval Coefficient set into a more usable form.
   */
  void FlUn2FlRcSet(My_RC_FL_Un, RC_Set_1FL*);

  std::string _RCFFileName;
  std::string _RCFId;
  RCF_HDR     _RCFHdr;

  std::vector<RC_Set_1FL> _FlRcSetVec;

  std::vector<My_RC_FL_Un> flightLevelRCInfoVec;
  
};

#endif

/*
 ******************************************************************************
 ** retriever.h
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
 ** This class encapsulates the retrieval method which perdforms the inverse
 ** calculation of the radiative transfer model to take a set of brightness 
 ** temperatures and generate the physical temperature profile.  It does so by
 ** using the RetrievalCoefficientFileSet class to obtain the weighted averaged
 ** retrieval coefficients from the RCF that best matches our scan at our
 ** flight altitude.
 **
 ** Copyright 2015-2016 University Corporation for Atmospheric Research
 **  - VB6 and Algorithm Copyright MJ Mahoney, NASA Jet Propulsion Laboratory
 **
*/
#ifndef _RETRIEVER_h
#define _RETRIEVER_h

#include <stdio.h>
#include <string>
#include <vector>
#include "rcf_set.h"

class Retriever
{

public:

  /**
   * Construcctor
   * @param std::string is directory name containing the RCF files to be
   * used.
   */
  Retriever(RetrievalCoefficientFileSet rcfs): _Rcf_Set(rcfs) { };

  /**
   * Retrieve function.  Obtain the Physical temperature profile from a scan.
   *
   * @param[in] is an array fo floating point values indicating the brightness
   * temperature values in Degrees Kelvin from the scan.  The first 10 values 
   * are for channel 1 at each of the scan angles (highest angle to lowest 
   * angle), the second 10 are for channel 2 and the third 10 are for channel 3.
   *
   * @param[in] is the floating point altitude of the aircraft in km
   *
   */
  std::vector<float> Retrieve(float[], float) ;

private:
  RetrievalCoefficientFileSet _Rcf_Set;

};






#endif

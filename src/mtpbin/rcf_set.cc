#include <stdio.h>
#include <string.h> 
#include <iostream>
#include <fstream>
#include <arpa/inet.h>
#include <boost/filesystem.hpp>
#include <math.h>
#include "rcf_set.h"
#include "rcf.h"

using namespace std;

RetrievalCoefficientFileSet::RetrievalCoefficientFileSet(const string Directory) 
{

  _RCFDir = Directory;
  boost::filesystem::path RCFPath(_RCFDir);

  boost::filesystem::directory_iterator end_itr;

  for (boost::filesystem::directory_iterator itr(RCFPath); itr != end_itr; 
       ++itr)
  {
    // Ignore directories and files that don't have .RCF extension
    int i = 0;
    if (boost::filesystem::is_regular_file(itr->path()) && 
        itr->path().extension() == ".RCF") {
      string rcf_path = itr->path().string();
      _RCFs.push_back(RetrievalCoefficientFile(rcf_path));
      //cout << "Found RCF file:"<<rcf_path<<"  with ID:"
       //    <<_RCFs[i].getId()<<"\n";
      i++;
    }
  }

  return;
}

bool RetrievalCoefficientFileSet::setFlightLevelsKm(float FlightLevels[], 
                                                    int NumFlightLevels)
{
  if (_RCFs.size() == 0) 
  {
    cout<<"In: "<<__func__<<" call failed:\n";
    cout<<"ERROR:There are currently no RCFs in the set\n";
    return false;
  } 

  for (std::vector<RetrievalCoefficientFile>::iterator 
       it = _RCFs.begin(); it != _RCFs.end(); ++it) 
  {
    if (!it->setFlightLevelsKm(FlightLevels, NumFlightLevels))
    {
      cout<<"In: "<<__func__<<" call failed:\n";
      cout<<"ERROR: Failed to set flight levels for RCFID:"<<it->getId()<<"\n";
      return false;
    }
  }

  return true;
}

RC_Set_4Retrieval RetrievalCoefficientFileSet::getBestWeightedRCSet
                                  (float ScanBrightnessTemps[],float PAltKm,
                                   float BTBias)
{

  RC_Set_1FL AvgWtSet;
  float Weight, SumWeights;         // For RMS weighting
  float SumWeightedAvg, SumSquares; // Sum of weighted differences
  float Diff;			    // Measured BT - Model BT - BTBias
  int   NumBTsIncl;		    // Count of BTs included in Weighting
  int   BestRCIndex;                // Index of "best" template (so far)
  int   BTIndex;		    // Index into Scan and Model BT arrays
  int   thisRCFIndex=0;		    // Which RCF index are we looking at?
  float RCFBTWeightedMean;	    // The weighted mean of this RCF's BTs
  float RCFBTStdDev;		    // Standard Deviation about weighted mean(?)
  float Numerator, Denominator;	    // For RCFBTStdDev calculation
  float BestlnP;  // The sum of the ln of Probabilities for the BestRCIndex
  float thislnP;  // The sum of the ln of Probabilities for "this" RCF


  // Step through the vector of Retrieval Coefficient files (aka templates)
  // to obtain the best match for the scan at the input Altitude
  for (std::vector<RetrievalCoefficientFile>::iterator
       RCFit = _RCFs.begin(); RCFit != _RCFs.end(); ++RCFit)
  {
    SumWeightedAvg = SumSquares = SumWeights = NumBTsIncl = 0;

    // Get the weighted RC set for this template
    AvgWtSet=RCFit->getRCAvgWt(PAltKm);

    // Compare the template Brightness Temperatures with the 
    // measured brightness temperatures to find the quality of match
    for (BTIndex = 0; BTIndex < NUM_BRT_TEMPS; BTIndex++) 
    {
      Weight = 1/(AvgWtSet.MBTRms[BTIndex]*AvgWtSet.MBTRms[BTIndex]);
      SumWeights = SumWeights + Weight;
      Diff = ScanBrightnessTemps[BTIndex]-AvgWtSet.MBTAvg[BTIndex];
      if (Weight > 0)
      {
        NumBTsIncl+=1;
        SumWeightedAvg = SumWeightedAvg+Weight*Diff;
        SumSquares = SumSquares+Weight*(Diff*Diff);
      }
    }

    RCFBTWeightedMean = SumWeightedAvg/SumWeights;

    Numerator = (SumSquares-(SumWeights*(RCFBTWeightedMean*RCFBTWeightedMean)));
    Denominator = ((NumBTsIncl-1)*SumWeights/NumBTsIncl);
    if (Numerator/Denominator >= 0)
      RCFBTStdDev = sqrt(Numerator/Denominator);
    else
      RCFBTStdDev = RCFBTWeightedMean; // MJ "kludge" 

    // Calculate the Sum of the ln of probabilities (quality of match)
    thislnP = 8 * sqrt(RCFBTWeightedMean*RCFBTWeightedMean +
                       RCFBTStdDev*RCFBTStdDev) / NUM_BRT_TEMPS;

    if (RCFit == _RCFs.begin())
    {
      BestlnP = thislnP;
      BestRCIndex = 0;
    }
    else if (thislnP < BestlnP)
    {
      BestlnP = thislnP;
      BestRCIndex = thisRCFIndex;
    }
    thisRCFIndex++;
  }

  RC_Set_4Retrieval RC4R;
  RC4R.SumLnProb = BestlnP;
  RC4R.RCFFileName = _RCFs[BestRCIndex].getFileName();
  RC4R.RCFId = _RCFs[BestRCIndex].getId();
  RC4R.FL_RCs = _RCFs[BestRCIndex].getRCAvgWt(PAltKm);
  return RC4R;

}


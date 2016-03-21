#include <stdio.h>
#include <string.h> 
#include <iostream>
#include <fstream>
#include <arpa/inet.h>
#include <boost/filesystem.hpp>
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


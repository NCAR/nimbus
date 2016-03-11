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

  for (boost::filesystem::directory_iterator itr(RCFPath); itr != end_itr; ++itr)
  {
    // Ignore directories and files that don't have .RCF extension
    int i = 0;
    if (boost::filesystem::is_regular_file(itr->path()) && 
        itr->path().extension() == ".RCF") {
      string rcf_path = itr->path().string();
      _RCFs.push_back(RetrievalCoefficientFile(rcf_path));
      cout << "Found RCF file:"<<rcf_path<<"  with ID:"
           <<_RCFs[i].getId()<<"\n";
      i++;
    }
  }

  return;
}

/*
std::string RetrievalCoefficientFile::getId() 
{
  return _RCFId;
}

RC_Set_1FL RetrievalCoefficientFile::getRCAvgWt(float PAltKm)
{
  RC_Set_1FL RcSetAvWt;

  // First check to see if PAltKm is outside the range of Flight Level PAltKms
  //  - if so then the weighted average observalbe will be the average
  //    observable associated with the flight level whose PAltKm is closest 
  //    Assumption is that the Flight Level Retrieval Coefficient Set vector
  //    is stored in increasing Palt (decreasing aircraft altitude).
std::vector<int>::size_type sz = _FlRcSetVec.size();
cout<<"In get avg: PAltKm:"<<PAltKm<<"  1st level:"<<_FlRcSetVec.begin()->Palt<<"  last level:"<<_FlRcSetVec.end()->Palt<<"  -1:"<<_FlRcSetVec[sz-2].Palt<<"\n";
  if (PAltKm >= _RCFHdr.Zr[0]) 
  {
    for (int i = 0; i< NUM_OBSVBLS; i++) 
    {
      RcSetAvWt.OBAvg[i] = _FlRcSetVec.begin()->OBAvg[i];
      RcSetAvWt.OBRms[i] = _FlRcSetVec.begin()->OBRms[i];
      for (int j = 0; j < NUM_RETR_LVLS; j++) 
      {
        RcSetAvWt.RC[j][i] = _FlRcSetVec.begin()->RC[j][i];
      }
    }
    return RcSetAvWt;
  }
  if (PAltKm <= _RCFHdr.Zr[_RCFHdr.NFL-1])
  {
    for (int i = 0; i< NUM_OBSVBLS; i++) 
    {
      RcSetAvWt.OBAvg[i] = _FlRcSetVec.end()->OBAvg[i];
      RcSetAvWt.OBRms[i] = _FlRcSetVec.end()->OBRms[i];
      for (int j = 0; j < NUM_RETR_LVLS; j++) 
      {
        RcSetAvWt.RC[j][i] = _FlRcSetVec.end()->RC[j][i];
      }
    }
    return RcSetAvWt;
  }

  // Find two Flight Level Sets that are above and below the PAltKm provided.
  // Calculate the weight for averaging and identify the RC sets.
  vector<RC_Set_1FL>::const_iterator it, Botit, Topit;
  float BotWt,TopWt;
  int i = 0;
  for(it=_FlRcSetVec.begin(); it!=_FlRcSetVec.end(); it+=1)
  {
    if (PAltKm <= _RCFHdr.Zr[i] and PAltKm >= _RCFHdr.Zr[i+1])
    {
      BotWt = 1-((PAltKm-_RCFHdr.Zr[i+1])/(_RCFHdr.Zr[i]-_RCFHdr.Zr[i+1]));
      Topit = it;
      Botit = it+1;
cout<< "TopAlt:"<<_RCFHdr.Zr[i]<<"  BotAlt:"<<_RCFHdr.Zr[i+1]<<"  BotWt:"<<BotWt<<"\n";
    } else {
cout<< "i:"<<i<<"  iAlt:"<<_RCFHdr.Zr[i]<<"  i++Alt:"<<_RCFHdr.Zr[i+1]<<"  ChkAlt:"<<BotWt<<"\n";
    }
    i++;
  }
  TopWt = 1-BotWt;


  // Calculate the Weighted averages 
  for (int i = 0; i < NUM_OBSVBLS; i++)
  {
    RcSetAvWt.OBAvg[i] = Botit->OBAvg[i]*BotWt + Topit->OBAvg[i]*TopWt;
    RcSetAvWt.OBRms[i] = Botit->OBRms[i]*BotWt + Topit->OBRms[i]*TopWt;
    for (int j = 0; j < NUM_RETR_LVLS; j++)
    {
      RcSetAvWt.RC[j][i] = Botit->RC[j][i]*BotWt + Topit->RC[j][i]*(1-BotWt);
    }
  }

cout<<" RCwt:\n";
for (int j = 0; j < NUM_RETR_LVLS; j++) {
  for (int i = 0; i < NUM_OBSVBLS; i++) {
    if (i%5 != 0) cout<<"["<<j<<"]["<<i<<"]:"<<RcSetAvWt.RC[j][i];
    else cout<<"["<<j<<"]["<<i<<"]:"<<RcSetAvWt.RC[j][i]<<'\n';
  }
}
cout <<"\n\n";

  return RcSetAvWt;

}

bool RetrievalCoefficientFile::setFlightLevelsKm(float FlightLevels[], int Len)
{
  if (Len != _RCFHdr.NFL) 
  {
    cout<<"ERROR:Number of flight levels input is not equal to number in RCF\n";
    return false;
  } 
  for (int i = 0; i<Len; i++)
  {
    if ((i+1<Len) && (FlightLevels[i] <= FlightLevels[i+1]))
    {
      cout<<"ERROR: Flight Levels are not in decreasing altitude\n";
      return false;
    }
    _RCFHdr.Zr[i] = FlightLevels[i];
  }

  return true;
}

void RetrievalCoefficientFile::FlUn2FlRcSet(My_RC_FL_Un RcUn, RC_Set_1FL *RcSet)
{
  RcSet->Palt = RcUn.RC_read.sBP;
  for (int i=0; i<NUM_OBSVBLS; i++) 
  {
    RcSet->OBRms[i] = RcUn.RC_read.sOBrms[i];
    RcSet->OBAvg[i] = RcUn.RC_read.sOBav[i];
  }
  for (int i=0; i<NUM_RETR_LVLS; i++) 
  {
    RcSet->PAltRl[i] = RcUn.RC_read.sBPrl[i];
    RcSet->TAvgRl[i] = RcUn.RC_read.sRTav[i];
    RcSet->TVarRl[i] = RcUn.RC_read.sRMSa[i];
    RcSet->TRmsRl[i] = RcUn.RC_read.sRMSe[i];
  }
  // Convert Retrieval coefficients from column major storage
  int x=0,y=0;
  for (int i=0; i<990; i++) {
    RcSet->RC[x][y] = RcUn.RC_read.Src[i];
    x++;
    if (x>=NUM_RETR_LVLS) { x=0; y++; }
  }

// Output the RC matrix
for (x=0; x<NUM_RETR_LVLS; x++) {
  for (y=0; y<NUM_OBSVBLS; y++) {
    if (y%4==0) cout << " ["<<x<<"]["<<y<<"]:"<<RcSet->RC[x][y]<<"\n";
    else cout << "["<<x<<"]["<<y<<"]:"<<RcSet->RC[x][y];
  }
}
cout<<'\n';

}
*/

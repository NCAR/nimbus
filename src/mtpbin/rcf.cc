#include <stdio.h>
#include <string.h> 
#include <iostream>
#include <fstream>
#include <arpa/inet.h>
#include "rcf.h"

using namespace std;

RetrievalCoefficientFile::RetrievalCoefficientFile(string Filename) 
{

  _RCFFileName = Filename;

  My_Rcf_Hdr_Un RcfHdr;
  My_RC_FL_Un RcFlUn;
  RC_Set_1FL FlRcSet;

  string temp = _RCFFileName;
  size_t last_slash = temp.find_last_of("/");
  size_t last_dot = temp.find_last_of(".");
  _RCFId = temp.substr(last_slash+1, last_dot-last_slash-1);

  streampos size;

  ifstream file (_RCFFileName.c_str(), ios::in|ios::binary);
  if (file.is_open())
  {
    //size = file.tellg();
    size = sizeof(RCF_HDR); 
    file.seekg (0, ios::beg);
    file.read (RcfHdr.Array, size);

    if (!file)
    {
      cout << "In: RetrievalCoefficientFile Constructor:\n";
      cout << "ERROR!: Reading Header only " <<file.gcount()
           <<" bytes were read.\n";
      cout << "From RCF file:"<<_RCFFileName.c_str()<<"\n\n";
    }

    file.seekg ((-2*sizeof(float)),ios::cur);

    for (int i=0; i<RcfHdr.Rcf_Hdr.NFL; i++) {
      file.read (RcFlUn.Array, sizeof(RC_FL_Read));
      if (!file)
      {
        cout << "In: RetrievalCoefficientFile Constructor:\n";
        cout << "  ERROR!: only " <<file.gcount()<<" bytes were read.\n";
        cout << "  From Flight Level #:" << i << "\n";
        cout << "  From RCF file:"<<_RCFFileName.c_str()<<"\n\n";
      }
      FlUn2FlRcSet(RcFlUn, &FlRcSet);
      flightLevelRCInfoVec.push_back(RcFlUn);
      _FlRcSetVec.push_back(FlRcSet);
      if (i+1 == RcfHdr.Rcf_Hdr.NFL) _FlRcSetVec.push_back(FlRcSet);
    }
    //
    _FlRcSetVec.pop_back();


    file.close();

    // Fill first part of our header
    // NOTE: for some damn reason the header read screws up right
    //   after NFL ... still don't know why, but have workaround
    _RCFHdr.RCformat = RcfHdr.Rcf_Hdr.RCformat;
    strncpy(_RCFHdr.CreationDateTime,RcfHdr.Rcf_Hdr.CreationDateTime,8);
    strncpy(_RCFHdr.RAOBfilename, RcfHdr.Rcf_Hdr.RAOBfilename, 80);
    strncpy(_RCFHdr.RCfilename, RcfHdr.Rcf_Hdr.RCfilename, 80);
    _RCFHdr.RAOBcount=RcfHdr.Rcf_Hdr.RAOBcount;
    _RCFHdr.LR1=RcfHdr.Rcf_Hdr.LR1;
    _RCFHdr.zLRb=RcfHdr.Rcf_Hdr.zLRb;
    _RCFHdr.LR2=RcfHdr.Rcf_Hdr.LR2;
    _RCFHdr.RecordStep=RcfHdr.Rcf_Hdr.RecordStep;
    _RCFHdr.RAOBmin=RcfHdr.Rcf_Hdr.RAOBmin;
    _RCFHdr.ExcessTamplitude=RcfHdr.Rcf_Hdr.ExcessTamplitude;
    _RCFHdr.Nobs=RcfHdr.Rcf_Hdr.Nobs;
    _RCFHdr.Nret=RcfHdr.Rcf_Hdr.Nret;
    for (int i=0; i<NUM_RETR_LVLS; i++)
      _RCFHdr.dZ[i]=RcfHdr.Rcf_Hdr.dZ[i];
    _RCFHdr.NFL=RcfHdr.Rcf_Hdr.NFL;
    
    // Resynch header decoding at SURC
    My_EH EH;
    int i = 0;
    for (int j=1376; j<sizeof(END_HDR); j++,i++) EH.Array[i]=RcfHdr.Array[j];
  
    // Finish filling our header record with what we can get.
    strncpy(_RCFHdr.SURC, EH.EH.SURC, 4);
    _RCFHdr.RAOBbias=EH.EH.RAOBbias;
    _RCFHdr.CH1LSBloss=EH.EH.CH1LSBloss;
    //Convert from column major storage for SmatrixN1
    int x=0,y=0,z=0;
    for (int i=0; i<450;i++) {
      _RCFHdr.SmatrixN1[x][y][z] = EH.EH.Gmatrix[i];
     //cout << x << " " << y << " " << z << '\n';
      x++;
      if (x > 14) { 
        x=0;
        y++; 
        if (y>2) {
          y=0;
          z++;
        }
      }
    }

/* - maybe at some point these debug statements will help resolve 
 *   The decoding problems of the RCF header.
    for (int i=0; i<10;i++) {
      cout << "SMATRIXN1:"<<_RCFHdr.SmatrixN1[14][2][i]<<'\n';
    }
    cout << "SmatrixN1[5][1][5]:"<<_RCFHdr.SmatrixN1[5][1][5]<<'\n';
    for (int i = 450; i<460;i++) {cout<<" GM["<<i<<"]:"<<EH.EH.Gmatrix[i];}
    cout << "\n";
*/

    //Convert from column major storage for SmatrixN2
    // NOTE: about 1/2 way through this, we loose data and it all turns to
    // zeros... why?  Beats the heck out of me!  Gratefully I found where
    // the flight level data starts and resynch to it above.
    x=0,y=0,z=0;
    for (int i=450; i<900;i++) {
      _RCFHdr.SmatrixN2[x][y][z] = EH.EH.Gmatrix[i];
      x++;
      if (x > 14) { 
        x=0;
        y++; 
        if (y>2) {
          y=0;
          z++;
        }
      }
    }

/* - maybe at some point these debug statements will help resolve 
 *   The decoding problems of the RCF header.
    for (int i=0; i<10;i++) {
      cout << "SMATRIXN2:"<<_RCFHdr.SmatrixN2[0][0][i]<<'\n';
    }
    cout << "SmatrixN2[5][1][5]:"<<_RCFHdr.SmatrixN2[5][1][5]<<'\n';
*/

  }
  else cout << "Unable to open file";
  return;
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
    for (int i = 0; i< NUM_BRT_TEMPS; i++) 
    {
      RcSetAvWt.MBTAvg[i] = _FlRcSetVec.begin()->MBTAvg[i];
      RcSetAvWt.MBTRms[i] = _FlRcSetVec.begin()->MBTRms[i];
      for (int j = 0; j < NUM_RETR_LVLS; j++) 
      {
        RcSetAvWt.RC[j][i] = _FlRcSetVec.begin()->RC[j][i];
      }
    }
    return RcSetAvWt;
  }
  if (PAltKm <= _RCFHdr.Zr[_RCFHdr.NFL-1])
  {
    for (int i = 0; i< NUM_BRT_TEMPS; i++) 
    {
      RcSetAvWt.MBTAvg[i] = _FlRcSetVec.end()->MBTAvg[i];
      RcSetAvWt.MBTRms[i] = _FlRcSetVec.end()->MBTRms[i];
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
  for (int i = 0; i < NUM_BRT_TEMPS; i++)
  {
    RcSetAvWt.MBTAvg[i] = Botit->MBTAvg[i]*BotWt + Topit->MBTAvg[i]*TopWt;
    RcSetAvWt.MBTRms[i] = Botit->MBTRms[i]*BotWt + Topit->MBTRms[i]*TopWt;
    for (int j = 0; j < NUM_RETR_LVLS; j++)
    {
      RcSetAvWt.RC[j][i] = Botit->RC[j][i]*BotWt + Topit->RC[j][i]*(1-BotWt);
    }
  }

cout<<" RCwt:\n";
for (int j = 0; j < NUM_RETR_LVLS; j++) {
  for (int i = 0; i < NUM_BRT_TEMPS; i++) {
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
    cout<<"In "<< __func__ << " for RCFID: " << getId() << "\n";
    cout<<"ERROR:Number of flight levels input:";
    cout<< Len;
    cout<< " is not equal to number in RCF:";
    cout<< _RCFHdr.NFL;
    cout<<"\n";
    return false;
  } 
  for (int i = 0; i<Len; i++)
  {
    if ((i+1<Len) && (FlightLevels[i] <= FlightLevels[i+1]))
    {
      cout<<"In "<< __func__ << " for RCFID: " << getId() << "\n";
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
  for (int i=0; i<NUM_BRT_TEMPS; i++) 
  {
    RcSet->MBTRms[i] = RcUn.RC_read.sOBrms[i];
    RcSet->MBTAvg[i] = RcUn.RC_read.sOBav[i];
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

/* Output the RC matrix
for (x=0; x<NUM_RETR_LVLS; x++) {
  for (y=0; y<NUM_BRT_TEMPS; y++) {
    if (y%4==0) cout << " ["<<x<<"]["<<y<<"]:"<<RcSet->RC[x][y]<<"\n";
    else cout << "["<<x<<"]["<<y<<"]:"<<RcSet->RC[x][y];
  }
}
cout<<'\n';
*/

}

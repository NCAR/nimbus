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
cout << "slash:" << last_slash << " dot: "<<last_dot<<" size:"<<temp.length()<<'\n';
  _RCFId = temp.substr(last_slash+1, last_dot-last_slash-1);

cout << "RCFId:"<<_RCFId<<'\n';

  streampos size;

  ifstream file (_RCFFileName.c_str(), ios::in|ios::binary);
  if (file.is_open())
  {
    //size = file.tellg();
    size = sizeof(RCF_HDR);
    file.seekg (0, ios::beg);
    file.read (RcfHdr.Array, size);
    file.seekg ((-2*sizeof(float)),ios::cur);

    for (int i=0; i<RcfHdr.Rcf_Hdr.NFL; i++) {
      file.read (RcFlUn.Array, sizeof(RC_FL_Read));
      FlUn2FlRcSet(RcFlUn, &FlRcSet);
      flightLevelRCInfoVec.push_back(RcFlUn);
cout<<"IN LOOP:"<<FlRcSet.Palt;
      FlRcSetVec.push_back(FlRcSet);
      if (i+1 == RcfHdr.Rcf_Hdr.NFL) FlRcSetVec.push_back(FlRcSet);
    }
cout<<"\nAfter pushbacks:"<<FlRcSetVec.end()->Palt<<"\n";
    FlRcSetVec.pop_back();

    if (file)
      cout<< "all: "<<size<<" bytes were read.\n";
    else
      cout << "error: only " <<file.gcount()<<" bytes were read.\n";

    file.close();

    cout << "\n";
    cout << "Format    :"<<RcfHdr.Rcf_Hdr.RCformat << '\n';
    RcfHdr.Rcf_Hdr.RAOBfilename[79] = '\n';
    cout << RcfHdr.Rcf_Hdr.RAOBfilename << '\n';
    RcfHdr.Rcf_Hdr.RCfilename[79] = '\n';
    cout << RcfHdr.Rcf_Hdr.RCfilename << '\n';
    cout << "Raobcount :" << RcfHdr.Rcf_Hdr.RAOBcount << '\n';
    cout << "LR1       :"<<RcfHdr.Rcf_Hdr.LR1<<'\n';
    cout << "zLRb      :"<<RcfHdr.Rcf_Hdr.zLRb<<'\n';
    cout << "LR2       :"<<RcfHdr.Rcf_Hdr.LR2<<'\n';
    cout << "RecordStep:"<<RcfHdr.Rcf_Hdr.LR2<<'\n';
    cout << "RAOBmin   :"<<RcfHdr.Rcf_Hdr.RAOBmin<<'\n';
    cout << "ExcessTamplitude:"<<RcfHdr.Rcf_Hdr.ExcessTamplitude<<'\n';
    cout << "Nobs:"<<RcfHdr.Rcf_Hdr.Nobs<<'\n';
    cout << "Nret:"<<RcfHdr.Rcf_Hdr.Nret<<'\n';
    cout << "dZ:\n";
    for (int i=0; i<32; i++) {cout << RcfHdr.Rcf_Hdr.dZ[i] <<", ";}
    cout << RcfHdr.Rcf_Hdr.dZ[32]<<'\n';
    cout << "NFL:"<<RcfHdr.Rcf_Hdr.NFL<<'\n';
    cout << "Zr:\n";
    for (int i=0; i<19; i++) {cout << RcfHdr.Rcf_Hdr.Zr[i] <<", ";}
    cout << RcfHdr.Rcf_Hdr.Zr[19]<<'\n';
    cout << "\n";
    cout << "Nlo:"<<RcfHdr.Rcf_Hdr.Nlo<<'\n';
    cout << "SURC:"<<RcfHdr.Rcf_Hdr.SURC<<'\n';


    My_EH EH;
    int i = 0;
    for (int j=1376; j<sizeof(END_HDR); j++,i++) EH.Array[i]=RcfHdr.Array[j];
    cout << "SURC:" << EH.EH.SURC << '\n';
    cout << "CHnLSBloss:\n";
    for (int i=0; i<3;i++) {cout<< EH.EH.CHnLSBloss[i]<<',';}
    cout << '\n';
    int j = 0;

    //Convert from column major storage for SmatrixN1
    float SmatrixN1[15][3][10];
    int x=0,y=0,z=0;
    for (int i=0; i<450;i++) {
      SmatrixN1[x][y][z] = EH.EH.Gmatrix[i];
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
/*
    for (int i=0; i<10;i++) {
      cout << "SMATRIXN1:"<<SmatrixN1[14][2][i]<<'\n';
      cout << "    SMATrIXN2:"<<EH.EH.SmatrixN2[14][2][i]<<'\n';
    }
    cout << "SmatrixN1[5][1][5]:"<<SmatrixN1[5][1][5]<<'\n';
    for (int i = 450; i<460;i++) {cout<<" GM["<<i<<"]:"<<EH.EH.Gmatrix[i];}
    cout << "\n";
    //Convert from column major storage for SmatrixN2
    float SmatrixN2[15][3][10];
    x=0,y=0,z=0,j=0;
    for (int i=450; i<900;i++) {
      SmatrixN2[x][y][z] = EH.EH.Gmatrix[i];
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
    for (int i=0; i<10;i++) {
      cout << "SMATRIXN2:"<<SmatrixN2[0][0][i]<<'\n';
      cout << "    SMATrIXN2:"<<SmatrixN2[14][2][i]<<'\n';
    }
    cout << "SmatrixN2[5][1][5]:"<<SmatrixN2[5][1][5]<<'\n';
*/
    //char SURC[4]; 
    //for (int j=1376; j<1380; j++,i++) SURC[i]=RcfHdr.Array[j];
    //cout << "NewSURC:"<<SURC<<"\n";
/*
    cout << "IFoff:\n";
    for (int i=0; i<3; i++) {
      for (int j=0; j<16; j++) {
        cout << RcfHdr.Rcf_Hdr.IFoff[i][j] <<', ';
      }
      cout << '\n';
    }
    cout << "Nif:"<<RcfHdr.Rcf_Hdr.Nif<<'\n';
    for (int i=0; i<10; i++) { 
      for (int j=0; j<3; j++) { 
        cout << RcfHdr.Rcf_Hdr.SmatrixN2[1][j][i] << ', ';
      } 
      cout <<'\n';
    }
*/

    cout << "PALT at Flight Levels:" << '\n';
    vector<My_RC_FL_Un>::const_iterator it;
    for(it=flightLevelRCInfoVec.begin(); it!=flightLevelRCInfoVec.end(); it+=1)
    {
      cout << it->RC_read.sBP <<'\n';
    } 
    vector<RC_Set_1FL>::const_iterator rit;
    for (rit=FlRcSetVec.begin(); rit!=FlRcSetVec.end(); rit+=1)
    {
      cout << rit->Palt<<'\n';
    }

  }
  else cout << "Unable to open file";
  return;
}

std::string RetrievalCoefficientFile::getId() 
{
  return _RCFId;
}

RC_Set_1FL RetrievalCoefficientFile::getRCAvgWt(float PAlt)
{
  RC_Set_1FL RcSetAvWt;

  // First check to see if PAlt is outside the range of Flight Level PAlts
  //  - if so then the weighted average observalbe will be the average
  //    observable associated with the flight level whose PAlt is closest 
  //    Assumption is that the Flight Level Retrieval Coefficient Set vector
  //    is stored in increasing Palt (decreasing aircraft altitude).
std::vector<int>::size_type sz = FlRcSetVec.size();
cout<<"In get avg: PAlt:"<<PAlt<<"  1st level:"<<FlRcSetVec.begin()->Palt<<"  last level:"<<FlRcSetVec.end()->Palt<<"  -1:"<<FlRcSetVec[sz-2].Palt<<"\n";
  if (PAlt <= FlRcSetVec.begin()->Palt) 
  {
    for (int i = 0; i< NUM_OBSVBLS; i++) 
    {
      RcSetAvWt.OBAvg[i] = FlRcSetVec.begin()->OBAvg[i];
      RcSetAvWt.OBRms[i] = FlRcSetVec.begin()->OBRms[i];
      for (int j = 0; j < NUM_RETR_LVLS; j++) 
      {
        RcSetAvWt.RC[j][i] = FlRcSetVec.begin()->RC[j][i];
      }
    }
    return RcSetAvWt;
  }
  if (PAlt >= FlRcSetVec.end()->Palt)
  {
    for (int i = 0; i< NUM_OBSVBLS; i++) 
    {
      RcSetAvWt.OBAvg[i] = FlRcSetVec.end()->OBAvg[i];
      RcSetAvWt.OBRms[i] = FlRcSetVec.end()->OBRms[i];
      for (int j = 0; j < NUM_RETR_LVLS; j++) 
      {
        RcSetAvWt.RC[j][i] = FlRcSetVec.end()->RC[j][i];
      }
    }
    return RcSetAvWt;
  }

  // Find two Flight Level Sets that are above and below the PAlt provided.
  // Calculate the weight for averaging and identify the RC sets.
  vector<RC_Set_1FL>::const_iterator it, Botit, Topit;
  float BotWt,TopWt;
  for(it=FlRcSetVec.begin(); it!=FlRcSetVec.end(); it+=1)
  {
    if (PAlt >= it->Palt and PAlt <= (it+1)->Palt)
    {
      BotWt = 1 - ((PAlt - (it+1)->Palt)/(it->Palt - (it+1)->Palt));
      Topit = it;
      Botit = it+1;
cout<< "TopAlt:"<<it->Palt<<"  BotAlt:"<<(it+1)->Palt<<"  BotWt:"<<BotWt<<"\n";
    }
  }
  TopWt = 1-BotWt;


  // Calculate the Weighted averages 
  for (int i = 0; i < NUM_OBSVBLS; i++)
  {
    RcSetAvWt.OBAvg[i] = Botit->OBAvg[i]*BotWt + Topit->OBAvg[i]*(1-BotWt);
    RcSetAvWt.OBRms[i] = Botit->OBRms[i]*BotWt + Topit->OBRms[i]*(1-BotWt);
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

void RetrievalCoefficientFile::FlUn2FlRcSet(My_RC_FL_Un RcUn, RC_Set_1FL *RcSet)
{
  RcSet->Palt = RcUn.RC_read.sBP;
cout<<"Palt:"<<RcSet->Palt<<"  RMS vec:"<<'\n';
  for (int i=0; i<NUM_OBSVBLS; i++) 
  {
    RcSet->OBRms[i] = RcUn.RC_read.sOBrms[i];
cout << " ["<<i<<"]: "<<RcSet->OBRms[i];
    RcSet->OBAvg[i] = RcUn.RC_read.sOBav[i];
  }
cout<<'\n';
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
  for (y=0; y<NUM_OBSVBLS; y++) {
    if (y%4==0) cout << " ["<<x<<"]["<<y<<"]:"<<RcSet->RC[x][y]<<"\n";
    else cout << "["<<x<<"]["<<y<<"]:"<<RcSet->RC[x][y];
  }
}
cout<<'\n';
*/

}

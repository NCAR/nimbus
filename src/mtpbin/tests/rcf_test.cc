#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>

#include "../rcf.h"

main () {
  char SURCplus[5];  // Account for end of string
  std::vector<RC_Set_1FL> FL_RC_Vec;
  std::vector<RC_Set_1FL>::const_iterator rit;

  RetrievalCoefficientFile RCF(std::string("/home/local/raf/instruments/mtp/src/mtpbin/RCF/NRCKA189.RCF"));

  RCF_HDR Rcf_Hdr = RCF.getRCF_HDR();

  std::cout << "Raobcount :" << Rcf_Hdr.RAOBcount << "\n";
  std::cout << "NFL:"<< Rcf_Hdr.NFL<<"\n";
  strncpy(SURCplus, Rcf_Hdr.SURC, 4);
  SURCplus[4] = '\0';
  std::cout << "SURC:"<< SURCplus <<"\n";

  float flightLevelsKm[]={14.5,13.0,12.0,11.0,10.0,9.0,8.0,7.0,6.0,5.0,4.0,3.0,2.0,1.0,0.0};
  int numFlightLevels=15;
  if (!RCF.setFlightLevelsKm(flightLevelsKm,numFlightLevels))
  {
    std::cout<< "Major failure, exiting\n";
    exit(EXIT_FAILURE);
  }
  std::cout << "RCFid =" << RCF.getId() << '\n';
  std::cout << "PALT at Flight Levels:" << "\n";
  FL_RC_Vec = RCF.getFL_RC_Vec();
  for (rit=FL_RC_Vec.begin(); rit!=FL_RC_Vec.end(); rit+=1)
  {
    std::cout << rit->Palt<<"\n";
  }
  RC_Set_1FL AvgWtSet1;
  AvgWtSet1 = RCF.getRCAvgWt(5.3473);


  std::cout<<"MBTAvg:\n";
  for (int i = 0; i<NUM_OBSVBLS; i++) 
    std::cout << " ["<<i<<"]:"<<AvgWtSet1.MBTAvg[i];
  std::cout<<"\n";
  std::cout<<"MBTRms:\n";
  for (int i = 0; i<NUM_OBSVBLS; i++) 
    std::cout << " ["<<i<<"]:"<<AvgWtSet1.MBTRms[i];
  std::cout<<"\n";
  
  RetrievalCoefficientFile RCF2(std::string("/home/local/raf/instruments/mtp/src/mtpbin/RCF/NRCKB189.RCF"));

  Rcf_Hdr = RCF2.getRCF_HDR();
  std::cout << "Raobcount :" << Rcf_Hdr.RAOBcount << "\n";
  std::cout << "NFL:"<< Rcf_Hdr.NFL<<"\n";
  strncpy(SURCplus, Rcf_Hdr.SURC, 4);
  SURCplus[4] = '\0';
  std::cout << "SURC:"<< SURCplus <<"\n";

  if (!RCF2.setFlightLevelsKm(flightLevelsKm,numFlightLevels))
  {
    std::cout<< "Major failure, exiting\n";
    exit(EXIT_FAILURE);
  }
  
  std::cout << "RCFid =" << RCF2.getId() << '\n';
  RC_Set_1FL AvgWtSet2;
  AvgWtSet2 = RCF2.getRCAvgWt(5.3473);
  
  std::cout<<"MBTAvg:\n";
  for (int i = 0; i<NUM_OBSVBLS; i++) 
    std::cout << " ["<<i<<"]:"<<AvgWtSet2.MBTAvg[i];
  std::cout<<"\n";
  std::cout<<"MBTRms:\n";
  for (int i = 0; i<NUM_OBSVBLS; i++) 
    std::cout << " ["<<i<<"]:"<<AvgWtSet2.MBTRms[i];
  std::cout<<"\n";
  
  RetrievalCoefficientFile RCF3(std::string("/home/local/raf/instruments/mtp/src/mtpbin/RCF/NRCKH189.RCF"));
  if (!RCF3.setFlightLevelsKm(flightLevelsKm,numFlightLevels))
  {
    std::cout<< "Major failure, exiting\n";
    exit(EXIT_FAILURE);
  }
  
  std::cout << "RCFid =" << RCF3.getId() << '\n';
  RC_Set_1FL AvgWtSet3;
  AvgWtSet3 = RCF3.getRCAvgWt(5.3473);
  
  std::cout<<"MBTAvg:\n";
  for (int i = 0; i<NUM_OBSVBLS; i++) 
    std::cout << " ["<<i<<"]:"<<AvgWtSet3.MBTAvg[i];
  std::cout<<"\n";
  std::cout<<"MBTRms:\n";
  for (int i = 0; i<NUM_OBSVBLS; i++) 
    std::cout << " ["<<i<<"]:"<<AvgWtSet3.MBTRms[i];
  std::cout<<"\n";
  
}

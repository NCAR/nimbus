#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "rcf.h"

main () {
  RetrievalCoefficientFile RCF(std::string("/home/local/raf/instruments/mtp/src/mtpbin/RCF/NRCKA189.RCF"));

  //float flightLevelsKm[]={0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.5};
  float flightLevelsKm[]={14.5,13.0,12.0,11.0,10.0,9.0,8.0,7.0,6.0,5.0,4.0,3.0,2.0,1.0,0.0};
  int numFlightLevels=15;
  if (!RCF.setFlightLevelsKm(flightLevelsKm,numFlightLevels))
  {
    std::cout<< "Major failure, exiting\n";
    exit(EXIT_FAILURE);
  }
  std::cout << "RCFid =" << RCF.getId() << '\n';
  RC_Set_1FL AvgWtSet1;
  AvgWtSet1 = RCF.getRCAvgWt(5.3473);


  std::cout<<"OBAvg:\n";
  for (int i = 0; i<NUM_OBSVBLS; i++) 
    std::cout << " ["<<i<<"]:"<<AvgWtSet1.OBAvg[i];
  std::cout<<"\n";
  std::cout<<"OBRms:\n";
  for (int i = 0; i<NUM_OBSVBLS; i++) 
    std::cout << " ["<<i<<"]:"<<AvgWtSet1.OBRms[i];
  std::cout<<"\n";
  
  RetrievalCoefficientFile RCF2(std::string("/home/local/raf/instruments/mtp/src/mtpbin/RCF/NRCKB189.RCF"));
  if (!RCF2.setFlightLevelsKm(flightLevelsKm,numFlightLevels))
  {
    std::cout<< "Major failure, exiting\n";
    exit(EXIT_FAILURE);
  }
  
  std::cout << "RCFid =" << RCF2.getId() << '\n';
  RC_Set_1FL AvgWtSet2;
  AvgWtSet2 = RCF2.getRCAvgWt(5.3473);
  
  std::cout<<"OBAvg:\n";
  for (int i = 0; i<NUM_OBSVBLS; i++) 
    std::cout << " ["<<i<<"]:"<<AvgWtSet2.OBAvg[i];
  std::cout<<"\n";
  std::cout<<"OBRms:\n";
  for (int i = 0; i<NUM_OBSVBLS; i++) 
    std::cout << " ["<<i<<"]:"<<AvgWtSet2.OBRms[i];
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
  
  std::cout<<"OBAvg:\n";
  for (int i = 0; i<NUM_OBSVBLS; i++) 
    std::cout << " ["<<i<<"]:"<<AvgWtSet3.OBAvg[i];
  std::cout<<"\n";
  std::cout<<"OBRms:\n";
  for (int i = 0; i<NUM_OBSVBLS; i++) 
    std::cout << " ["<<i<<"]:"<<AvgWtSet3.OBRms[i];
  std::cout<<"\n";
  
}

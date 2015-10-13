#include <iostream>
#include <fstream>

#include "rcf.h"

main () {
RetrievalCoefficientFile RCF(std::string("/home/local/raf/instruments/mtp/src/mtpbin/RCF/NRCKA189.RCF"));

std::cout << "RCFid =" << RCF.getId() << '\n';
RC_Set_1FL AvgWtSet1;
AvgWtSet1 = RCF.getRCAvgWt(575.7458);


std::cout<<"OBAvg:\n";
for (int i = 0; i<NUM_OBSVBLS; i++) 
  std::cout << " ["<<i<<"]:"<<AvgWtSet1.OBAvg[i];
std::cout<<"\n";
std::cout<<"OBRms:\n";
for (int i = 0; i<NUM_OBSVBLS; i++) 
  std::cout << " ["<<i<<"]:"<<AvgWtSet1.OBRms[i];
std::cout<<"\n";

RetrievalCoefficientFile RCF2(std::string("/home/local/raf/instruments/mtp/src/mtpbin/RCF/NRCKB189.RCF"));

std::cout << "RCFid =" << RCF2.getId() << '\n';
RC_Set_1FL AvgWtSet2;
AvgWtSet2 = RCF2.getRCAvgWt(575.7458);

std::cout<<"OBAvg:\n";
for (int i = 0; i<NUM_OBSVBLS; i++) 
  std::cout << " ["<<i<<"]:"<<AvgWtSet2.OBAvg[i];
std::cout<<"\n";
std::cout<<"OBRms:\n";
for (int i = 0; i<NUM_OBSVBLS; i++) 
  std::cout << " ["<<i<<"]:"<<AvgWtSet2.OBRms[i];
std::cout<<"\n";

RetrievalCoefficientFile RCF3(std::string("/home/local/raf/instruments/mtp/src/mtpbin/RCF/NRCKI189.RCF"));

std::cout << "RCFid =" << RCF3.getId() << '\n';
RC_Set_1FL AvgWtSet3;
AvgWtSet3 = RCF3.getRCAvgWt(575.7458);

std::cout<<"OBAvg:\n";
for (int i = 0; i<NUM_OBSVBLS; i++) 
  std::cout << " ["<<i<<"]:"<<AvgWtSet3.OBAvg[i];
std::cout<<"\n";
std::cout<<"OBRms:\n";
for (int i = 0; i<NUM_OBSVBLS; i++) 
  std::cout << " ["<<i<<"]:"<<AvgWtSet3.OBRms[i];
std::cout<<"\n";

}

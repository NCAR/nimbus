#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>

#include "../rcf.h"

/*
 *  This program is designed to test the RetrievalCoefficientFile class
 *  to assure that it is performing as expected.  The current output file:
 *  rcf_test.out has been vetted to assure that it has correct information
 *  for the tests performed in this program.  
 *
 *  Usage: 
 *  rcf_test > rcf_test.new
 *  diff rcf_test.out rcf_test.new
 *
 *  There should be no differences found. If differences are found then
 *  something has affected the functionality of the class and should be
 *  investigated and resolved.
 *
 */

using namespace std;

main () {
  char SURCplus[5];  // Account for end of string
  std::vector<RC_Set_1FL> FL_RC_Vec;
  std::vector<RC_Set_1FL>::const_iterator rit;

  /* Validate that a file initializes correctly */
  RetrievalCoefficientFile RCF(std::string("../RCF/NRCKA189.RCF"));

  RCF_HDR Rcf_Hdr = RCF.getRCF_HDR();

  std::cout << "Raobcount :" << Rcf_Hdr.RAOBcount << "\n";
  std::cout << "NFL:"<< Rcf_Hdr.NFL<<"\n";
  strncpy(SURCplus, Rcf_Hdr.SURC, 4);
  SURCplus[4] = '\0';
  std::cout << "SURC:"<< SURCplus <<"\n";

  /* Validate that flight levels initialize properly */
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

  /* Check on a few more items from the initialization */
  std::cout << "FileName:"<<RCF.getFileName().c_str()<<"\n";
  std::vector<RC_Set_1FL> FlRcSetVec = RCF.getFL_RC_Vec();
  std::vector<int>::size_type sz = FlRcSetVec.size();
  std::cout <<"PAltKm:5.3473"<<"  1st level:"<<FlRcSetVec.begin()->Palt
            <<"  last level:"<<FlRcSetVec.end()->Palt<<"  -1:"
            <<FlRcSetVec[sz-2].Palt<<"\n";

  /* Check on the average weighting function */
  RC_Set_1FL AvgWtSet1;
  AvgWtSet1 = RCF.getRCAvgWt(5.3473);
  std::cout<<"Num,   Avg,  Rms\n";
  for (int i = 0; i < NUM_BRT_TEMPS; i++) {
    std::cout<<i<<", "<<AvgWtSet1.MBTAvg[i]<<", "<<AvgWtSet1.MBTRms[i]<<"\n";
  }

  std::cout<<"MBTAvg:\n";
  for (int i = 0; i<NUM_BRT_TEMPS; i++) 
    std::cout << " ["<<i<<"]:"<<AvgWtSet1.MBTAvg[i];
  std::cout<<"\n";
  std::cout<<"MBTRms:\n";
  for (int i = 0; i<NUM_BRT_TEMPS; i++) 
    std::cout << " ["<<i<<"]:"<<AvgWtSet1.MBTRms[i];
  std::cout<<"\n";
  cout<<" RCwt:\n";
  for (int j = 0; j < NUM_RETR_LVLS; j++) {
    for (int i = 0; i < NUM_BRT_TEMPS; i++) {
      if (i%5 != 0) cout<<"["<<j<<"]["<<i<<"]:"<<AvgWtSet1.RC[j][i];
      else cout<<"["<<j<<"]["<<i<<"]:"<<AvgWtSet1.RC[j][i]<<'\n';
    }
  }
  cout <<"\n\n";


  /* Now do all the same testing on a second file
   * Validate that a file initializes correctly */
  RetrievalCoefficientFile RCF2(std::string("../RCF/NRCKB189.RCF"));

  Rcf_Hdr = RCF2.getRCF_HDR();
  std::cout << "Raobcount :" << Rcf_Hdr.RAOBcount << "\n";
  std::cout << "NFL:"<< Rcf_Hdr.NFL<<"\n";
  strncpy(SURCplus, Rcf_Hdr.SURC, 4);
  SURCplus[4] = '\0';
  std::cout << "SURC:"<< SURCplus <<"\n";

  /* Validate that flight levels initialize properly */
  if (!RCF2.setFlightLevelsKm(flightLevelsKm,numFlightLevels))
  {
    std::cout<< "Major failure, exiting\n";
    exit(EXIT_FAILURE);
  }
  
  /* Check on a few more items from the initialization */
  std::cout << "RCFid =" << RCF2.getId() << '\n';
  std::cout << "FileName:"<<RCF2.getFileName().c_str()<<"\n";
  FlRcSetVec = RCF2.getFL_RC_Vec();
  sz = FlRcSetVec.size();
  std::cout <<"PAltKm:5.3473"<<"  1st level:"<<FlRcSetVec.begin()->Palt
            <<"  last level:"<<FlRcSetVec.end()->Palt<<"  -1:"
            <<FlRcSetVec[sz-2].Palt<<"\n";

  /* Check on the average weighting function */
  RC_Set_1FL AvgWtSet2;
  AvgWtSet2 = RCF2.getRCAvgWt(5.3473);
  std::cout<<"Num,   Avg,   Rms\n";
  for (int i = 0; i < NUM_BRT_TEMPS; i++) {
    std::cout<<i<<", "<<AvgWtSet2.MBTAvg[i]<<", "<<AvgWtSet2.MBTRms[i]<<"\n";
  }
  
  std::cout<<"MBTAvg:\n";
  for (int i = 0; i<NUM_BRT_TEMPS; i++) 
    std::cout << " ["<<i<<"]:"<<AvgWtSet2.MBTAvg[i];
  std::cout<<"\n";
  std::cout<<"MBTRms:\n";
  for (int i = 0; i<NUM_BRT_TEMPS; i++) 
    std::cout << " ["<<i<<"]:"<<AvgWtSet2.MBTRms[i];
  std::cout<<"\n";
  cout<<" RCwt:\n";
  for (int j = 0; j < NUM_RETR_LVLS; j++) {
    for (int i = 0; i < NUM_BRT_TEMPS; i++) {
      if (i%5 != 0) cout<<"["<<j<<"]["<<i<<"]:"<<AvgWtSet2.RC[j][i];
      else cout<<"["<<j<<"]["<<i<<"]:"<<AvgWtSet2.RC[j][i]<<'\n';
    }
  }
  cout <<"\n\n";
  
  /* Now do all the similar testing on a third file
   * Validate that a file initializes correctly */
  RetrievalCoefficientFile RCF3(std::string("../RCF/NRCKH189.RCF"));

  /* Validate that flight levels initialize properly */
  if (!RCF3.setFlightLevelsKm(flightLevelsKm,numFlightLevels))
  {
    std::cout<< "Major failure, exiting\n";
    exit(EXIT_FAILURE);
  }
  
  /* Check on a few more items from the initialization */
  std::cout << "RCFid =" << RCF3.getId() << '\n';
  RC_Set_1FL AvgWtSet3;
  std::cout << "FileName:"<<RCF3.getFileName().c_str()<<"\n";
  FlRcSetVec = RCF3.getFL_RC_Vec();
  sz = FlRcSetVec.size();
  std::cout <<"PAltKm:5.3473"<<"  1st level:"<<FlRcSetVec.begin()->Palt
            <<"  last level:"<<FlRcSetVec.end()->Palt<<"  -1:"
            <<FlRcSetVec[sz-2].Palt<<"\n";

  /* Check on the average weighting function */
  AvgWtSet3 = RCF3.getRCAvgWt(5.3473);
  std::cout<<"Num,   Avg,  Rms\n";
  for (int i = 0; i < NUM_BRT_TEMPS; i++) {
    std::cout<<i<<",  "<<AvgWtSet3.MBTAvg[i]<<",  "<<AvgWtSet3.MBTRms[i]<<"\n";
  }
  
  std::cout<<"MBTAvg:\n";
  for (int i = 0; i<NUM_BRT_TEMPS; i++) 
    std::cout << " ["<<i<<"]:"<<AvgWtSet3.MBTAvg[i];
  std::cout<<"\n";
  std::cout<<"MBTRms:\n";
  for (int i = 0; i<NUM_BRT_TEMPS; i++) 
    std::cout << " ["<<i<<"]:"<<AvgWtSet3.MBTRms[i];
  std::cout<<"\n";
  cout<<" RCwt:\n";
  for (int j = 0; j < NUM_RETR_LVLS; j++) {
    for (int i = 0; i < NUM_BRT_TEMPS; i++) {
      if (i%5 != 0) cout<<"["<<j<<"]["<<i<<"]:"<<AvgWtSet3.RC[j][i];
      else cout<<"["<<j<<"]["<<i<<"]:"<<AvgWtSet3.RC[j][i]<<'\n';
    }
  }
  cout <<"\n\n";
  
}

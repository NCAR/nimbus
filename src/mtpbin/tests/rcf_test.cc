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

void rcf_test(std::string);

main () {

  rcf_test(std::string("../RCF/NRCKA189.RCF"));
  rcf_test(std::string("../RCF/NRCKB189.RCF"));
  rcf_test(std::string("../RCF/NRCKH189.RCF"));

}

void rcf_test(std::string file)
{
  char SURCplus[5];  // Account for end of string
  std::vector<RC_Set_1FL> FL_RC_Vec;
  std::vector<RC_Set_1FL>::const_iterator rit;

  float flightLevelsKm[]={14.5,13.0,12.0,11.0,10.0,9.0,8.0,7.0,6.0,5.0,4.0,3.0,2.0,1.0,0.0};
  int numFlightLevels=15;

   /* Validate that a file initializes correctly */
  RetrievalCoefficientFile RCF(file);

  RCF_HDR Rcf_Hdr = RCF.getRCF_HDR();

  std::cout << "Raobcount :" << Rcf_Hdr.RAOBcount << "\n";
  std::cout << "NFL:"<< Rcf_Hdr.NFL<<"\n";
  strncpy(SURCplus, Rcf_Hdr.SURC, 4);
  SURCplus[4] = '\0';
  std::cout << "SURC:"<< SURCplus <<"\n";

  /* Validate that flight levels initialize properly */
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
  RC_Set_1FL AvgWtSet;
  AvgWtSet = RCF.getRCAvgWt(5.3473);
  std::cout<<"Num,   Avg,  Rms\n";
  for (int i = 0; i < NUM_BRT_TEMPS; i++) {
    std::cout<<i<<", "<<AvgWtSet.MBTAvg[i]<<", "<<AvgWtSet.MBTRms[i]<<"\n";
  }

  std::cout<<"MBTAvg:\n";
  for (int i = 0; i<NUM_BRT_TEMPS; i++) 
    std::cout << " ["<<i<<"]:"<<AvgWtSet.MBTAvg[i];
  std::cout<<"\n";
  std::cout<<"MBTRms:\n";
  for (int i = 0; i<NUM_BRT_TEMPS; i++) 
    std::cout << " ["<<i<<"]:"<<AvgWtSet.MBTRms[i];
  std::cout<<"\n";
  cout<<" RCwt:\n";
  for (int j = 0; j < NUM_RETR_LVLS; j++) {
    for (int i = 0; i < NUM_BRT_TEMPS; i++) {
      if (i%5 != 0) cout<<"["<<j<<"]["<<i<<"]:"<<AvgWtSet.RC[j][i];
      else cout<<"["<<j<<"]["<<i<<"]:"<<AvgWtSet.RC[j][i]<<'\n';
    }
  }
  cout <<"\n\n";
}

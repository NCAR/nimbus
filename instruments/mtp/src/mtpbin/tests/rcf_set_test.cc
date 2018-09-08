#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "../rcf_set.h"
#include "test.h"

using namespace std;

/*
 * This program is designed to test the RetrievalCoefficientFileSet class
 * to assure that it's performing as expected.  The current output file:
 * rcf_set_test.out has been vetted to assure that it has correct information
 * resulting from the tests this program peforms.
 *
 * Usage:
 * rcf_set_test > rcf_set_test.new
 * diff rcf_set_test.out rcf_set_test.new
 *
 * There should be no differences found.  If differences are found then 
 * something has affected the functionality of the class and should be 
 * investigated and resolved.
 */

void rcf_set_test(std::vector<float>,double,std::string,RetrievalCoefficientFileSet);

main () {

  /* Test initialization with a bogus directory */
  RetrievalCoefficientFileSet Bad_RCF_Set(std::string("../"));

  // Test error handling for setting of flight levels
  cout << "Check that setting flight levels on uninitialized set fails. \n";
  Bad_RCF_Set.setFlightLevelsKm(flightLevelsKmH, numFlightLevelsH);

  /* Now initialize with a valid RCF directory */
  RetrievalCoefficientFileSet RCF_Set(RCFdirH);

  std::vector<RetrievalCoefficientFile> RCFs = RCF_Set.getRCFVector();

  RCF_HDR Rcf_Hdr;

  for (std::vector<RetrievalCoefficientFile>::iterator
       it = RCFs.begin(); it != RCFs.end(); ++it)
  {
    cout << "\n\nFound RCF file:"<<it->getFileName()<<" with ID:"<<it->getId()
         <<"\n";
    Rcf_Hdr = it->getRCF_HDR();
    cout << "Raobcount :" << Rcf_Hdr.RAOBcount << "\n";
    cout << "NFL:"<< Rcf_Hdr.NFL<<"\n";
    cout << "First Smatrix2:"<<Rcf_Hdr.SmatrixN2[0][0][0]<<"\n";
  }

  /* Set flight levels */
  RCF_Set.setFlightLevelsKm(flightLevelsKmH, numFlightLevelsH);

  /* Validate the function for selecting an RCF (template) from the set */
  rcf_set_test(scanBTsH1,ACAltKmH1,FlightDescH1,RCF_Set);

  rcf_set_test(scanBTsH3,ACAltKmH3,FlightDescH3,RCF_Set);
}



void rcf_set_test(std::vector<float> scanBTs,double ACAltKm,std::string FlightDesc,
	RetrievalCoefficientFileSet RCF_Set) {

  RC_Set_4Retrieval BestWtdRCSet;
  std::vector<RC_Set_1FL> FlRcSetVec;
  std::vector<int>::size_type sz;
  RC_Set_1FL AvgWtSet;

  // Validate that the function for selecting an RCF (template) from the
  // set is still functioning properly.  Provide a couple of scan values 
  // and verify that the correct RCF is selected.
  cout<<"About to call getBestWeightedRCSet\n";
  BestWtdRCSet = RCF_Set.getBestWeightedRCSet(scanBTs, ACAltKm, 0.0);
  RetrievalCoefficientFile TempRCF(RCF_Set.getRCFbyId(BestWtdRCSet.RCFId));

  // Provide some verification that math going into RCF selection is correct
  // by looking at the avg weighted flight level data.
  if (TempRCF.isValid()) 
  {
    cout<<"BestWeightedRCSet is from file:"
        <<BestWtdRCSet.RCFFileName.c_str()<<"\n";
    FlRcSetVec = TempRCF.getFL_RC_Vec();
    sz = FlRcSetVec.size();
    std::cout <<"PAltKm:"<<ACAltKm<<" 1st level:"<<FlRcSetVec.begin()->Palt
              <<"  last level:"<<FlRcSetVec.end()->Palt<<"  -1:"
              <<FlRcSetVec[sz-2].Palt<<"\n";
    AvgWtSet = TempRCF.getRCAvgWt(ACAltKm);
    std::cout<<"Num,   Avg,  Rms\n";
    for (int i = 0; i < NUM_BRT_TEMPS; i++) 
    {
      std::cout<<i<<", "<<AvgWtSet.MBTAvg[i]<<", "<<AvgWtSet.MBTRms[i]<<"\n";
    }
    cout<<" RCwt:\n";
    for (int j = 0; j < NUM_RETR_LVLS; j++) {
      for (int i = 0; i < NUM_BRT_TEMPS; i++) {
        if (i%5 != 0) cout<<"["<<j<<"]["<<i<<"]:"<<AvgWtSet.RC[j][i];
        else cout<<"["<<j<<"]["<<i<<"]:"<<AvgWtSet.RC[j][i]<<'\n';
      }
    }
    cout <<"\n\n";
  }

  cout<<"Best Retrieval Coef for "<<FlightDesc<<":\n"
      <<"  ID:"<<BestWtdRCSet.RCFId.c_str()<<"\n  "
      <<"Filename:"<<BestWtdRCSet.RCFFileName.c_str()<<"\n  "
      <<"SumLnProb:"<<BestWtdRCSet.SumLnProb<<"\n";
}

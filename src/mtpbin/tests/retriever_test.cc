#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "../retriever.h"
#include "test.h"

using namespace std;

/*
 * This program is designed to test the Retriever class
 * to assure that it's performing as expected.  The current output file:
 * retriever.out has been vetted to assure that it has correct information
 * resulting from the tests this program peforms.
 *
 * Usage:
 * retriever_test > retriever_test.new
 * diff retriever_test.out retriever_test.new
 *
 * There should be no differences found.  If differences are found then 
 * something has affected the functionality of the class and should be 
 * investigated and resolved.
 */

void retriever_test(float *, int, std::vector<float>, double, std::string);

main () {
  // First HIPPO test
  retriever_test(flightLevelsKmH,numFlightLevelsH,scanBTsH1,ACAltKmH1,RCFdirH);

  // Second HIPPO test
  retriever_test(flightLevelsKmH,numFlightLevelsH,scanBTsH2,ACAltKmH2,RCFdirH);

  // DEEPWAVE test
  retriever_test(flightLevelsKmD,numFlightLevelsD,scanBTsD1,ACAltKmD1,RCFdirD);

  // CSET test
  //retriever_test(flightLevelsKmC,numFlightLevelsC,scanBTsC1,ACAltKmC1,RCFdirC);
}

void retriever_test(float *flightLevelsKm, int numFlightLevels, 
	std::vector<float> scanBTs,double ACAltKm,std::string RCFdir) {

  /* Put together a functioning retrieval_coefficient_fileset */
  RetrievalCoefficientFileSet RCF_Set(RCFdir);
  RCF_Set.setFlightLevelsKm(flightLevelsKm, numFlightLevels);
  Retriever Rtr(RCF_Set);

  // Validate that the function for selecting an RCF (template) from the
  // set is still functioning properly.  Provide a couple of scan values 
  // and verify that the correct RCF is selected.
  cout<<"About to call Retrieve\n";
  AtmosphericTemperatureProfile ATP;
  ATP = Rtr.Retrieve(scanBTs, ACAltKm);

  cout<<"Profile Temperatures:\n";
  for (int i=0; i<NUM_RETR_LVLS; i++) 
  {
    cout << "[" << i << "]:" << ATP.Temperatures[i];
    if (i%5 == 0) cout << "\n";
  }
  cout<<"\n\n";

  cout<<"Profile Altitudes:\n";
  for (int i=0; i<NUM_RETR_LVLS; i++)
  {
    cout << "[" << i << "]:" << ATP.Altitudes[i];
    if (i%5 == 0) cout << "\n";
  } 
  cout<<"\n\n";

}
/******************************************************************************/

/*
  RC_Set_4Retrieval BestWtdRCSet = RCF_Set.getBestWeightedRCSet(scanBTs, 
                                                                12.49947, 0.0);
  RetrievalCoefficientFile TempRCF1(RCF_Set.getRCFbyId(BestWtdRCSet.RCFId));
  std::vector<RC_Set_1FL> FlRcSetVec;
  std::vector<int>::size_type sz;
  RC_Set_1FL AvgWtSet;

  // Provide some verification that math going into RCF selection is correct
  // by looking at the avg weighted flight level data.
  if (TempRCF1.isValid()) 
  {
    cout<<"1st BestWeightedRCSet is from file:"
        <<BestWtdRCSet.RCFFileName.c_str()<<"\n";
    FlRcSetVec = TempRCF1.getFL_RC_Vec();
    sz = FlRcSetVec.size();
    std::cout <<"PAltKm:12.49947  1st level:"<<FlRcSetVec.begin()->Palt
              <<"  last level:"<<FlRcSetVec.end()->Palt<<"  -1:"
              <<FlRcSetVec[sz-2].Palt<<"\n";
    AvgWtSet = TempRCF1.getRCAvgWt(12.49947);
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

  cout<<"Best Retrieval Coef for HIPPO-5 flt on 8/9/2011 at 152700 hours:\n"
      <<"  ID:"<<BestWtdRCSet.RCFId.c_str()<<"\n  "
      <<"Filename:"<<BestWtdRCSet.RCFFileName.c_str()<<"\n  "
      <<"SumLnProb:"<<BestWtdRCSet.SumLnProb<<"\n";

  // ob(1)-ob(30) values for scan at 220214 HIPPPO-5 flight date 20110901
  scanBTs   =       {262.1815, 262.0143, 262.2418, 263.3536, 264.2274,
                     264.8418, 264.9377, 265.8375, 266.8653, 268.4430,
                     263.2054, 263.5550, 263.7445, 264.3099, 264.4827, 
                     264.8418, 264.9814, 265.5672, 266.2898, 267.8371,
                     262.9489, 263.1401, 263.4613, 264.2195, 264.6548,
                     264.8418, 265.1116, 265.6754, 266.2920, 267.0385};

  cout<<"About to call getBestWeightedRCSet\n";
  BestWtdRCSet = RCF_Set.getBestWeightedRCSet(scanBTs, 2.050817, 0.0);
  RetrievalCoefficientFile TempRCF2(RCF_Set.getRCFbyId(BestWtdRCSet.RCFId));
  if (TempRCF2.isValid()) 
  {
    cout<<"2nd BestWeightedRCSet is from file:"
        <<BestWtdRCSet.RCFFileName.c_str()<<"\n";
    FlRcSetVec = TempRCF2.getFL_RC_Vec();
    sz = FlRcSetVec.size();
    std::cout <<"PAltKm:2.050817  1st level:"<<FlRcSetVec.begin()->Palt
              <<"  last level:"<<FlRcSetVec.end()->Palt<<"  -1:"
              <<FlRcSetVec[sz-2].Palt<<"\n";
    AvgWtSet = TempRCF2.getRCAvgWt(2.050817);
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
  

  cout<<"Best Retrieval Coef for HIPPO-5 flt on 9/1/2011 at 220214 hours:\n"
      <<"  ID:"<<BestWtdRCSet.RCFId.c_str()<<"\n  "
      <<"Filename:"<<BestWtdRCSet.RCFFileName.c_str()<<"\n  "
      <<"SumLnProb:"<<BestWtdRCSet.SumLnProb<<"\n";

*/

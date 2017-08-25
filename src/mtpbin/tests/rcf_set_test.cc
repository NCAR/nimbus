#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "../rcf_set.h"

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

main () {

  /* Test initialization with a bogus directory */
  RetrievalCoefficientFileSet Bad_RCF_Set(std::string("../"));

  float flightLevelsKm[]={14.5,13.0,12.0,11.0,10.0,9.0,8.0,7.0,6.0,5.0,4.0,3.0,2.0,1.0,0.0};
  int numFlightLevels=15;

  // Test error handling for setting of flight levels
  cout << "Check that setting flight levels on uninitialized set fails. \n";
  Bad_RCF_Set.setFlightLevelsKm(flightLevelsKm, numFlightLevels);


  /* Now initialize with a valid RCF directory */
  RetrievalCoefficientFileSet RCF_Set(std::string("../RCF/"));

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

  RCF_Set.setFlightLevelsKm(flightLevelsKm, numFlightLevels);

  // Validate that the function for selecting an RCF (template) from the
  // set is still functioning properly.  Provide a couple of scan values 
  // and verify that the correct RCF is selected.
  //
  // Values were determined by running the VB program MTPBin in the debugger
  // and checking input and output values there.  
  //
  // ob(1)-ob(30) values for scan at 152700 HIPPPO-5 flight date 20110809
  //
  std::vector<float> scanBTs = {210.9473, 213.5502, 215.0358, 216.6606, 217.8942,
                     219.3694, 219.8724, 221.0873, 223.0352, 225.3323,
                     214.6268, 214.9759, 216.3311, 217.3704, 218.7520, 
                     219.3694, 219.3790, 219.9641, 220.7292, 222.0580,
                     216.1557, 216.8213, 217.1181, 217.9902, 218.5839,
                     219.3694, 219.3111, 219.1711, 219.6915, 220.3420};

  cout<<"About to call getBestWeightedRCSet\n";
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
  scanBTs = {262.1815, 262.0143, 262.2418, 263.3536, 264.2274,
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


}

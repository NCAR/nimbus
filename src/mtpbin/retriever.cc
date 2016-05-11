#include <stdio.h>
#include <string.h>
#include <iostream>
#include "rcf_set.h"
#include "retriever.h"

using namespace std;

std::vector<float> Retriever::Retrieve(float ScanBTs[], float ACAltKm)
{
  float Temperature;
  std::vector<float> TempProf;
  RC_Set_4Retrieval BestWtdRCSet = 
                      _Rcf_Set.getBestWeightedRCSet(ScanBTs, ACAltKm, 0.0);
  for (int L = 0; L < NUM_RETR_LVLS; L++) 
  {
    Temperature = BestWtdRCSet.FL_RCs.TAvgRl[L];
    for (int j = 0; j < NUM_BRT_TEMPS; j++)
    {
      float BtDiff = (ScanBTs[j] - BestWtdRCSet.FL_RCs.MBTAvg[j]);
      Temperature = Temperature + (BestWtdRCSet.FL_RCs.RC[L][j] * BtDiff);
    }
    TempProf.push_back(Temperature);
  }

  return TempProf;
}


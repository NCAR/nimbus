#include <stdio.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include "rcf_set.h"
#include "retriever.h"

using namespace std;

AtmosphericTemperatureProfile Retriever::Retrieve(float ScanBTs[], 
                                                  float ACAltKm)
{
  float Temperature;
  AtmosphericTemperatureProfile ATP;
  std::vector<float> PressureAlts;
  RC_Set_4Retrieval BestWtdRCSet = 
                      _Rcf_Set.getBestWeightedRCSet(ScanBTs, ACAltKm, 0.0);
  for (int L = 0; L < NUM_RETR_LVLS; L++) 
  {
    Temperature = BestWtdRCSet.FL_RCs.TAvgRl[L];
    PressureAlts.push_back(BestWtdRCSet.FL_RCs.PAltRl[L]);
    for (int j = 0; j < NUM_BRT_TEMPS; j++)
    {
      float BtDiff = (ScanBTs[j] - BestWtdRCSet.FL_RCs.MBTAvg[j]);
      Temperature = Temperature + (BestWtdRCSet.FL_RCs.RC[L][j] * BtDiff);
    }
    ATP.Temperatures.push_back(Temperature);
  }

  ATP.Altitudes = Pressure2Km(PressureAlts);

  return ATP;
}

/*
 *  MJ's rather elaborate way of converting from pressure altitude to km.
 */
std::vector<float> Retriever::Pressure2Km(std::vector<float> Pressures)
{
  std::vector<float> Altitudes;
  float Altitude;

  for (std::vector<float>::iterator Pit = Pressures.begin(); 
                                Pit != Pressures.end(); ++Pit)
  {
    if (*Pit > 226.3206)  // < 11km
    {
      Altitude = 44.3307692307692 * 
                        (1.0 - pow((*Pit/1013.25),0.190263235151657));
    } else if (*Pit > 54.7488)  // < 20km
    {
      Altitude = 11.0 - (6.34161998393947*log(*Pit/226.3206));
    } else if (*Pit > 8.680185) // < 32km
    {
      Altitude = 20.0 - (216.65 * 
                         (1.0 - pow((*Pit/54.7488),-0.0292712699464088)));
    } else if (*Pit > 1.109063)  // < 47km
    {
      Altitude = 32.0 - (81.6607142857143 * 
                         (1.0 - pow((*Pit/8.680185),-0.0819595474499447)));
    } else if (*Pit > 0.6693885)  // < 51km
    {
      Altitude = 47.0 - (7.92226839904554 * log(*Pit/1.109063));
    } else if (*Pit > 0.03956419)   // <71 km
    {
      Altitude = 51.0 + (96.6607142857143 * 
                         (1.0 - pow((*Pit/0.6693885),(0.0819595474499447))));
    } else if (*Pit > 1.45742511874549E-03)   //<90 km
    {
      Altitude = 71.0 + (107.325 * 
                         (1.0 - pow((*Pit/0.03956419),(5.85425338928176E-02))));
    } else if (*Pit > 5.8654139565495E-04)   // <95 km
    {
      Altitude = 84.852 - (5.47214624555127 * log(*Pit/0.003733834));
    } else if (*Pit > 2.40645796828482E-04)  // <100 km
    {
      Altitude = 95.0 - (140.597 * 
                          (1.0 - pow((*Pit/5.8654139565495E-04),
                                     (-3.92234986852218E-02))));
    } else if (*Pit > 1.03251578598705E-04)  // <105 km
    {
      Altitude = 100.0 - (71.20438 * 
                          (1.0 - pow((*Pit/2.40645796828482E-04),
                                     (-8.02032717123127E-02))));
    } else if (*Pit > 4.81695302325482E-05)  // <110 km
    {
      Altitude = 105.0 - (33.46154 * 
                          (1.0 - pow((*Pit/1.03251578598705E-04),
                                     (-0.18265269904593))));
    } else
    {
      if (*Pit <= 0) *Pit=0.000001;
      Altitude = 110.0 - (20.0 * 
                           (1.0 - pow((*Pit/4.81695302325482E-05), 
                                      (-0.351255203356906))));
    }

    Altitudes.push_back(Altitude);
  }

  return Altitudes;

}

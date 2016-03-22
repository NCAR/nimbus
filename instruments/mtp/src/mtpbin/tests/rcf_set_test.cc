#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "../rcf_set.h"

using namespace std;

main () {
  RetrievalCoefficientFileSet Bad_RCF_Set(std::string("/home/local/raf/instruments/mtp/src/mtpbin/"));

  float flightLevelsKm[]={14.5,13.0,12.0,11.0,10.0,9.0,8.0,7.0,6.0,5.0,4.0,3.0,2.0,1.0,0.0};
  int numFlightLevels=15;

  // Test error handling for setting of flight levels
  cout << "Check that setting flight levels on uninitialized set fails. \n";
  Bad_RCF_Set.setFlightLevelsKm(flightLevelsKm, numFlightLevels);


  RetrievalCoefficientFileSet RCF_Set(std::string("/home/local/raf/instruments/mtp/src/mtpbin/RCF/"));

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
}

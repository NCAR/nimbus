#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "../calibrator.h"

using namespace std;

/*
 * This program is designed to test the Calibrator class to assure that 
 * it is performing as expected.  The current output file:
 * calibrator.out has been vetted to assure that it has correct information
 * resulting from the tests this program peforms.
 *
 * Usage:
 * calibrator_test > calibrator_test.new
 * diff calibrator_test.out calibrator_test.new
 *
 * There should be no differences found.  If differences are found then 
 * something has affected the functionality of the class and should be 
 * investigated and resolved.
 */

main () {

  // Put together a sample case for testing the calibrator 

  // HIPPO-5 from Tom's gain eqn document
  // Create vectors with three elements
  //std::vector<float> cnd0={86.702,76.458,79.942};
  //std::vector<float> gof={40.6,0.0,0.0};
  //std::vector<float> gec1={18.98,23.27,26.27};
  //std::vector<float> gec2={-0.01,-0.01,-0.01};

  // Get cnd0, gof, gec1, and gec2 from the REF file. Ref is binary, so I am reading
  // these from the CAL file. (DEEPWAVE/raw/20140606/NG20140606.CAL)
  std::vector<float> cnd0={86.926,76.711,80.198};
  std::vector<float> gof={40.6,0.0,0.0};
  std::vector<float> gec1={19.87,23.10,26.05};
  std::vector<float> gec2={-0.1,-0.1,-0.1};
  

  // Initialize a calibrator instance.
  Calibrator CBrator(cnd0, gof, gec1, gec2);

  // Call the calibrator function to convert counts to Tb.
  // DEEPWAVE 20140606 (RF01) 06:33:33 (23613)
  // (data_dump -i 99,500 20140606_040820_rf01.ads)
  std::vector<int> ScanCounts = {17103,18227,17524,17128,18238,17537,17146,
      18257,17546,17178,18295,17568,17215,18331,17592,17255,18369,17614,17285,
      18381,17640,17315,18407,17661,17363,18431,17690,17402,18459,17716};
  std::vector<int> PltWireCnts= {2173,13809,13808,7809,13344,13296,13230,14456};
  std::vector<int> TargetCnts= {20806,22251,22084,19101,20508,20031};


  // Calculate Tb from counts
  std::vector<float> ScanBTs = CBrator.Calibrate(ScanCounts, PltWireCnts,TargetCnts);

  // Write generated Tb to screen.
  cout<<"ScanBTs:";
  for (int i=0; i<NUM_BRT_TEMPS;i++)
  {
      cout<< ScanBTs[i] << ",";
  }
  cout<<"\n\n";

}

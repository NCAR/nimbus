#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "../calibrator.h"
#include "test.h"

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
  // Get constants from test.h

  // Initialize a calibrator instance.
  Calibrator CBrator(cnd0, gof, gec1, gec2);

  // Call the calibrator function to convert counts to Tb.
  // Calculate Tb from counts
  std::vector<float> ScanBTs = CBrator.Calibrate(ScanCounts, PltWireCnts,TargetCnts, OAT);

  // Write generated Tb to screen.
  cout<<"ScanBTs:";
  for (int i=0; i<NUM_BRT_TEMPS;i++)
  {
      cout<< ScanBTs[i] << ",";
  }
  cout<<"\n";
}

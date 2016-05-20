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

  std::vector<float> cnd0;
  std::vector<float> gof;
  std::vector<float> gec1;
  std::vector<float> gec2;

  for (int i=0; i<NUM_CHANNELS; i++) 
  {
    cnd0.push_back(0.0);
    gof.push_back(0.0);
    gec1.push_back(0.0);
    gec2.push_back(0.0);
  }

  Calibrator CBrator(cnd0, gof, gec1, gec2);
}

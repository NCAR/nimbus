/*
-------------------------------------------------------------------------
OBJECT NAME:	nc_compare.cc

FULL NAME:	netCDF compare program

DESCRIPTION:	Compare two netCDF files and output the differnces.

COPYRIGHT:	University Corporation for Atmospheric Research, 2016
-------------------------------------------------------------------------
*/

#include "NcComparison.h"

#include <iostream>
#include <boost/program_options.hpp>

using std::cout;
using std::cerr;
namespace po = boost::program_options;

int
nc_compare(int argc, char *argv[])
{
  // Declare the supported options.
  po::options_description
    desc("nc_compare [options] primary_file secondary_file");
  desc.add_options()
    ("help", "Show this help message.")
    ("showequal", "Show equal objects as well as different.")
    ("ignore", po::value<std::vector<std::string> >()->composing(),
     "Ignore attributes and variables with the given names.")
    ("file", po::value<std::vector<std::string> >(), "Input files.")
    ("delta", po::value<double>(),
     "Error delta allowed for floating point values to be equal.  "
     "This is a floating point delta value, meaning "
     "the absolute difference between two numbers, no matter how large, must "
     "be less than the error delta.  To determine if floats of arbitrary "
     "range are close enough, use the ulps option.")
    ("ulps", po::value<int>(),
     "Number of Units the Last Places in which floating point numbers can "
     "differ and still be considered equal.")
    ;

  po::positional_options_description p;
  p.add("file", 2);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).
	    options(desc).positional(p).run(), vm);
  po::notify(vm);

  if (vm.count("help")) {
    cout << desc << "\n";
    return 1;
  }

  std::vector<std::string> files;
  if (vm.count("file"))
    files = vm["file"].as<std::vector<std::string> >();
  if (files.size() != 2)
  {
    cerr << "Two input files must be specified.\n";
    exit(1);
  }

  ncopts = 0;
  putenv((char *)"TZ=UTC");	// All time calcs in UTC.

  NcCache left(files[0]);
  NcCache right(files[1]);

  CompareNetcdf ncdiff(&left, &right);
  ncdiff.showEqual(vm.count("showequal") > 0);
  std::vector<std::string> ignores;
  if (vm.count("ignore"))
    ignores = vm["ignore"].as<std::vector<std::string> >();
  if (ignores.size())
  {
    ncdiff.ignore(ignores);
  }
  compare_floating_point cfp;
  if (vm.count("ulps"))
  {
    int ulps = vm["ulps"].as<int>();
    cfp.setULPS(ulps);
    cfp.useULPS();
  }
  if (vm.count("delta"))
  {
    double delta = vm["delta"].as<double>();
    cfp.setDoubleDelta(delta);
    cfp.setFloatDelta(delta);
    cfp.useDelta();
  }
  ncdiff.setFloatComparator(cfp);
  ncdiff.compare();
  ncdiff.report(cout);
  int ndiffs = ncdiff.countDifferences();
  if (ndiffs)
  {
    std::cout << ndiffs << " differences.\n";
  }
  return ndiffs;
}


int
main(int argc, char *argv[])
{
  try {
    return nc_compare(argc, argv);
  }
  catch (const std::runtime_error& error)
  {
    cerr << "Error: " << error.what() << std::endl;
    exit(1);
  }
}
    

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
    ("showindex",
     "For vector values, report the indexes of the differences.")
    ("ignore", po::value<std::vector<std::string> >()->composing(),
     "Ignore attributes and variables with the given names.")
    ("file", po::value<std::vector<std::string> >(), "Input files.")
    ("delta", po::value<double>(),
     "Error delta allowed for floating point values to be equal.  "
     "This is a floating point delta value, meaning "
     "the absolute difference between two numbers, no matter how large, must "
     "be less than the error delta.  To determine if floats of arbitrary "
     "range are close enough, use the ulps option.")
    ("ulps",
     po::value<int>()->default_value(compare_floating_point().getULPS()),
     "Number of Units the Last Places in which floating point numbers can "
     "differ and still be considered equal.  This is the floating point "
     "comparison used if --delta or --ulps is not specified explicitly.")
    ("limit",
     po::value<int>()->default_value(CompareNetcdf::DEFAULT_REPORT_LIMIT),
     "Maximum number of differences to show in the report.  Once the limit "
     "is reached, no more differences are shown.")
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
  ncdiff.showIndex(vm.count("showindex") > 0);
  std::vector<std::string> ignores;
  if (vm.count("ignore"))
    ignores = vm["ignore"].as<std::vector<std::string> >();
  if (ignores.size())
  {
    ncdiff.ignore(ignores);
  }
  if (vm.count("limit"))
  {
    int limit = vm["limit"].as<int>();
    ncdiff.setReportLimit(limit);
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
    // Typically IO errors.
    cerr << "Error: " << error.what() << std::endl;
    exit(1);
  }
  catch (const boost::program_options::error& error)
  {
    // Error parsing command-line options.
    cerr << error.what() << std::endl;
    cerr << "Use option --help to see usage info." << std::endl;
    exit(1);
  }
}
    

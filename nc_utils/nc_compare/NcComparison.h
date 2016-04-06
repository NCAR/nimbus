// -*- C++ -*-

#ifndef _NcComparison_h_
#define _NcComparison_h_

#include <string>
#include "NcCache.h"
#include <stdexcept>
#include <list>
#include "compare_numbers.h"

class CompareNetcdfException : std::runtime_error
{
public:
  CompareNetcdfException(const std::string& what):
    std::runtime_error(what)
  {}

};


class Comparison;
class CompareNetcdf;

/**
 * Compare two components of a netcdf file.  All comparisons have a basic
 * result: added, deleted, equal, or different, depending upon whether the
 * same named component exists in the left file, right file, or both.
 * Subclasses can generate different types of reports based on what is
 * being compared, such as dimensions, variables, or attributes.
 *
 * The base class stores a reference to the CompareNetcdf, which in turn
 * contains the left and right NcCache instances in case a comparison needs
 * to access the files directly.
 *
 * Comparisons go through three stages: Construction initializes the
 * references to the two objects being compared, where one of the objects
 * can be null.  The compare() method runs the comparison, saves the
 * result, and computes any related artifacts.  The text summary of the
 * comparison can be generated with report().  Report generators should not
 * generate or update any differences, only report the artifacts already
 * computed.
 **/
class Comparison
{
public:
  
  typedef enum Result
  {
    Unknown, Equal, Added, Deleted, Different
  }
  Result;

  Comparison(CompareNetcdf* ncf);

  /**
   * Compute the differences between the two objects and cache the result.
   **/
  Result
  compare();

  /**
   * Return the result of the last compare().
   **/
  Result
  getResult()
  {
    return _result;
  }

  /**
   * Return true when the sides are equal.
   **/
  bool
  isEqual()
  {
    return _result == Equal;
  }

  /**
   * Return true when the sides are not equal.
   **/
  bool
  isDifferent()
  {
    return _result != Equal;
  }

  /**
   * Generate a comparison report for the two objects.  Use +/- to prefix
   * object summaries which have been added or removed, or use an empty
   * prefix for objects which are identical.
   **/
  virtual std::ostream&
  generateReport(std::ostream& out);

  /**
   * Return a pointer to the left object.  Subclasses implement this to
   * return a nc_object pointer to the specialized instance in the
   * subclass.
   **/
  virtual nc_object*
  getLeft() = 0;

  virtual nc_object*
  getRight() = 0;

protected:

  /**
   * Subclasses implement this to compute all the comparison artifacts and
   * return the basic comparison result.  The default implementation just
   * compares the text summaries of the two sides.
   **/
  virtual Result
  computeDifferences();

  /**
   * Convenience method for subclasses to create a result by first checking
   * whether the object on either side is empty, meaning the result is
   * Added or Deleted.  If an object exists on both sides, then return
   * Unknown.
   **/
  Result
  compareObjects();

  CompareNetcdf* _ncf;
  Result _result;
};


/**
 * A Comparison subclass which compares some aspect of the two files and
 * not specific objects, therefore it does not have left and right objects.
 **/
class FileComparison : public Comparison
{
public:
  FileComparison(CompareNetcdf* ncf) :
    Comparison(ncf)
  {}

  virtual nc_object*
  getLeft()
  {
    return 0;
  }

  virtual nc_object*
  getRight()
  {
    return 0;
  }

  virtual std::ostream&
  generateReport(std::ostream& out)
  {
    throw CompareNetcdfException("FileComparison does not implement "
				 "generateReport()");
  }

protected:
  virtual Result
  computeDifferences()
  {
    return Comparison::Unknown;
  }

};


/**
 * Compare time arrays between the two files.
 **/
class CompareTimes : public FileComparison
{
public:
  CompareTimes(CompareNetcdf* ncf);

  virtual std::ostream&
  generateReport(std::ostream& out);

protected:
  virtual Result
  computeDifferences();

  nc_time leftbegin;
  nc_time leftend;

  nc_time rightbegin;
  nc_time rightend;

  /// Number of times which are in common between the two files.
  int noverlap;

  /// Percentage of times in common out of union of all times.
  float percent_overlap;

  /// Number of times on left side which are unique.
  int nuniqueleft;

  /// Number of times on right side which are unique.
  int nuniqueright;

};


template <typename T>
class CompareObjects : public Comparison
{
public:
  virtual nc_object*
  getLeft()
  {
    return left;
  }

  virtual nc_object*
  getRight()
  {
    return right;
  }

protected:
  CompareObjects(CompareNetcdf* ncf, T* left_, T* right_):
    Comparison(ncf),
    left(left_),
    right(right_)
  {}

  T* left;
  T* right;
};



class CompareDimensions : public CompareObjects<nc_dimension>
{
public:
  CompareDimensions(CompareNetcdf* ncf,
		    nc_dimension* left, nc_dimension* right);

  virtual Result
  computeDifferences();
};


class CompareAttributes : public CompareObjects<nc_attribute>
{
public:
  CompareAttributes(CompareNetcdf* ncf, nc_attribute* left, nc_attribute* right);

  virtual Comparison::Result
  computeDifferences();

  virtual std::ostream&
  generateReport(std::ostream& out);

  using typename Comparison::Result;
  using CompareObjects<nc_attribute>::left;
  using CompareObjects<nc_attribute>::right;

  // These would only get used for numeric attributes.
  double absolute_error;
  double relative_error;
};


class CompareVariables : public CompareObjects<nc_variable>
{
public:
  CompareVariables(CompareNetcdf* ncf, nc_variable* left, nc_variable* right);

  virtual Comparison::Result
  computeDifferences();

  /** 
   * If the variables differ by type, dimensions, name, or attributes, then
   * report the different definitions.  The statistical differences are
   * reported in reportStatistics().
   **/
  virtual std::ostream&
  generateReport(std::ostream& out);

  std::ostream&
  statisticsHeader(std::ostream& out);

  std::ostream&
  reportStatistics(std::ostream& out);

  bool
  meansNearEqual();

  using typename Comparison::Result;
  using CompareObjects<nc_variable>::left;
  using CompareObjects<nc_variable>::right;

  double absolute_error;
  double relative_error;
  std::vector< boost::shared_ptr<CompareAttributes> > atts;
  bool dimsequal;
  variable_ranges ranges;
};



class CompareNetcdf
{
public:
  CompareNetcdf(NcCache* left, NcCache* right);

  void
  compare();

  std::ostream&
  report(std::ostream& out);

  inline NcCache*
  getLeft()
  {
    return _left;
  }

  inline NcCache*
  getRight()
  {
    return _right;
  }

  void
  showEqual(bool flag)
  {
    _show_equal = flag;
  }

  bool
  getShowEqual()
  {
    return _show_equal;
  }

  void
  showIndex(bool flag)
  {
    _show_index = flag;
  }

  bool
  getShowIndex()
  {
    return _show_index;
  }

  void
  setReportLimit(int limit)
  {
    _report_limit = limit;
  }

  int
  getReportLimit()
  {
    return _report_limit;
  }

  void
  ignore(const std::vector<std::string>& ignores)
  {
    _ignores = ignores;
  }

  /**
   * This is just a convenient wrapper to the compare_floating_point
   * template method.
   **/
  template <typename T>
  inline bool
  near_equal(const T& left, const T& right)
  {
    return _comparator.near_equal(left, right);
  }

  void
  setFloatComparator(const compare_floating_point& cfp)
  {
    _comparator = cfp;
  }

  int
  countDifferences();

  bool
  isIgnored(const std::string& name);

  void
  addWarning(const std::string, const std::string& msg)
  {
    _warnings.push_back(msg);
  }

  std::vector<std::string>
  getWarnings()
  {
    return _warnings;
  }

  static int DEFAULT_REPORT_LIMIT;

private:

  void
  compareVariables();

  void
  compareDimensions();

  void
  compareAttributes();

  NcCache* _left;
  NcCache* _right;
  bool _show_equal;
  bool _show_index;
  int _report_limit;
  std::vector<std::string> _ignores;

  std::vector< boost::shared_ptr<CompareDimensions> > dims;
  std::vector< boost::shared_ptr<CompareVariables> > vars;

  // Global attribute comparisons.
  std::vector< boost::shared_ptr<CompareAttributes> > atts;

  CompareTimes times;

  std::vector<std::string> _warnings;

  compare_floating_point _comparator;
};




#endif // _NcComparison_h_

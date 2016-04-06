

#include "NcComparison.h"

#include <iostream>
#include <boost/bind.hpp>
#include <boost/format.hpp>

#include "compare_lists.h"

using boost::bind;
using boost::mem_fn;
using boost::format;
using boost::shared_ptr;
using std::cout;
using std::cerr;

int CompareNetcdf::DEFAULT_REPORT_LIMIT = 10;


CompareNetcdf::
CompareNetcdf(NcCache* left, NcCache* right):
  _left(left),
  _right(right),
  _show_equal(false),
  _show_index(false),
  _report_limit(DEFAULT_REPORT_LIMIT),
  times(this)
{
  const char* default_ignores[] =
    { "date_created", "ProcessorURL", "ProcessorRevision", "NIDASrevision" };
  std::vector<std::string> ignores(default_ignores, default_ignores+4);
  ignore(ignores);
}


CompareTimes::
CompareTimes(CompareNetcdf* ncf):
  FileComparison(ncf)
{}


std::ostream&
CompareTimes::
generateReport(std::ostream& out, const ReportStyle& style)
{
  nc_time epoch = nc_time_from_time_t(0);
  out << "Time comparison:\n";
  if (leftbegin == epoch || rightbegin == epoch)
  {
    out <<
      " *** At least one file has a base_time of 0, this implies asc2cdf was\n"
      "     used without '-b' option.  Merged data may be shifted in time.\n";

    if (leftbegin == epoch && rightbegin == epoch)
    {
      out <<
	" *** Both base_time's are 0, files must have identical start\n"
	"     times or else merged data will be shifted in time.\n";
    }
  }

  if (rightend < leftbegin || rightbegin > leftend)
  {
    out <<
      " *** Time periods do not overlap at all!\n"
      "     Data comparison may be invalid.\n";
  }

  out << style
      << "Left file time period: " << leftbegin << " - " << leftend << "\n";
  out << style
      << "Right file time period: " << rightbegin << " - " << rightend << "\n";
  out << style
      << "Number of times in common: " << noverlap << "\n";
  out << style
      << "Number of unique times in left file: " << nuniqueleft << "\n";
  out << style
      << "Number of unique times in right file: " << nuniqueright << "\n";
  out << style
      << format("Percentage of overlap: %3.1f%%\n") % percent_overlap;
  return out;
}



Comparison::Result
CompareTimes::
computeDifferences()
{
  NcCache* left = _ncf->getLeft();
  NcCache* right = _ncf->getRight();
  
  std::vector<nc_time>& timesleft = left->times();
  std::vector<nc_time>& timesright = right->times();

  if (timesleft.size())
  {
    leftbegin = timesleft[0];
    leftend = timesleft[timesleft.size()-1];
  }
  if (timesright.size())
  {
    rightbegin = timesright[0];
    rightend = timesright[timesright.size()-1];
  }

  noverlap = 0;
  nuniqueleft = 0;
  nuniqueright = 0;

  unsigned int ileft = 0;
  unsigned int iright = 0;
  while (ileft < timesleft.size() || iright < timesright.size())
  {
    if (ileft < timesleft.size())
    {
      if (iright < timesright.size())
      {
	if (timesleft[ileft] == timesright[iright])
	{
	  ++noverlap;
	  ++ileft;
	  ++iright;
	}
	else if (timesleft[ileft] < timesright[iright])
	{
	  ++nuniqueleft;
	  ++ileft;
	}
	else
	{
	  ++nuniqueright;
	  ++iright;
	}
      }
      else
      {
	++nuniqueleft;
	++ileft;
      }
    }
    else
    {
      ++nuniqueright;
      ++iright;
    }
  }
  int ntotal = noverlap + nuniqueleft + nuniqueright;
  if (ntotal)
  {
    percent_overlap = 100.0 * float(noverlap) / float(ntotal);
  }

  if (ntotal == nuniqueright)
  {
    return Comparison::Added;
  }
  if (ntotal == nuniqueleft)
  {
    return Comparison::Deleted;
  }
  return (ntotal == noverlap ? Comparison::Equal : Comparison::Different);
}


template <typename L, typename T>
void
run_comparisons(CompareNetcdf* ncf, L& left, L& right, T& comps)
{
  comps.clear();
  compare_lists(ncf, left, right, comps);
  for_each(comps.begin(), comps.end(),
	   bind(mem_fn(&T::value_type::element_type::compare), _1));
}


void
CompareNetcdf::
compareAttributes()
{
  run_comparisons(this, _left->global_attributes, _right->global_attributes, atts);
}


void
CompareNetcdf::
compareDimensions()
{
  run_comparisons(this, _left->dimensions, _right->dimensions, dims);
}


bool
compare_relative_error(shared_ptr<CompareVariables> x,
		       shared_ptr<CompareVariables> y)
{
  return x->relative_error > y->relative_error;
}


void
CompareNetcdf::
compareVariables()
{
  run_comparisons(this, _left->variables, _right->variables, vars);
}


std::ostream&
CompareNetcdf::
report(std::ostream& out)
{
  const ReportStyle& style = getLeft()->getStyle();
  if (_show_equal || countDifferences())
  {
    out << "--- " << _left->getPath() << " (left)\n";
    out << "+++ " << _right->getPath() << " (right)\n"; 
  }
  if (_show_equal || times.isDifferent())
  {
    times.generateReport(out, style.derive(1));
  }
  bool header = false;
  for (unsigned int i = 0; i < dims.size(); ++i)
  {
    if (_show_equal || dims[i]->getResult() != Comparison::Equal)
    {
      if (!header)
      {
	header = true;
	out << "dimensions:\n";
      }
      dims[i]->generateReport(out, style.derive(1));
    }
  }
  header = false;
  for (unsigned int i = 0; i < atts.size(); ++i)
  {
    if (_show_equal || atts[i]->getResult() != Comparison::Equal)
    {
      if (!header)
      {
	cout << "global attributes:\n";
	header = true;
      }
      atts[i]->generateReport(out, style.derive(1));
    }
  }
  header = false;
  for (unsigned int i = 0; i < vars.size(); ++i)
  {
    if (_show_equal || vars[i]->getResult() != Comparison::Equal)
    {
      if (!header)
      {
	cout << "variables:\n";
	header = true;
      }
      vars[i]->generateReport(out, style.derive(1));
    }
  }

  // Now report the variable statistics comparisons in order of relative
  // error.  Do not report a variable if the differences in the mean are
  // beneath the threshold, regardless of the comparison result.
  std::vector< boost::shared_ptr<CompareVariables> > evars(vars);
  std::sort(evars.begin(), evars.end(), compare_relative_error);

  header = false;
  for (unsigned int i = 0; i < evars.size(); ++i)
  {
    if (_show_equal || !evars[i]->meansNearEqual())
    {
      if (!header)
      {
	evars[i]->statisticsHeader(out);
	header = true;
      }
      evars[i]->reportStatistics(out);
    }
  }
  return out;
}


bool
CompareNetcdf::
isIgnored(const std::string& name)
{
  return std::find(_ignores.begin(), _ignores.end(), name) != _ignores.end();
}


void
CompareNetcdf::
compare()
{
  times.compare();
  compareDimensions();
  compareVariables();
  compareAttributes();
}

int
CompareNetcdf::
countDifferences()
{
  int count = 0;
  count = count_if(vars.begin(), vars.end(),
		   bind(mem_fn(&CompareVariables::isDifferent), _1));
  count += count_if(dims.begin(), dims.end(),
		    bind(mem_fn(&CompareDimensions::isDifferent), _1));
  count += _warnings.size();
  count += times.isDifferent();
  return count;
}


Comparison::
Comparison(CompareNetcdf* ncf) :
  _ncf(ncf)
{}


Comparison::Result
Comparison::
compare()
{
  _result = computeDifferences();
  return _result;
}


Comparison::Result
Comparison::
computeDifferences()
{
  if (getLeft())
  {
    if (getRight())
    {
      return (getLeft()->textSummary() == getRight()->textSummary()) ?
	Equal : Different;
    }
    return Deleted;
  }
  return Added;
}


std::ostream&
Comparison::
generateReport(std::ostream& out, const ReportStyle& style)
{
  Result result = getResult();
  std::string tleft;
  std::string tright;
  if (getLeft())
  {
    tleft = getLeft()->textSummary() + "\n";
  }
  if (getRight())
  {
    tright = getRight()->textSummary() + "\n";
  }
  // Kludge alert: If the text summaries are the same, even if the
  // comparison result is not Equal, then only print the summary once, on
  // the assumption that the difference is actually in some sub-component
  // that will be reported separately.  The exclamation point is on the
  // left just to offset it from the all the +/- in the second column.

  if (tleft == tright)
  {
    if (result == Equal)
    {
      out << style << tleft;
    }
    else
    {
      out << style.merge("!  ") << tleft;
    }
    return out;
  }
  switch (result)
  {
  case Added:
    out << style.merge(" + ") << tright;
    break;
  case Deleted:
    out << style.merge(" - ") << tleft;
    break;
  case Equal:
    out << style.merge("   ") << tleft;
    break;
  case Different:
    out << style.merge(" - ") << tleft;
    out << style.merge(" + ") << tright;
    break;
  case Unknown:
    out << style.merge(" ? ") << tleft;
    out << style.merge(" ? ") << tright;
    break;
  }
  return out;
}


/**
 * Simple algorithm to create a diff result based just on which sides the
 * key exists on.  If the key does not exist on one side, that key is
 * empty.
 **/
Comparison::Result
Comparison::
compareObjects()
{
  if (!getLeft())
  {
    return Added;
  }
  if (!getRight())
  {
    return Deleted;
  }
  return Unknown;
}


CompareDimensions::
CompareDimensions(CompareNetcdf* ncf,
		  nc_dimension* left_, nc_dimension* right_):
  CompareObjects(ncf, left_, right_)
{
}


Comparison::Result
CompareDimensions::
computeDifferences()
{
  Result result = compareObjects();
  if (result != Unknown)
  {
    return result;
  }
  if (*left == *right)
  {
    return Equal;
  }
  return Different;
}



CompareVariables::
CompareVariables(CompareNetcdf* ncf, nc_variable* left, nc_variable* right):
  CompareObjects<nc_variable>(ncf, left, right),
  absolute_error(0),
  relative_error(0),
  dimsequal(false)
{}


// We want to compare variables in their native type, so create a template
// which does that, and use a visitor interface to dispatch the comparison
// to the right template instantiation.

template <typename T>
variable_ranges
compare_variables(CompareNetcdf* ncf, nc_var<T>* left, nc_variable* _right)
{
  variable_ranges diffs;
  nc_var<T>* right = dynamic_cast<nc_var<T>*>(_right);
  coordinates coords = left->begin();
  bool inrange = false;
  variable_range range(coords);
  do
  {
    if (! ncf->near_equal(left->get(coords), right->get(coords)))
    {
      if (!inrange)
      {
	range.start = coords;
	inrange = true;
      }
      range.end = coords;
    }
    else if (inrange)
    {
      inrange = false;
      diffs.push_back(range);
    }
  }
  while (coords.next());
  return diffs;
}

class compare_visitor : public variable_visitor
{
public:
  compare_visitor(CompareNetcdf* ncf, nc_variable* right) :
    _ncf(ncf),
    _right(right)
  {}

  virtual void visit(nc_var<double>* left)
  {
    _ranges = compare_variables(_ncf, left, _right);
  };

  virtual void visit(nc_var<float>* left)
  {
    _ranges = compare_variables(_ncf, left, _right);
  };

  virtual void visit(nc_var<int>* left)
  {
    _ranges = compare_variables(_ncf, left, _right);
  };

  virtual void visit(nc_var<unsigned int>* left)
  {
    _ranges = compare_variables(_ncf, left, _right);
  };

  virtual void visit(nc_var<char>* left)
  {
    _ranges = compare_variables(_ncf, left, _right);
  };

  virtual void visit(nc_var<unsigned char>* left)
  {
    _ranges = compare_variables(_ncf, left, _right);
  };

  virtual void visit(nc_var<short>* left)
  {
    _ranges = compare_variables(_ncf, left, _right);
  };

  virtual void visit(nc_var<unsigned short>* left)
  {
    _ranges = compare_variables(_ncf, left, _right);
  };

  variable_ranges&
  get_ranges()
  {
    return _ranges;
  }

private:
  CompareNetcdf* _ncf;
  nc_variable* _right;
  variable_ranges _ranges;
};


Comparison::Result
CompareVariables::
computeDifferences()
{
  // Note that nothing here compares the types of the two variables, so it
  // is possible to compare a float with a double.
  if (left)
    left->computeStatistics();
  if (right)
    right->computeStatistics();

  Result result = Comparison::compareObjects();
  if (result != Comparison::Unknown)
  {
    return result;
  }

  // The dimensions should be the same too.
  dimsequal = (left->dimensions.size() == right->dimensions.size());
  for (unsigned int d = 0; dimsequal && d < left->dimensions.size(); ++d)
  {
    dimsequal = (*left->dimensions[d] == *right->dimensions[d]);
  }
  
  // Run element by element comparisons using the comparison visitor.
  if (dimsequal)
  {
    compare_visitor cv(this->_ncf, right);
    left->visit(&cv);
    ranges = cv.get_ranges();
  }

  // Compute statistical differences.
  absolute_error = left->getMean() - right->getMean();
  if (fabs(right->getMean()) > fabs(left->getMean()))
    relative_error = fabs(absolute_error / right->getMean());
  else
    relative_error = fabs(absolute_error / left->getMean());
  if (isnan(relative_error))
    relative_error = 1.0;
  relative_error *= 100.0;
  bool equal = meansNearEqual();

  // Check for attribute differences.
  run_comparisons(this->_ncf, left->attributes, right->attributes, atts);
  int ndiff = count_if(atts.begin(), atts.end(),
		       bind(mem_fn(&CompareAttributes::isDifferent), _1));
  equal = equal && (ndiff == 0);
  return equal ? Comparison::Equal : Comparison::Different;
}



std::ostream&
CompareVariables::
generateReport(std::ostream& out, const ReportStyle& style)
{
  bool header = false;
  if (!left || !right || left->textSummary() != right->textSummary())
  {
    header = true;
    Comparison::generateReport(out, style);
  }
  // Dump attribute differences next, if any.
  int ndiff = count_if(atts.begin(), atts.end(),
		       bind(mem_fn(&CompareAttributes::isDifferent), _1));
  if ((ndiff || !dimsequal ||
       (_ncf->getShowIndex() && ranges.size()) ||
       _ncf->getShowEqual()) && !header)
  {
    Comparison::generateReport(out, style);
  }
  if (_ncf->getShowIndex())
  {
    if (!dimsequal)
    {
      out << style << "[Dimensions differ, values not compared by index.]\n";
    }
    // The dimension differences are included in the header.  Now report
    // any ranges where variables differ, up to the limit.
    for (unsigned int i = 0;
	 i < ranges.size() && i < (unsigned int)_ncf->getReportLimit(); ++i)
    {
      out << style.derive(1, " - ") << left->rangeSummary(ranges[i]) << "\n";
      out << style.derive(1, " + ") << right->rangeSummary(ranges[i]) << "\n";
    }
  }

  for (unsigned int i = 0; i < atts.size(); ++i)
  {
    if (_ncf->getShowEqual() || atts[i]->getResult() != Comparison::Equal)
    {
      atts[i]->generateReport(out, style);
    }
  }
  return out;
}


std::ostream&
CompareVariables::
statisticsHeader(std::ostream& out)
{
  out << format("%2s %-16s %14s %14s %12s %12s\n")
    % "" % "Variable" % "Left Mean" % "Right Mean"
    % "Abs Error"
    % "Rel Err (%)";
  out << format("%2s %-16s %14s %14s %12s %12s\n")
    % "" % "--------" % "---------" % "----------"
    % "------------"
    % "------------";
  return out;
}


std::ostream&
CompareVariables::
reportStatistics(std::ostream& out)
{
  std::string name(left ? left->name : right->name);

  out << format("%2s %-16s %14s %14s %s")
    % (left ? (right ? "" : "-") : "+")
    % name
    % (left ? str(format("%14.6f") % left->getMean()) : "")
    % (right ? str(format("%14.6f") % right->getMean()) : "")
    % (left && right ? str(format("%12.8f %12.2f")
			   % absolute_error % relative_error) : "");
  out << "\n";
  return out;
}


bool
CompareVariables::
meansNearEqual()
{
  if (!left || !right)
  {
    return false;
  }
  return _ncf->near_equal(left->getMean(), right->getMean());
}



template <typename T>
bool
compare_attribute_values(CompareNetcdf* ncf,
			 nc_attribute* attleft,
			 nc_attribute* attright)
{
  nc_att<T>* left = dynamic_cast<nc_att<T>*>(attleft);
  nc_att<T>* right = dynamic_cast<nc_att<T>*>(attright);

  if (!left || !right)
    return false;
  const std::vector<T>& vleft = left->get_values();
  const std::vector<T>& vright = right->get_values();
  if (vleft.size() != vright.size())
    return false;
  bool result = true;
  for (unsigned int i = 0; result && i < vleft.size(); ++i)
  {
    result = ncf->near_equal(vleft[i], vright[i]);
  }
  return result;
}



CompareAttributes::
CompareAttributes(CompareNetcdf* ncf, nc_attribute* left, nc_attribute* right):
  CompareObjects<nc_attribute>(ncf, left, right)
{}


Comparison::Result
CompareAttributes::
computeDifferences()
{
  Result result = Comparison::compareObjects();
  if (result != Comparison::Unknown)
  {
    return result;
  }
  bool equal = true;
  equal = equal && (left->datatype == right->datatype);
  equal = equal && (left->len == right->len);
  if (equal)
  {
    nc_type datatype = left->datatype;
    if (datatype == NC_DOUBLE)
    {
      equal = compare_attribute_values<double>(_ncf, left, right);
    }
    else if (datatype == NC_FLOAT)
    {
      equal = compare_attribute_values<float>(_ncf, left, right);
    }
    else if (datatype == NC_INT)
    {
      equal = compare_attribute_values<int>(_ncf, left, right);
    }
    else if (datatype == NC_SHORT)
    {
      equal = compare_attribute_values<short>(_ncf, left, right);
    }
    else
    {
      equal = (left->as_string() == right->as_string());
    }
  }
  result = equal ? Comparison::Equal : Comparison::Different;
  return result;
}

std::ostream&
CompareAttributes::
generateReport(std::ostream& out, const ReportStyle& style)
{
  return CompareObjects<nc_attribute>::generateReport(out, style.derive(1));
}


// -*- C++ -*-

#ifndef _compare_lists_h_
#define _compare_lists_h_

// Given a type being compared, provide a template to convert that type to
// the corresponding comparison class.
template <typename T>
struct comparison_type;

template <>
struct comparison_type<nc_dimension>
{
  typedef CompareDimensions result;
};

template <>
struct comparison_type<nc_variable>
{
  typedef CompareVariables result;
};

template <>
struct comparison_type<nc_attribute>
{
  typedef CompareAttributes result;
};

template <typename T>
inline boost::shared_ptr<typename comparison_type<T>::result>
make_comparison(CompareNetcdf* ncf, T* left, T* right)
{
  return boost::make_shared<
    typename comparison_type<T>::result>(ncf, left, right);
}

/**
 * This template function is an algorithm to build up a list of comparisons
 * of two nc_object lists, a left and a right.  It works like a line
 * difference algorithm, except it matches up objects by name.
 **/
template <typename LIST, typename CLIST>
void
compare_lists(CompareNetcdf* ncf, LIST& left, LIST& right, CLIST& comps)
{
  typename LIST::iterator lit = left.begin();
  typename LIST::iterator rit = right.begin();
  typename LIST::value_type nullp;
  
  for (lit = left.begin(); lit != left.end(); ++lit)
  {
    std::string name = (*lit)->name;
    if (ncf->isIgnored(name))
    {
      continue;
    }
    for (rit = right.begin(); rit != right.end(); ++rit)
    {
      if ((*rit)->name == name)
      {
	comps.push_back(make_comparison(ncf, (*lit).get(), (*rit).get()));
	break;
      }
    }
    if (rit == right.end())
    {
      comps.push_back(make_comparison(ncf, (*lit).get(), nullp.get()));
    }
  }
  for (rit = right.begin(); rit != right.end(); ++rit)
  {
    std::string name = (*rit)->name;
    if (ncf->isIgnored(name))
    {
      continue;
    }
    for (lit = left.begin(); lit != left.end(); ++lit)
    {
      if ((*lit)->name == name)
      {
	break;
      }
    }
    if (lit == left.end())
    {
      comps.push_back(make_comparison(ncf, nullp.get(), (*rit).get()));
    }
  }
}


#endif // _compare_lists_h_

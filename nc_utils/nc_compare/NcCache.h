// -*- C++ -*-

#ifndef _NcCache_h_
#define _NcCache_h_


#include <string>
#include <vector>
#include <map>
#include <netcdf.h>
#include <boost/lexical_cast.hpp>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

typedef boost::posix_time::ptime nc_time;

inline nc_time
nc_time_from_time_t(time_t ttime)
{
  nc_time epoch = nc_time(boost::gregorian::date(1970, 1, 1));
  return epoch + boost::posix_time::time_duration(0, 0, ttime);
}


class NcCache;

/**
 * All netcdf objects have a key name and an integer ID, and if non-empty
 * then they are associated with the NcCache object from whence they
 * originated.  If the object has not been initialized, then it has no
 * name, the ID is negative, and empty() returns true.
 **/
struct nc_object
{
  nc_object() :
    ncc(0),
    name(),
    id(-1)
  {}
    
  nc_object(NcCache* ncc_in, const std::string& name_in, int id_in) :
    ncc(ncc_in),
    name(name_in),
    id(id_in)
  {}

  bool
  empty()
  {
    return name.empty();
  }

  virtual std::string
  textSummary() = 0;

  NcCache* ncc;
  std::string name;
  int id;
};

struct nc_dimension : public nc_object
{
  nc_dimension();

  nc_dimension(NcCache* ncc, const std::string& name_in,
	       int id_in, size_t len_in);

  virtual std::string
  textSummary();

  size_t len;
};
    


struct nc_attribute : public nc_object
{
  nc_attribute();
  
  nc_attribute(NcCache* ncc, const std::string& name, int varid);

  virtual std::string
  textSummary();

  virtual std::string
  as_string() = 0;

  virtual void
  load_values() = 0;

  nc_type datatype;
  size_t len;
};


template <typename T>
struct nc_att : public nc_attribute
{
  nc_att(NcCache* ncc, const std::string& name, int varid);

  void
  get_att(T* values);

  const std::vector<T>&
  get_values();

  void
  set_values(const std::vector<T>&);

  void
  load_values();

  virtual std::string
  as_string();

  std::vector<T> values;
};


template <>
struct nc_att<std::string> : public nc_attribute
{
  nc_att();

  nc_att(NcCache* ncc, const std::string& name, int varid);

  void
  load_values();

  virtual std::string
  as_string();

  std::string value;
};

typedef nc_att<std::string> nc_string_attribute;


typedef std::vector<unsigned int> coords_t;


class coordinates
{
public:
  coordinates(const std::vector<nc_dimension*>& dimensions) :
    dims(dimensions),
    coords(dims.size(), 0),
    index(0)
  {
    npoints = 1;
    for (unsigned int d = 0; d < dimensions.size(); ++d)
    {
      npoints *= dimensions[d]->len;
    }
  }
    
  inline bool
  next()
  {
    if (index < npoints)
    {
      ++index;
      return true;
    }
    return false;
  }

#ifdef notdef
  // This might be useful to step through a variable using the netcdf
  // coordinate interface, but for the moment we always have a flat copy of
  // the variable in memory and so the 1-d index is enough to traverse the
  // values.
  bool
  next()
  {
    unsigned int ndims = dims.size();
    int carry = 1;
    for (unsigned int d = ndims-1; d >= 0 && carry; --d)
    {
      unsigned int dlen = dimensions[d]->len;
      n[d] += carry;
      if (n[d] >= dlen)
      {
	carry = n[d] / dlen;
	n[d] = n[d] % dlen;
      }
    }
    return (!carry);
  }
#endif

  /**
   * Return the coordinate vector, possible converted from the index.
   **/
  coords_t
  as_vector() const
  {
    coords_t coords(dims.size(), 0);
    unsigned int factor = npoints;
    unsigned int i = index;
    for (unsigned int d = 0; d < dims.size(); ++d)
    {
      factor = factor / dims[d]->len;
      coords[d] = i / factor;
      i = i % factor;
    }
    return coords;
  }

  std::vector<nc_dimension*> dims;
  coords_t coords;
  unsigned int index;
  unsigned int npoints;
};


inline std::ostream&
operator<<(std::ostream& out, const coordinates& c)
{
  out << "[";
  coords_t cv = c.as_vector();
  for (unsigned int d = 0; d < c.dims.size(); ++d)
  {
    if (d)
      out << ",";
    out << c.dims[d]->name << "=" << cv[d];
  }
  out << "]";
  return out;
}


class variable_visitor;

struct nc_variable : public nc_object
{
  nc_variable();

  nc_variable(NcCache* ncc, const std::string& name, int id);

  virtual double
  getMean() = 0;

  virtual void
  loadValues() = 0;

  virtual void
  computeStatistics() = 0;

  virtual std::string
  textSummary() = 0;

  /**
   * Add a dimension to this variable and update the total number of points
   * (npoints) for this variable in this file.
   **/
  void
  addDimension(nc_dimension* dim);

  nc_attribute*
  getAttribute(const std::string& name);

  coordinates
  begin()
  {
    return coordinates(dimensions);
  }

  virtual void
  visit(variable_visitor*) = 0;

  // The variable owns its attributes because they are not shared with
  // anything else.  Dimensions belong to the file, but can be referenced
  // in multiple variables.
  std::vector<boost::shared_ptr<nc_attribute> > attributes;
  std::vector<nc_dimension*> dimensions;
  size_t npoints;
  nc_type datatype;
};


template <typename T>
struct nc_var : public nc_variable
{
public:
  nc_var();

  nc_var(NcCache* ncc, const std::string& name_in, int id_in);

  double
  getMean()
  {
    return mean;
  }

  void
  loadValues();

  void
  computeStatistics();

  virtual std::string
  textSummary();

  virtual void
  visit(variable_visitor*);

  inline T
  get(const coordinates& where)
  {
    return data[where.index];
  }

  boost::shared_array<T> data;
  T missing_value;
  T mean;
};


/**
 * Visitor interface for getting at native variable types.
 **/
class variable_visitor
{
public:
  virtual void visit(nc_var<double>*)
  {};

  virtual void visit(nc_var<float>*)
  {};

  virtual void visit(nc_var<int>*)
  {};

  virtual void visit(nc_var<unsigned int>*)
  {};

  virtual void visit(nc_var<char>*)
  {};

  virtual void visit(nc_var<unsigned char>*)
  {};

  virtual void visit(nc_var<short>*)
  {};

  virtual void visit(nc_var<unsigned short>*)
  {};

};


template <typename T>
void
nc_var<T>::visit(variable_visitor* visitor)
{
  visitor->visit(this);
}


inline bool
operator==(const nc_dimension& left, const nc_dimension& right)
{
  return (left.name == right.name && left.len == right.len);
}


template <class T>
inline T*
find_nc_object(std::vector<boost::shared_ptr<T> >& objects, 
	       const std::string& name)
{
  typename std::vector<boost::shared_ptr<T> >::iterator it;
  for (it = objects.begin(); it != objects.end(); ++it)
  {
    if ((*it)->name == name)
    {
      return (*it).get();
    }
  }
  return 0;
}



/**
 * Encapsulate netcdf file access and cache the metadata in more accessible
 * data structures.
 **/
class NcCache
{
public:

  NcCache(const std::string& path);

  ~NcCache();

  std::string
  getPath()
  {
    return _path;
  }

  void
  close();

  void
  loadDimensions();

  void
  loadVariables();

  void
  loadTimes();

  std::vector<nc_time>&
  times()
  {
    return _times;
  }

  void
  loadGlobalAttributes();

  nc_variable*
  getVariable(const std::string& name);

  nc_dimension*
  getDimension(const std::string& name);
  
  nc_attribute*
  getGlobalAttribute(const std::string& name);

  nc_time
  basetime()
  {
    return _basetime;
  }

  typedef std::vector< boost::shared_ptr<nc_dimension> > dimension_vector_t;
  dimension_vector_t dimensions;

  typedef std::vector< boost::shared_ptr<nc_variable> > variable_vector_t;
  variable_vector_t variables;

  typedef std::vector< boost::shared_ptr<nc_attribute> > attribute_vector_t;
  attribute_vector_t global_attributes;

  int ncid;

private:

  void
  loadAttributes(attribute_vector_t& atts, int varid);

  boost::shared_ptr<nc_attribute>
  makeAttribute(int ncid, int varid, int attnum);

  std::string _path;

  // basetime is taken from a base_time variable or from the time variable
  // units attribute.  All the values in the time variable are assumed to
  // be relative to the basetime, and assumed to be in units of seconds.
  nc_variable* _vtime;
  nc_time _basetime;
  std::vector<nc_time> _times;

};


template <typename T>
struct nc_datatype_traits
{
  static const char* name;
  static const nc_type datatype;
};

template <typename T>
inline std::string
nc_typename()
{
  return nc_datatype_traits<T>::name();
}

template <typename T>
inline nc_type
nc_datatype()
{
  return nc_datatype_traits<T>::datatype;
}

#define DEFINE_TYPENAME(T, NAME, DATATYPE)\
  template <> struct nc_datatype_traits<T> {	\
    static inline std::string name() { return #NAME; }	\
    static const nc_type datatype = DATATYPE; \
  };

DEFINE_TYPENAME(double,double,NC_DOUBLE)
DEFINE_TYPENAME(float,float,NC_FLOAT)
DEFINE_TYPENAME(int,int,NC_INT)
DEFINE_TYPENAME(unsigned int,uint,NC_UINT)
DEFINE_TYPENAME(short,short,NC_SHORT)
DEFINE_TYPENAME(unsigned short,ushort,NC_USHORT)
DEFINE_TYPENAME(char,char,NC_CHAR)
DEFINE_TYPENAME(unsigned char,uchar,NC_BYTE)



#endif // _NcCache_h_


#include "NcCache.h"

#include <time.h>
#include <string.h>
#include <iostream>

using std::string;

#include <stdexcept>
using std::runtime_error;

using boost::shared_array;
using boost::shared_ptr;
using boost::make_shared;
using boost::posix_time::time_duration;

#include "gsl.hh"

NcCache::
NcCache(const std::string& path) :
  ncid(-1),
  _path(path)
{
  if (nc_open(path.c_str(), NC_NOWRITE, &ncid) != NC_NOERR)
  {
    throw runtime_error(string("Cannot open: ") + path);
  }
    
  loadDimensions();
  loadGlobalAttributes();
  loadVariables();
  loadTimes();
}



nc_time
basetime_from_units(const std::string& units, 
		    const std::string& strptime_format)
{
  int year, month, day, hour, minute, second, tz_offset;

  // If given an explicit strptime format, use it.  Otherwise parse with
  // strptime and grab the individual fields from the struct tm.  The time
  // is assumed to be in UTC and thus tz_offset is ignored.
  //
  if (strptime_format.length())
  {
    struct tm tm1;
    strptime(units.c_str(), strptime_format.c_str(), &tm1);
    year = tm1.tm_year + 1900;
    month = tm1.tm_mon + 1;
    day = tm1.tm_mday;
    hour = tm1.tm_hour;
    minute = tm1.tm_min;
    second = tm1.tm_sec;
  }
  else
  {
    sscanf(units.c_str(), "seconds since %d-%d-%d %d:%d:%d %d",
	   &year, &month, &day, &hour, &minute, &second, &tz_offset);
  }
  // Sanity check.
  if (year == 0) year = 1970;
  if (month == 0) month = 1;
  if (day == 0) day = 1;
  if (hour < 0 || hour > 23) hour = 0;
  if (minute < 0 || minute > 59) minute = 0;
  if (second < 0 || second > 59) second = 0;

  return nc_time(boost::gregorian::date(year, month, day), 
		 boost::posix_time::time_duration(hour, minute, second));
}


void
NcCache::
loadTimes()
{
  _vtime = getVariable("Time");
  if (! _vtime)
    _vtime = getVariable("time");

  if (! _vtime)
  {
    throw runtime_error(_path + ": does not contain a time variable");
  }

  nc_attribute* units = _vtime->getAttribute("units");
  if (! units)
  {
    throw runtime_error(_path + ": time variable has no units");
  }
  std::string strptime_format;
  nc_attribute* att = _vtime->getAttribute("strptime_format");
  if (att)
  {
    strptime_format = att->as_string();
  }
  _basetime = basetime_from_units(units->as_string(), strptime_format);

  std::vector<double> dtimes(_vtime->npoints);

  // Now that we have a base time, fill the times vector.
  nc_get_var_double(ncid, _vtime->id, &(dtimes[0]));
  _times.resize(dtimes.size());
  for (unsigned int i = 0; i < dtimes.size(); ++i)
  {
    _times[i] = _basetime + time_duration(0, 0, dtimes[i]);
  }
}

NcCache::
~NcCache()
{
  close();
}

void
NcCache::
close()
{
  if (ncid >= 0)
  {
    nc_close(ncid);
  }
  ncid = -1;
}


void
NcCache::
loadGlobalAttributes()
{
  global_attributes.clear();
  loadAttributes(global_attributes, NC_GLOBAL);
}



shared_ptr<nc_attribute>
NcCache::
makeAttribute(int ncid, int varid, int attnum)
{
  char name[NC_MAX_NAME];
  nc_type datatype;
  shared_ptr<nc_attribute> vp;
  
  nc_inq_attname(ncid, varid, attnum, name);
  nc_inq_atttype(ncid, varid, name, &datatype);

  if (datatype == NC_DOUBLE)
  {
    vp = make_shared< nc_att<double> >(this, name, varid);
  }
  else if (datatype == NC_FLOAT)
  {
    vp = make_shared< nc_att<float> >(this, name, varid);
  }
  else if (datatype == NC_INT)
  {
    vp = make_shared< nc_att<int> >(this, name, varid);
  }
  else if (datatype == NC_SHORT)
  {
    vp = make_shared< nc_att<short> >(this, name, varid);
  }
  else if (datatype == NC_BYTE)
  {
    vp = make_shared< nc_att<char> >(this, name, varid);
  }
  else if (datatype == NC_CHAR)
  {
    vp = make_shared< nc_string_attribute >(this, name, varid);
  }
  else
  {
    std::cerr << "could not instantiate attribute for " << name
	      << ", type " << datatype << std::endl;
  }
  return vp;
}


void
NcCache::
loadAttributes(attribute_vector_t& atts, int varid)
{
  int natts;

  if (varid == NC_GLOBAL)
  {
    nc_inq(ncid, 0, 0, &natts, 0);
  }
  else
  {
    nc_inq_varnatts(ncid, varid, &natts);
  }
  for (int attnum = 0; attnum < natts; ++attnum)
  {
    shared_ptr<nc_attribute> vp;
    vp = makeAttribute(ncid, varid, attnum);
    if (vp.get())
    {
      vp->load_values();
      atts.push_back(vp);
    }
  }
}


void
NcCache::
loadDimensions()
{
  char name[NC_MAX_NAME];
  int ndims;
  size_t len;

  nc_inq_ndims(ncid, &ndims);
  for (int dimid = 0; dimid < ndims; ++dimid)
  {
    nc_inq_dim(ncid, dimid, name, &len);
    dimensions.push_back(make_shared<nc_dimension>(this, name, dimid, len));
  }
}


nc_dimension*
NcCache::
getDimension(const std::string& name)
{
  return find_nc_object(dimensions, name);
}


void
NcCache::
loadVariables()
{
  int varid;
  int nvars;

  nc_inq(ncid, 0, &nvars, 0, 0);
  for (varid = 0; varid < nvars; ++varid)
  {
    char name[NC_MAX_NAME];
    int ndims;
    int dimids[NC_MAX_DIMS];
    nc_type datatype;
    shared_ptr<nc_variable> vp;

    nc_inq_var(ncid, varid, name, &datatype, &ndims, dimids, 0);
    if (datatype == NC_DOUBLE)
    {
      vp = make_shared< nc_var<double> >(this, name, varid);
    }
    else if (datatype == NC_FLOAT)
    {
      vp = make_shared< nc_var<float> >(this, name, varid);
    }
    else if (datatype == NC_INT)
    {
      vp = make_shared< nc_var<int> >(this, name, varid);
    }
    else if (datatype == NC_SHORT)
    {
      vp = make_shared< nc_var<short> >(this, name, varid);
    }
    else if (datatype == NC_BYTE)
    {
      vp = make_shared< nc_var<char> >(this, name, varid);
    }
    if (vp.get())
    {
      for (int d = 0; d < ndims; ++d)
      {
	vp->addDimension(dimensions[dimids[d]].get());
      }
      loadAttributes(vp->attributes, varid);
      variables.push_back(vp);
    }
  }
}


nc_variable*
NcCache::
getVariable(const std::string& name)
{
  return find_nc_object(variables, name);
}

nc_attribute*
NcCache::
getGlobalAttribute(const std::string& name)
{
  return find_nc_object(global_attributes, name);
}



nc_dimension::
nc_dimension():
  len(0)
{}

nc_dimension::
nc_dimension(NcCache* ncc, const std::string& name_in,
	     int id_in, size_t len_in):
  nc_object(ncc, name_in, id_in),
  len(len_in)
{}

std::string
nc_dimension::
textSummary()
{
  std::ostringstream out;
  out << "        " << name << " = " << len << " ;";
  return out.str();
}


nc_variable::
nc_variable()
{}


nc_variable::
nc_variable(NcCache* ncc, const std::string& name, int id):
  nc_object(ncc, name, id),
  npoints(1)
{}


void
nc_variable::
addDimension(nc_dimension* dim)
{
  if (dimensions.empty())
  {
    npoints = 1;
  }
  dimensions.push_back(dim);
  npoints *= dim->len;
}


nc_attribute*
nc_variable::
getAttribute(const std::string& name)
{
  return find_nc_object(attributes, name);
}


template <typename T>
nc_var<T>::
nc_var()
{}

template <typename T>
nc_var<T>::
nc_var(NcCache* ncc, const std::string& name, int id):
  nc_variable(ncc, name, id),
  missing_value(0),
  mean(0)
{}


template <typename T>
std::string
nc_var<T>::
textSummary()
{
  std::ostringstream out;
  out << "        "
      << nc_typename<T>() << " "
      << name << "(";
  for (unsigned int d = 0; d < dimensions.size(); ++d)
  {
    if (d > 0)
      out << ", ";
    out << dimensions[d]->name;
  }
  out << ") ;";
  return out.str();
}


template <typename T>
void
nc_var<T>::
loadValues()
{
  char name[NC_MAX_NAME];
  nc_type datatype;

  int ncid = ncc->ncid;
  nc_inq_var(ncid, id, name, &datatype, 0, 0, 0); 

  npoints = 1;
  for (unsigned int d = 0; d < dimensions.size(); ++d)
  {
    npoints *= dimensions[d]->len;
  }

  data.reset(new T[npoints]);
  nc_get_var(ncid, id, data.get());
  nc_get_att(ncid, id, "_FillValue", &missing_value);
}



template <typename T>
void
nc_var<T>::
computeStatistics()
{
  if (!data.get())
  {
    loadValues();
  }
  std::vector<T> cleaned_data;
  for (size_t i = 0; i < npoints; ++i)
  {
    if (data[i] != missing_value)
      cleaned_data.push_back(data[i]);
  }
  mean = gsl::gsl_stats_mean(&cleaned_data[0], 1, cleaned_data.size());
}



/*
 * Explicit instantiations of nc_var for netcdf data types.
 */
template class nc_var<double>;
template class nc_var<float>;
template class nc_var<int>;
template class nc_var<unsigned int>;
template class nc_var<char>;
template class nc_var<unsigned char>;
template class nc_var<short>;
template class nc_var<unsigned short>;


nc_attribute::
nc_attribute() :
  datatype(0),
  len(0)
{}

nc_attribute::
nc_attribute(NcCache* ncc, const std::string& name, int varid) :
  nc_object(ncc, name, varid),
  datatype(0),
  len(0)
{
}

std::string
nc_attribute::
textSummary()
{
  std::ostringstream out;
  std::string quote;
  if (datatype == NC_CHAR)
    quote = "\"";
  out << "                "
      << name << " = " << quote << as_string() << quote << " ;";
  return out.str();
}


template <typename T>
nc_att<T>::
nc_att(NcCache* ncc, const std::string& name, int varid):
  nc_attribute(ncc, name, varid)
{
  datatype = nc_datatype<T>();
}


template <typename T>
const std::vector<T>&
nc_att<T>::
get_values()
{
  return values;
}

template <typename T>
void
nc_att<T>::
set_values(const std::vector<T>& invalues)
{
  len = invalues.size();
  values = invalues;
}

template <typename T>
void
nc_att<T>::
load_values()
{
  nc_inq_att(ncc->ncid, id, name.c_str(), &datatype, &len);
  values.resize(len);
  get_att(&(values[0]));
}

template <typename T>
std::string
nc_att<T>::
as_string()
{
  std::string text;
  for (unsigned int i = 0; i < values.size(); ++i)
  {
    if (i > 0)
    {
      text += ", ";
    }
    text += boost::lexical_cast<std::string>(values[i]);
  }
  return text;
}


nc_att<std::string>::
nc_att() :
  nc_attribute()
{}


nc_att<std::string>::
nc_att(NcCache* ncc, const std::string& name, int varid):
  nc_attribute(ncc, name, varid)
{}


void
nc_att<std::string>::
load_values()
{
  nc_inq_att(ncc->ncid, id, name.c_str(), &datatype, &len);
  boost::shared_array<char> buf(new char[len+1]);
  nc_get_att_text(ncc->ncid, id, name.c_str(), buf.get());
  buf[len] = '\0';
  value = std::string(buf.get());
}


std::string
nc_att<std::string>::
as_string()
{
  return value;
}


template <>
inline void
nc_att<double>::
get_att(double* values)
{
  nc_get_att_double(ncc->ncid, id, name.c_str(), values);
}


template <>
inline void
nc_att<float>::
get_att(float* values)
{
  nc_get_att_float(ncc->ncid, id, name.c_str(), values);
}


template <>
inline void
nc_att<int>::
get_att(int* values)
{
  nc_get_att_int(ncc->ncid, id, name.c_str(), values);
}


template <>
inline void
nc_att<short>::
get_att(short* values)
{
  nc_get_att_short(ncc->ncid, id, name.c_str(), values);
}

template <>
inline void
nc_att<char>::
get_att(char* values)
{
  nc_get_att_text(ncc->ncid, id, name.c_str(), values);
}


template class nc_att<double>;
template class nc_att<float>;
template class nc_att<int>;
template class nc_att<short>;
template class nc_att<std::string>;

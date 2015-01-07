
#include <boost/python.hpp>

using namespace boost::python;

#include "vardb.hh"

namespace
{
  void (VDBFile::*save_filename)(const std::string&) = &VDBFile::save;
  VDBVar* (VDBFile::*get_var_string)(const std::string&) = &VDBFile::get_var;

  std::string (VDBVar::*get_attribute)
  (const std::string&, const std::string&) const = &VDBVar::get_attribute;

  BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(get_attribute_overloads, 
					 get_attribute, 1, 2);

  template<class T>
  list std_vector_to_py_list(const std::vector<T>& v)
  {
    list result;
    for (typename std::vector<T>::const_iterator it = v.begin();
	 it != v.end(); ++it)
    {
      result.append(*it);
    }
    return result;
  }

  list convert_standard_names(VDBFile* vdb)
  {
    return std_vector_to_py_list(vdb->get_standard_names());
  }

  list convert_categories(VDBFile* vdb)
  {
    return std_vector_to_py_list(vdb->get_categories());
  }

}


BOOST_PYTHON_MODULE(_vardb)
{
  object vdbvar = class_<VDBVar>("VDBVar", no_init)
    .def("name", &VDBVar::name)
    .def("get_attribute", get_attribute, get_attribute_overloads())
    ;

  vdbvar.attr("UNITS") = VDBVar::UNITS;
  vdbvar.attr("LONG_NAME") = VDBVar::LONG_NAME;
  vdbvar.attr("IS_ANALOG") = VDBVar::IS_ANALOG;
  vdbvar.attr("VOLTAGE_RANGE") = VDBVar::VOLTAGE_RANGE;
  vdbvar.attr("DEFAULT_SAMPLE_RATE") = VDBVar::DEFAULT_SAMPLE_RATE;
  vdbvar.attr("MIN_LIMIT") = VDBVar::MIN_LIMIT;
  vdbvar.attr("MAX_LIMIT") = VDBVar::MAX_LIMIT;
  vdbvar.attr("CATEGORY") = VDBVar::CATEGORY;
  vdbvar.attr("MODULUS_RANGE") = VDBVar::MODULUS_RANGE;
  vdbvar.attr("DERIVE") = VDBVar::DERIVE;
  vdbvar.attr("DEPENDENCIES") = VDBVar::DEPENDENCIES;
  vdbvar.attr("STANDARD_NAME") = VDBVar::STANDARD_NAME;
  vdbvar.attr("REFERENCE") = VDBVar::REFERENCE;

  class_<VDBFile>("VDBFile", init<const std::string&>())
    .def("open", &VDBFile::open)
    .def("close", &VDBFile::close)
    .def("load", &VDBFile::load)
    .def("save", save_filename)
    .def("is_valid", &VDBFile::is_valid)
    .def("get_var", get_var_string,
	 return_internal_reference<>())
    .def("num_vars", &VDBFile::num_vars)
    .def("get_standard_names", convert_standard_names)
    .def("get_categories", convert_categories)
    ;
}

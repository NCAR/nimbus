
#include <boost/python.hpp>

using namespace boost::python;

#include "acDatabase.hh"
#include "ncTrack.hh"
#include "ProjectInfo.hh"
#include "Config.hh"
#include "AircraftTrack.hh"
#include "AircraftTrackKML.hh"
#include "osm.hh"
#include "ACTrackDriver.hh"

namespace
{
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

  list lat(AircraftTrack* track)
  {
    return std_vector_to_py_list(track->lat);
  }

  list lon(AircraftTrack* track)
  {
    return std_vector_to_py_list(track->lon);
  }

  list thdg(AircraftTrack* track)
  {
    return std_vector_to_py_list(track->thdg);
  }

}


BOOST_PYTHON_MODULE(_actrack)
{
  object config = class_<Config>("Config")
    .def("dump", &Config::dump);

  object track = class_<AircraftTrack>("AircraftTrack")
    .def("ok", &AircraftTrack::ok)
    .def("updated", &AircraftTrack::updated)
    .def("clearStatus", &AircraftTrack::clearStatus)
    .def("clear", &AircraftTrack::clear)
    .def("npoints", &AircraftTrack::npoints)
    .def("isMissingValue", &AircraftTrack::isMissingValue)
    .def("lat", lat)
    .def("lon", lon)
    .def("thdg", thdg);

  object driver = class_<ACTrackDriver>("ACTrackDriver")
    .def("loadTrackNetCDF", &ACTrackDriver::loadTrackNetCDF)
    .def("getTrack", &ACTrackDriver::getTrack,
	 return_internal_reference<>());

  object osm = class_<OSM>("OSM")
    .def("formatPositionJSON", &OSM::formatPositionJSON);

}


#ifndef __angle_hh__
#define __angle_hh__

#include <math.h>
#include <boost/lexical_cast.hpp>


template <template <typename T> class UNITS, typename T>
struct angle_units
{
  typedef T value_type;
  typedef UNITS<T> units_type;

  static void
  normalize(value_type& value)
  {
    while (value >= units_type::origin + units_type::circle)
    {
      value -= units_type::circle;
    }
    while (value < units_type::origin)
    {
      value += units_type::circle;
    }
  }

  /**
   * Template method to normalize a range of angles
   * in these units.
   **/
  template <typename IT>
  static void
  normalize(IT begin, IT end)
  {
    for (IT it = begin; it != end; ++it)
    {
      normalize(*it);
    }
  }

};


template <typename T>
struct radians : public angle_units< radians, T >
{
  static const T circle = 2*M_PI;
  static const T origin = 0;
};

template <typename T>
struct heading_degrees : public angle_units< heading_degrees, T >
{
  typedef T value_type;
  static const T circle = 360;
  static const T origin = 0;
};

template <typename T>
struct longitude_degrees : public angle_units< longitude_degrees, T >
{
  typedef T value_type;
  static const T circle = 360;
  static const T origin = -180;
};


template <typename T>
struct longitude_positive_degrees : 
  public angle_units< longitude_positive_degrees, T >
{
  typedef T value_type;
  static const T circle = 360;
  static const T origin = 0;
};



template <typename T, typename UNITS=heading_degrees<T> >
class angle
{
public:
  typedef T value_type;
  typedef UNITS units_type;

  angle(const T& value = T(0))
  {
    _value = value;
    normalize();
  }

  void
  normalize()
  {
    units_type::normalize(_value);
  }

  T
  value() const
  {
    return _value;
  }

  operator T() const
  {
    return _value;
  }

  bool
  operator< (const angle& right) const
  {
    return bool(angle_difference(*this, right) >= 0);
  }

  bool
  operator== (const angle& right) const
  {
    return this->_value == right._value;
  }

  /**
   * Formatting an angle, especially floating point, can round the formatted
   * value to a value outside the normalized range.  So to get a formatted
   * angle inside the normalized range, we format it first and then normalize
   * the formatted value.
   **/
  std::string
  format(const std::string& spec) const
  {
    char buf[64];
    snprintf(buf, sizeof(buf), spec.c_str(), value());
    angle<T,UNITS> a2(boost::lexical_cast<T>(buf));
    snprintf(buf, sizeof(buf), spec.c_str(), a2.value());
    return buf;
  }

  template <typename A>
  A
  convert() const
  {
    return A(((double)_value / units_type::circle) * A::units_type::circle);
  }

private:

  T _value;

};


typedef angle<double, heading_degrees<double> > heading_double;
typedef angle<float, heading_degrees<float> > heading_float;

template <typename T>
struct longitude : public angle<T, longitude_degrees<T> >
{
  typedef angle<T, longitude_degrees<T> > angle_t;
  longitude(const T& value = T(0)) : 
    angle_t(value)
  {}
};

typedef longitude<float> longitude_float;
typedef longitude<double> longitude_double;


/**
 * Return the smallest difference between two angles: @p a2 - @p a1.
 **/
template <typename T>
typename T::value_type
angle_difference(T a1, T a2)
{
  typename T::value_type adiff;
  adiff = a2.value() - a1.value();
  if (adiff > T::units_type::circle/2)
  {
      adiff -= T::units_type::circle;
  }
  else if (adiff < -(T::units_type::circle/2))
  {
      adiff += T::units_type::circle;
  }
  return adiff;
}


#endif // __angle_hh__

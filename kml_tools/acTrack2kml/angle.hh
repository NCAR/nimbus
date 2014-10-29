
#ifndef __angle_hh__
#define __angle_hh__

#include <math.h>

template <typename T>
struct radians
{
  static const T circle = 2*M_PI;
};


template <typename T>
struct degrees
{
  static const T circle = 360;
};


template <typename T, typename UNITS=degrees<T> >
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
    while (_value > UNITS::circle)
    {
      _value -= UNITS::circle;
    }
    while (_value < 0)
    {
      _value += UNITS::circle;
    }
  }

  T
  value()
  {
    return _value;
  }

  operator T()
  {
    return _value;
  }

  template <typename A>
  A
  convert()
  {
    return A(((double)_value / units_type::circle) * A::units_type::circle);
  }

private:

  T _value;

};


/**
 * Return the smallest angular difference between two angles in degrees.
 * The angles should already be in the interval 0 - 360.
 **/
template <typename T>
typename T::value_type
angle_difference(T a1, T a2)
{
  a1.normalize();
  a2.normalize();
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

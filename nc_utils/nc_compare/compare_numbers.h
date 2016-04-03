// -*- C++ -*-
#ifndef _compare_numbers_h_
#define _compare_numbers_h_

#include <math.h>

/**
 * Utilities for testing numeric values for equality, especially floating
 * point values which need to be tested for near equality.  The
 * compare_floating_point class parameterizes comparison methods and allows
 * alternate methods to be configured and selected.  For testing that
 * floating point values are near each other, regardless of magnitude,
 * there are implementations for each of two diferent approaches.
 *
 * The almost_equal_floats() and almost_equal_doubles() functions test that
 * two floats are within some number of Units in the Last Places, by
 * translating the IEEE bit representation into integers whose difference
 * is the number of representable floats between the two numbers.  The
 * implementation is taken from the page below, as referenced in the google
 * test documentation about comparing floats:
 * 
 * http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
 *
 * The other implementation, in almost_equal_float_nsteps() and
 * almost_equal_double_nsteps(), uses the nextafter() functions available
 * as of C99.  The nextafter() functions allow stepping through each
 * successive representable floating point value.  If two floats are not
 * equal after stepping from one to the other for a maximum number of
 * steps, then they are not close enough.
 *
 * Right now the nsteps functions are being used for the
 * compare_floating_point ULPS selection, since the implementation is much
 * more straightforward and probably more portable also.  However, no
 * performance testing has been done.
 **/


typedef union
{
  int asInt;
  float asFloat;
}
twos_complement_float_t;
  

inline bool
almost_equal_floats(float A, float B, int maxUlps=2)
{
  // Make sure maxUlps is non-negative and small enough that the
  // default NAN won't compare as equal to anything.
  twos_complement_float_t a;
  a.asFloat = A;
  int aInt = a.asInt;
  // Make aInt lexicographically ordered as a twos-complement int
  if (aInt < 0)
    aInt = 0x80000000 - aInt;
  // Make bInt lexicographically ordered as a twos-complement int
  twos_complement_float_t b;
  b.asFloat = B;
  int bInt = b.asInt;
  if (bInt < 0)
    bInt = 0x80000000 - bInt;
  int intDiff = aInt - bInt;
  if (intDiff < 0)
    intDiff = -intDiff;
  if (intDiff <= maxUlps)
    return true;
  return false;
}


typedef union
{
  long int asInt;
  double asDouble;
}
twos_complement_double_t;
  
inline bool
almost_equal_doubles(double A, double B, int maxUlps=2)
{
  twos_complement_double_t a;
  a.asDouble = A;
  long int aInt = a.asInt;
  // Make aInt lexicographically ordered as a twos-complement int
  if (aInt < 0)
    aInt = 0x8000000000000000 - aInt;
  // Make bInt lexicographically ordered as a twos-complement int
  twos_complement_double_t b;
  b.asDouble = B;
  long int bInt = b.asInt;
  if (bInt < 0)
    bInt = 0x8000000000000000 - bInt;

  long int intDiff = aInt - bInt;
  if (intDiff < 0)
    intDiff = -intDiff;
  if (intDiff <= maxUlps)
    return true;
  return false;
}


/**
 * The nsteps comparison functions use the nextafter*() functions added to
 * C99.  Once C++11 is more widely available, these can switch to using the
 * overloaded nextafter() functions. @p nsteps must be >= 0.
 *
 **/
inline bool
almost_equal_float_nsteps(float a, float b, int nsteps)
{
  int i = 0;
  while (i++ <= nsteps)
  {
    if (a == b)
      return true;
    a = nextafterf(a, b);
  }
  return false;
}

inline bool
almost_equal_double_nsteps(double a, double b, int nsteps)
{
  int i = 0;
  while (i++ <= nsteps)
  {
    if (a == b)
      return true;
    a = nextafter(a, b);
  }
  return false;
}



/**
 * Parameterize floating point comparisons.  This class provides alternate
 * ways to test two floating point numbers for equality, testing either
 * that their difference is within an absolute delta or that they are equal
 * within some number of Units in the Last Place (ULPs).  These objects
 * can be copied and constructed in place, so it is convenient to construct
 * and configure a floating point comparison like so:
 *
 * compare_floating_point().setDelta(1e-8).useDelta().near_equal(x, y);
 *
 **/
class compare_floating_point
{
public:
  /**
   * Construct the default floating point comparison object.  Floating
   * point values are equal if within 2 steps of each other.
   **/
  compare_floating_point() :
    _fdelta(1e-6),
    _ddelta(1e-10),
    _nulps(4)
  {
    useULPS();
  }

  /**
   * This template method is just a convenient way to compare any numeric
   * types, where the default implementation tests for exact equality.  The
   * template is specialized below for the float and double types to use
   * the inexact comparisons.
   **/
  template <typename T>
  inline bool
  near_equal(const T& left, const T& right)
  {
    return left == right;
  }

  compare_floating_point&
  useDelta()
  {
    _float_equal = &compare_floating_point::near_equal_delta_float;
    _double_equal = &compare_floating_point::near_equal_delta_double;
    return *this;
  }

  compare_floating_point&
  useULPS()
  {
    assert(sizeof(int) == sizeof(float));
    assert(sizeof(long int) == sizeof(double));
    _float_equal = &compare_floating_point::near_equal_ulps_float;
    _double_equal = &compare_floating_point::near_equal_ulps_double;
    return *this;
  }

  compare_floating_point&
  setDelta(double ddelta)
  {
    _ddelta = ddelta;
    _fdelta = ddelta;
    return *this;
  }

  compare_floating_point&
  setFloatDelta(float efloat)
  {
    _fdelta = efloat;
    return *this;
  }

  compare_floating_point&
  setDoubleDelta(double edouble)
  {
    _ddelta = edouble;
    return *this;
  }

  compare_floating_point&
  setULPS(int nulps)
  {
    _nulps = nulps;
    // This is a requirement for the 2's complement algorithm.
    //assert(_nulps > 0 && _nulps < 4 * 1024 * 1024);
    return *this;
  }

  bool
  near_equal_ulps_float(float left, float right)
  {
    // return almost_equal_floats(left, right, _nulps);
    return almost_equal_float_nsteps(left, right, _nulps);
  }

  bool
  near_equal_ulps_double(double left, double right)
  {
    // return almost_equal_doubles(left, right, _nulps);
    return almost_equal_double_nsteps(left, right, _nulps);
  }

  bool
  near_equal_delta_float(float left, float right)
  {
    return (left - right) < _fdelta && (left - right) > -_fdelta;
  }

  bool
  near_equal_delta_double(double left, double right)
  {
    return (left - right) < _ddelta && (left - right) > -_ddelta;
  }

private:

  float _fdelta;
  double _ddelta;

  int _nulps;

  bool (compare_floating_point::*_float_equal)(float left, float right);
  bool (compare_floating_point::*_double_equal)(double left, double right);

};


/**
 * Test two doubles for equality using the current comparison scheme
 * and parameters.
 **/
template <>
inline bool
compare_floating_point::
near_equal(const double& left, const double& right)
{
  return (this->*_double_equal)(left, right);
}
    
/**
 * Test two floats for equality using the current comparison scheme and
 * parameters.
 **/
template <>
inline bool
compare_floating_point::
near_equal(const float& left, const float& right)
{
  return (this->*_float_equal)(left, right);
}
    
#endif // _compare_numbers_h_


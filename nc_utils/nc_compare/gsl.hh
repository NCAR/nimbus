
#ifndef _gsl_hh_
#define _gsl_hh_

#include <gsl/gsl_statistics.h>
#include <gsl/gsl_statistics_float.h>
#include <gsl/gsl_statistics_int.h>
#include <gsl/gsl_statistics_uint.h>
#include <gsl/gsl_statistics_char.h>
#include <gsl/gsl_statistics_uchar.h>


/**
 * The gsl namespace contains C++ template methods parameterized by the
 * data type.  Specializations of the templates call the corresponding
 * type-specific gsl function.
 **/
namespace gsl {

  template <typename T>
  T
  gsl_stats_mean(const T data[], const size_t stride, const size_t n);

  /* Use some CPP magic to define the specializations.
   */
#define GSL_STATS(T, ID) \
template <> T \
gsl_stats_mean(const T data[], const size_t stride, const size_t n) { \
  return ::gsl_stats_##ID##mean(data, stride, n);  }

  GSL_STATS(double,)
  GSL_STATS(float,float_)
  GSL_STATS(int,int_)
  GSL_STATS(unsigned int,uint_)
  GSL_STATS(short,short_)
  GSL_STATS(unsigned short,ushort_)
  GSL_STATS(char,char_)
  GSL_STATS(unsigned char,uchar_)
}



#endif // _gsl_hh_

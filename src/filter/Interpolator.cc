// -*- mode: C++; c-basic-offset: 2; -*-

#include "Interpolator.h"

#include <math.h>
#include <gsl/gsl_spline.h>
#include <sstream>

using std::ostringstream;
using std::string;

typedef Interpolator::InterpolationType InterpolationType;

struct Interpolator_pt
{
  Interpolator_pt()
  {
    acc = 0;
    linear = 0;
    spline = 0;
    x = 0;
    y = 0;
    npoints = 0;
    itype = Interpolator::Linear;
    error = false;
    extrapolate = false;
  }

  ~Interpolator_pt()
  {
    release();
  }

  void
  release()
  {
    if (linear)
      gsl_interp_free(linear);
    if (spline)
      gsl_spline_free(spline);
    if (acc)
      gsl_interp_accel_free(acc);
  }

  gsl_interp_accel *acc;
  gsl_interp *linear;
  gsl_spline *spline;
  double* x;
  double* y;
  unsigned int npoints;
  InterpolationType itype;
  bool error;
  bool extrapolate;
};


Interpolator::
~Interpolator()
{
  delete _p;
}


Interpolator::
Interpolator() :
  _p(new Interpolator_pt)
{
}


Interpolator::
Interpolator(InterpolationType itype, double* x, double* y, unsigned int npoints) :
  _p(new Interpolator_pt)
{
  setup(x, y, npoints, itype);
}


void
Interpolator::
setup(double* x, double* y, unsigned int npoints, InterpolationType itype)
{
  _p->release();
  _p->x = x;
  _p->y = y;
  _p->npoints = npoints;
  _p->itype = itype;

  if (itype == Interpolator::Linear)
  {
    _p->linear = gsl_interp_alloc(gsl_interp_linear, npoints);
    gsl_interp_init(_p->linear, x, y, npoints);
  }
  else if (itype == Interpolator::AkimaSpline && npoints >= 5)
  {
    _p->spline = gsl_spline_alloc(gsl_interp_akima, npoints);
    gsl_spline_init(_p->spline, x, y, npoints);
  }
  else
  {
    _p->spline = gsl_spline_alloc(gsl_interp_cspline, npoints);
    gsl_spline_init(_p->spline, x, y, npoints);
  }
  _p->acc = gsl_interp_accel_alloc();
}


double
Interpolator::
eval(double x)
{
  // Warn if the requested X coordinate is outside the domain, then
  // extrapolate.
  double y;
  int n = 0;
  _p->error = false;
  if (x < _p->x[0])
  {
    n = 1;
  }
  else if (x > _p->x[_p->npoints - 1])
  {
    n = _p->npoints - 1;
  }
  if (n)
  {
    _p->error = true;
    if (_p->extrapolate)
    {
      double*& ya = _p->y;
      double*& xa = _p->x;
      y = (ya[n] - ya[n-1])*(x - xa[n-1])/(xa[n] - xa[n-1]) + ya[n-1];
    }
    else
    {
      y = nanf("");
    }
  }
  else if (_p->itype == Linear)
  {
    y = gsl_interp_eval(_p->linear, _p->x, _p->y, x, _p->acc);
  }
  else
  {
    y = gsl_spline_eval(_p->spline, x, _p->acc);
  }
  return y;
}


static inline 
double&
index_relative(double* first, double* end, int i)
{
  if (i < 0)
  {
    return *(end+i);
  }
  return *(first+i);
}


double&
Interpolator::
x(int i)
{
  return index_relative(_p->x, _p->x+_p->npoints, i);
}

double&
Interpolator::
y(int i)
{
  return index_relative(_p->y, _p->y+_p->npoints, i);
}


void
Interpolator::
enableExtrapolation(bool enable)
{
  _p->extrapolate = enable;
}


bool
Interpolator::
outOfBounds()
{
  return _p->error;
}


std::string
Interpolator::
errorMessage(double x)
{
  ostringstream msg;
  msg << "Interpolating @ x=" << x << ", out of bounds [" 
      << _p->x[0] << "," << _p->x[_p->npoints-1] << "]";
  return msg.str();
}


bool
Interpolator::
isValid()
{
  return bool(_p && (_p->linear || _p->spline) && (_p->npoints > 0));
}


// -*- mode: C++; c-basic-offset: 4; -*-

#ifndef _nimbus_Interpolator_h_
#define _nimbus_Interpolator_h_

#include <string>

struct Interpolator_pt;

class Interpolator
{
public:

    enum InterpolationType { Linear=0, CubicSpline=1, AkimaSpline=2 };

    Interpolator();

    Interpolator(InterpolationType itype, double* x, double* y, 
		 unsigned int npoints);

    void
    setup(double* x, double* y, unsigned int npoints, 
	  InterpolationType itype = Linear);

    /**
     * Enable or disable extrapolation when evaluating points outside the
     * bounds of the X array.
     **/
    void
    enableExtrapolation(bool enable);

    /**
     * Evaluate @param x using the current interpolation.  Return NaN if
     * @param x is outside the bounds of the X array, unless extrapolation
     * is enabled through enableExtrapolation().
     **/
    double
    eval(double x);

    /**
     * Return true if the last eval() calls was outside the bounds of the X
     * array.
     **/
    bool
    outOfBounds();

    /**
     * Format a generic out of bounds message for @param x.  Typically
     * called after outOfBounds() returns true.
     **/
    std::string
    errorMessage(double x);

    double&
    x(int i);

    double&
    y(int i);

    bool
    isValid();

    ~Interpolator();

private:
    Interpolator_pt* _p;

};



#endif // _nimbus_Interpolator_h_

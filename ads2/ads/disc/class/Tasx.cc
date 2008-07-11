/* Tasx.cc

   Computes the aircraft true air speed from QCX, PSX, TTX.
   These are uncorrected values.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <Tasx.h>

/******************************************************************************
** Public Functions
******************************************************************************/

Tasx::Tasx (SyncVar& q, SyncVar& p, SyncVar& t) : 
            qcx (q), psx (p), ttx (t)

// Constructor.
{
}
/*****************************************************************************/

void Tasx::compute ()

// Compute the derived value from the current input parameter values.
{
  double rmach2; 
  double qcX;

  if ((qcX = qcx.value()) < 0.01)
    qcX = .01;

// Compute the mach number squared function.
  rmach2 = 5.0 * (pow((double)((qcX / psx.value()) + 1.), 
                      (double)0.28571) - 1.0);

// Compute the true air speed.
// RECFRA = recovery factor for radome true air speed - depends on
// TT used. Use 0.95 for now until search code is installed.  .19 = .2 * .95.
  d_value = (ttx.value() + 273.16) * (401.86 / (1.0 / rmach2 + 0.19));
 
  if (d_value < 0.0)
    d_value = -d_value;

  d_value = sqrt (d_value);
//printf ("qcx = %f, psw = %f, ttx = %f, tasx = %f\n", 
//        qcx, psx.value(), ttx.value(), d_value);
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

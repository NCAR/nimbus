/* Palt.cc

   Computes the aircraft pressure altitude from PSX.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <Palt.h>

/******************************************************************************
** Public Functions
******************************************************************************/

Palt::Palt (SyncVar &p) : psx (p) 

// Constructor.
{
}
/*****************************************************************************/

void Palt::compute()

// Compute the derived value from the current input parameter values.
{
  d_value = 153.77 * SFCT *
            (1.0 - pow((double)(psx.value() / ASTG), (double)0.190284));
//printf ("Palt = %f\n", d_value);
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

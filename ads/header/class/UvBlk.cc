/* UvBlk.h
 
   UV Hygrometer data block class.
 
   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <UvBlk.h>

/******************************************************************************
** Public Functions
******************************************************************************/

UvBlk::UvBlk()

// Constructor. 
{
  memset ((char*)&uv_blk, 0, sizeof (uv_blk));
  idx = 0;
}
/*****************************************************************************/

void UvBlk::display()
 
// Diagnostic function to display the contents of the current sample.
{
  printf ("%hu %hu %hu %hu %hu %hu %hu %hu %f\n",
          getHydrSmp(),
          getHydrRef(),
          getKrypSmp(),
          getKrypRef(),
          getChopSmp(),
          getChopRef(),
          getTemp(),
          getPressure(),
          getHumidity());
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

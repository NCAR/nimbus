/* Ophir3Blk.cc
 
   Ophir 3 radiometer data block class.
 
   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <Ophir3Blk.h>

/******************************************************************************
** Public Functions
******************************************************************************/

Ophir3Blk::Ophir3Blk()

// Constructor. 
{
  memset ((char*)&oph3_blk, 0, sizeof (oph3_blk));
}
/*****************************************************************************/

void Ophir3Blk::display()
 
// Diagnostic function to display the contents of the current block.
{
  printf ("%hu %c %hu %hu %hu %hu %hu %hu %hu %hu %hu\n",
          oph3_blk.gain,
          oph3_blk.bb_pos[0],
          oph3_blk.det_bias,
          oph3_blk.snout_tmp,
          oph3_blk.bench_tmp,
          oph3_blk.det_tmp,
          oph3_blk.cool_tmp,
          oph3_blk.float_tmp,
          oph3_blk.motor_spd,
          oph3_blk.det_sig,
          oph3_blk.box_tmp);
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

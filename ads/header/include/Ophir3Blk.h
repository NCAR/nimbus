/* Ophir3Blk.h

   Ophir 3 radiometer data block class.
 
   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#ifndef OPHIR3BLK_H
#define OPHIR3BLK_H

#include <stdio.h>
#include <string.h>
#include <header.h>

class Ophir3Blk {

public:
  Ophir3Blk();
  void display();			// display diagnostic

  void setGain (short val)	   	{oph3_blk.gain = val;}
  void setBBPos (char val)	   	{oph3_blk.bb_pos[0] = val;}
  void setDetBias (short val)   	{oph3_blk.det_bias = val;}
  void setSnoutTmp (short val) 		{oph3_blk.snout_tmp = val;}
  void setBenchTmp (short val) 		{oph3_blk.bench_tmp = val;}
  void setDetTmp (short val)	   	{oph3_blk.det_tmp = val;}
  void setCoolTmp (short val)   	{oph3_blk.cool_tmp = val;}
  void setFloatTmp (short val) 		{oph3_blk.float_tmp = val;}
  void setMotorSpd (short val) 		{oph3_blk.motor_spd = val;}
  void setDetSig (short val)	   	{oph3_blk.det_sig = val;}
  void setBoxTmp (short val)	   	{oph3_blk.box_tmp = val;}

  short getGain()			{return oph3_blk.gain;}
  char getBBPos()  			{return oph3_blk.bb_pos[0];}
  short getDetBias()			{return oph3_blk.det_bias;}
  short getSnoutTmp()			{return oph3_blk.snout_tmp;}
  short getBenchTmp()			{return oph3_blk.bench_tmp;}
  short getDetTmp()			{return oph3_blk.det_tmp;}
  short getCoolTmp()			{return oph3_blk.cool_tmp;}
  short getFloatTmp()			{return oph3_blk.float_tmp;}
  short getMototSpd()			{return oph3_blk.motor_spd;}
  short getDetSig()			{return oph3_blk.det_sig;}
  short getBoxTmp()			{return oph3_blk.box_tmp;}

  const char *data() 			{ return (const char*)&oph3_blk; }

private:
  Oph3_blk oph3_blk;			// local data block

};
#endif

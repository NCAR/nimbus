/* UvBlk.h

   UV Hygrometer data block class.
 
   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#ifndef UVBLK_H
#define UVBLK_H

#include <stdio.h>
#include <string.h>
#include <header.h>

class UvBlk {
public:

  UvBlk();
  void display();			// display current sample

  int firstSample()		{idx = 0; return TRUE;}
  int nextSample()        	{return (idx < RATE_50 - 1)? ++idx: FALSE;}
  void setSample (Uv_smp *samp)	{uv_blk.uvsmp[idx] = *samp;}
  const Uv_smp *sample()      	{return (const Uv_smp*)&uv_blk.uvsmp[idx];}
  const char *buffer() 	 	{return (const char*)&uv_blk;}
  int index()			{return idx;}

  void setHydrSmp (short val)		{uv_blk.uvsmp[idx].hydr_smp = val;}	
  void setHydrRef (short val)		{uv_blk.uvsmp[idx].hydr_ref = val;}	
  void setKrypSmp (short val)		{uv_blk.uvsmp[idx].kryp_smp = val;}	
  void setKrypRef (short val)		{uv_blk.uvsmp[idx].kryp_ref = val;}	
  void setChopSmp (short val)		{uv_blk.uvsmp[idx].chop_smp = val;}	
  void setChopRef (short val)		{uv_blk.uvsmp[idx].chop_ref = val;}	
  void setTemp (short val)		{uv_blk.uvsmp[idx].temp = val;}	
  void setPressure (short val)		{uv_blk.uvsmp[idx].pressure = val;}	
  void setHumidity (float val)		{uv_blk.uvsmp[idx].humidity = val;}	

  short getHydrSmp()			{return uv_blk.uvsmp[idx].hydr_smp;}	
  short getHydrRef()			{return uv_blk.uvsmp[idx].hydr_ref;}	
  short getKrypSmp()			{return uv_blk.uvsmp[idx].kryp_smp;}	
  short getKrypRef()			{return uv_blk.uvsmp[idx].kryp_ref;}	
  short getChopSmp()			{return uv_blk.uvsmp[idx].chop_smp;}	
  short getChopRef()			{return uv_blk.uvsmp[idx].chop_ref;}	
  short getTemp()			{return uv_blk.uvsmp[idx].temp;}	
  short getPressure()			{return uv_blk.uvsmp[idx].pressure;}	
  float getHumidity()			{return uv_blk.uvsmp[idx].humidity;}	

private:
  Uv_blk uv_blk;			// local data buffer
  int idx;
};
#endif

/* Irs.h
 
   Honeywell YG1854 IRS interface class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef HWIRS_H
#define HWIRS_H

#include <string.h>
#include <Bc635Vme.h>
#include <IP429.h>

#include <arinc429.h>
#include <dsmctl.h>
#include <header.h>

#define HW_TASALT_LEN		3		// length of tas_alt array
#define HW_TAS_IDX		0		// tas word index
#define HW_ALT_IDX		1		// altitude word index
#define HW_ALT_RATE_IDX		2		// altitude rate word index

#define HW_TAS_ALT_GAP		41		// gap time value for ~20 hz

class HwIrs {
public:
	HwIrs (IP429*, Bc635Vme&, int rxchan, int txchan);
  void	secondAlign();			// one second alignment
  void	readIsr();			// sampling isr

  char	*buffer();			// get last sec buffer
  void	setTasAlt(float tas, float alt);// build arinc 429 tas_alt words
  void	sendTasAlt()			// send current tas_alt data
         {ip429->write429(tx_chan, tas_alt, HW_TASALT_LEN, HW_TAS_ALT_GAP);}

//  inline long irs_pitch(int i)          {return irs_blk.pitch_angle[i];}
//  inline long irs_roll(int i)           {return irs_blk.roll_angle[i];}

  int drift() 		{return cur_drift;}
  int groundSpeed()	{return cur_gnd_speed;}
  int inertialAlt() 	{return cur_inertial_alt;}
  int latitude() 	{return cur_lat;}
  int longitude() 	{return cur_lon;}
  int pitch() 		{return cur_pitch;}
  int roll() 		{return cur_roll;}
  int trueHeading() 	{return cur_true_heading;}
 
private:
  Irs_blk	irs_blk;		// data buffers
  long	ring[TOG][1300], *head;
  IP429	*ip429;				// arinc interface class
  Bc635Vme &tfp;                       	// time-freq processor class

  bool	newSecond;
  int	gtog;				// get buffer index
  int	ptog;				// put buffer index
  int	rx_chan;				// receive arinc channel
  int	tx_chan;				// transmit arinc channel
  int	tas_alt[HW_TASALT_LEN];		// tas, alt, and alt rate words
  float	last_alt;			// last altitude value

  int cur_drift;			// current arinc ground speed word
  int cur_gnd_speed;			// current arinc ground speed word
  int cur_inertial_alt;			// current arinc inertial altitude word
  int cur_lat;				// current arinc latitude word
  int cur_lon;				// current arinc longitude word
  int cur_pitch;			// current arinc pitch word
  int cur_roll;				// current arinc roll word
  int cur_true_heading;			// current arinc true heading word

int cntr[250];

};

#endif

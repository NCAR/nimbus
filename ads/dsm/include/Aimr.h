/* Aimr.h
 
   Aimr arinc 429 interface class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef AIMR_H
#define AIMR_H

#include <Bc635Vme.h>
#include <HwIrs.h>
#include <IP429.h>

#include <arinc429.h>
#include <dsmctl.h>
#include <header.h>

#define AIMR_BLOCK_INTERMEDIATE_GAP	5	// full block intermediate gap
#define AIMR_BLOCK_LAST_GAP	63		// full block last gap time
#define AIMR_ROLL_GAP		97		// roll gap time (10 hz)
#define AIMR_BLOCK_SIZE		11		// block size in ints

#define AIMR_DAY_SHIFT		25		// bits to shift bcd day
#define AIMR_MONTH_SHIFT	19		// bits to shift bcd month
#define AIMR_YEAR_SHIFT		11		// bits to shift bcd year
#define AIMR_HOUR_SHIFT		25		// bits to shift bcd hour
#define AIMR_MINUTE_SHIFT	18		// bits to shift bcd minute
#define AIMR_SECOND_SHIFT	11		// bits to shift bcd second

#define AIMR_FINE_DATA_SHIFT	7		// bits to shift coarse data 
#define AIMR_FINE_DATA_MASK	0x7ffc0000	// mask to create fine data
#define AIMR_KALMAN_ALT_SHIFT	1		// bits to shift inertial alt 
#define AIMR_RADIO_ALT_SHIFT	3		// bits to shift inertial alt 
#define AIMR_SIGN_MASK		0x40000000	// aimr word format parity bits 
#define AIMR_FORMAT_SSM_VALID	0x30000000	// aimr word format SSM bits 
#define AIMR_PARITY_MASK	0x08000000	// aimr word format parity bits 

typedef struct {
  int gmt;				// time
  int date;				// date
  int latitude;				// latitude
  int longitude;			// longitude
  int true_heading;			// true heading
  int drift;				// drift angle
  int pitch;				// pitch angle
  int roll;				// roll angle
  int kal_alt;				// kalman altitude
  int gnd_speed;			// ground speed
  int tas;				// true air speed
  int palt;				// pressure altitude
  int flight;
  int inertial_alt;			// inertial altitude
} AimrArinc;

class Aimr {
public:
  Aimr (IP429*, Bc635Vme&, HwIrs*, int txchan);
  void buildArinc ();			// build arinc block from irs params
  void buildRoll ();			// build the roll word
  void buildGndSpeed ();			// build the roll word
  void setFlight(int flight);
  void setTas (float tas);		// build the tas arinc word
  void setPalt (float palt);		// build the palt arinc word
  void sendArinc(); 			// send current arinc data
 
private:
  int convertToAimrStd (int value); 	// convert to Aimr fmt
  int convertToAimr (int value); 	// convert to Aimr fmt with 2's comp
  char reverseByte (char byte);		// reverse bit order of byte
  int parity (int value);		// compute parity of value

  IP429 *ip429;				// arinc interface class
  Bc635Vme &tfp;                       	// time-freq processor class
  HwIrs *hwirs;				// Honeywell Irs class

  AimrArinc arinc;			// current data block
  int *arincp;				// pointer to the arinc block
  int idx;				// current index
  int tx_chan;				// transmit arinc channel
  int new_arinc;			// new arinc block available flag
};

#endif

/* ArincGps.h
 
   ArincGps interface class.

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef ARINCGPS_H
#define ARINCGPS_H

#include <Bc635Vme.h>
#include <IP429.h>

#include <arinc429.h>
#include <dsmctl.h>
#include <header.h>

#define ARINC_BLOCK_INTERMEDIATE_GAP	1	// full block intermediate gap
#define ARINC_BLOCK_LAST_GAP	63		// full block last gap time
#define ARINC_ROLL_GAP		97		// roll gap time (10 hz)
#define ARINC_BLOCK_SIZE		11	// block size in ints

// #define ARINCGPS_BLOCK_SIZE         16          // block size in ints
//#define ARINCGPS_BLOCK_INTER_GAP    5           // full block last gap time
//#define ARINCGPS_BLOCK_LAST_GAP     954         // full block last gap time
//#define ARINCGPS_BLOCK_SIZE         55          // block size in ints
#define ARINCGPS_BLOCK_SIZE         3          // block size in ints
#define ARINCGPS_BLOCK_INTER_GAP    0           // full block last gap time
#define ARINCGPS_BLOCK_LAST_GAP     0         // full block last gap time

#define ARINC_DAY_SHIFT		25		// bits to shift bcd day
#define ARINC_MONTH_SHIFT	19		// bits to shift bcd month
#define ARINC_YEAR_SHIFT		11	// bits to shift bcd year
#define ARINC_HOUR_SHIFT		25	// bits to shift bcd hour
#define ARINC_MINUTE_SHIFT	18		// bits to shift bcd minute
#define ARINC_SECOND_SHIFT	11		// bits to shift bcd second

#define ARINC_FINE_DATA_SHIFT	7		// bits to shift coarse data 
#define ARINC_FINE_DATA_MASK	0x7ffc0000	// mask to create fine data
#define ARINC_KALMAN_ALT_SHIFT	1		// bits to shift inertial alt 
#define ARINC_RADIO_ALT_SHIFT	3		// bits to shift inertial alt 
#define ARINCGPS_SIGN_MASK	0x40000000	// arinc word format parity bits
#define ARINC_FORMAT_SSM_VALID	0x30000000	// arinc word format SSM bits 
#define ARINCGPS_PARITY_MASK	0x08000000	// arinc word format parity bits
/*
typedef struct {
  int meas_status;			// measurement status
  int utc_time;				// utc_time
  int altitude;				// altitude
  int hdop;				// GPS hdop
  int vdop;				// GPS vdop
  int latitude;				// latitude
  int longitude;			// longitude
  int ground_speed;			// ground speed
  int lat_fine;				// lat_fine 
  int long_fine;			// long_fin
  int vertical_fom;			// vertical_fom
  int vertical_velocity;		// vertical_velocity
  int ns_velocity;			// ns_velocity
  int ew_velocity;			// ew_velocity
  int horizontal_fom;			// horizontal_fom
  int sensor_status;			// sensor_status
} GpsArinc;
*/
typedef struct {
  int integ_vert_accel[3];                 // integrated vertical accel. 
} GpsArinc;

class ArincGps {
public:
  ArincGps (IP429*, Bc635Vme&, int txchan);
  void buildArincGps ();		// build arinc block from irs params
  void sendArincGps(); 			// send current arinc data
 
private:

  IP429 *ip429;				// arinc interface class
  Bc635Vme &tfp;                       	// time-freq processor class

  GpsArinc arincgps;			// current data block
  int *arincgpsp;			// pointer to the arinc block
  int idx;				// current index
  int tx_chan;				// transmit arinc channel
  int new_arincgps;			// new arinc block available flag
};

#endif

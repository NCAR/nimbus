/* arinc429.h
   Arinc 429 label and bit defines.
 
   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#define ARINC_LABEL_MASK        0x000000FF      /* label bits mask */
#define ARINC_PARITY_MASK      	0x00000100      /* parity bit mask */
#define ARINC_SSM1_MASK		0x00000200	/* SSM1 bit mask */
#define ARINC_SSM2_MASK		0x00000400	/* SSM1 bit mask */
#define ARINC_SSM_VALID		0x00000600	/* status/sign matrix valid */
#define ARINC_SIGN_MASK        	0x80000000      /* sign bit mask */
#define ARINC_BIT_27_MASK	0x20000000	/* arinc bit #27 mask */
#define ARINC_BIT_26_MASK	0x10000000	/* arinc bit #26 mask */
#define ARINC_BIT_25_MASK	0x08000000	/* arinc bit #25 mask */
#define ARINC_12_BIT_DATA_MASK 	0x7FF80000   	/* 12 bit data mask */
#define ARINC_15_BIT_DATA_MASK 	0x7FFF0000   	/* 15 bit data mask */
#define ARINC_16_BIT_DATA_MASK 	0x7FFF8000   	/* 16 bit data mask */
#define ARINC_17_BIT_DATA_MASK 	0x7FFFC000   	/* 17 bit data mask */
#define ARINC_20_BIT_DATA_MASK 	0x7FFFF800   	/* 20 bit data mask */

#define ARINC_20_BIT_DATA_SHIFT 11              /* bits to shift 20 bit data */
#define ARINC_18_BIT_DATA_SHIFT 13              /* bits to shift 18 bit data */
#define ARINC_17_BIT_DATA_SHIFT 14              /* bits to shift 17 bit data */
#define ARINC_15_BIT_DATA_SHIFT 16              /* bits to shift 15 bit data */
 
/* Arinc 429 octal labels. */
#define ARINC_GPS_MEAS_STATUS_LABEL	0060 	/* GPS meas status */
#define ARINC_GPS_UTC_TIME_LABEL	0074	/* GPS UTC time */
#define ARINC_GPS_GPS_ALT_LABEL		0076	/* GPS altitude */
#define ARINC_GPS_HDOP_LABEL		0101	/* GPS HDOP */
#define ARINC_GPS_VDOP_LABEL		0102	/* GPS VDOP */
#define ARINC_GPS_LAT_LABEL		0110	/* GPS latitude */
#define ARINC_GPS_LON_LABEL		0111	/* GPS longitude */
#define ARINC_GPS_GND_SPEED_LABEL	0112	/* GPS ground speed */
#define ARINC_GPS_LAT_FINE_LABEL	0120	/* GPS latitude fine */
#define ARINC_GPS_LON_FINE_LABEL	0121	/* GPS longitude fine */
#define ARINC_GPS_VERTICAL_FOM_LABEL	0136	/* GPS vertical FOM */
#define ARINC_GPS_VERTICAL_VEL_LABEL	0165	/* GPS vertical vel. */
#define ARINC_GPS_NS_VEL_LABEL		0166	/* GPS N/S velocity */
#define ARINC_GPS_EW_VEL_LABEL		0174	/* GPS E/W velocity */
#define ARINC_GPS_HOR_FOM_LABEL		0247	/* GPS horizontal FOM */
#define ARINC_GPS_SENSOR_STAT_LABEL	0273	/* GPS sensor status */
#define ARINC_GMT_LABEL         	0124	/* gmt */ 
#define ARINC_RADIO_ALT_LABEL		0165    /* radio altitude */ 
#define ARINC_TAS_LABEL         	0210   	/* true air speed */
#define ARINC_PALT_LABEL        	0203    /* pressure altitude */
#define ARINC_BARO_ALT_LABEL        	0204    /* pressure alt for aimr */
#define ARINC_PALT_RATE_LABEL   	0212    /* pressure altitude rate */
#define ARINC_DATE_LABEL        	0240    /* date */
#define ARINC_FLIGHT_LABEL		0261    /* flight number */
#define ARINC_INTEG_VERT_ACCEL_LABEL	0265	/* integrated vert. accel */
#define ARINC_FINE_LAT_LABEL    	0300    /* fine latitude (aimr only) */
#define ARINC_FINE_LON_LABEL    	0301    /* fine longitude (aimr only) */
#define ARINC_LATITUDE_LABEL    	0310    /* latitude */
#define ARINC_LONGITUDE_LABEL   	0311    /* longitude */
#define ARINC_GND_SPEED_LABEL   	0312    /* ground speed */
#define ARINC_TRUE_HEADING_LABEL   	0314    /* true heading */
#define ARINC_DRIFT_LABEL   		0321    /* drift label */
#define ARINC_PITCH_LABEL   		0324    /* pitch label */
#define ARINC_ROLL_LABEL		0325    /* roll label */
#define ARINC_KALMAN_ALT_LABEL  	0355	/* kalman altitude (aimr only)*/
#define ARINC_INERTIAL_ALT_LABEL  	0361    /* kalman altitude (aimr only)*/
#define ARINC_PITCH_RATE_LABEL		0326
#define ARINC_ROLL_RATE_LABEL		0327
#define ARINC_YAW_RATE_LABEL		0330
#define ARINC_LONG_ACCEL_LABEL		0331
#define ARINC_LAT_ACCEL_LABEL		0332
#define ARINC_NORMAL_ACCEL_LABEL	0333
#define ARINC_TRACK_ANG_RATE_LABEL	0335
#define ARINC_PITCH_ATT_RATE_LABEL	0336
#define ARINC_ROLL_ATT_RATE_LABEL	0337
#define ARINC_POT_VERT_SPEED_LABEL	0360
#define ARINC_VERT_ACCEL_LABEL		0364
#define ARINC_DUMMY_LABEL		0120
 
/* Arinc 429 data resolution values. */
#define ARINC_TAS_RES           0.0625          /* tas meters/sec per bit */

#define ARINC_NUM_BYTES		4		/* num bytes in an arinc word */

/* aircraft.h -- this file sets up what airplane is being used

   Original Author:  Keith S. Barr
   Copyright 1994
   National Center for Atmospheric Research
   Boulder, Colorado

   Written in support of ASHOE/MAESA

   Revision History:
      08/17/1994 ksb Created
*/

#ifndef AIRCRAFT_H
#define AIRCRAFT_H

/* what aircraft are we on? */
#define ER2      FALSE
#define DC8	 FALSE
#define C130     TRUE 
#define STRATO   FALSE
#define FALCON   FALSE
#define WB57F    FALSE 

/* what boards do we have installed */
/* Green Springs IP-Serial */
/* gives us another serial port */
#define IP_SERIAL_IP_A	FALSE
#define IP_SERIAL_IP_B	FALSE
#define IP_SERIAL_IP_C	FALSE
#define IP_SERIAL_IP_D	FALSE

/* Green Springs IP-Digital 24 */
/* on/off controller */
#define IP_PIO_IP_A		FALSE
#define IP_PIO_IP_B		FALSE
#define IP_PIO_IP_C		FALSE
#define IP_PIO_IP_D		FALSE

/* Green Springs IP-Precision-ACD */
/* monitors Analog voltages */
#define IP_ADC_IP_A		FALSE
#define IP_ADC_IP_B		TRUE
#define IP_ADC_IP_C		FALSE
#define IP_ADC_IP_D		FALSE

#endif


/* Aimr.cc

   Aimr arinc 429 interface class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <Aimr.h>

/******************************************************************************
** Public Functions
******************************************************************************/

Aimr::Aimr (IP429 *ip, Bc635Vme &tp, HwIrs *hw, int txchan) : 
            tfp (tp)
         
// Constructor.
{
  hwirs = hw;
  ip429 = ip;
  tx_chan = txchan;

  ip429->setSpeedLow (tx_chan);		// transmit at low speed
  new_arinc = FALSE;

// Set arinc block pointer and index.
  arincp = (int*)&arinc;		
  idx = 0;
}
/*****************************************************************************/
 
void Aimr::buildArinc ()

// Builds the AimrArinc block except for the flight, tas and palt fields.
// This routine is called once per second.
{
// Thei sign bit is set in the date and gmt fields to prevent convertToAimr
// from inverting the data.

  arinc.date = (tfp.bcdDay() << AIMR_DAY_SHIFT) +
               (tfp.bcdMonth() << AIMR_MONTH_SHIFT) +
               (tfp.bcdYear() << AIMR_YEAR_SHIFT) + 
               ARINC_DATE_LABEL;
//printf ("%02X/%02X/%02X, date = 0x%X\n", tfp.bcdDay(), tfp.bcdMonth(), 
//        tfp.bcdYear(), arinc.date);

//arinc.date = convertToAimr (arinc.date);

  arinc.gmt = (tfp.bcdHour() << AIMR_HOUR_SHIFT) +
              (tfp.bcdMinute() << AIMR_MINUTE_SHIFT) +
              (tfp.bcdSecond() << AIMR_SECOND_SHIFT) + 
              ARINC_SIGN_MASK + ARINC_GMT_LABEL;

// This is ugly, but it works.  Figure it out later.
  arinc.gmt = convertToAimr (arinc.gmt);
  arinc.gmt &= ~AIMR_SIGN_MASK;
  if (arinc.gmt & AIMR_PARITY_MASK)
    arinc.gmt &= ~AIMR_PARITY_MASK;
  else
    arinc.gmt |= AIMR_PARITY_MASK;
  arinc.gmt &= ~AIMR_FORMAT_SSM_VALID;

//printf ("arinc.gmt = 0x%X\n", arinc.gmt);


/** These all work.
  arinc.latitude = ((int)((39.0 / 180.0) * 2147483648.0) & 0xfffff800) + 
               ARINC_LATITUDE_LABEL;
  arinc.latitude = convertToAimrStd (arinc.latitude);

  arinc.longitude = ((int)((105.0 / 180.0) * 2147483648.0) & 0xfffff800) + 
               ARINC_LONGITUDE_LABEL;
  arinc.longitude = convertToAimrStd (arinc.longitude);

  arinc.inertial_alt = ((int)((10122.0/131072.0)*2147483648.0) & 0xfffff800) + 
               ARINC_INERTIAL_ALT_LABEL;
  arinc.inertial_alt = convertToAimr (arinc.inertial_alt);

  arinc.gnd_speed = ((int)((300.0/4096.0)*2147483648.0) & 0xfffff800) + 
               ARINC_GND_SPEED_LABEL;
  arinc.gnd_speed = convertToAimr (arinc.gnd_speed);

  arinc.true_heading = ((int)((-80.0 / 180.0) * 2147483648.0) & 0xfffff800) + 
               ARINC_TRUE_HEADING_LABEL;
  arinc.true_heading = convertToAimrStd (arinc.true_heading);

  arinc.drift = ((int)((-19.0 / 180.0) * 2147483648.0) & 0xfffff800) + 
               ARINC_DRIFT_LABEL;
  arinc.drift = convertToAimr (arinc.drift);

  arinc.pitch = ((int)((-18.0 / 180.0) * 2147483648.0) & 0xfffff800) + 
               ARINC_PITCH_LABEL;
  arinc.pitch = convertToAimrStd (arinc.pitch);
 
  setTas (60.0);
**/

// Aimr latitude, longitude, and inertial altitude words are passed through
// from the irs.
  arinc.latitude = (hwirs->latitude() & 
                    (ARINC_20_BIT_DATA_MASK | ARINC_SIGN_MASK)) + 
                   ARINC_LATITUDE_LABEL;
  arinc.latitude = convertToAimrStd (arinc.latitude);

  arinc.longitude = (hwirs->longitude() & 
                     (ARINC_20_BIT_DATA_MASK | ARINC_SIGN_MASK)) + 
                    ARINC_LONGITUDE_LABEL;
  arinc.longitude = convertToAimrStd (arinc.longitude);

  arinc.true_heading = (hwirs->trueHeading() & 
                        (ARINC_20_BIT_DATA_MASK | ARINC_SIGN_MASK)) + 
                       ARINC_TRUE_HEADING_LABEL;
  arinc.true_heading = convertToAimrStd (arinc.true_heading);

  arinc.drift = (hwirs->drift() & 
                 (ARINC_20_BIT_DATA_MASK | ARINC_SIGN_MASK)) + 
                ARINC_DRIFT_LABEL;
  arinc.drift = convertToAimr (arinc.drift);

  arinc.pitch = (hwirs->pitch() & 
                 (ARINC_15_BIT_DATA_MASK | ARINC_SIGN_MASK)) + 
                ARINC_PITCH_LABEL;
  arinc.pitch = convertToAimrStd (arinc.pitch);

  buildGndSpeed();
  buildRoll();

  new_arinc = TRUE;		// set new block available flag
}
/*****************************************************************************/

void Aimr::buildRoll ()

// Builds the arinc 429 format roll word from the current irs value.
{

//arinc.roll = ((int)((0.0 / 180.0) * 2147483648.0) & 0xfffff800) + 
//             ARINC_ROLL_LABEL;

  arinc.roll = (hwirs->roll() & 
                (ARINC_20_BIT_DATA_MASK | ARINC_SIGN_MASK)) + 
               ARINC_ROLL_LABEL;

  arinc.roll = convertToAimr (arinc.roll);
}
/*****************************************************************************/

void Aimr::buildGndSpeed ()

// Builds the arinc 429 format ground speed  word from the current irs value.
{
  float gspd;

  gspd = ((float)(hwirs->groundSpeed() & 
         (ARINC_20_BIT_DATA_MASK | ARINC_SIGN_MASK))  / 2147483648.0) * 4096.0;

  if (gspd < 100.0)		// Set a minimum speed of 100 kts
    gspd = 100.0;

  arinc.gnd_speed = ((int)((gspd / 4096.0) * 2147483648.0) & 0xffff0000) + 
               ARINC_GND_SPEED_LABEL;
  arinc.gnd_speed = convertToAimr (arinc.gnd_speed);
//arinc.gnd_speed = convertToAimrStd (arinc.gnd_speed);

// For now, set the tas == ground speed.
  arinc.tas = ((int)(((gspd) / 2048.0) * 2147483648.0) & 0xffff0000) + 
              ARINC_TAS_LABEL;
//arinc.tas = convertToAimr (arinc.tas);
  arinc.tas = convertToAimrStd (arinc.tas);
}
/*****************************************************************************/
 
void Aimr::setFlight (int flight)

// Builds the arinc 429 format flight word from the passed in value. 
// This routine does not yet work.
{
// Convert to BCD.
  arinc.flight = flight / 100;
  flight %= 100; 
  arinc.flight = (arinc.flight << BCD_DIGIT_SHIFT) + flight / 10;
  flight %= 10;
  arinc.flight = (arinc.flight << BCD_DIGIT_SHIFT) + flight;
  arinc.flight <<= ARINC_15_BIT_DATA_SHIFT;

  arinc.flight += ARINC_FLIGHT_LABEL;
//printf ("arinc.flight = 0x%X\n", arinc.flight);

  arinc.flight = convertToAimrStd (arinc.flight);
}
/*****************************************************************************/
 
void Aimr::setTas (float tas)

// Builds the arinc 429 format true air speed word from the passed in value in
// meters/sec.
{

/** For now, tas is being set equal to the gnd speed. See buildGndSpeed.
  tas *= (float)SECS_PER_HOUR / METERS_PER_NAUT;       // convert m/s to kts

  arinc.tas = ((int)((tas / 2048.0) * 2147483648.0) & 0xfffff800) + 
              ARINC_TAS_LABEL;
  arinc.tas = convertToAimrStd (arinc.tas);
**/
}
/*****************************************************************************/
 
void Aimr::setPalt (float palt)

// Builds the arinc 429 format true air speed word from the passed in value in
// meters/sec.
{
  if ((palt *= (float)FEET_PER_METER) < 1000.0)
    palt = 1000.0;

  arinc.palt = ((int)((palt / 131072.0) * 2147483648.0) & 0xfffff800) + 
               ARINC_BARO_ALT_LABEL;
  arinc.palt = convertToAimr (arinc.palt);

  arinc.kal_alt = ((int)((palt / 65536.0) * 2147483648.0) & 0xfffff800) + 
               ARINC_KALMAN_ALT_LABEL;
  arinc.kal_alt = convertToAimr (arinc.kal_alt);

//printf ("palt = %f, arinc.palt = %x\n", palt, arinc.palt);
//printf ("arinc.kal_alt = %x\n", arinc.kal_alt);
}
/*****************************************************************************/
 
void Aimr::sendArinc ()

// Sends the current arinc data to the Aimr.
{
  int len;
  int gap;

// If a new arinc block is available, then send it in IP429_FIFO_SIZE sections.
  if (new_arinc) {
//  ip429->write429 (tx_chan, arincp, 4, 88);
//  new_arinc = FALSE;			

// Determine the number of words and gap time for this section. 
    if ((len = AIMR_BLOCK_SIZE - idx) > IP429_FIFO_SIZE) {
      len = IP429_FIFO_SIZE;
      gap = AIMR_BLOCK_INTERMEDIATE_GAP;
    }
    else {
      gap = AIMR_BLOCK_LAST_GAP;
      new_arinc = FALSE;				// end of the block
    }

    ip429->write429 (tx_chan, arincp + idx, len, gap);	// send the data 

// Increment the index if there is more of the block to send, else reset it.
    if (new_arinc)
      idx += len;
    else
      idx = 0;
  }

// Send just the roll at a 10 hz rate.
  else {
    buildRoll(); 
    ip429->write429 (tx_chan, &arinc.roll, 1, AIMR_ROLL_GAP);
  }
}
/******************************************************************************
** Private Functions
******************************************************************************/

int Aimr::convertToAimrStd (int value)

{
  char ar_bytes[ARINC_NUM_BYTES];
  int temp;
  int ivalue;
  int j;

// Copy the data value into the byte array, and shift msb down to bit 29 pos.
// If the data value is positive, convert to a two's complement, since this
// is what the Aimr expects.
  *(int*)ar_bytes = ((value >> 3) & 0x0FFFFF00) +
                    ((value >> 3) & 0x10000000);
  ivalue = value;

// Reverse the data byte bit orders.
  for (j = 0; j < (ARINC_NUM_BYTES - 1); j++)
    ar_bytes[j] = reverseByte (ar_bytes[j]);

// Copy to temp, shifting D29 back to the msb, and set the label and SSM bits.
  temp = (*(int*)ar_bytes << 3) + AIMR_FORMAT_SSM_VALID + 
         (value & ARINC_LABEL_MASK);

// If the original bit 26 and arinc bit 27 are set, then set the corresponding 
// arinc SSM bits which now map into those bit positions.
  if (ivalue & ARINC_BIT_27_MASK)
    temp |= ARINC_SSM1_MASK;

  if (ivalue & ARINC_BIT_26_MASK)
    temp |= ARINC_SSM2_MASK;

// If the original bit 25 is set, then adjust the word parity to be even.  The
// arinc chip will then set the parity bit which now maps into arinc bit 25
// of the aimr format word.  If the original bit 25 is not set, then adjust
// the word parity to be odd, so that the parity bit will not be set.
  if (((ivalue & ARINC_BIT_25_MASK) && parity(temp)) ||	 // bit set & odd par
     (!(ivalue & ARINC_BIT_25_MASK) && !parity(temp))) { // bit !set & even par
      temp |= AIMR_PARITY_MASK;	// toggle current aimr format parity bit 
  }
  return temp;
}
/*****************************************************************************/

int Aimr::convertToAimr (int value)

// The Aimr arinc receiver reads data one byte at a time, and in a bit 
// reversed order.  The IP429-1 transmitter sends the label byte in a bit
// reversed order, but it sends the 3 data bytes in a normal bit order.
// Thus the data bytes must be reversed.
{
  char ar_bytes[ARINC_NUM_BYTES];
  int temp;
  int ivalue;
  int j;

// Copy the data value into the byte array, and shift msb down to bit 29 pos.
// If the data value is positive, convert to a two's complement, since this
// is what the Aimr expects.
  if (value >= 0) {
    *(int*)ar_bytes = ((~(value >> 3) + 0x00000100) & 0x0FFFFF00) +
                      ((value >> 3) & 0x10000000);
    ivalue = *(int*)ar_bytes << 3;
  }
  else {
    *(int*)ar_bytes = ((value >> 3) & 0x0FFFFF00) +
                      ((value >> 3) & 0x10000000);
    ivalue = value;
  }

// Reverse the data byte bit orders.
  for (j = 0; j < (ARINC_NUM_BYTES - 1); j++)
    ar_bytes[j] = reverseByte (ar_bytes[j]);

// Copy to temp, shifting D29 back to the msb, and set the label and SSM bits.
  temp = (*(int*)ar_bytes << 3) + AIMR_FORMAT_SSM_VALID + 
         (value & ARINC_LABEL_MASK);

// If the original bit 26 and arinc bit 27 are set, then set the corresponding 
// arinc SSM bits which now map into those bit positions.
  if (ivalue & ARINC_BIT_27_MASK)
    temp |= ARINC_SSM1_MASK;

  if (ivalue & ARINC_BIT_26_MASK)
    temp |= ARINC_SSM2_MASK;

// If the original bit 25 is set, then adjust the word parity to be even.  The
// arinc chip will then set the parity bit which now maps into arinc bit 25
// of the aimr format word.  If the original bit 25 is not set, then adjust
// the word parity to be odd, so that the parity bit will not be set.
  if (((ivalue & ARINC_BIT_25_MASK) && parity(temp)) ||	 // bit set & odd par
     (!(ivalue & ARINC_BIT_25_MASK) && !parity(temp))) { // bit !set & even par
      temp |= AIMR_PARITY_MASK;	// toggle current aimr format parity bit 
  }
  return temp;
}
/*****************************************************************************/

char Aimr::reverseByte (char byte)

// Reverses the bit order of the passed in byte.
{
  int j;
  char temp = 0;

  for (j = 0; j < 8; j++) {
    temp <<= 1;
    if (byte & 0x01)
      temp |= 0x01;
    byte >>= 1;
  }
  return temp;
}
/*****************************************************************************/

int Aimr::parity (int value)

// Returns true if value has odd parity, false if even parity.
{
  int j;
  int par;

  for (j = 0, par = 0; j < 32; j++, value >>= 1) {
    if (value & 0x01)
      par++;
  }

  return par & 0x01;
}
/*****************************************************************************/

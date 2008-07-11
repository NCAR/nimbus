/* Ophir3.cc
 
   Ophir 3 radiometer class.
 
   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <Ophir3.h>

/******************************************************************************
** Public Functions
******************************************************************************/

Ophir3::Ophir3 (const char *base, const int port, 
                void (*statMsg)(const char* msg)) :
                Ophir3Blk(),
                IsioPort (base, port, OPHIR3_BAUD, OPHIR3_BITS, OPHIR3_PARITY,
                OPHIR3_STOP, OPHIR3_FLOW, statMsg), statusMsg (statMsg)

// Constructor. The interrupt routine must be previously set up outside this 
// class.
{
  memset (buf, 0, sizeof (buf));
  bb_state = OPH3_BB_NORM;
  zero_cnt = 0;
  zero_flag = FALSE;
  term_str[1] = '\n';
  term_str[1] = '\r';
  term_str[2] = '\0';
  igetstr (term_str);			// issue first read
}
/*****************************************************************************/

void Ophir3::checkStatus()

// Check the serial port status.
{
  if (checkPort() != TRUE) {
    igetstr (term_str);         // new read if an error or dropped interrupt
    statusMsg ("Ophir3: Dropped isio interrupt.\n");
  } 
}
/*****************************************************************************/

void Ophir3::startCal()

// Sends the OPH3_START_CAL char to the instrument to begin the calibration
// sequence.
{
  if (!(int)pputcnt (OPH3_START_CAL, strlen (OPH3_START_CAL))) 
    statusMsg ("Ophir3: Calibration sequence started.\n");
  else
    statusMsg ("Ophir3: Failure sending the start calibration command.\n");
}

/*****************************************************************************/
void Ophir3::checkZero()
 
// Sends a zero check command to the Ophir3 every OPH3_ZERO_INTVL seconds.
// This routine is called once per second.
{
  if (!(zero_cnt = ++zero_cnt % OPH3_ZERO_INTVL) &&
      (bb_state == OPH3_BB_NORM)) {

    if ((int)pputcnt (OPH3_SET_NULL, strlen(OPH3_SET_NULL)))
      statusMsg ("Ophir3: Failure sending the select zero command.\n");

    bb_state = OPH3_BB_ZERO;
    zero_flag = TRUE;
  }

// If in the zero check state, wait for OPH3_ZERO_DUR seconds, and switch
// back to the normal operation state.
  else if (zero_flag && (zero_cnt >= OPH3_ZERO_DUR) &&
           (bb_state == OPH3_BB_ZERO)) {

    if ((int)pputcnt (OPH3_SET_CTL_BB, strlen(OPH3_SET_CTL_BB)))
      statusMsg (
        "Ophir3: Failure sending the select controlled black body command.\n");

    bb_state = OPH3_BB_NORM;
    zero_flag = FALSE;
  }
}

/*****************************************************************************/
void Ophir3::isr()
 
// Pulls in data from the isio and builds the Ophir data struct.
{
  char *datap;
  str2 bb_pos;
  short gain, det_bias, snout_tmp, bench_tmp, det_tmp, cool_tmp, float_tmp,
        motor_spd, det_sig, box_tmp;
 
// Check for data from the isio port.
  if (!(int)(datap = getData()))
    return;
 
// Pull in the data.
  memcpy (buf, datap, OPH3_DATA_LEN);
 
// Check the data alignment.
  if (((buf[OPH3_GAIN_IDX]==OPH3_GAIN1) || (buf[OPH3_GAIN_IDX]==OPH3_GAIN2) ||
       (buf[OPH3_GAIN_IDX]==OPH3_GAIN3) || (buf[OPH3_GAIN_IDX]==OPH3_GAIN0)) &&
      ((buf[OPH3_BB_IDX]==OPH3_BB_NORM) || (buf[OPH3_BB_IDX]==OPH3_BB_ZERO) ||
       (buf[OPH3_BB_IDX]==OPH3_BB_COOL) || (buf[OPH3_BB_IDX]==OPH3_BB_HEAT))) {
 
// Parse the ophir 3 data string and build the recorded data block.
    sscanf (buf, "%hu %s %hu %hu %hu %hu %hu %hu %hu %hu %hu",
            &gain, &bb_pos, &det_bias, &snout_tmp, &bench_tmp, &det_tmp,
            &cool_tmp, &float_tmp, &motor_spd, &det_sig, &box_tmp);

    setGain (gain);
    setBBPos (bb_pos[0]);
    setDetBias (det_bias);
    setSnoutTmp (snout_tmp);
    setBenchTmp (bench_tmp);
    setDetTmp (det_tmp);
    setCoolTmp (cool_tmp);
    setFloatTmp (float_tmp);
    setMotorSpd (motor_spd);
    setDetSig (det_sig);
    setBoxTmp (box_tmp);
  }
  bb_state = buf[OPH3_BB_IDX];
 
  igetstr (term_str);                        // issue next read
}

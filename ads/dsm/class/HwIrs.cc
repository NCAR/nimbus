/* HwIrs.cc

   Honeywell YG1854 IRS interface class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <HwIrs.h>

/******************************************************************************
** Public Functions
******************************************************************************/

HwIrs::HwIrs(IP429 *ip, Bc635Vme& tp, int rxchan, int txchan) : tfp(tp)
{
  ip429 = ip;
  rx_chan = rxchan;
  tx_chan = txchan;

  ptog = 0;
  gtog = 1;
  last_alt = 0.0;
  head = ring[ptog];
cntr[2] = 0;

  cur_drift = 0;
  cur_gnd_speed = 0;
  cur_inertial_alt = 0;
  cur_lat = 0;
  cur_lon = 0;
  cur_pitch = 0;
  cur_roll = 0;
  cur_true_heading = 0;

  if (tx_chan) {
    ip429->setSpeedLow(tx_chan);		// transmit at low speed
    setTasAlt(0.0, 0.0);			// set default values
  }
}

/*****************************************************************************/
void HwIrs::secondAlign()

// Called at the start of a second to align the indices and toggle buffers.
{
  newSecond = true;
  gtog = ptog;                   	// toggle the get buffer index 

}

/*****************************************************************************/
void HwIrs::readIsr()	// Sampling isr. 
{
  long	data;

  *(short*)(((char*)&data) + sizeof(short)) = ip429->data1(rx_chan);
  *(short*)&data = ip429->data2(rx_chan);

  if (newSecond && (data & ARINC_LABEL_MASK) == 0265)
    {
    *head = 0;
    ptog = 1 - gtog;
    head = ring[ptog];
    newSecond = false;
    }

  *head++ = data;

}

/*****************************************************************************/
void HwIrs::setTasAlt(float tas, float alt)

// Builds the arinc 429 format tas, altitude, and altitude rate words.
// This routine is called once per second.
{
  float rate;

// True air speed (kts). 
  tas *= (float)SECS_PER_HOUR / METERS_PER_NAUT;	// convert m/s to kts	
  tas_alt[HW_TAS_IDX] = ((int)((tas / 2048.0) * 2147483648.0) & 0xfffff800) +
                       ARINC_SSM_VALID + ARINC_TAS_LABEL;

// Altitude (feet).
  alt *= (float)FEET_PER_METER;		// convert altitude to feet
  tas_alt[HW_ALT_IDX] = ((int)((alt / 131072.0) * 2147483648.0) & 0xfffff800) +
                       ARINC_SSM_VALID + ARINC_PALT_LABEL;

// Altitude rate (feet/min) is computed as the change from the last altitude
// times SECS_PER_MIN, since this routine is called once per second.
  rate = (alt - last_alt) * (float)SECS_PER_MIN;
  tas_alt[HW_ALT_RATE_IDX] = 
                ((int)((rate / 32768.0) * 2147483648.0) & 0xfffff800) +
                ARINC_SSM_VALID + ARINC_PALT_RATE_LABEL;

  last_alt = alt;			// save the altitude value

//printf ("tas = %f, alt = %f, rate = %f\n", tas, alt, rate);
//printf ("tas_alt[] = %X %X %X\n", tas_alt[0], tas_alt[1], tas_alt[2]);
}

/*****************************************************************************/
char *HwIrs::buffer()
{
  long	*arinc_data, hz50_cnt, label;
  bool	firstLabelSet = false;

  memset((char *)&irs_blk, 0, sizeof(Irs_blk));

//cntr[0] = 0;
  for (	arinc_data = ring[gtog], hz50_cnt = -1;
	hz50_cnt < 50 && *arinc_data;
	++arinc_data)
    {
    label = *arinc_data & ARINC_LABEL_MASK;

//++cntr[0];
    ++cntr[label];

    switch (label) {  	// switch on the arinc label
 
      // The 50 Hz parameters 
      case 0265:
        irs_blk.integ_vert_accel[hz50_cnt] = *arinc_data;
        ++hz50_cnt;
        firstLabelSet = true;
        break;

      case 0324:
        irs_blk.pitch_angle[hz50_cnt] = *arinc_data;
        cur_pitch = *arinc_data;
        if (!firstLabelSet) {
          ++hz50_cnt;
          firstLabelSet = true;
          }
        break;

      case 0325:
        irs_blk.roll_angle[hz50_cnt] = *arinc_data;
        cur_roll = *arinc_data;
        if (!firstLabelSet) {
          ++hz50_cnt;
          firstLabelSet = true;
          }
        break;

      case 0326:
        irs_blk.pitch_rate[hz50_cnt] = *arinc_data;
        if (!firstLabelSet) {
          ++hz50_cnt;
          firstLabelSet = true;
          }
        break;

      case 0327:
        irs_blk.roll_rate[hz50_cnt] = *arinc_data;
        if (!firstLabelSet) {
          ++hz50_cnt;
          firstLabelSet = true;
          }
        break;

      case 0330:
        irs_blk.yaw_rate[hz50_cnt] = *arinc_data;
        break;

      case 0331:
        irs_blk.long_accel[hz50_cnt] = *arinc_data;
        break;

      case 0332:
        irs_blk.lat_accel[hz50_cnt] = *arinc_data;
        break;

      case 0333:
        irs_blk.normal_accel[hz50_cnt] = *arinc_data;
        break;

      case 0335:
        irs_blk.track_ang_rate[hz50_cnt] = *arinc_data;
        firstLabelSet = false;
        break;

      case 0336:
        irs_blk.pitch_att_rate[hz50_cnt] = *arinc_data;
        firstLabelSet = false;
        break;

      case 0337:
        irs_blk.roll_att_rate[hz50_cnt] = *arinc_data;
        firstLabelSet = false;
        break;

      case 0360:
        irs_blk.pot_vert_speed[hz50_cnt] = *arinc_data;
        firstLabelSet = false;
        break;
  
      case 0364:
        irs_blk.vertical_accel[hz50_cnt] = *arinc_data;
        firstLabelSet = false;
        break;
 

      // 25 Hz parameters 
      case 0313:				// first 25 Hz param
        irs_blk.track_angle_true[hz50_cnt >> 1] = *arinc_data;
        break;

      case 0365:
        irs_blk.inrt_vert_speed[hz50_cnt >> 1] = *arinc_data;
        break;

      case 0361:
        irs_blk.inertial_alt[hz50_cnt >> 1] = *arinc_data;
        cur_inertial_alt = *arinc_data;
        break;

      case 0321:
        irs_blk.drift_angle[hz50_cnt >> 1] = *arinc_data;
        cur_drift = *arinc_data;
        break;

      case 0314:
        irs_blk.true_heading[hz50_cnt >> 1] = *arinc_data;
        cur_true_heading = *arinc_data;
        break;

 
      // The 10 Hz parameters
      case 0312:				// first 10 Hz param
        irs_blk.ground_speed[hz50_cnt / 5] = *arinc_data;
        cur_gnd_speed = *arinc_data;
        break;

      case 0315:
        irs_blk.wind_speed[hz50_cnt / 5] = *arinc_data;
        break;

      case 0316:
        irs_blk.wind_dir_true[hz50_cnt / 5] = *arinc_data;
        break;

      case 0366:
        irs_blk.velocity_ns[hz50_cnt / 5] = *arinc_data;
        break;

      case 0367:
        irs_blk.velocity_ew[hz50_cnt / 5] = *arinc_data;
        break;

      case 0334:
        irs_blk.platform_hdg[hz50_cnt / 5] = *arinc_data;
        break;

 
      // 5 Hz parameters
      case 0310:				// first 5 Hz param
        irs_blk.present_lat[hz50_cnt / 10] = *arinc_data;
        cur_lat = *arinc_data;
        break;

      case 0311:
        irs_blk.present_lon[hz50_cnt / 10] = *arinc_data;
        cur_lon = *arinc_data;
        break;

      case 0370:
        irs_blk.norm_accel[hz50_cnt / 10] = *arinc_data;
        break;

 
      // 2 Hz parameters
      case 0350:
        irs_blk.irs_maint_discretes[hz50_cnt / 25] = *arinc_data;
        break;

      case 0270:
        irs_blk.irs_discretes[hz50_cnt / 25] = *arinc_data;
        break;

      case 0371:
        irs_blk.equipment_id[hz50_cnt / 25] = *arinc_data;
        break;

      case 0351:
        irs_blk.time_to_nav_ready[hz50_cnt / 25] = *arinc_data;
        break;
      }
    }

//logMsg("Total cnt = %d\n", cntr[0],0,0,0,0,0);

for (int i=180; i<250; ++i)
  if (cntr[i] > 0)
    {
    switch (i)
      {
      case 0265: case 0323: case 0324: case 0325: case 0326: case 0327:
      case 0330: case 0331: case 0332: case 0333: case 0335: case 0336:
      case 0354: case 0360: case 0362: case 0363: case 0364:
        if (cntr[i] != 50)
          logMsg("%d:%d:%d  cntr[%o]=%d, should be 50\n",
		tfp.hour(), tfp.minute(), tfp.second(), i, cntr[i], 0);
        break;

      case 0313: case 0314: case 0317: case 0320: case 0321: case 0322:
      case 0361: case 0365:
        if (cntr[i] != 25)
          logMsg("%d:%d:%d  cntr[%o]=%d, should be 25\n",
		tfp.hour(), tfp.minute(), tfp.second(), i, cntr[i], 0);
        break;

      case 0312: case 0315: case 0316: case 0334: case 0366: case 0367:
        if (cntr[i] != 10)
          logMsg("%d:%d:%d  cntr[%o]=%d, should be 10\n",
		tfp.hour(), tfp.minute(), tfp.second(), i, cntr[i], 0);
        break;

      case 0310: case 0311: case 0370:
        if (cntr[i] != 5)
          logMsg("%d:%d:%d  cntr[%o]=%d, should be 5\n",
		tfp.hour(), tfp.minute(), tfp.second(), i, cntr[i], 0);
        break;

      }

    cntr[i]=0;
    }

  return((char *)&irs_blk);

}

/* END HWIRS.CC */

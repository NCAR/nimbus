/* AnalogCal.cc

   Class for controling analog calibration and diagnostics.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <AnalogCal.h>

/******************************************************************************
** Public Functions
******************************************************************************/
 
AnalogCal::AnalogCal (TapeHeader &hdr,  Dsp56002 *dp, DsmConfig &dc, 
                      DsmMessage &msg) : 
                      dsm_hdr(hdr), dsm_config (dc), dsm_msg (msg)
{
  dsp = dp;
  curMode = ANALOG_RUN_MODE;
  calVolt = ANALOG_V0_SELECT;
  calState = CAL_STATE_SELECT_CHANNELS;
  calSampleCount = 0;
  calSettleCount = 0;
}

/*****************************************************************************/
void AnalogCal::checkCal(const char *const buf)

// Controls the auto-cal sequence.  This routine is called once per second.
// buf is a pointer the the current 1 second data block.
{

// Don't do anything if not currently in the calibration or QC mode.
  if (curMode != ANALOG_CAL_MODE && curMode != ANALOG_QC_MODE)
    return;


  switch (calState) {
    case CAL_STATE_SELECT_CHANNELS:
      dsp->ana_tbl.resetCalTable();	// clear existing cal table entries
      setCalVoltByGain();		// set the cal voltage
      calState = CAL_STATE_SETTLE;

      // enable cal on selected channels
      if (enableCalByVoltage() == 0 && nextCalVoltage() != ANALOG_V0_SELECT)
        calState = CAL_STATE_SELECT_CHANNELS;

      calSampleCount = 0;
      calSettleCount = 0;
      return;

    case CAL_STATE_SETTLE:		// allow channels to settle
      if (++calSettleCount >= CAL_MAX_SETTLE)
        calState = CAL_STATE_SAMPLE;
      return;

    case CAL_STATE_SAMPLE:
      if (collectSamples(buf)) {	// collect samples
        nextCalVoltage();		// select next gain

        if (calState != CAL_STATE_DONE)
          calState = CAL_STATE_SELECT_CHANNELS;	// start next cal
      }
      return;

    case CAL_STATE_DONE:
      computeFactors();			// compute the conversion factors

      if (curMode == ANALOG_CAL_MODE) {
        updateHeader();			// update header with new factors
        dsm_hdr.writeFile(dsm_config.dsmHeaderName()); // write to disk
      }
      else {
        int stat;
        FILE *fp;
        char buff[256];

        sprintf(buff, "%s/hosts/%s/qc.%s", getenv("PROJ_DIR"),
                sysBootParams.hostName,
                dsm_config.location());

        if ((fp = fopen(buff, "a")) == NULL)
          fprintf(stderr, "AnalogCal: fopen failure; %s\n", buff);
        else {
          for (stat = dsm_hdr.firstDesc(); stat; stat = dsm_hdr.nextDesc()) {
            if (dsm_hdr.analogType()) {
              fprintf(fp, "%-8s  %-8s  %2x  %8.4f  %8.4f\n",
                          dsm_config.location(),
                          dsm_hdr.name(),
                          dsm_hdr.adsaddr(),
                          zeroSamples[dsm_hdr.adsaddr()],
                          spanSamples[dsm_hdr.adsaddr()]);
            }
          }
          fclose(fp);
        }
      }

      setRunMode();			// return to normal running
      return;

    default:
      fprintf (stderr, "AnalogCal: Unknown calState, %d\n", calState);
      return;
  }
}
/*****************************************************************************/

void AnalogCal::control (int action, int adr, int volt, int gain, int offset)

// Sets the mode from the passed in parameters.
{
//  printf (
//    "AnalogCal, action= %2d, adr= %2d, volt= %2d, gain= %2d, offset= %2d.\n",
//     action, adr, volt, gain, offset);

  switch (action) {
    case ANALOG_RUN_MODE:
      setRunMode();
      break;

    case ANALOG_QC_MODE:
      setQCmode();
      break;

    case ANALOG_CAL_MODE:
      setCalMode();
      break;

    case ANALOG_DIAG_MODE:
      setDiagnosticMode(adr, volt, gain, offset);
      break;

    default:
      fprintf(stderr, "AnalogCal, invalid action received, %d\n", action);
      return;
  }
  curMode = action;
}

/******************************************************************************
** Private Functions
******************************************************************************/
void AnalogCal::setRunMode()

// Sets the Dsp back to the normal sampling mode.
{
  dsp->ana_tbl.resetCalTable();		// clear cal table entries
  dsp->assignTable();			// assign sample table to dsp
  curMode = ANALOG_RUN_MODE;

// Send a message to indicate that the mode has been set to run.
  sprintf(msg, "Analog run mode set on the dsm at location %s.\n",
          dsm_config.location());
 
  dsm_msg.sendAnalogMsg(ANALOG_RUN_MODE, dsm_config.location(), 0,
                        ANALOG_V10_SELECT, ANALOG_GAIN_1_SELECT,
                        ANALOG_OFFSET_0_SELECT, msg);
}

/*****************************************************************************/
void AnalogCal::setQCmode()

// Initializes the QC sequence.
{
  curMode = ANALOG_QC_MODE;
  calVolt = ANALOG_V0_SELECT;
  calState = CAL_STATE_SELECT_CHANNELS;
  calSampleCount = 0;
  calSettleCount = 0;
  memset((char*)zeroSamples, 0, sizeof(zeroSamples));
  memset((char*)spanSamples, 0, sizeof(spanSamples));

// Send a message to indicate the calibration has started.
  sprintf(msg, "Analog QC started at location %s.\n", dsm_config.location());

  dsm_msg.sendAnalogMsg(ANALOG_QC_MODE, dsm_config.location(), 0,
                        ANALOG_V10_SELECT, ANALOG_GAIN_1_SELECT,
                        ANALOG_OFFSET_0_SELECT, msg);

  printf("AnalogCal: location = %s, %s\n", dsm_config.location(), msg);

}

/*****************************************************************************/
void AnalogCal::setCalMode()

// Initializes the auto-calibration sequence.
{
  dsp->ana_tbl.resetCalTable();		// clear cal table entries
  curMode = ANALOG_CAL_MODE;
  calVolt = ANALOG_V0_SELECT;
  calState = CAL_STATE_SELECT_CHANNELS;
  calSampleCount = 0;
  calSettleCount = 0;
  memset((char*)zeroSamples, 0, sizeof(zeroSamples));
  memset((char*)spanSamples, 0, sizeof(spanSamples));

// Send a message to indicate the calibration has started.
  sprintf (msg, "Analog calibration started at location %s.\n",
           dsm_config.location());

  dsm_msg.sendAnalogMsg (ANALOG_CAL_MODE, dsm_config.location(), 0,
                         ANALOG_V10_SELECT, ANALOG_GAIN_1_SELECT,
                         ANALOG_OFFSET_0_SELECT, msg);

  printf("AnalogCal: location = %s, %s\n", dsm_config.location(), msg);

}

/*****************************************************************************/
void AnalogCal::setDiagnosticMode(int adr, int volt, int gain, int offset)

// Sets the diagnostic mode from the current message parameters.
{
printf("%d %d %d %d\n", adr, volt, gain, offset);
  dsp->ana_tbl.resetCalTable();		// clear existing cal table entries
  dsp->ana_tbl.selectByAddress(adr);	// select the channel
  dsp->ana_tbl.setVolt(volt);		// set cal voltage

  dsp->ana_tbl.setGain(gain);		// set channel gain
  dsp->ana_tbl.setOffset(offset);	// set channel offset
  dsp->ana_tbl.enableCal();		// enable channel cal
  dsp->assignCalTable();		// assign cal table to dsp
  curMode = ANALOG_DIAG_MODE;
}

/*****************************************************************************/
void AnalogCal::setCalVoltByGain ()

// Set the calibration voltage based upon the current calibration gain.
{
  switch (calVolt) {
    case ANALOG_V0_SELECT:		// cal all channels at 0 volts
      dsp->ana_tbl.setVolt(ANALOG_V0_SELECT);
      printf("AnalogCal::setCalVoltByGain() V0 selected.\n");
      break;
 
    case ANALOG_V10N_SELECT:		// cal gain of 1 channels at -10 volts
      dsp->ana_tbl.setVolt(ANALOG_V10N_SELECT);
      printf("AnalogCal::setCalVoltByGain() V-10 selected.\n");
      break;

    case ANALOG_V10_SELECT:		// cal gain of 1 channels at 10 volts
      dsp->ana_tbl.setVolt(ANALOG_V10_SELECT);
      printf("AnalogCal::setCalVoltByGain() V10 selected.\n");
      break;
 
    case ANALOG_V5_SELECT:		// cal gain of 4 channels at 2.5 volts
      dsp->ana_tbl.setVolt(ANALOG_V5_SELECT);
      printf("AnalogCal::setCalVoltByGain() V5 selected.\n");
      break;
 
    case ANALOG_V1_SELECT:		// cal gain of 20 channels at 1 volt
      dsp->ana_tbl.setVolt(ANALOG_V1_SELECT);
      printf("AnalogCal::setCalVoltByGain() V1 selected.\n");
      break;
  }
}

/*****************************************************************************/
int AnalogCal::enableCalByVoltage()

// Enables calibration on all channels whose gain matches the current gain.
{
  int  stat;

  enabledCount = 0;

  printf("  Enabling: ");

  for (stat = dsm_hdr.firstDesc(); stat; stat = dsm_hdr.nextDesc()) {
    if (dsm_hdr.analogType()) {
      enabled[dsm_hdr.adsaddr()] = FALSE;

      switch (calVolt) {
        case ANALOG_V0_SELECT:
          if (dsm_hdr.ch_gain() != 1)
            enabled[dsm_hdr.adsaddr()] = TRUE;
          break;

        case ANALOG_V10N_SELECT:
          if (dsm_hdr.ch_gain() == 1)
            enabled[dsm_hdr.adsaddr()] = TRUE;
          break;

        case ANALOG_V1_SELECT:
          if (dsm_hdr.ch_gain() == 20)
            enabled[dsm_hdr.adsaddr()] = TRUE;
          break;

        case ANALOG_V5_SELECT:
          if (dsm_hdr.ch_gain() == 4 ||
             (dsm_hdr.ch_gain() == 2 && dsm_hdr.ch_offset() == 0))
            enabled[dsm_hdr.adsaddr()] = TRUE;
          break;

        case ANALOG_V10_SELECT:
          if (dsm_hdr.ch_gain() == 1 ||
             (dsm_hdr.ch_gain() == 2 && dsm_hdr.ch_offset() == 10))
            enabled[dsm_hdr.adsaddr()] = TRUE;
          break;
      }

      if (enabled[dsm_hdr.adsaddr()]) {
        printf("%d, ", dsm_hdr.adsaddr());
        dsp->ana_tbl.selectByAddress(dsm_hdr.adsaddr()); // select the channel
        dsp->ana_tbl.enableCal();			// enable channel cal
        ++enabledCount;					// increment count
      }
    }
  }

  dsp->assignCalTable();		// assign cal table to dsp
  printf("\n");

  return(enabledCount);			// return number of channels selected

}

/*****************************************************************************/
int AnalogCal::nextCalVoltage()
{
  switch (calVolt) {
    case ANALOG_V0_SELECT:
      return calVolt = ANALOG_V10N_SELECT;
 
    case ANALOG_V10N_SELECT:
      return calVolt = ANALOG_V1_SELECT;
 
    case ANALOG_V1_SELECT:
      return calVolt = ANALOG_V5_SELECT;
 
    case ANALOG_V5_SELECT:
      return calVolt = ANALOG_V10_SELECT;
 
    case ANALOG_V10_SELECT:
      calState = CAL_STATE_DONE;
      return calVolt = ANALOG_V0_SELECT;
  }
}

/*****************************************************************************/
int AnalogCal::collectSamples (const char *const buf)

// Collects cal samples from the passed in 1 second data block.
// Returns TRUE when CAL_MAX_SAMPLES have been collected.
{
  int stat;
  int j;
  short temp;

 if (curMode == ANALOG_CAL_MODE)
  for (stat = dsm_hdr.firstDesc(); stat; stat = dsm_hdr.nextDesc()) {
    if (dsm_hdr.analogType() && enabled[dsm_hdr.adsaddr()]) {

      // Check if collecting zeroSamples else spanSamples
      if (calVolt == ANALOG_V0_SELECT || calVolt == ANALOG_V10N_SELECT)
        for (j = 0; j < RATE_5; j++){		// take first 5 samples
          temp = *(short*)(buf + dsm_hdr.start() + dsm_hdr.offset() * j);
          if (j == 0) printf("analog = %d\n",temp);
          zeroSamples[dsm_hdr.adsaddr()] += temp;
//          zeroSamples[dsm_hdr.adsaddr()] += *(short*)(buf + dsm_hdr.start() + 
//						dsm_hdr.offset() * j);
        }
      else
        for (j = 0; j < RATE_5; j++) {
          temp = *(short*)(buf + dsm_hdr.start() + dsm_hdr.offset() * j);
          if (j == 0) printf("analog = %d\n",temp);
          spanSamples[dsm_hdr.adsaddr()] += temp;
//          spanSamples[dsm_hdr.adsaddr()] += *(short*)(buf + dsm_hdr.start() + 
//						dsm_hdr.offset() * j);
        }
    }
  }


 if (curMode == ANALOG_QC_MODE)
  for (stat = dsm_hdr.firstDesc(); stat; stat = dsm_hdr.nextDesc()) {
    if (dsm_hdr.analogType() && enabled[dsm_hdr.adsaddr()]) {

      // Check if collecting zeroSamples else spanSamples
      if (calVolt == ANALOG_V0_SELECT || calVolt == ANALOG_V10N_SELECT)
        for (j = 0; j < RATE_5; j++)		// take first 5 samples
          zeroSamples[dsm_hdr.adsaddr()] += (*(short*)(buf + dsm_hdr.start() + 
		dsm_hdr.offset() * j) - dsm_hdr.getShConvertOffset()) *
		dsm_hdr.convert();
      else
        for (j = 0; j < RATE_5; j++)
          spanSamples[dsm_hdr.adsaddr()] += (*(short*)(buf + dsm_hdr.start() + 
		dsm_hdr.offset() * j) - dsm_hdr.getShConvertOffset()) *
		dsm_hdr.convert();
    }
  }

  return(++calSampleCount >= CAL_MAX_SAMPLES);

}

/*****************************************************************************/
void AnalogCal::computeFactors ()

// Computes analog cal factors.
{
  int stat;
  int adr;

  for (stat = dsm_hdr.firstDesc(); stat; stat = dsm_hdr.nextDesc()) {
    if (dsm_hdr.analogType()) {
      adr = dsm_hdr.adsaddr();

      // Compute sample averages.
      zeroSamples[adr] /= CAL_MAX_SAMPLES * RATE_5;
      spanSamples[adr] /= CAL_MAX_SAMPLES * RATE_5;

      if (curMode == ANALOG_QC_MODE)
        continue;

      if (dsm_hdr.ch_gain() == 2 && dsm_hdr.ch_offset() == 0)
        spanFactors[adr] = 10.0 / (spanSamples[adr] - zeroSamples[adr]);
      else
        spanFactors[adr] = 20.0 / (spanSamples[adr] - zeroSamples[adr]);
      

      if (dsm_hdr.ch_gain() == 2 && dsm_hdr.ch_offset() == 0)
        ;
      else
        zeroSamples[adr] = (spanSamples[adr] + zeroSamples[adr]) / 2.0;

      printf ("zeroFactors[] = %f, spanFactors[] = %f\n", 
              zeroSamples[adr], spanFactors[adr]);
    }
  }
}

/*****************************************************************************/
void AnalogCal::updateHeader()
{
  int stat;

  // Update convert and convert_offset fields in the header.

  for (stat = dsm_hdr.firstDesc(); stat; stat = dsm_hdr.nextDesc()) {
    if (dsm_hdr.analogType()) {
      dsm_hdr.setShConvert (spanFactors[dsm_hdr.adsaddr()]);
      dsm_hdr.setShConvertOffset ((int)(zeroSamples[dsm_hdr.adsaddr()] + 0.5));
    }
  }
}

/* END ANALOGCAL.CC */

/* AnalogCal.h
   Class for controling analog calibration and diagnostics.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef ANALOGCAL_H
#define ANALOGCAL_H

#include <messageDefs.h>
#include <AnalogTable.h>
#include <DsmConfig.h>
#include <DsmMessage.h>
#include <Dsp56002.h>
#include <TapeHeader.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAL_STATE_SELECT_CHANNELS	0
#define CAL_STATE_SETTLE		1
#define CAL_STATE_SAMPLE		2
#define CAL_STATE_DONE			3

#define CAL_MAX_SAMPLES			8	// number of seconds to avg
#define CAL_MAX_SETTLE			5	// number of seconds to settle

class AnalogCal {
public:
  AnalogCal (TapeHeader &hdr, Dsp56002 *dp, DsmConfig &dc, DsmMessage &msg);	
  void checkCal (const char *const buf);	// checks cal status
  void control (int action, int adr, int volt, int gain, int offset);
 
private:
  void setRunMode();				// sets dsp to sampling mode
  void setCalMode();				// inits auto cal mode
  void setQCmode();				// check cals.
  void setDiagnosticMode(int adr, int volt, int gain, int offset);

  void setCalVoltByGain();			// set cal volt by gain
  int enableCalByVoltage();			// enables cals by gain
  int nextCalVoltage();				// select next gain value
  int collectSamples (const char *const buf);	// collect cal samples
  void computeFactors();			// compute conversion factors
  void updateHeader();				// update factors in the header

  Dsp56002 *dsp;				// M56002 dsp board class
  TapeHeader &dsm_hdr;				// tape header class
  DsmConfig &dsm_config;			// dsm configuration class
  DsmMessage &dsm_msg;				// message handler class

  int curMode;					// current cal mode
  int calVolt;					// current cal gain	
  int calState;					// current cal state
  int calSampleCount;				// number of samples taken
  int calSettleCount;				// number seconds for settling
  int enabled[ANA_MAX_CHAN];			// Current list of enabled addrs
  int enabledCount;
  float zeroSamples[ANA_MAX_CHAN];		// averaged zero samples array
  float spanSamples[ANA_MAX_CHAN];		// averaged span samples array
  float spanFactors[ANA_MAX_CHAN];		// computed span factors 
  char msg[DSM_MSG_MAX_LEN];			// message string

};

#endif


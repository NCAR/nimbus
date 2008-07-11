/* DigOut.h

   DigOut control class.

   Original Author: Christopher J. Webster
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef DIGOUT_H
#define DIGOUT_H

#include <Vmio12.h>
#include <Vmio14Out.h>
#include <DsmMessage.h>
#include <TextFile.h>

#include <string.h>

#include <dsmctl.h>
#include <dsmVmeDefs.h>
#include <messageDefs.h>

#define MAX_CHANNELS	8

class DigOut {
public:
  DigOut (char host[], char target[], Vmio12 *vmio12[], DsmMessage& dm);
  void control(int action, int connector, int channel);	

private:
  void setChannel(int connector, int channel);
  void clearChannel(int connector, int channel);
  DsmMessage& dsm_msg;				// message handler class

  Vmio14Out *dig_out[MAX_CHANNELS];		// DigOut output port class
  int  curOut[MAX_CHANNELS];
};

#endif


/* DigOut.cc
   DigOut control class.

   Original Author: Christopher J. Webster
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <DigOut.h>

 
/* -------------------------------------------------------------------- */
DigOut::DigOut(char host[], char target[], Vmio12 *vmio12[], DsmMessage& dm) : dsm_msg (dm)
{
  int i, connector, nButtons;
  char *line[128], fileName[256];
  TextFile digOuts;

  sprintf(fileName, "%s/hosts/%s/DigOuts", getenv("PROJ_DIR"), host);
  nButtons = digOuts.readTextFile(fileName, line);
 
  memset(dig_out, 0, sizeof(dig_out));
  memset(curOut, 0, sizeof(curOut));

  for (i = 0; i < nButtons; ++i) {
    if (strcmp(target, strtok(line[i], ",\t ")) == 0) {
      if (!(int)vmio12[0]) {
        vmio12[0] = new Vmio12((char*)(A24D16_BASE + VMIO12_BASE_0));
      }

      connector = atoi(strtok(NULL, ",\t "));

      if (dig_out[connector] == NULL) {
        dig_out[connector] = new Vmio14Out(vmio12[0], connector);

        dig_out[connector]->writePort(curOut[connector]);
      }
    }
  }
 
  digOuts.freeTextFile(line);
 
}

/* -------------------------------------------------------------------- */
void DigOut::control(int action, int connector, int channel)
{
  if (action)
    setChannel(connector, channel);
  else
    clearChannel(connector, channel);
}

/******************************************************************************
** Private Functions
******************************************************************************/

/* -------------------------------------------------------------------- */
void DigOut::setChannel(int connector, int channel)
{
  int	mask = 0x000000ff;

  curOut[connector] |= (0x01 << channel);

  if (connector > 7)
    mask = 0x0000000f;

  dig_out[connector]->writePort(curOut[connector] & mask);
}

/* -------------------------------------------------------------------- */
void DigOut::clearChannel(int connector, int channel)
{
  int	mask = 0x000000ff;

  curOut[connector] &= ~(0x01 << channel);

  if (connector > 7)
    mask = 0x0000000f;

  dig_out[connector]->writePort(curOut[connector] & mask);
}

/* Aerosol.cc
 
   NOAA CMDL (John Ogren) instrument  class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <Aerosol.h>

/******************************************************************************
** Public Functions
******************************************************************************/

Aerosol::Aerosol (const char *base, const int port, 
                void (*statMsg)(const char* msg)) :
                IsioPort (base, port, AEROSOL_BAUD, AEROSOL_BITS, 
                AEROSOL_PARITY, AEROSOL_STOP, AEROSOL_FLOW, statMsg), 
                statusMsg (statMsg)

// Constructor. The interrupt routine must be previously set up outside this 
// class.
{
  ptog = 0;
  gtog = 0;
  memset (buf, 0, sizeof (buf));
  term_str[0] = '\r';
  term_str[1] = '\n';
//  term_str[2] = '\0';
  igetstr (term_str);			// issue first read
}
/*****************************************************************************/

void Aerosol::checkStatus()

// Check the serial port status.
{
  if (checkPort() != TRUE) {
    igetstr (term_str);         // new read if an error or dropped interrupt
    statusMsg ("Aerosol: Dropped isio interrupt.\n");
  } 
}
/*****************************************************************************/
 
void Aerosol::secondAlign ()
 
// This routine is to be called at each 1 second clock tick. The Aerosol_blk
// buffers are toggled.
{
 
  gtog = ptog;
  ptog = 1 - ptog;
  memset((char *)&aerosol_blk[ptog], 0, sizeof(aerosol_blk[ptog]));
 
}
/*****************************************************************************/

void Aerosol::isr()
 
// Pulls in data from the isio and builds the Aerosol data struct.
{
  char *datap;
  char *p;
  float *fp = (float *)&aerosol_blk[ptog];
  int i;

// Check for data from the isio port.
  if (!(int)(datap = getData()))
    return;

// Pull in the data.
  memcpy (buf, datap, strlen(datap));

  p = strtok(buf,", \r\n");
  for (i = 0; i < AEROSOL_NUM_PARMS; ++i) {
    fp[i] = atof(p);
    p = strtok(NULL,", \r\n");
//    logMsg("fp[i] = %d, i = %d\n",(int)fp[i],i,0,0,0,0);
  }

  igetstr (term_str);                        // issue next read

}

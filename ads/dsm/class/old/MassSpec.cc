/* MassSpec.cc
 
   Mass Spec serial class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <MassSpec.h>

/******************************************************************************
** Public Functions
******************************************************************************/

MassSpec::MassSpec(const char *base, const int port, 
                void (*statMsg)(const char* msg)) :
                IsioPort (base, port, MASSPEC_BAUD, MASSPEC_BITS, 
                MASSPEC_PARITY, MASSPEC_STOP, MASSPEC_FLOW, statMsg), 
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

void MassSpec::checkStatus()

// Check the serial port status.
{
  if (checkPort() != TRUE) {
    igetstr (term_str);         // new read if an error or dropped interrupt
    statusMsg ("MassSpec: Dropped isio interrupt.\n");
  } 
}
/*****************************************************************************/
 
void MassSpec::secondAlign ()
 
// This routine is to be called at each 1 second clock tick. The MassSpec_blk
// buffers are toggled.
{
 
  gtog = ptog;
  ptog = 1 - ptog;
  memset((char *)&mass_blk[ptog], 0, sizeof(mass_blk[ptog]));

} 
/*****************************************************************************/

void MassSpec::isr()
 
// Pulls in data from the isio and builds the mass spec data struct.
{
  char *datap;
  char *p;
  int i;

// Check for data from the isio port.
  if (!(int)(datap = getData()))
    return;

// Pull in the data.
  memcpy (buf, datap, strlen(datap));

  p = strtok(buf,", \r\n");
  mass_blk[ptog].mass1 = atof(p);

  p = strtok(NULL,", \r\n");
  mass_blk[ptog].mass2 = atof(p);

  p = strtok(NULL,", \r\n");
  mass_blk[ptog].mass3 = atof(p);

  p = strtok(NULL,", \r\n");
  mass_blk[ptog].mass4 = atof(p);

  igetstr (term_str);                        // issue next read
}
/******************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

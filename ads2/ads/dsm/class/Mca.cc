/* Mca.cc
 
   MCA  class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <Mca.h>

/******************************************************************************
** Public Functions
******************************************************************************/

Mca::Mca (const char *base, const int port, 
                void (*statMsg)(char* msg)) :
                IsioPort (base, port, MCA_BAUD, MCA_BITS, MCA_PARITY,
                MCA_STOP, MCA_FLOW, statMsg), statusMsg (statMsg)

// Constructor. The interrupt routine must be previously set up outside this 
// class.
{
  int j;

  ptog = 0;
  gtog = 0;
  memset (buf, 0, sizeof (buf));

  for (j = 0; j <= 2; j++) command[j] = 0;
  command[3] = 3;				// baud rate = 38K
  command[4] = 3;
  pputcnt((char*)command,MCA_CMD_LEN);
  baudRate(38000);				// set port baud rate
//  printf("Mca port = %d\n",port);

}
/*****************************************************************************/
void Mca::initMca()
{
  int j;
/*
// Send data command.

  for (j = 0; j <= 4; j++) command[j] = 0;
  pputcnt((char*)command,MCA_CMD_LEN);
  igetcnt (MCA_DATA_LEN);			// issue first read
*/
// Send delete data command.
  command[0] = 5;
  command[1] = 1;
  command[2] = 0;
  command[3] = 1;
  command[4] = 7;
  pputcnt((char*)command,MCA_CMD_LEN);

// Send start command.
  command[0] = 1;
  command[1] = 0x1E;
  command[2] = 10;
  command[3] = 0;
  command[4] = 0x2F;
  pputcnt((char*)command,MCA_CMD_LEN);

}
/*****************************************************************************/

void Mca::checkStatus()

// Check the serial port status.
{
  if (checkPort() != TRUE) {
    statusMsg ("Mca: Dropped isio interrupt.\n");
  } 
}
/*****************************************************************************/
void Mca::secondAlign()
 
// Called at the start of a second to align the indices and toggle buffers.
{
  int i;

  gtog = ptog;                          // toggle the get buffer index
  ptog = 1 - gtog;

}
/*****************************************************************************/
void Mca::reqstData ()

// This routine is to be called 1 times per second .
{
  int j;

// Send stop command.

  command[0] = 1;
  command[1] = 0x0E;
  command[2] = 10;
  command[3] = 0;
  command[4] = 0x1F;
  pputcnt((char*)command,MCA_CMD_LEN);

// Send data command.

  for (j = 0; j <= 4; j++) command[j] = 0;
  pputcnt((char*)command,MCA_CMD_LEN);

  igetcnt (MCA_DATA_LEN);			// issue first read

}

/*****************************************************************************/

void Mca::isr()
 
// Pulls in data from the isio and builds the Mca data struct.
{
  char *datap;

  logMsg("In Mca ISR 1\n",0,0,0,0,0,0);
// Check for data from the isio port.
  if (!(int)(datap = getData()))
    return;
 
  logMsg("In MCA ISR 2\n",0,0,0,0,0,0);
// Pull in the data.
  memcpy(mca_status[ptog].DataChkSum, datap, MCA_STATUS_LEN);
  memcpy(mca_blk[ptog].counts, &datap[MCA_STATUS_LEN], MCA_DATA_LEN);

// Now delete data and start again. 
  command[0] = 5;
  command[1] = 1;
  command[2] = 0;
  command[3] = 1;
  command[4] = 7;
  pputcnt((char*)command,MCA_CMD_LEN);

  command[0] = 1;
  command[1] = 0x1E;
  command[2] = 10;
  command[3] = 0;
  command[4] = 0x2F;
  pputcnt((char*)command,MCA_CMD_LEN);

}

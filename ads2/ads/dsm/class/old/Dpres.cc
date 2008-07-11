/* Dpres.cc
 
   Digital Pressure  class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <Dpres.h>

/******************************************************************************
** Public Functions
******************************************************************************/

Dpres::Dpres (const char *base, const int port, 
                void (*statMsg)(const char* msg)) :
                IsioPort (base, port, DPRES_BAUD, DPRES_BITS, DPRES_PARITY,
                DPRES_STOP, DPRES_FLOW, statMsg), statusMsg (statMsg)

// Constructor. The interrupt routine must be previously set up outside this 
// class.
{
  ptog = 0;
  gtog = 0;
  idx = 0;
  memset (buf, 0, sizeof (buf));

  term_str[0] = '\r';
  term_str[1] = '\n';
  term_str[2] = '\0';
  start_press[0] = '$';
  start_press[1] = 'M';
// 'R' requests a single sample, 'C' specifies continuous transmission
  start_press[2] = 'R';
//  start_press[2] = 'C';
  start_press[3] = '\r';

//  pputcnt ((char*)start_press, 4);	// command continuous pressure
//  clrbuf();
//  igetstr (term_str);			// issue first read
}
/*****************************************************************************/

void Dpres::checkStatus()

// Check the serial port status.
{
  if (checkPort() != TRUE) {
//    igetstr (term_str);         // new read if an error or dropped interrupt
    statusMsg ("Dpres: Dropped isio interrupt.\n");
  } 
}
/*****************************************************************************/
 
void Dpres::secondAlign ()
 
// This routine is to be called at each 1 second clock tick. The Dpres_blk
// buffers are toggled.
{
//  if (idx != 5)
//    logMsg("Dpres: idx == %d\n", idx,0,0,0,0,0);

  idx=0;
  gtog = ptog;
  ptog = 1 - ptog;
  memset((char *)&dpres_blk[ptog], 0, sizeof(dpres_blk[ptog]));

}
/*****************************************************************************/
 
void Dpres::reqstData ()
 
// This routine is to be called 5 times per second .
{
  pputcnt ((char*)start_press, 4);	// command one pressure
  igetstr (term_str); 

}
/*****************************************************************************/

void Dpres::isr()
 
// Pulls in data from the isio and builds the Dpres data struct.
{
  char *datap;
  float press;
  char status[20] = "";

// Check for data from the isio port.
  if (!(int)(datap = getData()) || idx > 4)
    return;
 
// Pull in the data.
  memcpy (buf, datap, strlen(datap));
 
// Check the data alignment.
  if ((buf[DPRES_SYNC_IDX]==DPRES_SYNC)) {
    press = atof(buf);

    if((int)press == 0){
      if(idx == 0)
        dpres_blk[ptog].press[0] = dpres_blk[gtog].press[4];
      else
        dpres_blk[ptog].press[idx] = dpres_blk[ptog].press[idx-1];
    }
    else 
      dpres_blk[ptog].press[idx] = press;

//    if((int)press == 0)
//      logMsg ("Digital pressure = %d, idx = %d\n",(int)press,idx,0,0,0,0);
/*
    if(strlen(&status[1]) < 3){
      printf("STATUS: %s\n",*status);
      (char*)*status = "";
    }
*/
  } 
//  igetstr (term_str);                        // issue next read
  idx++;
}
/*****************************************************************************
** Private Functions
******************************************************************************/
/*****************************************************************************/

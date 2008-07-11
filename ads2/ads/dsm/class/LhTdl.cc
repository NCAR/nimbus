/* LhTdl.cc
 
   TDL  class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <LhTdl.h>

/******************************************************************************
** Public Functions
******************************************************************************/

LhTdl::LhTdl (const char *base, const int port, 
                void (*statMsg)(char* msg)) :
                IsioPort (base, port, LHTDL_BAUD, LHTDL_BITS, LHTDL_PARITY,
                LHTDL_STOP, LHTDL_FLOW, statMsg), statusMsg (statMsg)

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
}
/*****************************************************************************/
void LhTdl::initTdl()
{

  igetstr (term_str);			// issue first read

}
/*****************************************************************************/

void LhTdl::checkStatus()

// Check the serial port status.
{
  if (checkPort() != TRUE) {
    clrbuf();
//    igetstr (term_str);         // new read if an error or dropped interrupt
    statusMsg ("LhTdl: Dropped isio interrupt.\n");
  } 
}
/*****************************************************************************/
void LhTdl::secondAlign()
 
// Called at the start of a second to align the indices and toggle buffers.
{
  gtog = ptog;                          // toggle the get buffer index
  ptog = 1 - gtog;
  idx = 0;
 
}
/*****************************************************************************/
void LhTdl::zero()
{
 
  memset((char*)&lhtdl_blk[gtog], 0, sizeof(lhtdl_blk));
 
}
/*****************************************************************************/

void LhTdl::isr()
 
// Pulls in data from the isio and builds the JplTdl data struct.
{
  char *datap;
  int hk1, hk2, hk3, hk4;
  float ppm, vmr, relhum, temp, press;

// Check for data from the isio port.
  if (!(int)(datap = getData()) || idx > 3)
    return;
 
// Pull in the data.
  memcpy(buf, datap, strlen(datap));
 

// Parse the LHTDL 3 data string and build the recorded data block.
  sscanf (&buf[1], "%f %f %f %f %f %d %d %d %d", &ppm, &vmr, &relhum, &press,
            &temp, &hk1, &hk2, &hk3, &hk4);

  lhtdl_blk[ptog].tdllh[idx].ppmv = ppm;
  lhtdl_blk[ptog].tdllh[idx].vmr = vmr;
  lhtdl_blk[ptog].tdllh[idx].relhum = relhum;
  lhtdl_blk[ptog].tdllh[idx].press = press;
  lhtdl_blk[ptog].tdllh[idx].temp = temp;
  lhtdl_blk[ptog].tdllh[idx].pp2f = (float)hk1;
  lhtdl_blk[ptog].tdllh[idx].Pv0 = (float)hk2;
  lhtdl_blk[ptog].tdllh[idx].Indexmax = (float)hk3;
  lhtdl_blk[ptog].tdllh[idx].Yzero = (float)hk4;
  idx++;
  
  logMsg("vmr = %d, ppm = %d, relhum = %d, press = %d, temp = %d\n",
         (int)vmr,(int)ppm,(int)relhum,(int)press,(int)temp,0);
  if(checkPort() != TRUE) clrbuf();
  igetstr (term_str);                        // issue next read

}

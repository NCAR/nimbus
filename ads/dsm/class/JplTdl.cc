/* JplTdl.cc
 
   JPL TDL  class.
 
   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
 
*/

#include <JplTdl.h>

/******************************************************************************
** Public Functions
******************************************************************************/

JplTdl::JplTdl (const char *base, const int port, 
                void (*statMsg)(char* msg)) :
                IsioPort (base, port, JPLTDL_BAUD, JPLTDL_BITS, JPLTDL_PARITY,
                JPLTDL_STOP, JPLTDL_FLOW, statMsg), statusMsg (statMsg)

// Constructor. The interrupt routine must be previously set up outside this 
// class.
{
  ptog = 0;
  gtog = 0;
  idx = 0;
  memset (buf, 0, sizeof (buf));
  term_str[0] = '@';
  term_str[1] = '\n';
  term_str[2] = '\0';
//  igetstr (term_str);			// issue first read
}
/*****************************************************************************/
void JplTdl::initTdl()
{

  igetstr (term_str);			// issue first read

}
/*****************************************************************************/

void JplTdl::checkStatus()

// Check the serial port status.
{
  if (checkPort() != TRUE) {
    igetstr (term_str);         // new read if an error or dropped interrupt
    statusMsg ("JplTdl: Dropped isio interrupt.\n");
  } 
}
/*****************************************************************************/
void JplTdl::secondAlign()
 
// Called at the start of a second to align the indices and toggle buffers.
{
  gtog = ptog;                          // toggle the get buffer index
  ptog = 1 - gtog;
  idx = 0;
 
}
/*****************************************************************************/
void JplTdl::zero()
{
 
  memset((char*)&jpltdl_blk[gtog], 0, sizeof(Jpltdl_blk));
 
}
/*****************************************************************************/

void JplTdl::isr()
 
// Pulls in data from the isio and builds the JplTdl data struct.
{
  char *datap;
  unsigned int hk1, hk3;
  int hk2, hk4;
  float vmr1, vmr2, temp, press, hk5;

// Check for data from the isio port.
  if (!(int)(datap = getData()) || idx > 2)
    return;
 
// Pull in the data.
  memcpy(buf, datap, strlen(datap));
 
// Check the data alignment.
  if ((buf[JPL_SYNC0_IDX]==JPL_SYNC0)) {

// Parse the JPL TDL 3 data string and build the recorded data block.
  sscanf (&buf[1], "%f %f %f %f %u %d %u %d %f", &vmr1, &vmr2, &press, &temp,
            &hk1, &hk2, &hk3, &hk4, &hk5);
//    sscanf (&buf[1], "%f %f %f %u %d %u %d %f", &vmr1, &press, &temp,
//            &hk1, &hk2, &hk3, &hk4, &hk5);

    jpltdl_blk[ptog].jptdl[idx].vmrs = vmr1;
    jpltdl_blk[ptog].jptdl[idx].vmrw = vmr2;
//  jpltdl_blk[ptog].jptdl[idx].vmrw = vmr1*vmr1*0.17046 + vmr1*1.0388-0.02372;
//  jpltdl_blk[ptog].jptdl[idx].vmrs = vmr1;
    jpltdl_blk[ptog].jptdl[idx].press = press;
    jpltdl_blk[ptog].jptdl[idx].temp = temp;
    idx++;
  }
// logMsg("vmrs = %d, vmrw = %d, press = %d, temp = %d\n",(int)vmr1,(int)vmr2,
//         (int)press,(int)temp,0,0);
  igetstr (term_str);                        // issue next read

}

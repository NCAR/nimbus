/* DigitalIn.cc

   Digital input sampling class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <DigitalIn.h>


/******************************************************************************
** Public Functions
******************************************************************************/
 
DigitalIn::DigitalIn (DigitalTable &dt, Vmio12 *vmio12[], Bc635Vme& tp) : dig_tbl(dt), tfp(tp)
 
// Class constructor.
{
  int j;
  int stat;

  vmio_12 = vmio12[0];

// Null class pointers.
  for (j = 0; j < DIG_MAX_CHAN; j++) 
    dig_in[j] = (void*)0;

// Create the Vmio14In or APN232 digital input port classes.
  for (stat = dig_tbl.firstEntry(); stat; stat = dig_tbl.nextEntry()) {
       
// Get the port number.
    j = dig_tbl.portNumber();
    switch (dig_tbl.portType()) {
      case DIG_PORT_RADAR_ALT:
        dig_in[j] = new APN232 (vmio12[dig_tbl.boardNumber()], 
                                      dig_tbl.address());
        break;

// If this is a 16 bit port, create an additional 8 bit port object for the 
// next port up in address.
      case DIG_PORT_16:
        dig_in[j+1] = new Vmio14In (vmio12[dig_tbl.boardNumber()], 
                                            dig_tbl.address() + 1);
//        printf("D16 address = %d\n",dig_tbl.address());
      case DIG_PORT_8:
//        printf("D8 address = %d\n",dig_tbl.address());
      case DIG_PORT_2:
        dig_in[j] = new Vmio14In (vmio12[dig_tbl.boardNumber()], 
                                        dig_tbl.address());
//        printf("D2 address = %d\n",dig_tbl.address());
        break;

      default:
        fprintf (stderr, "DigitalIn: Unknown port type, %d.\n",
                 dig_tbl.portType());
        exit (ERROR);
    }
  }

// DON THORNTON HARDWIRE!!!!!!!
/*
  dig_in[12] = new Vmio14In (vmio12[0], 12);
  dig_in[14] = new Vmio14In (vmio12[0], 14);
*/
// Create the data buffer classes.
  for (j = 0; j < TOG; j++) {

    if (dig_tbl.len55()) {
      buf_55[j] = new SerialBuf (dig_tbl.len55() * RATE_55);
      memset((char *)buf_55[j]->getBuf(), 0xff, dig_tbl.len55() * RATE_55);
    }
    else
      buf_55[j] = (SerialBuf*)NULL;

    if (dig_tbl.len50()) {
      buf_50[j] = new SerialBuf (dig_tbl.len50() * RATE_50);
    }
    else
      buf_50[j] = (SerialBuf*)NULL;

    if (dig_tbl.len25()) {
      buf_25[j] = new SerialBuf (dig_tbl.len25() * RATE_25);
    }
    else
      buf_25[j] = (SerialBuf*)NULL;

    if (dig_tbl.len5()) {
      buf_5[j] = new SerialBuf (dig_tbl.len5() * RATE_5);
    }
    else
      buf_5[j] = (SerialBuf*)NULL;

    if (dig_tbl.len1()) {
      buf_1[j] = new SerialBuf (dig_tbl.len1());
    }
    else
      buf_1[j] = (SerialBuf*)NULL;
  }
  ptog = 0;
  gtog = 0;
}
/*****************************************************************************/

void DigitalIn::enableInt (int cio_num, int vector)

// Enable Digital In interrupt (Bandy). 
{

  if (cio_num == 2) {
    vmio_12->enableCioInt (cio_num, vector);
  }
}

/*****************************************************************************/
void DigitalIn::readIsr ()

// Interrupt service routine
{
  int	j;
  ushort data;

  data = tfp.readmsec();
//  logMsg("DigIn:isr %d\n",data,0,0,0,0,0);
  dig_tbl.firstEntry();
  buf_55[ptog]->copyToBuf((char *)&data, sizeof(data));
  dig_tbl.nextEntry();

  for (j = 1; j < dig_tbl.cnt55(); ++j)
    {
    data = readPort();
//    logMsg("  %d\n",data,0,0,0,0,0);
    buf_55[ptog]->copyToBuf((char *)&data, sizeof(data));
    dig_tbl.nextEntry();
    }

  vmio_12->clearCioInt (2);

}

/*****************************************************************************/
void DigitalIn::getData (int hz_cnt)

// Checks for and gets data from the data classes.
// This routine is called at the sample rate.
{
  int j, status;
  unsigned short data;

// If this is the start of a new second, release the last second's buffers
// and toggle the buffers.
  if (!hz_cnt) {
    if (dig_tbl.cnt55()) {		// release 50 hz
      memset((char *)&buf_55[gtog]->getBuf()[269], 0xffff, 10*2*3);
      buf_55[gtog]->releaseBuf();
    }
    if (dig_tbl.cnt50())		// release 50 hz
      buf_50[gtog]->releaseBuf();
    if (dig_tbl.cnt25())          	// release 25 hz
      buf_25[gtog]->releaseBuf();
    if (dig_tbl.cnt5())            	// release 5 hz
      buf_5[gtog]->releaseBuf();
    if (dig_tbl.cnt1())            	// release 1 hz
      buf_1[gtog]->releaseBuf();

    gtog = ptog;                                // toggle buffers
    ptog = ++ptog % TOG;
  }


// Skip past 55Hz crap.
  for (j = 0, dig_tbl.firstEntry(); j < dig_tbl.cnt55(); j++, dig_tbl.nextEntry());

// Check if time for 50 hertz sampling. 
  for (j = 0; j < dig_tbl.cnt50(); j++, dig_tbl.nextEntry()) {
    data = readPort();
    buf_50[ptog]->copyToBuf ((char*)&data, sizeof (data));
  }

// Check if time for 25 hertz sampling. 
  if (!(hz_cnt % RATE_2)) {
    for (j = 0; j < dig_tbl.cnt25(); j++, dig_tbl.nextEntry()) {
      data = readPort();
      buf_25[ptog]->copyToBuf ((char*)&data, sizeof (data));
    }
  }

// Check if time for 5 hertz sampling. 
  if (!(hz_cnt % RATE_10)) {
    for (j = 0; j < dig_tbl.cnt5(); j++, dig_tbl.nextEntry()) {
      data = readPort();
//      printf("Dig_8 data = %d\n",data);
      buf_5[ptog]->copyToBuf ((char*)&data, sizeof (data));
    }
  }  

// Check if time for 1 hertz sampling. 
  if (!(hz_cnt % RATE_50)) {
    status = vmio_12->cioPortStatus (2); 
    for (j = 0; j < dig_tbl.cnt1(); j++, dig_tbl.nextEntry()) {
      data = readPort();
      buf_1[ptog]->copyToBuf ((char*)&data, sizeof (data));
    }
  }

}

/******************************************************************************
** Private Functions
******************************************************************************/

short DigitalIn::readPort ()

// Reads data from the currently selected port(s).
{
  short data;

  switch (dig_tbl.portType()) {
    case DIG_PORT_RADAR_ALT:
      return ((APN232*)dig_in[dig_tbl.portNumber()])->readPort();

    case DIG_PORT_16:
      data = (short)((Vmio14In*)dig_in[dig_tbl.portNumber()])->readPort();
      data += (short)((Vmio14In*)dig_in[dig_tbl.portNumber() + 1])->readPort() 
              << 8;
      return data;

    case DIG_PORT_8:
      return ((Vmio14In*)dig_in[dig_tbl.portNumber()])->readPort();

// If this is a two bit port, shift c2,c3 to the lower bit positions.
    case DIG_PORT_2:
      data = ((Vmio14In*)dig_in[dig_tbl.portNumber()])->readPort();
      return (data >> 2) & 0x03;
 
    default:;
      fprintf (stderr, "DigitalIn: Unknown port type, %d.\n",
               dig_tbl.portType());
      exit (ERROR);
  }
}
/*****************************************************************************/


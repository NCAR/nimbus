/* IsioPort.cc
   Force ISIO-2 serial port class.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <IsioPort.h>

/******************************************************************************
** Public Functions
******************************************************************************/
IsioPort::IsioPort (const char *base, const int port, const int baud, 
                    const int bits, const int parity, const float stop, 
                    const int flow, void (*statMsg)(char* msg)) :
                    isio ((Isio*)base),
                    port_num (port),
                    rx_chan ((port - 1) * 2),
                    tx_chan ((port - 1) * 2 + 1),
                    baud_rate (baud),
                    data_bits (bits),
                    parity_type (parity),
                    stop_bits (stop),
                    flow_ctl (flow),
                    statusMsg (statMsg)
                   
// Constructor.
{
  if (initPort() == ERROR) {
    sprintf (msg, "IsioPort: Failure initializing port %2d.\n", port_num);
    statusMsg (msg);
    exit (ERROR);
  }
}

/*****************************************************************************/
char* IsioPort::getData()

// Returns a pointer to the rx channel dual port ram, or NULL if a command
// is still pending or an error has occurred.
{
  if (!(isio->cmdram[rx_chan].cmd & 0x00FF)) 
    return (char*)isio->cmdram[rx_chan].ptr1;
  else
    return (char*)0;
}

/*****************************************************************************/
int IsioPort::checkPort ()
 
// Checks the status of the port.
// Returns TRUE if a command is pending, FALSE if no command is pending.
{
  int stat = 0;
 
// Check for errors.
  if ((((stat = isio->cmdram[tx_chan].cmd) < 0) && (stat & 0x00FF)) ||
      (((stat = isio->cmdram[rx_chan].cmd) < 0) && (stat & 0x00FF))) {
    sprintf(msg, "IsioPort: Error 0x%4X on port %2d.\n", stat, port_num);
    statusMsg(msg);
    if (initPort() == ERROR) {
      sprintf(msg, "IsioPort: Failure reinitializing port %2d.\n", port_num);
      statusMsg(msg);
      exit(ERROR);
    }
    return(FALSE);
  }

// Check for a dropped interrupt.  If stat is < 0, no command is pending.  The
// command status is checked a second time in case the first read was done just
// as a read was completing prior to an interrupt.
/*
  else if ((stat < 0) && (isio->cmdram[rx_chan].cmd < 0)) {
    printf("IsioPort: Error 0x%4X on port %2d.\n", stat, port_num);
    return FALSE;
  }
*/

  return TRUE;
}

/*****************************************************************************/
int IsioPort::getcnt (char *const str, const int len)
 
/* Reads a counted string of characters from the specified channel */
{
  int stat; 

  isio->cmdram[rx_chan].parm1 = len;          // set the length 
  isio->cmdram[rx_chan].cmd = GETCNT;         // issue command 
/* 
  while (isio->cmdram[rx_chan].cmd > 0);         // wait for completion
 
  if (!(stat = isio->cmdram[rx_chan].cmd  & 0x00ff)) {   // if no error 
    memcpy (str, (char*)isio->ibufs[rx_chan], len);	// get the data
  }
  return (isio->cmdram[rx_chan].cmd & 0x00ff);   
*/
}
/*****************************************************************************/
void IsioPort::igetcnt (const int len)

// Issues a get count command with an interrupt requested on completion. 
{
  isio->cmdram[rx_chan].parm1 = (short)len;   	// set the length 
  isio->cmdram[rx_chan].cmd = GETCNT | INTFLG;  // issue command 
}

/*****************************************************************************/
void IsioPort::getstr (const char *const term_str)

// Issues a get string command with an interrupt requested on completion. 
{
  isio->cmdram[rx_chan].parm1 = *(short*)term_str;   	// set the delimiter 
  isio->cmdram[rx_chan].cmd = GETSTR;		   	// issue command 
  isio->cmdram[rx_chan].parm1 = *(short*)term_str;   	// set the delimiter 
}

/*****************************************************************************/
void IsioPort::igetstr (const char *const term_str)

// Issues a get string command with an interrupt requested on completion. 
{
  isio->cmdram[rx_chan].parm1 = *(short*)term_str;   	// set the delimiter 
  isio->cmdram[rx_chan].cmd = GETSTR | INTFLG;   	// issue command 
  isio->cmdram[rx_chan].parm1 = *(short*)term_str;   	// set the delimiter 
}

/*****************************************************************************/
int IsioPort::putcnt (const char *const str, const int len)
 
/* Writes a counted string of characters to the specified channel */
{
  int stat; 
 
  if (((stat = isio->cmdram[tx_chan].cmd) < 0) && !(stat & 0x00FF)) {
    isio->cmdram[tx_chan].parm1 = len;               // set the length 
    memcpy ((void *)isio->ibufs[tx_chan], str, len); // move data to isio 
    isio->cmdram[tx_chan].ptr1 = (long)isio->ibufs[tx_chan];
    isio->cmdram[tx_chan].cmd = PUTCNT | OUTFLAG;
  }
  while (isio->cmdram[tx_chan].cmd > 0);         // wait for completion
 
  return (isio->cmdram[tx_chan].cmd & 0x00ff);   
}

/*****************************************************************************/
int IsioPort::pputcnt (const char *const str, const int len)

// Writes a counted string of characters to the specified channel. Does
// not wait for completion of the command. 

{
  int stat;

  if (((stat = isio->cmdram[tx_chan].cmd) < 0) && !(stat & 0x00FF)) {
    isio->cmdram[tx_chan].parm1 = len;               // set the length 
    memcpy ((void *)isio->ibufs[tx_chan], str, len); // move data to isio 
    isio->cmdram[tx_chan].ptr1 = (long)isio->ibufs[tx_chan];
    isio->cmdram[tx_chan].cmd = PUTCNT | OUTFLAG;
  }
  return (stat & 0x00FF);
}

/*****************************************************************************/
int IsioPort::clrbuf()

// Clears the interrupt driver input buffer of the receive channel. 
{
  isio->cmdram[rx_chan].cmd = CLRBUF;  	// issue the command */
  taskDelay(sysClkRateGet());
//  while (isio->cmdram[rx_chan].cmd > 0);
  return(isio->cmdram[rx_chan].cmd & 0x00FF);
}

/*****************************************************************************/

int IsioPort::abort (const int chan)

// Aborts the current operations on the channel.

{
  char tmp;

  isio->abort = chan + 1;               // set channel to abort */
                                        // isio firmware  starts with chan 1
  tmp = isio->iabort[0].abint;          // generate isio local interrupt
  while (isio->abort);                  // wait for the abort to clear
  return isio->cmdram[chan].cmd & 0x00FF;
}

/*****************************************************************************/
int IsioPort::recoff()

// Disables the DUSCC receiver for the specified channel. 
{
  isio->cmdram[rx_chan].cmd = RECOFF;   	// issue the command 
  while (isio->cmdram[rx_chan].cmd > 0);
  return isio->cmdram[rx_chan].cmd & 0x00FF;
}

/*****************************************************************************/
int IsioPort::recon()

// Enables the DUSCC receiver for the specified channel. 
{
  isio->cmdram[rx_chan].cmd = RECON;   	// issue the command 
  while (isio->cmdram[rx_chan].cmd > 0);
  return isio->cmdram[rx_chan].cmd & 0x00FF;
}
/*****************************************************************************/

void IsioPort::enableInterrupt (const int vect, const int level)

// Enables port interrupts.
{
  clrbuf();

  switch (port_num) {
    case 1:
    case 2:
      isio->bim[0].bimvr1 = vect;       		// load vector
      isio->bim[0].bimcr1 = BIM_INT_ENABLE | level; 	// enable level
      break;
    case 3:
    case 4:
      isio->bim[0].bimvr2 = vect;       		
      isio->bim[0].bimcr2 = BIM_INT_ENABLE | level; 
      break;
    case 5:
    case 6:
      isio->bim[0].bimvr3 = vect;       	
      isio->bim[0].bimcr3 = BIM_INT_ENABLE | level;
      break;
    case 7:
    case 8:
      isio->bim[0].bimvr4 = vect;       	
      isio->bim[0].bimcr4 = BIM_INT_ENABLE | level; 
      break;
    default:;
  }

}
/******************************************************************************
** Private Functions
******************************************************************************/

int IsioPort::initPort ()

// Initialize the port for asynchronous communications according to the 
// initial parameters. Returns TRUE if successful FALSE otherwise.
{
  ChiniRegs regs;
// Set the parity, and asynchronous mode.
  switch (parity_type) {
    case SER_NONE:
      regs.cmr1 = (char)(CMR1_NO_PARITY | CMR1_ASYNC_MODE);
      break;
    case SER_EVEN:
      regs.cmr1 = (char)(CMR1_EVEN_PARITY | CMR1_ASYNC_MODE);
      break;
    case SER_ODD:
      regs.cmr1 = (char)(CMR1_ODD_PARITY | CMR1_ASYNC_MODE);
      break;
    default:
      return ERROR;
  }

// Set polled or interrupt for the data transfer interface.
  regs.cmr2 = (char)CMR2_POLL_INT;

// No syn words in async mode.
  regs.syn1 = (char)0;
  regs.syn2 = (char)0;

// Set the number of data bits.
  switch (data_bits) {
    case 7:
      regs.tpr = (char)TPR_RPR_BITS_7;
      regs.rpr = (char)TPR_RPR_BITS_7 | RPR_STRIP_PARITY;
      break;
    case 8:
      regs.tpr = (char)TPR_RPR_BITS_8;
      regs.rpr = (char)TPR_RPR_BITS_8 | RPR_STRIP_PARITY;
      break;
    default:
      return ERROR;
  }

// Select flow control if requested.
  if (flow_ctl) {
    regs.tpr |= (char)TPR_CTS_ENAB; 
    regs.rpr |= (char)RPR_RTS_CTL;
  }

// Set the number of stop bits;
  if (stop_bits == 1.0) 
    regs.tpr |= TPR_STOP_1_000;
  else if (stop_bits == 2.0)
    regs.tpr |= TPR_STOP_2_000;
  else
    return ERROR;

// Set the baud rate, and use the bit rate generator for a clock source.
  switch (baud_rate) {
    case 1200:
      regs.ttr = (char)(TTR_BRG_CLK | TTR_RTR_BAUD_1200);
      regs.rtr = (char)(RTR_BRG_CLK | TTR_RTR_BAUD_1200);
      break;
    case 2400:
      regs.ttr = (char)(TTR_BRG_CLK | TTR_RTR_BAUD_2400);
      regs.rtr = (char)(RTR_BRG_CLK | TTR_RTR_BAUD_2400);
      break;
    case 4800:
      regs.ttr = (char)(TTR_BRG_CLK | TTR_RTR_BAUD_4800);
      regs.rtr = (char)(RTR_BRG_CLK | TTR_RTR_BAUD_4800);
      break;
    case 9600:
      regs.ttr = (char)(TTR_BRG_CLK | TTR_RTR_BAUD_9600);
      regs.rtr = (char)(RTR_BRG_CLK | TTR_RTR_BAUD_9600);
      break;
    case 19200:
      regs.ttr = (char)(TTR_BRG_CLK | TTR_RTR_BAUD_19200);
      regs.rtr = (char)(RTR_BRG_CLK | TTR_RTR_BAUD_19200);
      break;
    case 38400:
      regs.ttr = (char)(TTR_BRG_CLK | TTR_RTR_BAUD_38400);
      regs.rtr = (char)(RTR_BRG_CLK | TTR_RTR_BAUD_38400);
      break;
    default:
      return ERROR;
  }

// Set the output pins for true logic.
  regs.omr = (char)OMR_TRUE_OUTPUTS;

// Disable the counter preset.
  regs.ctcr = (char)(CTCR_NO_PRESET | CTCR_PRESCALE_32 | CTCR_RTXC_CLK);
  regs.ctpl = (char)0;
  regs.ctph = (char)0;

// Set the control regs to first reset and then enable the receiver.
  regs.ccr1 = CCR_RX_RESET; 
  regs.ccr2 = CCR_RX_ENABLE; 

// Set the pin configuration to use the synout/rts pin as an rts output, 
// the rtxc pin as an input, and the trxc pin as an BRG 16x output.
  regs.pcr = PCR_SYNRTS_RTS | PCR_RTXC_INPUT | PCR_TRXC_TXCLK_1X;

/**
  printf ("cmr1 = 0x%2X\n", regs.cmr1);
  printf ("cmr2 = 0x%2X\n", regs.cmr2);
  printf ("syn1 = 0x%2X\n", regs.syn1);
  printf ("syn2 = 0x%2X\n", regs.syn2);
  printf ("tpr = 0x%2X\n", regs.tpr);
  printf ("ttr = 0x%2X\n", regs.ttr);
  printf ("rpr = 0x%2X\n", regs.rpr);
  printf ("rtr = 0x%2X\n", regs.rtr);
  printf ("omr = 0x%2X\n", regs.omr);
  printf ("ctcr = 0x%2X\n", regs.ctcr);
  printf ("ctpl = 0x%2X\n", regs.ctpl);
  printf ("ctph = 0x%2X\n", regs.ctph);
  printf ("ccr1 = 0x%2X\n", regs.ccr1);
  printf ("ccr2 = 0x%2X\n", regs.ccr2);
  printf ("pcr = 0x%2X\n", regs.pcr);
**/
  
  if (setoffs (rx_chan) & 0x00FF)     		// set board address 
    return ERROR;
  if (setoffs (tx_chan) & 0x00FF)
    return ERROR;
  if (recoff () & 0x00FF)      			// disable input receiver 
    return ERROR;
  if (clrbuf () & 0x00FF)      			// clear input buffer 
    return ERROR;
  if (chini2 (&regs) & 0x00FF) 			// init & enable DUSCC 
   return ERROR;

  return OK;
}
/*****************************************************************************/
void IsioPort::baudRate (const int baud_rate)
{
  ChiniRegs regs;
// Set the baud rate, and use the bit rate generator for a clock source.
  switch (baud_rate) {
    case 1200:
      regs.ttr = (char)(TTR_BRG_CLK | TTR_RTR_BAUD_1200);
      regs.rtr = (char)(RTR_BRG_CLK | TTR_RTR_BAUD_1200);
      break;
    case 2400:
      regs.ttr = (char)(TTR_BRG_CLK | TTR_RTR_BAUD_2400);
      regs.rtr = (char)(RTR_BRG_CLK | TTR_RTR_BAUD_2400);
      break;
    case 4800:
      regs.ttr = (char)(TTR_BRG_CLK | TTR_RTR_BAUD_4800);
      regs.rtr = (char)(RTR_BRG_CLK | TTR_RTR_BAUD_4800);
      break;
    case 9600:
      regs.ttr = (char)(TTR_BRG_CLK | TTR_RTR_BAUD_9600);
      regs.rtr = (char)(RTR_BRG_CLK | TTR_RTR_BAUD_9600);
      break;
    case 19200:
      regs.ttr = (char)(TTR_BRG_CLK | TTR_RTR_BAUD_19200);
      regs.rtr = (char)(RTR_BRG_CLK | TTR_RTR_BAUD_19200);
      break;
    case 38400:
      regs.ttr = (char)(TTR_BRG_CLK | TTR_RTR_BAUD_38400);
      regs.rtr = (char)(RTR_BRG_CLK | TTR_RTR_BAUD_38400);
      break;
    default:;
  }
}
/*****************************************************************************/


int IsioPort::chini2 (const ChiniRegs *const regs)

// Programs the channel DUSCC register. 
{
// Copy regs to isio dpram.
  memcpy((void *)isio->ibufs[rx_chan], (char*)regs, sizeof (ChiniRegs));
  isio->cmdram[rx_chan].ptr1 = (long)isio->ibufs[rx_chan];

  isio->cmdram[rx_chan].cmd = CHINIT2; 	// issue the command 

  while (isio->cmdram[rx_chan].cmd > 0);     	// wait for completion 
  return isio->cmdram[rx_chan].cmd & 0x00FF;	// return error code 
}
/*****************************************************************************/

int IsioPort::setoffs (const int chan)

// Loads the ISIO-2 VME base address into the channel offset address. 
{
  short icmd = SETOFFS;

  if (chan & 0x01)
    icmd |= OUTFLAG;                    	// check for output channel 

  isio->cmdram[chan].ptr1 = (long)isio;      	// load board address 
  isio->cmdram[chan].cmd = icmd;

  while (isio->cmdram[chan].cmd > 0);         // wait for completion 
  return isio->cmdram[chan].cmd & 0x00FF;	// return error code 
}
/*****************************************************************************/

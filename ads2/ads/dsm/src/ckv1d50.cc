/* ckv1d.cc 
   This program checks operation of the NCAR VME1D interface.
   mvme 162 version

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/
#include <vxWorks.h>
#include <logLib.h>
#include <stdioLib.h>
#include <wdLib.h>

#include <header.h>
#include <dsmctl.h>
#include <dsmVmeDefs.h>

#include <Pms1Vme.h>

void v1d_wdisr ();
void disp_v1d (int chan, int rate, int bins, int space);

Pms1Vme pms1 ((char*)(A24D16_BASE + VME1D_BASE_0));

WDOG_ID wid;
int new_sec  = 0;
int ticks50 = sysClkRateGet() / RATE_50;	// ticks per 50 hz interval

ckv1d ()
{
  int intf;					// interface #
  int chan;					// channel #
  int rate;					// sample rate
  int bins;					// # bins
  int range;					// probe range setting
  int space;					// spacing enabled flag
  char tchar;					//temp
  int pcnt = 0;

// Get the operating parameters. 

  printf("\nNCAR VME/PMS-1D interface test program\n\n");

  printf ("Enter the operating parameters in the order shown:\n");
  printf ("Interface   Channel   Rate       Bins       Range      Spacing \n");
  printf ("0,1        0,1,2     1,5,10,25      16,32,64   0-3        y,n\n");
  printf ("__________________________________________________________\n");
  
  scanf ("%d %d %d %d %d %c", &intf, &chan, &rate, &bins, &range, &tchar);

// Check the user entry.
  if ((intf < 0) || (intf > 1)) {
    printf ("Sorry, invalid interface number entered.\n");
    exit(0);
  }
  if ((chan < 0) || (chan > MX_P1DV2)) {
    printf ("Sorry, invalid channel number entered.\n");
    exit(0);
  }
  if ((rate != RATE_1) && (rate != RATE_5) && (rate != RATE_10) &&
      (rate != RATE_25)) {
    printf ("Sorry, invalid rate entered.\n");
    exit(0);
  }
  if ((bins != BINS_16) && (bins != BINS_32) && (bins != BINS_64)) {
    printf ("Sorry, invalid bin number entered.\n");
    exit(0);
  }
  if ((range < RANGE_0) || (range > RANGE_3)) {
    printf ("Sorry, invalid range entered.\n");
    exit(0);
  }
  if ((tchar == 'y') || (tchar == 'Y'))
    space = 1;
  else
    space = 0;

// Initialize the 1d interface class.
  if (intf == 0) 
    pms1.Pms1Vme ((char*)(A24D16_BASE + VME1D_BASE_0));
  else
    pms1.Pms1Vme ((char*)(A24D16_BASE + VME1D_BASE_1));

// Initialize the channel.
  if (!pms1.initChan (chan, rate, bins, range, space)) {
    printf ("Failure initializing the channel\n");
    exit (0);
  }
  printf ("done with init chan\n");

// Start up a watchdog timer at a 50 hertz interval. 
  wid = wdCreate();              
  wdStart ((WDOG_ID)wid, ticks50, (FUNCPTR)v1d_wdisr, FALSE);

  while (1) {
    if (new_sec) {
      new_sec = 0;	
      if (!pcnt)
        disp_v1d (chan, rate, bins, space);
      pcnt = (pcnt + 1) % RATE_5;
    }
  }
}
/*****************************************************************************/

void v1d_wdisr()

{
  static int hz50_cnt = 0;
  static int first = 1;
  
// Restart the timer.
  wdStart ((WDOG_ID)wid, ticks50, (FUNCPTR)v1d_wdisr, FALSE);

// Every 50 interrupts set the new second flag.
  if (!hz50_cnt)
    new_sec = 1;

// Call the vme 1d sampling isr.
  pms1.pms1VmeIsr(hz50_cnt);

  hz50_cnt = (hz50_cnt + 1) % RATE_50;
}
/*****************************************************************************/

void disp_v1d (int chan, int rate, int bins, int space)

// Displays channel data in a formatted form on the terminal.
{
  struct P64v2_blk *p64;
  struct P64v2_blk *paux;
  struct P1dSpc_blk *psp;
  int j;
  int k;
  int cnt;

// Get a pointer to the last second's data block.
  p64 = (struct P64v2_blk *)pms1.binBlk (chan);
  paux = (struct P64v2_blk *)pms1.binBlk (chan);
  psp = (struct P1dSpc_blk *)pms1.spcBlk (chan);

  printf ("Sizing data\n");
/**
  for (k = 0, cnt = 0; k < rate; k++) {
    for (j = 0; j < bins; j++) 
      cnt += p64->bins[j];
    for (j = 0; j < bins; j += 16) {
      printf ("%4hX %4hX %4hX %4hX %4hX %4hX %4hX %4hX %4hX %4hX %4hX %4hX %4hX %4hX %4hX %4hX\n",
        p64->bins[j], p64->bins[j+1], p64->bins[j+2], p64->bins[j+3],
        p64->bins[j+4], p64->bins[j+5], p64->bins[j+6], p64->bins[j+7],
        p64->bins[j+8], p64->bins[j+9], p64->bins[j+10], p64->bins[j+11],
        p64->bins[j+12], p64->bins[j+13], p64->bins[j+14], p64->bins[j+15]);
    }
    switch (bins) {
      case 16:
        (char*)p64 = (char*)p64 + sizeof (struct P16v2_blk);
        break;
      case 32:
        (char*)p64 = (char*)p64 + sizeof (struct P32v2_blk);
        break;
      case 64:
        p64++;
        break;
      default:
        printf ("Invalid number of bins.\n");
        exit(0);
    }
  }
**/
  printf ("Total sizing counts = %d\n", cnt);
  putchar ('\n');

  printf ("First sample spacing data\n");
  for (j = 0, cnt = 0; j < BINS_64; j += 16) {
    for (k = j; k < j + 16; k++)
      cnt += psp->bins[k];
    printf ("%4hX %4hX %4hX %4hX %4hX %4hX %4hX %4hX %4hX %4hX %4hX %4hX %4hX %4hX %4hX %4hX\n",
      psp->bins[j], psp->bins[j+1], psp->bins[j+2], psp->bins[j+3],
      psp->bins[j+4], psp->bins[j+5], psp->bins[j+6], psp->bins[j+7],
      psp->bins[j+8], psp->bins[j+9], psp->bins[j+10], psp->bins[j+11],
      psp->bins[j+12], psp->bins[j+13], psp->bins[j+14], psp->bins[j+15]);
  }
  printf ("Total first sample spacing counts = %d\n", cnt);
  putchar ('\n');

  printf ("First sample aux data\n");
  printf ("%4hX %4hX %4hX %4hX %4hX %4hX %4hX %4hX %4hX %4hX %4hX %4hX\n",
    paux->aux[0], paux->aux[1], paux->aux[2], paux->aux[3],
    paux->aux[4], paux->aux[5], paux->aux[6], paux->aux[7],
    paux->aux[8], paux->aux[9], paux->aux[10], paux->aux[11]);
  putchar ('\n');
}  
/*****************************************************************************/

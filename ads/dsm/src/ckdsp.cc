/* ckdsp.cc
   Check the dsp.  Requires to tfp to operating correctly.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include "mv162.h"         // mvme162 defines
#include <vxWorks.h>
#include <intLib.h>
#include <logLib.h>
#include <stdioLib.h>
#include <taskLib.h>

// Program include files.
#include <dsmVmeDefs.h>

// Class include files.
#include <AnalogTable.h>
#include <Bc635Vme.h>

#define DSP_TABLE_OFFSET        0x0200          // offset to the sample table
#define DSP_BUFFER_OFFSET       0x26E0          // offset to the output buffer
#define DSP_SEMAPHORE_OFFSET    0x8000          // offset to the dp semaphore

// Dual-port ram semaphore macros.
#define get_sem() for (*dsp_sem = 0; *dsp_sem & 0x01; *dsp_sem = 0)
#define rel_sem() *dsp_sem = 1

// Program functions.
void initInterrupts ();				// initialize interrupts
void buildDspTable();				// build a test table
void tfp_isr ();				// time-freq processor isr
void assignTable();                          	// copy smp table to dsp memory
int dataReady();                             	// check the data ready flag
void clearDataReady();                       	// clear the data ready flag
int checkTable();
void display_first_samples(short buf[]);
static void tfpInit();					// init time-freq class

// Class declarations.
static Bc635Vme *tfp;                           // time-freq processor

// Global variables.
DspTable table;					// local copy of setup table
DspTable *dsp_table = (DspTable*)(A24D16_BASE + M56002_1_BASE+DSP_TABLE_OFFSET);
short *dsp_sem = (short*)(A24D16_BASE + M56002_1_BASE + DSP_SEMAPHORE_OFFSET);
unsigned short *dsp_buf;

// Define the output file name.
#define OUTFILE "ckdsp.out"

// Set the number of channels at each sample rate.
int cnt5000 = 0;
int cnt1000 = 0;
int cnt250 = 0;
int cnt25 = 7;
int cnt5 = 0;

// Uncomment to select no cal or cal in the flag field.
  int flag = ANA_SETUP_FLAG;

// Uncomment to select cal or no cal bit in the setup field.
// int cal = ANA_CAL_MASK;
int cal = 0;

// Uncomment desired cal voltage.
  int cal_volt = ANA_V10_MASK;
//int cal_volt = ANA_V5_MASK;
//int cal_volt = ANA_V2D5_MASK;
//int cal_volt = ANA_V1_MASK;
//int cal_volt = ANA_V0_MASK;
//int cal_volt = ANA_V10N_MASK;

// Uncomment desired gain.
int gain = ANA_GAIN_1_MASK;
//int gain = ANA_GAIN_2_MASK;
//int gain = ANA_GAIN_4_MASK;
//int gain = ANA_GAIN_20_MASK;

// Uncomment to select offset or no offset.
//int offset = ANA_OFFSET_MASK;
int offset = 0;

// Set the file recording channel number.
int rec_channel = 1;

int start250;				// 250 hz starting index
int start25;				// 25 hz starting index
int start5;				// 5 hz starting index
int size;				// total buffer size

/* -------------------------------------------------------------------- */
int ckdsp ()
{
  int j, k;
  short buf[8192];
  short *adr;
  FILE *fp;				// output file pointer
  int sec_cnt = 0;
  int rec_rate;				// recording channel rate
  int rec_start;			// recording channel start
  int rec_offset;			// recording channel offset

char cwd[1024];

// Initialize the time-freq processor class.
  printf ("Initializing the time-frequency processor.\n");
  tfpInit();

// Initialize interrupts.
  initInterrupts ();

// Open the output file.

/*  if (!(int)(fp = fopen (OUTFILE, "w"))) {
    fprintf (stderr, "Failure opening %s output file.\n", OUTFILE); 
    exit (ERROR);
  }
  else
    fprintf (stderr, "%s output file opened.\n", OUTFILE); 
*/
// Build the setup table and pass it to the Dsp.
  buildDspTable();

// Compute the starting indices for the 250, 25 and 5 hz blocks.
  start250 = table.cnthigh * RATE_1000 / RATE_5;
  start25 = start250 + table.cnt250 * RATE_250 / RATE_5;
  start5 = start25 + table.cnt25 * RATE_25 / RATE_5;
  size = (start5 + table.cnt5) * sizeof(short);
  
  printf ("start250 = %d, start25 = %d, start5 = %d, size = %d\n",
          start250, start25, start5, size);

// Compute the recording channel parameters.
  for (j = 0; j < table.tot_cnt; j++) {
    if (table.addrs[j] == rec_channel) {
      if (cnt1000 > j) {
        rec_rate = RATE_1000;
        rec_start = j;
        rec_offset = cnt1000;
      }
      else if ((cnt1000 + cnt250) > j) {
        rec_rate = RATE_250;
        rec_start = start250 + (j - cnt1000);
        rec_offset = cnt250;
      }
      else if ((cnt1000 + cnt250 + cnt25) > j) {
        rec_rate = RATE_25;
        rec_start = start25 + (j - cnt1000 - cnt250);
        rec_offset = cnt25;
      }
      else {
        rec_rate = RATE_5;
        rec_start = start5 + (j - cnt1000 - cnt250 - cnt25);
        rec_offset = cnt5;
      }
      printf (
        "rec_channel = %d, rec_start = %d, rec_rate = %d, rec_offset = %d.\n",
        rec_channel, rec_start, rec_rate, rec_offset);
      break;
    }
  }

  k = 0;
  while (TRUE) {

    if (dataReady()) {
      k++;

// Get the buffer address, converting from a word to a byte address.
      dsp_buf = (unsigned short*)(A24D16_BASE + M56002_1_BASE + 
                                  (dsp_table->buf_adr << 1));
      if (((int)dsp_buf != 0xf0105400) && ((int)dsp_buf != 0xf0102ae0)) {
        printf ("Invalid buffer address, 0x%X\n", (int)dsp_buf);
        clearDataReady();
        continue;
      }
      memcpy ((char*)buf, (char*)dsp_buf, size);
      clearDataReady();

// Write the samples of the first 25 hz channel to the output file.

/*      for (j = 0; j < (rec_rate / 5); j++) 
        fprintf (fp, "%d\n",(short)buf[rec_start + j * rec_offset]);
*/
/**
        fprintf (fp, "%d\n",(short)buf[start25 + j * cnt25]);
      for (j = 0; j < 5; j++) 
        fprintf (fp, "%d\n",(short)buf[rec_start + j * cnt25]);
**/
      if (tfp->newSecond()) {
        tfp->clearNewSecond();
//        printf ("buffer count = %d\n", k);
        k = 0;
//      printf ("buffer address = 0x%X\n", dsp_table->buf_adr);

        if (!(sec_cnt++ % 2)) 
          display_first_samples (buf);

        checkTable();
      }
    }
  }
}
/*****************************************************************************/

void initInterrupts ()

// Initialize interrupts and handlers.
{
// Connect the time-freq processor isr.
  if (intConnect ((VOIDFUNCPTR*)TFP_ADR,(VOIDFUNCPTR)tfp_isr, 0)) {
    perror ("cktfp: intConnect tfp_isr");
    exit(ERROR);
  }

// Enable the interrupt handler on the mv162 board.
  *VMECHIP2_LBIER = 0x04;     

// Enable strobe interrupts from the tfp at 5 hertz.
  tfp->enableMinorStrobeInt (TFP_VCT, 3, 200);
}
/*****************************************************************************/

void buildDspTable ()

// Creates a test setup table.
{
  int j;

  table.flag = 0;
  table.cal_volt = cal_volt;
  table.tot_cnt = cnt1000 + cnt250 + cnt25 + cnt5;
  table.cnthigh = cnt1000;
  table.cnt250 = cnt250;
  table.cnt25 = cnt25;
  table.cnt5 = cnt5;

  for (j = 0; j <  table.tot_cnt; j++) {
    table.addrs[j] = j;
  }
//table.addrs[7] = 8;

  for (j = 0; j <  table.tot_cnt; j++) {
    if (table.addrs[j] % 2) 		// odd channels in the upper nibble
      table.setup[table.addrs[j]/2] |= (gain + offset + cal) << 
                                       ANA_ODD_SETUP_SHIFT;
    else				// even channels in the lower nibble
      table.setup[table.addrs[j]/2] |= gain + offset + cal;
  }
  table.data_ready = FALSE;

// Copy the table to the dsp dual-port memory.
  assignTable();
}
/*****************************************************************************/

void tfp_isr ()

// Interrupt service routine for the time-freq processor.
{
  tfp->strobeIsr ();
}
/*****************************************************************************/

void assignTable ()
 
// Assign the dsp sample table to the dsp memory.
{
  get_sem();
  *dsp_table = table;
  dsp_table->flag = flag;
  rel_sem();
  printf ("Setup table assigned to the DSP.\n");
}
/*****************************************************************************/
 
int dataReady ()
 
// Checks for available data in the dp ram.
{
  int data_rdy;
 
  get_sem();
  data_rdy = dsp_table->data_ready;
  rel_sem();
 
  return data_rdy & ANA_DATA_READY_FLAG;
}
/*****************************************************************************/
 
void clearDataReady ()
 
// Clears the data ready flag in the dp ram.
{
  get_sem();
  dsp_table->data_ready = FALSE;
  rel_sem();
}
/*****************************************************************************/
 
int checkTable ()
 
// Checks for a valid running flag.
 
{
  if (dsp_table->flag != (short)ANA_RUNNING_FLAG) {
    assignTable();
    printf ("New sample table assigned to the dsp.\n");
    return FALSE;
  }
  return TRUE;
}
/*****************************************************************************/

void display_first_samples (short buf[])

// Displays the first sample of each channel.
{
  int j;

  if (table.cnthigh) {
    printf ("1000 hz channels.\n");
    for (j = 0; j < table.cnthigh; j++)
      printf ("%04X ", (unsigned short)buf[j]);
  }
  putchar ('\n');
 
  if (table.cnt250) {
    printf ("250 hz channels.\n");
    for (j = 0; j < table.cnt250; j++)
      printf ("%04X ", (unsigned short)buf[j + start250]);
    putchar ('\n');
  }
 
  if (table.cnt25) {
    printf ("25 hz channels.\n");
    for (j = 0; j < table.cnt25; j++)
      printf ("%04X ", (unsigned short)buf[j + start25]);
    putchar ('\n');
  }
 
  if (table.cnt5) {
    printf ("5 hz channels.\n");
    for (j = 0; j < table.cnt5; j++)
      printf ("%04X ", (unsigned short)buf[j + start5]);
    putchar ('\n');
  }
}
/*****************************************************************************/
 
static void tfpInit ()
 
// Initialize the time-frequency processor.
{
  tfp = new Bc635Vme ((char*)(A24D16_BASE + TFP_BASE));
 
  tfp->setPath();                                // set default path packet
  tfp->selectModulatedInput();                   // select modulated irig input
  tfp->selectModulatedOutput();                  // select modulated irig output
 
// Sync to the 1PPS if operating as the time master.
  tfp->select1PPSMode();
 
// Set the periodic output at 10 Khz, and a 50 usec pulse width.
  tfp->setPeriodicOutput (10000, 50);
}

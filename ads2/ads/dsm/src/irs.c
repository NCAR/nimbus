// System include files.
#include <vxWorks.h>
#include <bootLib.h>
#include <intLib.h>
#include <logLib.h>
#include <msgQLib.h>
#include <semLib.h>
#include <stdioLib.h>
#include <sysLib.h>
#include <taskLib.h>

#include "/usr/local/ads/dsm/include/ipicDefs.h"

#define IP429_NUM_CHANS	6
#define IP429_IE_EIN_BASE       0x04
#define IP429_ADR	0x0160
#define IP429_VCT	0x58

typedef struct {
  char unused0;
  char svc_rqst;                        // service request
  char unused1;
  char misc_status;                     // miscellaneous status
  char unused2;
  char int_enable;                      // interrupt enable
  char unused3;
  char int_vect;                        // interrupt acknowledge
  char unused4;
  char tx4;                             // transmit channel 4 control
  char unused5;
  char tx5;                             // transmit channel 5 control
  char unused6;
  char go4;                             // gap time channel 4
  char unused7;
  char go5;                             // gap time channel 5
  char unused8;
  char sync;                            // sync output control
  char unused9;
  char reset;                           // write resets the IP429
} Ip429Ctl;

// Define the individual channel data registers.
typedef struct {
  short data1;                          // chan data word 1
  short data2;                          // chan data word 2
} Ip429Chan;

// Define the board data registers.
typedef struct {
  Ip429Chan chan[IP429_NUM_CHANS];      // rx (0-3) and tx (4-5) data regs
  short unused[4];
  short ctl_a;                          // control word A
  short ctl_b;                          // control word B
} Ip429Data;


#define TRUE	1
#define FALSE	0

volatile Ip429Ctl *ip_ctl;            // control registers
volatile Ip429Data *ip_data;          // data registers

int	idx50, idx25, idx10, idx5, idx2;
int	aligned = FALSE;
int	counter[256], missCnt;

void initIP429(char *base_adr, char vector);
void isr(int);

static SEM_ID isr_sem;

/* -------------------------------------------------------------------- */
cjw_irs()
{
  int	i;

  sysClkRateSet(10);

  idx50 = idx25 = idx10 = idx5 = idx2 = 0;

  initIP429((char*)IP_a_IO_BASE, IP429_VCT);

  intConnect((VOIDFUNCPTR*)IP429_ADR, (VOIDFUNCPTR)isr, 0);

  *IPIC_IP_a_INT0_CTL = IPIC_INT_CTL_IEN | IPIC_INT_CTL_LEVEL_5;
  ip_ctl->int_enable |= IP429_IE_EIN_BASE;

  if (!(isr_sem = semBCreate(SEM_Q_FIFO, SEM_EMPTY))) {
    fprintf (stderr, "Failure creating isr_sem.\n");
    exit (ERROR);
  }

  clearCounter();

  while (1)
    {
    semTake(isr_sem, WAIT_FOREVER);

    if (counter[0265] != 50)
      logMsg("50Hz: 0265 = %d\n", counter[0265],0,0,0,0,0);

    if (counter[0324] != 50)
      logMsg("50Hz: 0324 = %d\n", counter[0324],0,0,0,0,0);

    if (counter[0325] != 50)
      logMsg("50Hz: 0325 = %d\n", counter[0325],0,0,0,0,0);

    if (counter[0326] != 50)
      logMsg("50Hz: 0326 = %d\n", counter[0326],0,0,0,0,0);

    if (counter[0327] != 50)
      logMsg("50Hz: 0327 = %d\n", counter[0327],0,0,0,0,0);

    if (counter[0330] != 50)
      logMsg("50Hz: 0330 = %d\n", counter[0330],0,0,0,0,0);

    if (counter[0331] != 50)
      logMsg("50Hz: 0331 = %d\n", counter[0331],0,0,0,0,0);

    if (counter[0332] != 50)
      logMsg("50Hz: 0332 = %d\n", counter[0332],0,0,0,0,0);

    if (counter[0333] != 50)
      logMsg("50Hz: 0333 = %d\n", counter[0333],0,0,0,0,0);

    if (counter[0335] != 50)
      logMsg("50Hz: 0335 = %d\n", counter[0335],0,0,0,0,0);

    if (counter[0336] != 50)
      logMsg("50Hz: 0336 = %d\n", counter[0336],0,0,0,0,0);

    if (counter[0337] != 50)
      logMsg("50Hz: 0337 = %d\n", counter[0337],0,0,0,0,0);

    if (counter[0360] != 50)
      logMsg("50Hz: 0360 = %d\n", counter[0360],0,0,0,0,0);

    if (counter[0364] != 50)
      logMsg("50Hz: 0364 = %d\n", counter[0364],0,0,0,0,0);


    if (counter[0313] != 25)
      logMsg("25Hz: 0313 = %d\n", counter[0313],0,0,0,0,0);

    if (counter[0365] != 25)
      logMsg("25Hz: 0365 = %d\n", counter[0365],0,0,0,0,0);

    if (counter[0361] != 25)
      logMsg("25Hz: 0361 = %d\n", counter[0361],0,0,0,0,0);

    if (counter[0321] != 25)
      logMsg("25Hz: 0321 = %d\n", counter[0321],0,0,0,0,0);

    if (counter[0314] != 25)
      logMsg("25Hz: 0314 = %d\n", counter[0314],0,0,0,0,0);


    if (counter[0312] != 10)
      logMsg("10Hz: 0312 = %d\n", counter[0312],0,0,0,0,0);

    if (counter[0315] != 10)
      logMsg("10Hz: 0315 = %d\n", counter[0315],0,0,0,0,0);

    if (counter[0316] != 10)
      logMsg("10Hz: 0316 = %d\n", counter[0316],0,0,0,0,0);

    if (counter[0366] != 10)
      logMsg("10Hz: 0366 = %d\n", counter[0366],0,0,0,0,0);

    if (counter[0367] != 10)
      logMsg("10Hz: 0367 = %d\n", counter[0367],0,0,0,0,0);

    if (counter[0334] != 10)
      logMsg("10Hz: 0334 = %d\n", counter[0334],0,0,0,0,0);


    if (counter[0310] != 5)
      logMsg("5Hz: 0310 = %d\n", counter[0310],0,0,0,0,0);

    if (counter[0311] != 5)
      logMsg("5Hz: 0311 = %d\n", counter[0311],0,0,0,0,0);

    if (counter[0370] != 5)
      logMsg("5Hz: 0370 = %d\n", counter[0370],0,0,0,0,0);


    if (counter[0350] != 2)
      logMsg("2Hz: 0350 = %d\n", counter[0350],0,0,0,0,0);

    if (counter[0270] != 2)
      logMsg("2Hz: 0270 = %d\n", counter[0270],0,0,0,0,0);

    if (counter[0371] != 2)
      logMsg("2Hz: 0371 = %d\n", counter[0371],0,0,0,0,0);

    if (counter[0351] != 2)
      logMsg("2Hz: 0351 = %d\n", counter[0351],0,0,0,0,0);

    clearCounter();
    }

}

/* -------------------------------------------------------------------- */
void isr(int x)
{
  long  arinc_data;
  register int   label;
 
  arinc_data = (ip_data->chan[0].data2 << 16) + ip_data->chan[0].data1;
  label = arinc_data & 0x00FF;

  if (label == 0)
    logMsg("ARINC label == 0.\n",0,0,0,0,0,0);

  if (!aligned)
    {
    if (label == 0351)
      aligned = TRUE;

    return;
    }

  ++counter[label];

  if (label == 0351)
    {
    ++idx2;

    if (idx2 % 2 == 0)	// New second
      semGive(isr_sem);
    }


return;

  switch (label)
    {
    // The 50 Hz parameters
    case 0364:
      ++idx50;
      break;
 
    // 25 Hz parameters
    case 0314:
      ++idx25;
      break;
 
    // The 10 Hz parameters
    case 0334:
      ++idx10;
      break;
 
    // 5 Hz parameters
    case 0370:
      ++idx5;
      break;

    // 2 Hz Params
    case 0351:
      ++idx2;

      if (idx2 % 2 == 0)
        semGive(isr_sem);

      break;
    }


}

/* -------------------------------------------------------------------- */
#define IP429_DATA_REG_OFFSET   0x40            // byte offset to the data regs
 
// Define control register bits.
#define IP429_CTL_RX_LOW_SPEED  0x4000          // rx at low speed (12.5 khz)
#define IP429_CTL_TX_LOW_SPEED  0x2000          // tx at low speed (12.5 khz)
#define IP429_CTL_NO_LOOPBACK   0x0020          // disable loopback test
 
#define IP429_TX_ENTX           0x80            // tx enabled status bit
#define IP429_TX_GAP_SCALE_LOW  0x00            // set .1 ms gap time resolution
#define IP429_TX_GAP_SCALE_HIGH 0x10            // set 1 ms gap time resolution
#define IP429_TX_ENABLE         0x01            // master transmitter enable
 
#define IP429_IE_TEIN           0x02            // master tx interrupt enable
#define IP429_IE_REIN           0x01            // master rx interrupt enable
#define IP429_IE_EIN_BASE       0x04            // channel intr enable base bi


void initIP429(char *base_adr, char vector)
{
  short	ctl_a, ctl_b;

// Set the register pointers.
  ip_ctl = (Ip429Ctl*)base_adr;
  ip_data = (Ip429Data*)(base_adr + IP429_DATA_REG_OFFSET);
 
// Issue a reset.
  ip_ctl->reset = (char)0;
 
  taskDelay(sysClkRateGet() / 10);
 
// Init the control registers for no loopback and high speed.
  ctl_a = IP429_CTL_NO_LOOPBACK;
  ip_data->ctl_a = ctl_a;
 
  ctl_b = ctl_a;
  ip_data->ctl_b = ctl_b;
 
// Enable the transmitters and set the gap time resolution to 1 ms per count
  ip_ctl->tx4 = IP429_TX_GAP_SCALE_HIGH | IP429_TX_ENABLE;
  ip_ctl->tx5 = IP429_TX_GAP_SCALE_HIGH | IP429_TX_ENABLE;
  ip_ctl->sync = 0;                     // no sync output
 
// Load the interrupt vector. Set the master interrupt enable bits.
  ip_ctl->int_vect = vector;
  ip_ctl->int_enable = IP429_IE_TEIN | IP429_IE_REIN;
 

}

clearCounter()
{
  memset((char *)counter, 0, 4*256);

}

/* ckmasp.cc 

   This program checks operation of the masp interface.
   mvme 162 version

   Original Author: Mike Spowart
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/
#include <mv162.h>

#include <vxWorks.h>
#include <stdio.h>
#include <sysLib.h>
#include <taskLib.h>
#include <wdLib.h>
#include <intLib.h>
#include <logLib.h>
#include <bootLib.h>

#include <header.h>
#include <dsmctl.h>
#include <dsmVmeDefs.h>
#include <DsmConfig.h>
#include <Bc635Vme.h>
#include <Masp.h>

void statusMsg (char *msg);
void maspReadIsr();
void tfp_isr ();
void tfpInit();
void initInterrupts();

WDOG_ID wid;
int ticks50; 				// ticks per 50 hz interval
int ticks25; 				// ticks per 25 hz interval
int ticks1; 				// ticks per 1 hz interval
int hour;
int minute;
int second;

//Class declarations.

Bc635Vme tfp ((char*)(A24D16_BASE + TFP_BASE)); // time-freq processor
Masp masp ((unsigned char*)(A24D16_BASE + VMEMA_BASE), tfp, statusMsg);
 
ckmasp ()
{
  char *buf;
  int tas = 25;
  unsigned short param = 0x0;
  int hour = 8;
  int minute = 30;
  int second = 59;
  int new_second;

// Init the classes.

// Initialize the time-freq processor class.
  printf ("Initializing the time-frequency processor.\n");
  tfpInit();

  masp.Masp ((unsigned char*)(A24D16_BASE + VMEMA_BASE), tfp, statusMsg);

  ticks50 = sysClkRateGet() / RATE_50;
  ticks25 = sysClkRateGet() / RATE_25;
  ticks1 = sysClkRateGet();

// Get the operating parameters. 
  printf("In check program\n");

  masp.setTas (tas);
  masp.setParam (param);

// Start up a watchdog timer at a 25 hertz interval. 
//  wid = wdCreate();              
//  wdStart ((WDOG_ID)wid, ticks1, (FUNCPTR)p2d_wdisr, FALSE);

  printf ("Starting the main loop.\n");

  while (1) {
// Check for Masp data avilable.
    masp.collect();
    if (masp.md_bufFull())
       masp.md_releaseBuf();
    if (masp.ms_bufFull())
       masp.ms_releaseBuf();
    if(masp.cal_flag) masp.calibrate();
    if (new_second = tfp.newSecond()) {
      tfp.clearNewSecond();
    }
    taskDelay (sysClkRateGet());
  }
}
/*****************************************************************************/

void statusMsg (char *msg)
 
// Class wrapper for sending status messages.
{
  fprintf (stderr, msg);
}
/*****************************************************************************/
void tfpInit ()
 
// Initialize the time-frequency processor.
{
  tfp.Bc635Vme ((char*)(A24D16_BASE + TFP_BASE));
 
  tfp.setPath();                               // set default path packet
  tfp.selectModulatedInput();                  // select modulated irig input
  tfp.selectModulatedOutput();                 // select modulated irig output
 
// Sync to the 1PPS if operating as the time master.
 tfp.select1PPSMode();
 
 taskDelay(sysClkRateGet() * 2);
 
// Set the periodic output at 10 Khz, and a 50 usec pulse width.
 tfp.setPeriodicOutput (10000, 50);
 
}
/*****************************************************************************/
 
void tfp_isr ()
 
// Interrupt service routine for the time-freq processor.
{
  tfp.strobeIsr ();
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

// Connect the masp processor isr.
  if (intConnect ((VOIDFUNCPTR*)SP2_ADR,(VOIDFUNCPTR)maspReadIsr, 0)){
    perror ("intConnect maspReadIsr");
    exit(ERROR);
  }
// Enable the interrupt handler on the mv162 board.
  *VMECHIP2_LBIER = 0x05;

// Enable strobe interrupts from the tfp at 5 hertz.
  tfp.enableMinorStrobeInt (TFP_VCT, 3, 200);
}
/*****************************************************************************/
void maspReadIsr()
 
// Masp read isr wrapper.
{
  masp.masp_isr();
}
 
/*****************************************************************************/

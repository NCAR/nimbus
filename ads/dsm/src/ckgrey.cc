/* ckGrey.cc
   Test program for the VME based grey scale interface.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

// Set PROBE to 0 or 1, depending on which probe is to be sampled in the 
// single particle mode.
#define PROBE		0

#include <vxWorks.h>
#include <mv162.h>
#include <bootLib.h>
#include <logLib.h>
#include <intLib.h>
#include <stdioLib.h>
#include <stdio.h>
#include <taskLib.h>
#include <wdLib.h>

#include <dsmVmeDefs.h>
#include <header.h>

#include <GreyVme.h>

void draw_grey_image (GreyParticle*);
void init_isr ();
void isr_wrap (int probe);
void grey_wdisr ();
void statusMsg (char *msg);

unsigned char *buf;
WDOG_ID wid;
int tick = FALSE;
int hour;
int minute;
int second;
int msec;
int full;			// buffer full flag
int interrupt_enabled;		// interrupt enabled flag

static GreyVme *grey;

int ckgrey()
{
  struct GreyParticle *cur_part;
  int j;
  int k;
  int part_len;
  int tas = 20;
  
  hour = 8;
  minute = 30;
  second = 59;
  msec = 0;
  full = FALSE;
  interrupt_enabled = FALSE;

// Init the class.
  
  grey = new GreyVme((unsigned char*)(A24D16_BASE + GREYVME_BASE_0), statusMsg);
  grey->initProbe (0, "G1", GA1, 25, 5, 0);		// init probe 0
  grey->initProbe (1, "G2", GA1, 25, 5, 0);		// init probe 1
  grey->setTas (tas);
  grey->startSampling();
  grey->setTime (hour, minute, second);

// Uncomment this line to run with interrupts.
//  init_isr();
  
  while(1) {
  }
// Start up a watchdog timer at a 50 hertz interval.
  wid = wdCreate();
  wdStart ((WDOG_ID)wid, sysClkRateGet() / 50, (FUNCPTR)grey_wdisr, FALSE);

  while (1) {

// Uncomment this block to collect full buffers. 
    if (full || !interrupt_enabled) {

      full = FALSE;
      grey->collect();

      if (grey->bufFull()) {
        for (j = 0, k = 0, cur_part = (GreyParticle*)grey->buffer();
             (cur_part->slice_cnt != GREY_END_OF_RECORD) && 
             (j < grey->length());
             k = sizeof(*cur_part) + cur_part->slice_cnt * 
             GREY_BYTES_PER_SLICE + cur_part->chg_len * GREY_BYTES_PER_CHG_SMP, 
             (char*)cur_part += k, j += k) {
  
          if (cur_part->slice_cnt < 0) {
            printf ("slice_cnt = %d\n", cur_part->slice_cnt);
            exit(0);
          }

          draw_grey_image (cur_part);
        }

        printf ("BUFFER ACQUIRED\n");
        grey->setTas (tas);
        grey->releaseBuf();
      }
    }
        
// Uncomment this block to look at individual particles as they are acquired. 
/**
    grey->collectParticle (PROBE);
    if (grey->partLength(PROBE)) {
      draw_grey_image (grey->particle(PROBE));
      grey->releasePart(PROBE);
    }
**/

    taskDelay (sysClkRateGet() / 10);
  }
}
/*****************************************************************************/

void draw_grey_image (GreyParticle *part)

// Draws an ascii based image of a grey scale particle.
{
  int h, i, j, k;
  char image_str[80];
  struct GreySlice *slice = (struct GreySlice*)(part + 1);
  short *charge=(short*)((char*)(part+1)+part->slice_cnt*GREY_BYTES_PER_SLICE);

  printf ("\n%02d:%02d:%02d.%03d ena-> %3ld.%06ld acq-> %3ld.%06ld\n", 
           part->hour, part->minute, part->second, part->msec,
           part->ena_sec, part->ena_usec,
           part->acq_sec, part->acq_usec);
   
  printf ("id = %s, slice_cnt = %d, chg_len = %d, ovld_cnt = %d, tas = %d\n",
          part->id, part->slice_cnt, part->chg_len, part->ovld_cnt, part->tas);
  printf ("width = %d, length = %d\n", part->width, part->length);

  if ((part->slice_cnt < 0) || (part->slice_cnt > 128)) {
    printf ("slice_cnt exceeds limits.\n");
    exit(0);
  }


/* Display the image. */
  for (j = 0; j < part->slice_cnt; j++, slice++) {
    for (k = 0, h = 0; k < GREY_BYTES_PER_SLICE; k++) {
      for (i = 0; i < 4; i++, slice->segment[k] <<= 2, h++) {
        switch (slice->segment[k] & 0xC0) {
          case 0xC0:                                /* no shading */
            image_str[h] = ' ';
            break;
          case 0x40:                                /* min shading */
            image_str[h] = '/';
            break;
          case 0x80:                                /* mid shading */
            image_str[h] = 'X';
            break;
          case 0x00:                                /* max shading */
            image_str[h] = '#';
            break;
        }
      }
    }
    image_str[h] = '\0';
    printf ("%s\n", image_str);
  }

/* Display the charge data. */
  printf ("\nCharge Data\n");
  for (j = 0; j < part->chg_len; j += 2)
    printf ("%5d     ", *(charge + j));

  printf ("\nSplash Data\n");
  for (j = 1; j < part->chg_len; j += 2)
    printf ("%5d     ", *(charge + j));
  printf ("\n");
}
/*****************************************************************************/

void init_isr ()

// Sets up the isr routine.
{
  int stat;

// Connect the isr for both probes. 
  if (stat=intConnect ((VOIDFUNCPTR*)GREY_ADR0, (VOIDFUNCPTR)isr_wrap, 0)) {
    perror ("intConnect: probe 0");
    exit(1);
  }

  if (stat=intConnect ((VOIDFUNCPTR*)GREY_ADR1, (VOIDFUNCPTR)isr_wrap, 1)) {
    perror ("intConnect: probe 1");
    exit(1);
  }

// Enable the vmebus interrupt handler on the mv162 board for level 1.
  *VMECHIP2_LBIER = 0x01;

// Enable the interface BIM for a level 1 interrupt. The vector for probe 0
// must have a zero in its lsb.  The vector for probe 1 is GREY_VCT0 + 1.
  grey->enableInterrupt (GREY_VCT0, 1);
  interrupt_enabled = TRUE;
}
/*****************************************************************************/
 
void isr_wrap (int probe)
 
// C++ isr wrapper.
{
  grey->clearInterrupt();
  full = TRUE;
  logMsg ("Probe %1d interrupt occurred.\n", probe, 0,0,0,0,0);
}
/*****************************************************************************/

void grey_wdisr ()
{

// Restart the timer.
  wdStart ((WDOG_ID)wid, sysClkRateGet() / 50, (FUNCPTR)grey_wdisr, FALSE);

  tick = TRUE;

// Update the clock.
  if (!(msec = (msec + 10) % 1000)) {
    if (!(second = ++second % 60)) {
      if (!(minute = ++minute % 60))
        hour = ++hour % 24;
    }
  }

}
/*****************************************************************************/

void statusMsg (char *msg)
{
  printf (msg);
}
/*****************************************************************************/

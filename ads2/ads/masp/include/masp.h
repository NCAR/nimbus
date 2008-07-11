/* masp.h -- top level header file for MASP

   Original Author:  Keith S. Barr
   Copyright 1994
   National Center for Atmospheric Research
   Boulder, Colorado

   Written in support of ASHOE/MAESA

   Revision History:
      07/20/1994 ksb Created
*/

#ifndef MASP_H
#define MASP_H

/* do we have a MASP? */
#define FOR_MASP	TRUE

/* define the MASP constants */
#define INTERVAL      1000	/* minutes between cal. cycles		*/
#define MODE_LEVEL_1  5
#define MODE_LEVEL_2  5
#define MODE_LEVEL_3  5
#define MODE_WIDTH_1  5
#define MODE_WIDTH_2  5
#define MODE_WIDTH_3  5
#define KI            2000
#define KP            2000
#define TRIG_THRESH   1000	/* trigger threshold			*/
#define MIN_TTIME     10	/* minimum transit time			*/
#define MAX_MASP_PKTS 10 	/* doesn't apply to cal cycles!		*/
#define HISTO_INT     10	/* default seconds between histograms	*/
#define HISTO_BIN     1		/* default histogram grouping		*/
#define MASK_TOTAL_AR 800	/* masked acceptance ratio * 1000	*/
#define MASK_DOF_GAIN 1000	/* Masked Ap DOF Rejection Gain		*/
#define TOTAL_DOF_GAIN 1	/* Total Ap DOF Rejection Gain		*/

/* subroutine prototypes */
void PrintMaspHk(struct MASP_HK_PKT *hkpkt);	/* print HK to screen	*/
void PrintMaspPd(struct MASP_PART_PKT *data);	/* print PD to screen	*/
void unpak31 (unsigned short signal, unsigned short *low, /* unpack 3 to 1 */
              unsigned short *mid, unsigned short *high);
void unpak21 (unsigned short signal, unsigned short *low, /* unpack 2 to 1 */
              unsigned short *high);
void drawHistos();				/* draw histograms	*/
void sizeBins(); 				/* size bins for histos	*/
void buildHisto(struct MASP_PART_PKT *masp_dat); /* build histograms	*/
int probe_init();				/* initialize masp	*/
void masp_isr();				/* interrupt service routine */

#endif
   

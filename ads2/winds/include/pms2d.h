/* pms2d.h 

   Definitions for controlling and displaying the data from the pms 2d and
   grey scale probes.
*/

#ifndef PMS2D_H
#define PMS2D_H

#define C1         		0x4331	/* 'C1' */
#define C2         		0x4332	/* 'C2' */
#define P1         		0x5031	/* 'P1' */
#define P2        		0x5032	/* 'P2' */
#define H1        		0x4831	/* 'H1' */
#define H2        		0x4832	/* 'H2' */

#define GREY_MAX_LEVELS		3	/* number of grey scale levels */
#define GREY_MIN_IDX		0	/* min level array index */
#define GREY_MID_IDX		1	/* mid level array index */
#define GREY_MAX_IDX		2	/* max level array index */

#define GREY_PIXEL_MASK 	0xC0	/* upper pixel mask for a slice seg */
#define GREY_MIN_LEVEL		0x40	/* min level mask for upper 2 bits */
#define GREY_MID_LEVEL		0x80	/* mid level mask for upper 2 bits */
#define GREY_MAX_LEVEL		0x00	/* max level mask for upper 2 bits */

#define GREY_BITS_PER_PIXEL     2       /* grey scale bits per pixel */
#define GREY_PIX_PER_BYTE	4       /* grey scale pix per byte of data */
#define GREY_PIX_PER_SLICE	64	/* grey scale pix per slice of data */

#define HVPS_BITS_PER_PIXEL     1       /* hvps bits per pixel */
#define HVPS_PIX_PER_INT	32      /* hvps pix per 32 bit int of data */
#define HVPS_PIX_PER_SLICE	256	/* hvps pixels per slice */
#define HVPS_INTS_PER_SLICE	8	/* hvps 32 bit ints per slice */
#define HVPS_DECOMP_SIZE        4112	/* hvps decomp buf size (4096+32ovf) */
#define HVPS_PIX_MASK_SIZE      16	/* hvps pixel mask buf size (ints)*/

#define STD2D_PIXEL_MASK      	0x80000000      /* std 2d upper pixel mask */
#define STD2D_PIX_PER_SLICE	32  	/* standard 2d pix per slice of data */

#define P2D_PRIORITY_GET	1	/* get a display priority */
#define P2D_PRIORITY_INC	2	/* increment display priorities */
#define P2D_PRIORITY_RESET	3	/* reset a display priority */

#define P2D_DISP_RATE_MIN    	1       /* minimum display update interval */
#define P2D_DISP_RATE_MAX    	20      /* maximum display update interval */

#endif

/* ipicDefs.h
   MVME162 Industry Pack IC defs.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/
#ifndef IPICDEFS_H
#define IPICDEFS_H

/* Chip memory map */
#define IP_a_IO_BASE 		0xfff58000 		/* module A I/O adr */
#define IP_a_ID_BASE 		0xfff58080 		/* module A ID adr */
#define IP_b_IO_BASE 		0xfff58100 		/* module B I/O adr */
#define IP_b_ID_BASE 		0xfff58180 		/* module B ID adr */
#define IP_c_IO_BASE 		0xfff58200 		/* module C I/O adr */
#define IP_c_ID_BASE 		0xfff58280 		/* module C ID adr */
#define IP_d_IO_BASE 		0xfff58300 		/* module D I/O adr */
#define IP_d_ID_BASE 		0xfff58380 		/* module D ID adr */

/* Control registers */
#define IPIC_IP_a_INT0_CTL	(char*)0xfffbc010	/* mod A intr 0 ctl */
#define IPIC_IP_a_INT1_CTL	(char*)0xfffbc011	/* mod A intr 1 ctl */
#define IPIC_IP_b_INT0_CTL	(char*)0xfffbc012	/* mod B intr 0 ctl */
#define IPIC_IP_b_INT1_CTL	(char*)0xfffbc013	/* mod B intr 1 ctl */
#define IPIC_IP_c_INT0_CTL	(char*)0xfffbc014	/* mod C intr 0 ctl */
#define IPIC_IP_c_INT1_CTL	(char*)0xfffbc015	/* mod C intr 1 ctl */
#define IPIC_IP_d_INT0_CTL	(char*)0xfffbc016	/* mod D intr 0 ctl */
#define IPIC_IP_d_INT1_CTL	(char*)0xfffbc017	/* mod D intr 1 ctl */

/* Interrupt control register bits. */
#define IPIC_INT_CTL_LEVEL_1	(char)0x01		/* level 1 interrupt */
#define IPIC_INT_CTL_LEVEL_2	(char)0x02		/* level 2 interrupt */
#define IPIC_INT_CTL_LEVEL_3	(char)0x03		/* level 3 interrupt */
#define IPIC_INT_CTL_LEVEL_4	(char)0x04		/* level 4 interrupt */
#define IPIC_INT_CTL_LEVEL_5	(char)0x05		/* level 5 interrupt */
#define IPIC_INT_CTL_LEVEL_6	(char)0x06		/* level 6 interrupt */
#define IPIC_INT_CTL_LEVEL_7	(char)0x07		/* level 7 interrupt */
#define IPIC_INT_CTL_ICLR	(char)0x08		/* clr edge sens int */
#define IPIC_INT_CTL_IEN	(char)0x10		/* enable interrupt */
#define IPIC_INT_CTL_INT	(char)0x20		/* int pending status */
#define IPIC_INT_CTL_EDGE	(char)0x40		/* enable edge sense */
#define IPIC_INT_CTL_PLTY	(char)0x80		/* int active hi/lo */
 
#endif
 

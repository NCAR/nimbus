/* scn68562.h
   Defines the scn68562 serial communications controller.
 
   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:
*/

#ifndef SCN68562_H
#define SCN68562_H

/* Define register bits. */

#define CMR1_NO_PARITY   	0x00
#define CMR1_EVEN_PARITY 	0x10
#define CMR1_ODD_PARITY  	0x30
#define CMR1_ASYNC_MODE  	0x07

#define CMR2_POLL_INT	 	0x38

#define TPR_RPR_BITS_5	 	0x00
#define TPR_RPR_BITS_6	 	0x01
#define TPR_RPR_BITS_7	 	0x02
#define TPR_RPR_BITS_8	 	0x03
#define TPR_CTS_ENAB	 	0x04
#define TPR_RTS_CTL      	0x08
#define TPR_STOP_1_000	 	0x70
#define TPR_STOP_2_000	 	0xF0

#define TTR_BRG_CLK	   	0x30
#define TTR_RTR_BAUD_1200  	0x09
#define TTR_RTR_BAUD_2400  	0x0B
#define TTR_RTR_BAUD_4800  	0x0C
#define TTR_RTR_BAUD_9600  	0x0D
#define TTR_RTR_BAUD_19200 	0x0E
#define TTR_RTR_BAUD_38400 	0x0F

#define RPR_STRIP_PARITY	0x08
#define RPR_RTS_CTL		0x10

#define RTR_BRG_CLK 		0x20

#define OMR_TRUE_OUTPUTS	0x07

#define CTCR_NO_PRESET		0x40
#define CTCR_PRESCALE_32	0x10
#define CTCR_RTXC_CLK		0x00

#define CCR_RX_RESET		0x40
#define CCR_RX_ENABLE		0x42

#define PCR_SYNRTS_RTS		0x20
#define PCR_RTXC_INPUT		0x00
#define PCR_TRXC_INPUT		0x00
#define PCR_TRXC_TXCLK_1X	0x06

#endif


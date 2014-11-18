/*
 * uart2ssi.h
 *
 *  Created on: Sep 9, 2014
 *      Author: bakerd
 */

#ifndef UART2SSI_H_
#define UART2SSI_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>//tolower()
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include "grlib/grlib.h"

#define CONFIG_WRITE	0x03
#define CONFIG_READ		0x0B

void UARTIntHandler(void);
void SSI1IntHandler(void);
void SSI0IntHandler(void);
void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count);
void UARTSend_NB(const uint8_t *pui8Buffer, uint32_t ui32Count);
void ADC_1_init(uint32_t cfg_reg_val);
void ADC_0_init(uint32_t cfg_reg_val);
void uart2ssi_config(void);

short int reset_flag_0, reset_flag_1;
static unsigned long nbytes;
short int state=0;//1 for first, 2 for normal read, 3 for ADC read, else 0
//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif


#endif /* UART2SSI_H_ */

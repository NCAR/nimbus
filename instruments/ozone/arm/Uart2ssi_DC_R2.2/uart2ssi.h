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
#include "driverlib/adc.h"
#include "driverlib/rom.h"
#include "grlib/grlib.h"
#include "driverlib/systick.h"

#define CONFIG_WRITE	0x03
#define CONFIG_READ		0x0B
#define CS_A GPIO_PIN_0
#define CS_B GPIO_PIN_1

void UARTIntHandler(void);
void ADC_init_a(uint32_t cfg_reg_val);
void ADC_init_b(uint32_t cfg_reg_val);
void ADC_Sync(void);
void uart2ssi_config(void);
void blink(void);
void wait(int mSecs);
void dPot_pr_ctr(char chan, bool state);
void dPot_wp_ctr(char chan, bool state);
void dPot_ctr(char chan, int command_num, uint32_t address, uint32_t data);
void detector_pCyc(int PosOrNeg);
void chip_select(int csVal, char csChan, char csDev );


short int  reset_flag_1;
static unsigned long nbytes;
char cs;//chip select variable
bool cont_mode=0;
uint32_t new_data_A[4],new_data_B[4];
static uint32_t config_reg_val;
static uint32_t dPot_data[2];
int time_trigger=0;
int adc_init_a=0, adc_init_b=0;


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

/*
 * uart2ssi.c
 *
 *  Created on: Aug 10, 2014
 *      Author: bakerd
 */

#include "uart2ssi.h"

//*****************************************************************************
//
// This Program will take data from the UART module, send the data on the SSI
// module. If data is returned from the SSI, it is then sent back to the UART
//
//*****************************************************************************
int main(void){

	//
	// Call the function to configure
	// SSI and UART modules
	//
	uart2ssi_config();

    //
    // Loop forever
    //
    while(1)
    {
		//
		// Blink the LED
		//
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);

		//
		// Delay
		//
		SysCtlDelay(SysCtlClockGet() / (500 * 3));

		//
		// Turn off the LED
		//
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
		SysCtlDelay(SysCtlClockGet() / (3/3));
    }
}//end main

//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************
void
UARTIntHandler(void)
{
    uint32_t ui32Status;
    int i;
    static char rwc=0, udata[16];
    static uint32_t cmd;
    static uint32_t cfg_val[4];
    //
    // Get the interrupt status.
    //
	ui32Status = ROM_UARTIntStatus(UART1_BASE, true);

    //
    // Clear the asserted interrupts.
    //
	ROM_UARTIntClear(UART1_BASE, ui32Status);

	//
	//Uart convertor message format: uint8_t data;
	//byte 1 - 'W' or 'w' for write command
	//		 - 'R' or 'r' for read command
	//		 - 'C' or 'c' for conversion read
	//byte 2 - # of read/write bytes
    //byte 3 - 8 bit command
	//byte 4
	// - byte n	- null or 0 for read, or write data

    rwc=ROM_UARTCharGet(UART1_BASE);
    switch(tolower((rwc)))
    {
    case 'w':	nbytes=ROM_UARTCharGet(UART1_BASE);
   				cmd=ROM_UARTCharGet(UART1_BASE);
   				for(i=0; i<nbytes ; i++)
   				{
   					udata[i]=ROM_UARTCharGetNonBlocking(UART1_BASE);
   				}
   				while(SSIBusy(SSI1_BASE));
   				while(SSIBusy(SSI0_BASE));
   	   	   	   	ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 , 0x00);//CS low - active
   				ROM_SSIDataPut(SSI0_BASE, (uint32_t) cmd);
   				ROM_SSIDataPut(SSI1_BASE, (uint32_t) cmd);
   				for(i=0; i<nbytes; i++)
   				{
   					ROM_SSIDataPut(SSI0_BASE, udata[i]);
   					ROM_SSIDataPut(SSI1_BASE, udata[i]);
   				}
   				while(SSIBusy(SSI1_BASE));
   				while(SSIBusy(SSI0_BASE));
   				ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 , 0x03);//CS A high - inactive
      	   	    break;

    case 'r':   nbytes=ROM_UARTCharGet(UART1_BASE);
				cmd=ROM_UARTCharGet(UART1_BASE);
				while(SSIBusy(SSI1_BASE));
				while(SSIBusy(SSI0_BASE));
				ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 , 0x00);//CS low - active
				ROM_SSIDataPut(SSI0_BASE,(uint32_t) cmd);
				ROM_SSIDataPut(SSI1_BASE, (uint32_t) cmd);
				state=2;
				for(i=0; i<nbytes; i++)
				{
					while(SSIBusy(SSI1_BASE));
					ROM_SSIIntEnable(SSI1_BASE, SSI_RXFF);
					ROM_SSIDataPut(SSI1_BASE,0x00);
					ROM_SSIIntDisable(SSI1_BASE, SSI_RXFF);
				}
				while(SSIBusy(SSI1_BASE));
				while(SSIBusy(SSI0_BASE));
				ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 , 0x03);//CS A high - inactive
	   	        break;

    case 'c':	nbytes=ROM_UARTCharGet(UART1_BASE);
				cmd=ROM_UARTCharGet(UART1_BASE);

				while(SSIBusy(SSI1_BASE));
				while(SSIBusy(SSI0_BASE));
				ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 , 0x00);//CS low - active
				ROM_SSIDataPut(SSI0_BASE, (uint32_t) cmd);
				ROM_SSIDataPut(SSI1_BASE, (uint32_t) cmd);
				while(ROM_GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4));
				ROM_SSIDataPut(SSI0_BASE,0x00);// first 8 clks to shift out register
				ROM_SSIDataPut(SSI1_BASE,0x00);
				state=3;
				for(i=0; i<3; i++) // next 24 clks return conversion value
				{
					while(SSIBusy(SSI1_BASE));
					ROM_SSIIntEnable(SSI1_BASE, SSI_RXFF);
					ROM_SSIDataPut(SSI1_BASE,0x00);
					while(SSIBusy(SSI1_BASE));
					ROM_SSIIntDisable(SSI1_BASE, SSI_RXFF);
				}
				ROM_SSIDataPut(SSI0_BASE,0x00);// first 8 clks to shift out register
				ROM_SSIDataPut(SSI1_BASE,0x00);
				while(SSIBusy(SSI1_BASE));
				while(SSIBusy(SSI0_BASE));
				ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 , 0x03);//CS A high - inactive
				//ROM_SSIIntDisable(SSI1_BASE, SSI_RXFF);
	   	        break;

    case 's':   cfg_val[0]= ROM_UARTCharGet(UART1_BASE);
				cfg_val[1]= ROM_UARTCharGet(UART1_BASE);
				cfg_val[2]= ROM_UARTCharGet(UART1_BASE);
				cfg_val[3]= ROM_UARTCharGet(UART1_BASE);

				cmd= ((cfg_val[0]<<24) | ((cfg_val[1]<<16) | ((cfg_val[2]<<8) | cfg_val[0])));
    			// ADC_0_init(cmd);
    			ADC_1_init(cmd);
    			break;

    //default :	ROM_UARTCharPutNonBlocking(UART1_BASE, rwc);
				//break;
   }

    //
    // Blink the LED to show a character transfer is occuring.
    //
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);

	//
	// Delay for 1 millisecond.  Each SysCtlDelay is about 3 clocks.
	//
	SysCtlDelay(SysCtlClockGet() / (1000 * 3));// one hundred nanoseconds?

	//
	// Turn off the LED
	//
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
}

//*****************************************************************************
//
// Interrupt handler for the SSI1 module
//
//*****************************************************************************
void
SSI1IntHandler(void)
{
    uint32_t ui32Status;
    static uint32_t ssi_buf[8], SDI_hi_buf[8];
  //  static char uart_buf[32];
    volatile short int i;
    //
    // Get the interrupt status.
    //
	ui32Status = ROM_SSIIntStatus(SSI1_BASE, true);

    //
    // Clear the asserted interrupts.
    //
	ROM_SSIIntClear(SSI1_BASE, ui32Status);

	switch(state)//1 for first, 2 for normal read, 3 for ADC read, else 0
	{

	case 1:	ROM_SSIDataGet(SSI1_BASE, ssi_buf);
			if(ssi_buf[0]==0x10)
			{
				reset_flag_1=0;
				state=0;
			}

			break;

	case 2:	for(i=0; i<nbytes; i++)
			{
				ROM_SSIDataGet(SSI1_BASE, &ssi_buf[i]);
				ROM_UARTCharPut(UART1_BASE, ssi_buf[i]);
			}
			state=0;
			break;

	case 3://	ROM_SSIDataGet(SSI1_BASE, &ssi_buf[3]);
			// ROM_SSIDataGet(SSI1_BASE, &ssi_buf[2]);
			// ROM_SSIDataGet(SSI1_BASE, &ssi_buf[1]);
			// ROM_SSIDataGet(SSI1_BASE, ssi_buf);
			for(i=0; i<nbytes; i++)
			{
				ROM_SSIDataGet(SSI1_BASE, &ssi_buf[i]);
				ROM_UARTCharPut(UART1_BASE, ssi_buf[i]);
			}
			state=0;
			break;//ADC read

	default:ROM_SSIDataGet(SSI1_BASE, SDI_hi_buf);
			break;
	}

}


/*-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-
 * -<>-<>-<>-<>-<>-<>-FUNCTIONS-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-
 -<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-*/

//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void
UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    //
    // Loop while there are more characters to send.
    //
    while(ui32Count--)
    {
        //
        // Write the next character to the UART.
        //
        ROM_UARTCharPutNonBlocking(UART1_BASE, *pui8Buffer);
    }
}

//*****************************************************************************
//
// Initialize CS5530 on SSI1
//
//*****************************************************************************
void
ADC_1_init(uint32_t cfg_reg_val)
{
	volatile short int i;
    reset_flag_1=1;
    state=1;

    //  ----  ADC Initialization
	//SYNC
	while(SSIBusy(SSI1_BASE));
  	ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 , 0x00);//CS low - active
  	for(i=0; i<16; i++)
  	{

  		ROM_SSIDataPut(SSI1_BASE, 0xFF);
  		ROM_SSIDataPut(SSI1_BASE, 0xFF);
  		ROM_SSIDataPut(SSI1_BASE, 0xFF);
  		ROM_SSIDataPut(SSI1_BASE, 0xFF);
  		ROM_SSIDataPut(SSI1_BASE, 0xFF);
  	}
	ROM_SSIDataPut(SSI1_BASE, 0xFF);
	ROM_SSIDataPut(SSI1_BASE, 0xFF);
	ROM_SSIDataPut(SSI1_BASE, 0xFF);
	ROM_SSIDataPut(SSI1_BASE, 0xFF);
	ROM_SSIDataPut(SSI1_BASE, 0xFE);
	while(SSIBusy(SSI1_BASE));
	ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x03);//CS high - inactive
	SysCtlDelay(SysCtlClockGet() / (5000* 3));

	while(reset_flag_1)//repeat until a valid reset is read
	{

		while(SSIBusy(SSI1_BASE));
		ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 , 0);//CS low - active
		ROM_SSIDataPut(SSI1_BASE, 0X03);// write reset bit high
		ROM_SSIDataPut(SSI1_BASE, 0x20);
		ROM_SSIDataPut(SSI1_BASE, 0x00);
		ROM_SSIDataPut(SSI1_BASE, 0x00);
		ROM_SSIDataPut(SSI1_BASE, 0x00);
		while(SSIBusy(SSI1_BASE));
		ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 , 0x03);//CS high - inactive

		SysCtlDelay(SysCtlClockGet() / (2000000 * 3));//delay for 2 uSeconds (8 master clks ~= 1.63uS)

		while(SSIBusy(SSI1_BASE));
		ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);//CS low - active
		ROM_SSIDataPut(SSI1_BASE, 0x03); // write reset bit low
		ROM_SSIDataPut(SSI1_BASE, 0x00);
		ROM_SSIDataPut(SSI1_BASE, 0x00);
		ROM_SSIDataPut(SSI1_BASE, 0x00);
		ROM_SSIDataPut(SSI1_BASE, 0x00);
		while(SSIBusy(SSI1_BASE));
		ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x03);//CS high - inactive

		while(SSIBusy(SSI1_BASE));
		ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 , 0x00);//CS low - active
		ROM_SSIDataPut(SSI1_BASE, 0x0B); // read reset bit
		while(SSIBusy(SSI1_BASE));
		ROM_SSIIntEnable(SSI1_BASE, SSI_RXFF);
		while(SSIBusy(SSI1_BASE));
		ROM_SSIDataPut(SSI1_BASE, 0x00);
		while(SSIBusy(SSI1_BASE));
		ROM_SSIIntDisable(SSI1_BASE, SSI_RXFF);
		while(SSIBusy(SSI1_BASE));
		ROM_SSIDataPut(SSI1_BASE, 0x00);
		ROM_SSIDataPut(SSI1_BASE, 0x00);
		ROM_SSIDataPut(SSI1_BASE, 0x00);
		while(SSIBusy(SSI1_BASE));

		ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 , 0x03);//CS high - inactive

		SysCtlDelay(SysCtlClockGet() / (2000000 * 3));//delay for 2 uSeconds (8 master clks ~= 1.63uS)

	}

	while(SSIBusy(SSI1_BASE));
	ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 , 0);//CS low - active
	ROM_SSIDataPut(SSI1_BASE, 0x03);// write reset bit high
	ROM_SSIIntEnable(SSI1_BASE, SSI_RXFF);
	ROM_SSIDataPut(SSI1_BASE, cfg_reg_val>>24);
	ROM_SSIDataPut(SSI1_BASE, cfg_reg_val>>16);
	ROM_SSIDataPut(SSI1_BASE, cfg_reg_val>>8);
	ROM_SSIDataPut(SSI1_BASE, cfg_reg_val);
	ROM_SSIIntDisable(SSI1_BASE, SSI_RXFF);
	while(SSIBusy(SSI1_BASE));
	ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 , 0x03);//CS high - inactive
}


//*****************************************************************************
//
// Configure the peripherals for this program
//
//*****************************************************************************
void
uart2ssi_config(void)
{
	//
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    //
    ROM_FPULazyStackingEnable();
	//ROM_FPUStackingEnable();

    //
    // Set the clocking to run directly from the crystal.
    //
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    //
    // Enable the peripherals
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//Uart 0 and SSI0
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//Uart 1
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);//SSI 1
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);// HW control
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);// User LED
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);

    //
    // Enable the GPIO pins for the LED (PF2).
    //
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1| GPIO_PIN_2  );

    //
    // Set GPIO B0 and B1 as UART1 pins.
    //
    ROM_GPIOPinConfigure(GPIO_PB0_U1RX);
    ROM_GPIOPinConfigure(GPIO_PB1_U1TX);
    ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Set GPIO A2, A4 and A5 as SSI0 pins.
    //
    ROM_GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    ROM_GPIOPinConfigure(GPIO_PA4_SSI0RX);
    ROM_GPIOPinConfigure(GPIO_PA5_SSI0TX);
    ROM_GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);

    //
    // Set GPIO D0, D3 and F0 as SSI1 pins.
    //
    ROM_GPIOPinConfigure(GPIO_PD0_SSI1CLK);
    ROM_GPIOPinConfigure(GPIO_PD2_SSI1RX);
    ROM_GPIOPinConfigure(GPIO_PD3_SSI1TX);
    ROM_GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    //GPIO control pins
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0 );//to act as CS A for longer conversions
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1 );//to act as CS B for longer conversions
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_4  );//SDO flag controller

    //
    // Configure the UARTs for 115200, 8-N-1 operation.
    //
    ROM_UARTConfigSetExpClk(UART1_BASE, ROM_SysCtlClockGet(), 115200,
                                (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                                 UART_CONFIG_PAR_NONE));

    //
    // Configure the SSIs for moto mode 0, master, 1MHz, 8bit word operation.
    //
    ROM_SSIConfigSetExpClk(SSI0_BASE, ROM_SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 2000000, 8);
    ROM_SSIConfigSetExpClk(SSI1_BASE, ROM_SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 2000000, 8);

    //
    // Enable the UART interrupts.
    //
    ROM_IntEnable(INT_UART1);
    ROM_UARTIntEnable(UART1_BASE, UART_INT_RX| UART_INT_RT );

    //
    // Enable the SSI modules
    //
    ROM_SSIEnable(SSI0_BASE);
    ROM_SSIEnable(SSI1_BASE);
    ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 , 0x03);//CS high for active low

    //
    // Enable the SSI interrupts.
    //
   // ROM_IntEnable(INT_SSI0);
   // ROM_SSIIntEnable(SSI0_BASE, SSI_RXFF | SSI_RXOR );
    ROM_IntEnable(INT_SSI1);
    //ROM_SSIIntEnable(SSI1_BASE, SSI_RXFF);// | SSI_RXOR | SSI_RXTO  );

    //
    // set interrupt priorities: SSI highest, Uart lower
    // (set by top 3 bits, 000 highest to 111 lowest)
    //
    ROM_IntPrioritySet(INT_SSI0, 0x00);
    ROM_IntPrioritySet(INT_SSI1, 0x00);
    ROM_IntPrioritySet(INT_UART1, 0x20);

    //
    // set state to indicate first run
    //
    state=1;

    //
    // Enable processor interrupts.
    //
    ROM_IntMasterEnable();
    ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
}


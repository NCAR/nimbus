/*
 * uart2ssi.c
 *
 *  Created on: Nov 14, 2014
 *      Author: Darin Baker
 */

#include "uart2ssi.h"
//*****************************************************************************
//
// This Program will take data from the UART module, send the data on the SSI
// module. If data is returned from the SSI, it is then sent back to the UART
//
//*****************************************************************************
int main(void)
{
	int i;
	uint32_t iData_a[4]={0,0,0,0};
	uint32_t iData_b[4]={0,0,0,0};
	uint32_t dead_buf[4];
	union Data
	{
		uint32_t i_buf;
		char  c_buf[4];
	} bias_buf_a, bias_buf_b;


	// Call the function to configure
	// SSI and UART modules
	uart2ssi_config();

    // Loop forever
    while(1)
    {
    	if(time_trigger)
    	{
    		//Disable UART to protect Data Acquisition. if a packet is received the interrupt will be asserted when re-enabled
    		ROM_UARTIntDisable(UART1_BASE, UART_INT_RX| UART_INT_RT );

    	    // Trigger the bias ADC sample sequence.
    	    ROM_ADCProcessorTrigger(ADC0_BASE, 1);

    		//Get Data A
    		//while(SSIBusy(SSI0_BASE));
    		chip_select(0,'a','c');
    		ROM_SSIDataPut(SSI0_BASE,0x00);//clock out so SSI peripheral will recognize incoming data
    		while(SSIBusy(SSI0_BASE));
    		while(ROM_SSIDataGetNonBlocking(SSI0_BASE, dead_buf));// clear SSI hardware FIFO/
    		for(i=0;i<3;i++)
    		{
    			ROM_SSIDataPut(SSI0_BASE,0x00);//clock out so SSI peripheral will recognize incoming data
    			while(SSIBusy(SSI0_BASE));
    			ROM_SSIDataGet(SSI0_BASE, &iData_a[i]);//get new data
    		}
    		ROM_SSIDataPut(SSI0_BASE,0x00);//clock out OVF byte
    		while(SSIBusy(SSI0_BASE));
    		while(ROM_SSIDataGetNonBlocking(SSI0_BASE, dead_buf));// clear SSI hardware FIFO and OVF byte
    		chip_select(1,'a','c');


    		// Read the value for bias A from the ADC.
    		ADCSoftwareOversampleDataGet(ADC0_BASE, 1, &bias_buf_a.i_buf, 1);

    		// Trigger the bias ADC sample sequence.
    		ROM_ADCProcessorTrigger(ADC0_BASE, 2);

    		//get data B
    		//while(SSIBusy(SSI0_BASE));
    		chip_select(0,'b','c');
    		ROM_SSIDataPut(SSI0_BASE,0x00);//clock out so SSI peripheral will recognize incoming data
    		while(SSIBusy(SSI0_BASE));
    		while(ROM_SSIDataGetNonBlocking(SSI0_BASE, dead_buf));// clear SSI hardware FIFO and OVF byte
    		for(i=0;i<3;i++)
    		{
    			ROM_SSIDataPut(SSI0_BASE,0x00);//clock out so SSI peripheral will recognize incoming data
    			while(SSIBusy(SSI0_BASE));
    			ROM_SSIDataGet(SSI0_BASE, &iData_b[i]);//get new data
    		}
    		ROM_SSIDataPut(SSI0_BASE,0x00);
    		while(SSIBusy(SSI0_BASE));
    		while(ROM_SSIDataGetNonBlocking(SSI0_BASE, dead_buf));// clear SSI hardware FIFO and OVF byte
    		chip_select(1,'b','c');


    		// Read the value for bias B from the ADC.
    		ADCSoftwareOversampleDataGet(ADC0_BASE, 2, &bias_buf_b.i_buf, 1);

			ROM_UARTCharPut(UART1_BASE, iData_a[0]);// Send O3 ADC data A
			ROM_UARTCharPut(UART1_BASE, iData_a[1]);// Send O3 ADC data A
			ROM_UARTCharPut(UART1_BASE, iData_a[2]);// Send O3 ADC data A
			ROM_UARTCharPut(UART1_BASE, iData_b[0]);// send O3 ADC data B
			ROM_UARTCharPut(UART1_BASE, iData_b[1]);// send O3 ADC data B
			ROM_UARTCharPut(UART1_BASE, iData_b[2]);// send O3 ADC data B
			ROM_UARTCharPut(UART1_BASE, bias_buf_a.c_buf[1]);// send bias ADC data A
			ROM_UARTCharPut(UART1_BASE, bias_buf_a.c_buf[0]);// send bias ADC data A
			ROM_UARTCharPut(UART1_BASE, bias_buf_b.c_buf[1]);// send DC bias ADC data B
			ROM_UARTCharPut(UART1_BASE, bias_buf_b.c_buf[0]);// send DC bias ADC data B

/*			iData_a[0]=0;
			iData_a[1]=0;
			iData_a[2]=0;
			iData_b[0]=0;
			iData_b[1]=0;
			iData_b[2]=0;
			bias_buf_a.i_buf=0;
			bias_buf_b.i_buf=0;
*/
			time_trigger=0;
			ROM_UARTIntEnable(UART1_BASE, UART_INT_RX| UART_INT_RT );
    	}

    	if(adc_init_a)
    	{
    		ADC_init_a(config_reg_val);
    		adc_init_a=0;
    	}

    	if(adc_init_b)
    	{
    		ADC_init_b(config_reg_val);
    		adc_init_b=0;
    	}

    	//Over-sample and accumulate for averaging



    }//end while
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
    static uint32_t udata[16];
    static uint32_t cfg_val[4], chan, cmd, /*last_conv_buff[16],*/ dead_buf[16], dPot_address, dPot_state;
   // uint32_t rd=1;
    static union data
    	{
    		uint32_t i_buf;
    		char  c_buf[4];
    	} dPot_data,bias_buf_a, bias_buf_b;

    // Get the interrupt status.
	ui32Status = ROM_UARTIntStatus(UART1_BASE, true);

    // Clear the asserted interrupts.
	ROM_UARTIntClear(UART1_BASE, ui32Status);

	ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_PIN_5);//Uart Received indicator flag

	//Uart convertor message format: uint8_t data;
	//byte 1 - 'W' or 'w' for write command
	//		 - 'R' or 'r' for read command
	//		 - 'C' or 'c' for conversion read
	//
	//byte 2 - # of read/write bytes
    //byte 3 - 8 bit command
	//byte 4 - channel A or B
	// - byte n	- null or 0 for read, or write data
	// Special Exception for case 'd' - digital pot control
    switch(ROM_UARTCharGet(UART1_BASE))
    {
    case 'a':	//get ADC A bias value
    			ROM_ADCProcessorTrigger(ADC0_BASE, 1);// trigger the ADC sequence
    			wait(10);
    			// Read the value for bias A from the ADC.
    			ADCSoftwareOversampleDataGet(ADC0_BASE, 1, &bias_buf_a.i_buf, 1);
    			ROM_UARTCharPut(UART1_BASE, bias_buf_a.c_buf[1]);// send bias ADC data A
    			ROM_UARTCharPut(UART1_BASE, bias_buf_a.c_buf[0]);
    			break;

    case 'b':	//get ADC B bias value
				ROM_ADCProcessorTrigger(ADC0_BASE, 2);// trigger the ADC sequence
				wait(10);
				// Read the value for bias A from the ADC.
				ADCSoftwareOversampleDataGet(ADC0_BASE, 2, &bias_buf_b.i_buf, 1);
				ROM_UARTCharPut(UART1_BASE, bias_buf_b.c_buf[1]);// send bias ADC data A
				ROM_UARTCharPut(UART1_BASE, bias_buf_b.c_buf[0]);
				break;

    case 'w':	nbytes=ROM_UARTCharGet(UART1_BASE);
   				cmd=ROM_UARTCharGet(UART1_BASE);
   				chan=ROM_UARTCharGet(UART1_BASE);
   				for(i=0; i<nbytes ; i++)
   				{
   					udata[i]=ROM_UARTCharGetNonBlocking(UART1_BASE);
   				}
   				while(SSIBusy(SSI0_BASE));

   	   	   	   	//ROM_GPIOPinWrite(GPIO_PORTE_BASE,  GPIO_PIN_1 , 0);//CS low - active
   				chip_select(0,chan,'c');

   				ROM_SSIDataPut(SSI0_BASE, (uint32_t) cmd);

   				for(i=0; i<nbytes; i++)
   				{
   					ROM_SSIDataPut(SSI0_BASE, udata[i]);
   				}
   				while(SSIBusy(SSI0_BASE));
   				ROM_SSIDataGetNonBlocking(SSI0_BASE, dead_buf);// clear SSI hardware FIFO
   				//ROM_GPIOPinWrite(GPIO_PORTE_BASE,  GPIO_PIN_1 , GPIO_PIN_1);//CS A high - inactive
   				chip_select(1,chan,'c');
      	   	    break;

    case 'r':   nbytes=ROM_UARTCharGet(UART1_BASE);
				cmd=ROM_UARTCharGet(UART1_BASE);
				chan=ROM_UARTCharGet(UART1_BASE);

				while(ROM_SSIDataGetNonBlocking(SSI0_BASE, dead_buf));// clear SSI hardware FIFO/

				while(SSIBusy(SSI0_BASE));
				chip_select(0,chan,'c');
				ROM_SSIDataPut(SSI0_BASE, (uint32_t) cmd);
				ROM_SSIDataGet(SSI0_BASE, &udata[0]);//get incoming data
				for(i=0; i<nbytes; i++)
				{
					while(SSIBusy(SSI0_BASE));
					ROM_SSIDataPut(SSI0_BASE,0x00);//send 0's to run sclk
					while(SSIBusy(SSI0_BASE));
					ROM_SSIDataGet(SSI0_BASE, &udata[i]);//get incoming data
				}
				while(SSIBusy(SSI0_BASE));
				chip_select(1,chan,'c');

				for(i=0; i<nbytes; i++)
				{
					ROM_UARTCharPut(UART1_BASE, udata[i]);//send data to Prom
				}
	   	        break;

    case 'c':	nbytes=ROM_UARTCharGet(UART1_BASE);
				cmd=ROM_UARTCharGet(UART1_BASE);
				chan=ROM_UARTCharGet(UART1_BASE);
				while(SSIBusy(SSI0_BASE));
				//ROM_GPIOPinWrite(GPIO_PORTE_BASE,  GPIO_PIN_1 , 0);//CS low - active
				chip_select(0,chan,'c');
				ROM_SSIDataPut(SSI0_BASE, (uint32_t) cmd);
				wait(535);
				ROM_SSIDataPut(SSI0_BASE,0x00);
				ROM_SSIDataGetNonBlocking(SSI0_BASE, dead_buf);//clear SSI hardware FIFO
				for(i=0; i<3; i++) // next 24 clks return conversion value
				{
					while(SSIBusy(SSI0_BASE));
					ROM_SSIDataPut(SSI0_BASE,0x00);
					ROM_SSIDataGet(SSI0_BASE, (uint32_t *) udata+i);
					ROM_UARTCharPut(UART1_BASE, udata[i]);//send data to Prom
				}

				ROM_SSIDataPut(SSI0_BASE,0x00);
				ROM_SSIDataGet(SSI0_BASE,(uint32_t *) udata);
				if(udata[0]==0x04)//Over Voltage Flag (OVF) has been asserted by the ADC
					ROM_GPIOPinWrite(GPIO_PORTE_BASE,  GPIO_PIN_2 , GPIO_PIN_2 );//OVF
				else
					ROM_GPIOPinWrite(GPIO_PORTE_BASE,  GPIO_PIN_2 , 0 );//OVF
				while(SSIBusy(SSI0_BASE));
				//ROM_GPIOPinWrite(GPIO_PORTE_BASE,  GPIO_PIN_1 , GPIO_PIN_1);//CS A high - inactive
				chip_select(1,chan,'c');
	   	        break;


    case 'i':  	cfg_val[0]= ROM_UARTCharGet(UART1_BASE);
				cfg_val[1]= ROM_UARTCharGet(UART1_BASE);
				cfg_val[2]= ROM_UARTCharGet(UART1_BASE);
				cfg_val[3]= ROM_UARTCharGet(UART1_BASE);
				config_reg_val= ((cfg_val[0]<<24) | ((cfg_val[1]<<16) | ((cfg_val[2]<<8) | cfg_val[3])));
				adc_init_a=1;
    			break;

    case 'j':  	cfg_val[0]= ROM_UARTCharGet(UART1_BASE);
				cfg_val[1]= ROM_UARTCharGet(UART1_BASE);
				cfg_val[2]= ROM_UARTCharGet(UART1_BASE);
				cfg_val[3]= ROM_UARTCharGet(UART1_BASE);
				config_reg_val= ((cfg_val[0]<<24) | ((cfg_val[1]<<16) | ((cfg_val[2]<<8) | cfg_val[3])));
				adc_init_b=1;
    			break;

    case 's':	ADC_Sync();
    			break;

    case 'x':	while(SSIBusy(SSI0_BASE));
				//ROM_GPIOPinWrite(GPIO_PORTE_BASE,  GPIO_PIN_0 | GPIO_PIN_1 , 0);//CS low - active
    			chip_select(0,'a','c');
    			chip_select(0,'b','c');
				ROM_SSIDataPut(SSI0_BASE, 0xC0);//Continuous conversion command
				while(ROM_SSIDataGetNonBlocking(SSI0_BASE,dead_buf));//clear SSI Hardware FIFO
				cont_mode=1;
				while(SSIBusy(SSI0_BASE));
				//ROM_GPIOPinWrite(GPIO_PORTE_BASE,  GPIO_PIN_0 | GPIO_PIN_1 , GPIO_PIN_0 | GPIO_PIN_1);//CS A high - inactive
				chip_select(1,'a','c');
				chip_select(1,'b','c');
				break;

    case 'q': 	cont_mode=0;
				chip_select(0,'a','c');
				chip_select(0,'b','c');
				ROM_SSIDataPut(SSI0_BASE, 0xFF);//send FF to both ADCs to stop continuous conversion
				ROM_SSIDataPut(SSI0_BASE, 0xFF);
				chip_select(1,'a','c');
				chip_select(1,'b','c');
    			while(ROM_SSIDataGetNonBlocking(SSI0_BASE,dead_buf));//clear SSI Hardware FIFO
    			break;

    case 'd':	// Case d for digital pot control
    			chan=ROM_UARTCharGet(UART1_BASE);//a or b
    			cmd=ROM_UARTCharGet(UART1_BASE);// instruction #
    			if(cmd==0x16)//WP
    			{
    				dPot_state=ROM_UARTCharGet(UART1_BASE);
    				dPot_wp_ctr(chan, (bool) dPot_state);
    			}

    			else if(cmd==0x17)//PR
    			{
    				dPot_state=ROM_UARTCharGet(UART1_BASE);
    				dPot_pr_ctr(chan, (bool) dPot_state);
    			}

    			else if (cmd==0x18)//repeat by CS Strobe
    			{
    				chip_select(0,chan,'d');
    				chip_select(1,chan,'d');
    			}
    			else if(cmd==0x3 || cmd==0x11)
    			{
    				dPot_address=ROM_UARTCharGet(UART1_BASE);
    				dPot_data.c_buf[0]=ROM_UARTCharGet(UART1_BASE);
    				dPot_data.c_buf[1]=ROM_UARTCharGet(UART1_BASE);
    				dPot_ctr(chan, cmd, dPot_address, dPot_data.i_buf);
    			}
    			else
    			{
    				dPot_address=ROM_UARTCharGet(UART1_BASE);
    				dPot_ctr(chan, cmd, dPot_address, 0x0000);
    			}
    			break;

    case 'p':	chan=ROM_UARTCharGet(UART1_BASE);	//Case p for power control
    			detector_pCyc(chan);
    			break;

    case 'z':	chan = ROM_UARTCharGet(UART1_BASE);
    			break;
   }
    ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0);
}//end UARTIntHandler

void
SysTickIntHandler(void)
{
	if(cont_mode)
	{
		time_trigger=1;
	}
}//end SysTickIntHandler

/*-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-
 * -<>-<>-<>-<>-<>-<>-FUNCTIONS-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-
 -<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-<>-*/
//*****************************************************************************
//
// Initialize CS5530 on SSI0
//
//*****************************************************************************
void
ADC_Sync(void)
{
	volatile short int i;

	while(SSIBusy(SSI0_BASE));
  	//ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);//CS low - active
	chip_select(0,'a','c');
	chip_select(0,'b','c');
  	for(i=0; i<20; i++)
  	{

  		ROM_SSIDataPut(SSI0_BASE, 0xFF);
  		ROM_SSIDataPut(SSI0_BASE, 0xFF);
  		ROM_SSIDataPut(SSI0_BASE, 0xFF);
  		ROM_SSIDataPut(SSI0_BASE, 0xFF);
  		ROM_SSIDataPut(SSI0_BASE, 0xFF);
  	}
	ROM_SSIDataPut(SSI0_BASE, 0xFF);
	ROM_SSIDataPut(SSI0_BASE, 0xFF);
	ROM_SSIDataPut(SSI0_BASE, 0xFF);
	ROM_SSIDataPut(SSI0_BASE, 0xFF);
	ROM_SSIDataPut(SSI0_BASE, 0xFE);
	while(SSIBusy(SSI0_BASE));
	//ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_0 | GPIO_PIN_1 );//CS high - inactive
	chip_select(1,'a','c');
	chip_select(1,'b','c');

	SysCtlDelay(SysCtlClockGet() / (5000* 3));
}//end ADC_Sync


void
ADC_init_a(uint32_t cfg_reg_val)
{
	volatile short int i;
    uint32_t ssi_data[16];
    bool success_a=0;

    //  ----  ADC Initialization
	//SYNC
    ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0);// success a indicator flag, set low

    	while(success_a)//init A
		{
			while(SSIBusy(SSI0_BASE));
			//ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);//CS low - active
			chip_select(0,'a','c');
			ROM_SSIDataPut(SSI0_BASE, 0X03);// write reset bit high
			ROM_SSIDataPut(SSI0_BASE, 0x20);
			ROM_SSIDataPut(SSI0_BASE, 0x00);
			ROM_SSIDataPut(SSI0_BASE, 0x00);
			ROM_SSIDataPut(SSI0_BASE, 0x00);
			while(SSIBusy(SSI0_BASE));
			//ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1);//CS high - inactive
			chip_select(1,'a','c');

			wait(1);

			while(SSIBusy(SSI0_BASE));
			//ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0x00);//CS low - active
			chip_select(0,'a','c');
			ROM_SSIDataPut(SSI0_BASE, 0x03); // write reset bit low
			ROM_SSIDataPut(SSI0_BASE, 0x00);
			ROM_SSIDataPut(SSI0_BASE, 0x00);
			ROM_SSIDataPut(SSI0_BASE, 0x00);
			ROM_SSIDataPut(SSI0_BASE, 0x00);
			while(SSIBusy(SSI0_BASE));
			//ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1);//CS high - inactive
			chip_select(1,'a','c');

			wait(1);

			while(SSIBusy(SSI0_BASE));
		//	ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0x00);//CS low - active
			chip_select(0,'a','c');
			ROM_SSIDataPut(SSI0_BASE, 0x0B); // read reset bit
			while(SSIBusy(SSI0_BASE));
			ROM_SSIDataGetNonBlocking(SSI0_BASE, ssi_data);//empty HW fifo
			while(SSIBusy(SSI0_BASE));
			ROM_SSIDataPut(SSI0_BASE, 0x00);
			ROM_SSIDataGet(SSI0_BASE, &ssi_data[15]);
			while(SSIBusy(SSI0_BASE));
			ROM_SSIDataPut(SSI0_BASE, 0x00);
			while(SSIBusy(SSI0_BASE));
			ROM_SSIDataPut(SSI0_BASE, 0x00);
			while(SSIBusy(SSI0_BASE));
			ROM_SSIDataPut(SSI0_BASE, 0x00);
			//ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1);//CS high - inactive
			chip_select(1,'a','c');

			wait(1);

			if(ssi_data[15]==0x10)
				success_a=1;
		}

    	ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_PIN_5);//success flag high
		while(SSIBusy(SSI0_BASE));
		//ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1 , 0);//CS low - active
		chip_select(0,'a','c');
		ROM_SSIDataPut(SSI0_BASE, 0x03);// set configuration value
		ROM_SSIDataPut(SSI0_BASE, cfg_reg_val>>24);
		while(SSIBusy(SSI0_BASE));
		ROM_SSIDataPut(SSI0_BASE, cfg_reg_val>>16);
		while(SSIBusy(SSI0_BASE));
		ROM_SSIDataPut(SSI0_BASE, cfg_reg_val>>8);
		while(SSIBusy(SSI0_BASE));
		ROM_SSIDataPut(SSI0_BASE, cfg_reg_val);
		while(SSIBusy(SSI0_BASE));
		//ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1 , GPIO_PIN_1);//CS high - inactive
		chip_select(1,'a','c');
		wait(1);
		ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0);//success flag low
}//end ADC_init_a


void
ADC_init_b(uint32_t cfg_reg_val)
{
	volatile short int i;
    uint32_t ssi_data[16];
    bool success_b=0;

    //  ----  ADC Initialization
	//SYNC
    ROM_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_5, 0);// success b indicator flag, set low

    	while(success_b)//init B
		{
			while(SSIBusy(SSI0_BASE));
			//ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0);//CS low - active
			chip_select(0,'b','c');
			ROM_SSIDataPut(SSI0_BASE, 0X03);// write reset bit high
			ROM_SSIDataPut(SSI0_BASE, 0x20);
			ROM_SSIDataPut(SSI0_BASE, 0x00);
			ROM_SSIDataPut(SSI0_BASE, 0x00);
			ROM_SSIDataPut(SSI0_BASE, 0x00);
			while(SSIBusy(SSI0_BASE));
			//ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_PIN_0);//CS high - inactive
			chip_select(1,'b','c');

			wait(1);

			while(SSIBusy(SSI0_BASE));
			//ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0x00);//CS low - active
			chip_select(0,'b','c');
			ROM_SSIDataPut(SSI0_BASE, 0x03); // write reset bit low
			ROM_SSIDataPut(SSI0_BASE, 0x00);
			ROM_SSIDataPut(SSI0_BASE, 0x00);
			ROM_SSIDataPut(SSI0_BASE, 0x00);
			ROM_SSIDataPut(SSI0_BASE, 0x00);
			while(SSIBusy(SSI0_BASE));
			//ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_PIN_0);//CS high - inactive
			chip_select(1,'b','c');

			wait(1);

			while(SSIBusy(SSI0_BASE));
			//ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0x00);//CS low - active
			chip_select(0,'b','c');
			ROM_SSIDataPut(SSI0_BASE, 0x0B); // read reset bit
			while(SSIBusy(SSI0_BASE));
			ROM_SSIDataGetNonBlocking(SSI0_BASE, ssi_data);//empty HW fifo
			while(SSIBusy(SSI0_BASE));
			ROM_SSIDataPut(SSI0_BASE, 0x00);
			ROM_SSIDataGet(SSI0_BASE, &ssi_data[15]);
			while(SSIBusy(SSI0_BASE));
			ROM_SSIDataPut(SSI0_BASE, 0x00);
			while(SSIBusy(SSI0_BASE));
			ROM_SSIDataPut(SSI0_BASE, 0x00);
			while(SSIBusy(SSI0_BASE));
			ROM_SSIDataPut(SSI0_BASE, 0x00);
			//ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_PIN_0);//CS high - inactive
			chip_select(1,'b','c');

			wait(1);
			if(ssi_data[15]==0x10)
				success_b=1;
		}

    	ROM_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_5, GPIO_PIN_5);//success flag high
		while(SSIBusy(SSI0_BASE));
		//ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 , 0);//CS low - active
		chip_select(0,'b','c');
		ROM_SSIDataPut(SSI0_BASE, 0x03);// set configuration value
		ROM_SSIDataPut(SSI0_BASE, cfg_reg_val>>24);
		while(SSIBusy(SSI0_BASE));
		ROM_SSIDataPut(SSI0_BASE, cfg_reg_val>>16);
		while(SSIBusy(SSI0_BASE));
		ROM_SSIDataPut(SSI0_BASE, cfg_reg_val>>8);
		while(SSIBusy(SSI0_BASE));
		ROM_SSIDataPut(SSI0_BASE, cfg_reg_val);
		while(SSIBusy(SSI0_BASE));
		//ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 , GPIO_PIN_0);//CS high - inactive
		chip_select(1,'b','c');
		wait(1);
		ROM_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_5, 0);//success flag low
}// end ADC_init_b


//*****************************************************************************
//
// Configure the peripherals for this program
//
//*****************************************************************************
void
uart2ssi_config(void)
{
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    ROM_FPULazyStackingEnable();

    // Set the clocking to run directly from the crystal.
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    // Enable the peripherals
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);// ADC0
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//Uart 0
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//GPIO
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);//SSI 1
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//GPIO
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
   // ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);

    /*     	Setup GPIO control pins    		*/
    //GPIO control pins
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3 );//PB2=SDNN_CTR; PB3=CSPA
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_5 |GPIO_PIN_6 );//PD6=CSPB
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 );//PE0=CSAB; PE1=CSAA; PE4=SDNP_CTR
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_3 );//PA3=RDYA
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_7 );//PD7=RDYB
    ROM_GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_3);//PD3=Ain4=VB2B
    ROM_GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);//PE3=Ain0=VB2A


    //Set Chip Selects (CSs) high
    ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3);//CSPA High
    ROM_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, GPIO_PIN_6);//CSPB High
    ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_0 | GPIO_PIN_1 );//CSAA and CSAB High

    // Set Shutdown Control pins low
    ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0);//SDNN_CTR low
    ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);//SDNP_CTR low



    /*  Configure and enable UART  */
    // Set GPIO B0 and B1 as UART1 pins.
    ROM_GPIOPinConfigure(GPIO_PB0_U1RX);
    ROM_GPIOPinConfigure(GPIO_PB1_U1TX);
    ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);


    // Configure the UARTs for 115200, 8-N-1 operation.
    ROM_UARTConfigSetExpClk(UART1_BASE, ROM_SysCtlClockGet(), 115200,
                                (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                                 UART_CONFIG_PAR_NONE));

    // Enable the UART interrupts.
    ROM_IntEnable(INT_UART1);
    ROM_UARTIntEnable(UART1_BASE, UART_INT_RX| UART_INT_RT );

    // Set GPIO as SSI0 pins.
    ROM_GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    ROM_GPIOPinConfigure(GPIO_PA4_SSI0RX);
    ROM_GPIOPinConfigure(GPIO_PA5_SSI0TX);
    ROM_GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5);

    /*	Configure and enable SSI	*/
    // Configure the SSIs for moto mode 0, master, 1MHz, 8bit word operation.
    ROM_SSIConfigSetExpClk(SSI0_BASE, ROM_SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 2000000, 8);

    // Enable the SSI modules
    ROM_SSIEnable(SSI0_BASE);
    ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 , 0x03);//CS high for active low

    /* 	setup and configure ADC  */
    // Enable the first sample sequencer to capture the value of channel 0 when
    // the processor trigger occurs.
    ROM_ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
    ROM_ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 1);

    ROM_ADCHardwareOversampleConfigure(ADC0_BASE, 64);

    ADCSoftwareOversampleConfigure(ADC0_BASE, 1, 4);
    ADCSoftwareOversampleConfigure(ADC0_BASE, 2, 4);

    ADCSoftwareOversampleStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_END | ADC_CTL_CH0);
    ADCSoftwareOversampleStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_END | ADC_CTL_CH4);

    ROM_ADCSequenceEnable(ADC0_BASE, 1);
    ROM_ADCSequenceEnable(ADC0_BASE, 2);




    /*setup and configure SysTick */
    ROM_SysTickEnable();
    ROM_SysTickIntEnable ();
    SysTickPeriodSet (2133333);//2133333 16MHz clocks for 7.5Hz Wrap


    // Enable processor interrupts.
    ROM_IntMasterEnable();
}//end uart2ssi_config

void
wait(int mSecs)
{
	int i;
	for(i=0;i<mSecs;i++)// SDO does not stay high as it should, this will compensate
	{
		// Delay for 1 millisecond.  Each SysCtlDelay is about 3 clocks.
		SysCtlDelay(SysCtlClockGet() / (1000 * 3));
	}
}// end wait

void
dPot_pr_ctr(char chan, bool state)
{
	uint32_t cmd_mask;
	uint32_t dead_data[4];
	if(state==1)
		cmd_mask = config_reg_val | 0x01000000;//pr controlled by ADC output A1, set A1 high
	else
		cmd_mask = config_reg_val & 0xFEFFFFFF;//set A1 low

	while(SSIBusy(SSI0_BASE));

	/*if(chan == 'a'||chan == 'A')
		ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1 , 0);//CSAA low - active
	else if(chan == 'b' || chan == 'B')
		ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 , 0);//CSAB low - active*/
	chip_select(0,chan,'d');

	SysCtlDelay(1);

	ROM_SSIDataPut(SSI0_BASE, 0x03);// set configuration value
	while(SSIBusy(SSI0_BASE));
	ROM_SSIDataPut(SSI0_BASE, cmd_mask>>24);
	ROM_SSIDataGet(SSI0_BASE, &dead_data[0]);//clean out SSI buffer from each clocked data byte
	while(SSIBusy(SSI0_BASE));
	ROM_SSIDataPut(SSI0_BASE, cmd_mask>>16);
	ROM_SSIDataGet(SSI0_BASE, &dead_data[1]);//clean out SSI buffer from each clocked data byte
	while(SSIBusy(SSI0_BASE));
	ROM_SSIDataPut(SSI0_BASE, cmd_mask>>8);
	ROM_SSIDataGet(SSI0_BASE, &dead_data[2]);//clean out SSI buffer from each clocked data byte
	while(SSIBusy(SSI0_BASE));
	ROM_SSIDataPut(SSI0_BASE, cmd_mask);
	ROM_SSIDataGet(SSI0_BASE, &dead_data[3]);//clean out SSI buffer from each clocked data byte
	while(SSIBusy(SSI0_BASE));
	SysCtlDelay(1);

	/*if(chan == 'a'||chan == 'A')
		ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1 , GPIO_PIN_1);//CSAA high - inactive
	else if(chan == 'b' || chan == 'B')
		ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 , GPIO_PIN_0);//CSAB high - inactive*/
	chip_select(1,chan,'d');
}//end dpot_pr_ctr

void
dPot_wp_ctr(char chan, bool state)
{
	uint32_t cmd_mask;
	uint32_t dead_data[4];
	if(state==1)
		cmd_mask = config_reg_val | 0x00800000;//wp controlled by ADC output A0, set A0 high
	else
		cmd_mask = config_reg_val & 0xFF7FFFFF;//set A0 low

	while(SSIBusy(SSI0_BASE));

	/*if(chan == 'a'||chan == 'A')
		ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1 , 0);//CSAA low - active
	else if(chan == 'b' || chan == 'B')
		ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 , 0);//CSAB low - active*/
	chip_select(0,chan,'d');

	// Delay for 1 millisecond.  Each SysCtlDelay is about 3 clocks.
	SysCtlDelay(SysCtlClockGet() / (1000 * 3));

	ROM_SSIDataPut(SSI0_BASE, 0x03);// set configuration value
	while(SSIBusy(SSI0_BASE));
	ROM_SSIDataPut(SSI0_BASE, cmd_mask>>24);
	ROM_SSIDataGet(SSI0_BASE, &dead_data[0]);//clean out SSI buffer from each clocked data byte
	while(SSIBusy(SSI0_BASE));
	ROM_SSIDataPut(SSI0_BASE, cmd_mask>>16);
	ROM_SSIDataGet(SSI0_BASE, &dead_data[1]);//clean out SSI buffer from each clocked data byte
	while(SSIBusy(SSI0_BASE));
	ROM_SSIDataPut(SSI0_BASE, cmd_mask>>8);
	ROM_SSIDataGet(SSI0_BASE, &dead_data[2]);//clean out SSI buffer from each clocked data byte
	while(SSIBusy(SSI0_BASE));
	ROM_SSIDataPut(SSI0_BASE, cmd_mask);
	ROM_SSIDataGet(SSI0_BASE, &dead_data[3]);//clean out SSI buffer from each clocked data byte
	while(SSIBusy(SSI0_BASE));

	SysCtlDelay(1);

	/*if(chan == 'a'||chan == 'A')
		ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1 , GPIO_PIN_1);//CSAA high - inactive
	else if(chan == 'b' || chan == 'B')
		ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 , GPIO_PIN_0);//CSAB high - inactive*/
	chip_select(1,chan,'d');
}//end dpot_wp_ctr

void
dPot_ctr(char chan, int command_num, uint32_t address, uint32_t data)
{
	static uint32_t dead_data[1], live_data[4];
	static int data_write=0;

	static union dpot_Data
	{
		uint32_t i_buf;
		char  c_buf[4];
	} send_data;

	while(SSIBusy(SSI0_BASE));

	if(chan == 'a'||chan == 'A')
		ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3 , 0);//CSPA low - active
	else if(chan == 'b' || chan == 'B')
		ROM_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6 , 0);//CSPB low - active
	//chip_select(0,chan,'d');

	SysCtlDelay(1);

	while(SSIBusy(SSI0_BASE));

	switch(command_num)
	{
	case 0x0: 	/* No Operation, just pushes data out of SDO and captures*/
				ROM_SSIDataPut(SSI0_BASE, (0x10|address));
				ROM_SSIDataGet(SSI0_BASE, &live_data[0]);
				while(SSIBusy(SSI0_BASE));
				ROM_SSIDataPut(SSI0_BASE, 0x00);
				ROM_SSIDataGet(SSI0_BASE, &live_data[1]);
				while(SSIBusy(SSI0_BASE));
				ROM_SSIDataPut(SSI0_BASE, 0x00);
				ROM_SSIDataGet(SSI0_BASE, &live_data[2]);
				dPot_data[0]=live_data[0]|(live_data[1]<<8)|(live_data[2]<<16);
				data_write=2;
				break;

	case 0x1: 	/* restore EEMEM(address 0 or 1) to RDAC(address 0 or 1) */
				ROM_SSIDataPut(SSI0_BASE, (0x10|address));
				break;

	case 0x2: 	/* store wiper setting of RDAC(address 0 or 1) to EEMEM(address 0 or 1) */
				ROM_SSIDataPut(SSI0_BASE, (0x20|address));
				break;

	case 0x3: 	/* store serial contents to EEMEM(Address) */
				ROM_SSIDataPut(SSI0_BASE, (0x30|address));
				data_write=1;
				break;

	case 0x4: 	/* Decrement RDAC(address) by 6dB (right shift to half value) */
				ROM_SSIDataPut(SSI0_BASE, (0x40|address));
				break;

	case 0x5: /* Decrement all RDACs by 6dB (right shift to half value) */
			ROM_SSIDataPut(SSI0_BASE, 0x50);
			break;

	case 0x6: /* Decrement RDAC(address) by 1 */
			ROM_SSIDataPut(SSI0_BASE, (0x60|address));
			break;

	case 0x7:/* Decrement all RDACs by 1 */
			ROM_SSIDataPut(SSI0_BASE, 0x70);
			break;

	case 0x8:/* Reset all RDACs with previous EEMEM value */
			ROM_SSIDataPut(SSI0_BASE, 0x80);
			break;

	case 0x9:/* Read out EEMEM to MISO on next frame */
			ROM_SSIDataPut(SSI0_BASE, (0x90|address));
			break;

	case 0x10:/* Read out RDAC(ADDRESS) to MISO on next frame */
			ROM_SSIDataPut(SSI0_BASE, (0xA0|address));
			break;

	case 0x11:/* write serial data to RDAC(address) */
			ROM_SSIDataPut(SSI0_BASE, (0xB0|address));
			data_write=1;
			break;

	case 0x12:/* increment RDAC(address) by 6dB (left shift to double value) */
			ROM_SSIDataPut(SSI0_BASE, (0xC0|address));
			break;

	case 0x13:/* increment all RDACs by 6dB (left shift to double value) */
			ROM_SSIDataPut(SSI0_BASE, 0xD0);
			break;

	case 0x14:/* increment RDAC(address) by 1 */
			ROM_SSIDataPut(SSI0_BASE, (0xE0|address));
			break;

	case 0x15: /* Increment all RDACs by 1 */
			ROM_SSIDataPut(SSI0_BASE, 0xF0);
			break;
	}

	if(data_write==0)
	{
		ROM_SSIDataGet(SSI0_BASE, &dead_data[0]);
		while(SSIBusy(SSI0_BASE));
		ROM_SSIDataPut(SSI0_BASE, 0x00);
		ROM_SSIDataGet(SSI0_BASE, &dead_data[0]);
		while(SSIBusy(SSI0_BASE));
		ROM_SSIDataPut(SSI0_BASE, 0x00);
		ROM_SSIDataGet(SSI0_BASE, &dead_data[0]);
	}

	else if (data_write == 1)
	{
		send_data.i_buf=data;
		ROM_SSIDataGet(SSI0_BASE, &dead_data[0]);
		while(SSIBusy(SSI0_BASE));
		ROM_SSIDataPut(SSI0_BASE, send_data.c_buf[1]);
		ROM_SSIDataGet(SSI0_BASE, &dead_data[0]);
		while(SSIBusy(SSI0_BASE));
		ROM_SSIDataPut(SSI0_BASE, send_data.c_buf[0]);
		ROM_SSIDataGet(SSI0_BASE, &dead_data[0]);
		data_write=0;
	}
	while(SSIBusy(SSI0_BASE));

	SysCtlDelay(1);

	if(chan == 'a'||chan == 'A')
		ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3 , GPIO_PIN_3);//CSPA high - inactive
	else if(chan == 'b' || chan == 'B')
		ROM_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6 , GPIO_PIN_6);//CSPB high - inactive
	//chip_select(1,chan,'d');
}

void detector_pCyc(int PosOrNeg)
{
	if(PosOrNeg=='p')
	{
		 ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);//SDNP_CTR High
		 wait(20);//wait 20mS
		 ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);//SDNP_CTR Low
	}

	else if(PosOrNeg=='n')
	{
		ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_PIN_2);//SDNN_CTR High
		wait(20);//wait 20mS
		ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0);//SDNN_CTR Low
	}
}//end dpot_ctr

void
chip_select(int csVal, char csChan, char csDev )
{
	/* this funtion simplifies the use of the chip selects for the SPI bus
	 csVal is the value to place on the chip select (all are active low) 1=high, 0=low
	 csChan is the board channel 'a' or 'b'
	 csDev is the device for the selected board, 'c'=ADC, 'd'=Digital pot */

	if((csChan=='a') && (csDev=='c') && (csVal==1))
		ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1 );// ADC A high

	else if((csChan=='a') && (csDev=='c') && (csVal==0))
		ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);// ADC A low

	else if((csChan=='a') && (csDev=='d') && (csVal==1))
		ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3 );// digital pot A high

	else if((csChan=='a') && (csDev=='d') && (csVal==0))
		ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0);// digital pot A low

	else if((csChan=='b') && (csDev=='c') && (csVal==1))
		ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_PIN_0 );// ADC B high

	else if((csChan=='b') && (csDev=='c') && (csVal==0))
		ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0);// ADC B low

	else if((csChan=='b') && (csDev=='d') && (csVal==1))
		ROM_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, GPIO_PIN_6 );// digital pot B high

	else if((csChan=='b') && (csDev=='d') && (csVal==0))
		ROM_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, 0);// digital pot B low
}// end chip_select




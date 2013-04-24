#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "config_extern.h"

/* ************************************
Hard coded analog channel information is all defined in these functions.
Channel configuration for Prometheus AD is defined here.
R1 and R2 are set up in hardware so that voltages from all sources are withing +/- 5V.
R1 and R2 are 3-wire configuration dividers, with R2 being the one sampled.
Func is the processing function used to convert the final AD voltage to engineering units. */
int PRM_Chan_Config(struct CHAN_CONFIG chan_Conf[], BYTE nChannels)
{
	unsigned int i; // General counter
	
	for ( i=0; i < nChannels; i++ ) //Set default values for all channels to calculate voltages.
	{
		chan_Conf[i].R1 = 0; //Not all channels have dividers.
		chan_Conf[i].R2 = 1;
		chan_Conf[i].Func = VoltageCalc; //Initialize all processing functions to calc voltage. We will define P, T below.
	}
	
	for ( i=0; i < 4; i++ ) //The first 4 channels are temperatures.
	{
		chan_Conf[i].R1 = 10; //R1 is resistance between thermistor and VREF.
		chan_Conf[i].R2 = 10; //R2 is ressitance between thermistor and ground (zero in case of single ended AD.
		chan_Conf[i].Func = TemperatureCalc;
	}
	chan_Conf[0].varName = "TAin";
	chan_Conf[1].varName = "TBin";
	chan_Conf[2].varName = "TAout";
	chan_Conf[3].varName = "TBout";
	chan_Conf[0].varNameLong = "Temperature of air in cell A inlet, C";
	chan_Conf[1].varNameLong = "Temperature of air in cell B inlet, C";
	chan_Conf[2].varNameLong = "Temperature of air in cell A outlet, C";
	chan_Conf[3].varNameLong = "Temperature of air in cell B outlet, C";
	
	chan_Conf[4].varNameLong = "Reference +5V for pressure transducers, V";
	chan_Conf[4].varName = "VrefP5V";
	chan_Conf[4].R1 = 0.5; //Channel 4 is the 5V reference for the pressure transducers.
	chan_Conf[4].R2 = 2;
	
	chan_Conf[5].varNameLong = "Pressure Baratron, mbar";
	chan_Conf[5].varName = "PressBrt";
	chan_Conf[5].Func = PressureBaratron; // Channel 5 is Baratron. It uses simply 300 * ADvoltage for its Func. Do it in Main above.
	//Resistances don't need to be defined for the Baratron channel, default values are fine.
	
	chan_Conf[6].varNameLong = "Pressure cell B, mbar";
	chan_Conf[6].varName = "PressB";
	chan_Conf[6].Func = PressureCalc; // Chan. 6 is Cell B absolute pressure. R1 and R2 are not used and don't matter.
	
	chan_Conf[7].varNameLong = "Current +28V, Amp";
	chan_Conf[7].varName = "Curr28V";
	chan_Conf[7].Func = CurrentCalc; // Chan. 7 is 28V current.
	chan_Conf[7].R2 = 0.2;

	return 0;
}

/* ************************************
Hard coded analog channel information is all defined in these functions.
Channel configuration for DMM 16-channel AD is defined here.
R1 and R2 are set up so that voltages from all sources are withing +/- 5V.
R1 and R2 are 3-wire configuration dividers, with R2 being the one sampled.
Func is the processing function used to convert the final AD voltage to engineering units. */
int DMM_Chan_Config(struct CHAN_CONFIG chan_Conf[], BYTE nChannels)
{
	unsigned int i; // General counter
	
	for ( i=0; i < nChannels; i++ ) //Set default values to calculate voltages.
	{
		chan_Conf[i].R1 = 0; //Not all channels have dividers.
		chan_Conf[i].R2 = 1;
		chan_Conf[i].Func = VoltageCalc; //Initialize all processing functions to calc voltage. We will define P, T below.
	}
	
	for ( i=0; i < 7; i++ ) //The first 7 channels are temperatures. They all have the same 3-wire divider configuration, R2=0.
	{
		chan_Conf[i].R1 = 20; //R1 is resistance between thermistor and VREF.
		chan_Conf[i].R2 = 0; //R2 is ressitance between thermistor and ground (zero in case of single ended AD.
		chan_Conf[i].Func = TemperatureCalc;
	}
	chan_Conf[0].varNameLong = "Temperature Lamp, C";
	chan_Conf[1].varNameLong = "Temperature Detector, C";
	chan_Conf[2].varNameLong = "Temperature Scrubber, C";
	chan_Conf[3].varNameLong = "Temperature DSM Vent, C";
	chan_Conf[4].varNameLong = "Temperature DSM PS plate, C";
	chan_Conf[5].varNameLong = "Temperature DSM T Controller, C";
	chan_Conf[6].varNameLong = "Temperature DSM PC-104, C";
	chan_Conf[0].varName = "TLamp";
	chan_Conf[1].varName = "TDetect";
	chan_Conf[2].varName = "TScrub";
	chan_Conf[3].varName = "TdsmVent";
	chan_Conf[4].varName = "TdsmPlate";
	chan_Conf[5].varName = "TdsmTcontr";
	chan_Conf[6].varName = "TdsmPC104";
	
	chan_Conf[7].varNameLong = "Pressure cell A, mbar";
	chan_Conf[7].varName = "PressA";
	chan_Conf[7].Func = PressureCalc; //Channel 7 is is Cell A Absolute Pressure. R1 and R2 are not used and don't matter.
	
	chan_Conf[8].R1 = chan_Conf[15].R1 = 10; //5V power supply, PC104 5VDC
	chan_Conf[8].R2 = chan_Conf[15].R2 = 20;
	chan_Conf[8].varNameLong = "Power supply general +5V, V";
	chan_Conf[15].varNameLong = "Power supply at PC104 +5V, V";
	chan_Conf[8].varName = "VPos5V";
	chan_Conf[15].varName = "VPosPC+5V";
	
	chan_Conf[9].R1 = chan_Conf[10].R1 = chan_Conf[11].R1 = 10; //+28, +24, -24VDC
	chan_Conf[9].R2 = chan_Conf[10].R2 = chan_Conf[11].R2 = 2;
	chan_Conf[9].varNameLong = "Power supply +28V, V";
	chan_Conf[10].varNameLong = "Power supply detector board +24V, V";
	chan_Conf[11].varNameLong = "Power supply detector board -24V, V";
	chan_Conf[9].varName = "VPos28V";
	chan_Conf[10].varName = "SPos24V";
	chan_Conf[11].varName = "VNeg24V";
	
	chan_Conf[12].R1 = chan_Conf[13].R1 = chan_Conf[14].R1 = 20; //+15, -15, +12VDC
	chan_Conf[12].R2 = chan_Conf[13].R2 = chan_Conf[14].R2 = 10;
	chan_Conf[12].varNameLong = "Power supply Baratron +15V, V";
	chan_Conf[13].varNameLong = "Power supply Baratron -15V, V";
	chan_Conf[14].varNameLong = "Power supply +12V, V";
	chan_Conf[12].varName = "VPosBar15V";
	chan_Conf[13].varName = "VNegBar15V";
	chan_Conf[14].varName = "VPos12V";

	return 0;
}

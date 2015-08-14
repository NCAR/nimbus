#include <math.h> //For pow, log
#include "config_extern.h"

/* ************************************
Convert DMM AD output to voltage, depending on DMM board settings. Replaces AD voltage data with real voltage data.
Voltages are AD readings from DMM; nVoltages is the number of channels; vRange is the full scale AD input range; isBipolar tells if
the AD is set up for bi-polar or unipolar measurement mode. */
int DMM_AD_Voltage(float Voltages[], BYTE nVoltages, float vRange, BYTE isBipolar)
{
	unsigned int i;
	for ( i=0; i < nVoltages; i++ )
	{
		if ( isBipolar )
			Voltages[i] = Voltages[i] * ( vRange / 2048 ) - vRange;
		else
			Voltages[i] = Voltages[i] * ( vRange / 4096 );
	}
	return 0;
}

/* ************************************
Convert Prometheus AD output to voltage, depending on DMM board settings. Replaces AD voltage data with real voltage data.
Voltages are AD readings from PRM; nVoltages is the number of channels; vRange is the full scale AD input range; isBipolar tells if
the AD is set up for bi-polar or unipolar measurement mode. */
int PRM_AD_Voltage(float Voltages[], BYTE nVoltages, float vRange, BYTE isBipolar)
{
	unsigned int i;
	for ( i=0; i < nVoltages; i++ )
	{
		if ( isBipolar )
			Voltages[i] = Voltages[i] * ( vRange / 32768 );
		else
			Voltages[i] = ( Voltages[i] + 32768 ) * ( vRange / 65536 );
	}
	return 0;
}

/* ************************************
Calculate Voltage based on the voltage divider configuration. */
float VoltageCalc( float inData, float R1, float R2, float vRange )
{
	return inData * ( R1 + R2 ) / R2;
}

/* ************************************
Calculate Pressure from AD voltage using TrueStability transfer function. */
float PressureCalc( float inData, float R1, float R2, float vRange )
{
	return 517.1059 * inData - 258.5529;
}

/* ************************************
Calculate Temperature from AD voltage using the Steinhart-Hart equation for the Unicurve type 16 thermocouple.
Return the temperature in degrees C. inData is the AD voltage, R1 and R2 are Vref and Ground plane resistances in the 4-wire configuration,
vRange is the Vref voltage (5V in our case, typically). */
float TemperatureCalc( float inData, float R1, float R2, float vRange )
{
	inData = inData * (R1 + R2)/(vRange - inData) * 1000; //Calculate thermistor resistance in Ohms.
	if ( inData < 0 ) return -90.0;
	return (1 / (0.001126 + 0.0002346 * log(inData) + 0.0000000861 * ( pow( log(inData), 3 )))) - 273.16;
}

/* ************************************
Calculate Current based on the voltage divider configuration. */
float CurrentCalc( float inData, float R1, float R2, float vRange )
{
	return inData / R2 * 1000;
}

/* ************************************
Calculate Temperature from AD voltage using the Steinhart-Hart equation for the Unicurve type 16 thermocouple.
Return the temperature in degrees C. inData is the AD voltage, R1 and R2 are Vref and Ground plane resistances in the 4-wire configuration,
vRange is the Vref voltage (5V in our case, typically). */
float PressureBaratron( float inData, float R1, float R2, float vRange )
{
	return inData * 300;
}
/* ************************************
Calculate Flow using the 5000 series Honeywell flow meter transfer function. The meter is supposed to return 1V at 0 slpm and 5V at 5 slpm. */
float FlowCalc( float inData, float R1, float R2, float vRange )
{
	return 1.25 * inData - 1.25;
}

//Define widely used types.
typedef unsigned short int WORD;
typedef unsigned char BYTE;

//AD channel configuration data type.
struct CHAN_CONFIG {
	float (*Func)(float, float, float, float);
	float R1;
	float R2;
	};

//DMM board functions.
extern int DMM_AD_Voltage(float Voltages[], BYTE nVoltages, float vRange, BYTE isBipolar);
extern int PRM_AD_Voltage(float Voltages[], BYTE nVoltages, float vRange, BYTE isBipolar);
extern int DMM_AD_Scan(WORD base, BYTE ChannelLow, BYTE ChannelHigh, WORD nAverages, WORD nDelay, float Result[]);
extern int DMM_Chan_Config(struct CHAN_CONFIG chan_conf[], BYTE nChannels);

//AD conversion functions.
extern float VoltageCalc( float inData, float R1, float R2, float vRange );
extern float PressureCalc( float inData, float R1, float R2, float vRange );
extern float TemperatureCalc( float inData, float R1, float R2, float vRange );
extern float CurrentCalc( float inData, float R1, float R2, float vRange );

//Prometheus DAQ circuit functions...
extern int PRM_AD_Scan ( WORD base, BYTE ChannelLow, BYTE ChannelHigh, WORD nAverages, WORD nDelay, float Result[]);
extern int PRM_Chan_Config(struct CHAN_CONFIG chan_Conf[], BYTE nChannels);
//...and DIO functions.
extern int DIO_Config( WORD base, BYTE config );
extern int Valve_Fwd( WORD base, BYTE valve_position );
extern int Valve_Check( BYTE valve_position, unsigned int * nSkips, unsigned int * nStops, unsigned int * prev_position );
extern BYTE Valve_GetPosition( WORD base );

//QMM counter board functions.
extern void QMM_SetAddresses( WORD base );
extern int QMM_Read(unsigned int data[], BYTE isOP1);
extern int QMM_Setup(WORD base, BYTE isOP1, unsigned int DOWN_COUNTS );
extern int QMM_Set_Register8(WORD ControlReg, WORD DataReg, WORD Pointer, WORD Data);
extern unsigned int QMM_Get_Register8(WORD ControlReg, WORD DataReg, WORD Pointer);
extern int QMM_Restart_Counters( BYTE isOP1 );
extern int QMM_ReadTimer();
extern BYTE QMM_CheckVersion();
extern int QMM_WaitForTC1();

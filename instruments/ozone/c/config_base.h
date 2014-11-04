//This is the general configuration file for the OP system.
//Define base addresses for all boards.
const unsigned short int BASE_QMM = 0x380; //Detector board V>F counter board.
const unsigned short int BASE_PROM_DAQ = 0x280; //Prometheus CPU board Data Acquisition subsystem base address. Used for Diff A/D and DigOut.
const unsigned short int BASE_DMM = 0x340; //MM-NA-XT analog card. Used for housekeeping parameter single ended, bi-polar A/D.
const unsigned short int BASE_IRIG = 0x300; //Time sync card. Used only in OP-1 and is probably going to be removed, replaced by NTP sync.

//Define timing parameters.
const unsigned int DOWN_COUNTS = 53333; //Loop timer start value.
const unsigned int MAIN_FREQ = 400000; //Loop timer clock frequency is F2=1/10 of source clock of 4 MHz.

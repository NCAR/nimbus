/*QMM master mode register settings for the particular chip.
QMM has onboard 4 MHz clock and can put out FOUT divided by Divider. We don't use this feature. */
const WORD QMM_MASTER_CONFIG = 0xC2B0; //TOD disable; Comparator disable; FOUT=F1; FOUT div=2; FOUT=Off; Data bus=8bit; Data ptr auto advance=Off; Scale mode=BCD.

//QMM counter settings. Descriptions as read left to wright are from LSB to MSB (inverted from actual binary values).
const WORD QMM_MODE_LOOP_CONFIG = 0xC02; //Toggle output on TC; Cnt Down; Binary; Once; Use Load reg; Special Gate Off; F2=1/10 source; Rising edge; No gating
const WORD QMM_MODE_A1_CONFIG = 0x9228; //Output Off; Count Up; Binary; Repeat; Load; Special Gate Off; Source2; Falling edge; Active High Level GateN
const WORD QMM_MODE_A2_CONFIG = 0x1028; //Output Off; Count Up; Binary; Repeat; Load; Special Gate Off; Previous counter TC; Falling edge; No gating
//Counter for channel B reads from Source 4 on OP-2, chip 1
const WORD QMM_MODE_B1_OP2_CONFIG = 0x9428; //Output Off; Count Up; Binary; Repeat; Load; Special Gate Off; Source4; Falling edge; Active High Level GateN
const WORD QMM_MODE_B2_OP2_CONFIG = 0x1028; //Output Off; Count Up; Binary; Repeat; Load; Special Gate Off; Previous counter TC; Falling edge; No gating
//Counter for channel B reads from Source 2 on OP-1, chip 2
const WORD QMM_MODE_B1_OP1_CONFIG = 0x9228; //Output Off; Count Up; Binary; Repeat; Load; Special Gate Off; Source2; Falling edge; Active High Level GateN
const WORD QMM_MODE_B2_OP1_CONFIG = 0x1028; //Output Off; Count Up; Binary; Repeat; Load; Special Gate Off; Previous counter TC; Falling edge; No gating

/*Define QMM pointers. These are supplied to the Status/Control register to point the next operation to a particular register.
There are 4 registers for each of the chips in the QMM: Control/Status, Mode, Data, Load and Hold, plus a Masermode that defines the entire chip.
QMM can have one or two chips (QMM-5 or QMM-10) and is set up with 5 counters per chip. */
const WORD QMM_REG_MODE1 = 1; //Counter 1 mode register
const WORD QMM_REG_MODE2 = 2; //Counter 2 mode register
const WORD QMM_REG_MODE3 = 3; //Counter 3 mode register
const WORD QMM_REG_MODE4 = 4; //Counter 4 mode register
const WORD QMM_REG_MODE5 = 5; //Counter 5 mode register
const WORD QMM_REG_LOAD1 = 9; //Counter 1 Load register
const WORD QMM_REG_LOAD2 = 10; //Counter 2 Load register
const WORD QMM_REG_LOAD3 = 11; //Counter 3 Load register
const WORD QMM_REG_LOAD4 = 12; //Counter 4 Load register
const WORD QMM_REG_LOAD5 = 13; //Counter 5 Load register
const WORD QMM_REG_HOLD1 = 17; //Counter 1 Hold register
const WORD QMM_REG_HOLD2 = 18; //Counter 2 Hold register
const WORD QMM_REG_HOLD3 = 19; //Counter 3 Hold register
const WORD QMM_REG_HOLD4 = 20; //Counter 4 Hold register
const WORD QMM_REG_HOLD5 = 21; //Counter 5 Hold register
const WORD QMM_REG_MASTERMODE = 23; //Mastermode register for the entire chip

//QMM master mode commands. These are written to Control/Status register to carry out an immediate command.
const WORD QMM_8BIT_MODE = 0xE7; //Enable 8 bit mode of data transfer (clear MM13)
const WORD QMM_16BIT_MODE = 0xEF; //Enable 16 bit data transfer mode (set MM13)
const WORD QMM_MASTER_RESET = 0xFF; //Reset counter
const WORD QMM_POINTER_AUTO = 0xE0; //Enable data pointer auto sequencing
const WORD QMM_POINTER_NOAUTO = 0xE8; //Disable data pointer auto sequencing

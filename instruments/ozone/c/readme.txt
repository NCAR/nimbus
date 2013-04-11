Ozone photometer PC104 computer includes 3 boards used for data acquisition (DAQ):
1. Prometheus CPU board with onboard AD and DIO functions.
2. DMM-NA-XT 16-channel AD board.
3. QMM-5 or QMM-10 counter board.

Each board has a base address, function set and a check-operation program as described below.
A makefile is provided that configures the compiler to include all relevant files when
building executables.


config_base.h - header to define board base addresses and loop timer value.

config_extern.h - header to define data types and external functions.

config_qmm.h - header to define hardware specific data for the counter board.



op_check_dmm.c - main function to check AD operation of the DMM board.

op_check_prom.c - main function to check AD operation of the Prometheus AD circuit.

op_check_qmm.c - main program to check operation of the QMM counter board.

op_check_dio.c - main program to check operation of the valve using the DIO on Prometheus.

op_check_time.c - main program to verify time stamp operation.



op_func_dmm.c - DMM board control and DAQ functions for housekeeping data AD.

op_func_dio.c - Prometheus DIO functions to control valve motor motion.

op_func_qmm.c - QMM counter board functions for Channel A and B counts reading.

op_func_ad.c - AD functions to convert voltages to P, T etc. Used by both DMM and Prometheus AD converters.

op_func_time.c - functions to generate a time and date stamps for data recording.


op_main.c - main instrument data acquisition program. This is the one that runs in flight.

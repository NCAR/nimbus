all:
	g++ -lm -Wall -O2 flow_test.c -o flow_test
	
	g++ -lm -Wall -O2 op_check_dio.c op_func_dio.c -o op_check_dio
	g++ -lm -Wall -O2 op_check_qmm.c op_func_qmm.c -o op_check_qmm
	g++ -lm -Wall -O2 op_check_dmm.c config_ad.c op_func_dmm.c op_func_ad.c -o op_check_dmm
	g++ -lm -Wall -O2 op_check_prom.c config_ad.c op_func_adprom.c op_func_ad.c -o op_check_prom
	g++ -lm -Wall -O2 op_check_time.c op_func_time.c -o op_check_time
	g++ -lm -Wall -O2 op_main.c config_ad.c op_func_adprom.c op_func_dmm.c op_func_qmm.c op_func_ad.c op_func_dio.c op_func_time.c -o op_main
	g++ -lm -Wall -O2 serial_main4.c config_ad.c op_func_adprom.c op_func_dmm.c op_func_qmm.c op_func_ad.c op_func_dio.c op_func_time.c UdpSocket.cc op_func_dac.c rs232.c -o serial_main4
	g++ -lm -Wall -O2 flow_control_main.c config_ad.c op_func_adprom.c op_func_dmm.c op_func_qmm.c op_func_ad.c op_func_dio.c op_func_time.c UdpSocket.cc op_func_dac.c rs232.c -o flow_main



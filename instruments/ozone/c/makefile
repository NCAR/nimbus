all:
	gcc -lm -Wall -O2 op_check_dio.c op_func_dio.c -o op_check_dio
	gcc -lm -Wall -O2 op_check_qmm.c op_func_qmm.c -o op_check_qmm
	gcc -lm -Wall -O2 op_check_dmm.c op_func_dmm.c op_func_ad.c -o op_check_dmm
	gcc -lm -Wall -O2 op_check_prom.c op_func_adprom.c op_func_ad.c -o op_check_prom
	gcc -lm -Wall -O2 op_main.c op_func_adprom.c op_func_dmm.c op_func_qmm.c op_func_ad.c op_func_dio.c -o op_main

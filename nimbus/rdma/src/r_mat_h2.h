const int num_bins = 230;const int lnDp_seg = 200;const int file_length = 120;const int flight_segments = 25;                         const int num_scans = 3000;const int max_voltage_levels = 20;void getinfo(int &flight_num, int &beg_scan_num, int &end_scan_num,	int &full_inv, int &matrices, float &T_max_input, char original_file[file_length],   float v_low_input[max_voltage_levels], float v_high_input[max_voltage_levels],   int &mat_num, int &opc, char opc_1_file[file_length], int &opc_1,   char opc_1_ch_dia_file[file_length], float &opc_1_input_factor,   char opc_2_file[file_length], int &opc_2,   char opc_2_ch_dia_file[file_length], float &opc_2_input_factor,   long unsigned int start_time[flight_segments],   long unsigned int end_time[flight_segments], int &num_volt_levels,   long int &time_offset);void save_scan_files(char original_file[file_length], int flight_num, int beg_scan_num,	int end_scan_num, int &last_scan);void get_ambient_properties(int beg_scan_num, int end_scan_num, int flight_num,	long unsigned int time_offset);void get_lat_long(int beg_scan_num, int end_scan_num, int flight_num,	long unsigned int time_offset);void get_uw_neph_rh(int beg_scan_num, int end_scan_num, int flight_num,	long unsigned int time_offset);
void get_opc_averages(int beg_scan_num, int end_scan_num, int flight_num,	char opc_file[file_length], int opc, int full_inv,   char opc_ch_dia_file[file_length], long unsigned int time_offset,   float opc_input_factor);void calc_mobility_bounds(float &Zp_low_v_max, float &Zp_high_v_min,	float T_max, float v_high, float v_low, int matrices, int mat_num,   int full_inv);void get_effective_voltages2(float Zp_ratio_high, float Zp_ratio_low,
   float v_combined_up[num_bins], float v_combined_down[num_bins],
   float &t_total_up, float &t_total_down, float t_up[2],
   float t_down[2], float v_low, float v_high, float v_effective_up[num_bins],
   float v_effective_down[num_bins], float &t_delay);
   /*
void get_effective_voltages(float Zp_high_v_min, float Zp_low_v_max,
	float v_combined_up[num_bins], float v_combined_down[num_bins],
   float &t_total_up, float &t_total_down, float t_up[2],   float t_down[2], float v_low, float v_high, float v_effective_up[num_bins],
   float v_effective_down[num_bins], float Q_combined_up[num_bins],
   float Q_combined_down[num_bins], float Q_combined_time_step_up[num_bins],
   float Q_combined_time_step_down[num_bins], float Qsh_effective_up[num_bins],
   float Qsh_effective_down[num_bins], int &effective_bins_up, int &effective_bins_down,
   float Qsh_effective_time_step_up[num_bins], float Qsh_effective_time_step_down[num_bins],
   float t_delay[2], float Q_V_effective_up[num_bins], float Q_V_effective_down[num_bins]);
                                 */
void calc_mobility_limits(float v_combined_up[num_bins],
	float v_combined_down[num_bins],
   float T_max, float Zp_upper_limit[2][num_bins],
   float Zp_lower_limit[2][num_bins], float t_total_up, float t_total_down);
float calc_res_time(float temperature);
void check_instrument(int beg_scan_num, int end_scan_num, int flight_num,
    float T_extreme[2], float P_extreme[2],
    long unsigned int gmt_secs[num_scans], int full_inv,
    float v_low_input[max_voltage_levels], float v_high_input[max_voltage_levels],
    int num_volt_levels, int scan_volt_level[num_scans],
    int &final_num_volt_levels, float volt_low[max_voltage_levels],
    float volt_high[max_voltage_levels], long unsigned int time_offset,
    float scan_temp[num_scans], int mat_num);

void find_diameter_range(float T_extreme[2], float P_extreme[2],	float volt_high[max_voltage_levels], float volt_low[max_voltage_levels],   float Dp_seg[lnDp_seg], int flight_num, char opc_1_ch_dia_file[file_length],   char opc_2_ch_dia_file[file_length],int opc_1, int opc_2,   int num_voltage_levels, int beg_scan_num, int end_scan_num);void correct_for_cnc(int beg_scan_num, int end_scan_num, int flight_num);void solve_for_n_approx(int flight_num, int scan_num,	float v_effective_up[num_bins], float v_effective_down[num_bins],
   int &bins_final, float n_final[num_bins], float dia_final[num_bins]);
void calc_inversion_matrices(int flight_num, int scan_num, float Zp_high_v_min,	float Zp_low_v_max, float v_low,	float v_high, int mat_num, int volt_level, char single_matrix, float T_K_in,
   float P_atm_in, int scan_up_in, float rh_charge, float rh_dma);
void get_inversion_matrix(int flight_num, int scan_num, int mat_num, 	float alpha[num_bins][num_bins], float t_up_total, float t_down_total,
   float volt_avg_up[num_bins], float volt_avg_down[num_bins],
   int &bins, float dia_from_inv[3][num_bins], int volt_level, float Zp_high_v_min,
   float Zp_low_v_max, float v_low, float v_high);     
void get_opc_adjustment_factor(int flight_num, float t_total_up,
   float t_total_down, float t_up[2], float t_down[2],
   char opc_2_ch_dia_file[file_length], int scan, 
   float &opc_adj, float alpha[num_bins][num_bins],
   float dia_from_inv[3][num_bins], int bins );

void get_final_counts_vector(float t_up[2], float t_down[2], float t_total_up,	float t_total_down, float dia_from_inv[3][num_bins], int scan_num, int flight_num,   float counts_vector[num_bins], int &extr_low_bins, int &extr_high_bins,   char opc_2_ch_dia_file[file_length], float opc_adj, int first_time_final_counts);void solve_for_n(int flight_num, int scan_num, float dia_from_inv[3][num_bins],	float alpha[num_bins][num_bins], float counts_vector[num_bins], int bins,
   int extr_low_bins, int extr_high_bins, int &bins_final,   float n_final[num_bins], float dia_final[num_bins], float &dia_max);void save_distribution(int scan_num, int flight_num, float n_final[num_bins],	float dia_final[num_bins], float Dp_seg[lnDp_seg], int bins_final,
   int beg_scan_num, int end_scan_num, char opc_1_ch_dia_file[file_length],
   char opc_2_ch_dia_file[file_length]);
void find_avg_conc(int flight_num, unsigned long int start_time[flight_segments],	unsigned long int end_time[flight_segments],
   unsigned long int gmt_secs[num_scans], int beg_scan_num, int end_scan_num);
char original_file[file_length], rough, opc_1_file[file_length],	opc_2_file[file_length], opc_1_ch_dia_file[file_length],   opc_2_ch_dia_file[file_length];int beg_scan_num, end_scan_num, flight_num, scan_1_up, full_inv, last_scan,   fl, sc, i, scan_num, bins, scan, extr_low_bins, extr_high_bins,   matrices, mat_num, opc, bins_final, num_volt_levels, volt_level,   scan_volt_level[num_scans], final_num_volt_levels, opc_1, opc_2,   effective_bins_up, effective_bins_down;long unsigned int start_time[flight_segments], end_time[flight_segments],	gmt_secs[num_scans];long int time_offset;float res_time, t_up[2], t_down[2], t_total_up, t_total_down, v_high, v_low,
	v_expected_up[num_bins], v_expected_down[num_bins], tau, T_max,
   dia_from_inv[3][num_bins], v_effective_up[num_bins], v_effective_down[num_bins],
   v_combined_up[num_bins], v_combined_down[num_bins],
   alpha[num_bins][num_bins], counts_vector[num_bins], Zp_high_v_min,
   Zp_low_v_max, Zp_lower_limit[2][num_bins], Zp_upper_limit[2][num_bins],
   T_extreme[2], P_extreme[2], Dp_seg[lnDp_seg], T_max_input, Zp_low_v_min,
   v_low_input[max_voltage_levels], v_high_input[max_voltage_levels],
   opc_adj, dia_final[num_bins], n_final[num_bins], dia_max,
   avg_opc_adj, volt_low[max_voltage_levels], volt_high[max_voltage_levels],
   RH_extreme[2], opc_1_input_factor, opc_2_input_factor, opc_input_factor,
   scan_temp[num_scans], Q_combined_up[num_bins], Q_combined_down[num_bins],
   Q_combined_time_step_up[num_bins], Q_combined_time_step_down[num_bins],
   t_delay, x;

float Qsh_effective_up[num_bins], Qsh_effective_down[num_bins],
	Qsh_effective_time_step_up[num_bins], Qsh_effective_time_step_down[num_bins],
   Q_V_effective_up[num_bins], Q_V_effective_down[num_bins];

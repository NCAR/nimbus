/*
-------------------------------------------------------------------------
OBJECT NAME:	rdma_mat.cc

FULL NAME:	RDMA Matrix

ENTRY POINTS:	main() or rdma_mat()

STATIC FNS:	many, program is self-contained.

DESCRIPTION:	Two programs are run to re-compute RDMA histograms.  This
		is the first.  See rdma_inv.cc for the second.  This
		program is forked from nimbus whenever a histogram shows
		up (every 3 minutes?).

		This program produces new cal coes based on pressure and
		temperature.  rdma_inv will read these new cals and produce
		a new concentration array.

INPUT:		

OUTPUT:		

REFERENCES:	none

REFERENCED BY:	none or rdma processor (rdmap).

NOTES:		This program can operate standalone by making sure the
		entry point is called main(), or compiled into other
		programs (e.g. rdmap) by changing entry point to rdma_mat.
		See function at bottom of file.

COPYRIGHT:	University Corporation for Atmospheric Research, 2002
-------------------------------------------------------------------------
*/

#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>
#include "nrutil.h"

static const double R1_RDMA = 0.24;		// cm
static const double R2_RDMA = 5.04;		// cm
static const double B_RDMA = 1.0;		// cm
static const double L_TSI_DMA = 44.44;		// cm
static const double R1_TSI_DMA = 0.937;		// cm
static const double R2_TSI_DMA = 1.958;		// cm
static const double L_CIT_CDMA = 41.19;		// cm
static const double R1_CIT_CDMA = 0.945;	// cm
static const double R2_CIT_CDMA = 1.92;		// cm
static const double L_nano_DMA = 5.0;
static const double R1_nano_DMA = 0.94;
static const double R2_nano_DMA = 1.91;
static const double L_eff_TSI_DMA = 1.3;	// m             
static const double pi = M_PI;
static const double G_tilda_rdma = 2.92;
static const double G_TSI_DMA = 4.25;
static const double G_nano_DMA = 4.25;
static const double k = 1.381e-23;		// J/K
static const double e = 1.602e-19;		// C
static const double ratio_of_pos_to_neg_ion_conc = 1.0;
static const double ratio_of_pos_to_neg_ion_mob = 0.875;
static const double ep0 = 8.854e-12;		// C^2/(N-m^2)
static const double mfp_1_atm_298_K = 65.1e-9;	// m
static const double Dp_min = 1.0e-9;		// m, Used for dia calc from Zp
static const double Dp_max = 2.0e-5;		// m                    
static const int segments = 9;			// For averaging over dia bin
static const double alpha_min = 1.0e-9;   // limit for inversion factor
static const double omega_limit = 1.0e-5; // limit for transfer function
static const int num_pts_fit_low_Dp = 8;  // num pts used for extrapolation
static const int num_pts_fit_high_Dp = 4;
static const int num_bins = 200;	// num_bins must be >= actual number
static const int num_extr_bins = 20; // num_extr_bins >= extrapolated pts + pts used for extrapolation
static const double inv_factor_ratio = 1.0e5; // largest allowed ratio of two different inv(Dpi, countsj)
static const int file_length = 100;	// number of characters in a file name
static const int num_param = 17;   // num_param >= number of parameters stored
static const int max_chg = 10;		// highest # charges considered
static const double R = 8.314;		// J/mol-K
static const double mobility_ratio_increment = 1.01;
static const double unreasonable_number = -1.0e10;
static const int max_lines = 25;
static const double mw_air = 29.0;
static const double mw_positive = 290;
static const double mw_negative = 140;
static const double Z_positive = 1.15e-4;
static const double Z_negative = 1.425e-4;
static const double max_charge_error = 4.0e-4;
static const double kQ = 8.988e9;
static const double very_large_number = 1.0e25;
static const double very_small_number = 1.0e-25;

static char	base_directory[1024];
static char	tubing_lengths_file[file_length], first_time_tubing_file;

static double	var[num_param];

static void gcf(double *gammcf, double a, double x, double *gln);
static void gser(double *gamser, double a, double x, double *gln);
static double gammln(double xx);
void	nrerror(char error_text[]);

using namespace std;

/* -------------------------------------------------------------------- */
static void getinfo(int &mat_num)
{
  double value[num_param];
  int	i, j;
  char	log_file[file_length], log_save, mat_param[file_length],
  	tubing_lengths_file_name[file_length], 
	multiple_lengths, mat_dir_char, mat_dir_char_str[2],
	mat_dir[file_length], parameter_file[file_length];


  strcpy(base_directory, getenv("DATA_DIR"));
  strcat(base_directory, "/rdma/dma/inv/");

  first_time_tubing_file = 'y';
  log_save = 'n';
  mat_num = 1;

  /*  Each set of matrices has an I.D. # associated with it.  */   

  strcpy(mat_dir, base_directory);
  strcat(mat_dir, "input/mat_");
  mat_dir_char = mat_num + '0';
  mat_dir_char_str[0] = mat_dir_char;
  mat_dir_char_str[1] = '\0';
  strcat(mat_dir, mat_dir_char_str);
  strcpy(log_file, mat_dir);
  strcat(log_file, "/log.txt");   

  ofstream log_out(log_file, ios::trunc);

//  cout<<"\nAccount for different tubing lengths with different flows (y or n) ? : ";
//  cin>>multiple_lengths;
  multiple_lengths = 'n';

  /*  If aerosol passes through different tubes at different flow rates,
   * this can be taken into account.  If so, the user simply creates a file
   * giving tubing lengths and flow rates as follows :

               length 1 (meters)  flow rate 1  (L/min)
               length 2           flow rate 2
               .
               .
               .

   * The file should be placed in the misc folder in the input directory.
   */
  if ((multiple_lengths == 'y') || (multiple_lengths == 'Y'))
    {
    cout<<"\nEnter the file name listing the tubing lengths and flows : ";
    cin>>tubing_lengths_file_name;
    strcpy(tubing_lengths_file, base_directory);
    strcat(tubing_lengths_file, "input/misc/");
    strcat(tubing_lengths_file, tubing_lengths_file_name);
//    get_single_length = 0;

    log_out<<endl<<"Tubing losses calculated using lengths and flowrates listed in "
	<<tubing_lengths_file_name;
    }
  else  {
    strcpy(tubing_lengths_file, "none");
//    get_single_length = 1;

    log_out<<endl<<"Tubing losses calculated using single effective length";
    }

  log_out<<endl<<endl;

  strcpy(parameter_file, base_directory);
  strcat(parameter_file, "stored/param.txt");
  ifstream param(parameter_file);

  for (i = 0 ; i < num_param; ++i)
    {
    param >> value[i];  // all values stored as double numbers
    var[i] = value[i];
    }

  param.close();

  i = 0;
  log_out << "\n("<<i<<setw(40)<<
            	") Scan time (sec) =                     "<<value[i]; ++i;
  log_out << "\n("<<i<<setw(40)<<
            	") Counting time (sec) =                 "<<value[i]; ++i;
  log_out << "\n("<<i<<setw(40)<<
            	") Aerosol flow rate (L/min) =           "<<value[i]; ++i;
  log_out << "\n("<<i<<setw(40)<<
            	") Monodisperse flow rate (L/min) =      "<<value[i]; ++i;
  log_out << "\n("<<i<<setw(40)<<
            	") Sheath flow rate (L/min) =            "<<value[i]; ++i;
  log_out << "\n("<<i<<setw(40)<<
            	") Excess flow rate (L/min) =            "<<value[i]; ++i;
  log_out << "\n("<<i<<setw(40)<<
            	") CNC flow rate (L/min) =               "<<value[i]; ++i;
  log_out << "\n("<<i<<setw(40)<<
            	") Maximum voltage =                     "<<value[i]; ++i;
  log_out << "\n("<<i<<setw(40)<<
            	") Minimum voltage =                     "<<value[i]; ++i;
  log_out << "\n("<<i<<setw(40)<<
       		") DMA used :                            "<<value[i]; ++i;
  log_out << "\n        1 = Caltech RDMA";
  log_out << "\n        2 = Caltech Cylindrical DMA";
  log_out << "\n        3 = TSI Cylindrical DMA";
  log_out << "\n        4 = TSI nano DMA";
  log_out << "\n("<<i<<setw(40)<<
		") Detector used :                       "<<value[i]; ++i;
  log_out << "\n        1 = Ultrafine modified TSI 3010";
  log_out << "\n        2 = TSI 3760";
  log_out << "\n        3 = Electrometer";

  log_out << "\n("<<i<<setw(40)<<
            	") CNC saturator temperature (C)         "<<value[i]; ++i;
  log_out << "\n("<<i<<setw(40)<<
            	") CNC condenser temperature (C)         "<<value[i]; ++i;
  log_out << "\n("<<i<<setw(40)<<
            	") Length of tubing for aerosol flow (m)  "<<value[i]; ++i;
  log_out << "\n("<<i<<setw(40)<<
            	") Greatest # of charges considered      "<<value[i]; ++i;
  log_out << "\n("<<i<<setw(40)<<
            	") Temperature (K)                       "<<value[i]; ++i;
  log_out << "\n("<<i<<setw(40)<<
            	") Pressure (atm)                        "<<value[i]; ++i;


  sprintf(mat_param, "%sstored/m_par_%s.txt", base_directory, mat_dir_char_str);
  ofstream mat_param_out(mat_param, ios::trunc);

  for (j = 0; j <= i; ++j)
    mat_param_out << value[j] << endl;

  mat_param_out.close();
  
/*
  cout<<"\nEnter comments for this run ('*' to stop) : "<<endl<<endl;
  log_out<<endl<<endl<<"Comments : "<<endl<<endl;

  char_in_line = 0;

  while (1)
    {
    cin>>comments;
    if (comments[0] == '*') break;

    for(letter = 0; letter < 50; ++letter)
      if(comments[letter] == '\0') break;

    char_in_line += letter;

    if(char_in_line > 55) {
      char_in_line = 0;
      log_out<<endl;
      cout<<endl;
      }
    else
      log_out<<comments<<" ";
    }
*/

  log_out<<endl<<endl;

  log_out.close();
   
}

/* -------------------------------------------------------------------- */
static double get_diameter(double Zp, double pressure, double T_K)
{
  double Dp_high, Dp_low, Dp_Cc, Dp_mid, Cc, mfp, viscosity;

  viscosity = (4.64e-8)*T_K + 4.52e-6;

  Dp_high = Dp_max;
  Dp_low = Dp_min;
  Dp_Cc = e/(3.0*pi*viscosity*Zp);

  mfp = mfp_1_atm_298_K*(T_K/298.0)/(pressure/1.0);

  while (1) {
    Dp_mid = (Dp_high + Dp_low)/2.0;
    Cc = 1.0 + (2.0*mfp/Dp_mid)*(1.257 + 0.4*exp(-1.1*Dp_mid/(2.0*mfp)));

    if ((fabs(((Dp_mid/Cc)-Dp_Cc)/Dp_Cc)) < 0.00001)
      break;
    else
    if ((Dp_mid/Cc)>Dp_Cc)
      Dp_high = Dp_mid;
    else
      Dp_low = Dp_mid;
  }

  return(Dp_mid);

}

/* -------------------------------------------------------------------- */
static double get_omega(double Zp, double Zp_star, int chg, double Zp_ratio_low, 
	double Zp_ratio_high)
{
  double erff(double x);
  double beta, Qa, Qm, Qsh, Qe, delta, Zp_tilda, Pe_rdma, sigma_tilda = 0.0,
	err1, err2, err3, err4, err3_4, x1, x2, x3, x4, omega, T_K;
  int	dma_type;


  dma_type = int(var[9]);
  Qa = var[2];
  Qm = var[3];
  Qsh = var[4];
  Qe = var[5];
  T_K = var[15];

  beta = (Qa+Qm)/(Qsh+Qe);
  delta = (Qm-Qa)/(Qm+Qa);

  Zp_tilda = Zp/Zp_star;

  Pe_rdma = (B_RDMA/100.0)*((Qsh+Qe)/60000.0)*double(chg)*e/(2.0*pi*
	((R2_RDMA*R2_RDMA - R1_RDMA*R1_RDMA)/10000.0)*k*T_K*Zp_star); //no units

  switch (dma_type)
    {
    case 1 :
      sigma_tilda = sqrt(G_tilda_rdma/Pe_rdma+(0.0014*Qsh+0.0298)*
		(0.0014*Qsh+0.0298));
      break;

    case 2 :
      sigma_tilda = sqrt(G_TSI_DMA*4.0*pi*(L_CIT_CDMA/100.0)*k*T_K*Zp_star/
		(((Qsh+Qe)/60000.0)*e*double(chg)));
      break;

    case 3 :
      sigma_tilda = sqrt(G_TSI_DMA*4.0*pi*(L_TSI_DMA/100.0)*k*T_K*Zp_star/
		(((Qsh+Qe)/60000.0)*e*double(chg)));
      break;

    case 4 :
      sigma_tilda = sqrt(G_nano_DMA*4.0*pi*(L_nano_DMA/100.0)*k*T_K*Zp_star/
		(((Qsh+Qe)/60000.0)*e*double(chg)));
      break;

    }

  if ((Zp_tilda > Zp_ratio_high) || (Zp_tilda < Zp_ratio_low))
    omega = 0.0;
  else
    {
    x1 = (Zp_tilda-(1.0+beta))/((sqrt(2.0))*sigma_tilda);
    x2 = (Zp_tilda-(1.0-beta))/((sqrt(2.0))*sigma_tilda);
    x3 = (Zp_tilda-(1.0+beta*delta))/((sqrt(2.0))*sigma_tilda);
    x4 = (Zp_tilda-(1.0-beta*delta))/((sqrt(2.0))*sigma_tilda);

    err1 = x1*erff(x1)+(exp(-x1*x1))/(sqrt(pi));
    err2 = x2*erff(x2)+(exp(-x2*x2))/(sqrt(pi));
    err3 = x3*erff(x3)+(exp(-x3*x3))/(sqrt(pi));

    if (delta != 0)  {
      err4 = x4*erff(x4)+(exp(-x4*x4))/(sqrt(pi));
      err3_4 = err3+err4;
      }
    else
      err3_4 = 2.0*err3;

    omega = (sigma_tilda/((sqrt(2.0))*beta*(1.0-delta)))*(err1+err2-err3_4);
    }

  return(omega);
}

/* -------------------------------------------------------------------- */
static double calc_res_time()
{
  double res_time = 0.0, Qa, Qm, Qsh, Qe;
  int dma_type, i;

  Qa = var[2];
  Qm = var[3];
  Qsh = var[4];
  Qe = var[5];
  dma_type = int(var[9]);

  if ((Qa+Qsh) != (Qm+Qe)) {
    cout<<"Flow rates do not match";
    cin>>i;
    exit(8);
    }

  switch (dma_type)   {
    case 1 :
      res_time = B_RDMA*pi*(R2_RDMA*R2_RDMA-R1_RDMA*R1_RDMA)*
		(60.0/1000.0)/(Qa+Qsh);
      break;
 
    case 2 :
      res_time = pi*L_CIT_CDMA*(R2_CIT_CDMA*R2_CIT_CDMA -
		R1_CIT_CDMA*R1_CIT_CDMA)*(60.0/1000.0)/(Qa+Qsh);
      break;
 
    case 3 :
      res_time = pi*L_TSI_DMA*(R2_TSI_DMA*R2_TSI_DMA -
		R1_TSI_DMA*R1_TSI_DMA)*(60.0/1000.0)/(Qa+Qsh);
      break;

    case 4 :
      res_time = pi*L_nano_DMA*(R2_nano_DMA*R2_nano_DMA -
		R1_nano_DMA*R1_nano_DMA)*(60.0/1000.0)/(Qa+Qsh);
      break;
    }

  return(res_time);

}

/* -------------------------------------------------------------------- */
static void get_mobility_ratio(double &Zp_ratio_high, double &Zp_ratio_low, double T_K,
	double Zp_star, double Qsh_Lpm)
{

  double Zp, omega, ratio_low, ratio_high;

  ratio_high = 1.0e10;
  ratio_low = 1.0e-10;

  Zp = Zp_star;

  while (1)
    {
    Zp *= mobility_ratio_increment;
    omega = get_omega(Zp, Zp_star, 1, ratio_low, ratio_high);

    if (omega < omega_limit) {
      Zp_ratio_high = Zp / Zp_star;
      break;
      }
    }

  Zp = Zp_star;

  while (1)
    {
    Zp /= mobility_ratio_increment;
    omega = get_omega(Zp, Zp_star, 1, ratio_low, ratio_high);

    if (omega < omega_limit)  {
      Zp_ratio_low = Zp / Zp_star;
      break;
      }
    }
}

/* -------------------------------------------------------------------- */
static void calc_mobility_bounds(double &Zp_low_v_max, double &Zp_high_v_min, int mat_num)
{
  int		dma_type;
  double	Zp_high_v = 0.0, Zp_low_v = 0.0, Zp_high_ratio, Zp_low_ratio,
		volt_high, volt_low, Qsh, T_K;
	
  Qsh = var[4];
  volt_high = var[7];
  volt_low = var[8];
  dma_type = int(var[9]);
  T_K = var[15];
    
  switch(dma_type)  
  {
    case 1 :
	Zp_high_v = 0.5*((2.0*Qsh)/60000.0)*(B_RDMA/100.0)/
		(2.0*pi*volt_high*((R2_RDMA*R2_RDMA - R1_RDMA*R1_RDMA)/10000.0));
	Zp_low_v = 2.0*((2.0*Qsh)/60000.0)*(B_RDMA/100.0)/
		(2.0*pi*volt_low*((R2_RDMA*R2_RDMA - R1_RDMA*R1_RDMA)/10000.0));
	break;

    case 2 :
      	Zp_high_v = ((2.0*Qsh)/60000.0)*(log(R2_CIT_CDMA/R1_CIT_CDMA))/
		(4.0*pi*(L_CIT_CDMA/100.0)*volt_high);
      	Zp_low_v = ((2.0*Qsh)/60000.0)*(log(R2_CIT_CDMA/R1_CIT_CDMA))/
		(4.0*pi*(L_CIT_CDMA/100.0)*volt_low);
	break;

    case 3 :
      	Zp_high_v = ((2.0*Qsh)/60000.0)*(log(R2_TSI_DMA/R1_TSI_DMA))/
		(4.0*pi*(L_TSI_DMA/100.0)*volt_high);
      	Zp_low_v = ((2.0*Qsh)/60000.0)*(log(R2_TSI_DMA/R1_TSI_DMA))/
		(4.0*pi*(L_TSI_DMA/100.0)*volt_low);
	break;

    case 4 :
      	Zp_high_v = ((2.0*Qsh)/60000.0)*(log(R2_nano_DMA/R1_nano_DMA))/
		(4.0*pi*(L_nano_DMA/100.0)*volt_high);
      	Zp_low_v = ((2.0*Qsh)/60000.0)*(log(R2_nano_DMA/R1_nano_DMA))/
		(4.0*pi*(L_nano_DMA/100.0)*volt_low);
	break;
  }

  get_mobility_ratio(Zp_high_ratio, Zp_low_v_max, T_K, Zp_high_v, Qsh);
  get_mobility_ratio(Zp_high_v_min, Zp_low_ratio, T_K, Zp_low_v, Qsh);

}

/* -------------------------------------------------------------------- */
static void get_effective_voltages(double Zp_ratio_high, double Zp_ratio_low, double v_combined_up[num_bins], double v_combined_down[num_bins], double &t_total_up, double &t_total_down, double t_up[2], double t_down[2], double v_effective_up[num_bins], double v_effective_down[num_bins], double &tdelay)
{
  char	last_loop;
  int	end_low_down, end_low_up, end_high_down, end_high_up, dma_type;

  double	v_low_extr_up[num_extr_bins], v_low_extr_down[num_extr_bins],
		v_high_extr_up[num_extr_bins], v_high_extr_down[num_extr_bins],
		t, tscan, tstep, t_extr, tau, vol_DMA = 0.0, Qsh, Qa, tres, v_high,
		v_low;

  tscan = var[0];
  tstep = var[1];
  Qa = var[2];
  Qsh = var[4];
  v_high = var[7];
  v_low = var[8];
  dma_type = int(var[9]);

  switch (dma_type)   {

		case 1 :

			vol_DMA = pi*B_RDMA*(R2_RDMA*R2_RDMA - R1_RDMA*R1_RDMA)/(1.0e6);
			break;

		case 2 :

			vol_DMA = pi*L_CIT_CDMA*(R2_CIT_CDMA*R2_CIT_CDMA - 
				R1_CIT_CDMA*R1_CIT_CDMA)/(1.0e6);
			break;

		case 3 :

			vol_DMA =  pi*L_TSI_DMA*(R2_TSI_DMA*R2_TSI_DMA - 
				R1_TSI_DMA*R1_TSI_DMA)/(1.0e6);

		case 4 :

			vol_DMA =  pi*L_nano_DMA*(R2_nano_DMA*R2_nano_DMA - 
				R1_nano_DMA*R1_nano_DMA)/(1.0e6);
	}

	
	tdelay = double(tstep*(int(2 + vol_DMA/(((Qsh+Qa)/60000.0))/tstep)));
	tres = calc_res_time();
   
	tau = tscan/(log(v_high/v_low));

                     // Assumes ramp starts at t = 0 ,  tscan / tstep = int
	for (t = 0; t<(tscan+tdelay); t+=tstep)   {

		if (t == 0)    {
			v_effective_up[int(t/tstep)] = v_low;
			v_effective_down[int(t/tstep)] = v_high;
		}
		else if (t <= tres)        {
			v_effective_up[int(t/tstep)] = (v_low/(tstep*tres)) * (tres*tstep -
				t*t/2.0 + (t - tstep)*(t - tstep)/2.0 +
				tau*tau*(exp(t/tau) - exp((t - tstep)/tau)) - tau*tstep);

			v_effective_down[int(t/tstep)] = (v_high/(tstep*tres)) * (tres*tstep -
				t*t/2.0 + (t - tstep)*(t - tstep)/2.0 +
				tau*tau*(exp(-t/tau) - exp((-t + tstep)/tau)) + tau*tstep);
		}
		else if ((t - tstep) < tres)  {
			v_effective_up[int(t/tstep)] = (v_low/(tstep*tres)) * (tres*tres/2.0 -
				tres*(t - tstep) + (t - tstep)*(t - tstep)/2.0 +
				tau*tau*(exp(tres/tau) - exp((t - tstep)/tau)) -
				tau*(tres + tstep - t) + tau*tau*(exp(t/tau) -
        		exp(tres/tau) - exp((t - tres)/tau) + 1.0));

			v_effective_down[int(t/tstep)] = (v_high/(tstep*tres)) * (tres*tres/2.0 -
				tres*(t - tstep) + (t - tstep)*(t - tstep)/2.0 +
				tau*tau*(exp(-tres/tau) - exp((-t + tstep)/tau)) +
				tau*(tres + tstep - t) + tau*tau*(exp(-t/tau) -
        		exp(-tres/tau) - exp((-t + tres)/tau) + 1.0));

		}
		else if (t <= tscan)    {
			v_effective_up[int(t/tstep)] = (v_low*tau*tau/(tres*tstep))*(exp(t/tau) -
        		exp((t - tstep)/tau) - exp((t - tres)/tau) + exp((t - tstep - tres)/tau));
			v_effective_down[int(t/tstep)] = (v_high*tau*tau/(tres*tstep))*(exp(-t/tau) -
        		exp((-t + tstep)/tau) - exp((-t + tres)/tau) + exp((-t + tstep + tres)/tau));
		}
		else if (t <= (tscan + tres))    {
			v_effective_up[int(t/tstep)] = (v_high/(tres*tstep)) * (t*t/2.0 -
				(t - tstep)*(t - tstep)/2.0 - tscan*tstep + tau*tstep -
				(exp(-tscan/tau))*tau*tau*(exp((t - tres)/tau) -
				exp((t - tstep - tres)/tau)));

			v_effective_down[int(t/tstep)] = (v_low/(tres*tstep)) * (t*t/2.0 -
				(t - tstep)*(t - tstep)/2.0 - tscan*tstep - tau*tstep -
				(exp(tscan/tau))*tau*tau*(exp((-t + tres)/tau) -
				exp((-t + tstep + tres)/tau)));

		}
		else if ((t - tstep) < (tscan + tres))      {
			v_effective_up[int(t/tstep)] = (v_high/tstep)*(((tscan + tres)*(tscan + tres)/2.0 
				- (t - tstep)*(t - tstep)/2.0 - tscan*(tscan + tres - t + tstep) + 
				tau*(tscan + tres - t + tstep) - tau*tau*(exp(-tscan/tau))*(exp((tscan)/tau) -
				exp((t - tstep - tres)/tau)))/tres + t - tscan - tres);

			v_effective_down[int(t/tstep)] = (v_low/tstep)*(((tscan + tres)*(tscan + tres)/2.0 -
				(t - tstep)*(t - tstep)/2.0 - tscan*(tscan + tres - t + tstep) - 
				tau*(tscan + tres - t + tstep) - tau*tau*(exp(tscan/tau))*(exp((-tscan)/tau) -
				exp((-t + tstep + tres)/tau)))/tres + t - tscan - tres);

		}
		else    {
      		v_effective_up[int(t/tstep)] = v_high;
			v_effective_down[int(t/tstep)] = v_low;

		}

	}

  
	t_extr = 0;
	end_low_up = end_low_down = end_high_up = end_high_down = 0;
	t_up[0] = t_down[0] = t_up[1] = t_down[1] = tstep;
	last_loop = 'n';

	while(1)     {

		v_low_extr_up[int(t_extr/tstep)] = v_low*exp(-(double(t_extr + tstep))/tau);
		v_high_extr_down[int(t_extr/tstep)] = v_high*exp((double(t_extr + tstep))/tau);
		v_low_extr_down[int(t_extr/tstep)] = 
			v_effective_down[int((tscan+tdelay-tstep)/tstep)]*
			exp(-(double(t_extr + tstep))/tau);
		v_high_extr_up[int(t_extr/tstep)] = 
			v_effective_up[int((tscan+tdelay-tstep)/tstep)]*
			exp((double(t_extr + tstep))/tau);

		if (v_low_extr_up[int(t_extr/tstep)] < (v_low / Zp_ratio_high))
            end_low_up = 1;
          	else t_up[0] += tstep;
		if (v_low_extr_down[int(t_extr/tstep)] < (v_low / Zp_ratio_high))
       		end_low_down = 1;
         	else t_down[1] += tstep;
		if (v_high_extr_up[int(t_extr/tstep)] > (v_high / Zp_ratio_low))
            end_high_up = 1;
           	else t_up[1] += tstep;
		if (v_high_extr_down[int(t_extr/tstep)] > (v_high / Zp_ratio_low))
            end_high_down = 1;
           	else t_down[0] += tstep;
		if(last_loop == 'y')  break;
		if((end_high_down + end_high_up + end_low_down + end_low_up) == 4)
       		last_loop = 'y';

		t_extr += tstep;
	}

	t_total_up = tscan + tdelay + t_up[0] + t_up[1];
	t_total_down = tscan + tdelay + t_down[0] + t_down[1];

	for (t = 0; t < t_total_up; t += tstep)   {
   		if(t < t_up[0])
      		v_combined_up[int(t/tstep)] = v_low_extr_up[int((t_up[0] - t - tstep)/tstep)];

		else if (t < (t_up[0] + tscan + tdelay))
      		v_combined_up[int(t/tstep)] = v_effective_up[int((t - t_up[0])/tstep)];

		else v_combined_up[int(t/tstep)] =
      		v_high_extr_up[int((t - tscan - tdelay - t_up[0])/tstep)];

	}
   
	for (t = 0; t < t_total_down; t += tstep)   {
   		if(t < t_down[0])
      		v_combined_down[int(t/tstep)] =	
				v_high_extr_down[int((t_down[0] - t - tstep)/tstep)];

		else if (t < (t_down[0] + tscan + tdelay))
      		v_combined_down[int(t/tstep)] =
         		v_effective_down[int((t - t_down[0])/tstep)];

		else v_combined_down[int(t/tstep)] =
      		v_low_extr_down[int((t - tscan - tdelay - t_down[0])/tstep)];

	}

}

/* -------------------------------------------------------------------- */
static void calc_mobility_limits(double v_combined_up[num_bins], double v_combined_down[num_bins], double T_K, double Zp_upper_limit[2][num_bins], double Zp_lower_limit[2][num_bins], double t_total_up, double t_total_down)
{
  double Zp_star = 0.0, Zp_ratio_high, Zp_ratio_low, t_total, time_step, Qsh;
  int	scan_up, bin, dma_type;
  char	ratio_file[20];

  time_step = var[1];
  Qsh = var[4];
  dma_type = int(var[9]);

  for (scan_up = 0; scan_up <= 1; ++scan_up)
    {
    if (scan_up) {
      t_total = t_total_up;
      strcpy(ratio_file, "ratio_up.txt");
      }
    else {
      t_total = t_total_down;
      strcpy(ratio_file, "ratio_dn.txt");
      }

    for (bin = 0; bin < t_total/time_step; ++bin)
      {
      if (scan_up)
        {
        switch(dma_type)
          {
          case 1 :
            Zp_star = ((2.0*Qsh)/60000.0)*(B_RDMA/100.0)/
		(2.0*pi*v_combined_up[bin]*((R2_RDMA*R2_RDMA - R1_RDMA*R1_RDMA)
		/10000.0));
            break;
            
          case 2 :

            Zp_star = ((2.0*Qsh)/60000.0)*(log(R2_CIT_CDMA/R1_CIT_CDMA))/
		(4.0*pi*(L_CIT_CDMA/100.0)*v_combined_up[bin]);
            break;

          case 3 :

            Zp_star = ((2.0*Qsh)/60000.0)*(log(R2_TSI_DMA/R1_TSI_DMA))/
		(4.0*pi*(L_TSI_DMA/100.0)*v_combined_up[bin]);
            break;

          case 4 :

            Zp_star = ((2.0*Qsh)/60000.0)*(log(R2_nano_DMA/R1_nano_DMA))/
		(4.0*pi*(L_nano_DMA/100.0)*v_combined_up[bin]);
            break;
          }
        }
      else
        {
        switch(dma_type)
          {
          case 1 :
            Zp_star = ((2.0*Qsh)/60000.0)*(B_RDMA/100.0)/
		(2.0*pi*v_combined_down[bin]*((R2_RDMA*R2_RDMA - R1_RDMA*R1_RDMA)
		/10000.0));
            break;

          case 2 :
            Zp_star = ((2.0*Qsh)/60000.0)*(log(R2_CIT_CDMA/R1_CIT_CDMA))/
		(4.0*pi*(L_CIT_CDMA/100.0)*v_combined_down[bin]);
            break;

          case 3 :
            Zp_star = ((2.0*Qsh)/60000.0)*(log(R2_TSI_DMA/R1_TSI_DMA))/
		(4.0*pi*(L_TSI_DMA/100.0)*v_combined_down[bin]);
            break;

          case 4 :
            Zp_star = ((2.0*Qsh)/60000.0)*(log(R2_nano_DMA/R1_nano_DMA))/
		(4.0*pi*(L_nano_DMA/100.0)*v_combined_down[bin]);
            break;
          }
        }

      if (scan_up)  
        get_mobility_ratio(Zp_ratio_high, Zp_ratio_low,	T_K, Zp_star, Qsh);
      else 
        get_mobility_ratio(Zp_ratio_high, Zp_ratio_low, T_K, Zp_star, Qsh);
            
      Zp_lower_limit[scan_up][bin] = Zp_ratio_low;
      Zp_upper_limit[scan_up][bin] = Zp_ratio_high;
      }
    }

}

/* -------------------------------------------------------------------- */
static void calc_rough_charge_prob(double Dp_nm, double T_K, double charge_prob[max_chg], int chgs)
{
  double power1, Dp_m, c1, c2, c3;
  int i;

  Dp_m = double(Dp_nm * 1.0e-9);

  c1 = double(e/(sqrt(4.0*pi*pi*ep0*k*T_K*Dp_m)));
  c2 = double(2.0*pi*ep0*k*T_K*Dp_m/(e*e));
  c3 = double(log(ratio_of_pos_to_neg_ion_conc*ratio_of_pos_to_neg_ion_mob));

  power1 =	-2.3484*(pow(log10(Dp_nm), 0.0)) +
		0.60440*(pow(log10(Dp_nm), 1.0)) +
		0.48000*(pow(log10(Dp_nm), 2.0)) +
		0.00130*(pow(log10(Dp_nm), 3.0)) -
		0.15440*(pow(log10(Dp_nm), 4.0)) +
		0.03200*(pow(log10(Dp_nm), 5.0));

  for(i=1; i<=chgs; ++i)
    {
    charge_prob[i] = c1*exp(-(pow((i-c2*c3), 2.0))/(2.0*c2));
    if (Dp_nm < 55.0)
      charge_prob[1] = pow(10.0, power1);
    }
}

/* -------------------------------------------------------------------- */
/*
static double Potential(double b, double T_K, double a, int p)
{
  double y, result;

  if(b == 0) return(0);
  y = a/b;
  result = kQ*( -pow(a,3)*e*e/( 2*y*y*(y*y-a*a) ) + p*e*e/y );

  return( exp(result/k/T_K) );

}
*/
/* -------------------------------------------------------------------- */
/*
static double alpha(double KE_ls, double E_ls, double a, int p, double ls)
{
  double A, B, C, x1, v1, v2, v_min, v_max, v_mid, result, re_error;

  if (KE_ls == 0.0)
    return(4.0);

  re_error = max_charge_error;

  A = 1 + E_ls/KE_ls;
  B = 0.5*pow(a, 3)*e*e/KE_ls*kQ;
  C = p*e*e/KE_ls*kQ;

  v_min = a;
  v_max = ls;

  v1 = ls*ls - a*a;

  if ((2*A*ls - C - 2.0*B*ls/v1/v1) <= 0)
    {
    if ((A + B*(a*a + 3.0*ls*ls) / pow(v1, 3)) >= 0)
      return(1.0);
    else
      {
      while ((v_max-v_min)/v_min > re_error)
        {
        v_mid=0.5*(v_min+v_max);
        v2=v_mid*v_mid-a*a;

        if (A+B*(a*a+3*v_mid*v_mid)/pow(v2,3) >= 0)
          v_min=v_mid;
        else
          v_max=v_mid;
        }
                   
      v_max=0.5*(v_min+v_max);
      v_min=a;

      v2=v_max*v_max-a*a;

      if (2*A*v_max-C-2*B*v_max/v2/v2 <= 0)
        return(1.0);
      }
    }	
  
  while ((v_max-v_min)/v_min > re_error)
    {
    v_mid=0.5*(v_min+v_max);
    v1=v_mid*v_mid-a*a;
                      
    if ( 2*A*v_mid-C-2*B*v_mid/v1/v1 >= 0)
      v_max=v_mid;
    else
      v_min=v_mid;
    }
            
  x1=0.5*(v_min+v_max);

  if ((A*x1*x1-C*x1+B/( x1*x1-a*a )) < (ls*ls))
    result = A*x1*x1-C*x1+B/( x1*x1-a*a );
  else
    result = ls*ls;

  if (result < 0)
    return(0.0);

  return( result/ls/ls );

}
*/
/* -------------------------------------------------------------------- */
/*
static double simpson(double aa, double bb,double eps, double(*f)(double, double, double, int), double T_K, double a, int p)
{

  double simp = 0.0, t1, t2, s1, ep, pp, hh, b;
  int n, k;

  n = 1;
  hh = bb - aa;
  t1 = hh * ((*f)(aa, T_K, a, p)+(*f)(bb, T_K, a, p)) / 2.0;

  s1 = t1;
  ep = eps+1.0;

  while(ep >= eps)
    {
    pp = 0.0;

    for(k = 0; k <= n-1; k++) {
      b = aa+(k+0.5)*hh;
      pp = pp+(*f)(b, T_K, a, p);
      }

    t2 = (t1+hh*pp)/2.0;
    simp = (4.0*t2-t1)/3.0;
    ep = fabs((simp-s1)/simp);
    t1 = t2;
    s1 = simp;
    n = n+n;
    hh = hh/2;
    }

  return(simp);
}
*/
/* -------------------------------------------------------------------- */
/*
static void calc_charge_prob(double Dp_nm, double T_K, double P_atm, double charge_prob[max_chg], int charges_in)
{
  double cc1, cc2, cc3, char_total, diffcoef, dc_negative, dc_positive,
	mean_velocity, velocity_positive, velocity_negative, mfp, mfp_negative,
	mfp_positive, a, charge_ratio[max_chg*2 + 1], E_ls, eta[max_chg*2 + 1],
	beta[max_chg*2 + 1], ls;

  int	j1, p, charges_out;

  charges_out = max_chg;

  for (j1 = 0; j1 <= charges_in; j1++)
    charge_prob[j1] = 0.0;

  a = Dp_nm*(1.0e-9)/2.0;

  dc_positive = k*T_K*Z_positive*sqrt(T_K/294.15)/(e*P_atm);
  dc_negative = k*T_K*Z_negative*sqrt(T_K/294.15)/(e*P_atm);

  velocity_positive = sqrt(8.0e3*R*T_K/(pi*mw_positive));
  velocity_negative = sqrt(8.0e3*R*T_K/(pi*mw_negative));

  mfp_positive = 16.0*sqrt(2.0)*dc_positive*sqrt(mw_air/(mw_air+mw_positive))/
  		(3.0*pi*velocity_positive);
  mfp_negative =16.0*sqrt(2.0)*dc_negative*sqrt(mw_air/(mw_air+mw_negative))/
  		(3.0*pi*velocity_negative);

  mfp = mfp_positive;
  diffcoef = dc_positive;
  mean_velocity = velocity_positive;

  cc1 = 4.0*pi*a*diffcoef;

  ls =	pow(a, 3.0)/mfp/mfp*(0.2*pow(1 + mfp/a, 5.0) - (1 + mfp*mfp/a/a) *
	pow(1 + mfp/a, 3.0)/3.0 + 2.0/15.0*pow(1 + mfp*mfp/a/a, 2.5));

  for (p = -charges_out; p <= (charges_out - 1); p++)
    {
    E_ls = kQ*(-pow(a, 3.0)*e*e/(2*ls*ls*(ls*ls - a*a)) + p*e*e/ls);
    cc2 = 4.0*diffcoef*a/mean_velocity/ls/ls*exp(E_ls/k/T_K);
    cc3 = simpson(0, a/ls, max_charge_error, Potential, T_K, a, p);

    if (alpha((1.5*k*T_K), E_ls, a, p, ls))
      eta[max_chg + p] = cc1/( cc2/alpha((1.5*k*T_K), E_ls, a, p, ls) + cc3 );
    else {
      charges_out = p;
      break;
      }
    }

  mfp = mfp_negative;
  diffcoef = dc_negative;
  mean_velocity = velocity_negative;

  cc1 = 4.0*pi*a*diffcoef;

  ls =	pow(a, 3.0)/mfp/mfp*(0.2*pow(1+mfp/a, 5.0)- (1 + mfp*mfp/a/a) *
	pow(1 + mfp/a, 3.0)/3.0+ 2.0/15.0*pow(1 + mfp*mfp/a/a, 2.5));

  for (p = -charges_out; p <= (charges_out - 1); p++)
    {
    E_ls = kQ*(-pow(a, 3.0)*e*e/(2.0*ls*ls*(ls*ls - a*a)) + p*e*e/ls);
    cc2 = 4.0*diffcoef*a/mean_velocity/ls/ls*exp(E_ls/k/T_K);
    cc3 = simpson(0, a/ls, max_charge_error, Potential, T_K, a, p);

    if(alpha((1.5*k*T_K), E_ls, a, p, ls))
      beta[max_chg - p] = cc1 / (cc2/alpha((1.5*k*T_K), E_ls, a, p, ls) + cc3);
    else {
      charges_out = p;
      break;
      }
    }

  charge_ratio[max_chg] = 1.0;
  charge_ratio[max_chg + 1] = eta[max_chg]/beta[max_chg + 1];
  charge_ratio[max_chg - 1] = beta[max_chg]/eta[max_chg - 1];

  for (j1 = 2; j1 <= charges_out; j1++)
    {
    charge_ratio[max_chg + j1] = charge_ratio[max_chg + j1 - 1] *
	eta[max_chg + j1 - 1]/beta[max_chg + j1];
    charge_ratio[max_chg - j1] = charge_ratio[max_chg - j1 + 1] *
	beta[max_chg - j1 + 1]/eta[max_chg - j1];
    }

  char_total=0.0;

  for (j1 = -charges_out; j1 <= charges_out; j1++)
    char_total += charge_ratio[max_chg + j1];
  
  for (j1 = 0; j1 <= charges_out; j1++) 
    charge_prob[j1] = charge_ratio[j1 + max_chg]/char_total;

}
*/
/* -------------------------------------------------------------------- */
static double get_cnc_counting_eff(double Dp_m, int charge)
{
  int		cnc_type;
  double	cnc_eff = 0.0, delta_T_cnc, a, D1_nm, D2_nm, D0_nm,
		Dp_nm, T_sat, T_cond;

  T_sat = var[11];
  T_cond = var[12];
  cnc_type = int(var[10]);

  delta_T_cnc = T_sat - T_cond;
  Dp_nm = (1.0e9)*Dp_m;

  switch (cnc_type)
    {
    case 1 :
      a = 0.0202*delta_T_cnc + 1.06;
      D1_nm = 25.3*exp(-0.066*delta_T_cnc);
      D2_nm = 100.3*pow(delta_T_cnc, -1.28);
      D0_nm = D2_nm*log(a - 1.0) + D1_nm;
	
      if((Dp_nm < D0_nm) || (Dp_nm > 200.0))
        cnc_eff = 1.0;
      else
        cnc_eff = 1.0 - a/(1.0 + exp((Dp_nm - D1_nm)/D2_nm));

      break;

    case 2 :
      cnc_eff = 1.0;
      break;

    case 3 :
      cnc_eff = charge;
      break;
    }

  return(1.0);
//  return(cnc_eff);

}

/* -------------------------------------------------------------------- */
static double get_tubing_penetration(double single_length, double Q, double D)
{
  double eff, tubing_length, flow_rate;

  eff = 1.0;

  if ((strcmp(tubing_lengths_file, "none")) == 0)  
    eff *= exp(-5.5*(pow((single_length*D/(Q/60000.0)), 0.6667)));
  else {
    if (first_time_tubing_file != 'n') {
      ofstream eof_out(tubing_lengths_file, ios::app);
      eof_out<<endl<<0;
      eof_out.close();
      first_time_tubing_file = 'n';
      }

    // Tubing length in meters and flow_rate in L/min

    ifstream values_in(tubing_lengths_file);
        
    eff = 1.0;
    while (1) {
      values_in>>tubing_length>>flow_rate;

      if (tubing_length == 0)
        break;

      eff *= exp(-5.5*(pow((tubing_length*D/(flow_rate/60000.0)), 0.6667)));
      }
    }

  return(eff);
}

/* -------------------------------------------------------------------- */
static double get_efficiences(double Dp_nm, double vis, double Cc)
{
  double Qa, Qsh, Qe, D, pen_rdma, tubing_length, Pe_rdma, Dp_m, pen_tubing,
	eff, pen_TSI_DMA, u_TSI_DMA, T_K;
  int	dma_type;

  Dp_m = Dp_nm*1.0e-9;

  Qa = var[2];
  Qsh = var[4];
  Qe = var[5];
  dma_type = int(var[9]);
  tubing_length = var[13];
  T_K = var[15];

  D = k*T_K*Cc/(3.0*pi*vis*Dp_m);     //  m^2/s if vis in N s/m^2

  u_TSI_DMA = D*L_eff_TSI_DMA/(Qa/(60.0*1000.0));  // no units if Qa in L/min 
  pen_TSI_DMA = 0.82*exp(-11.5*u_TSI_DMA) + 0.1*exp(-70.0*u_TSI_DMA) +
		0.03*exp(-180.0*u_TSI_DMA) + 0.02*exp(-340.0*u_TSI_DMA);

  Pe_rdma = B_RDMA*(Qsh+Qe)/(2.0*pi*(R2_RDMA*R2_RDMA-R1_RDMA*R1_RDMA)*D*600.0); //no units
  // From Zhang 1996
  pen_rdma = 0.98*exp(-5.6*(pow(Pe_rdma, -0.6667)));

  pen_tubing = get_tubing_penetration(tubing_length, Qa, D);

  eff = pen_tubing;

  switch (dma_type)
    {
    case 1 :
      eff *= pen_rdma;
      break;

    case 3 :
      eff *= pen_TSI_DMA;
      break;
    }

  return(1.0);
//  return(eff);

}

/* -------------------------------------------------------------------- */
static void get_diameter_bins(double volt_avg_up[num_bins], double volt_avg_down[num_bins], int bins, double Dp_m[3][num_bins], int scan_up)
{
  double volt_avg[num_bins], Zp_star[3][num_bins], Qsh, T_K, P_atm;
  int	bin, dma_type;


  Qsh = var[4];
  dma_type = int(var[9]);
  T_K = var[15];
  P_atm = var[16];

  for (bin = 0; bin < bins; ++bin)
    {
    if (scan_up)
      volt_avg[bin] = volt_avg_up[bin];
    else
      volt_avg[bin] = volt_avg_down[bin];

    switch (dma_type)
      {
      case 1 :
        Zp_star[1][bin] = ((2.0*Qsh)/60000.0)*(B_RDMA/100.0)/
		(2.0*pi*volt_avg[bin]*((R2_RDMA*R2_RDMA - R1_RDMA*R1_RDMA)
		/10000.0));
        break;

      case 2 :
        Zp_star[1][bin] = ((2.0*Qsh)/60000.0)*(log(R2_CIT_CDMA/R1_CIT_CDMA))/
		(4.0*pi*(L_CIT_CDMA/100.0)*volt_avg[bin]);
               
        break;

      case 3 :
        Zp_star[1][bin] = ((2.0*Qsh)/60000.0)*(log(R2_TSI_DMA/R1_TSI_DMA))/
		(4.0*pi*(L_TSI_DMA/100.0)*volt_avg[bin]);
        break;

      case 4 :
        Zp_star[1][bin] = ((2.0*Qsh)/60000.0)*(log(R2_nano_DMA/R1_nano_DMA))/
		(4.0*pi*(L_nano_DMA/100.0)*volt_avg[bin]);
        break;
      }

    Dp_m[1][bin] = get_diameter(Zp_star[1][bin], P_atm, T_K);
    }

  for (bin = 1; bin < (bins - 1); ++bin)
    {
    if (scan_up)   {
      Dp_m[0][bin] = exp((log(Dp_m[1][bin-1]) + log(Dp_m[1][bin]))/2.0);
      Dp_m[2][bin] = exp((log(Dp_m[1][bin+1]) + log(Dp_m[1][bin]))/2.0);
      }
    else {
      Dp_m[0][bin] = exp((log(Dp_m[1][bin+1]) + log(Dp_m[1][bin]))/2.0);
      Dp_m[2][bin] = exp((log(Dp_m[1][bin-1]) + log(Dp_m[1][bin]))/2.0);
      }
    }
}

/* -------------------------------------------------------------------- */
static void calc_inversion_matrices(double Zp_high_v_min, double Zp_low_v_max, int mat_num)
{
  int	scan_up, i, bin, j, chgs, seg, s, chg, bins, c,
   	scan_direction_1, scan_direction_2, effective_bins;

  double Qm, Qcnc, T_K, viscosity, Zp_star[3][num_bins], scan_time, t_delay, 
	Cc[3][num_bins], time_step, Zp_seg,	omega, inv_avg, 
	charge_prob[max_chg], alpha, Dp_seg, lnDp, Cc_seg, eff_seg, Qsh,
	T_sat_cnc, T_cond_cnc, P_atm, v_low, v_high, Dp_m[3][num_bins],
	max, min_inv_factor, test[num_bins][max_chg], omega_test,
	eff[num_bins], charging_prob_bin[num_bins][max_chg],
	volt_avg_up[num_bins], volt_avg_down[num_bins], t_up_total,
	t_down_total, t_up[2], t_down[2], Zp_upper_limit[2][num_bins], 
	Zp_lower_limit[2][num_bins], v_effective_up[num_bins],
	v_effective_down[num_bins], dia_chg_bins[num_bins], mfp, cnc_eff,
	cnc_eff_seg, effective_Dp_m[3][num_bins], Zp_star_eff, Cc_eff;

  char	matrix_file[2][file_length], mat_dir[file_length], mat_dir_char, 
	mat_dir_char_str[2], mat_par_inv_file[file_length],
	mat_dia_lv_file[2][file_length], mat_dia_inv_file[2][file_length];

  strcpy(mat_dir, base_directory);
  strcat(mat_dir, "input/mat_");
  mat_dir_char = mat_num + '0';
  mat_dir_char_str[0] = mat_dir_char;
  mat_dir_char_str[1] = '\0';
  strcat(mat_dir, mat_dir_char_str);
  mkdir(mat_dir, 0x666);
  strcat(mat_dir, "/");

  strcpy(mat_par_inv_file, mat_dir);
  strcat(mat_par_inv_file, "par_inv.txt");
  strcpy(mat_dia_lv_file[1], mat_dir);
  strcpy(mat_dia_lv_file[0], mat_dir);
  strcat(mat_dia_lv_file[1], "Dp_lv_up.txt");
  strcat(mat_dia_lv_file[0], "Dp_lv_dn.txt");
  strcpy(mat_dia_inv_file[1], mat_dir);
  strcpy(mat_dia_inv_file[0], mat_dir);
  strcat(mat_dia_inv_file[1], "Dpinv_up.txt");
  strcat(mat_dia_inv_file[0], "Dpinv_dn.txt");
  strcpy(matrix_file[0], mat_dir);
  strcpy(matrix_file[1], mat_dir);
  strcat(matrix_file[0], "mat_dn.txt");
  strcat(matrix_file[1], "mat_up.txt");

  ofstream mat_par_inv_out(mat_par_inv_file, ios::trunc);
	
  scan_time = var[0];
  time_step = var[1];
  Qm = var[3];
  Qsh = var[4];
  Qcnc = var[6];
  v_high = var[7];
  v_low = var[8];
  T_sat_cnc = var[11];
  T_cond_cnc = var[12];
  chgs = int(var[14]);
  T_K = var[15];
  P_atm = var[16];

  scan_direction_1 = 0;
  scan_direction_2 = 1;  
    
  viscosity = (4.64e-8)*T_K + 4.52e-6;  //  kg/m-s
	
  mfp = mfp_1_atm_298_K*(T_K/298.0)/(P_atm/1.0); 

  get_effective_voltages(Zp_high_v_min, Zp_low_v_max, volt_avg_up, volt_avg_down, 
	t_up_total, t_down_total, t_up, t_down, v_effective_up, 
	v_effective_down, t_delay);
	
  effective_bins = int((scan_time + t_delay)/time_step + 0.01);

  mat_par_inv_out<<t_up_total<<endl<<t_down_total<<endl<<t_up[0]<<endl<<
      	t_up[1]<<endl<<t_down[0]<<endl<<t_down[1]<<endl<<t_delay<<endl<<
	num_pts_fit_low_Dp<<endl<<num_pts_fit_high_Dp;

  mat_par_inv_out.close();

  calc_mobility_limits(volt_avg_up, volt_avg_down, T_K, Zp_upper_limit, Zp_lower_limit, 
	t_up_total,t_down_total);

  for (scan_up = scan_direction_1; scan_up <= scan_direction_2; ++scan_up)
    {
    if (scan_up)  {
//	cout<<endl<<"Creating up scan matrix";
	bins = int(t_up_total/time_step + 0.0001);
	}
    else  {
//	cout<<endl<<"Creating down scan matrix";
	bins = int(t_down_total/time_step + 0.0001);
	}
        
    get_diameter_bins(v_effective_up, v_effective_down, effective_bins,
			effective_Dp_m, scan_up);

    ofstream dia_lv_out(mat_dia_lv_file[scan_up], ios::trunc);
        
    for (bin = 0; bin < effective_bins; ++bin)
    {
      dia_lv_out << effective_Dp_m[1][bin] << endl;

      Cc_eff = 1.0 + (2.0*mfp/effective_Dp_m[1][bin])*(1.257 +
		0.4*exp(-1.1*effective_Dp_m[1][bin]/(2.0*mfp)));
      Zp_star_eff = e*Cc_eff/(3.0*pi*viscosity*effective_Dp_m[1][bin]);
    }

    dia_lv_out.close();

    get_diameter_bins(volt_avg_up, volt_avg_down, bins, Dp_m, scan_up);

    ofstream inv_dia_out(mat_dia_inv_file[scan_up], ios::trunc);

    for (bin = 1; bin < (bins - 1); ++bin)
      inv_dia_out<<Dp_m[0][bin]<<"  "<< Dp_m[1][bin]<<"  "<<Dp_m[2][bin]<<endl;

    inv_dia_out.close();

    for (bin = 1; bin < (bins - 1); ++bin)
      dia_chg_bins[bin] = Dp_m[1][bin];            

    ofstream matrix_out(matrix_file[scan_up], ios::trunc);

    for (bin = 1; bin < (bins-1); ++bin)
      {
      for (s = 0; s <= 2; ++s) {
        Cc[s][bin] = 1.0 + (2.0*mfp/Dp_m[s][bin])*(1.257 +
		0.4*exp(-1.1*Dp_m[s][bin]/(2.0*mfp)));
        Zp_star[s][bin] = e*Cc[s][bin]/(3.0*pi*viscosity*Dp_m[s][bin]);
        }

      eff[bin] = get_efficiences((Dp_m[1][bin]*1.0e9), viscosity, Cc[1][bin]);

      calc_rough_charge_prob((dia_chg_bins[bin]*1.0e9), T_K, charge_prob, chgs);

      for (c = 1; c <= chgs; ++c)
        charging_prob_bin[bin][c] = charge_prob[c];
      }

    for (i = 1; i < (bins - 1); ++i)
      {
      max = 0.0;

      for (j = 1; j < (bins - 1); ++j)
        {
        for (chg = 1; chg <= chgs; ++chg)
          {
          cnc_eff = get_cnc_counting_eff(Dp_m[1][j], chg);

          omega_test = get_omega((double(chg)*Zp_star[1][j]), Zp_star[1][i],
		chg, Zp_lower_limit[scan_up][i], Zp_upper_limit[scan_up][i]);

          test[j][chg] = omega_test*charging_prob_bin[j][chg]*eff[j]*cnc_eff;

          if (test[j][chg] > max)
            max = test[j][chg];
          }
        }

      min_inv_factor = max / inv_factor_ratio;

      for (j = 1; j < (bins - 1); ++j)
        {
        alpha = 0.0;
        for (chg = 1; chg <= chgs; ++chg)
          {
          inv_avg = 0.0;
          if (test[j][chg] > min_inv_factor)
            {
            for (seg = 0; seg <= segments; ++seg)
              {
              lnDp = log(Dp_m[0][j])+(log(Dp_m[2][j]) - log(Dp_m[0][j])) *
			double(seg)/double(segments);
              Dp_seg = exp(lnDp);

              Cc_seg = 1.0 + (2.0*mfp/Dp_seg)*(1.257 + 0.4*exp(-1.1*Dp_seg/(2.0*mfp)));
              Zp_seg = chg*e*Cc_seg/(3.0*pi*viscosity*Dp_seg);
              calc_rough_charge_prob((Dp_seg*1.0e9), T_K, charge_prob, chgs);
              eff_seg = get_efficiences((Dp_seg*1.0e9), viscosity, Cc_seg);
              cnc_eff_seg = get_cnc_counting_eff(Dp_seg, chg);
              omega = get_omega(Zp_seg, Zp_star[1][i], chg, 
		Zp_lower_limit[scan_up][i], Zp_upper_limit[scan_up][i]);

              inv_avg += omega*eff_seg*cnc_eff_seg*charge_prob[chg]/double(segments + 1);
              }

            alpha += inv_avg;
            }	
          else
            alpha += test[j][chg];

          }

        if (alpha < alpha_min)
          alpha = 0.0;

        if (Qcnc > Qm)
          alpha *= (Qm*1000.0/60.0)*time_step* (log10(Dp_m[2][j]/Dp_m[0][j]));    // n in #/cm^3
        else
          alpha *= (Qcnc*1000.0/60.0)*time_step* (log10(Dp_m[2][j]/Dp_m[0][j]));

        matrix_out<<alpha<<"  ";
        }

      matrix_out<<endl;
      }

    matrix_out.close();
    }   

}

/* -------------------------------------------------------------------- */
static double gammp(double a, double x)
{
  double gamser,gammcf,gln;

  if (x < 0.0 || a <= 0.0)
    nrerror("Invalid arguments in routine gammp");

  if (x < (a+1.0)) {
    gser(&gamser,a,x,&gln);
    return gamser;
    }
  else {
    gcf(&gammcf,a,x,&gln);
    return 1.0-gammcf;
    }
}

/* -------------------------------------------------------------------- */
double erff(double x)
{
  return(x < 0.0 ? -gammp(0.5,x*x) : gammp(0.5,x*x));
}

/* -------------------------------------------------------------------- */
static const int ITMAX = 100;
static const float EPS = 3.0e-7;
static const float FPMIN = 1.0e-30;

static void gcf(double *gammcf, double a, double x, double *gln)
{
  int i;
  double an,b,c,d,del,h;

  *gln=gammln(a);
  b=x+1.0-a;
  c=1.0/FPMIN;
  d=1.0/b;
  h=d;

  for (i = 1; i <= ITMAX; i++) {
    an = -i*(i-a);
    b += 2.0;
    d = an * d + b;
    if (fabs(d) < FPMIN) d=FPMIN;
    c = b + an / c;
    if (fabs(c) < FPMIN) c=FPMIN;
    d = 1.0 / d;
    del = d * c;
    h *= del;
    if (fabs(del-1.0) < EPS)
      break;
    }

  if (i > ITMAX)
    nrerror("a too large, ITMAX too small in gcf");

  *gammcf=exp(-x+a*log(x)-(*gln))*h;

}

/* -------------------------------------------------------------------- */
static void gser(double *gamser, double a, double x, double *gln)
{
  int	n;
  double sum,del,ap;

  *gln=gammln(a);
  if (x <= 0.0) {
    if (x < 0.0) nrerror("x less than 0 in routine gser");
    *gamser=0.0;
    return;
  } else {
    ap = a;
    del = sum = 1.0 / a;
    for (n = 1; n <= ITMAX; n++) {
      ++ap;
      del *= x / ap;
      sum += del;
      if (fabs(del) < fabs(sum)*EPS) {
        *gamser = sum * exp(-x+a*log(x)-(*gln));
        return;
        }
      }
    nrerror("a too large, ITMAX too small in routine gser");
    return;
    }
}

/* -------------------------------------------------------------------- */
static double gammln(double xx)
{
  double x,y,tmp,ser;
  static double cof[6]={76.18009172947146,-86.50532032941677,
		24.01409824083091,-1.231739572450155,
		0.1208650973866179e-2,-0.5395239384953e-5};
  int j;

  y = x = xx;
  tmp = x + 5.5;
  tmp -= (x+0.5)*log(tmp);
  ser = 1.000000000190015;

  for (j = 0; j <= 5; j++)
    ser += cof[j] / ++y;

  return(-tmp + log(2.5066282746310005 * ser / x));

}

/* -------------------------------------------------------------------- */
//void main(void) 	// For standalone program
void rdma_mat(void) 	// For compiling into rdmap(1)
{
  int	mat_num;
  double Zp_low_v_max, Zp_high_v_min;

  getinfo(mat_num); 
  calc_mobility_bounds(Zp_low_v_max, Zp_high_v_min, mat_num);  
  calc_inversion_matrices(Zp_high_v_min, Zp_low_v_max, mat_num);  

}

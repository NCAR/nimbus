#include <iostream.h>
#include <fstream.h>
#include <iomanip.h>

#include <string>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "nrutil.h"

static const double lowest_counts = 1.0;
static const int file_length = 75;
static const int num_bins = 250;
static const int num_param = 5;
static const int num_twomey_iter = 50;

static char raw_in_file[file_length], raw_out_file_1[file_length],
	raw_out_file_2[file_length], n_long_file_1[file_length],
	n_long_file_2[file_length], n_labview_file[2][file_length], 
	matrix_file[2][file_length], dia_file[2][file_length],
	labview_parameter_file[file_length], matrix_parameter_file[file_length],
	lv_dia_file[2][file_length], Dp_labview_file[2][file_length],
	x, *base_directory, *timeString, *dateString, *flightNumber;

static double	delay_time, plumb_time, tau_cnc, flow_settling_time,
		desmearing_accuracy, scan_time, time_step, temperature,
		pressure;

static int	num_pts_fit_low_Dp, num_pts_fit_high_Dp;


/* -------------------------------------------------------------------- */
#define NR_END 1

static double *vector(long nl, long nh)
// allocate a double vector with subscript range v[nl..nh]
{
  double *v;

  v=(double *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(double)));
//  if (!v) nrerror("allocation failure in vector()");
  return v-nl+NR_END;
}

/* -------------------------------------------------------------------- */
static void nrerror(char error_text[])
{
  fprintf(stderr,"Numerical Recipes run-time error...\n");
  fprintf(stderr,"%s\n",error_text);
  fprintf(stderr,"...now exiting to system...\n");
  exit(1);
}

/* -------------------------------------------------------------------- */
static double gammln(double xx)
{
  double x,y,tmp,ser;
  static double cof[6]={76.18009172947146,-86.50532032941677,
		24.01409824083091,-1.231739572450155,
		0.1208650973866179e-2,-0.5395239384953e-5};
  int	j;

  y=x=xx;
  tmp=x+5.5;
  tmp -= (x+0.5)*log(tmp);
  ser=1.000000000190015;
  for (j=0;j<=5;j++) ser += cof[j]/++y;

  return -tmp+log(2.5066282746310005*ser/x);

}

/* -------------------------------------------------------------------- */
#define ITMAX 100
#define EPS 3.0e-7

static void gser(double *gamser, double a, double x, double *gln)
{
  int n;
  double sum,del,ap;

  *gln=gammln(a);
  if (x <= 0.0)
    {
    if (x < 0.0) nrerror("x less than 0 in routine gser");
    *gamser=0.0;
    return;
    }
  else
    {
    ap=a;
    del=sum=1.0/a;
    for (n=1;n<=ITMAX;n++)
      {
      ++ap;
      del *= x/ap;
      sum += del;
      if (fabs(del) < fabs(sum)*EPS) {
        *gamser=sum*exp(-x+a*log(x)-(*gln));
        return;
        }
      }
    nrerror("a too large, ITMAX too small in routine gser");
    return;
    }

}

/* -------------------------------------------------------------------- */
#undef ITMAX
#undef EPS

#define ITMAX 100
#define EPS 3.0e-7
#define FPMIN 1.0e-30

static void gcf(double *gammcf, double a, double x, double *gln)
{
	int i;
	double an,b,c,d,del,h;

	*gln=gammln(a);
	b=x+1.0-a;
	c=1.0/FPMIN;
	d=1.0/b;
	h=d;
	for (i=1;i<=ITMAX;i++) {
		an = -i*(i-a);
		b += 2.0;
		d=an*d+b;
		if (fabs(d) < FPMIN) d=FPMIN;
		c=b+an/c;
		if (fabs(c) < FPMIN) c=FPMIN;
		d=1.0/d;
		del=d*c;
		h *= del;
		if (fabs(del-1.0) < EPS) break;
	}
	if (i > ITMAX) nrerror("a too large, ITMAX too small in gcf");
	*gammcf=exp(-x+a*log(x)-(*gln))*h;
}

/* -------------------------------------------------------------------- */
#undef ITMAX
#undef EPS
#undef FPMIN

static double gammq(double a, double x)
{
  double gamser,gammcf,gln;

  if (x < 0.0 || a <= 0.0)
    nrerror("Invalid arguments in routine gammq");

  if (x < (a+1.0)) {
    gser(&gamser,a,x,&gln);
    return 1.0-gamser;
    }
  else {
    gcf(&gammcf,a,x,&gln);
    return gammcf;
    }
}

/* -------------------------------------------------------------------- */
#define NRANSI

static void fit(double x[], double y[], int ndata, double sig[], int mwt, double *a,
	double *b, double *siga, double *sigb, double *chi2, double *q)
{
	int i;
	double wt,t,sxoss,sx=0.0,sy=0.0,st2=0.0,ss,sigdat;

	*b=0.0;
	if (mwt) {
		ss=0.0;
		for (i=1;i<=ndata;i++) {
			wt=1.0/SQR(sig[i]);
			ss += wt;
			sx += x[i]*wt;
			sy += y[i]*wt;
		}
	} else {
		for (i=1;i<=ndata;i++) {
			sx += x[i];
			sy += y[i];
		}
		ss=ndata;
	}
	sxoss=sx/ss;
	if (mwt) {
		for (i=1;i<=ndata;i++) {
			t=(x[i]-sxoss)/sig[i];
			st2 += t*t;
			*b += t*y[i]/sig[i];
		}
	} else {
		for (i=1;i<=ndata;i++) {
			t=x[i]-sxoss;
			st2 += t*t;
			*b += t*y[i];
		}
	}
	*b /= st2;
	*a=(sy-sx*(*b))/ss;
	*siga=sqrt((1.0+sx*sx/(ss*st2))/ss);
	*sigb=sqrt(1.0/st2);
	*chi2=0.0;
	if (mwt == 0) {
		for (i=1;i<=ndata;i++)
			*chi2 += SQR(y[i]-(*a)-(*b)*x[i]);
		*q=1.0;
		sigdat=sqrt((*chi2)/(ndata-2));
		*siga *= sigdat;
		*sigb *= sigdat;
	} else {
		for (i=1;i<=ndata;i++)
			*chi2 += SQR((y[i]-(*a)-(*b)*x[i])/sig[i]);
		*q=gammq(0.5*(ndata-2),0.5*(*chi2));
	}
}

/* -------------------------------------------------------------------- */
#undef NRANSI

static void create_file_names(char set_num[3])
{
  char	input_directory[file_length], input_folder[file_length],
	output_directory[file_length], output_folder[file_length];

  sprintf(input_directory, "%s/rdma/dma/inv/input/", base_directory);
  strcpy(input_folder, input_directory);
  strcat(input_folder, "mat_");
  strcat(input_folder, set_num);
  strcat(input_folder, "/");
  sprintf(output_directory, "%s/rdma/dma/inv/output/", base_directory);
  strcpy(output_folder, output_directory);
  strcat(output_folder, "mat_");
  strcat(output_folder, set_num);
  strcat(output_folder, "/");

  if (chdir(output_folder))
    mkdir(output_folder, 0xff);

  strcpy(raw_in_file, input_directory);
  strcat(raw_in_file, "counts.txt");
  strcpy(raw_out_file_1, output_folder);
  strcat(raw_out_file_1, "raw1.");
  strcat(raw_out_file_1, flightNumber);
  strcpy(raw_out_file_2, output_folder);
  strcat(raw_out_file_2, "raw2.");
  strcat(raw_out_file_2, flightNumber);
  strcpy(n_long_file_1, output_folder);
strcpy(n_long_file_1, getenv("DATA_DIR"));
strcat(n_long_file_1, "/rdma/");
  strcat(n_long_file_1, flightNumber);
  strcpy(n_long_file_2, output_folder);
strcat(n_long_file_2, "out_to_nimbus");
//  strcat(n_long_file_2, "n_comb2.txt");
  strcpy(Dp_labview_file[1], output_directory);
  strcat(Dp_labview_file[1], "Dp_up.txt");
  strcpy(Dp_labview_file[0], output_directory);
  strcat(Dp_labview_file[0], "Dp_dn.txt");
  strcpy(n_labview_file[1], output_directory);
  strcat(n_labview_file[1], "n_scan_u.txt");
  strcpy(n_labview_file[0], output_directory);
  strcat(n_labview_file[0], "n_scan_d.txt");
  strcpy(matrix_file[1], input_folder);
  strcat(matrix_file[1], "mat_up.txt");
  strcpy(matrix_file[0], input_folder);
  strcat(matrix_file[0], "mat_dn.txt");
  strcpy(dia_file[1], input_folder);
  strcat(dia_file[1], "Dpinv_up.txt");
  strcpy(dia_file[0], input_folder);
  strcat(dia_file[0], "Dpinv_dn.txt");
  strcpy(labview_parameter_file, input_directory);
  strcat(labview_parameter_file, "LVparOut.txt");
  strcpy(lv_dia_file[1], input_folder);
  strcat(lv_dia_file[1], "Dp_lv_up.txt");
  strcpy(lv_dia_file[0], input_folder);
  strcat(lv_dia_file[0], "Dp_lv_dn.txt");
  strcpy(matrix_parameter_file, input_folder);
  strcat(matrix_parameter_file, "par_inv.txt");

}

/* -------------------------------------------------------------------- */
static void read_in_data(double &t_total_up, double &t_total_down, int &total_bins_up, int &total_bins_down, double t_extr_up[2], double t_extr_down[2])
{
  int	i;
  char	filename[256];
  double var[num_param];

  sprintf(filename, "%s/rdma/dma/inv/stored/param.txt", base_directory);
  ifstream param_in(filename);
  for (i = 0; i < num_param; ++i)
    param_in>>var[i];
  param_in.close();

  scan_time = var[0];
  time_step = var[1];

  ifstream labview_par_in(labview_parameter_file);
  labview_par_in>>flow_settling_time>>plumb_time>>tau_cnc>>desmearing_accuracy;
  labview_par_in.close();

  ifstream matrix_par_in(matrix_parameter_file);
  matrix_par_in>>t_total_up>>t_total_down>>t_extr_up[0]>>t_extr_up[1]>>t_extr_down[0]>> t_extr_down[1]>>delay_time>>num_pts_fit_low_Dp>>num_pts_fit_high_Dp;

  matrix_par_in.close();

  total_bins_up = int(t_total_up/time_step + 0.01);
  total_bins_down = int(t_total_down/time_step + 0.01);

}

/* -------------------------------------------------------------------- */
static void correct_for_cnc(double counts_unsmeared_up[num_bins], double counts_unsmeared_down[num_bins])
{
  double total_time_up_down, all_counts_raw, seg1_secs, seg2_secs,
	counts_raw[num_bins], alpha[50], sum_alpha, counts_unsmeared[num_bins],
	counts_inv[num_bins], seconds_smeared, scan_num_double, cnc_conc;
  int	total_bins, bin, bins_smeared, smear, start_bin, i, scan_num, bins_scan;

  seconds_smeared = -tau_cnc*log(desmearing_accuracy/100.0);

  if (seconds_smeared)
    bins_smeared = int(seconds_smeared/time_step + 1.01);
  else
    bins_smeared = 0;

  bins_scan = int((scan_time + delay_time)/time_step + 0.01);
    
  start_bin = int((flow_settling_time + delay_time)/time_step);
  total_time_up_down = 2.0*scan_time + 3.0*delay_time + plumb_time + flow_settling_time;

  if (int(total_time_up_down/time_step) != (total_time_up_down/time_step))
    total_bins = int(total_time_up_down/time_step + 2.01);
  else
    total_bins = int(total_time_up_down/time_step + 1.01);

  ofstream raw_out_1(raw_out_file_1, ios::app);

  ifstream raw_in_1(raw_in_file);
  raw_in_1>>scan_num_double;
  scan_num = int(scan_num_double);

  raw_out_1<<endl<<endl<<setw(12)<<"Time (sec)"<<setw(12)<<"Raw Counts"<<endl<<endl;

  for (bin = 1; bin < total_bins; ++bin)
    {
    raw_in_1>>all_counts_raw;
    raw_out_1<<setw(12)<<(bin*time_step)<<setw(12)<<all_counts_raw<<endl;
    counts_raw[bin] = all_counts_raw;
    }

  raw_in_1>>cnc_conc;
   
  raw_out_1.close();
  raw_in_1.close();

  ofstream raw_out_2(raw_out_file_2, ios::app);

  ifstream raw_in_2(raw_in_file);
  raw_in_2>>scan_num_double;
  scan_num = int(scan_num_double);

  raw_out_2<<endl<<endl<<setw(12)<<"Time (sec)"<<setw(12)<<"Raw Counts"<<endl<<endl;

  for (bin = 0; bin < total_bins; ++bin)
    {
    raw_in_2>>all_counts_raw;
    raw_out_2<<setw(12)<<(bin*time_step)<<setw(12)<<all_counts_raw<<endl;
    }

  raw_out_2.close();
  raw_in_2.close();

  if (bins_smeared == 0)
    alpha[0] = 1.0;
  else
    {
    sum_alpha = 0.0;
    for (i=0; i<=bins_smeared; ++i)
      sum_alpha += exp(-i*time_step/tau_cnc);
    for (i=0; i<=bins_smeared; ++i)
       alpha[i] = exp(-i*time_step/tau_cnc)/sum_alpha;
    }

  for (bin = 0; bin < total_bins; ++bin)
    {
    counts_unsmeared[bin] = counts_raw[bin];

    for (smear=(bin-1); smear >= (bin - bins_smeared); --smear)
      if (smear >= 0)
        counts_unsmeared[bin] -= counts_unsmeared[smear]*alpha[bin-smear];

    counts_unsmeared[bin] /= alpha[0];  
    }
     
  for (bin = 0; bin < int((total_bins - plumb_time/time_step)); ++bin)
    {
    seg1_secs = time_step*(plumb_time/time_step - int(plumb_time/time_step));
    seg2_secs = time_step - seg1_secs;

    counts_inv[bin] = 0.0;
    counts_inv[bin] +=
	counts_unsmeared[int(bin + (plumb_time - seg1_secs)/time_step + 0.01)] *
	seg2_secs/time_step +
	counts_unsmeared[int(bin + (plumb_time + seg2_secs)/time_step + 0.01)] *
	seg1_secs/time_step;
    }

  for (bin = 0; bin < total_bins; ++bin)
    if (fabs(counts_inv[bin]) < lowest_counts)  counts_inv[bin] = 0.0;   

  for (bin = start_bin; bin < (bins_scan + start_bin); ++bin)
    counts_unsmeared_up[bin - start_bin] = counts_inv[bin];

  for (bin = (bins_scan + start_bin); bin < total_bins; ++bin)
    counts_unsmeared_down[bin - start_bin - bins_scan] = counts_inv[bin];

}	/* END CORRECT_FOR_CNC */

/* -------------------------------------------------------------------- */
static void get_final_counts_vector(double t_up[2], double t_down[2], double t_total_up, double t_total_down, double diameter[3][num_bins], double counts_unsmeared[num_bins], int &extr_low_bins, int &extr_high_bins, int scan_up, int bins, double counts[num_bins])
{
  int	bin, i, k, num_pts_beg, num_pts_end, s, num_pts_extr_low_bin,
	num_pts_extr_high_bin, mwt;
  double  a_high_bin, a_low_bin, b_high_bin, b_low_bin, n_extr, lnDp,
	*lnD_unit_offset, *known_counts_unit_offset, t_total, t_extr[2],
	dia_max, siga_unused, sigb_unused, chi2_unused, q_unused,
	*sig_unused_array;
	
  ifstream dia_in(dia_file[scan_up]);
  for (bin = 1; bin < (bins - 1); ++bin)
    dia_in >> diameter[0][bin] >> diameter[1][bin] >> diameter[2][bin];
  dia_in.close();

  lnD_unit_offset = vector(1, 20);
  known_counts_unit_offset = vector(1, 20);
  sig_unused_array = vector(1, 20);

  mwt = 0;  // std dev not used in fitting

  if (scan_up)
    {
    t_total = t_total_up - 2*time_step;
    num_pts_extr_low_bin = num_pts_fit_low_Dp;
    num_pts_extr_high_bin = num_pts_fit_high_Dp;
    for (k = 0; k < 2; ++k) t_extr[k] = t_up[k] - time_step;
    dia_max = diameter[1][int(t_total/time_step + 0.0001)];
    }
  else
    {
    t_total = t_total_down - 2*time_step;
    num_pts_extr_low_bin = num_pts_fit_high_Dp;
    num_pts_extr_high_bin = num_pts_fit_low_Dp;
    for (k = 0; k < 2; ++k)  t_extr[k] = t_down[k] - time_step;
    dia_max = diameter[1][1];
    }

  extr_low_bins = int(t_extr[0]/time_step + 0.0001);
  extr_high_bins = int(t_extr[1]/time_step + 0.0001);

  bins = int(t_total/time_step + 0.0001);

  for (i = 1; i <= bins; ++i)
    counts[i] = 0.0;

  for (i = int(t_extr[0]/time_step + 1 + 0.00001); 
	i < int((scan_time + delay_time + t_extr[0])/time_step + 1 + 0.0001); ++i)  
    counts[i] = counts_unsmeared[int(i - (t_extr[0]/time_step + 1) + 0.0001)];
   
  num_pts_beg = num_pts_end = 0;
  i = 1;

  for (bin = 1; bin <= bins; ++bin)
    {
    if (num_pts_beg < ((t_extr[0]/time_step) + num_pts_extr_low_bin))
      {
      if (bin == 1 || diameter[1][bin] != diameter[1][bin-1])
        {
        ++num_pts_beg;

        lnDp = log(diameter[1][bin]);
        if (num_pts_beg > (t_extr[0]/time_step))
          {
          lnD_unit_offset[i] = lnDp;
          known_counts_unit_offset[i] = counts[bin];
          ++i;
          }
        }
      }
    }

  fit(	lnD_unit_offset, known_counts_unit_offset, num_pts_extr_low_bin,
	sig_unused_array, mwt, &a_low_bin, &b_low_bin, &siga_unused,
	&sigb_unused, &chi2_unused, &q_unused);

  for (s = 1; s <= (t_extr[0]/time_step); ++s)
    {
    n_extr = b_low_bin*(log(diameter[1][s])) + a_low_bin;      
    counts[s] = n_extr;
    }

  i = 1;

  for (bin = bins; bin > 0; --bin)
    {
    if (num_pts_end < ((t_extr[1]/time_step) + num_pts_extr_high_bin))
      {
      if ((bin == bins) || (diameter[1][bin] != diameter[1][bin+1]))
        {
        ++num_pts_end;

        lnDp = log(diameter[1][bin]);
        if (num_pts_end > (t_extr[1] / time_step))
          {
          lnD_unit_offset[i] = lnDp;
          known_counts_unit_offset[i] = counts[bin];
          ++i;
          }
        }
      }
    }

  fit(	lnD_unit_offset, known_counts_unit_offset, num_pts_extr_high_bin,
	sig_unused_array, mwt, &a_high_bin, &b_high_bin, &siga_unused,
	&sigb_unused, &chi2_unused, &q_unused);

  for (s = 0; s < (t_extr[1]/time_step); ++s)
    {
    n_extr = b_high_bin * (log(diameter[1][bins - s])) + a_high_bin;    
    counts[bins - s] = n_extr;
    }

}	/* END GET_FINAL_COUNTS_VECTOR */

/* -------------------------------------------------------------------- */
static void solve_for_n(double dia_from_inv[3][num_bins], double counts[num_bins], int bins, int extr_low_bins, int extr_high_bins, int scan_up, ofstream &nimbus_out)
{
  double  denominator, alpha[num_bins][num_bins], n_vector[num_bins],
	X[num_bins], alpha_max_check, alpha_max, new_alpha, total_conc,
	total_vol, n_final[num_bins], dia_final[num_bins];

  int	i, j, bin, loop, bins_summed, bins_averaged, bins_final,
	hour, minute, second;

  time_t	start, end, total_sec;
  struct tm	dat, *ndat;

  ifstream alpha_max_in(matrix_file[scan_up]);

  alpha_max = 0.0;

  for (i = 0; i < (bins - 2); ++i)
    {
    for (j = 0; j < (bins - 2); ++j)
      {
      alpha_max_in>>alpha_max_check;
      if (alpha_max_check > alpha_max)
        alpha_max = alpha_max_check;
      }
    }
	
  alpha_max_in.close();

  ifstream matrix_in(matrix_file[scan_up]);

  for (i = 0; i < (bins - 2); ++i)
    {
    for (j = 0; j < (bins - 2); ++j)
      {
      matrix_in>>new_alpha;
      alpha[i][j] = new_alpha/alpha_max;
      }
    }	

  matrix_in.close();

  for (bin = 1; bin < (bins - 1); ++bin)
    {
    bins_averaged = 0;
    while (counts[bin] < 0.0)
      {
      ++bins_averaged;
      bins_summed = 0;
      for (i = 0; i <= bins_averaged; ++i)
        {
        if((bin+i) < (bins - 1)) ++bins_summed;
        if((bin-i) >= 0)  ++bins_summed;
        }

      counts[bin] *= (2.0/bins_summed);
      for (i = 1; i <= bins_averaged; ++i)
        {
        if ((bin+i) < (bins - 1))
          {
          counts[bin] += counts[bin+i]/bins_summed;
          counts[bin+i] *= ((bins_summed - 1.0)/bins_summed);
          }
        if ((bin-i) >= 0)
          {
          counts[bin] += counts[bin-i]/bins_summed;
          counts[bin-i] *= ((bins_summed - 1.0)/bins_summed);
          }
        }
      }
    }

  for (i = 0; i < (bins - 3); ++i)
    {
    n_vector[i] = counts[i+1];  

    if (n_vector[i] < 1.0)
      n_vector[i] = 0.1;
    }
    
  for (loop = 0; loop < num_twomey_iter; ++loop)
    {
    for (i = 0; i < (bins - 3); ++i)
      {
      denominator = 0.0;

      for (j = 0; j < (bins - 3); ++j)
        denominator += (alpha[i][j]*n_vector[j]);
		
      if (denominator <= 0)   
        cout<<endl<<loop<<"  "<<i<<"  "<<denominator<<"  "<<n_vector[j]<<"  "<<dia_from_inv[1][i];
			
      X[i] = counts[i+1]/denominator;
      }

    for (i = 0; i < (bins - 3); ++i)
      for (j = 0; j < (bins - 3); ++j)
        n_vector[j] *= (1.0 + (X[i] - 1.0)*alpha[i][j]);
    }

  for (i = 0; i < (bins - 3); ++i)
    n_vector[i] /= alpha_max;


  total_conc = total_vol = 0.0;

  for (i = 0, bin = 1; bin < bins - 1; ++bin)
    {
    if ((bin > extr_low_bins) && (bin < (bins - extr_high_bins - 1)))
      {
      n_final[i] = n_vector[bin - 1];

      dia_final[i] = dia_from_inv[1][bin];

      total_conc += n_final[i]*fabs(log10(dia_from_inv[2][bin]/dia_from_inv[0][bin]));
      total_vol += (pow((dia_from_inv[1][bin]*1.0e6), 3.0))*M_PI*(1.0/6.0)*
	n_final[i]*fabs(log10(dia_from_inv[2][bin]/dia_from_inv[0][bin]));

      ++i;
      }
    }

  bins_final = i;

  ofstream n_combined_out(n_long_file_1, ios::app);

  sscanf(dateString, "%02d/%02d/%02d", &dat.tm_year, &dat.tm_mon, &dat.tm_mday);
  sscanf(timeString, "%02d:%02d:%02d", &dat.tm_hour, &dat.tm_min, &dat.tm_sec);
  dat.tm_year += 100;	// since 1900.
  --dat.tm_mon;

  total_sec = mktime(&dat);

  n_combined_out << flightNumber << ",";

  if (scan_up)
    {
    start = total_sec - 154;
    end = total_sec - 94;

    ndat = gmtime(&start);
    ndat->tm_year += 1900;

    n_combined_out <<	setw(4) << setfill('0') << ndat->tm_year << "/" <<
    			setw(2) << setfill('0') << ndat->tm_mon+1 << "/" <<
    			setw(2) << setfill('0') << ndat->tm_mday << ",";

    n_combined_out <<	setw(2) << setfill('0') << ndat->tm_hour << ":" <<
    			setw(2) << setfill('0') << ndat->tm_min << ":" <<
    			setw(2) << setfill('0') << ndat->tm_sec << "-";

    ndat = gmtime(&end);

    n_combined_out <<	setw(2) << setfill('0') << ndat->tm_hour << ":" <<
    			setw(2) << setfill('0') << ndat->tm_min << ":" <<
    			setw(2) << setfill('0') << ndat->tm_sec << "\n";

    start = total_sec - 124;
    ndat = gmtime(&start);
    nimbus_out <<	setw(2) << setfill('0') << ndat->tm_hour << ":" <<
    			setw(2) << setfill('0') << ndat->tm_min << ":" <<
    			setw(2) << setfill('0') << ndat->tm_sec << "  up\n";
    }
  else
    {
    start = total_sec - 87;
    end = total_sec - 27;

    ndat = gmtime(&start);
    ndat->tm_year += 1900;

    n_combined_out <<	setw(4) << setfill('0') << ndat->tm_year << "/" <<
    			setw(2) << setfill('0') << ndat->tm_mon+1 << "/" <<
    			setw(2) << setfill('0') << ndat->tm_mday << ",";

    n_combined_out <<	setw(2) << setfill('0') << ndat->tm_hour << ":" <<
    			setw(2) << setfill('0') << ndat->tm_min << ":" <<
    			setw(2) << setfill('0') << ndat->tm_sec << "-";

    ndat = gmtime(&end);

    n_combined_out <<	setw(2) << setfill('0') << ndat->tm_hour << ":" <<
    			setw(2) << setfill('0') << ndat->tm_min << ":" <<
    			setw(2) << setfill('0') << ndat->tm_sec << "\n";

    start = total_sec - 57;
    ndat = gmtime(&start);
    nimbus_out <<	setw(2) << setfill('0') << ndat->tm_hour << ":" <<
    			setw(2) << setfill('0') << ndat->tm_min << ":" <<
    			setw(2) << setfill('0') << ndat->tm_sec << "  down\n";
    }

  n_combined_out << temperature << "," << pressure << "," << total_conc
	<< "," << total_vol << "," << endl;

  nimbus_out << total_conc << " " << total_vol << endl;


  for (bin = 0; bin < i; ++bin)
    {
    n_combined_out << (dia_final[bin] * 1.0e9);

    if (bin < i-1)
      {
      n_combined_out << ",";

      if (((bin+1) % 5) == 0)
        n_combined_out << endl;
      }

    if (scan_up)
      nimbus_out << (dia_final[bin] * 1.0e9) << " " <<n_final[bin] << "\n";
    else
      nimbus_out <<(dia_final[i-bin-1]*1.0e9) <<" " <<n_final[i-bin-1] <<"\n";
    }

  n_combined_out << endl;

  for (bin = 0; bin < i; ++bin)
    {
    n_combined_out << n_final[bin];

    if (bin < i-1)
      {
      n_combined_out << ",";

      if (((bin+1) % 5) == 0)
        n_combined_out << endl;
      }
    }


  n_combined_out << endl;

  n_combined_out.close();

}	/* END SOLVE_FOR_N */

/* -------------------------------------------------------------------- */
//void main(int argc, char *argv[])
void rdma_inv(int argc, char *argv[])
{
  double diameter_up[3][num_bins], diameter_down[3][num_bins],
	counts_unsmeared_up[num_bins], counts_unsmeared_down[num_bins],
	t_extr_up[2], t_extr_down[2], t_total_up, t_total_down,
	final_counts_up[num_bins], final_counts_down[num_bins];

  int	extr_low_bins, extr_high_bins, scan_up, total_bins_up, total_bins_down;
  char	set_num_char[3], buffer[1024];

  static bool	firstTime = true;

  flightNumber = argv[1];
  dateString = argv[2];
  timeString = argv[3];
  pressure = atof(argv[4]);
  temperature = atof(argv[5]);


  if (firstTime)
    {
    base_directory = getenv("NIMBUS");

    sprintf(buffer, "%s/rdma/dma/inv/input/counts.txt", base_directory);
    ifstream set_num_in(buffer);
    set_num_in >> set_num_char;
    set_num_in.close();

    create_file_names(set_num_char);
    firstTime = false;
    }

  read_in_data(t_total_up, t_total_down, total_bins_up, total_bins_down, t_extr_up, t_extr_down);

  correct_for_cnc(counts_unsmeared_up, counts_unsmeared_down);

  ofstream	nimbus_out(n_long_file_2, ios::beg | ios::trunc);

  for (scan_up = 1; scan_up >= 0; --scan_up)
    {
    if (scan_up)
      {
      get_final_counts_vector(t_extr_up, t_extr_down, t_total_up, t_total_down,
		diameter_up, counts_unsmeared_up, extr_low_bins, extr_high_bins,
		scan_up, total_bins_up, final_counts_up);

      solve_for_n(diameter_up, final_counts_up, total_bins_up, extr_low_bins,
		extr_high_bins, scan_up, nimbus_out);
      }
    else
      {
      get_final_counts_vector(t_extr_up, t_extr_down, t_total_up, t_total_down,
		diameter_down, counts_unsmeared_down, extr_low_bins,
		extr_high_bins, scan_up, total_bins_down, final_counts_down);

      solve_for_n(diameter_down, final_counts_down, total_bins_down,
		extr_low_bins, extr_high_bins, scan_up, nimbus_out);
      }
    }

  nimbus_out.close();

}	/* END MAIN */

/* END RDMA_INV.CC */

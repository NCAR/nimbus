static char SccsId[] = "@(#)dplot.c	@(#)dplot.c	1.1   4/12/94";

/*
 *    program plots specific range vs Mach#, param by gross weight
 *
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "./ncg.struct"

FILE *result_file = NULL;

#define NB 500

float plot_x_min = 0.2, plot_x_max = 0.9;
//float plot_x_min = 0.2, plot_x_max = 0.50;
float plot_y_min = 0.05, plot_y_max = 0.25;
struct NCG_parameters ncg;

int title_length, title2_length;
char title[30] = "Specific Range, FLxxx";

 /* set for ncar calls   */
int zero = 0, one = 1, two = 2, three = 3, four = 4, five = 5, six = 6;
int seven = 7, eight = 8, nine = 9;
int minus_one = -1, one_hundred = 100;
float fzero = 0.;
float size = 14., angle = 0., center = 0.;
char label[80];
int label_l;
int gwt[10], igwt = -1;

/*
 *   function declarations
 */

void plchhq_ ();
void colors_ ();
void gcolor_ ();
void gsclip_ ();
void set_ ();
void setusv_ ();
void lablx_ ();
void lably_ ();


double log_plot_x_min, log_plot_x_max, log_plot_x;

int TwoThousand = 2000;
int FourThousand = 4000;


main (argc, argv)
     int argc;
     char *argv[];

{
    int i, j, k, n;
    int date;
    int end_flag;			/* 0 to accumulate plot info, 1 for final */
    char *ch;
    char line[200];
    char idlabel[200];
    float plow = 0.2, phigh = 0.8;
    int line_width = 4000, one = 1, label_length;
    int iz, ic, id;

    int ifile, nb;
    char file_name[100];
    float x, y, angd = 0., cntr = -1., size_letter = 15.;
    float x_traj, y_traj, z_traj;
    double bin_size;
    float xplot, yplot;

    printf(" argc = %d\n", argc);
    if (argc > 1) {
        printf(" pt 3\n");
        printf(" argc=%d\n", argc);
        printf(" argv[1]=%s\n", argv[1]);
        ic = atoi(argv[1]);
        printf(" ic=%d\n", ic);
    }
    date = kdate_ ();
//	read from data file:
    float xp, yp; 
    char *chc, cch;
    float Sr[10][20], Mt[10][20];
    float V1, V2;
    int gw = -1, gwlast = -1;
    int ij = 0, ilast = 0;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 20; j++) {
            Mt[i][j] = 0.;
        }
    }
    float MachMin = 1., MachMax = 0.;
    float SrMin = 1., SrMax = 0.;
// read data from encoding of PTS.xxx from flight manual:
//  FILE *data_file = fopen ("./PTS.350", "r");
    char fname[50];
    int FL = ic;
    sprintf(fname, "./PTS.%d", FL);
    printf(" ready to read data file, file name = %s\n", fname);
    FILE *data_file = fopen (fname, "r");
	// skip first three lines:
    chc = fgets (line, 200, data_file);
    chc = fgets (line, 200, data_file);
    chc = fgets (line, 200, data_file);
    while ((chc = fgets (line, 200, data_file)) != NULL) {
        sscanf(line, "%d,%f,%f", &gw, &V1, &V2);
        printf(" data line char %d M# %f SR %f\n", gw, V1, V2);
        if (gw != gwlast) {
            igwt++;
            ij = 0;
        }
        gwlast = gw;
        gwt[igwt] = gw;
        Mt[igwt][ij] = V1;
        Sr[igwt][ij] = V2;
        if (V1 < MachMin) {MachMin = V1;}
        if (V1 > MachMax) {MachMax = V1;}
        if (V2 < SrMin) {SrMin = V2;}
        if (V2 > SrMax) {SrMax = V2;}
        ij++;
    }
    title_length = strlen (title);

    ncg.yl2 = 0.9;
    printf(" here is the ncar () call, %f %f %f %f\n", plot_x_min, plot_x_max, plot_y_min, plot_y_max);
    ncar_ (&plot_x_min, &plot_x_max, &plot_y_min, &plot_y_max, &seven,
             &five, &four, &five, &one, &nine, 10);

    lably_("Specific Range", 14);
    lablx_("Mach No.",8);
    setusv_("LW", &TwoThousand);
    xplot = 0.8;
    frstpt_(&xplot, &plot_y_min);
    vector_(&xplot, &plot_y_max);
    sprintf(title, "Specific Range, FL%d", FL);
    title_ (&one, title, title_length);

    gcolor_(&five);
    int lw = 2500;
    gcolor_(&five);
    setusv_("LW", &lw);
    for (i = 0; i < 10; i++) {
	for (j = 0; j < 20; j++) {
//	    if (j == 0) {frstpt_(&(Mt[i][j]), &(Sr[i][j]));}
//          else if (Mt[i][j] > 0.) {vector_(&(Mt[i][j]), &(Sr[i][j]));}
        }
    }
// now do Lagrange interpolation and plot smoothed points:
    int NT = 4, First, iM;
    float Mach;
    char lbl[4];
// open file for save of interpolated values:
    char sname[] = "SRT.txt";
    FILE *save_file = fopen (sname, "a");
    
    for (i = 0; i <= igwt; i++) {		// i indexes gross weight
        fprintf (save_file, "%d,", FL);
        fprintf (save_file, "%d", gwt[i]);
        gcolor_(&eight);
	Mach = Mt[i][0];		// do first point in list
        xplot = Mach - 0.02;
        yp = Sr[i][0];
        yplot = yp - 0.003;
        sprintf (lbl, "%2d", gwt[i]);
        if (i%2 != 1) { plchhq_(&xplot, &yplot, lbl, &size, &angle, &center, 2);}
	frstpt_(&Mach, &yp);
//      fprintf (save_file, ",%0.2f,%0.4f", Mach, yp);
        for (iM = 20; iM < 90; iM++) {	// iM indexes Mach No
	    Mach = iM / 100.;
	    printf(" Mach = %f, Mt[%d][0] = %f, i = %d\n", Mach, i, Mt[i][0], i);
            if (Mach > Mt[i][0]) {
                k = 0;
                while (Mt[i][k] > 0. && k < 19 && Mt[i][k] < Mach) {k++;}
                k -= 2;			// this leaves 2 before and 2 after pt
                if (k < 0) {k = 0;}
                while (Mt[i][k+3] == 0.) {k--;}
		while (Mt[i][k] == 0.) {k++;}
		if (Mach < Mt[i][k+3]) {
		    printf(" call to lagrg with Mach = %f and k = %d \n", Mach, k);
                    (void) lagrg_(&Mach, &NT, &(Mt[i][0])+k, &(Sr[i][0])+k, &yp);
		    printf(" return from lagrg with yp = %f \n", yp);
	            vector_(&Mach, &yp);
                    fprintf (save_file, ",%.3f,%.3f", Mach, yp);
		}
	    }
	}
        fprintf (save_file, "\n");
	k += 3;
	Mach = Mt[i][k];
	yp = Sr[i][k];
	vector_(&Mach, &yp);
        gcolor_(&seven);
        xplot = Mt[i][4];
        yplot = Sr[i][4];
//      plchhq_(&xplot, &yplot, ":KGU:", &size, &angle, &center, 5);
        plchhq_(&xplot, &yplot, "+", &size, &angle, &center, 1);
        printf(" long-range point for i=%d is %f, %f\n", i, Mt[i][4], Sr[i][4]);
    }

    frame_ ();
    clsgks_();
    fclose (save_file);
}


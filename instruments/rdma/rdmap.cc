
// Use this #define to insert a fixed raw count array.

//#define FIXED


#include <algorithm>
#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>
#include <string>

#include "Time.h"
#include "netcdf.hh"

#define N_IN_BINS	62	// Number of RDMA bins from instrument.

static const float	fixed_atx = 293.0;
static const float	fixed_psx = 0.75;

void	ReadNetCDF(char fn[]),
	processScan(char date[], char time[], float counts[]),
	readNewScan(char time[], float diam[]);

void rdma_mat(void);
void rdma_inv(int argc, char *argv[]);
static float polyinterp(float xa[], double ya[], int n, float x);
static float average(float *val, int recNum);


static char		buffer[4096], temperature[32], pressure[32];
static bool		pigeon;

// NetCDF file globals.
static NcFile		*netCDF_file;
static NcVar		*avar = 0, *cvar = 0, *conc = 0;
static char		*FlightNumber, *ProjectNumber;
static FlightClock	startTime, endTime;

static float	newDiam[300], midDiam[300];
static float	*atx, *psx;	// Temp & pressure from netCDF file (whole flight).

static float counts1[] = { 0, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 3, 3, 3, 3, 1, 2, 2, 1, 2, 4, 7, 6, 5, 8, 8, 5, 7, 14, 12, 9, 12, 11, 12, 14, 12, 6, 11, 8, 16, 8, 8, 11, 5, 9, 5, 3, 13, 17, 22, 14, 12, 19, 27, 41, 61, 92, 108, 91, 78, 44, 65, 91, 84, 79, 77, 61, 79, 84, 80, 78, 78, 77, 114, 86, 69, 48, 68, 93, 126, 109, 77, 67, 27, 27, 20, 8, 8, 13, 10, 7, 10, 4, 8, 11, 16, 7, 11, 8, 20, 16, 11, 16, 8, 17, 18, 7, 8, 8, 6, 9, 4, 3, 9, 3, 8, 5, 5, 3, 3, 1, 1, 2, 5, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, -99 };

//using namespace std;

/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  int	i;
  float	counts[300];
  char	time[32], date[32];
  char	counts_filename[256], netCDF_filename[256];


  // "Pigeon hole" the conc array, don't bother doing interp.  Useful for
  // diagnostics against what the laptop produced.
  if (argc > 1 && strcmp(argv[1], "-p") == 0)
    pigeon = true;
  else
    pigeon = false;
   

  std::cout << "Enter RDMA counts file [ /jnet/data/181rf01.cnts.rdma ] :";
  std::cin >> counts_filename;

//strcpy(counts_filename, "/jnet/data/161rf09");
//std::cout << " WARNING !!!!!  Filename is fixed at " << counts_filename;

  if (strstr(counts_filename, ".cnts.rdma") == NULL)
    strcat(counts_filename, ".cnts.rdma");

  std::ifstream	counts_file(counts_filename);

  std::cout << "\n";

  counts_file >> netCDF_filename; 
  ReadNetCDF(netCDF_filename);

#ifdef FIXED
for (i = 2; counts1[i] != -99; ++i)
  counts1[i] = 10.0;
#endif

  while (1)
    {
    counts_file >> date; 

    if (counts_file.eof())
      break;

    counts_file >> time; 
    counts_file >> temperature; 
    counts_file >> pressure; 

    if (strcmp(pressure, "nan") == 0 || atof(pressure) < 0.05 || atof(pressure) > 1.1)
      strcpy(pressure, "0.75");

#ifdef FIXED
    sprintf(temperature, "%f", fixed_atx);
    sprintf(pressure, "%f", fixed_psx);
#endif

std::cout << "--------------------------\n" << date << " " << time << "\n";

    i = 0;
    do
      {
      counts_file >> counts[i];
#ifdef FIXED
      counts[i] = counts1[i];  // Over-ride file with const array.
#endif
      }
    while (counts[i++] != -99.0);

    processScan(date, time, counts);
    readNewScan(time, counts);
    }

  netCDF_file->close();

}	/* END MAIN */

/* -------------------------------------------------------------------- */
void processScan(char date[], char time[], float counts[])
{
  int	i;
  char	*args[12];


  strcpy(buffer, getenv("DATA_DIR"));
  strcat(buffer, "/rdma/dma/inv/stored/param.txt");
  std::ofstream	parmFP(buffer, std::ios::trunc);

  parmFP << "60\n1\n0.49\n0.49\n4.98\n4.98\n0.88\n5000\n25\n1\n1\n25\n18\n1\n5\n" << temperature << "\n" << pressure << "\n";

  parmFP.close();


  strcpy(buffer, getenv("DATA_DIR"));
  strcat(buffer, "/rdma/dma/inv/input/counts.txt");
  std::ofstream	cntsFP(buffer, std::ios::trunc);

  cntsFP << "1\n";

  for (i = 0; counts[i] != -99; ++i)
    cntsFP << (int)counts[i] << "\n";

  cntsFP.close();


  // Compute new counts & cell diameters based on press & temp.
  rdma_mat();

  // Compute new concentrations based on press & temp.
  args[1] = FlightNumber;
  args[2] = date;
  args[3] = time;
  args[4] = pressure;
  args[5] = temperature;

  rdma_inv(6, args);

}

/* -------------------------------------------------------------------- */
void readNewScan(char time[], float counts[])
{
  int	i, k, inCnts, recordNumber, nc, oc;
  long	start[3], count[3];
  float	netCDFcounts[150], concOut[300], diameter[300], newTotalConc;
  double	concentration[300], oldTotalConc, totalVol,
		abar, pbar, press, temp, inConc, dlogd[300];

  char	dummy[64];
  FlightClock	thisTime;


  /* Read processed scan from mat & inv, performs interp to fixed cell
   * sizes and then writes to the netCDF file.
   */


  strcpy(buffer, getenv("DATA_DIR"));
  strcat(buffer, "/rdma/dma/inv/output/mat_1/out_to_rdmap");

  press = atof(pressure);
  temp = atof(temperature);

  std::ifstream inFP(buffer);

  if (inFP.is_open() == false)
  {
    printf("readNewScan: %s open failed.\n", buffer);
    exit(1);
  }

  for (inCnts = 14, i = 0; i < 2; ++i)	// 2 scans, up & down
    {
    inFP >> time >> dummy >> inConc >> totalVol;

    oldTotalConc = newTotalConc = 0.0;

    /* Counts array consists of 14 hold samples, 62 up scan, 6 hold
     * and 62 down.  Total of 157 samples come from RDMA.
     */
    for (k = 1; k <= N_IN_BINS; ++inCnts, ++k)
      {
      netCDFcounts[k] = counts[inCnts];
      inFP >> diameter[k] >> dlogd[k] >> concentration[k];
      oldTotalConc += concentration[k] * dlogd[k];
      }

    inCnts += 6;

    thisTime = time;

//std::cout << "   " << startTime.c_str() << " " << thisTime.c_str() << "\n";
//std::cout << "   " << thisTime - startTime << "\n";

    recordNumber = thisTime - startTime;
    if (recordNumber < 0)
      recordNumber += 86400;

    if (recordNumber > 43200)
      continue;
//std::cout << "   " << recordNumber << "\n";

    start[0] = recordNumber; start[1] = start[2] = 0;
    count[0] = count[1] = 1; count[2] = 64;

/*
for(k = 1; k < 15; ++k)
  printf("%.2f ", midDiam[k]);
printf("\n");

for(k = 1; k < 15; ++k)
  printf("%.2f ", diameter[k]);
printf("\n");
*/

    for (nc = 1; nc < 64; ++nc)
      concOut[nc] = -32767.0;

    if (pigeon)
      {
      for (oc = 1; oc <= N_IN_BINS; ++oc)
        {
        for (nc = 1; newDiam[nc] < diameter[oc]; ++nc)
          ;

        if (concOut[nc] < 0.0)
          concOut[nc] = concentration[oc];
        else
          {
          concOut[nc] += concentration[oc];
          concOut[nc] /= 2;
          }
        }
      }
    else
      {
      float	f;

      for (nc = 1; newDiam[nc] < diameter[1]; ++nc)
        ;

      concOut[nc] = polyinterp(&diameter[1], &concentration[1], 3, midDiam[nc]);
      ++nc;

      for (; nc < 64; ++nc)
        {
        for (oc = 1; oc <= N_IN_BINS && diameter[oc] < midDiam[nc]; ++oc)
          ;

        if (oc >= N_IN_BINS)
          break; 

        oc = std::min(oc-2, N_IN_BINS-4);
        f = polyinterp(&diameter[oc], &concentration[oc], 4, midDiam[nc]);
/*
        // Avoid letting things go below zero.
        if (f < 0.0)
          if (concentration[oc+1] < 1.0 && concentration[oc+2] < 1.0)
            f = 0.0;
          else
            f = std::min(concentration[oc+1], concentration[oc+2]);
*/
        concOut[nc] = f;
        }
      }


    pbar = average(psx, recordNumber);
    abar = average(atx, recordNumber) + 273.15;

#ifdef FIXED
    pbar = fixed_psx * 1000.0;
    abar = fixed_atx;
#endif

    for (k = 1; k < 64; ++k)
      {
      if (concOut[k] > 0.0)
        {
        if (pbar > 0.0 || abar > 0.0)
          concOut[k] *= (pbar / (press * 1000.0)) * (temp / abar);

        concOut[k] *= dlogd[k];
        newTotalConc += concOut[k];
//        concOut[k] /= 2.3035;
        }
      }

printf("iConc=%f, oConc=%f, nConc=%f\n", inConc, oldTotalConc, newTotalConc);

    netCDFcounts[0] = concOut[0] = 0.0;
    netCDFcounts[63] = -32767.0;

    avar->set_cur(start);
    avar->put(netCDFcounts, count);

    cvar->set_cur(start);
    cvar->put(concOut, count);

    conc->set_cur(start);
    conc->put(&newTotalConc, 1);
printf("%ld %f\n", start[0], newTotalConc);
    }

  inFP.close();

}

/* -------------------------------------------------------------------- */
/**
 * Open netCDF file, locate ARDMA & CRDMA (counts and conc arrays) and CONC_FCR
 * total concentration in file and assign to static (this file) vars.
 * Locate AND read ATX & PSXC for whole flight.
 * Leave file open for later re-writing of CRDMA & CONC_FCR.
 */
void ReadNetCDF(char fileName[])
{
  int	i;
  NcVar	*var, *atxv = 0, *psxv = 0;
  NcAtt	*attr;

  netCDF_file = new NcFile(fileName, NcFile::Write);

  if (netCDF_file->is_valid() == false)
  {
    std::cerr << "Can't open " << fileName << "\n";
    exit(1);
  }

  attr = netCDF_file->get_att("ProjectNumber");
  if (attr->is_valid())
    ProjectNumber = attr->as_string(0);

  attr = netCDF_file->get_att("FlightNumber");
  if (attr->is_valid())
    FlightNumber = attr->as_string(0);

  attr = netCDF_file->get_att("TimeInterval");
  if (attr->is_valid())
  {
    startTime = attr->as_string(0);
    endTime = strchr(attr->as_string(0), '-')+1;
  }

  /* Scan variables for RDMA vectors.
   */
  avar = cvar = NULL;

  for (i = 2; i < netCDF_file->num_vars(); ++i)
  {
    var = netCDF_file->get_var(i);

    if (var->num_dims() == 3)
    {
      if (strncmp("ARDMA", var->name(), 5) == 0)
        avar = var;

      if (strncmp("CRDMA", var->name(), 5) == 0)
        cvar = var;
    }


    if (strcmp("CONCR_FCR", var->name()) == 0)
      conc = var;

    if (strcmp("ATX", var->name()) == 0)
      atxv = var;

    if (strcmp("PSXC", var->name()) == 0)
      psxv = var;

/*
    if (strncmp("HOUR", var->name(), 5) == 0)
      hour = var;

    if (strncmp("MINUTE", var->name(), 5) == 0)
      min = var;

    if (strncmp("SECOND", var->name(), 5) == 0)
      sec = var;
*/
  }

  if (avar == 0 || cvar == 0 || conc == 0)
  {
    std::cerr << "ReadNetCDF: ARDMA or CRDMA or CONCR_FCR not found.\n";
    exit(1);
  }

/*
  if ((attr = avar->get_att("FirstBin")))
    firstBin = attr->as_short(0);
  else
    firstBin = 1;

  if ((attr = avar->get_att("LastBin")))
    lastBin = attr->as_short(0);
  else
    lastBin = avar->get_dim(2)->size()-1;
*/

  attr = avar->get_att("CellSizes");

  if (attr->is_valid())
  {
    for (i = 0; i < attr->num_vals(); ++i)
      newDiam[i] = attr->as_float(i) * 1000; // Change to micrometers.
  }
  else
  {
    std::cerr << "rdmap: CellSizes attribute not found.\n";
    exit(1);
  }


  for (i = 1; i < attr->num_vals(); ++i)
  {
    midDiam[i] = (newDiam[i-1] + newDiam[i]) / 2;
//printf("%f\n", midDiam[i]);
  }


  if (atxv)
  {
    atx = new float[atxv->num_vals()];
    atxv->get(atx, atxv->num_vals());
  }
  else
  {
    std::cerr << "ReadNetCDF: No ATX variable\n";
    exit(1);
  }

  if (psxv)
  {
    psx = new float[psxv->num_vals()];
    psxv->get(psx, psxv->num_vals());
  }
  else
  {
    std::cerr << "ReadNetCDF: No PSXC variable\n";
    exit(1);
  }

}	/* READNETCDF */

/* -------------------------------------------------------------------- */
static float polyinterp(float xa[], double ya[], int n, float x)
{
  int	i, m, ns = 0;
  double	den, dif, dift, ho, hp, w;
  double	*c, *d, y, dy;
//printf(" pix: %.2f %.2f %.2f %.2f = %.2f\n", xa[0], xa[1], xa[2], xa[3], x);

  dif = fabs(x - xa[0]);
  c = new double[n];
  d = new double[n];

  for (i = 0; i < n; ++i)
    {
    if ((dift = fabs(x - xa[i])) < dif)
      {
      ns = i;
      dif = dift;
      }

    c[i] = d[i] = log10(ya[i]);

    if (ya[i] < 0.01)
      c[i] = d[i] = -2.0;
    }
//printf(" log: %.2f %.2f %.2f %.2f = ", c[0], c[1], c[2], c[3]);

  y = log10(ya[ns--]);

  for (m = 1; m < n; ++m)
    {
    for (i = 0; i < n-m; ++i)
      {
      ho = xa[i] - x;
      hp = xa[i+m] - x;
      w = c[i+1] - d[i];
      if ((den = ho-hp) == 0.0) return(-9.0);
      den = w / den;
      d[i] = hp * den;
      c[i] = ho * den;
      }

    y += (dy = (2 * ns < (n-m) ? c[ns+1] : d[ns--]));
    }

  delete [] c;
  delete [] d;
//printf("%.3f\n", y);
//printf(" piy: %.2f %.2f %.2f %.2f = %.3f\n", ya[0], ya[1], ya[2], ya[3], pow(10.0, y));
  return((float)pow((double)10.0, (double)y));

}

/* -------------------------------------------------------------------- */
static float average(float *val, int recNum)
{
  float	bar = 0.0;
  int	k, bcntr = 0;

  for (k = -31; k < 31; ++k)
    {
    if (recNum+k > 0)	// Does not handle going off the end of flight...
      {
      bar += val[recNum+k];
      ++bcntr;
      }
    }

  return(bar / bcntr);

}
/* END RDMAP.CC */

#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip.h>
#include <cstdlib>
#include <cstdio>
#include <string>

#include "/jnet/shared/include/netcdf.hh"

void ReadNetCDF(char fn[]);
void processScan(char date[], char time[], char temp[], char press[], float counts[]);
void rdma_mat(void);
void rdma_inv(int argc, char *argv[]);

static NcFile	*netCDF_file;
static NcVar	*avar, *cvar;
static char	*FlightNumber;


/* -------------------------------------------------------------------- */
main(int argc, char *argv[])
{
  int	i;
  float	counts[300];
  char	time[32], date[32], temperature[32], pressure[32];
  char	counts_filename[256], netCDF_filename[256];


  cout << "Enter RDMA counts file [ /jnet/data/181rf01.cnts.rdma ] :";
  cin >> counts_filename;

  if (strstr(counts_filename, ".cnts.rdma") == NULL)
    strcat(counts_filename, ".cnts.rdma");

  ifstream	counts_file(counts_filename);

  cout << "\n";

  counts_file >> netCDF_filename; 
  ReadNetCDF(netCDF_filename);

  while(1)
    {
    counts_file >> date; 

    if (counts_file.eof())
      break;

    counts_file >> time; 
    counts_file >> temperature; 
    counts_file >> pressure; 

    i = 0;
    do
      {
      counts_file >> counts[i];
      }
    while (counts[i++] != -99.0);

    processScan(date, time, temperature, pressure, counts);
    }

  netCDF_file->close();

}	/* END MAIN */

/* -------------------------------------------------------------------- */
void processScan(	char date[], char time[],
			char temp[], char press[],
			float counts[])
{
  int	i;
  char	buffer[4096], *args[12];


  strcpy(buffer, getenv("NIMBUS"));
  strcat(buffer, "/rdma/dma/inv/stored/param.txt");
  ofstream	parmFP(buffer, ios::beg | ios::trunc);

  parmFP << "60\n1\n0.49\n0.49\n4.98\n4.98\n0.88\n5000\n25\n1\n1\n25\n18\n1\n5\n" << temp << "\n" << press << "\n";

  parmFP.close();


  strcpy(buffer, getenv("NIMBUS"));
  strcat(buffer, "/rdma/dma/inv/input/counts.txt");
  ofstream	cntsFP(buffer, ios::beg | ios::trunc);

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
  args[4] = press;
  args[5] = temp;

  rdma_inv(6, args);

/*
  sprintf(buffer, "%s/rdma/dma/inv/input/out_to_nimbus", getenv("NIMBUS"));

  if ((inFP = fopen(buffer, "r")) == NULL) {
    fprintf(stderr, "rdma.c: can't open %s\n", buffer);
    exit(1);
    }

// Read in counts & conc.???  Interp to fixed diameters.


  fclose(inFP);
*/
}

/* -------------------------------------------------------------------- */
void ReadNetCDF(char fileName[])
{
  NcAtt	*attr;

  netCDF_file = new NcFile(fileName);

  if (netCDF_file->is_valid() == false) {
    cerr << "Can't open " << fileName << "\n";
    exit(1);
    }

  attr = netCDF_file->get_att("FlightNumber");
  if (attr->is_valid())
    FlightNumber = attr->as_string(0);

}

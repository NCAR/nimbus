/**
 * This program makes two small test netCDF files.  See nVariables and nRecords
 * for file dimensions.  The first file is all 1hz data, the second has 5  variables
 * at 5hz and 5 at 25hz.  Future developments could/would include histogram
 * variables.
 */

#include <cstdlib>
#include <netcdfcpp.h>

static const size_t nVariables = 10;
static const size_t nRecords = 60;

void createGlobalAttrs(NcFile & file, NcVar *time_v)
{
  file.add_att("Source", "NCAR Research Aviation Facility");
  file.add_att("ProjectName", "EPIC2001");
  file.add_att("DateProcessed", "2009-11-14 21:22:07 +0000");
  file.add_att("Platform", "N130AR");
  file.add_att("ProjectNumber", "161");
  file.add_att("FlightNumber", "rf09");
  file.add_att("FlightDate", "09/20/2001");
  file.add_att("TimeInterval", "16:00:00-16:00:59");

  time_v->add_att("long_name", "time of measurement");
  time_v->add_att("standard_name", "time");
  time_v->add_att("units", "seconds since 2001-09-20 16:00:00 +0000");
  time_v->add_att("strptime_format", "seconds since %F %T %z");
}

void makeLRTfile()
{
  NcFile file("lrt.nc", NcFile::New);
  NcDim *time_d;
  NcVar *vars[10], *time_v;
  float *varData[nVariables];

  if (!file.is_valid())
  {
    std::cerr << "Failed to create file\n";
    exit(1);
  }

  time_d = file.add_dim("Time", nRecords);	// 1 minute of data.
  time_v = file.add_var("Time", ncInt, time_d);
  createGlobalAttrs(file, time_v);

  for (size_t i = 0; i < nVariables; ++i)
  {
    char name[64];
    sprintf(name, "VAR%d", i);
    vars[i] = file.add_var(name, ncFloat, time_d);
    vars[i]->add_att("_FillValue", (float)-32767.0);
    vars[i]->add_att("units", "test_units");
    sprintf(name, "Title for variable %d", i);
    vars[i]->add_att("long_name", name);
    varData[i] = new float[nRecords];
  }

  int timeData[nRecords];

  for (size_t i = 0; i < nRecords; ++i)
  {
    timeData[i] = i;
    for (size_t j = 0; j < nVariables; ++j)
      varData[j][i] = 100 * j + i;
  }

  time_v->put(timeData, nRecords);
  for (size_t i = 0; i < nVariables; ++i)
  {
    vars[i]->put(varData[i], nRecords);
  }

  file.close();
}


void setData(float *varData, int i, int offset, int rate)
{
  for (size_t j = 0; j < rate; ++j)
    varData[(i*rate)+j] = (float)offset + i + ((float)j / rate);
}


void makeHRTfile()
{
  NcFile file("hrt.nc", NcFile::New);
  NcDim *time_d, *rate5_d, *rate25_d;
  NcVar *vars[10], *time_v;
  float *varData[nVariables];

  // This routine creates 5 variables at 5hz and 5 at 25hz.
  int rates[] = { 5, 5, 5, 5, 5, 25, 25, 25, 25, 25 };

  if (!file.is_valid())
  {
    std::cerr << "Failed to create file\n";
    exit(1);
  }

  time_d = file.add_dim("Time", nRecords);
  rate5_d = file.add_dim("sps5", 5);
  rate25_d = file.add_dim("sps25", 25);

  time_v = file.add_var("Time", ncInt, time_d);
  createGlobalAttrs(file, time_v);

  for (size_t i = 0; i < nVariables; ++i)
  {
    char name[64];
    sprintf(name, "VAR%d", i);
    if (rates[i] == 5)
      vars[i] = file.add_var(name, ncFloat, time_d, rate5_d);
    if (rates[i] == 25)
      vars[i] = file.add_var(name, ncFloat, time_d, rate25_d);

    vars[i]->add_att("_FillValue", (float)-32767.0);
    vars[i]->add_att("units", "test_units");
    sprintf(name, "Title for variable %d", i);
    vars[i]->add_att("long_name", name);
    varData[i] = new float[nRecords * rates[i]];
  }

  int timeData[nRecords];

  for (size_t i = 0; i < nRecords; ++i)
  {
    timeData[i] = i;

    for (size_t j = 0; j < nVariables; ++j)
      setData(varData[j], i, j * 100, rates[j]);
  }

  time_v->put(timeData, nRecords);
  for (size_t i = 0; i < nVariables; ++i)
  {
    vars[i]->put(varData[i], nRecords, rates[i]);
  }

  file.close();
}


int main(int argc, char *argv[])
{
  makeLRTfile();
  makeHRTfile();

  return 0;
}

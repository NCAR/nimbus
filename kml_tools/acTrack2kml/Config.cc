

#include "Config.hh"
#include <stdlib.h>  // getenv()

#include <iostream>

using std::cerr;
using std::endl;

Config::
Config() : 
  TimeStep(15),
  TAS_CutOff(20.0), 
  ts_Freq(2000), 
  barb_Freq(5), 
  altMode("absolute"), 
  convertToFeet(1.0), 
  showit(true),
  onboard(false),
  verbose(0),
  update_interval_secs(30),
  position_interval_secs(3),
  run_once(false)
{
}


bool
Config::
verifyDatabaseHost()
{
  if (database_host.empty())
  {
    char *p = getenv("PGHOST");
    if (p) 
    {
      database_host = p;
    }
  }
  return !database_host.empty();
}


void
Config::
dump(std::ostream& out)
{
  out << "       flight_data dir: " << flightDataDir << "\n";
  out << " google earth data dir: " << flightDataDir + "/GE" << "\n";
  out << "              Web host: " << webHost << "\n";
  out << "    Platform selection: " << platform << "\n";
  out << "          Onboard mode: " << onboard << "\n";
  out << "   KML interval (secs): " << update_interval_secs << "\n";
  out << "  JSON interval (secs): " << position_interval_secs << "\n";
  out << "      Time step (secs): " << TimeStep << "\n";
  out << "            TAS cutoff: " << TAS_CutOff << "\n";
  out << " Time stamps (minutes): " << ts_Freq << "\n";
  out << "  Wind Barbs (minutes): " << barb_Freq << "\n";
  out << "     KML altitude mode: " << altMode << "\n";
  out << "       KML output file: " << outputKML << "\n";
  out << "       netCDFinputFile: " << netCDFinputFile << "\n";
  out << "         Database host: " << database_host << "\n";
  out << "         Database name: " << dbname << "\n";
  out << "         Verbose level: " << verbose << "\n";
}



void
Config::
setAltitudeUnits(const string& units)
{
  convertToFeet = 1.0;
  if (units == "m")
  {
    convertToFeet = 3.2808; // feet per meter
  }
  cerr << "cfg.alt: " << altVariable
       << " units: " << units << endl;
  cerr << "cfg.convertToFeet: " << convertToFeet << endl;
}


std::string
Config::
getLatitudeVariable(const std::string& sourcename)
{
  if (latVariable.empty())
  {
    latVariable = sourcename;
  }
  return latVariable;
}

std::string
Config::
getLongitudeVariable(const std::string& sourcename)
{
  if (lonVariable.empty())
  {
    lonVariable = sourcename;
  }
  return lonVariable;
}

std::string
Config::
getAltitudeVariable(const std::string& sourcename)
{
  if (altVariable.empty())
  {
    altVariable = sourcename;
  }
  return altVariable;
}

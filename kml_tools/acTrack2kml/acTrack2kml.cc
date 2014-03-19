/*
 * Program to read real-time flight database periodically (say 60 seconds)
 * and create a KML for google earth.
 *
 * COPYRIGHT: University Corporation for Atmospheric Research, 2005-2012
 */
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <cmath>
#include <cstdio>
#include <cstring>

using namespace std;

// Output directories for .xml & .kml files.  Ground.
static const string grnd_flightDataDir = "/net/www/docs/flight_data";
static const string grnd_webHost = "www.eol.ucar.edu";

// Output directories for .xml & .kml files.  Onboard.
static const string onboard_flightDataDir = "/var/www/html/flight_data";
static const string onboard_webHost = "acserver.raf.ucar.edu";

#include "acDatabase.hh"
#include "ncTrack.hh"
#include "ProjectInfo.hh"
#include "Config.hh"
#include "AircraftTrack.hh"
#include "AircraftTrackKML.hh"
#include "osm.hh"

/* -------------------------------------------------------------------- */
int usage(const char *argv0)
{
  cerr
	<< "Usage: has two forms, one for real-time use and the other to scan\n"
	<< "	a netCDF file in post-processing mode.\n\n"
	<< "Real-time onboard form:\n"
	<< "	acTrack2kml -o [-h database_host] [-p platform]\n\n"
	<< "Real-time ground form:\n"
	<< "	acTrack2kml [-h database_host] -p platform\n\n"
	<< "Post-processing:\n"
	<< "	acTrack2kml infile.nc outfile.kml\n\n"
	<< "Options:\n"
	<< "  -p platform       Platform name, valid values (C130, DC8, GV, WKA, A10, N42RF, N43RF, B146).\n"
	<< "  -h database_host  Database server host with data.\n"
	<< "  -o                Run onboard, changes webhost to onboard server.\n"
	<< "  -b barb_freq      Frequency of wind barbs in minutes, default is 5.\n"
	<< "  -c                Clamp track to ground, ignore altitude.\n"
	<< "  -v                Verbose output.\n"
	<< "  -t ts_freq        Frequency of time stamps in minutes, default is 2000 (off).\n"
	<< "  -s time_step      Time interval of data points for track in seconds, default is 15.\n"
	<< "  -i update_secs    In real-time mode, seconds between track updates, default is 30.\n"
	<< "  -j update_secs    In real-time mode, seconds between JSON position updates, default is 3.\n"
	<< "  -f path           Override flight_data output directory, location of position.json,\n"
	<< "                    and KML goes into <flight_dir>/GE.\n";

  return 1;
}

/* -------------------------------------------------------------------- */
int
parseRunstring(int argc, char** argv, Config& cfg)
{
  extern char *optarg;	/* set by getopt() */
  extern int optind;	/* "  "     "     */
  int opt_char;		/* option character */

  // Default to ground, -p and netCDF mode.
  cfg.webHost = grnd_webHost;

  while ((opt_char = getopt(argc, argv, "p:h:b:s:t:i:j:f:ocv")) != -1)
  {
    switch (opt_char)
    {
    case 'p':	// platform selection, used to select dbname
      cfg.platform = optarg;
      // TODO query the platforms DB to get a list of these...
      if ( cfg.platform.compare("GV") &&
           cfg.platform.compare("N42RF") &&	// NOAA AOC P3 - Kermit
           cfg.platform.compare("N43RF") &&	// NOAA AOC P3 - Miss Piggy
           cfg.platform.compare("N49RF") &&	// NOAA AOC G4
           cfg.platform.compare("DC8") &&
           cfg.platform.compare("A10") &&
           cfg.platform.compare("WKA") &&	// Wyoming King Air
           cfg.platform.compare("B146") &&	// UK BAE146
           cfg.platform.compare("C130") )
      {
        cerr << "\n\tplatform must be GV, C130, WKA, DC8, A10, N42RF, N43RF, N49RF, B146\n\n";
        return usage(argv[0]);
      }
      break;

    case 'h':	// PGHOST over-ride.
      cfg.database_host = optarg;
      break;

    case 's':	// Time-step, default is 15 seconds.
      cfg.TimeStep = atoi(optarg);
      if (cfg.TimeStep < 5) 
	cfg.TimeStep = 5;
      break;

    case 'b':	// Windbarb Frequency, default is 5 minutes.
      cfg.barb_Freq = atoi(optarg);
      if (cfg.barb_Freq < 1) cfg.barb_Freq = 2000; // 24 hours, basically turn off.
      break;

    case 't':	// Time Stamp Frequency, default is 20 minutes.
      cfg.ts_Freq = atoi(optarg);
      if (cfg.ts_Freq < 1) cfg.ts_Freq = 2000; // 24 hours, basically turn off.
      break;

    case 'o':	// onboard.  Modify some defaults if this is set.
      cfg.onboard = true;
      break;

    case 'c':	// Clamp to ground
      cfg.altMode = "clampToGround";
      break;

    case 'v':
      cfg.verbose = true;
      break;

    case 'f':
      cfg.flightDataDir = optarg;
      break;

    case 'i':
      {
	int period = atoi(optarg);
	if (period == 0)
	{
	  cerr << "Invalid update interval: " << optarg << endl;
	  return usage(argv[0]);
	}
	cfg.update_interval_secs = period;
      }
      break;

    case 'j':
      {
	int period = atoi(optarg);
	if (period == 0)
	{
	  cerr << "Invalid update interval: " << optarg << endl;
	  return usage(argv[0]);
	}
	cfg.position_interval_secs = period;
      }
      break;

    case '?':
      return usage(argv[0]);
      break;
    }
  }

  if ((optind+2) == argc)	// netCDF mode.
  {
    cfg.netCDFinputFile = argv[optind];
    cfg.outputKML = argv[optind+1];
  }

  if ((optind+1) == argc)	// netCDF mode, but they left off output file.
  {
    return usage(argv[0]);
  }

  // Derive the database name according to the platform setting.

  if (! cfg.netCDFinputFile.empty())
  {
    // This is not real-time.
  }
  else if (cfg.onboard)
  {
    cfg.dbname = "real-time";
    cfg.webHost = onboard_webHost;
    // It's possible to run onboard but for a platform other than the host
    // platform.  So if platform is set, it implies using the
    // platform-specific forms of database name and GE path.
    if (cfg.flightDataDir.empty())
    {
      if (cfg.platform.empty())
      {
	cfg.flightDataDir = onboard_flightDataDir;
      }
      else
      {
	cfg.flightDataDir = onboard_flightDataDir + "/" + cfg.platform;
      }
    }
    // If platform is set, then use the platform-specific database name and
    // pull the track points from the database server on the ground.
    if (! cfg.platform.empty())
    {
      cfg.dbname = "real-time-" + cfg.platform;

      // On the plane PGHOST is always set, so we must preempt it to pull
      // from the ground database.
      if (cfg.database_host.empty())
	cfg.database_host = "eol-rt-data.fl-ext.ucar.edu";
    }
  }
  else if (cfg.platform.empty())
  {
    cerr << "Platform must be set with -p to derive database name when not onboard." << endl;
    return usage(argv[0]);
  }
  else
  {
    cfg.dbname = "real-time-" + cfg.platform;
    if (cfg.flightDataDir.empty())
    {
      cfg.flightDataDir = grnd_flightDataDir+"/"+cfg.platform;
    }
  }

  if (cfg.netCDFinputFile.empty() && ! cfg.verifyDatabaseHost())
  {
    cerr << "Database host must be set, either with -h or PGHOST." << endl;
  }

  return 0;
}

// build up config from command-line options
// create our AircraftTrack "model"
// if database:
// 	create database
// 	connect to database
// 	database loads variables and attributes
//     while updating-in-real-time:
// 	fill model
// 	generate kml
// else
// 	open netcdf file
// 	fill model
// 	generate kml


/* -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
  int rc;
  Config cfg;
  AircraftTrack track;
  AircraftTrackKML kml;
  OSM osm;

  if (argc == 1 || strstr(argv[1], "usage") || strstr(argv[1], "help"))
    return usage(argv[0]);

  if (argc > 1)
  {
    if ((rc = parseRunstring(argc, argv, cfg)) != 0)
      return rc;
  }

  if (cfg.verbose)
  {
    cerr << "Initial Configuration:\n";
    cfg.dump(cerr);
  }

  kml.setConfig(cfg);
  if (cfg.netCDFinputFile.length() > 0)
  {
    kml.postProcessMode = true;
    ncTrack nct;
    if (nct.open(cfg.netCDFinputFile))
    {
      nct.setConfig(cfg);
      nct.fillAircraftTrack(track);
      kml.setTrack(&track);
      kml.WriteGoogleEarthKML(cfg.outputKML);
      return 0;
    }
    else
    {
      return 1;
    }
  }

  cout << endl;
  cout << "  Using database host : " << cfg.database_host << endl;
  cout << "  Output directory : " << cfg.flightDataDir << endl;
  cout << endl;

  // Real-time mode.
  //
  // Keep the database object around so it can maintain the global
  // attributes and variable information, none of which we need to be
  // refreshed unless the track gets too old.

  acDatabase db;
  db.setConfig(cfg);
  osm.setConfig(cfg);
  time_t last_kml = 0;
  time_t last_json = 0;
  while (1)
  {
    while (! db.openDatabase())
    {
      sleep(3);
    }

    bool rewrite = db.fillAircraftTrack(track) && track.npoints() > 0;
    db.closeDatabase();
    time_t now = time(0);

    if (now - last_kml >= cfg.update_interval_secs)
    {
      last_kml = now;
      // Avoid writing a new track file if the track has not changed, and
      // do not erase an old track until there are points in a new one.
      kml.setTrack(&track);
      string realtime = cfg.flightDataDir + "/GE/real-time.kml";
      if (rewrite || !kml.checkFile(realtime))
	kml.WriteGoogleEarthKML(realtime);
      string animated = cfg.flightDataDir + "/GE/animated-track.kml";
      if (rewrite || !kml.checkFile(animated))
	kml.WriteGoogleEarthAnimatedKML(animated);
      string current = cfg.flightDataDir + "/GE/current_pos.kml";
      if (rewrite || !kml.checkFile(current))
	kml.WriteCurrentPositionKML(current);

      if (cfg.verbose && !rewrite)
      {
	cerr << "Track has not changed, existing KML files not rewritten." << endl;
      }
    }

    if (now - last_json >= cfg.position_interval_secs)
    {
      last_json = now;
      std::string filename = cfg.flightDataDir + "/position.json";
      osm.writePositionJSON(track, filename);
    }

    // Sleep until the next time for an update.
    int delay = std::min(last_kml + cfg.update_interval_secs - now,
			 last_json + cfg.position_interval_secs - now);
    if (cfg.verbose)
    {
      cerr << "Sleeping for " << delay << " seconds..." << endl;
    }
    if (delay > 0)
      sleep(delay);
  }

  return 0;
}

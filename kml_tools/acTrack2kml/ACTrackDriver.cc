/*
 * Program to read real-time flight database periodically (say 60 seconds)
 * and create a KML for google earth.
 *
 * COPYRIGHT: University Corporation for Atmospheric Research, 2005-2014
 */

#include "ACTrackDriver.hh"

#include "acDatabase.hh"
#include "ncTrack.hh"
#include "ProjectInfo.hh"

#include <iostream>
#include <fstream>

#include <cmath>
#include <cstdio>
#include <cstring>
#include <unistd.h> // alarm()
#include <signal.h>
#include <getopt.h>

using namespace std;
using namespace boost::posix_time;

ACTrackDriver::
ACTrackDriver()
{
}


/*
 * The check could use the lag threshold as the end time in the select
 * query, and it could grab only the last point prior to that by using
 * descending order and limit 1.  Then the AircraftTrackKML::checkFile()
 * method does not need to back through times to account for lag, and only
 * one point needs to be retrieved instead of a whole track.  That approach
 * is easy enough with SQL but maybe a little harder with netcdf, but the
 * netcdf check is not really necessary.
 *
 * It would be nice to abbreviate the status messages a little more, or
 * combine them one all the files are up to date.
 */
int
ACTrackDriver::
StatusCheck(bool ok, const std::string& msg)
{
  std::string name = "acTrack2kml-onboard";
  if (!cfg.platform.empty())
  {
    name = "acTrack2kml-" + cfg.platform;
  }
  if (ok)
  {
    std::cerr << name << " OK - " << msg << endl;
  }
  else
  {
    std::cerr << name << " CRITICAL - " << msg << endl;
  }
  return (ok ? 0 : 2);
}



/* -------------------------------------------------------------------- */
std::string
ACTrackDriver::
usage(const char *argv0)
{
  std::ostringstream buf;
  Config default_config;

  buf 	<< 
    "Usage: Two forms, one for real-time use and the other to scan\n" <<
    "       a netCDF file in post-processing mode.\n\n" <<
    "Real-time onboard form:\n" <<
    "   acTrack2kml -o [-h database_host] [-p platform]\n\n" <<
    "Real-time ground form:\n" <<
    "   acTrack2kml [-h database_host] -p platform\n\n" <<
    "Post-processing:\n" <<
    "   acTrack2kml infile.nc outfile.kml [position.json]\n\n" <<
    "Options:\n" <<
    "  -p platform       Platform name, valid values \n" <<
    "                    (C130, DC8, GV, WKA, A10, N42RF, N43RF, B146).\n" <<
    "  -h database_host  Database server host with data.\n" <<
    "  -o                Run onboard, changes URLs to onboard server.\n" <<
    "  -b barb_freq      Wind barb interval in minutes, default is 5.\n" <<
    "  -c                Clamp track to ground, ignore altitude.\n" <<
    "  -v                Each -v increases the verbose level.\n" <<
    "  --showstats       Report statistics about the track and any\n" <<
    "                    generated paths.\n" <<
    "  --path algo       Specify track path algorithm.\n" <<
    "                    Default is 'timestep'. See below for details.\n" <<
    "  -t ts_mins        Interval between time stamps in minutes,\n" << 
    "                    default is 2000 (off).\n" <<
    "  -s timestep_secs  Time interval between track data points,\n" <<
    "                    in seconds, when the track path\n" <<
    "                    algorithm is 'timestep'.  The default is 15.\n" <<
    "  -i update_secs    In real-time mode, seconds between track\n" << 
    "                    updates.  Default is 30.\n" <<
    "  -j update_secs    In real-time mode, seconds between JSON position\n" <<
    "                    updates.  Default is 3.\n" <<
    "  --once            Run once and exit, without looping to update.\n" <<
    "  -f path           Override flight_data output directory. Location\n" <<
    "                    of position.json and KML goes into <flight_dir>/GE.\n"
	<<
    "  --output <prefix> Path prefix to be used for all output files.\n" <<
    "                    Overrides flight_data output directory (-f).\n" <<
    "                      --output WINTER-rf13- \n" <<
    "                    Write WINTER-rf13-real-time.kml instead of \n" <<
    "                      <flight_data>/GE/real-time.kml.\n" <<
    "  -u url            Override URL for the flight_data directory.\n" <<
    "                     Plane default: " <<
    Config::defaultOnboardFlightDataURL() << "\n" <<
    "                     Ground default: " <<
    Config::defaultGroundFlightDataURL() << "\n" <<
    "  --color           KML track color, in the hex form aabbggrr.\n" <<
    "  --check           Do not generate any files, only check that\n" <<
    "                    the expected output is up to date with track.\n" <<
    "There are two path simplification algorithms, and their specifiers\n" <<
    "take these forms:\n" <<
    "  --path headingstep[:interval]\n" <<
    "                    Skip points until the heading differs by\n" <<
    "                    <interval> degrees.  The default interval is \n" <<
    "                    " << default_config.HeadingStep << " degrees.\n" <<
    "  --path timestep[:interval]\n" <<
    "                    Skip points until the timestamp differs by\n" <<
    "                    " << default_config.TimeStep << " minutes.\n" <<
    "                    This is the original algorithm.\n" <<
    "The position variables can be overridden with these options:\n" <<
    "  --lat <lat>       Specify the latitude variable.\n" <<
    "  --lon <lon>       Specify the longitude variable.\n" <<
    "  --alt <alt>       Specify the altitude variable.\n" <<
    "This example selects GGLAT and GGLON from a netcdf file instead\n" <<
    "of LATC and LONC.\n" <<
    "  acTrack2kml --lat=GGLAT --lon=GGLON infile.nc outfile.kml\n";

  return buf.str();
}


/* -------------------------------------------------------------------- */
int
ACTrackDriver::
parseRunstring(int argc, char** argv)
{
  int opt_char;		/* option character */

  while (1)
  {
    static struct option long_options[] = {
      {"lat",      required_argument,  0,  '0' },
      {"lon",      required_argument,  0,  '1' },
      {"alt",      required_argument,  0,  '2' },
      {"once",     0,                  0,  '3' },
      {"check",    0,                  0,  '4' },
      {"path",     required_argument,  0,  '5' },
      {"color",    required_argument,  0,  '6' },
      {"showstats",0,                  0,  '7' },
      {"output",   required_argument,  0,  '8' },
      {0,          0, 0, 0 }
    };

    opt_char = getopt_long(argc, argv, "p:h:b:s:t:i:j:f:u:ocv",
			   long_options, 0);

    if (opt_char == -1)
      break;

    switch (opt_char)
    {
    case '0':
      cfg.latVariable = optarg;
      break;

    case '1':
      cfg.lonVariable = optarg;
      break;

    case '2':
      cfg.altVariable = optarg;
      break;

    case '3':
      cfg.run_once = true;
      break;

    case '4':
      cfg.check = true;
      break;

    case '5':
      // Parse the specifier and enable the algorithm.
      cfg.path_method = optarg;
      break;

    case '6':
      cfg.color = optarg;
      break;

    case '7':
      cfg.showstats = true;
      break;

    case '8':
      cfg.outputPrefix = optarg;
      break;

    case 'p':	// platform selection, used to select dbname
      if (! cfg.setPlatform(optarg))
      {
        cerr << usage(argv[0]);
	return 1;
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
      cfg.verbose += 1;
      break;

    case 'f':
      cfg.flightDataDir = optarg;
      break;

    case 'u':
      cfg.flightDataURL = optarg;
      break;

    case 'i':
      {
	int period = atoi(optarg);
	if (period == 0)
	{
	  cerr << "Invalid update interval: " << optarg << endl;
	  cerr << usage(argv[0]);
	  return 1;
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
	  cerr << usage(argv[0]);
	  return 1;
	}
	cfg.position_interval_secs = period;
      }
      break;

    case '?':
      cerr << usage(argv[0]);
      return 1;
      break;
    }
  }

  if ((optind+2) == argc)	// netCDF mode.
  {
    cfg.netCDFinputFile = argv[optind];
    cfg.outputKML = argv[optind+1];
  }

  if ((optind+3) == argc)	// netCDF mode with position output.
  {
    cfg.netCDFinputFile = argv[optind];
    cfg.outputKML = argv[optind+1];
    cfg.outputPositionJSON = argv[optind+2];
  }

  if ((optind+1) == argc)	// netCDF mode, but they left off output file.
  {
    cerr << usage(argv[0]);
    return 1;
  }

  cfg.fillDefaults();

  if (cfg.netCDFinputFile.empty())
  {
    if (! cfg.verifyDatabaseHost())
    {
      cerr << "Database host must be set, either with -h or PGHOST." << endl;
      return 1;
    }
    if (! cfg.onboard && cfg.platform.empty())
    {
      cerr << "Platform must be set with -p to derive database name "
	   << "when not onboard." << endl;
      cerr << usage(argv[0]);
      return 1;
    }
  }
  return 0;
}


namespace
{
  std::vector<std::string> startup_args;
}

void
ACTrackDriver::
store_args(int argc, char* argv[])
{
  startup_args.clear();
  for (int i = 0; i < argc; ++i)
  {
    startup_args.push_back(argv[i]);
  }
}


void
handle_alarm(int sig)
{
  // Nothing to do but restart from scratch.
  vector<char *> args;
  for (unsigned int i = 0; i < startup_args.size(); ++i)
  {
    args.push_back((char*)(startup_args[i].c_str()));
  }
  args.push_back(0);
  std::cerr << "\n*** Restarting on alarm signal... ***\n" << std::endl;
  execvp(args[0], &(args[0]));
  exit(1);
}

void
install_alarm_handler()
{
  struct sigaction sa;
  struct sigaction oldsa;
  sigset_t mask;

  sa.sa_handler = handle_alarm;
  sigemptyset(&mask);
  sa.sa_mask = mask;
  sa.sa_flags = 0;
  sa.sa_restorer = 0;
  sigaction(SIGALRM, &sa, &oldsa);
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



void
ACTrackDriver::
loadTrackNetCDF(const std::string& ncpath)
{
  ncTrack nct;
  nct.setConfig(cfg);
  nct.updateTrack(track, ncpath);
}


void
ACTrackDriver::
loadTrackDatabase()
{
  // Make sure the database has been updated with the current config before
  // attempting to update the track.
  db.setConfig(cfg);
  db.updateTrack(track);
}


void
ACTrackDriver::
loadTrack()
{
  if (cfg.netCDFinputFile.length() > 0)
  {
    loadTrackNetCDF(cfg.netCDFinputFile);
  }
  else
  {
    loadTrackDatabase();
  }
  if (cfg.showstats)
  {
    // Use a default path to get statistics about the track.
    TrackPath path(track);
    path.generate();
    PathStats stats = path.getStats();
    std::cout << "Track loaded: " << stats << "\n";
  }
}


int
ACTrackDriver::
parseConfig(int argc, char *argv[])
{
  int rc = 0;

  store_args(argc, argv);
  if (argc == 1 || strstr(argv[1], "usage") || strstr(argv[1], "help"))
  {
    cerr << usage(argv[0]);
    return 1;
  }

  if (argc > 1)
  {
    if ((rc = parseRunstring(argc, argv)) != 0)
      return rc;
  }

  return rc;
}


void
ACTrackDriver::
setConfig(Config& config)
{
  cfg = config;
  if (cfg.verbose)
  {
    cerr << "Configuration:\n";
    cerr << cfg.dump();
  }

  kml.setConfig(cfg);
  osm.setConfig(cfg);

  if (cfg.realtimeMode())
  {
    install_alarm_handler();
  }
}


/* -------------------------------------------------------------------- */
int
ACTrackDriver::
main(int argc, char *argv[])
{
  int rc = parseConfig(argc, argv);
  if (rc != 0)
  {
    return rc;
  }
  setConfig(cfg);

  return mainLoop();
}



int
ACTrackDriver::
mainLoop()
{
  ptime last_kml = from_time_t(0);
  ptime last_json = from_time_t(0);
  while (1)
  {
    track.clearStatus();
    loadTrack();

    while (cfg.realtimeMode() && !track.ok())
    {
      sleep(3);
      track.clearStatus();
      loadTrack();
    }

    if (!track.ok())
    {
      if (cfg.run_once)
	return 1;
      if (cfg.check)
	return StatusCheck(false, track.statusMessage);
    }

    // It is imperative that this loop not hang up, so use an alarm() to
    // kill the process if the loop gets stuck.  I suspect that sometimes
    // writing the KML file over NFS can get hung up, in which case it's
    // better to kill the process and restart.  It's more heavy-handed than
    // should be necessary, but oh well. The alarm() and sleep() call are
    // not mixed in case sleep() uses alarm().  Someday it might be also
    // necessary to re-exec the process in the signal handler, but this is
    // a start.  The initial load of a large track can take a while, so
    // this is a balance between minimizing gaps without restarting
    // prematurely.
    if (cfg.realtimeMode())
    {
      alarm(60);
    }

    ptime now = second_clock::universal_time();
    kml.setTrack(&track);

    if (cfg.check)
    {
      std::ostringstream status;
      bool ok = true;

      if (!cfg.outputKML.empty())
	ok = kml.appendStatus(status, cfg.outputKML) && ok;
      if (!cfg.outputAnimatedKML.empty())
	ok = kml.appendStatus(status, cfg.outputAnimatedKML) && ok;
      if (!cfg.outputPositionKML.empty())
	ok = kml.appendStatus(status, cfg.outputPositionKML) && ok;
      if (!cfg.outputPositionJSON.empty())
	ok = kml.appendStatus(status, cfg.outputPositionJSON, 
			      2*cfg.position_interval_secs) && ok;
      return StatusCheck(ok, status.str());
    }

    if (now - last_kml >= seconds(cfg.update_interval_secs))
    {
      last_kml = now;
      bool rewrite = track.updated() || cfg.run_once;

      // Avoid writing a new track file if the track has not changed, and
      // do not erase an old track until there are points in a new one.
      if (cfg.outputKML.length())
      {
	if (rewrite || !kml.checkFile(cfg.outputKML))
	  kml.WriteGoogleEarthKML(cfg.outputKML);
      }
      if (cfg.outputAnimatedKML.length())
      {
	if (rewrite || !kml.checkFile(cfg.outputAnimatedKML))
	  kml.WriteGoogleEarthAnimatedKML(cfg.outputAnimatedKML);
      }
      if (cfg.outputPositionKML.length())
      {
	if (rewrite || !kml.checkFile(cfg.outputPositionKML))
	  kml.WriteCurrentPositionKML(cfg.outputPositionKML);
      }
      if (cfg.verbose && !rewrite)
      {
	cerr << "Track has not changed, existing KML files not rewritten."
	     << endl;
      }
    }

    if (now - last_json >= seconds(cfg.position_interval_secs))
    {
      last_json = now;
      if (cfg.outputPositionJSON.length())
      {
	osm.writePositionJSON(track, cfg.outputPositionJSON);
      }
    }

    if (! cfg.realtimeMode())
      break;

    // Sleep until the next time for an update.
    ptime next = std::min(last_kml + seconds(cfg.update_interval_secs),
			  last_json + seconds(cfg.position_interval_secs));
    int delay = (next - now).total_seconds();
    if (cfg.verbose)
    {
      cerr << now << ": sleeping for " << delay << " seconds..."
	   << endl;
    }
    alarm(0);
    if (delay > 0)
      sleep(delay);
  }

  return 0;
}

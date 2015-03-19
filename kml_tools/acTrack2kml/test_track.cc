
#include "gtest/gtest.h"

#include "AircraftTrack.hh"
#include "TrackPath.hh"
#include "osm.hh"
#include "angle.hh"

using namespace boost::posix_time;


AircraftTrack
make_track(ptime first, int npoints, 
	   PathPoint ll=PathPoint(40, -108),
	   float scale=0.01)
{
  AircraftTrack track;
  int timestep = 2;

  for (int i = 0; i < npoints; ++i)
  {
    track.date.push_back(first + seconds(i*timestep));
    track.lat.push_back(ll.lat + i*scale);
    track.lon.push_back(ll.lon + i*scale);
    track.alt.push_back(5280 + i*10);
    track.thdg.push_back(i*0.5);
    track.tas.push_back(i*bool(i < npoints/2) + 
			(npoints - i)*bool(i >= npoints/2));
  }
  return track;
}

class AngleTest : public ::testing::Test
{
protected:

  void SetUp()
  {
  }

  void TearDown()
  {
  }

};


typedef angle<float> anglef;
typedef angle<float, radians<float> > angler;

TEST_F (AngleTest, Default)
{
  EXPECT_EQ (anglef().value(), 0);
  EXPECT_EQ (anglef(365).value(), 5);
  EXPECT_EQ (anglef(725).value(), 5);
  EXPECT_EQ (anglef(-360).value(), 0);
  EXPECT_EQ (anglef(-0).value(), 0);
  EXPECT_EQ (angle_difference(anglef(355), anglef(5)), 10);
  EXPECT_EQ (angle_difference(anglef(5), anglef(355)), -10);
  EXPECT_EQ (angle_difference(anglef(20), anglef(35)), 15);
  EXPECT_EQ (angle_difference(anglef(35), anglef(20)), -15);
  EXPECT_EQ (angle_difference(anglef(0), anglef(360)), 0);
  EXPECT_EQ (angle_difference(anglef(345), anglef(350)), 5);
  EXPECT_EQ (angle_difference(anglef(345), anglef(180)), -165);

  EXPECT_FLOAT_EQ (anglef(180).convert<angler>().value(), M_PI);
}


TEST_F (AngleTest, FormatAngle)
{
  EXPECT_EQ(anglef(359.999).format("%.1f"), "0.0");
  EXPECT_EQ(anglef(0.1).format("%.1f"), "0.1");
  EXPECT_EQ(anglef(-0.1).format("%.1f"), "359.9");
}

TEST_F (AngleTest, lessThan)
{
  EXPECT_EQ(anglef(5) < anglef(10), true);
  EXPECT_EQ(anglef(10) < anglef(5), false);
  EXPECT_EQ(anglef(5) < anglef(355), false);
  EXPECT_EQ(anglef(355) < anglef(10), true);
}



TEST (TrackTest, MissingValue)
{
  AircraftTrack track;

  EXPECT_EQ(track.missing_value, -32767);
  EXPECT_EQ(AircraftTrack::missing_value, -32767);
}

TEST (TrackTest, findTime)
{
  ptime one(time_from_string("2014-01-29 00:02:10"));
  ptime two = one + seconds(2);
  AircraftTrack track = make_track(one, 2);

  EXPECT_EQ(track.npoints(), 2);
  EXPECT_EQ(track.findTime(one), 0);
  EXPECT_EQ(track.findTime(one, true), 1);
  EXPECT_EQ(track.findTime(two), 1);
  EXPECT_EQ(track.findTime(two, true), 2);
  EXPECT_EQ(track.findTime(time_from_string("2014-01-29 00:02:11")), 0);
  EXPECT_EQ(track.findTime(time_from_string("2014-01-29 00:02:11"), true), 1);
  EXPECT_EQ(track.findTime(time_from_string("2014-01-29 00:02:09")), -1);
  EXPECT_EQ(track.findTime(time_from_string("2014-01-29 00:02:09"), true), -1);
  EXPECT_EQ(track.findTime(time_from_string("2014-01-29 00:02:13")), 1);
  EXPECT_EQ(track.findTime(time_from_string("2014-01-29 00:02:13"), true), 2);
}

TEST (TrackPath, pathTest)
{
  ptime one(time_from_string("2014-01-29 00:02:10"));
  AircraftTrack track = make_track(one, 720);

  TrackPath path(track);
  EXPECT_EQ(path.begin, 0);
  EXPECT_EQ(path.end, 720);

  // TAS is a unit ramp, so clipping should take off the same number from
  // each end.
  path.clipTAS(5);
  EXPECT_EQ(path.begin, 5);
  EXPECT_EQ(path.end, 720-5+1);
  
  // Going even further should leave an empty window.
  path.clipTAS(500);
  EXPECT_EQ(path.begin, 0);
  EXPECT_EQ(path.end, 0);

  // Paths generated on an empty window should be empty.
  path.generateByTimeStep(1);
  EXPECT_EQ(path.npoints(), 0);
  path.generateByHeading(0.1);
  EXPECT_EQ(path.npoints(), 0);

  // Reset the clipping.
  path.setClipping();
  EXPECT_EQ(path.begin, 0);
  EXPECT_EQ(path.end, 720);

  // Clipping by times not in the track or out of order should result in an
  // empty path.
  path.clipTimes(one - seconds(60), one - seconds(201));
  EXPECT_EQ(path.begin, 0);
  EXPECT_EQ(path.end, 0);
  path.generateByPointStep(path.track().npoints());
  EXPECT_EQ(path.npoints(), 0);

  // Generating with a zero time interval should include all the points.
  path.setClipping();
  path.generateByTimeStep(0);
  EXPECT_EQ(path.npoints(), path.track().npoints());
    
  path.clipTimes(one + seconds(10), one + seconds(201));
  EXPECT_EQ(path.begin, 5);
  EXPECT_EQ(path.end, 100+1);

  // A path of points every 5 seconds inside the current clipping window
  // should have 95 points * 2 seconds / 5 seconds ==> 38 points + endpoint.
  path.generateByTimeStep(5);
  EXPECT_EQ(path.npoints(), 39);
  EXPECT_EQ(path.path[0], 5);
  EXPECT_EQ(path.lastIndex(), 100);

  // A path of points for a 0.5 degree heading difference should contain
  // every point, and clipping should not have changed.
  EXPECT_EQ(path.begin, 5);
  EXPECT_EQ(path.end, 100+1);
  path.generateByHeading(0.5);
  EXPECT_EQ(path.npoints(), 96);
  EXPECT_EQ(path.path[0], 5);
  EXPECT_EQ(path.lastIndex(), 100);

  // Whereas a path of points for a 2 degree heading difference should
  // contain every fourth point, and clipping should not have changed.
  EXPECT_EQ(path.begin, 5);
  EXPECT_EQ(path.end, 100+1);
  path.generateByHeading(2.0);
  EXPECT_EQ(path.npoints(), 25);
  EXPECT_EQ(path.path[0], 5);
  EXPECT_EQ(path.path[10], 5+10*4);
  EXPECT_EQ(path.lastIndex(), 100);
}


TEST (TrackPath, addPoint)
{
  ptime one(time_from_string("2014-01-29 00:02:10"));
  AircraftTrack track = make_track(one, 720);

  TrackPath path(track);
  path.setClipping(0, 360);
  EXPECT_EQ(path.npoints(), 0);
  path.addPoint(0);
  EXPECT_EQ(path.npoints(), 1);
  EXPECT_EQ(path.path[0], 0);
  path.addPoint(10);
  EXPECT_EQ(path.npoints(), 2);
  EXPECT_EQ(path.path[1], 10);
  path.addPoint(5);
  EXPECT_EQ(path.npoints(), 3);
  EXPECT_EQ(path.path[1], 5);
  EXPECT_EQ(path.path[2], 10);
  path.addPoint(15);
  EXPECT_EQ(path.npoints(), 4);
  EXPECT_EQ(path.path[1], 5);
  EXPECT_EQ(path.path[2], 10);
  EXPECT_EQ(path.path[3], 15);

  // No change because point already in path.
  path.addPoint(10);
  EXPECT_EQ(path.npoints(), 4);
  EXPECT_EQ(path.path[1], 5);
  EXPECT_EQ(path.path[2], 10);
  EXPECT_EQ(path.path[3], 15);

  // No change because of clipping.
  path.addPoint(360);
  EXPECT_EQ(path.npoints(), 4);
  EXPECT_EQ(path.path[1], 5);
  EXPECT_EQ(path.path[2], 10);
  EXPECT_EQ(path.path[3], 15);
}


TEST (TrackPath, specifierTest)
{
  TrackPath path;

  EXPECT_EQ(path.getHeadingStep(), 0);
  EXPECT_EQ(path.getTimeStep(), 0);
  EXPECT_EQ(path.getMethod(), "");

  path.setSpecifier("headingstep:0.5");
  EXPECT_EQ(path.getHeadingStep(), 0.5);
  EXPECT_EQ(path.getMethod(), "headingstep");

  path.setSpecifier("timestep:22");
  EXPECT_EQ(path.getTimeStep(), 22);
  EXPECT_EQ(path.getMethod(), "timestep");
}

TEST (TrackPath, mergePath)
{
  ptime one(time_from_string("2014-01-29 00:02:10"));
  AircraftTrack track = make_track(one, 720);

  TrackPath lines(track);
  lines.clipTimes(one + seconds(10), one + seconds(20));
  EXPECT_EQ(lines.begin, 5);
  EXPECT_EQ(lines.end, 10+1);
  lines.generateByHeading(2.0);
  // At this point lines contains: { 5, 9, 10 }
  
  TrackPath labels(track);
  labels.addPoint(4);
  labels.addPoint(6);
  labels.addPoint(8);
  labels.addPoint(9);
  labels.addPoint(11);

  // After merging with labels, lines should contain: { 5, 6, 8, 9, 10 }
  lines.mergePath(labels);
  EXPECT_EQ(lines.begin, 5);
  EXPECT_EQ(lines.end, 10+1);
  EXPECT_EQ(lines.npoints(), 5);
  EXPECT_EQ(lines.path[0], 5);
  EXPECT_EQ(lines.path[1], 6);
  EXPECT_EQ(lines.path[2], 8);
  EXPECT_EQ(lines.path[3], 9);
  EXPECT_EQ(lines.path[4], 10);
}


TEST (TrackPath, stats)
{
  ptime one(time_from_string("2014-01-29 00:02:10"));
  AircraftTrack track = make_track(one, 720);

  TrackPath lines(track);
  lines.generate();

  PathStats stats = lines.getStats();
  EXPECT_EQ(stats.npoints_track, 720);
  EXPECT_EQ(stats.npoints_path, 720);
  EXPECT_EQ(stats.path_duration, time_duration(seconds(2*719)));
  PathRegion bbox = stats.bounding_box;
  EXPECT_FLOAT_EQ(40, bbox.sw.lat);
  EXPECT_FLOAT_EQ(-108, bbox.sw.lon);
  EXPECT_FLOAT_EQ(40+(719*0.01), bbox.ne.lat);
  EXPECT_FLOAT_EQ(-108+(719*0.01), bbox.ne.lon);
}


TEST (TrackPath, statsIDL)
{
  ptime one(time_from_string("2014-01-29 00:02:10"));
  AircraftTrack track = make_track(one, 500, PathPoint(-25, 170), 0.1);

  TrackPath lines(track);
  lines.generate();

  PathStats stats = lines.getStats();
  EXPECT_EQ(stats.npoints_track, 500);
  EXPECT_EQ(stats.npoints_path, 500);
  EXPECT_EQ(stats.path_duration, time_duration(seconds(2*499)));
  PathRegion bbox = stats.bounding_box;
  EXPECT_FLOAT_EQ(-25, bbox.sw.lat);
  EXPECT_FLOAT_EQ(170, bbox.sw.lon);
  EXPECT_FLOAT_EQ(24.9, bbox.ne.lat);
  EXPECT_FLOAT_EQ(-140.1, bbox.ne.lon);
}


TEST (TrackTest, getHeading)
{
  AircraftTrack track;
  double thdg;

  // Check that a valid heading gets returned.
  track.thdg.push_back(45.0);
  track.lat.push_back(0);
  track.lon.push_back(0);
  track.alt.push_back(100.0);
  track.date.push_back(time_from_string("2014-01-29 00:02:10"));
  EXPECT_EQ(track.getHeading(0, thdg), true);
  EXPECT_EQ(thdg, 45.0);

  // Cannot get heading from only one point.
  track.thdg[0] = track.missing_value;
  thdg = 10;
  EXPECT_EQ(track.getHeading(0, thdg), false);
  EXPECT_EQ(thdg, 10);

  track.thdg.push_back(track.missing_value);
  track.lat.push_back(1);
  track.lon.push_back(0);
  track.alt.push_back(123.4);
  track.date.push_back(time_from_string("2014-01-29 00:02:11"));

  EXPECT_EQ(track.getHeading(0, thdg), true);
  EXPECT_EQ(thdg, 0.0);
  thdg = 9;
  EXPECT_EQ(track.getHeading(1, thdg), true);
  EXPECT_EQ(thdg, 0.0);

  track.lat[1] = -0.5;
  EXPECT_EQ(track.getHeading(0, thdg), true);
  EXPECT_EQ(thdg, 180.0);

  OSM osm;

  std::string json = osm.formatPositionJSON(track);
  EXPECT_EQ(json,
	    "{\"timestamp\":\"2014-01-29 00:02:11\",\"alt\":\"123.4\","
	    "\"lat\":\"-0.500000\","
	    "\"head\":\"180.0\",\"declination\":\"0\",\"lon\":\"0.000000\"}");

}


#define FLOAT_ANGLE(A, B) \
  do { angle<float> a(A); EXPECT_EQ(a.value(), B); } while (0)

#define INT_ANGLE(A, B) \
  do { angle<int> a(A); EXPECT_EQ(a.value(), B); } while (0)


TEST(NormalizeTest, testNormalize)
{
  FLOAT_ANGLE(360.0, 0.0);
  FLOAT_ANGLE(0.0, 0.0);
  FLOAT_ANGLE(-0.0, 0.0);
  FLOAT_ANGLE(-1.0, 359.0);

  INT_ANGLE(45, 45);
  INT_ANGLE(0, 0);
  INT_ANGLE(360, 0);
  INT_ANGLE(359, 359);
  INT_ANGLE(-45, 315);
}

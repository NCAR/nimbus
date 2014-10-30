
#include "gtest/gtest.h"

#include "AircraftTrack.hh"
#include "osm.hh"
#include "angle.hh"

using namespace boost::posix_time;

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

TEST (TrackTest, MissingValue)
{
  AircraftTrack track;

  EXPECT_EQ(track.missing_value, -32767);
  EXPECT_EQ(AircraftTrack::missing_value, -32767);
}

TEST (TrackTest, getHeading)
{
  AircraftTrack track;
  float thdg;

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

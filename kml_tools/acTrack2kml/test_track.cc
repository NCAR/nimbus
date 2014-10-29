
#include "gtest/gtest.h"

#include "AircraftTrack.hh"
#include "angle.hh"

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


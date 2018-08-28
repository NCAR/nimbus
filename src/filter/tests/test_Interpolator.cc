
#include "gtest/gtest.h"

#include "Interpolator.h"
#include <math.h>

namespace
{
  double x[] = { 0, 1, 2, 3, 4, 5 };
  double y[] = { 0, 1, 2, 3, 4, 5 };
}

class InterpolatorTest : public ::testing::Test
{
protected:

  Interpolator interp;

  void SetUp()
  {
    interp.setup(x, y, sizeof(x)/sizeof(x[0]));
  }

  void TearDown()
  {
  }

};

TEST_F (InterpolatorTest, InterpolatorSetup)
{
  EXPECT_EQ (interp.outOfBounds(), false);
  EXPECT_EQ (&interp.x(0), x);
  EXPECT_EQ (&interp.y(0), y);
  EXPECT_EQ (interp.x(-1), x[5]);
  EXPECT_EQ (interp.y(-1), y[5]);
}

TEST_F (InterpolatorTest, InterpolatorEval)
{
  EXPECT_EQ (interp.eval(1.5), 1.5);
  EXPECT_TRUE (isnan(interp.eval(-1)));
  EXPECT_TRUE (isnan(interp.eval(7)));
}

TEST_F (InterpolatorTest, Extrapolate)
{
  interp.enableExtrapolation(true);
  EXPECT_EQ (interp.eval(1.5), 1.5);
  EXPECT_EQ (interp.outOfBounds(), false);
  EXPECT_EQ (interp.eval(-1), -1);
  EXPECT_EQ (interp.outOfBounds(), true);
  EXPECT_EQ (interp.eval(7), 7);
  EXPECT_EQ (interp.outOfBounds(), true);
  EXPECT_EQ (interp.eval(4.2), 4.2);
  EXPECT_EQ (interp.outOfBounds(), false);
}

TEST_F (InterpolatorTest, Valid)
{
  Interpolator noinit;
  EXPECT_EQ (noinit.isValid(), false);
  EXPECT_EQ (interp.isValid(), true);
}

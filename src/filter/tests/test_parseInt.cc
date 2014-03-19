
#include "gtest/gtest.h"

#include "../parseInt.cc"


TEST(ParseIntTest, ParseInts)
{
  int v;
  EXPECT_EQ(parseInt("12345", v), true);
  EXPECT_EQ(v, 12345);
  EXPECT_EQ(parseInt("12X345", v), false);
  EXPECT_EQ(v, 12345);
  EXPECT_EQ(parseInt("12.345", v), false);
  EXPECT_EQ(v, 12345);
  EXPECT_EQ(parseInt("0054321", v), true);
  EXPECT_EQ(v, 54321);
  EXPECT_EQ(parseInt("ABCD", v), false);
  EXPECT_EQ(v, 54321);
}


#include <gtest/gtest.h>
#include "ellipse.h"

namespace atn {

TEST(EllipseMeasures, ValidEccentricity) {
  atn::Ellipse e1{1.0, 2.0};
  atn::Ellipse e2{2.0, 1.0};
  EXPECT_DOUBLE_EQ(e1.e(), e2.e());
}

} // namespace atn

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#ifndef  _TEST_ELLIPSE_HPP_
#define  _TEST_ELLIPSE_HPP_

#include <gtest/gtest.h>
#include "ellipse.h"

namespace atn {

TEST(EllipseSuite, TestGetters) {
  atn::Ellipse e{2.0, 3.0};
  EXPECT_DOUBLE_EQ(e.a(), 2.0);
  EXPECT_DOUBLE_EQ(e.b(), 3.0);
}

TEST(EllipseSuite, TestFocalLength) {
  atn::Ellipse e1{2.0, 3.0};
  atn::Ellipse e2{3.0, 2.0};
  EXPECT_DOUBLE_EQ(e1.c(), e2.c());
}

TEST(EllipseSuite, TestEccentricity) {
  atn::Ellipse e1{1.0, 2.0};
  atn::Ellipse e2{2.0, 1.0};
  EXPECT_DOUBLE_EQ(e1.e(), e2.e());
}

TEST(EllipseSuite, TestH) {
  atn::Ellipse e{2.0, 3.0};
  EXPECT_DOUBLE_EQ(e.h(), 1.0 / 25.0);
}

} // namespace atn

#endif //  _TEST_ELLIPSE_HPP_
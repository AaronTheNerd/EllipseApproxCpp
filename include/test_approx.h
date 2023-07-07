// Written by Aaron Barge
// Copyright 2022

#ifndef _INCLUDE_TEST_APPROX_H_
#define _INCLUDE_TEST_APPROX_H_

#include <tuple>
#include <vector>

#include "calculator.h"
#include "ellipse.h"

namespace atn {

struct TestInfo {
  constexpr TestInfo(atn::Ellipse ellipse, double expected)
      : ellipse(ellipse), expected(expected) {}

  atn::Ellipse ellipse;
  double expected;
};

double test_approx(const atn::Calculator&, double);

}  // namespace atn

#endif  // _INCLUDE_TEST_APPROX_H_

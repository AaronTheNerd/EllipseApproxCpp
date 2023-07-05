// Written by Aaron Barge
// Copyright 2022

#ifndef _INCLUDE_ELLIPSE_H_
#define _INCLUDE_ELLIPSE_H_

#include <stdint.h>

namespace atn {

struct Ellipse {
  double _a, _b, _major, _minor, _c, _e, _h;
  bool _c_init, _e_init, _h_init;

  Ellipse(double, double);
  double a() const;
  double b() const;
  double c();
  double e();
  double h();
};

}  // namespace atn

#endif  // _INCLUDE_ELLIPSE_H_
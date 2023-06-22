// Written by Aaron Barge
// Copyright 2022

#ifndef _INCLUDE_ELLIPSE_H_
#define _INCLUDE_ELLIPSE_H_

#include <stdint.h>

namespace atn {

struct Ellipse {
  double _a, _b;

  double a() const;
  double b() const;
  double c() const;
  double e() const;
  double h() const;
};

}  // namespace atn

#endif  // _INCLUDE_ELLIPSE_H_
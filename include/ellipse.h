// Written by Aaron Barge
// Copyright 2022

#ifndef _INCLUDE_ELLIPSE_H_
#define _INCLUDE_ELLIPSE_H_

#include <limits>

/**
 * Credit to alexshtf on Github for posting a constexpr version of the sqrt
 * function() https://gist.github.com/alexshtf/eb5128b3e3e143187794
 */

namespace Detail {
double constexpr sqrtNewtonRaphson(double x, double curr, double prev) {
  return curr == prev ? curr
                      : sqrtNewtonRaphson(x, 0.5 * (curr + x / curr), curr);
}
}  // namespace Detail

/*
 * Constexpr version of the square root
 * Return value:
 *	- For a finite and non-negative value of "x", returns an approximation
 *for the square root of "x"
 *   - Otherwise, returns NaN
 */
double constexpr ct_sqrt(double x) {
  return x >= 0 && x < std::numeric_limits<double>::infinity()
             ? Detail::sqrtNewtonRaphson(x, x, 0)
             : std::numeric_limits<double>::quiet_NaN();
}
namespace atn {

struct Ellipse {
  double _a, _b, _major, _minor, _c, _e, _h;

  constexpr Ellipse(double a, double b)
      : _a(a),
        _b(b),
        _major((this->_a > this->_b) ? this->_a : this->_b),
        _minor((this->_a < this->_b) ? this->_a : this->_b),
        _c(ct_sqrt(this->_major * this->_major - this->_minor * this->_minor)),
        _e(this->_c / this->_major),
        _h(((this->_a - this->_b) / (this->_a + this->_b)) *
           ((this->_a - this->_b) / (this->_a + this->_b))) {}

  double a() const;
  double b() const;
  double c() const;
  double e() const;
  double h() const;
};

}  // namespace atn

#endif  // _INCLUDE_ELLIPSE_H_
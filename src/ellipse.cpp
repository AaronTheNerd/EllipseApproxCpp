// Written by Aaron Barge
// Copyright 2022

#include "ellipse.h"

#include <cmath>

#include "utils.h"

atn::Ellipse::Ellipse(double a, double b)
    : _a(a),
      _b(b),
      _major((this->_a > this->_b) ? this->_a : this->_b),
      _minor((this->_a < this->_b) ? this->_a : this->_b),
      _c(0.0),
      _e(0.0),
      _h(0.0),
      _c_init(false),
      _e_init(false),
      _h_init(false) {}

double atn::Ellipse::a() const { return this->_a; }

double atn::Ellipse::b() const { return this->_b; }

double atn::Ellipse::c() {
  if (!this->_c_init) {
    this->_c = sqrt(pow(this->_major, 2) - pow(this->_minor, 2));
    this->_c_init = true;
  }
  return this->_c;
}

double atn::Ellipse::e() {
  if (!this->_e_init) {
    this->_e = this->c() / this->_major;
    this->_e_init = true;
  }
  return this->_e;
}

double atn::Ellipse::h() {
  if (!this->_h_init) {
    this->_h = pow((this->_a - this->_b) / (this->_a + this->_b), 2);
    this->_h_init = true;
  }
  return this->_h;
}

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
      _c(sqrt(pow(this->_major, 2) - pow(this->_minor, 2))),
      _e(this->_c / this->_major),
      _h(pow((this->_a - this->_b) / (this->_a + this->_b), 2)) {}

double atn::Ellipse::a() const { return this->_a; }

double atn::Ellipse::b() const { return this->_b; }

double atn::Ellipse::c() const { return this->_c; }

double atn::Ellipse::e() const { return this->_e; }

double atn::Ellipse::h() const { return this->_h; }

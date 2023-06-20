// Written by Aaron Barge
// Copyright 2022

#include "ellipse.h"

#include <cmath>

#include "utils.h"

double atn::Ellipse::a() const { return this->_a; }

double atn::Ellipse::b() const { return this->_b; }

double atn::Ellipse::c() const {
  double major = (this->_a > this->_b) ? this->_a : this->_b;
  double minor = (this->_a < this->_b) ? this->_a : this->_b;
  return sqrt(pow(major, 2) - pow(minor, 2));
}

double atn::Ellipse::e() const {
  double major = (this->_a > this->_b) ? this->_a : this->_b;
  double minor = (this->_a < this->_b) ? this->_a : this->_b;
  return sqrt(pow(major, 2) - pow(minor, 2)) / major;
}

double atn::Ellipse::h() const {
  return pow((this->_a - this->_b) / (this->_a + this->_b), 2);
}

double atn::Ellipse::circumference(uint8_t precision) const {
  double result = 1.0;
  for (uint8_t n = 1; n <= precision; ++n) {
    result += pow(atn::utils::dfact(2 * n - 1) /
                      (pow(2, n) * atn::utils::factorial(n)),
                  2) *
              pow(this->h(), n) / pow(2 * n - 1, 2);
  }
  return result * M_PI * (this->_a + this->_b);
}

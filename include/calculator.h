// Written by Aaron Barge
// Copyright 2022

#ifndef _INCLUDE_CALCULATOR_H_
#define _INCLUDE_CALCULATOR_H_

#include <stdint.h>
#include <string>
#include <vector>
#include "ellipse.h"

namespace atn {

struct CalcElem {
  enum { VALUE, INPUT, UNARY_OP, BINARY_OP } tag;
  std::string repr;
  union {
    double value;
    double (Ellipse::*input)() const;
    double (*u_op)(double x);
    double (*bi_op)(double x, double y);
  };
  uint8_t args() const;
};

struct Calculator {
  std::vector<CalcElem> calc;
  int8_t validate() const;
  double calculate(const Ellipse&) const;
  std::string to_str() const;
};

struct CalculatorError : public std::exception {
  const char* msg;
  CalculatorError(const char*);
  const char* what() const throw();
};

struct OperatorError : public std::exception {
  int failed_index;
  OperatorError(int);
  const char* what() const throw();
};

std::vector<CalcElem> get_possible_elems();

atn::CalcElem get(const std::vector<atn::CalcElem>&, std::string);

}  // namespace atn

#endif  // _INCLUDE_CALCULATOR_H_
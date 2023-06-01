// Written by Aaron Barge
// Copyright 2022

#ifndef _INCLUDE_CALCULATOR_H_
#define _INCLUDE_CALCULATOR_H_

#include <stdint.h>
#include <string>
#include <vector>
#include "ellipse.h"

#define CALC_ELEM char // Of the form: 0b00TTVVVV
// TT is the type and VVVV is the value
#define CALC_TYPE_MASK   0b00110000
#define CALC_CONST       0b00000000
#define CALC_UNARY       0b00010000
#define CALC_BINARY      0b00100000
#define CALC_INVALID     0b00110000
#define CALC_TYPE_SHIFT  4
#define CALC_VALUE_MASK  0b00001111
#define CALC_PI          0b00000000
#define CALC_1           0b00000001
#define CALC_2           0b00000010
#define CALC_3           0b00000011
#define CALC_4           0b00000100
#define CALC_5           0b00000101
#define CALC_6           0b00000110
#define CALC_7           0b00000111
#define CALC_8           0b00001000
#define CALC_9           0b00001001
#define CALC_10          0b00001010
#define CALC_A           0b00001011
#define CALC_B           0b00001100
#define CALC_C           0b00001101
#define CALC_E           0b00001110
#define CALC_H           0b00001111
#define CALC_SQRT        0b00010000
#define CALC_NEG         0b00010001
#define CALC_INV         0b00010010
#define CALC_ADD         0b00100000
#define CALC_SUB         0b00100001
#define CALC_MUL         0b00100010
#define CALC_DIV         0b00100011
#define CALC_EXP         0b00100100

#define CALC_ARGS(x)     ((x & CALC_TYPE_MASK) >> CALC_TYPE_SHIFT)

namespace atn {
  
struct Calculator {
  std::vector<CALC_ELEM> calc;
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

std::string repr(CALC_ELEM);

std::vector<CALC_ELEM> get_possible_elems();

}  // namespace atn

#endif  // _INCLUDE_CALCULATOR_H_
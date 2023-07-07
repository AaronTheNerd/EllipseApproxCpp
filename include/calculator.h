// Written by Aaron Barge
// Copyright 2022

#ifndef _INCLUDE_CALCULATOR_H_
#define _INCLUDE_CALCULATOR_H_

#include <stdint.h>

#include <string>
#include <vector>

#include "ellipse.h"

#define CALC_ELEM char  // Of the form: 0b0EEEEMVV
// E is the expression, M is the modifier flag, V is the validation
#define CALC std::string

#define CALC_TYPE_MASK 0b00000111
#define CALC_VALIDATION_MASK 0b00000011
#define CALC_CONST 0b00000000
#define CALC_PROPERTY 0b00000100
#define CALC_UNARY 0b00000001
#define CALC_BINARY 0b00000010
#define CALC_VALUE_MASK 0b01111000

#define CALC_PI ((char)0b00000000)
#define CALC_1 ((char)0b00001000)
#define CALC_2 ((char)0b00010000)
#define CALC_3 ((char)0b00011000)
#define CALC_4 ((char)0b00100000)
#define CALC_5 ((char)0b00101000)
#define CALC_6 ((char)0b00110000)
#define CALC_7 ((char)0b00111000)
#define CALC_8 ((char)0b01000000)
#define CALC_9 ((char)0b01001000)
#define CALC_10 ((char)0b01010000)
#define CALC_HALF ((char)0b01011000)
#define CALC_THIRD ((char)0b01100000)
#define CALC_QUARTER ((char)0b01101000)

#define CALC_A ((char)0b00000100)
#define CALC_B ((char)0b00001100)
#define CALC_C ((char)0b00010100)
#define CALC_E ((char)0b00011100)
#define CALC_H ((char)0b00100100)

#define CALC_SQRT ((char)0b00000001)
#define CALC_NEG ((char)0b00001001)
#define CALC_INV ((char)0b00010001)

#define CALC_ADD ((char)0b00000010)
#define CALC_SUB ((char)0b00001010)
#define CALC_MUL ((char)0b00010010)
#define CALC_DIV ((char)0b00011010)
#define CALC_EXP ((char)0b00100010)

#define CALC_INVALID ((char)0b00000011)

#define CALC_ARGS(x) (x & CALC_VALIDATION_MASK)
#define CALC_TYPE(x) (x & CALC_TYPE_MASK)

namespace atn {

struct Calculator {
  CALC calc;
  uint8_t original_size;
  uint8_t max_added;
  Calculator(CALC, uint8_t);
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

}  // namespace atn

#endif  // _INCLUDE_CALCULATOR_H_
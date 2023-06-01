// Written by Aaron Barge
// Copyright 2022

#include "calculator.h"
#include <cmath>
#include <sstream>
#include <stack>
#include <stdexcept>

int8_t atn::Calculator::validate() const {
  int8_t result = 0;
  for (auto it = this->calc.begin(); it != this->calc.end(); ++it) {
      result += (1 - CALC_ARGS(*it));
    if (result < 1) {
      throw atn::CalculatorError("Stack ran out of numbers");
    }
  }
  return result;
}

double atn::Calculator::calculate(const Ellipse& ellipse) const {
  std::stack<double> num_stack;
  double a, b, result;
  int index = 0;
  for (auto it = this->calc.begin(); it != this->calc.end(); ++it, ++index) {
    switch (*it & CALC_TYPE_MASK) {
      case CALC_CONST:
        switch (*it) {
          case CALC_1:
            result = 1.0;
            break;
          case CALC_2:
            result = 2.0;
            break;
          case CALC_3:
            result = 3.0;
            break;
          case CALC_4:
            result = 4.0;
            break;
          case CALC_5:
            result = 5.0;
            break;
          case CALC_6:
            result = 6.0;
            break;
          case CALC_7:
            result = 7.0;
            break;
          case CALC_8:
            result = 8.0;
            break;
          case CALC_9:
            result = 9.0;
            break;
          case CALC_10:
            result = 10.0;
            break;
          case CALC_PI:
            result = M_PI;
            break;
          case CALC_A:
            result = ellipse.a();
            break;
          case CALC_B:
            result = ellipse.b();
            break;
          case CALC_C:
            result = ellipse.c();
            break;
          case CALC_E:
            result = ellipse.e();
            break;
          case CALC_H:
            result = ellipse.h();
            break;
        }
        break;
      case CALC_UNARY:
        a = num_stack.top();
        num_stack.pop();
        try {
          switch (*it) {
            case CALC_SQRT:
              result = sqrt(a);
              break;
            case CALC_NEG:
              result = -a;
              break;
            case CALC_INV:
              result = 1.0 / a;
              break;
          }
        } catch (std::exception& e) {
          throw atn::OperatorError(index);
        }
        break;
      case CALC_BINARY:
        a = num_stack.top();
        num_stack.pop();
        b = num_stack.top();
        num_stack.pop();
        try {
          switch (*it) {
            case CALC_ADD:
              result = a + b;
              break;
            case CALC_SUB:
              result = a - b;
              break;
            case CALC_MUL:
              result = a * b;
              break;
            case CALC_DIV:
              result = a / b;
              break;
            case CALC_EXP:
              result = std::pow(a, b);
              break;
          }
        } catch (std::exception& e) {
          throw atn::OperatorError(index);
        }
        break;
      default:
        throw atn::CalculatorError("Invalid symbol found in stack");
    }
    if (std::isnan(result)) {
      throw atn::OperatorError(index);
    }
    num_stack.push(result);
  }
  if (num_stack.size() != 1)
    throw atn::CalculatorError("Stack could not properly calculate result");
  return num_stack.top();
}

std::string atn::Calculator::to_str() const {
  std::stringstream ss;
  ss << "Calculator([";
  for (auto it = this->calc.begin(); it != this->calc.end(); ++it) {
    if (it != this->calc.begin()) ss << ", ";
    ss << atn::repr(*it);
  }
  ss << "])";
  return ss.str();
}

atn::CalculatorError::CalculatorError(const char* msg) : msg(msg) {}

const char* atn::CalculatorError::what() const throw() { return msg; }

atn::OperatorError::OperatorError(int failed_index)
    : failed_index(failed_index) {}

const char* atn::OperatorError::what() const throw() {
  return ("Operator error at " + std::to_string(this->failed_index)).c_str();
}

std::string atn::repr(CALC_ELEM x) {
  switch (x) {
    case CALC_ADD:
      return "ops.ADD_OP";
    case CALC_SUB:
      return "ops.SUB_OP";
    case CALC_MUL:
      return "ops.MUL_OP";
    case CALC_DIV:
      return "ops.DIV_OP";
    case CALC_EXP:
      return "ops.EXP_OP";
    case CALC_SQRT:
      return "ops.SQRT_OP";
    case CALC_NEG:
      return "ops.NEG_OP";
    case CALC_INV:
      return "ops.INV_OP";
    case CALC_PI:
      return "math.pi";
    case CALC_A:
      return "EllipseProperties.a";
    case CALC_B:
      return "EllipseProperties.b";
    case CALC_C:
      return "EllipseProperties.c";
    case CALC_E:
      return "EllipseProperties.e";
    case CALC_H:
      return "EllipseProperties.h";
    case CALC_1:
      return "1";
    case CALC_2:
      return "2";
    case CALC_3:
      return "3";
    case CALC_4:
      return "4";
    case CALC_5:
      return "5";
    case CALC_6:
      return "6";
    case CALC_7:
      return "7";
    case CALC_8:
      return "8";
    case CALC_9:
      return "9";
    case CALC_10:
      return "10";
  }
}

std::vector<CALC_ELEM> atn::get_possible_elems() {
  std::vector<CALC_ELEM> result{
    CALC_ADD,
    CALC_SUB,
    CALC_MUL,
    CALC_DIV,
    CALC_EXP,
    CALC_SQRT,
    CALC_NEG,
    CALC_INV,
    CALC_PI,
    CALC_A,
    CALC_B,
    /*
    CALC_C,
    CALC_E,
    */
    CALC_H,
    CALC_1,
    CALC_2,
    CALC_3,
    CALC_4,
    CALC_5,
    CALC_6,
    CALC_7,
    CALC_8,
    CALC_9,
    CALC_10
  };
  return result;
}

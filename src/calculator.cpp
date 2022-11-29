// Written by Aaron Barge
// Copyright 2022

#include "calculator.h"
#include <cmath>
#include <sstream>
#include <stack>
#include <stdexcept>

uint8_t atn::CalcElem::args() const {
  switch (this->tag) {
    case CalcElem::UNARY_OP:
      return 1;
    case CalcElem::BINARY_OP:
      return 2;
    default:
      return 0;
  }
}

int8_t atn::Calculator::validate() const {
  int8_t result = 0;
  for (auto it = this->calc.begin(); it != this->calc.end(); ++it) {
    switch (it->tag) {
      case CalcElem::VALUE:
      case CalcElem::INPUT:
        ++result;
        break;
      case CalcElem::BINARY_OP:
        --result;
        break;
    }
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
    switch (it->tag) {
      case CalcElem::VALUE:
        result = it->value;
        break;
      case CalcElem::INPUT:
        result = (ellipse.*(it->input))();
        break;
      case CalcElem::BINARY_OP:
        a = num_stack.top();
        num_stack.pop();
        b = num_stack.top();
        num_stack.pop();
        try {
          result = it->bi_op(a, b);
        } catch (std::exception& e) {
          throw atn::OperatorError(index);
        }
        break;
      case CalcElem::UNARY_OP:
        a = num_stack.top();
        num_stack.pop();
        try {
          result = it->u_op(a);
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
    ss << it->repr;
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

std::vector<atn::CalcElem> atn::get_possible_elems() {
  std::vector<atn::CalcElem> result;
  atn::CalcElem add_op;
  add_op.tag = atn::CalcElem::BINARY_OP;
  add_op.repr = "ops.ADD_OP";
  add_op.bi_op = [](double a, double b) { return a + b; };
  result.push_back(add_op);
  atn::CalcElem sub_op;
  sub_op.tag = atn::CalcElem::BINARY_OP;
  sub_op.repr = "ops.SUB_OP";
  sub_op.bi_op = [](double a, double b) { return a - b; };
  result.push_back(sub_op);
  atn::CalcElem mul_op;
  mul_op.tag = atn::CalcElem::BINARY_OP;
  mul_op.repr = "ops.MUL_OP";
  mul_op.bi_op = [](double a, double b) { return a * b; };
  result.push_back(mul_op);
  atn::CalcElem div_op;
  div_op.tag = atn::CalcElem::BINARY_OP;
  div_op.repr = "ops.DIV_OP";
  div_op.bi_op = [](double a, double b) { return a / b; };
  result.push_back(div_op);
  atn::CalcElem exp_op;
  exp_op.tag = atn::CalcElem::BINARY_OP;
  exp_op.repr = "ops.EXP_OP";
  exp_op.bi_op = &std::pow;
  result.push_back(exp_op);
  atn::CalcElem sqrt_op;
  sqrt_op.tag = atn::CalcElem::UNARY_OP;
  sqrt_op.repr = "ops.SQRT_OP";
  sqrt_op.u_op = &std::sqrt;
  result.push_back(sqrt_op);
  atn::CalcElem neg_op;
  neg_op.tag = atn::CalcElem::UNARY_OP;
  neg_op.repr = "ops.NEG_OP";
  neg_op.u_op = [](double a) { return -a; };
  result.push_back(neg_op);
  atn::CalcElem inv_op;
  inv_op.tag = atn::CalcElem::UNARY_OP;
  inv_op.repr = "ops.INV_OP";
  inv_op.u_op = [](double a) { return 1.0 / a; };
  result.push_back(inv_op);
  atn::CalcElem pi;
  pi.tag = atn::CalcElem::VALUE;
  pi.repr = "math.pi";
  pi.value = M_PI;
  result.push_back(pi);
  atn::CalcElem a_input;
  a_input.tag = atn::CalcElem::INPUT;
  a_input.repr = "EllipseProperties.a";
  a_input.input = &atn::Ellipse::a;
  result.push_back(a_input);
  atn::CalcElem b_input;
  b_input.tag = atn::CalcElem::INPUT;
  b_input.repr = "EllipseProperties.b";
  b_input.input = &atn::Ellipse::b;
  result.push_back(b_input);
  /*
  atn::CalcElem c_input;
  c_input.tag = atn::CalcElem::INPUT;
  c_input.repr = "EllipseProperties.c";
  c_input.input = &atn::Ellipse::c;
  result.push_back(c_input);
  atn::CalcElem e_input;
  e_input.tag = atn::CalcElem::INPUT;
  e_input.repr = "EllipseProperties.e";
  e_input.input = &atn::Ellipse::e;
  result.push_back(e_input);
  */
  atn::CalcElem h_input;
  h_input.tag = atn::CalcElem::INPUT;
  h_input.repr = "EllipseProperties.h";
  h_input.input = &atn::Ellipse::h;
  result.push_back(h_input);
  for (uint8_t i = 1; i <= 10; ++i) {
    atn::CalcElem c;
    c.tag = atn::CalcElem::VALUE;
    c.repr = std::to_string(i);
    c.value = 1.0 * i;
    result.push_back(c);
  }
  return result;
}

atn::CalcElem atn::get(const std::vector<atn::CalcElem>& elems,
                       std::string repr) {
  for (auto it = elems.begin(); it != elems.end(); ++it) {
    if (it->repr == repr) return *it;
  }
  throw atn::CalculatorError("CalcElem could not be found");
}

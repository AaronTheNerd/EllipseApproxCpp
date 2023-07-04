#ifndef _TEST_CALCULATOR_HPP_
#define _TEST_CALCULATOR_HPP_

#include <gtest/gtest.h>

#include <string>
#include <unordered_map>

#include "calculator.h"
#include "ellipse.h"

namespace atn {

struct CalcExpected {
  uint8_t type;
  uint8_t args;
  std::string repr;
};

std::unordered_map<CALC_ELEM, CalcExpected> tests{
    {CALC_PI, {CALC_CONST, 0, "math.pi"}},
    {CALC_1, {CALC_CONST, 0, "1"}},
    {CALC_2, {CALC_CONST, 0, "2"}},
    {CALC_3, {CALC_CONST, 0, "3"}},
    {CALC_4, {CALC_CONST, 0, "4"}},
    {CALC_5, {CALC_CONST, 0, "5"}},
    {CALC_6, {CALC_CONST, 0, "6"}},
    {CALC_7, {CALC_CONST, 0, "7"}},
    {CALC_8, {CALC_CONST, 0, "8"}},
    {CALC_9, {CALC_CONST, 0, "9"}},
    {CALC_10, {CALC_CONST, 0, "10"}},
    {CALC_HALF, {CALC_CONST, 0, "0.5"}},
    {CALC_THIRD, {CALC_CONST, 0, "1/3"}},
    {CALC_QUARTER, {CALC_CONST, 0, "0.25"}},
    {CALC_A, {CALC_PROPERTY, 0, "EllipseProperties.a"}},
    {CALC_B, {CALC_PROPERTY, 0, "EllipseProperties.b"}},
    {CALC_C, {CALC_PROPERTY, 0, "EllipseProperties.c"}},
    {CALC_E, {CALC_PROPERTY, 0, "EllipseProperties.e"}},
    {CALC_H, {CALC_PROPERTY, 0, "EllipseProperties.h"}},
    {CALC_SQRT, {CALC_UNARY, 1, "ops.SQRT_OP"}},
    {CALC_NEG, {CALC_UNARY, 1, "ops.NEG_OP"}},
    {CALC_INV, {CALC_UNARY, 1, "ops.INV_OP"}},
    {CALC_ADD, {CALC_BINARY, 2, "ops.ADD_OP"}},
    {CALC_SUB, {CALC_BINARY, 2, "ops.SUB_OP"}},
    {CALC_MUL, {CALC_BINARY, 2, "ops.MUL_OP"}},
    {CALC_DIV, {CALC_BINARY, 2, "ops.DIV_OP"}},
    {CALC_EXP, {CALC_BINARY, 2, "ops.EXP_OP"}},
    {CALC_INVALID, {CALC_INVALID, 3, ""}}};

TEST(CalculatorSuite, TestUniqueElements) {
  std::vector<CALC_ELEM> all_elems;
  for (auto it = tests.begin(); it != tests.end(); ++it) {
    all_elems.push_back(it->first);
  }
  for (uint8_t i = 0; i < all_elems.size() - 1; ++i) {
    CALC_ELEM elem1 = all_elems[i];
    for (uint8_t j = i + 1; j < all_elems.size(); ++j) {
      CALC_ELEM elem2 = all_elems[j];
      ASSERT_NE(elem1, elem2);
    }
  }
}

TEST(CalculatorSuite, TestType) {
  CALC_ELEM elem;
  uint8_t expected;
  for (auto test : tests) {
    elem = test.first;
    expected = test.second.type;
    ASSERT_EQ(CALC_TYPE(elem), expected);
  }
}

TEST(CalculatorSuite, TestArgs) {
  CALC_ELEM elem;
  uint8_t expected;
  for (auto test : tests) {
    elem = test.first;
    expected = test.second.args;
    ASSERT_EQ(CALC_ARGS(elem), expected);
  }
}

TEST(CalculatorSuite, TestRepr) {
  CALC_ELEM elem;
  std::string expected;
  for (auto test : tests) {
    elem = test.first;
    expected = test.second.repr;
    ASSERT_EQ(atn::repr(elem), expected);
  }
}

TEST(CalculatorSuite, TestDistFormula) {
  atn::Calculator calc{std::string() + CALC_2 + CALC_A + CALC_EXP + CALC_2 +
                           CALC_B + CALC_EXP + CALC_ADD + CALC_SQRT,
                       0};
  ASSERT_DOUBLE_EQ(calc.calculate(atn::Ellipse{3.0, 4.0}), 5.0);
}

}  // namespace atn

#endif  //  _TEST_CALCULATOR_HPP_
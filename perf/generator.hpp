#ifndef _TEST_GENERATION_HPP_
#define _TEST_GENERATION_HPP_

#include <gtest/gtest.h>

#include "calculator.h"
#include "generation.h"

namespace atn {

TEST(GeneratorSuite, PerformanceTest) {
  CALC initial_calc =
      std::string() + CALC_PI + CALC_A + CALC_B + CALC_ADD + CALC_MUL;
  atn::Calculator c{initial_calc, 6};
  CALC all_elems = std::string() + CALC_ADD + CALC_SUB + CALC_MUL + CALC_DIV +
                   CALC_EXP + CALC_SQRT + CALC_NEG + CALC_INV + CALC_PI +
                   CALC_A + CALC_B + CALC_C + CALC_E + CALC_H + CALC_1 +
                   CALC_2 + CALC_3 + CALC_4 + CALC_5 + CALC_6 + CALC_7 +
                   CALC_8 + CALC_9 + CALC_10 + CALC_HALF + CALC_THIRD +
                   CALC_QUARTER;
  atn::Generator gen(0, 1, c, all_elems, true);
  gen.run();
}

}  // namespace atn

#endif  //  _TEST_GENERATION_HPP_
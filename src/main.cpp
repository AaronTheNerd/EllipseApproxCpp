// Written by Aaron Barge
// Copyright 2022

#include <string>

#include "calculator.h"
#include "generation.h"
#include "interfaces.h"
#include "utils.h"

int main(int argc, char** argv) {
  CALC initial_calc =
      std::string() + CALC_PI + CALC_A + CALC_B + CALC_ADD + CALC_MUL;
  atn::Calculator c{initial_calc, 15};
  CALC all_elems = std::string() + CALC_ADD + CALC_SUB + CALC_MUL + CALC_DIV +
                   CALC_EXP + CALC_SQRT + CALC_NEG + CALC_INV + CALC_PI +
                   CALC_A + CALC_B + CALC_H + CALC_1 + CALC_2 + CALC_3 +
                   CALC_4 + CALC_5 + CALC_HALF + CALC_THIRD + CALC_QUARTER;
  atn::Generator gen{atn::utils::random_seed(), 4, c, all_elems, false};
  atn::interface::generation_started(gen);
  gen.run();
}
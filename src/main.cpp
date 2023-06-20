// Written by Aaron Barge
// Copyright 2022

#include <iomanip>
#include <iostream>
#include <string>

#include "calculator.h"
#include "ellipse.h"
#include "generation.h"
#include "interfaces.h"
#include "utils.h"

int main(int argc, char** argv) {
  CALC initial_calc =
      std::string() + CALC_PI + CALC_A + CALC_B + CALC_ADD + CALC_MUL;
  atn::Calculator c{initial_calc, 15};
  atn::Generator gen(atn::utils::random_seed(), c);
  atn::interface::generation_started();
  gen.run();
}
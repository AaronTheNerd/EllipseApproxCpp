// Written by Aaron Barge
// Copyright 2022

#include <iomanip>
#include <iostream>
#include "calculator.h"
#include "ellipse.h"
#include "generation.h"
#include "utils.h"

int main(int argc, char** argv) {
  std::vector<CALC_ELEM> elems = atn::get_possible_elems();
  atn::Calculator c{
      {CALC_PI, CALC_A,
       CALC_B, CALC_ADD,
       CALC_MUL}};
  atn::Generator gen(atn::utils::random_seed(), c);
  gen.run();
}
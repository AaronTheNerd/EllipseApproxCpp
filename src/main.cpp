// Written by Aaron Barge
// Copyright 2022

#include <iomanip>
#include <iostream>
#include "calculator.h"
#include "ellipse.h"
#include "generation.h"
#include "utils.h"

int main(int argc, char** argv) {
  std::vector<atn::CalcElem> elems = atn::get_possible_elems();
  atn::Calculator c{
      {atn::get(elems, "math.pi"), atn::get(elems, "EllipseProperties.a"),
       atn::get(elems, "EllipseProperties.b"), atn::get(elems, "ops.ADD_OP"),
       atn::get(elems, "ops.MUL_OP")}};
  atn::Generator gen(atn::utils::random_seed(), c);
  gen.run();
}
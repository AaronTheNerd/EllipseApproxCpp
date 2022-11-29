// Written by Aaron Barge
// Copyright 2022

#ifndef _INCLUDE_GENERATION_H_
#define _INCLUDE_GENERATION_H_

#include <chrono>
#include <random>
#include <thread>
#include "calculator.h"
#include "test_approx.h"

namespace atn {

struct Generator {
  std::default_random_engine rng;
  atn::Calculator calc;
  std::vector<atn::CalcElem> all_calc_elems;
  bool failed_due_to_error;
  int failed_index;
  atn::TestData data;
  std::chrono::time_point<std::chrono::high_resolution_clock> start;

  Generator(uint64_t, atn::Calculator);
  void run();

 private:
  static std::thread spawn_helper(Generator);
  void gen_approx(bool);
};

}  // namespace atn

#endif  // _INCLUDE_GENERATION_H_
// Written by Aaron Barge
// Copyright 2022

#ifndef _INCLUDE_GENERATION_H_
#define _INCLUDE_GENERATION_H_

#include <chrono>
#include <random>
#include <thread>
#include <set>

#include "calculator.h"
#include "test_approx.h"

namespace atn {

struct LayerState {
  uint8_t curr_index;
  CALC all_elems;
  CALC filtered_elems;
};

struct Generator {
  uint64_t seed;
  std::default_random_engine rng;
  atn::Calculator calc;
  uint8_t thread_count;
  CALC all_calc_elems;
  atn::TestData data;
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::vector<LayerState> layers;
  std::set<CALC> shortest_constants;
  bool silent;
  double epsilon;

  Generator(uint64_t, uint8_t, atn::Calculator, CALC, bool);
  void run();

 private:
  void generate_initial_layers();
  void generate_shortest_constants();
  CALC get_valid_calc_elems(const CALC&, uint8_t, uint8_t) const;
  static std::thread spawn_helper(Generator, CALC);
  void gen_approx_inline();
};

}  // namespace atn

#endif  // _INCLUDE_GENERATION_H_
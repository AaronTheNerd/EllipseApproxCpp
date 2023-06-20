// Written by Aaron Barge
// Copyright 2022

#include "generation.h"
#include <math.h>     // isnan
#include <algorithm>  // std::shuffle
#include <iostream>   // std::cout, std::endl
#include <mutex>      // std::mutex, std::lock_guard
#include "interfaces.h"
#include "utils.h"

#define THREAD_COUNT 4

std::mutex m;
CALC initial_valid_elems;
uint64_t tested_count = 0, submitted_count = 0, error_count = 0;
double curr_best = 500.0;

void register_formula(atn::Calculator calc, std::thread::id id) {
  std::lock_guard<std::mutex> guard(m);
  atn::interface::formula_created(calc, id);
}

void register_error() {
  std::lock_guard<std::mutex> guard(m);
  error_count++;
}

void register_test(std::chrono::time_point<std::chrono::high_resolution_clock> start_time) {
  std::lock_guard<std::mutex> guard(m);
  tested_count++;
  std::chrono::duration<double> dur =
      std::chrono::high_resolution_clock::now() - start_time;
  if (tested_count % 50000 == 0)
    atn::interface::tested(tested_count, submitted_count, error_count,
                          dur.count());
}

void register_submitted_test(double test_result, atn::Calculator calc) {
  std::lock_guard<std::mutex> guard(m);
  if (!isnan(test_result) && (test_result < curr_best || test_result < 100.0)) {
    curr_best = test_result;
    submitted_count++;
    atn::interface::submitted(calc, test_result, submitted_count);
  }
}

atn::Generator::Generator(uint64_t seed, atn::Calculator calc)
    : seed(seed),
      rng(seed),
      calc(calc),
      all_calc_elems(atn::get_possible_elems()),
      failed_due_to_error(false),
      failed_index(-1),
      data(atn::read_test_file("./data/test_data.csv")),
      start(std::chrono::high_resolution_clock::now()),
      layers() {
  std::cout << "- Test Data read" << std::endl;
  this->generate_initial_layers();
}

void atn::Generator::run() {
  std::cout << "- Generation running" << std::endl;
  if (THREAD_COUNT > 1) {
    initial_valid_elems = this->layers[0].filtered_elems;
    std::vector<std::thread> threads;
    std::vector<atn::Generator*> gens;
    auto valid_elems = atn::utils::split_calc(initial_valid_elems, 4);
    std::cout << "- Spawning " << THREAD_COUNT << " threads" << std::endl;
    for (uint8_t i = 0; i < THREAD_COUNT; ++i) {
      atn::Generator* gen = new Generator(*this);
      gens.push_back(gen);
      threads.push_back(atn::Generator::spawn_helper(*gen, valid_elems[i]));
    }
    for (std::thread& t : threads) {
      t.join();
    }
    for (atn::Generator* gen : gens) {
      delete gen;
    }
  } else {
    this->gen_approx_inline();
  }
}

void atn::Generator::generate_initial_layers() {
  for (uint8_t i = 0; i < this->calc.max_added; ++i) {
    std::shuffle(this->all_calc_elems.begin(), this->all_calc_elems.end(), this->rng);
    this->layers.push_back(atn::LayerState{0, this->all_calc_elems, this->all_calc_elems});
  }
}

CALC atn::Generator::get_valid_calc_elems(const CALC& all_elems, uint8_t layer_index) const {
  std::vector<atn::utils::predicate> filters;
  uint8_t validation = this->calc.validate();
  // Filter any operator which requires too many arguments
  filters.push_back(
      [validation](CALC_ELEM x) { return CALC_ARGS(x) <= validation; });

  // If the number stack elements left to have is equal to the number of binary
  // operators needed to complete the formula, filter non binary operators
  if (this->layers.size() - layer_index == validation - 1) {
    filters.push_back([](CALC_ELEM x) { return CALC_ARGS(x) > 1; });
  }
  return atn::utils::filter(all_elems, atn::utils::all(filters));
}

std::thread atn::Generator::spawn_helper(atn::Generator gen, CALC elems) {
  return std::thread(
      [elems](atn::Generator gen) {
        gen.layers[0].filtered_elems = elems;
        std::cout << "Thread: " << std::this_thread::get_id()
                  << " has begun                                         "
                  << std::endl;
        gen.gen_approx_inline();
        std::cout << "Thread: " << std::this_thread::get_id()
                  << " has ended                                         "
                  << std::endl;
      },
      gen);
}

void atn::Generator::gen_approx_inline() {
  uint8_t layer_index = 0;
  int8_t validation;
  double test_result = NAN;

  while (layer_index < this->layers.size()) {

    // Check that no other elements can be added
    if (this->layers[layer_index].curr_index >= this->layers[layer_index].filtered_elems.size()) {
      // Go up a layer
      this->calc.calc[this->calc.original_size + layer_index] = CALC_INVALID;
      layer_index--;
      continue;
    }

    // Add a new element to the calculator
    this->calc.calc[this->calc.original_size + layer_index] = 
      this->layers[layer_index].filtered_elems[this->layers[layer_index].curr_index++];
    //register_formula(this->calc, std::this_thread::get_id());

    // Test the current calculator
    try {
      validation = this->calc.validate();
    } catch (atn::OperatorError& e) {
      // Keep record of failure
      register_error();
      // Go back to when the layer index was setting the operator which failed
      layer_index = e.failed_index - this->calc.original_size;
      uint8_t replace_amount = this->calc.calc.size() - e.failed_index;
      this->calc.calc.replace(e.failed_index, replace_amount, replace_amount, CALC_INVALID);
      continue;
    }

    // Test calculator if appropriate
    if (validation == 1) {
      // Set lock and print testing info
      register_test(this->start);

      // Try to test approximation
      try {
        test_result = atn::test_approx(this->calc, this->data, curr_best);
      } catch (atn::OperatorError& e) {
        // Keep record of failure
        register_error();
        // Go back to when the layer index was setting the operator which failed
        layer_index = e.failed_index - this->calc.original_size;
        uint8_t replace_amount = this->calc.calc.size() - e.failed_index;
        this->calc.calc.replace(e.failed_index, replace_amount, replace_amount, CALC_INVALID);
        continue;
      }

      // Register test result
      register_submitted_test(test_result, this->calc);
    }

    // Prepare to expand formula if possible
    if (layer_index + 1 < this->layers.size()) {
      layer_index++;
      // Apply filters to next layer
      this->layers[layer_index].filtered_elems = this->get_valid_calc_elems(this->layers[layer_index].all_elems, layer_index);
      // Reset index in next layer
      this->layers[layer_index].curr_index = 0;
    } else {
      this->calc.calc[this->calc.original_size + layer_index] = CALC_INVALID;
      continue;
    }
  }
}


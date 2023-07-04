// Written by Aaron Barge
// Copyright 2022

#include "generation.h"

#include <math.h>  // isnan

#include <algorithm>      // std::shuffle, std::copy_if
#include <iostream>       // std::cout, std::endl
#include <mutex>          // std::mutex, std::lock_guard
#include <unordered_map>  // std::unordered_map

#include "interfaces.h"
#include "utils.h"

std::mutex m;
uint64_t tested_count = 0, submitted_count = 0, error_count = 0;
double curr_best = 1000.0;

void register_formula(atn::Calculator calc, std::thread::id id) {
  std::lock_guard<std::mutex> guard(m);
  atn::interface::formula_created(calc, id);
}

void register_error() {
  std::lock_guard<std::mutex> guard(m);
  error_count++;
}

void register_test(
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time,
    bool silent) {
  std::lock_guard<std::mutex> guard(m);
  tested_count++;
  if (silent) return;
  std::chrono::duration<double> dur =
      std::chrono::high_resolution_clock::now() - start_time;
  if (tested_count % 50000 == 0)
    atn::interface::tested(tested_count, submitted_count, error_count,
                           dur.count());
}

void register_submitted_test(double test_result, atn::Calculator calc,
                             double epsilon) {
  std::lock_guard<std::mutex> guard(m);
  if (!isnan(test_result) && test_result + epsilon < curr_best) {
    curr_best = test_result;
    submitted_count++;
    atn::interface::submitted(calc, test_result, submitted_count);
  }
}

atn::Generator::Generator(uint64_t seed, uint8_t thread_count,
                          atn::Calculator calc, CALC all_elems, bool silent)
    : seed(seed),
      rng(seed),
      calc(calc),
      thread_count(thread_count),
      all_calc_elems(all_elems),
      data(atn::read_test_file("./data/test_data.csv")),
      start(std::chrono::high_resolution_clock::now()),
      layers(),
      shortest_constants(),
      silent(silent) {
  if (!this->silent) std::cout << "- Test Data read" << std::endl;
  this->generate_initial_layers();
  this->generate_shortest_constants();
}

void atn::Generator::run() {
  if (!this->silent) std::cout << "- Generation running" << std::endl;
  if (this->thread_count > 1) {
    std::vector<std::thread> threads;
    std::vector<atn::Generator*> gens;
    auto valid_elems =
        atn::utils::split_calc(this->layers[0].filtered_elems, 4);
    if (!this->silent)
      std::cout << "- Spawning " << this->thread_count << " threads"
                << std::endl;
    for (uint8_t i = 0; i < this->thread_count; ++i) {
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
  atn::interface::generation_finished(tested_count);
}

void atn::Generator::generate_initial_layers() {
  for (uint8_t i = 0; i < this->calc.max_added; ++i) {
    std::shuffle(this->all_calc_elems.begin(), this->all_calc_elems.end(),
                 this->rng);
    this->layers.push_back(
        atn::LayerState{0, this->all_calc_elems, this->all_calc_elems});
  }
  // Filter the first layer
  this->layers[0].filtered_elems = this->get_valid_calc_elems(
      this->layers[0].all_elems, 0, this->calc.validate());
}

void atn::Generator::generate_shortest_constants() {
  std::unordered_map<double, CALC> shortest;
  std::vector<CALC_ELEM> constants;
  std::vector<CALC_ELEM> unary_ops;
  std::vector<CALC_ELEM> binary_ops;
  atn::Calculator calc("", 3);
  double value;
  // Fill all arrays
  for (uint8_t i = 0; i < this->all_calc_elems.size(); ++i) {
    CALC_ELEM elem = this->all_calc_elems[i];
    switch (elem & CALC_TYPE_MASK) {
      case CALC_CONST:
        constants.push_back(elem);
        break;
      case CALC_UNARY:
        unary_ops.push_back(elem);
        break;
      case CALC_BINARY:
        binary_ops.push_back(elem);
        break;
    }
  }

  // First add all constants
  for (uint8_t i = 0; i < constants.size(); ++i) {
    CALC_ELEM elem = constants[i];
    calc.calc = std::string() + elem;
    value = calc.calculate(atn::Ellipse{});
    shortest.insert(std::make_pair(value, calc.calc));
  }
  // Calculate with all constants within a unary operator
  for (uint8_t i = 0; i < constants.size(); ++i) {
    for (uint8_t j = 0; j < unary_ops.size(); ++j) {
      calc.calc = std::string() + constants[i] + unary_ops[j];
      value = calc.calculate(atn::Ellipse{});
      if (shortest.find(value) == shortest.end()) {
        shortest.insert(std::make_pair(value, calc.calc));
      }
    }
  }
  // Calculate with all pairs of constants within a binary operator
  for (uint8_t i = 0; i < constants.size(); ++i) {
    for (uint8_t j = 0; j < constants.size(); ++j) {
      for (uint8_t k = 0; k < binary_ops.size(); ++k) {
        calc.calc = std::string() + constants[i] + constants[j] + binary_ops[k];
        value = calc.calculate(atn::Ellipse{});
        if (shortest.find(value) == shortest.end()) {
          shortest.insert(std::make_pair(value, calc.calc));
        }
      }
    }
  }
  // Store all values from shortest expressions
  for (auto i : shortest) {
    this->shortest_constants.insert(i.second);
  }
}

CALC atn::Generator::get_valid_calc_elems(const CALC& all_elems,
                                          uint8_t layer_index,
                                          uint8_t validation) const {
  CALC filtered_elems = "";
  bool only_use_binary = this->layers.size() - layer_index == validation - 1u;
  bool only_use_unary =
      layer_index + 1u == this->layers.size() && validation == 1u;
  uint8_t last_index = this->calc.original_size + layer_index - 1;
  bool check_for_unary_const_expr =
      this->calc.calc.size() > last_index &&
      CALC_TYPE(this->calc.calc[last_index]) == CALC_CONST;
  bool check_for_binary_const_expr =
      this->calc.calc.size() > last_index &&
      this->calc.calc.size() > last_index - 1u &&
      CALC_TYPE(this->calc.calc[last_index]) == CALC_CONST &&
      CALC_TYPE(this->calc.calc[last_index - 1]) == CALC_CONST;

  std::copy_if(
      all_elems.begin(), all_elems.end(), std::back_inserter(filtered_elems),
      [validation, only_use_binary, only_use_unary, check_for_unary_const_expr,
       check_for_binary_const_expr, this](CALC_ELEM elem) {
        // Filter out all operators which don't have enough inputs
        if (validation < CALC_ARGS(elem)) return false;

        // Filter out all non-binary elements if only binary should be
        // used
        if (only_use_binary && CALC_ARGS(elem) < 2) return false;

        // Filter out all non-unary elements if only unary should be
        // used
        if (only_use_unary && CALC_ARGS(elem) != 1) return false;

        // Filter out all unary operators which would result in a constant
        // expression longer than necessary
        if (check_for_unary_const_expr && CALC_ARGS(elem) == 1) {
          CALC expr(this->calc.calc, this->calc.calc.size() - 1, 1);
          expr += elem;
          if (this->shortest_constants.find(expr) ==
              this->shortest_constants.end()) {
            return false;
          }
        }

        // Filter out all binary operators which would result in a constant
        // expression longer than necessary
        if (check_for_binary_const_expr && CALC_ARGS(elem) == 2) {
          CALC expr(this->calc.calc, this->calc.calc.size() - 2, 2);
          expr += elem;
          if (this->shortest_constants.find(expr) ==
              this->shortest_constants.end()) {
            return false;
          }
        }

        return true;
      });
  return filtered_elems;
}

std::thread atn::Generator::spawn_helper(atn::Generator gen, CALC elems) {
  bool silent = gen.silent;
  return std::thread(
      [elems, silent](atn::Generator gen) {
        gen.layers[0].filtered_elems = elems;
        if (!silent)
          std::cout << "Thread: " << std::this_thread::get_id()
                    << " has begun                                         "
                    << std::endl;
        gen.gen_approx_inline();
        if (!silent)
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
    if (this->layers[layer_index].curr_index >=
        this->layers[layer_index].filtered_elems.size()) {
      // Go up a layer
      this->calc.calc[this->calc.original_size + layer_index] = CALC_INVALID;
      layer_index--;
      continue;
    }

    // Add a new element to the calculator
    this->calc.calc[this->calc.original_size + layer_index] =
        this->layers[layer_index]
            .filtered_elems[this->layers[layer_index].curr_index++];
    // register_formula(this->calc, std::this_thread::get_id());

    // Test the current calculator
    try {
      validation = this->calc.validate();
    } catch (atn::OperatorError& e) {
      // Keep record of failure
      register_error();
      // Go back to when the layer index was setting the operator which failed
      layer_index = e.failed_index - this->calc.original_size;
      uint8_t replace_amount = this->calc.calc.size() - e.failed_index;
      this->calc.calc.replace(e.failed_index, replace_amount, replace_amount,
                              CALC_INVALID);
      continue;
    }

    // Test calculator if appropriate
    if (validation == 1) {
      // Set lock and print testing info
      register_test(this->start, this->silent);

      // Try to test approximation
      try {
        test_result = atn::test_approx(this->calc, this->data, curr_best);
      } catch (atn::OperatorError& e) {
        // Keep record of failure
        register_error();
        // Go back to when the layer index was setting the operator which failed
        layer_index = e.failed_index - this->calc.original_size;
        uint8_t replace_amount = this->calc.calc.size() - e.failed_index;
        this->calc.calc.replace(e.failed_index, replace_amount, replace_amount,
                                CALC_INVALID);
        continue;
      }

      // Register test result
      register_submitted_test(test_result, this->calc, this->epsilon);
    }

    // Prepare to expand formula if possible
    if (layer_index + 1u < this->layers.size()) {
      layer_index++;
      // Apply filters to next layer
      this->layers[layer_index].filtered_elems = this->get_valid_calc_elems(
          this->layers[layer_index].all_elems, layer_index, validation);
      // Reset index in next layer
      this->layers[layer_index].curr_index = 0;
    } else {
      this->calc.calc[this->calc.original_size + layer_index] = CALC_INVALID;
      continue;
    }
  }
}

// Written by Aaron Barge
// Copyright 2022

#include "generation.h"
#include <math.h>     // isnan
#include <algorithm>  // std::shuffle
#include <iostream>   // std::cout, std::endl
#include <mutex>      // std::mutex, std::lock_guard
#include "interfaces.h"
#include "utils.h"

#define MAX_STACK_SIZE 15
#define THREAD_COUNT 4

std::mutex m;
std::vector<atn::CalcElem> initial_valid_elems;
uint64_t tested_count = 0, submitted_count = 0, error_count = 0;
double curr_best = 10000.0;

atn::Generator::Generator(uint64_t seed, atn::Calculator calc)
    : rng(seed),
      calc(calc),
      all_calc_elems(atn::get_possible_elems()),
      failed_due_to_error(false),
      failed_index(-1),
      data(atn::read_test_file("./data/test_data.csv")),
      start(std::chrono::high_resolution_clock::now()) {}

void atn::Generator::run() {
  this->gen_approx(true);
}

std::thread atn::Generator::spawn_helper(atn::Generator gen) {
  return std::thread(
      [](atn::Generator gen) {
        atn::CalcElem current_elem;
        while (true) {
          {
            std::lock_guard<std::mutex> guard(m);
            if (initial_valid_elems.size() == 0) {
              std::cout << "Thread: " << std::this_thread::get_id()
                        << " has ended                                         "
                        << std::endl;
              return;
            }
            current_elem = initial_valid_elems.back();
            initial_valid_elems.pop_back();
          }
          std::cout << "Thread: " << std::this_thread::get_id() << " has begun "
                    << current_elem.repr << "                 " << std::endl;
          gen.calc.calc.emplace_back(current_elem);
          gen.failed_due_to_error = false;
          gen.failed_index = -1;
          gen.gen_approx(false);
          gen.calc.calc.pop_back();
        }
      },
      gen);
}

void atn::Generator::gen_approx(bool first_run) {
  int8_t validation = this->calc.validate();
  double test_result = NAN;

  if (this->failed_due_to_error) {
    if (this->failed_index == -1) {
      try {
        test_result = atn::test_approx(this->calc, this->data, curr_best);
      } catch (atn::CalculatorError& e) {
        this->failed_due_to_error = false;
        this->failed_index = -1;
      } catch (std::exception& e) {
        return;
      }
    } else if (this->calc.calc.size() <= this->failed_index) {
      this->failed_due_to_error = false;
      this->failed_index = -1;
    } else {
      return;
    }

  }
  
  if (!this->failed_due_to_error && validation == 1) {
    // Approx tested
    {
      std::lock_guard<std::mutex> guard(m);
      tested_count++;
      std::chrono::duration<double> dur =
          std::chrono::high_resolution_clock::now() - this->start;
      if (tested_count % 10000 == 0)
        atn::interface::tested(tested_count, submitted_count, error_count,
                               dur.count());
    }
    if (isnan(test_result)) {
      try {
        test_result = atn::test_approx(this->calc, this->data, curr_best);
      } catch (atn::OperatorError& e) {
        // Error
        {
          std::lock_guard<std::mutex> guard(m);
          error_count++;
        }
        this->failed_due_to_error = true;
        this->failed_index = e.failed_index;
        return;
      } catch (std::exception& e) {
        // Error
        {
          std::lock_guard<std::mutex> guard(m);
          error_count++;
        }
        this->failed_due_to_error = true;
        this->failed_index = -1;
        return;
      }
    }

    {
      std::lock_guard<std::mutex> guard(m);
      if (!isnan(test_result) && (test_result < curr_best || test_result < 100.0)) {
        curr_best = test_result;
        submitted_count++;
        atn::interface::submitted(this->calc, test_result, submitted_count);
      }
    }
  }

  if (this->calc.calc.size() >= MAX_STACK_SIZE ||
      MAX_STACK_SIZE - this->calc.calc.size() < validation - 1) {
    return;
  }

  std::vector<atn::utils::predicate> filters;

  // Filter any operator which requires too many arguments
  filters.push_back(
      [validation](atn::CalcElem x) { return x.args() <= validation; });

  // If the number stack elements left to have is equal to the number of binary
  // operators needed to complete the formula, filter non binary operators
  if (MAX_STACK_SIZE - this->calc.calc.size() == validation - 1) {
    filters.push_back([](atn::CalcElem x) { return x.args() > 1; });
  }

  auto valid_calc_elems =
      atn::utils::filter(this->all_calc_elems, atn::utils::all(filters));
  std::shuffle(valid_calc_elems.begin(), valid_calc_elems.end(), this->rng);

  if (first_run && THREAD_COUNT > 1) {
    initial_valid_elems = valid_calc_elems;
    std::vector<std::thread> threads;
    std::vector<atn::Generator*> gens;
    for (uint8_t i = 0; i < THREAD_COUNT; ++i) {
      atn::Generator* gen = new Generator(*this);
      gens.push_back(gen);
      threads.push_back(atn::Generator::spawn_helper(*gen));
    }
    for (std::thread& t : threads) {
      t.join();
    }
    for (atn::Generator* gen : gens) {
      delete gen;
    }
    return;
  }

  for (auto it = valid_calc_elems.begin(); it != valid_calc_elems.end(); ++it) {
    this->calc.calc.push_back(*it);
    this->gen_approx(false);
    this->calc.calc.pop_back();
  }
}

// Written by Aaron Barge
// Copyright 2022

#include "interfaces.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "utils.h"

#define FILEOUT "./output.txt"

void atn::interface::generation_started() {
  std::fstream fileout(FILEOUT, std::ios::app);
  if (!fileout.is_open()) {
    throw atn::utils::FileNotFound();
  }
  fileout << "# NEW GENERATION HAS BEGUN" << std::endl;
  fileout.close();
}

void atn::interface::formula_created(atn::Calculator calc, std::thread::id thread_id) {
  std::stringstream ss;
  ss << "./all-" << thread_id << ".txt";
  std::fstream fileout(ss.str(), std::ios::app);
  if (!fileout.is_open()) {
    throw atn::utils::FileNotFound();
  }
  fileout << calc.to_str() << std::endl;
  fileout.close();
}

void atn::interface::submitted(atn::Calculator calc, double test_result,
                               uint64_t submit_count) {
  std::fstream fileout(FILEOUT, std::ios::app);
  if (!fileout.is_open()) {
    throw atn::utils::FileNotFound();
  }
  fileout << "TEST_" << submit_count << " = " << calc.to_str()
          << " # test: "<< std::setprecision(10) << test_result << std::endl;
  fileout.close();
}

void atn::interface::tested(uint64_t test_count, uint64_t submit_count, uint64_t error_count, double duration) {
  double test_rate = round(1.0 * test_count / duration);
  double error_rate = 100.0 * error_count / test_count;
  std::cout << "Generated " << submit_count << "/" << test_count << " @ " << test_rate << " approx/s, " << error_rate << "% error                    \r";
}


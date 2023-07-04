// Written by Aaron Barge
// Copyright 2022

#include "test_approx.h"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "utils.h"
#ifdef NVIDIA_GPU
#include <thrust/device_vector.h>
#include <thrust/functional.h>
#include <thrust/transform_reduce.h>
#endif

atn::TestData atn::read_test_file(const char* filename) {
  std::string line, word;
  std::vector<double> parsed_line;
  atn::TestData data;
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw atn::utils::FileNotFound();
  }
  while (std::getline(file, line)) {
    parsed_line.clear();
    std::stringstream ss(line);
    while (std::getline(ss, word, ',')) {
      parsed_line.emplace_back(std::stod(word));
    }
    data.push_back(MAKE_TUPLE(parsed_line[0], parsed_line[1], parsed_line[2]));
  }
  file.close();
  return data;
}

#ifdef NVIDIA_GPU
struct atn::test_calc_functor {
  const atn::Calculator& c;
  test_calc_functor(atn::Calculator _c) : c(_c) {}
  __host__ __device__ double operator()(const atn::Tuple& x) const {
    Ellipse e{GET<0>(*it), GET<1>(*it)};
    double calc_perimeter;
    calc_perimeter = calc.calculate(e);
    if (calc_perimeter < 0 || std::isnan(calc_perimeter))
      throw atn::CalculatorError("Unknown error occurred");
    return 100.0 * thrust::abs(calc_perimeter - GET<2>(*it));
  }
};
#endif

double atn::test_approx(const atn::Calculator& calc, const atn::TestData& data,
                        double max_value) {
  double result = 0.0;
#ifndef NVIDIA_GPU
  double calc_perimeter;
  for (auto it = data.begin(); it != data.end(); ++it) {
    Ellipse e{GET<0>(*it), GET<1>(*it)};
    calc_perimeter = calc.calculate(e);
    result += 100.0 * std::abs(calc_perimeter - GET<2>(*it));
    if (max_value < result) return max_value;
  }
#else
  thrust::device_vector<atn::Tuple> dev_vec = data;
  result = thrust::transform_reduce(dev_vec.begin(), dev_vec.end(),
                                    atn::test_calc_functor(calc), 0.0,
                                    thrust::plus<double>);
#endif
  return result;
}

void atn::print(atn::TestData data) {
  std::cout << "Test Data Size: " << data.size() << std::endl;
  std::cout << "[" << std::endl;
  std::cout << std::setprecision(20);
  for (auto it = data.begin(); it != data.end(); ++it) {
    if (it != data.begin()) std::cout << "," << std::endl;
    std::cout << "  (" << GET<0>(*it) << ", " << GET<1>(*it) << ", "
              << GET<2>(*it) << ")";
  }
  std::cout << "]" << std::endl;
}

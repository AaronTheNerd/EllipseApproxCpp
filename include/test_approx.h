// Written by Aaron Barge
// Copyright 2022

#ifndef _INCLUDE_TEST_APPROX_H_
#define _INCLUDE_TEST_APPROX_H_

#include <tuple>
#include "calculator.h"
#include "ellipse.h"
#ifndef NVIDIA_GPU
#include <vector>
#else
#include <thrust/host_vector.h>
#endif

namespace atn {

#ifndef NVIDIA_GPU
using Tuple = std::tuple<double, double, double>;
using TestData = std::vector<Tuple>;
#else
using Tuple = thrust::tuple<double, double, double>;
using TestData = thrust::host_vector<Tuple>;
#endif

TestData read_test_file(const char*);

double test_approx(const atn::Calculator&, const TestData&, double);

void print(TestData);

}  // namespace atn

#endif  // _INCLUDE_TEST_APPROX_H_

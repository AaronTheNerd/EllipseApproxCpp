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
#define TUPLE_T std::tuple
#define GET std::get
#define MAKE_TUPLE std::make_tuple
#define VECTOR_T std::vector
#else
#define TUPLE_T thrust::tuple
#define GET thrust::get
#define MAKE_TUPLE thrust::make_tuple
#define VECTOR_T thrust::host_vector
#endif

using Tuple = TUPLE_T<double, double, double>;
using TestData = VECTOR_T<Tuple>;

TestData read_test_file(const char*);

double test_approx(const atn::Calculator&, const TestData&, double);

void print(TestData);

}  // namespace atn

#endif  // _INCLUDE_TEST_APPROX_H_

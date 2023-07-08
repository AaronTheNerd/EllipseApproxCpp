// Written by Aaron Barge
// Copyright 2022

#ifndef _INCLUDE_UTILS_H_
#define _INCLUDE_UTILS_H_

#include <time.h>

#include <vector>

#include "calculator.h"

namespace atn::utils {

struct FileNotFound : public std::exception {
  const char* what() const throw();
};

uint64_t random_seed();

std::vector<CALC> split_calc(const CALC&, uint8_t);

}  // namespace atn::utils

#endif  // _INCLUDE_UTILS_H_
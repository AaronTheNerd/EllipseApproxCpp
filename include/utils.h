// Written by Aaron Barge
// Copyright 2022

#ifndef _INCLUDE_UTILS_H_
#define _INCLUDE_UTILS_H_

#include <time.h>

#include <functional>

#include "calculator.h"

namespace atn::utils {

struct FileNotFound : public std::exception {
  const char* what() const throw();
};

typedef std::function<bool(CALC_ELEM)> predicate;

int random_seed();

std::vector<CALC> split_calc(const CALC&, uint8_t);

}  // namespace atn::utils

#endif  // _INCLUDE_UTILS_H_
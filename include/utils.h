// Written by Aaron Barge
// Copyright 2022

#ifndef _INCLUDE_UTILS_H_
#define _INCLUDE_UTILS_H_

#include <functional>
#include <time.h>
#include "calculator.h"

namespace atn::utils {

struct FileNotFound : public std::exception {
  const char* what() const throw();
};

typedef std::function<bool(CALC_ELEM)> predicate;

int random_seed();

double dfact(int);

double factorial(int);

predicate all(const std::vector<predicate>&);

std::vector<CALC_ELEM> filter(const std::vector<CALC_ELEM>, predicate);

std::vector<std::vector<CALC_ELEM>> split_vec(const std::vector<CALC_ELEM>&, uint8_t);

}  // namespace atn

#endif  // _INCLUDE_UTILS_H_
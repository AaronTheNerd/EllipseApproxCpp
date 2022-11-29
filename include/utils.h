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

typedef std::function<bool(atn::CalcElem)> predicate;

int random_seed();

double dfact(int);

double factorial(int);

predicate all(const std::vector<predicate>&);

std::vector<atn::CalcElem> filter(const std::vector<atn::CalcElem>, predicate);

std::vector<std::vector<atn::CalcElem>> split_vec(const std::vector<atn::CalcElem>&, uint8_t);

}  // namespace atn

#endif  // _INCLUDE_UTILS_H_
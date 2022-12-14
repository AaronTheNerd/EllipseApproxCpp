// Written by Aaron Barge
// Copyright 2022

#include "utils.h"
#include <iostream>
#include <algorithm> // std::copy_if, std::distance

const char* atn::utils::FileNotFound::what() const throw() {
  return "File failed to be open";
}

int atn::utils::random_seed() {
  return time(NULL);
}

double atn::utils::dfact(int x) {
  double result = 1.0;
  for (int i = x; i >= 1; i -= 2) {
    result *= i;
  }
  return result;
}

double atn::utils::factorial(int x) {
  if (x < 2) return 1.0 * x;
  double result = 1.0;
  for (int i = 1; i <= x; ++i) {
    result *= i;
  }
  return result;
}

atn::utils::predicate atn::utils::all(const std::vector<atn::utils::predicate>& predicates) {
  return [predicates] (atn::CalcElem x) {
    bool result;
    for (auto it = predicates.begin(); it != predicates.end(); ++it) {
      result = (*it)(x);
      if (!result) return false;
    }
    return true;
  };
}

std::vector<atn::CalcElem> atn::utils::filter(
    const std::vector<atn::CalcElem> elements, atn::utils::predicate pred) {
  std::vector<atn::CalcElem> out;
  std::copy_if(elements.begin(), elements.end(), std::back_inserter(out), pred);
  return out;
}

std::vector<std::vector<atn::CalcElem>> atn::utils::split_vec(const std::vector<atn::CalcElem>& vec, uint8_t n) {
  std::vector<std::vector<atn::CalcElem>> out_vec;
  size_t length = vec.size() / n;
  size_t remain = vec.size() % n;
  size_t begin = 0;
  size_t end = 0;

  for (size_t i = 0; i < n; ++i) {
    end += length + (i < remain);
    out_vec.emplace_back(vec.begin() + begin, vec.begin() + end);
    begin = end;
  }

  return out_vec;
}

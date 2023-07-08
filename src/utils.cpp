// Written by Aaron Barge
// Copyright 2022

#include "utils.h"

const char* atn::utils::FileNotFound::what() const throw() {
  return "File failed to be open";
}

uint64_t atn::utils::random_seed() { return time(NULL); }

std::vector<CALC> atn::utils::split_calc(const CALC& vec, uint8_t n) {
  std::vector<CALC> out_vec;
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

// Written by Aaron Barge
// Copyright 2022

#ifndef _INCLUDE_INTERFACES_H_
#define _INCLUDE_INTERFACES_H_

#include "calculator.h"

namespace atn::interface {

void submitted(atn::Calculator, double, uint64_t);

void tested(uint64_t, uint64_t, uint64_t, double);

}  // namespace atn

#endif  // _INCLUDE_INTERFACES_H_
#include <chrono>
#include <cstring>
#include <iostream>
#include <string>

#include "calculator.h"
#include "generation.h"

CALC_ELEM cmd_char_to_elem(char c) {
  switch (c) {
    case 'a':
      return CALC_PI;
    case 'b':
      return CALC_1;
    case 'c':
      return CALC_2;
    case 'd':
      return CALC_3;
    case 'e':
      return CALC_4;
    case 'f':
      return CALC_5;
    case 'g':
      return CALC_6;
    case 'h':
      return CALC_7;
    case 'i':
      return CALC_8;
    case 'j':
      return CALC_9;
    case 'k':
      return CALC_10;
    case 'l':
      return CALC_HALF;
    case 'm':
      return CALC_THIRD;
    case 'n':
      return CALC_QUARTER;
    case 'o':
      return CALC_A;
    case 'p':
      return CALC_B;
    case 'q':
      return CALC_C;
    case 'r':
      return CALC_E;
    case 's':
      return CALC_H;
    case 't':
      return CALC_SQRT;
    case 'u':
      return CALC_NEG;
    case 'v':
      return CALC_INV;
    case 'w':
      return CALC_ADD;
    case 'x':
      return CALC_SUB;
    case 'y':
      return CALC_MUL;
    case 'z':
      return CALC_DIV;
    case '0':
      return CALC_EXP;
  }
  return CALC_INVALID;
}

CALC cmd_string_to_calc(char* s) {
  CALC result;
  for (uint8_t i = 0; i < strlen(s); ++i) {
    result += cmd_char_to_elem(s[i]);
  }
  return result;
}

int main(int argc, char** argv) {
  auto start = std::chrono::high_resolution_clock::now();
  CALC initial_calc = cmd_string_to_calc(argv[1]);
  atn::Calculator c{initial_calc, std::stoi(argv[2])};
  CALC all_elems = cmd_string_to_calc(argv[3]);
  atn::Generator gen(0, 1, c, all_elems, true);
  gen.run();
  auto end = std::chrono::high_resolution_clock::now();
  auto runtime =
      std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
  std::cout << runtime.count() << std::endl;
}
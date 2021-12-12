#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdint.h>

struct TestPlayer {
  int8_t x;
  int8_t y;
};

struct Obstacle {
  int8_t x;
  int8_t y;
  int bool;       // 1 = top, 0 = bottom
  int collision;  // 1 = true, 0 = false
};

#endif
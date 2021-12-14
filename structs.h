#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdint.h>

struct Player {
  int8_t x;
  int8_t y;
  int8_t width;
  int8_t height;
};

struct Obstacle {
  int8_t x;
  int8_t y;
  int positionBool;  // 1 = top, 0 = bottom
  int collision;     // 1 = true, 0 = false
  int8_t width;
  int8_t height;
};

#endif
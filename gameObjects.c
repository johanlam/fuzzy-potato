#include <stdint.h>

#include "structs.h"

struct TestPlayer bird = {
    32,  // x initial positions
    16,  // y
};

struct TestPlayer* getPlayerBird() {
  return &bird;
};

struct Obstacle pipe = {
    100, 0,
    0,  // top

};

struct Obstacle pipe2 = {
    80, 32,
    1,  // bottom
};

struct Obstacle pipe3 = {
    128, 32,
    1,  // bottom
};

struct Obstacle* getPipe1() {
  return &pipe;
}

struct Obstacle* getPipe2() {
  return &pipe2;
}

struct Obstacle* getPipe3() {
  return &pipe3;
}

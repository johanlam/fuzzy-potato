#include <stdint.h>

#include "structs.h"

struct Player bird = {
    32,  // x initial positions
    10,  // y
    5,   // width
    5,   // height
};

struct Player* getPlayerBird() {
  return &bird;
};

struct Obstacle pipe1 = {
    100, 0,
    0,       // top
    0,       // collision false
    6,   9,  // width height

};

struct Obstacle pipe2 = {
    80, 32,
    1,       // bottom
    0,       // collision false
    2,  12,  // width height
};

struct Obstacle pipe3 = {
    128, 32,
    1,        // bottom
    0,        // collision false
    3,   10,  // width height
};

struct Obstacle pipe4 = {
    250, 32,
    1,        // bottom
    0,        // collision false
    4,   10,  // width height
};

struct Obstacle pipe5 = {
    150, 0,
    0,        // top
    0,        // collision false
    4,   11,  // width height
};

struct Obstacle pipe6 = {
    200, 0,
    0,        // top
    0,        // collision false
    2,   12,  // width height
};

struct Obstacle pipe7 = {
    290, 0,
    0,       // top
    0,       // collision false
    6,   7,  // width height
};

struct Obstacle pipe8 = {
    64, 0,
    0,      // top
    0,      // collision false
    4,  6,  // width height
};

struct Obstacle pipe9 = {
    175, 32,
    1,       // bottom, also change y to 32
    0,       // collision false
    2,   9,  // width height
};

struct Obstacle pipe10 = {
    278, 32,
    1,        // bottom, also change y to 32
    0,        // collision false
    2,   10,  // width height
};

struct Obstacle* getPipe1() {
  return &pipe1;
}

struct Obstacle* getPipe2() {
  return &pipe2;
}

struct Obstacle* getPipe3() {
  return &pipe3;
}
struct Obstacle* getPipe4() {
  return &pipe4;
}

struct Obstacle* getPipe5() {
  return &pipe5;
}

struct Obstacle* getPipe6() {
  return &pipe6;
}

struct Obstacle* getPipe7() {
  return &pipe7;
}

struct Obstacle* getPipe8() {
  return &pipe8;
}

struct Obstacle* getPipe9() {
  return &pipe9;
}

struct Obstacle* getPipe10() {
  return &pipe10;
}
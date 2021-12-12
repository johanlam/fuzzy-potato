/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include <stdint.h>  /* Declarations of uint_32 and the like */

#include "gameObjects.h"
#include "mipslab.h" /* Declatations for these labs */
#include "structs.h"

int prime = 1234567;

int mytime = 0x5957;
int timeoutcount = 0;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr(void) {
  if (IFS(0) &
      0x100) {   // verify flag is high, 0x100 = 1000 0000 and we want bit 8
    IFS(0) = 0;  // reset flag
    timeoutcount++;
    if (timeoutcount == 2) {
      //   int initY = 16;

      //   int bool = 0;
      //   while (!bool) {
      //     screen_clear(screen);

      //     // display_clear();
      //     initY--;

      //     toggle_pixel(64, initY, 1);
      //     display_image2(0, screen);
      //     delay(1000);
      //     if (initY == 0) {
      //       bool = 1;
      //     }
      //   }
      timeoutcount = 0;
    }
  }
}

/* Lab-specific initialization goes here */
void labinit(void) {
  display_init();
  volatile int* trise = (volatile int*)0xbf886100;
  *trise = *trise & 0xff00;  // sets bits 0:7 as outputs

  TRISDSET = 0xfe0;  // initializes port D and sets bits 5:11 to inputs

  T2CONSET = 0x70;                // set prescale to 256 0x70 = 111 0000
  PR2 = ((80000000 / 256) / 10);  // 100 ms = 1 second/10
  TMR2 = 0;                       // reset timer
  T2CONSET = 0x8000;              // start timer

  IEC(0) =
      (1 << 8);  // write one to T2IE bit in IEC(0) Interrupt Enable Control
  IPC(2) = (0x7 << 2);  // write highest prio to the 3 bits in T2IP in IPC(2)
                        // Interrupt Priority Control
  enable_interrupt();   // enable interrupts globally

  return;
}

/* This function is called repetitively from the main program */

// struct TestPlayer t2 = {
//     0,
//     0,
// };
// struct TestPlayer* getPlayert2() {
//   return &t2;
// };

struct TestPlayer* player;

struct Obstacle* obstacle1;
struct Obstacle* obstacle2;
struct Obstacle* obstacle3;

void drawBird(struct TestPlayer* playerObj) {
  int i;
  int j;
  for (i = 0; i < 6; i++) {
    for (j = 0; j < 6; j++) {
      toggle_pixel(playerObj->x + i, playerObj->y + j, 1);
    }
  }
}

void birdLoop() {
  player = getPlayerBird();
  // initial positions
  //   player->x = 32;
  //   player->y = 16;

  drawBird(player);
  toggle_pixel(player->x, player->y, 1);
  delay(1);
  player->y++;
  //   player->x++;

  //   int bool = 1;
  //   while (bool) {
  //     // screen_clear(screen);
  //     player->y++;
  //     toggle_pixel(player->x, player->y, 1);
  //     // display_image2(0, screen);
  //     delay(1000);
  //     if (player->y >= 33) {
  //       bool = 0;
  //     }
  //   }
}

// draw the pipes separately: top and bottom

// void drawPipeTop(int8_t width, int8_t height, struct Obstacle* pipeObj) {
//   int i;
//   int j;
//   int t;
//   for (t = 0; t < width; t++) {
//     for (i = 0; i < height; i++) {
//       toggle_pixel(pipeObj->x + t, i, 1);
//       for (j = 0; j < height; j++) {
//         toggle_pixel(pipeObj->x + t + 1, j, 1);
//       }
//     }
//   }
// }

// void drawPipeBottom(int8_t width, int8_t height, struct Obstacle* pipeObj) {
//   int i;
//   int j;
//   int t;
//   for (t = 0; t < width; t++) {
//     for (i = 0; i < height; i++) {
//       toggle_pixel(pipeObj->x + t, pipeObj->y - i, 1);
//       for (j = 0; j < height; j++) {
//         toggle_pixel(pipeObj->x + t + 1, pipeObj->y - j, 1);
//       }
//     }
//   }
// }

void drawPipe(int8_t width, int8_t height, struct Obstacle* pipeObj) {
  int i;
  int j;
  int t;

  if (!pipeObj->bool) {
    for (t = 0; t < width; t++) {
      for (i = 0; i < height; i++) {
        toggle_pixel(pipeObj->x + t, i, 1);
        for (j = 0; j < height; j++) {
          toggle_pixel(pipeObj->x + t + 1, j, 1);
        }
      }
    }

  } else {
    for (t = 0; t < width; t++) {
      for (i = 0; i < height; i++) {
        toggle_pixel(pipeObj->x + t, pipeObj->y - i, 1);
        for (j = 0; j < height; j++) {
          toggle_pixel(pipeObj->x + t + 1, pipeObj->y - j, 1);
        }
      }
    }
  }
}

void obstacleLoop() {
  obstacle1 = getPipe1();  // top
  drawPipe(6, 12, obstacle1);
  obstacle2 = getPipe2();  // bottom
  drawPipe(2, 12, obstacle2);
  obstacle3 = getPipe3();  // bottom
  drawPipe(3, 8, obstacle3);

  delay(1);
  //   int i;
  //   int j;
  //   for (i = 0; i < 12; i++) {
  //     toggle_pixel(obstacle1->x, i, 1);
  //     for (j = 0; j < 12; j++) {
  //       toggle_pixel(obstacle1->x + 1, j, 1);
  //     }
  //   }

  obstacle1->x--;
  obstacle2->x--;
  obstacle3->x--;
}

void reset() {
  player->x = 32;
  player->y = 16;
}

void detectCollision() {
  if (player->y + 5 > 32 || player->y < 0) {  // top bottom walls
    // display_string(0, "dead");
    // display_update();
  }

  //   if ((player->x + 5 == obstacle1->x) || (player->y == obstacle1->y)) {
  //     reset();
  //     display_string(3, "dead");
  //     display_update();
  //   }

  //   if ((player->x + 5 == obstacle2->x) || (player->y == obstacle2->y)) {
  //     reset();
  //     display_string(3, "dead");
  //     display_update();
  //   }
}

void obstacleCollision(int bool) {
  if (bool) {
    display_string(0, "KTH/ICT lab");
  }
}

void renderAll() {
  int sw = getsw();
  if (sw & 0b1) {
    player->x = 32;
    player->y = 16;
    main();
  }
  int btns = getbtns();

  if (btns & 0b1) {
    player->y -= 3;
  }
  if (btns & 0b10) {
    player->x += 1;
  }
  if (btns & 0b100) {
    player->x -= 1;
  }

  screen_clear(screen);
  detectCollision();
  birdLoop();
  obstacleLoop();

  display_image2(0, screen);
}

int lineSegment(int a, int b, int value) {
  int min, max;

  if (a < b) {
    max = b;
    min = a;
  } else {
    max = a;
    min = b;
  }

  if ((value > min) && (value < max)) return 1;
  if ((value == min) || (value == max)) return 1;
  return 0;
}

void labwork(void) {  // game loop

  renderAll();
}

// display_string(0, "KTH/ICT lab");
//   display_string(1, "in Computer");
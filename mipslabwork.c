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
    if (timeoutcount == 10) {
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

struct Player* player;

struct Obstacle* obstacle1;
struct Obstacle* obstacle2;
struct Obstacle* obstacle3;
struct Obstacle* obstacle4;
struct Obstacle* obstacle5;
struct Obstacle* obstacle6;
struct Obstacle* obstacle7;
struct Obstacle* obstacle8;
struct Obstacle* obstacle9;
struct Obstacle* obstacle10;

void drawBird(struct Player* playerObj) {
  int i;
  int j;
  for (i = 0; i < playerObj->width; i++) {
    for (j = 0; j < playerObj->height; j++) {
      toggle_pixel(playerObj->x + i, playerObj->y + j, 1);
    }
  }
}

void birdLoop() {
  player = getPlayerBird();
  // initial positions
  //   player->x = 32;
  //   player->y = 10;

  drawBird(player);
  //   toggle_pixel(player->x, player->y, 1);
  delay(1);
  player->y++;  // bird is falling from gravity
}

void drawPipe(int8_t width, int8_t height, struct Obstacle* pipeObj) {
  int i;
  int j;
  int t;

  // pipe drawn from top and bottom handled differently
  if (!pipeObj->positionBool) {  // top
    for (t = 0; t < width; t++) {
      for (i = 0; i < height; i++) {
        toggle_pixel(pipeObj->x + t, i, 1);
        for (j = 0; j < height; j++) {
          toggle_pixel(pipeObj->x + t + 1, j, 1);
        }
      }
    }

  } else {
    for (t = 0; t < width; t++) {  // bottom
      for (i = 0; i < height; i++) {
        toggle_pixel(pipeObj->x + t, pipeObj->y - i, 1);
        for (j = 0; j < height; j++) {
          toggle_pixel(pipeObj->x + t + 1, pipeObj->y - j, 1);
        }
      }
    }
  }
}

void deadHandler() {
  screen_clear(screen);
  struct Player* temp;
  temp = getPlayerBird();
  temp->x = 32;
  temp->y = 0;
  resetObstacles();
  display_string(0, "you died");
  display_update();
  delay2(1);
}

void resetObstacles() {
  obstacle1->x = 100;
  obstacle2->x = 80;
  obstacle3->x = 128;
  obstacle4->x = 250;
  obstacle5->x = 150;
  obstacle6->x = 200;
  obstacle7->x = 0;
  obstacle8->x = 64;
  obstacle9->x = 175;
  obstacle10->x = 278;
}

void obstacleLoop() {
  obstacle1 = getPipe1();  // top
  drawPipe(obstacle1->width, obstacle1->height, obstacle1);

  obstacle2 = getPipe2();  // bottom
  drawPipe(obstacle2->width, obstacle2->height, obstacle2);

  obstacle3 = getPipe3();  // bottom
  drawPipe(obstacle3->width, obstacle3->height, obstacle3);

  obstacle4 = getPipe4();  // bottom
  drawPipe(obstacle4->width, obstacle4->height, obstacle4);

  obstacle5 = getPipe5();  // top
  drawPipe(obstacle5->width, obstacle5->height, obstacle5);

  obstacle6 = getPipe6();  // top
  drawPipe(obstacle6->width, obstacle6->height, obstacle6);

  obstacle7 = getPipe7();  // top
  drawPipe(obstacle7->width, obstacle7->height, obstacle7);

  obstacle8 = getPipe8();  // top
  drawPipe(obstacle8->width, obstacle8->height, obstacle8);

  obstacle9 = getPipe9();  // bottom
  drawPipe(obstacle9->width, obstacle9->height, obstacle9);

  obstacle10 = getPipe10();  // bottom
  drawPipe(obstacle10->width, obstacle10->height, obstacle10);

  delay(1);

  obstacle1->x--;
  obstacle2->x--;
  obstacle3->x--;
  obstacle4->x--;
  obstacle5->x--;
  obstacle6->x--;
  obstacle7->x--;
  obstacle8->x--;
  obstacle9->x--;
  obstacle10->x--;
}

void detectCollision(struct Player* playerObj, struct Obstacle* obstacleObj) {
  if (player->y + 5 > 32 || player->y < 0) {  // top bottom walls
    deadHandler();
  }

  // detect obstacle collision, top and bottom collision handled differently

  int collisionYtop = lineSegment(
      obstacleObj->y, obstacleObj->y + obstacleObj->height, playerObj->y);
  int collisionXtop =
      lineSegment(obstacleObj->x, obstacleObj->x + obstacleObj->width,
                  playerObj->x + playerObj->width);

  if ((collisionYtop && collisionXtop) == 1) {
    deadHandler();
  }

  int collisionYbot =
      lineSegment(obstacleObj->y, obstacleObj->y - obstacleObj->height,
                  playerObj->y + playerObj->height);
  int collisionXbot =
      lineSegment(obstacleObj->x, obstacleObj->x + obstacleObj->width,
                  playerObj->x + playerObj->width);

  if ((collisionYbot && collisionXbot) == 1) {
    deadHandler();
  }
}

void renderAll() {
  int sw = getsw();
  if (sw & 0b1) {
    player->x = 32;
    player->y = 10;
    main();
  }
  int btns = getbtns();

  if (btns & 0b1) {
    player->y -= 4;  // bird jumping height
  }
  if (btns & 0b10) {
    player->x += 1;
  }
  if (btns & 0b100) {
    player->x -= 1;
  }

  screen_clear(screen);

  birdLoop();
  obstacleLoop();

  detectCollision(player, obstacle1);
  detectCollision(player, obstacle2);
  detectCollision(player, obstacle3);
  detectCollision(player, obstacle4);
  detectCollision(player, obstacle5);
  detectCollision(player, obstacle6);
  detectCollision(player, obstacle7);
  detectCollision(player, obstacle8);
  detectCollision(player, obstacle9);
  detectCollision(player, obstacle10);

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

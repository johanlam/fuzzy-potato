/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include <stdint.h>  /* Declarations of uint_32 and the like */

#include "mipslab.h" /* Declatations for these labs */

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
      time2string(textstring, mytime);
      display_string(3, textstring);
      display_update();
      tick(&mytime);

      timeoutcount = 0;
    }
  }
}

/* Lab-specific initialization goes here */
void labinit(void) {
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
void labwork(void) {
  prime = nextprime(prime);
  display_string(0, itoaconv(prime));
  display_update();
}

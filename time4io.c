#include <pic32mx.h>
#include <stdint.h>

#include "mipslab.h"

int getsw(void) {
  return (PORTD >> 8) &
         0x000f;  // right-shift 8 bits to reach bits 8:11 and mask
}

int getbtns(void) {
  return (PORTD >> 5) &
         0x0007;  // right-shift 5 bits to reach bits 5:7 and mask
}
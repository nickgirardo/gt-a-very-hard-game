#ifndef LOOPBOYH_H_
#define LOOPBOYH_H_

#include "common.h"

#define LOOPBOY_SIZE 6
#define LOOPBOY_COLOR 0xBB
#define LOOPBOY_DOUBLE_OFFSET 16
#define LOOPBOY_DOUBLE_OFFSET_LOG2 4
#define LOOPBOY_OFFSET 8

#define LOOPBOY_WRAPPING_FACTOR 0x7F

void init_loopboyh(char x, char y, char n, LoopBoyDirection dir);
void draw_loopboyh(char ix);
UpdateResult update_loopboyh(char ix);


#endif // LOOPBOYH_H_

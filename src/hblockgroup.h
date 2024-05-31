#ifndef HBLOCKGROUP_H_
#define HBLOCKGROUP_H_

#include "common.h"

#define HBLOCK_SIZE 6
#define HBLOCK_COLOR 0xBB
#define HBLOCK_OFFSET 16

void init_hblockgroup(char x, char y, char n, char d);
void draw_hblockgroup(char ix);
UpdateResult update_hblockgroup(char ix);

#endif // HBLOCKGROUP_H_

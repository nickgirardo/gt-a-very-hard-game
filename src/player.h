#ifndef  PLAYER_H
#define  PLAYER_H

#include "common.h"
#include "input.h"
#include "drawing_funcs.h"

#define PLAYER_ACCEL 12
#define PLAYER_MIN_V 0x0020
#define PLAYER_MIN_V_NEG -0x0020
#define PLAYER_SIZE 6

void init_player(char x, char y);
void draw_player(char ix);
void update_player(char ix);


#endif //  PLAYER_H

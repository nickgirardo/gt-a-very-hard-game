#ifndef  GOOMBA_H
#define  GOOMBA_H

#include "common.h"
#include "drawing_funcs.h"

#define GOOMBA_ACCEL 6

void init_goomba(char x, char y);
void draw_goomba(char ix);
void update_goomba(char ix);


#endif //  GOOMBA_H

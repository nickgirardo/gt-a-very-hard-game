#ifndef SHOOTER_H_
#define SHOOTER_H_

#include "common.h"

#define SHOOTER_SHOT_DELAY 150

void init_shooter(char x, char y);
void draw_shooter(char ix);
void update_shooter(char ix);

#endif // SHOOTER_H_

#ifndef  MAGGOT_H
#define  MAGGOT_H

#include "common.h"

#define MAGGOT_VEL 0x40
#define MAGGOT_CHARGE_VEL 0xC0
#define MAGGOT_CHANGE_DIR_TIMER 60
#define MAGGOT_SEARCH_SIZE_MAJOR 24
#define MAGGOT_SEARCH_SIZE_MINOR 4
#define MAGGOT_SIZE 4

void init_maggot(char x, char y);
void draw_maggot(char ix);
void update_maggot(char ix);


#endif //  MAGGOT_H

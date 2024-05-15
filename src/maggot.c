#include "maggot.h"
#include "common.h"
#include "drawing_funcs.h"
#include "player.h"
#include "random.h"

void init_maggot(char x, char y) {
  char i;
  MaggotData *data;

  for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
    if (entities[i] == EntityEmpty) {
      entities[i] = EntityMaggot;

      data = (MaggotData *)&entity_data[i];

      data->x.hl.h = x;
      data->x.hl.l = 0;
      data->y.hl.h = y;
      data->y.hl.l = 0;
      data->dir = DirLeft;
      data->timer = 0;

      return;
    }
  }

  // If we reach here it's because our entity table is full
}

void draw_maggot(char ix) {
  MaggotData data;

  data = *((MaggotData *)&entity_data[ix]);

  draw_box(data.x.hl.h, data.y.hl.h, MAGGOT_SIZE, MAGGOT_SIZE, 0x73);
}

#define CHARGE_CHECK_X()                                                       \
  (box_collision(data->x.hl.h - MAGGOT_SEARCH_SIZE_MAJOR,                      \
                 data->x.hl.h + MAGGOT_SEARCH_SIZE_MAJOR + MAGGOT_SIZE,        \
                 data->y.hl.h - MAGGOT_SEARCH_SIZE_MINOR,                      \
                 data->y.hl.h + MAGGOT_SEARCH_SIZE_MINOR + MAGGOT_SIZE,        \
                 player_data->x.hl.h, player_data->x.hl.h + PLAYER_SIZE,       \
                 player_data->y.hl.h, player_data->y.hl.h + PLAYER_SIZE))

#define CHARGE_CHECK_Y()                                                       \
  (box_collision(data->x.hl.h - MAGGOT_SEARCH_SIZE_MINOR,                      \
                 data->x.hl.h + MAGGOT_SEARCH_SIZE_MINOR + MAGGOT_SIZE,        \
                 data->y.hl.h - MAGGOT_SEARCH_SIZE_MAJOR,                      \
                 data->y.hl.h + MAGGOT_SEARCH_SIZE_MAJOR + MAGGOT_SIZE,        \
                 player_data->x.hl.h, player_data->x.hl.h + PLAYER_SIZE,       \
                 player_data->y.hl.h, player_data->y.hl.h + PLAYER_SIZE))

void update_maggot(char ix) {
  MaggotData *data;

  data = (MaggotData *)&entity_data[ix];

  if (data->timer == 0) {
    data->timer = MAGGOT_CHANGE_DIR_TIMER;

    if (CHARGE_CHECK_X()) {
      data->dir = player_data->x.hl.h < data->x.hl.h ? DirLeft : DirRight;
      data->charging = true;
    } else if (CHARGE_CHECK_Y()) {
      data->dir = player_data->y.hl.h < data->y.hl.h ? DirUp : DirDown;
      data->charging = true;
    } else {
      data->dir = rand_direction();
      data->charging = false;
    }
  }

  switch (data->dir) {
  case DirLeft:
    data->x.c -= data->charging ? MAGGOT_CHARGE_VEL : MAGGOT_VEL;
    break;
  case DirRight:
    data->x.c += data->charging ? MAGGOT_CHARGE_VEL : MAGGOT_VEL;
    break;
  case DirUp:
    data->y.c -= data->charging ? MAGGOT_CHARGE_VEL : MAGGOT_VEL;
    break;
  case DirDown:
    data->y.c += data->charging ? MAGGOT_CHARGE_VEL : MAGGOT_VEL;
    break;
  }

  data->timer--;
}

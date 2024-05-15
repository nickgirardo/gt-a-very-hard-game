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

#define DEBUG_DRAW_L()                                                         \
  (draw_box(data->x.hl.h - 24, data->y.hl.h - 4, 24, 12, 23))

#define DEBUG_DRAW_R()                                                         \
  (draw_box(data->x.hl.h + 4, data->y.hl.h - 4, 24, 12, 23))

#define DEBUG_DRAW_U()                                                         \
  (draw_box(data->x.hl.h - 4, data->y.hl.h - 24, 12, 24, 23))

#define DEBUG_DRAW_D()                                                         \
  (draw_box(data->x.hl.h - 4, data->y.hl.h + 4, 12, 24, 23))

#define CHARGE_CHECK_L()                                                       \
  (box_collision(data->x.hl.h - 24, data->x.hl.h, data->y.hl.h - 4,            \
                 data->y.hl.h + 8, player_data->x.hl.h,                        \
                 player_data->x.hl.h + PLAYER_SIZE, player_data->y.hl.h,       \
                 player_data->y.hl.h + PLAYER_SIZE))

#define CHARGE_CHECK_R()                                                       \
  (box_collision(data->x.hl.h + 4, data->x.hl.h + 28, data->y.hl.h - 4,        \
                 data->y.hl.h + 8, player_data->x.hl.h,                        \
                 player_data->x.hl.h + PLAYER_SIZE, player_data->y.hl.h,       \
                 player_data->y.hl.h + PLAYER_SIZE))

#define CHARGE_CHECK_U()                                                       \
  (box_collision(data->x.hl.h - 4, data->x.hl.h + 8, data->y.hl.h - 24,        \
                 data->y.hl.h, player_data->x.hl.h,                            \
                 player_data->x.hl.h + PLAYER_SIZE, player_data->y.hl.h,       \
                 player_data->y.hl.h + PLAYER_SIZE))

#define CHARGE_CHECK_D()                                                       \
  (box_collision(data->x.hl.h - 4, data->x.hl.h + 8, data->y.hl.h + 4,         \
                 data->y.hl.h + 28, player_data->x.hl.h,                       \
                 player_data->x.hl.h + PLAYER_SIZE, player_data->y.hl.h,       \
                 player_data->y.hl.h + PLAYER_SIZE))

#define NO_CHARGE()                                                            \
  if (1) {                                                                     \
    data->dir = rand_direction();                                              \
  }

void update_maggot(char ix) {
  MaggotData *data;

  data = (MaggotData *)&entity_data[ix];

  if (data->timer == 0) {
    data->charging = false;
    data->timer = MAGGOT_CHANGE_DIR_TIMER;

    if (player_data->x.hl.h < data->x.hl.h) {
      if (CHARGE_CHECK_L()) {
        data->dir = DirLeft;
        data->charging = true;
      } else {
        if (player_data->y.hl.h < data->y.hl.h) {
          if (CHARGE_CHECK_U()) {
            data->dir = DirUp;
            data->charging = true;
          } else {
            NO_CHARGE();
          }
        } else {
          if (CHARGE_CHECK_D()) {
            data->dir = DirDown;
            data->charging = true;
          } else {
            NO_CHARGE();
          }
        }
      }
    } else {
      if (CHARGE_CHECK_R()) {
        data->dir = DirRight;
        data->charging = true;
      } else {
        if (player_data->y.hl.h < data->y.hl.h) {
          if (CHARGE_CHECK_U()) {
            data->dir = DirUp;
            data->charging = true;
          } else {
            NO_CHARGE();
          }
        } else {
          if (CHARGE_CHECK_D()) {
            data->dir = DirDown;
            data->charging = true;
          } else {
            NO_CHARGE();
          }
        }
      }
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

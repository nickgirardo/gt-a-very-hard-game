#include "goomba.h"
#include "common.h"

void init_goomba(char x, char y) {
  char i;
  GoombaData *data;

  for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
    if (entities[i] == EntityEmpty) {
      entities[i] = EntityGoomba;

      data = (GoombaData *) &entity_data[i];

      data->x.hl.h = x;
      data->x.hl.l = 0;
      data->y.hl.h = y;
      data->y.hl.l = 0;
      data->vx.c = 0;
      data->vy.c = 0;

      return;
    }
  }

  // If we reach here it's because our entity table is full
}

void draw_goomba(char ix) {
  GoombaData data;

  data = *((GoombaData *) &entity_data[ix]);

  draw_box(data.x.hl.h, data.y.hl.h, 4, 4, 53);
}

void update_goomba(char ix) {
  GoombaData *data;

  data = (GoombaData *) &entity_data[ix];

  if (player_data->x.hl.h > data->x.hl.h) {
    data->vx.c += GOOMBA_ACCEL;
  } else {
    data->vx.c -= GOOMBA_ACCEL;
  }

  if (player_data->y.hl.h > data->y.hl.h) {
    data->vy.c += GOOMBA_ACCEL;
  } else {
    data->vy.c -= GOOMBA_ACCEL;
  }

  data->vx.c -= data->vx.c >> 6;
  data->vy.c -= data->vy.c >> 6;

  data->x.c += data->vx.c;
  data->y.c += data->vy.c;
}

#include "shooter.h"
#include "bullet.h"
#include "common.h"
#include "drawing_funcs.h"

void init_shooter(char x, char y) {
  char i;
  ShooterData *data;

  for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
    if (entities[i] == EntityEmpty) {
      entities[i] = EntityShooter;

      data = (ShooterData *) &entity_data[i];

      data->x.hl.h = x;
      data->x.hl.l = 0;
      data->y.hl.h = y;
      data->y.hl.l = 0;
      data->timer = SHOOTER_SHOT_DELAY;

      return;
    }
  }

  // If we reach here it's because our entity table is full
}

void draw_shooter(char ix) {
  ShooterData data;

  data = *((ShooterData *) &entity_data[ix]);

  draw_box(data.x.hl.h, data.y.hl.h, 6, 6, 0xF3);
}

void update_shooter(char ix) {
  ShooterData *data;

  data = (ShooterData *) &entity_data[ix];

  if (data->timer == 0) {
      data->timer = SHOOTER_SHOT_DELAY;
      init_bullet(data->x.hl.h, data->y.hl.h);
  }

  data->timer--;
}

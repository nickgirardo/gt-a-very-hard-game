#include "bullet.h"
#include "common.h"
#include "drawing_funcs.h"

void set_velocities(char i, char x, char y) {
    BulletData *data;
    short D;
    short dx;
    short dy;
    char j;
    bool flip_x;
    bool flip_y;

    data = (BulletData *) &entity_data[i];

    data->vx.c = 0;
    data->vy.c = 0;

    if (x > player_data->x.hl.h) {
        dx = x - player_data->x.hl.h;
        flip_x = true;
    } else {
        dx = player_data->x.hl.h - x;
        flip_x = false;
    }

    if (y > player_data->y.hl.h) {
        dy = y - player_data->y.hl.h;
        flip_y = true;
    } else {
        dx = player_data->y.hl.h - y;
        flip_y = false;
    }

    D = 2*dy - dx;

    for (j = 0; j < 12; j++) {
        if (D > 0) {
            // TODO explain why I'm incrementing this
            j++;
            data->vx.hl.l++;
            D -= 2*dx;
        }

        D += 2*dy;
        data->vy.hl.l++;
    }

    data->vx.c <<= 4;
    if (flip_x)
        data->vx.c *= -1;

    data->vy.c <<= 4;
    if (flip_y)
        data->vy.c *= -1;
}

void init_bullet(char x, char y) {
  char i;
  BulletData *data;

  for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
    if (entities[i] == EntityEmpty) {

      entities[i] = EntityBullet;

      data = (BulletData *) &entity_data[i];

      data->x.hl.h = x;
      data->x.hl.l = 0;
      data->y.hl.h = y;
      data->y.hl.l = 0;
      data->ttl = BULLET_TTL;

      set_velocities(i, x, y);

      return;
    }
  }

  // If we reach here it's because our entity table is full
}

void draw_bullet(char ix) {
  BulletData data;

  data = *((BulletData *) &entity_data[ix]);

  draw_box(data.x.hl.h, data.y.hl.h, 4, 4, 51);
}

void update_bullet(char ix) {
  BulletData *data;

  data = (BulletData *) &entity_data[ix];

  data->ttl--;
  if (data->ttl == 0) {
      entities[ix] = EntityEmpty;
      return;
  }

  data->x.c += data->vx.c;
  data->y.c += data->vy.c;
}

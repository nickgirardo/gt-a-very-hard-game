#include "banking.h"
#include "drawing_funcs.h"
#include "gametank.h"
#include "gt/drawing_funcs.h"
#include "hblockgroup.h"
#include "input.h"

#include "common.h"
#include "player.h"
#include "goomba.h"
#include "maggot.h"
#include "shooter.h"
#include "bullet.h"
#include "tilemap.h"

EntityKind entities[ENTITY_TABLE_SIZE];
PlayerData *player_data;
EntityData entity_data[ENTITY_TABLE_SIZE];
unsigned char tilemap[TILEMAP_SIZE];
unsigned char tilemap_decor[24];

unsigned char level_one[TILEMAP_SIZE] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 5, 4, 5, 4, 5, 4, 5, 4, 5, 0x14, 0x14, 0,
0, 0, 0, 0, 4, 5, 4, 5, 4, 5, 4, 5, 4, 0x14, 0x14, 0,
0, 0, 0, 0, 5, 4, 5, 4, 5, 4, 5, 4, 0, 0, 0, 0,
0, 0, 0, 0, 4, 5, 4, 5, 4, 5, 4, 5, 0, 0, 0, 0,
0, 0, 0, 0, 5, 4, 5, 4, 5, 4, 5, 4, 0, 0, 0, 0,
0, 0, 0, 0, 4, 5, 4, 5, 4, 5, 4, 5, 0, 0, 0, 0,
0, 0, 0, 0, 5, 4, 5, 4, 5, 4, 5, 4, 0, 0, 0, 0,
0, 5, 4, 5, 4, 5, 4, 5, 4, 5, 4, 5, 0, 0, 0, 0,
0, 4, 5, 4, 5, 4, 5, 4, 5, 4, 5, 4, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

unsigned char level_one_decor[] = {
24,
1, 24, 126, 15,
1, 39, 30, 56,
1, 95, 6, 18,
1, 113, 126, 14,
121, 39, 6, 18,
97, 57, 30, 56,
};

unsigned char level_one_entities[] = {
EntityPlayer,12,100,
EntityHBlockGroup, 41, 41, 4, 64,
EntityHBlockGroup, 33, 105, 4, -64,
EntityEmpty,
};

void noop(char ix) {
  return;
}

void clear_entities() {
  char i;

  for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
    entities[i] = EntityEmpty;
  }
}

void init_entities(const unsigned char *data) {
  clear_entities();
  while(*data != 0) {
    switch (*data) {
      case EntityPlayer:
        init_player(*(++data), *(++data));
        break;
      case EntityGoomba:
        init_goomba(*(++data), *(++data));
        break;
      case EntityHBlockGroup:
        init_hblockgroup(*(++data), *(++data), *(++data), *(++data));
        break;
      default:
        // We shouldn't ever hit this branch if our levels are crafted correctly
        // Just hard-lock
        while (1) {}
    }
    data++;
  }
}


void reset_level() {
  init_tilemap(level_one, level_one_decor);

  draw_tilemap_full();

  init_entities(level_one_entities);
}

void (*const drawing_fns[])(char) = {
  noop,
  draw_player,
  draw_goomba,
  draw_maggot,
  draw_shooter,
  draw_bullet,
  draw_hblockgroup,
};

void (*const update_fns[])(char) = {
  noop,
  update_player,
  update_goomba,
  update_maggot,
  update_shooter,
  update_bullet,
  update_hblockgroup,
};

int main() {
  char i;

  init_graphics();

  flip_pages();
  await_draw_queue();
  flip_pages();
  await_draw_queue();

  reset_level();

  // Run forever
  while (1) {
    PROFILER_START(1);
    update_inputs();

    draw_tilemap_partial();

    for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
      update_fns[entities[i]](i);
    }

    for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
      drawing_fns[entities[i]](i);
    }

    await_draw_queue();
    PROFILER_END(1);
    sleep(1);
    flip_pages();
  }

  //  We should never get here!
  return 0;
}

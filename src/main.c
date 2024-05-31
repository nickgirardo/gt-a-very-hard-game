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

unsigned char current_level = 0;

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

unsigned char level_two_entities[] = {
EntityPlayer,12,100,
EntityHBlockGroup, 41, 41, 4, 14,
EntityHBlockGroup, 33, 105, 4, -14,
EntityEmpty,
};

unsigned char level_one_reset_data[] = {
12,100,
};

LevelData levels[2];

void noop(char ix) {
  return;
}

UpdateResult noop_update(char ix) {
  return ResultOk;
}

void clear_entities() {
  char i;

  for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
    entities[i] = EntityEmpty;
  }
}

void init_entities(const unsigned char *data) {
  clear_entities();

  while(*data != EntityEmpty) {
    switch (*data) {
      case EntityPlayer:
        init_player(*(++data), *(++data));
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

void init_level() {
  LevelData l;
  l = levels[current_level];

  init_tilemap(l.tilemap, l.tilemap_decor);
  draw_tilemap_full();

  init_entities(l.entities);
}

void reset_level() {
  LevelData l;
  l = levels[current_level];

  reset_player(l.reset_data[0], l.reset_data[1]);
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

UpdateResult (*const update_fns[])(char) = {
  noop_update,
  update_player,
  noop_update,
  noop_update,
  noop_update,
  noop_update,
  update_hblockgroup,
};

int main() {
  char i;

  init_graphics();

  flip_pages();
  await_draw_queue();
  flip_pages();
  await_draw_queue();

  // Level set up
  levels[0].tilemap = level_one;
  levels[0].tilemap_decor = level_one_decor;
  levels[0].entities = level_one_entities;
  levels[0].reset_data = level_one_reset_data;

  levels[1].tilemap = level_one;
  levels[1].tilemap_decor = level_one_decor;
  levels[1].entities = level_two_entities;
  levels[1].reset_data = level_one_reset_data;

init_new_level:
  init_level();

  // Run forever
main_loop:
  while (1) {
    PROFILER_START(1);
    update_inputs();

    draw_tilemap_partial();

    for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
      switch (update_fns[entities[i]](i)) {
        case ResultFail:
          reset_level();
          goto main_loop;
        case ResultWin:
          current_level++;
          goto init_new_level;
      }
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

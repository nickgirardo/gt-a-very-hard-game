#include "gt/banking.h"
#include "gt/gametank.h"
#include "gt/drawing_funcs.h"
#include "gt/input.h"

#include "common.h"
#include "player.h"
#include "hblockgroup.h"
#include "loopboy.h"
#include "tilemap.h"

EntityKind entities[ENTITY_TABLE_SIZE];
PlayerData *player_data;
EntityData entity_data[ENTITY_TABLE_SIZE];
unsigned char tilemap[TILEMAP_SIZE];
unsigned char tilemap_decor[24];

unsigned char current_level = 0;

#pragma rodata-name (push, "PROG0")

const unsigned char level_one[TILEMAP_SIZE] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 5, 6, 5, 6, 5, 6, 5, 6, 5, 0x14, 0x14, 0,
0, 0, 0, 0, 6, 5, 6, 5, 6, 5, 6, 5, 6, 0x14, 0x14, 0,
0, 0, 0, 0, 5, 6, 5, 6, 5, 6, 5, 6, 0, 0, 0, 0,
0, 0, 0, 0, 6, 5, 6, 5, 6, 5, 6, 5, 0, 0, 0, 0,
0, 0, 0, 0, 5, 6, 5, 6, 5, 6, 5, 6, 0, 0, 0, 0,
0, 0, 0, 0, 6, 5, 6, 5, 6, 5, 6, 5, 0, 0, 0, 0,
0, 0, 0, 0, 5, 6, 5, 6, 5, 6, 5, 6, 0, 0, 0, 0,
0, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 0, 0, 0, 0,
0, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

const unsigned char level_two[TILEMAP_SIZE] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0,
0, 0, 0, 0, 5, 6, 5, 6, 5, 6, 5, 6, 5, 0, 0, 0,
0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0,
0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0,
0, 0x14, 0x14, 5, 6, 0, 0, 0, 0, 0, 6, 5, 6, 0, 0, 0,
0, 0x14, 0x14, 6, 5, 0, 0, 0, 0, 0, 5, 6, 5, 0, 0, 0,
0, 0x14, 0x14, 5, 6, 0, 0, 0, 0, 0, 6, 5, 6, 0, 0, 0,
0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0,
0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0,
0, 0, 0, 0, 5, 6, 5, 6, 5, 6, 5, 6, 5, 0, 0, 0,
0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

const unsigned char level_gg[TILEMAP_SIZE] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 6, 5, 6, 5, 6, 5, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 5, 0x5d, 0x3d, 0x1d, 5, 6, 5, 6, 5, 6, 5, 6, 0, 0,
0, 0, 6, 0x7d, 6, 5, 6, 5, 6, 0xfd, 0xdd, 0xbd, 6, 5, 0, 0,
0, 0, 5, 0x9d, 5, 6, 5, 6, 5, 0x1d, 5, 6, 5, 6, 0, 0,
0, 0, 6, 0xBd, 6, 0x5d, 0x9d, 5, 6, 0x3d, 6, 5, 6, 5, 0, 0,
0, 0, 5, 0xDd, 5, 6, 0x7d, 6, 5, 0x5d, 5, 0x9d, 0x3d, 6, 0, 0,
0, 0, 6, 0xFd, 0x1d, 0x3d, 0x5d, 5, 6, 0x7d, 6, 5, 0x1d, 5, 0, 0,
0, 0, 5, 6, 5, 6, 5, 6, 5, 0x9d, 0xbd, 0xdd, 0xfd, 6, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 6, 5, 6, 5, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

const unsigned char level_one_decor[] = {
24,
1, 24, 126, 15,
1, 39, 30, 56,
1, 95, 6, 18,
1, 113, 126, 14,
121, 39, 6, 18,
97, 57, 30, 56,
};

const unsigned char level_two_decor[] = {
44,
1, 24, 126, 7,
1, 31, 30, 32,
1, 63, 6, 26,
1, 89, 30, 32,
1, 121, 126, 6,
105, 31, 22, 90,
41, 31, 54, 8,
41, 113, 54, 8,
79, 49, 16, 14,
79, 89, 16, 14,
41, 49, 38, 54,
};

const unsigned char level_gg_decor[] = {
24,
1, 24, 126, 15,
1, 39, 14, 74,
113, 39, 14, 74,
1, 113, 126, 14,
15, 105, 48, 8,
65, 39, 48, 8,
};

const unsigned char level_one_entities[] = {
EntityPlayer,12,100,
// EntityHBlockGroup, 41, 41, 4, 64,
// EntityHBlockGroup, 33, 105, 4, -64,
EntityEmpty,
};

const unsigned char level_two_entities[] = {
EntityPlayer,84,73,
// // EntityLoopBoyH, 33, 41, 6, LoopBoyLeft,
EntityLoopBoyH, 97, 105, 6, LoopBoyRight,
// EntityLoopBoyH, 33, 105, 6, LoopBoyDown,
// EntityLoopBoyH, 97, 41, 6, LoopBoyUp,
EntityEmpty,
};

const unsigned char level_gg_entities[] = {
EntityPlayer,59,61,
EntityEmpty,
};

const unsigned char level_one_reset_data[] = {
12,100,
};

const unsigned char level_two_reset_data[] = {
84,73,
};

const unsigned char level_gg_reset_data[] = {
84,73,
};

#pragma rodata-name (pop)

LevelData levels[3];

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
      case EntityLoopBoyH:
        init_loopboy(*(++data), *(++data), *(++data), *(++data));
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
  LevelData *l;
  l = &levels[current_level];

  reset_player(l->reset_data[0], l->reset_data[1]);
}

void (*const drawing_fns[])(char) = {
  noop,
  draw_player,
  draw_hblockgroup,
  draw_loopboy,
};

UpdateResult (*const update_fns[])(char) = {
  noop_update,
  update_player,
  update_hblockgroup,
  update_loopboy,
};

int main() {
  char i;

  change_rom_bank(BANK_PROG0);

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

  levels[1].tilemap = level_two;
  levels[1].tilemap_decor = level_two_decor;
  levels[1].entities = level_two_entities;
  levels[1].reset_data = level_two_reset_data;

  levels[2].tilemap = level_gg;
  levels[2].tilemap_decor = level_gg_decor;
  levels[2].entities = level_gg_entities;
  levels[2].reset_data = level_gg_reset_data;

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

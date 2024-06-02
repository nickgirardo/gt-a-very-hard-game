#include "gt/banking.h"
#include "gt/feature/text/text.h"
#include "gt/gametank.h"
#include "gt/drawing_funcs.h"
#include "gt/input.h"

#include "common.h"
#include "entities/player.h"
#include "entities/hblockgroup.h"
#include "entities/loopboy.h"
#include "tilemap.h"

#include "levels/level_one.h"
#include "levels/level_two.h"
#include "levels/level_gg.h"

EntityKind entities[ENTITY_TABLE_SIZE];
PlayerData *player_data;
EntityData entity_data[ENTITY_TABLE_SIZE];
unsigned char tilemap[TILEMAP_SIZE];
unsigned char tilemap_decor[64];

unsigned char current_level = 0;

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

    init_text();
    text_cursor_x = 1;
    text_cursor_y = 7;
    text_use_alt_color = 1;
    print_text(l.name);
    flip_pages();
    init_text();
    text_cursor_x = 1;
    text_cursor_y = 7;
    text_use_alt_color = 1;
    print_text(l.name);

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

  init_graphics();
  load_font(0);

  change_rom_bank(BANK_PROG0);

  flip_pages();
  await_draw_queue();
  flip_pages();
  await_draw_queue();

  // Level set up
  levels[0].tilemap = level_one;
  levels[0].tilemap_decor = level_one_decor;
  levels[0].entities = level_one_entities;
  levels[0].reset_data = level_one_reset_data;
  levels[0].name = level_one_name;

  levels[1].tilemap = level_two;
  levels[1].tilemap_decor = level_two_decor;
  levels[1].entities = level_two_entities;
  levels[1].reset_data = level_two_reset_data;
  levels[1].name = level_two_name;

  levels[2].tilemap = level_gg;
  levels[2].tilemap_decor = level_gg_decor;
  levels[2].entities = level_gg_entities;
  levels[2].reset_data = level_gg_reset_data;
  levels[2].name = level_gg_name;


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

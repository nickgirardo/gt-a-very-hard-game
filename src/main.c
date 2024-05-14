#include "banking.h"
#include "drawing_funcs.h"
#include "gametank.h"
#include "input.h"
#include "persist.h"

#include "common.h"
#include "player.h"
#include "goomba.h"

char pos[4];

EntityKind entities[ENTITY_TABLE_SIZE];
PlayerData *player_data;
EntityData entity_data[ENTITY_TABLE_SIZE];

void noop(char ix) {
  return;
}

void (*const drawing_fns[])(char) = {
  noop,
  draw_player,
  draw_goomba
};

void (*const update_fns[])(char) = {
  noop,
  update_player,
  update_goomba
};

int main() {
  char i;

  init_graphics();

  flip_pages();
  clear_border(0);
  await_draw_queue();
  flip_pages();
  await_draw_queue();
  clear_border(0);

  change_rom_bank(SAVE_BANK_NUM);

  init_player();
  init_goomba(80, 80);
  init_goomba(10, 80);

  // Run forever
  while (1) {
    update_inputs();
    clear_screen(3);
    clear_border(3);

    for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
      update_fns[entities[i]](i);
    }

    for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
      drawing_fns[entities[i]](i);
    }

    await_draw_queue();
    sleep(1);
    flip_pages();
  }

  //  We should never get here!
  return 0;
}

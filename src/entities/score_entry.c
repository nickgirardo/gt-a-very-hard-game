#include "score_entry.h"

#include "../gt/input.h"
#include "../gt/feature/text/text.h"
#include "../gt/drawing_funcs.h"

#pragma code-name (push, "PROG0")

char next_char[64] = {
0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60,
0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x00,
};

void init_score_entry(unsigned short score) {
  ScoreEntryData *data;
  char i;

  for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
    if (entities[i] == EntityEmpty) {
      entities[i] = EntityScoreEntry;

      data = (ScoreEntryData *) &entity_data[i];
      data->score = score;

      data->entry[0] = 0x61;
      data->entry[1] = 0x62;
      data->entry[2] = 0x63;
      data->entry[3] = 0x64;
      data->entry[4] = 0x65;
      data->entry[5] = 0x66;
      data->entry[6] = 0x67;
      data->entry[7] = 0x68;
      data->entry[8] = 0x69;
      data->entry[9] = 0x6a;

      return;
    }
  }

  // This should never be reached!!
}

void draw_score_entry(char ix) {
  ScoreEntryData data;
  unsigned char i, j;

  data = *((ScoreEntryData *) &entity_data[ix]);

  clear_screen(0);
  await_draw_queue();

  init_text();
  text_color = TEXT_COLOR_WHITE;

  text_cursor_y = 40;
  text_cursor_x = 24;
  print_text("GREAT JOB!");
  text_cursor_y = 50;
  text_cursor_x = 8;
  print_text("Enter your name");

}

CollisionResult update_score_entry(char ix) {
  ScoreEntryData *data;

  data = (ScoreEntryData *) &entity_data[ix];

  if (player1_new_buttons & (INPUT_MASK_A | INPUT_MASK_START)) {
    // Return Win when we're done with name entry
    return ResultWin;
  }

  return ResultOk;
}

#pragma code-name (pop)

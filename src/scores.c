#include "scores.h"

#include <string.h>
#include "common.h"

#include "gt/drawing_funcs.h"
#include "gt/persist.h"
#include "gt/feature/text/text.h"

#define MAGIC_NUMBER 42

ScoreEntry *scores;
char *name;

unsigned char needs_draw;

#pragma code-name (push, "SAVE")
unsigned char saved_magic_number;
ScoreEntry saved_normal_scores[SCORE_ENTRIES];
ScoreEntry saved_secret_scores[SCORE_ENTRIES];
#pragma code-name (pop)

void init_scores() {
  unsigned char i;

  if (saved_magic_number == MAGIC_NUMBER) {
    // load saved scores
    memcpy(&normal_scores, &saved_normal_scores, SCORE_ENTRIES * sizeof(ScoreEntry));
    memcpy(&secret_scores, &saved_secret_scores, SCORE_ENTRIES * sizeof(ScoreEntry));
  } else {
    // initialize score table
    i = MAGIC_NUMBER;

    clear_save_sector();
    save_write(&i, &saved_magic_number, sizeof(unsigned char));

    normal_scores[0].score = 0x9999;
    normal_scores[0].name[0] = 't';
    normal_scores[0].name[1] = 'e';
    normal_scores[0].name[2] = 's';
    normal_scores[0].name[3] = 't';
    normal_scores[0].name[4] = '_';
    normal_scores[0].name[5] = 'n';
    normal_scores[0].name[6] = 'a';
    normal_scores[0].name[7] = 'm';
    normal_scores[0].name[8] = 'e';
    normal_scores[0].name[9] = '!';

    secret_scores[0].score = 0x6789;
    secret_scores[0].name[0] = 's';
    secret_scores[0].name[1] = 'e';
    secret_scores[0].name[2] = 'c';
    secret_scores[0].name[3] = 'r';
    secret_scores[0].name[4] = 'e';
    secret_scores[0].name[5] = 't';
    secret_scores[0].name[6] = '!';
    secret_scores[0].name[7] = '!';
    secret_scores[0].name[8] = '!';
    secret_scores[0].name[9] = '!';

    for (i = 1; i < SCORE_ENTRIES; i++) {
      memcpy(&normal_scores[i], &normal_scores[0], sizeof(ScoreEntry));
      memcpy(&secret_scores[i], &secret_scores[0], sizeof(ScoreEntry));
    }

    save_write(&normal_scores, &saved_normal_scores, SCORE_ENTRIES * sizeof(ScoreEntry));
    save_write(&secret_scores, &saved_secret_scores, SCORE_ENTRIES * sizeof(ScoreEntry));
  }
}

void enter_score_mode(ScoreEntry *_scores, char *_name) {
    major_mode = ModeScores;
    scores = _scores;
    name = _name;

    needs_draw = 2;
}

void run_scores() {
    unsigned char i, j;

    if (needs_draw == 0)
      return;

    needs_draw--;

    clear_screen(0);
    clear_border(0);
    await_draw_queue();

    init_text();
    text_color = TEXT_COLOR_WHITE;

    text_cursor_y = 10;
    text_cursor_x = 10;
    print_text(name);

    text_cursor_y = 22;

    for (i = 0; i < SCORE_ENTRIES; i++) {
        for (j = 0; j < SCORE_NAME_LENGTH; j++) {
            text_cursor_x = 4 + (j<<3);
            print_char(scores[i].name[j]);
        }
        text_cursor_x = 92;
        print_bcd_zp(scores[i].score);

        text_cursor_y += 10;
    }
}


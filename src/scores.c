#include "scores.h"

#include "common.h"

#include "gt/drawing_funcs.h"
#include "gt/feature/text/text.h"

#pragma code-name (push, "PROG0")

ScoreEntry *scores;
char *name;

unsigned char needs_draw;

void init_scores(ScoreEntry *_scores, char *_name) {
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
            print_char(scores[j].name[i]);
        }
        text_cursor_x = 92;
        print_bcd_zp(scores[j].score);

        text_cursor_y += 10;
    }
}

#pragma code-name (pop)

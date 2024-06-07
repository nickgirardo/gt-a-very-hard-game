#include "credits.h"

#include "gt/drawing_funcs.h"
#include "gt/input.h"
#include "gt/feature/text/text.h"

#include "common.h"

char credits_stage;

void init_credits() {
    major_mode = ModeCredits;
    credits_stage = 0;
}

void run_credits() {
    clear_screen(0);
    clear_border(0);
    await_draw_queue();

    init_text();
    text_print_line_start = 2;
    text_color = TEXT_COLOR_WHITE;

    switch (credits_stage) {
        case 0:
            text_cursor_x = 2;
            text_cursor_y = 9;
            print_text("A Very Hard Game\n\n\rCreated by Nick Girardo\n\n\rInspired by \n\r\"World's Hardest\n\rGame\"");
            if (player1_new_buttons & (INPUT_MASK_A | INPUT_MASK_START)) {
                credits_stage = 1;
            }
            break;
        case 1:
            text_cursor_x = 2;
            text_cursor_y = 9;
            print_text("TODO Music credits");
            if (player1_new_buttons & (INPUT_MASK_A | INPUT_MASK_START)) {
                credits_stage = 2;
            }
            break;
        case 2:
            text_cursor_x = 2;
            text_cursor_y = 9;
            print_text("Special Thanks:\n\n\rClyde Shaffer\n\rTons of help\n\rwith everything\n\n\rdarthbeep\n\rPlay testing and\n\rfeedback");
            if (player1_new_buttons & (INPUT_MASK_A | INPUT_MASK_START)) {
                init_game();
            }
            break;
    }
}

#include "credits.h"

#include "gt/drawing_funcs.h"
#include "gt/input.h"
#include "gt/feature/text/text.h"

#include "common.h"

#pragma code-name (push, "PROG0")

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
            set_cursor(2,9);
            print_text("A Very Hard Game\n\n\rCreated by Nick Girardo\n\n\rInspired by\n\r\"World's\n\rHardest Game\"");
            if (player1_new_buttons & (INPUT_MASK_A | INPUT_MASK_START)) {
                credits_stage = 1;
            }
            break;
        case 1:
            set_cursor(2,9);
            print_text("TODO Music credits");
            if (player1_new_buttons & (INPUT_MASK_A | INPUT_MASK_START)) {
                credits_stage = 2;
            }
            break;
        case 2:
            set_cursor(2,9);
            print_text("Special Thanks:\n\n\rClyde Shaffer\n\rTons of help\n\rwith everything\n\n\rdarthbeep\n\rPlay testing and\n\rfeedback\n\n\rHackManhattan");
            if (player1_new_buttons & INPUT_MASK_C) {
                credits_stage = 3;
            } else if (player1_new_buttons & (INPUT_MASK_A | INPUT_MASK_START)) {
                init_game();
            }
            break;
        case 3:
            set_cursor(2,9);
            print_text("why do they call\n\rit oven when you\n\rof in the cold\n\rfood of out the\n\rhot eat the food");
            if (player1_new_buttons & (INPUT_MASK_A | INPUT_MASK_START)) {
                init_game();
            }
            break;
    }
}

#pragma code-name (pop)

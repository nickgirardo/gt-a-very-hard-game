#include "input.h"
#include "gametank.h"

short player1_buttons = 0, player1_old_buttons = 0;
#ifdef TWO_PLAYER_INPUT
short player2_buttons = 0, player2_old_buttons = 0;
#endif


#pragma optimize (push, off)
void update_inputs(){
    char inputsA, inputsB;

#ifdef TWO_PLAYER_INPUT
    inputsA = *gamepad_2;
#endif

    inputsA = *gamepad_1;
    inputsB = *gamepad_1;

    player1_old_buttons = player1_buttons;
    player1_buttons = ~((((short) inputsB) << 8) | inputsA);
    player1_buttons &= INPUT_MASK_ALL_KEYS;

#ifdef TWO_PLAYER_INPUT
    inputsA = *gamepad_2;
    inputsB = *gamepad_2;
    player2_old_buttons = player2_buttons;
    player2_buttons = ~((((short) inputsB) << 8) | inputsA);
    player2_buttons &= INPUT_MASK_ALL_KEYS;
#endif
}
#pragma optimize (pop)

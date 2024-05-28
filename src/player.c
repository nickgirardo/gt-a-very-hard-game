#include "player.h"
#include "common.h"

void init_player(char x, char y) {
  PlayerData *data;
  char i;

  for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
    if (entities[i] == EntityEmpty) {
      entities[i] = EntityPlayer;

      data = (PlayerData *) &entity_data[i];

      data->x.hl.h = x;
      data->x.hl.l = 0;
      data->y.hl.h = y;
      data->y.hl.l = 0;
      data->vx.c = 0;
      data->vy.c = 0;

      player_data = data;

      return;
    }
  }

  // This should never be reached!!
}

void draw_player(char ix) {
  PlayerData data;

  data = *((PlayerData *) &entity_data[ix]);

  draw_box(data.x.hl.h, data.y.hl.h, PLAYER_SIZE, PLAYER_SIZE, 92);
}

void update_player(char ix) {
  PlayerData *data;

  data = (PlayerData *) &entity_data[ix];

  if (player1_buttons & INPUT_MASK_RIGHT) {
    data->vx.c += PLAYER_ACCEL;
  } else if (player1_buttons & INPUT_MASK_LEFT) {
    data->vx.c -= PLAYER_ACCEL;
  } else if (data->vx.c > 0 && data->vx.c < PLAYER_MIN_V) {
    data->vx.c = 0;
  } else if (data->vx.c < 0 && data->vx.c > PLAYER_MIN_V_NEG) {
    data->vx.c = 0;
  }

  if (player1_buttons & INPUT_MASK_DOWN) {
    data->vy.c += PLAYER_ACCEL;
  } else if (player1_buttons & INPUT_MASK_UP) {
    data->vy.c -= PLAYER_ACCEL;
  } else if (data->vy.c > 0 && data->vy.c < PLAYER_MIN_V) {
    data->vy.c = 0;
  } else if (data->vy.c < 0 && data->vy.c > PLAYER_MIN_V_NEG) {
    data->vy.c = 0;
  }

  data->vx.c -= data->vx.c >> PLAYER_FRICTION_COEFF;
  data->vy.c -= data->vy.c >> PLAYER_FRICTION_COEFF;

  data->x.c += data->vx.c;
  data->y.c += data->vy.c;
}

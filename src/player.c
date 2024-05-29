#include "player.h"
#include "common.h"
#include "gametank.h"

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

  // TILEMAP COLLISIONS
  // Are wrossing a tile boundry on the right?
  if (data->vx.c > 0 && (data->x.hl.h & (TILE_SIZE - 1)) == (TILE_SIZE - PLAYER_SIZE)) {
    char tx = (data->x.hl.h >> 3) + 1;
    char ty = data->y.hl.h >> 3;

    if (tilemap[tx + (ty << 4)] == TILE_WALL) {
        data->vx.c = 0;
    } else if ((data->y.hl.h & (TILE_SIZE - 1)) > (TILE_SIZE - PLAYER_SIZE)) {
      if (tilemap[tx + ((ty + 1)<< 4)] == TILE_WALL) {
        data->vx.c = 0;
      }
    }
  }

  // Are wrossing a tile boundry on the left?
  if (data->vx.c < 0 && (data->x.hl.h & (TILE_SIZE - 1)) == 0) {
    char tx = (data->x.hl.h >> 3) - 1;
    char ty = data->y.hl.h >> 3;

    if (tilemap[tx + (ty << 4)] == TILE_WALL) {
        data->vx.c = 0;
    } else if ((data->y.hl.h & (TILE_SIZE - 1)) > (TILE_SIZE - PLAYER_SIZE)) {
      if (tilemap[tx + ((ty + 1)<< 4)] == TILE_WALL) {
        data->vx.c = 0;
      }
    }
  }

  // Are wrossing a tile boundry on the bottom?
  if (data->vy.c > 0 && (data->y.hl.h & (TILE_SIZE - 1)) == (TILE_SIZE - PLAYER_SIZE)) {
    char tx = data->x.hl.h >> 3;
    char ty = (data->y.hl.h >> 3) + 1;

    if (tilemap[tx + (ty << 4)] == TILE_WALL) {
        data->vy.c = 0;
    } else if ((data->x.hl.h & (TILE_SIZE - 1)) > (TILE_SIZE - PLAYER_SIZE)) {
      if (tilemap[(tx + 1) + (ty << 4)] == TILE_WALL) {
        data->vy.c = 0;
      }
    }
  }

  // Are wrossing a tile boundry on the top?
  if (data->vy.c < 0 && (data->y.hl.h & (TILE_SIZE - 1)) == 0) {
    char tx = data->x.hl.h >> 3;
    char ty = (data->y.hl.h >> 3) - 1;

    if (tilemap[tx + (ty << 4)] == TILE_WALL) {
        data->vy.c = 0;
    } else if ((data->x.hl.h & (TILE_SIZE - 1)) > (TILE_SIZE - PLAYER_SIZE)) {
      if (tilemap[(tx + 1) + (ty << 4)] == TILE_WALL) {
        data->vy.c = 0;
      }
    }
  }

  data->x.c += data->vx.c;
  data->y.c += data->vy.c;
}

#include "loopboy.h"
#include "common.h"

#include "drawing_funcs.h"
#include "gametank.h"
#include "gt/drawing_funcs.h"
#include "player.h"

void init_loopboy(char x, char y, char n, LoopBoyDirection dir) {
  LoopBoyData *data;
  char i;

  for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
    if (entities[i] == EntityEmpty) {
      entities[i] = EntityLoopBoyH;

      data = (LoopBoyData *) &entity_data[i];

      data->x.hl.h = x;
      data->x.hl.l = 0;
      data->y.hl.h = y;
      data->y.hl.l = 0;
      data->n = n;
      data->d = 30;
      data->direction = dir;

      return;
    }
  }

  // This should never be reached!!
}

#define PREP_DRAW_BLOCK(y)                    \
    do {                                      \
        *dma_flags = flagsMirror | DMA_COLORFILL_ENABLE | DMA_OPAQUE; \
        vram[GX] = 0;                         \
        vram[GY] = 0;                         \
        vram[WIDTH] = LOOPBOY_SIZE;          \
        vram[HEIGHT] = LOOPBOY_SIZE;         \
        vram[COLOR] = ~LOOPBOY_COLOR;        \
    } while (0)

#define DRAW_BLOCK_X(x)                                     \
    do {                                                    \
    vram[VX] = x;                                           \
    vram[START] = 1;                                        \
} while (0)

#define DRAW_BLOCK_Y(y)                                     \
    do {                                                    \
    vram[VY] = y;                                           \
    vram[START] = 1;                                        \
} while (0)

void draw_loopboy(char ix) {
  LoopBoyData data;
  char i, j;

  data = *((LoopBoyData *) &entity_data[ix]);

  await_draw_queue();

  PREP_DRAW_BLOCK(data.y.hl.h);

  for (i = 0; i < data.n; i++) {
      j = ((i << LOOPBOY_DOUBLE_OFFSET_LOG2) + (data.d)) & LOOPBOY_WRAPPING_FACTOR;

      switch (data.direction) {
          case LoopBoyLeft:
            vram[VY] = data.y.hl.h;
            DRAW_BLOCK_X(data.x.hl.h + (j >> 1));
            break;
          case LoopBoyRight:
            vram[VY] = data.y.hl.h;
            DRAW_BLOCK_X(data.x.hl.h - (j >> 1));
            break;
          case LoopBoyUp:
            vram[VX] = data.x.hl.h;
            DRAW_BLOCK_Y(data.y.hl.h + (j >> 1));
            break;
          case LoopBoyDown:
            vram[VX] = data.x.hl.h;
            DRAW_BLOCK_Y(data.y.hl.h - (j >> 1));
            break;
          default:
              while (1) {};
      }
  }
}

UpdateResult update_loopboy(char ix) {
  LoopBoyData *data;
  register char i;
  register char j;

  data = (LoopBoyData *) &entity_data[ix];


  await_draw_queue();

  // Test for collisions with the player
  // All blocks moving left or right have the same y value
  // All blocks moving up or down have the same x value
  switch (data->direction) {
      case LoopBoyRight:
        if (box_collision_y(
                data->y.hl.h,
                data->y.hl.h + LOOPBOY_SIZE,
                player_data->y.hl.h,
                player_data->y.hl.h + PLAYER_SIZE)) {
            // TODO I think with some bit cleverness we can avoid a lot of work here
            for (i = 0; i < data->n; i++) {
                j = (((i << LOOPBOY_DOUBLE_OFFSET_LOG2) + (data->d)) & LOOPBOY_WRAPPING_FACTOR) >> 1;

                if (box_collision_x(
                        data->x.hl.h - j,
                        data->x.hl.h - j + LOOPBOY_SIZE,
                        player_data->x.hl.h,
                        player_data->x.hl.h + PLAYER_SIZE)) {
                    return ResultFail;
                }
            }
        }
        break;
      case LoopBoyLeft:
        // All blocks moving left or right have the same y value
        if (box_collision_y(
                data->y.hl.h,
                data->y.hl.h + LOOPBOY_SIZE,
                player_data->y.hl.h,
                player_data->y.hl.h + PLAYER_SIZE)) {
            // TODO I think with some bit cleverness we can avoid a lot of work here
            for (i = 0; i < data->n; i++) {
                j = (((i << LOOPBOY_DOUBLE_OFFSET_LOG2) + (data->d)) & LOOPBOY_WRAPPING_FACTOR) >> 1;

                if (box_collision_x(
                        data->x.hl.h + j,
                        data->x.hl.h + j + LOOPBOY_SIZE,
                        player_data->x.hl.h,
                        player_data->x.hl.h + PLAYER_SIZE)) {
                    return ResultFail;
                }
            }
        }
        break;
      case LoopBoyDown:
        // All blocks in an hblockgroup are horizontally aligned so only one y check is needed
        if (box_collision_x(
                data->x.hl.h,
                data->x.hl.h + LOOPBOY_SIZE,
                player_data->x.hl.h,
                player_data->x.hl.h + PLAYER_SIZE)) {
            // TODO I think with some bit cleverness we can avoid a lot of work here
            for (i = 0; i < data->n; i++) {
                j = (((i << LOOPBOY_DOUBLE_OFFSET_LOG2) + (data->d)) & LOOPBOY_WRAPPING_FACTOR) >> 1;

                if (box_collision_y(
                        data->y.hl.h - j,
                        data->y.hl.h - j + LOOPBOY_SIZE,
                        player_data->y.hl.h,
                        player_data->y.hl.h + PLAYER_SIZE)) {
                    return ResultFail;
                }
            }
        }
        break;
      case LoopBoyUp:
        // All blocks in an hblockgroup are horizontally aligned so only one y check is needed
        if (box_collision_x(
                data->x.hl.h,
                data->x.hl.h + LOOPBOY_SIZE,
                player_data->x.hl.h,
                player_data->x.hl.h + PLAYER_SIZE)) {
            // TODO I think with some bit cleverness we can avoid a lot of work here
            for (i = 0; i < data->n; i++) {
                j = (((i << LOOPBOY_DOUBLE_OFFSET_LOG2) + (data->d)) & LOOPBOY_WRAPPING_FACTOR) >> 1;

                if (box_collision_y(
                        data->y.hl.h + j,
                        data->y.hl.h + j + LOOPBOY_SIZE,
                        player_data->y.hl.h,
                        player_data->y.hl.h + PLAYER_SIZE)) {
                    return ResultFail;
                }
            }
        }
        break;
  }


  data->d--;
  if (data->d == 0) {
      data->d = 128;
  }

  return ResultOk;
}

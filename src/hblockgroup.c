#include "hblockgroup.h"
#include "common.h"
#include "drawing_funcs.h"

#include "gametank.h"
#include "banking.h"
#include "gt/drawing_funcs.h"

void init_hblockgroup(char x, char y, char n, char d) {
  HBlockGroupData *data;
  char i;

  for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
    if (entities[i] == EntityEmpty) {
      entities[i] = EntityHBlockGroup;

      data = (HBlockGroupData *) &entity_data[i];

      data->x.hl.h = x;
      data->x.hl.l = 0;
      data->y.hl.h = y;
      data->y.hl.l = 0;
      data->n = n;
      data->d_total = d;
      data->d_remaining = d;

      return;
    }
  }

  // This should never be reached!!
}

#define PREP_DRAW_BLOCK(y)                    \
    do {                                      \
        *dma_flags = flagsMirror | DMA_COLORFILL_ENABLE | DMA_OPAQUE; \
        vram[VY] = y;                         \
        vram[GX] = 0;                         \
        vram[GY] = 0;                         \
        vram[WIDTH] = 6;                      \
        vram[HEIGHT] = 6;                     \
        vram[COLOR] = ~HBLOCK_COLOR;          \
    } while (0)

#define DRAW_BLOCK(x)                                       \
    do {                                                    \
    vram[VX] = x;                                           \
    vram[START] = 1;                                        \
} while (0)

void draw_hblockgroup(char ix) {
  HBlockGroupData data;
  char i;

  data = *((HBlockGroupData *) &entity_data[ix]);

  await_draw_queue();

  PREP_DRAW_BLOCK(data.y.hl.h);

  for (i = 0; i < data.n; i++) {
      DRAW_BLOCK(data.x.hl.h + (i * HBLOCK_OFFSET));
  }
}

void update_hblockgroup(char ix) {
  HBlockGroupData *data;

  data = (HBlockGroupData *) &entity_data[ix];

  if ((signed char) data->d_remaining > 0) {
      data->y.hl.h++;
      data->d_remaining--;
  } else if ((signed char) data->d_remaining < 0) {
      data->y.hl.h--;
      data->d_remaining++;
  } if (data->d_remaining == 0) {
      data->d_total = -data->d_total;
      data->d_remaining = data->d_total;
  }

}

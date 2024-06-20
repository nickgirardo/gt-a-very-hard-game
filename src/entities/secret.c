#include "secret.h"
#include "player.h"


void init_secret(char x, char y) {
  SecretData *data;
  char i;

  for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
    if (entities[i] == EntityEmpty) {
      entities[i] = EntitySecret;

      data = (SecretData *) &entity_data[i];

      data->x = x;
      data->y = y;

      return;
    }
  }
};

// Secrets are not drawn
void draw_secret(char ix) {
#ifdef RENDER_SECRETS
  SecretData data;

  data = *((SecretData *) &entity_data[ix]);
  draw_box(data.x, data.y, 1, 1, 1);
#endif
  return;
}

// Secrets have no non-collision updates
void update_secret(char ix) {
  return;
}

CollisionResult collision_secret(char ix) {
  SecretData data;

  data = *((SecretData *) &entity_data[ix]);

  if (point_collision(
          data.x,
          data.y,
          player_data->x.hl.h,
          player_data->r,
          player_data->y.hl.h,
          player_data->d)) {
    return ResultGetSecret;
  }

  return ResultOk;

}

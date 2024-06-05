#ifndef SECRET_H_
#define SECRET_H_

#include "../common.h"

// For debugging purposes
// #define RENDER_SECRETS

void init_secret(char x, char y);
void draw_secret(char ix);
void update_secret(char ix);
CollisionResult collision_secret(char ix);


#endif // SECRET_H_

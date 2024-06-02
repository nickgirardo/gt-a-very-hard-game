#ifndef TILEMAP_H_
#define TILEMAP_H_

#define DECOR_COLOR 6

void init_tilemap(const unsigned char *map, const unsigned char *decor);
void draw_tilemap_full();
void draw_tilemap_partial();

#endif // TILEMAP_H_

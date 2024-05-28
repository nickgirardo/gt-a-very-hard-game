#ifndef COMMON_H_
#define COMMON_H_

#include <stdbool.h>

#define ENTITY_TABLE_SIZE 8

// Tilemap related definitions
#define TILE_SIZE 8
#define TILEMAP_WIDTH 16
#define TILEMAP_HEIGHT 16
#define TILEMAP_SIZE (TILEMAP_HEIGHT * TILEMAP_WIDTH)

#define rand_direction() (rnd() & 3)

#define box_collision(L1, R1, U1, D1, L2, R2, U2, D2) \
  (L1 < R2 && \
            R1 > L2 && \
            U1 < D2 && \
            D1 > U2)

typedef struct HLT {
  char l;
  char h;
} HL;

typedef union CoordU {
  short c;
  HL hl;
} Coord;

typedef enum { DirLeft = 0, DirRight, DirUp, DirDown } Direction;

typedef enum { EntityEmpty = 0, EntityPlayer, EntityGoomba, EntityMaggot, EntityShooter, EntityBullet } EntityKind;

typedef struct PlayerDataT {
  Coord x;
  Coord y;
  Coord vx;
  Coord vy;
} PlayerData;

typedef struct GoombaDataT {
  Coord x;
  Coord y;
  Coord vx;
  Coord vy;
} GoombaData;

typedef struct MaggotDataT {
  Coord x;
  Coord y;
  Direction dir;
  char timer;
  bool charging;
} MaggotData;

typedef struct ShooterDataT {
  Coord x;
  Coord y;
  char timer;
} ShooterData;

typedef struct BulletDataT {
  Coord x;
  Coord y;
  Coord vx;
  Coord vy;
  char ttl;
} BulletData;

typedef union EntityDataU {
  PlayerData pd;
  GoombaData gd;
  MaggotData md;
  ShooterData sd;
  BulletData bd;
} EntityData;

extern EntityKind entities[ENTITY_TABLE_SIZE];
extern PlayerData *player_data;
extern EntityData entity_data[ENTITY_TABLE_SIZE];
extern unsigned char tilemap[TILEMAP_SIZE];

#endif // COMMON_H_
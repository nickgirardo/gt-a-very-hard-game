#ifndef COMMON_H_
#define COMMON_H_

#include <stdbool.h>

#define ENTITY_TABLE_SIZE 8

// Tilemap related definitions
#define TILE_SIZE 8
#define TILEMAP_WIDTH 16
#define TILEMAP_HEIGHT 16
#define TILEMAP_SIZE (TILEMAP_HEIGHT * TILEMAP_WIDTH)

#define TILE_WALL 0
#define TILE_KILL 0xBB
#define TILE_GOAL 0x14

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

typedef enum { EntityEmpty = 0, EntityPlayer, EntityGoomba, EntityMaggot, EntityShooter, EntityBullet, EntityHBlockGroup } EntityKind;

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

typedef struct HBlockGroupDataT {
  Coord x;
  Coord y;
  char n;
  char d_total;
  char d_remaining;
} HBlockGroupData;

typedef union EntityDataU {
  PlayerData pd;
  GoombaData gd;
  MaggotData md;
  ShooterData sd;
  BulletData bd;
  HBlockGroupData hbgd;
} EntityData;

extern EntityKind entities[ENTITY_TABLE_SIZE];
extern PlayerData *player_data;
extern EntityData entity_data[ENTITY_TABLE_SIZE];
extern unsigned char tilemap[TILEMAP_SIZE];
extern unsigned char tilemap_decor[24];
extern void reset_level();

#endif // COMMON_H_

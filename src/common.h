#ifndef COMMON_H_
#define COMMON_H_

#define ENTITY_TABLE_SIZE 8

typedef struct HLT {
  char l;
  char h;
} HL;

typedef union CoordU {
  short c;
  HL hl;
} Coord;

typedef enum { EntityEmpty = 0, EntityPlayer, EntityGoomba } EntityKind;

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

typedef union EntityDataU {
  PlayerData pd;
  GoombaData gd;
} EntityData;

extern EntityKind entities[ENTITY_TABLE_SIZE];
extern PlayerData *player_data;
extern EntityData entity_data[ENTITY_TABLE_SIZE];

#endif // COMMON_H_

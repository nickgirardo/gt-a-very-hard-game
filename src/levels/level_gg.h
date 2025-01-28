#ifndef LEVEL_GG_H_
#define LEVEL_GG_H_

#include "../common.h"

#pragma rodata-name (push, "PROG0")

const unsigned char level_gg[TILEMAP_SIZE] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 6, 5, 6, 5, 6, 5, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 5, 0x5c, 0x3c, 0x1c, 5, 6, 5, 6, 5, 6, 5, 0xFC, 0, 0,
0, 0, 6, 0x7c, 6, 5, 6, 5, 6, 0xfc, 0xdc, 0xbc, 6, 5, 0, 0,
0, 0, 5, 0x9c, 5, 6, 5, 6, 5, 0x1c, 5, 6, 5, 6, 0, 0,
0, 0, 6, 0xBc, 6, 0x5c, 0x9c, 5, 6, 0x3d, 6, 5, 6, 5, 0, 0,
0, 0, 5, 0xDc, 5, 6, 0x7c, 6, 5, 0x5d, 5, 0x9d, 0x3c, 6, 0, 0,
0, 0, 6, 0xFc, 0x1c, 0x3c, 0x5c, 5, 6, 0x7d, 6, 5, 0x1c, 5, 0, 0,
0, 0, 5, 6, 5, 6, 5, 6, 5, 0x9c, 0xbc, 0xdc, 0xfc, 6, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 6, 5, 6, 5, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

const unsigned char level_gg_decor[] = {
24,
1, 24, 126, 15,
1, 39, 14, 74,
113, 39, 14, 74,
1, 113, 126, 14,
15, 105, 48, 8,
65, 39, 48, 8,
};

const unsigned char level_gg_entities[] = {
EntitySecretReward,17,96,
EntityPlayer,59,61,
EntityEmpty,
};

const unsigned char level_gg_reset_data[] = {
84,73,
};

const char level_gg_name[] = "Thx 4 playin!!";

#pragma rodata-name (pop)

#endif // LEVEL_GG_H_

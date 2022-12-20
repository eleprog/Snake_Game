#include <avr/pgmspace.h>

// ������ ������
#define TILE_BODY	0
#define TILE_NULL	6
#define TILE_FOOD	7
#define TILE_TURN	8
#define TILE_HEAD	16
#define TILE_TAIL	24
#define TILE_BONUS	28

const uint8_t tiles[40][2] PROGMEM = {
		{ 0xb0, 0xd },
		{ 0x26, 0x64 },
		{ 0xd0, 0xb },
		{ 0x46, 0x62 },
		{ 0xb6, 0x6d },
		{ 0xd6, 0x6b },
		{ 0x0, 0x0 },
		{ 0x52, 0x2 },
		{ 0xc0, 0x6a },
		{ 0x30, 0x65 },
		{ 0x56, 0x3 },
		{ 0xa6, 0xc },
		{ 0xc0, 0xea },
		{ 0x30, 0x75 },
		{ 0x57, 0x3 },
		{ 0xae, 0xc },
		{ 0x61, 0x7 },
		{ 0x65, 0x6 },
		{ 0x68, 0xe },
		{ 0x60, 0x56 },
		{ 0x25, 0x43 },
		{ 0x65, 0x9 },
		{ 0x4a, 0x2c },
		{ 0x90, 0x56 },
		{ 0xc0, 0xf },
		{ 0x44, 0x66 },
		{ 0x30, 0xf },
		{ 0x66, 0x44 },
		{ 0xac, 0xcf },
		{ 0x50, 0x37 },
		{ 0xfc, 0x5d },
		{ 0xf3, 0xab },
		{ 0x33, 0xc },
		{ 0x72, 0x5f },
		{ 0x90, 0xed },
		{ 0x53, 0xf7 },
		{ 0xda, 0x4f },
		{ 0x72, 0x2f },
		{ 0x10, 0xaf },
		{ 0x0, 0xaf }};
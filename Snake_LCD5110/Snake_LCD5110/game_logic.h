#define PLAYING_FIELD_X 20
#define PLAYING_FIELD_Y 9

#define TILE_TYPE_NULL		0b00
#define TILE_TYPE_FOOD		0b01
#define TILE_TYPE_BODY		0b10
#define TILE_TYPE_FILLED	0b11

typedef struct {
	uint8_t tileNumber	:6;
	uint8_t tileFood	:1;
	uint8_t tileBody	:1;
} mapCell;

struct {
	uint8_t length;
	uint8_t turn: 2;
	uint8_t turnOld: 2;
	uint8_t head;
	uint8_t tail;
}snakeData;

mapCell map[180];
uint8_t snakeBody[180];


void Game_Init();
void Game_Map_Output();
void Game_Cycle();
uint8_t Game_Head_Collision(uint8_t head);
void Game_Map_Clear();


#include "game_logic.c"
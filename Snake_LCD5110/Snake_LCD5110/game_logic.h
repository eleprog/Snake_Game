#define PLAYING_FIELD_X 20
#define PLAYING_FIELD_Y 9

typedef struct {
	uint8_t tileNumber	:6;
	uint8_t tileType	:2;
} mapCell;

struct {
	uint8_t length;
	uint8_t turn;
	uint8_t head;
	uint8_t tail;
}snakeData;

mapCell map[180];
uint8_t snakeBody[180];


void Game_Init();
void Game_Map_Output();
void Game_Map_Clear();


#include "game_logic.c"
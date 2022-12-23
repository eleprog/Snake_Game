#define PLAYING_FIELD_X 20
#define PLAYING_FIELD_Y 9

#define TILE_TYPE_NULL		0b00
#define TILE_TYPE_FOOD		0b01
#define TILE_TYPE_BODY		0b10
#define TILE_TYPE_FILLED	0b11

#define TILE_CHECK_BODY		0b10
#define TILE_CHECK_FOOD		0b01

#define COLLISION_NULL 0
#define COLLISION_FOOD 1
#define COLLISION_BODY 2
#define COLLISION_BONUS 3

typedef struct {
	uint8_t tileNumber	:6;
	uint8_t tileFood	:1;
	uint8_t tileBody	:1;
} mapCell;

struct {
	uint8_t turn		:2;
	uint8_t turnOld		:2;
	uint8_t difficulty	:3;
	uint8_t headPointer;
	uint8_t tailPointer;
	uint16_t score;
}snakeData;

mapCell map[180];
uint8_t snakeBody[180];

void Game_Init();
void Game_Cycle();
void Game_Map_Output();
void Game_Map_Clear();
void Game_Spawn_Food();
void Game_Draw_Tail();
void Game_Draw_Head(uint8_t pointer);
void Game_Draw_Frame();
uint8_t Game_Check_Collision(uint8_t pointer);
uint8_t Game_Calc_Pointer_Next_Step(uint8_t pointer);
void Game_Score_Output();


#include "game_logic.c"
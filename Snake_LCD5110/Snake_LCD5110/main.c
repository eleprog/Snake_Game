#include <avr/io.h>
#include <avr/pgmspace.h>

#include "LCD5110.h"
#include "tiles.h"

typedef struct {
	uint8_t tilelNumber	:6;
	uint8_t tileType	:2;
} mapCell;

mapCell map[180];
uint8_t snakeBody[180];

void Tiles_Print()
{
	for(uint8_t i = 0; i < 5; i++)
		for (uint8_t j = 0; j < 8; j++)
		{
			LCD5110_Setpos(j*5, i);
			
			LCD5110_Send(pgm_read_byte(&(tiles[i * 8 +j][0])) & 0x0F);
			LCD5110_Send(pgm_read_byte(&(tiles[i * 8 +j][0])) >> 4);
			
			LCD5110_Send(pgm_read_byte(&(tiles[i * 8 +j][1])) & 0x0F);
			LCD5110_Send(pgm_read_byte(&(tiles[i * 8 +j][1])) >> 4);
		}
}


int main(void)
{
	LCD5110_Init();
	Tiles_Print();
	
    while (1)
    {
		//LCD5110_Send(tiles[1][1]);
		//LCD5110_Send(map[0].tilelNumber);
    }
}


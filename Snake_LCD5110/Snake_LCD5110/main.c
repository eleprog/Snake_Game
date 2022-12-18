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


int main(void)
{
	LCD5110_Init();
    while (1)
    {
		LCD5110_Send(map[0].tilelNumber);
    }
}


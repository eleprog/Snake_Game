#include <avr/io.h>
#include <avr/pgmspace.h>

#include "LCD5110.h"
#include "graphics/tiles.h"
#include "graphics/numbers.h"

#define PLAYING_FIELD_X 20
#define PLAYING_FIELD_Y 9

typedef struct {
	uint8_t tileNumber	:6;
	uint8_t tileType	:2;
} mapCell;

mapCell map[180];
uint8_t snakeBody[180];

//////////* v(?_?)v *//////////
void Map_Output() {
	for(uint8_t bank = 0; bank < 5; bank++) {
		LCD5110_Setpos(2, bank + 1);
		
		for(uint8_t i = 0; i < PLAYING_FIELD_X; i++) {
				
			uint8_t tmp[4] = {0,0,0,0};					
			switch (bank) {
				case 0:	
				tmp[0] = 0x0F;
				tmp[1] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 0].tileNumber] [0]));
				tmp[2] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 0].tileNumber] [1]));
				tmp[3] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 1].tileNumber] [0]));
				break;
			case 1:
				tmp[0] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 1].tileNumber] [1]));
				tmp[1] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 2].tileNumber] [0]));
				tmp[2] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 2].tileNumber] [1]));
				tmp[3] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 3].tileNumber] [0]));
				break;
			case 2:
				tmp[0] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 3].tileNumber] [1]));
				tmp[1] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 4].tileNumber] [0]));
				tmp[2] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 4].tileNumber] [1]));
				tmp[3] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 5].tileNumber] [0]));
				break;
			case 3:
				tmp[0] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 5].tileNumber] [1]));
				tmp[1] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 6].tileNumber] [0]));
				tmp[2] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 6].tileNumber] [1]));
				tmp[3] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 7].tileNumber] [0]));
				break;
			case 4:
				tmp[0] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 7].tileNumber] [1]));
				tmp[1] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 8].tileNumber] [0]));
				tmp[2] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 8].tileNumber] [1]));
				tmp[3] = 0xF0;
				break;
		}
			uint8_t data[4] = {0,0,0,0};
			
			for(uint8_t j = 0; j < 4; j++) {
				if(tmp[0] & 1<<(0+j)) data[j] |= 1<<0;
				if(tmp[0] & 1<<(4+j)) data[j] |= 1<<1;
				if(tmp[1] & 1<<(0+j)) data[j] |= 1<<2;
				if(tmp[1] & 1<<(4+j)) data[j] |= 1<<3;
				if(tmp[2] & 1<<(0+j)) data[j] |= 1<<4;
				if(tmp[2] & 1<<(4+j)) data[j] |= 1<<5;
				if(tmp[3] & 1<<(0+j)) data[j] |= 1<<6;
				if(tmp[3] & 1<<(4+j)) data[j] |= 1<<7;
			}
			for(uint8_t k = 0; k < 4; k++)
				LCD5110_Send(data[k]);
		}
	}
}

int main(void)
{	
	LCD5110_Init();
	
	for(uint8_t i = 0; i < 180; i++)
		map[i].tileNumber = TILE_HEAD+1;
	
	Map_Output();
    while (1)
    {
		
    }
}


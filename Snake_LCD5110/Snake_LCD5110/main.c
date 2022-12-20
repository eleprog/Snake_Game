﻿#define F_CPU 8000000

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "LCD5110.h"
#include "graphics/tiles.h"
#include "graphics/numbers.h"

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

void Timer1_Init()
{
	TCCR1B = (1<<CS22)|(0<<CS21)|(1<<CS20);	// Установка предделителя (1024) таймера 1
	TCNT1 = 0;								// Обнуление счетного регистра таймера 1
	OCR1A = 2000;							// Установка регистра сравнения таймера 1
	TIMSK |= (1<<OCIE1A);					// разрешение прерываний
}

//////////* v(ಠ_ಠ)v *//////////
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

ISR(TIMER1_COMPA_vect)
{
	TCNT1 = 0;
	map[snakeData.head++].tileNumber = TILE_BODY;
	map[snakeData.head].tileNumber = TILE_HEAD;
	
	map[snakeData.tail++].tileNumber = TILE_NULL;
	map[snakeData.tail].tileNumber = TILE_TAIL;
	Map_Output();
}
int main(void)
{	
	snakeData.head = 6;
	snakeData.turn = 0;
	snakeData.tail = 0;
	
	for (uint8_t i = 0; i < 180; i++) {
		map[i].tileNumber = TILE_NULL;
	}
	
	for (uint8_t i = 0; i < 6; i++) {
		map[i].tileNumber = TILE_BODY;
	}
	
	LCD5110_Init();
	
	Timer1_Init();
	
	asm("sei");
	
    while (1)
    {
		
    }
}


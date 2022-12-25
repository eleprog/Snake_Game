﻿#define F_CPU 8000000

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "graphics/tiles.h"
#include "graphics/numbers.h"

#include "LCD5110.h"

#include "game_logic.h"


#define BUTTON_POLL_FREQ 20

/////////////* button pinout connection */////////////
#define BUTTON_DDR	DDRC
#define BUTTON_PORT PORTC
#define BUTTON_PIN	PINC

#define BUTTON_ENTER_PIN	1
#define BUTTON_LEFT_PIN		2
#define BUTTON_DOWN_PIN		3
#define BUTTON_RIGHT_PIN	4
#define BUTTON_UP_PIN		5


struct {
	uint8_t gameCycle	:1;
}flags;


void GPIO_Init();
void Timer0_Init();
void Timer1_Init();

void Game_Button_Handler();


void GPIO_Init() {
	BUTTON_DDR &= ~((1<<BUTTON_UP_PIN)|(1<<BUTTON_DOWN_PIN)|(1<<BUTTON_LEFT_PIN)|(1<<BUTTON_RIGHT_PIN)|(1<<BUTTON_ENTER_PIN));
	BUTTON_PORT |= (1<<BUTTON_UP_PIN)|(1<<BUTTON_DOWN_PIN)|(1<<BUTTON_LEFT_PIN)|(1<<BUTTON_RIGHT_PIN)|(1<<BUTTON_ENTER_PIN);
}

void Timer0_Init() {
	TCCR0 = (0<<CS22)|(1<<CS21)|(1<<CS20);	// установка предделителя (64) таймера 0
	TCNT0 = 0;								// обнуление счетного регистра таймера 0
	TIMSK |= (1<<TOIE0);					// разрешение прерываний по переполнению
}

void Timer1_Init() {
	TCCR1B = (1<<CS22)|(0<<CS21)|(1<<CS20);	// установка предделителя (1024) таймера 1
	TCNT1 = 0;								// обнуление счетного регистра таймера 1
	OCR1A = 2000;							// установка регистра сравнения таймера 1
	TIMSK |= (1<<OCIE1A);					// разрешение прерываний по совпадению
}

ISR(TIMER0_OVF_vect) {
	TCNT0 = 0;
	
	static uint8_t counter = 0;
	static const uint8_t comp = F_CPU / 64 / 256 / BUTTON_POLL_FREQ;
	counter++;
	
	if(counter > comp) {
		Game_Button_Handler();
		counter = 0;
	}
}

ISR(TIMER1_COMPA_vect) {
	TCNT1 = 0;
	flags.gameCycle = 1;
}

void Game_Button_Handler() {
	if(!(BUTTON_PIN & 1<<BUTTON_RIGHT_PIN) && snakeData.turnOld % 2)
		snakeData.turn = 0;
		
	else if(!(BUTTON_PIN & 1<<BUTTON_DOWN_PIN) && !(snakeData.turnOld % 2))
		snakeData.turn = 1;
		
	else if(!(BUTTON_PIN & 1<<BUTTON_LEFT_PIN) && snakeData.turnOld % 2)
		snakeData.turn = 2;
		
	else if(!(BUTTON_PIN & 1<<BUTTON_UP_PIN) && !(snakeData.turnOld % 2))
		snakeData.turn = 3;
}

int main(void)
{
	LCD5110_Init();
	GPIO_Init();
	Timer0_Init();
	Timer1_Init();
	
	srand(2);
	Game_Init();
	Game_Map_Output();
	asm("sei");
	
    while (1)
    {
		if(flags.gameCycle) {
			Game_Cycle();
			Game_Button_Handler();
			flags.gameCycle = 0;
		}
    }
}


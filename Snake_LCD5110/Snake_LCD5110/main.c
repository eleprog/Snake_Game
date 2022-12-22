#define F_CPU 8000000

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "LCD5110.h"
#include "graphics/tiles.h"
#include "graphics/numbers.h"

#include "game_logic.h"

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
	uint8_t collision	:1;
}flags;

void GPIO_Init() {
	BUTTON_DDR &= ~((1<<BUTTON_UP_PIN)|(1<<BUTTON_DOWN_PIN)|(1<<BUTTON_LEFT_PIN)|(1<<BUTTON_RIGHT_PIN)|(1<<BUTTON_ENTER_PIN));
	BUTTON_PORT |= (1<<BUTTON_UP_PIN)|(1<<BUTTON_DOWN_PIN)|(1<<BUTTON_LEFT_PIN)|(1<<BUTTON_RIGHT_PIN)|(1<<BUTTON_ENTER_PIN);
}

void Timer1_Init() {
	TCCR1B = (1<<CS22)|(0<<CS21)|(1<<CS20);	// Установка предделителя (1024) таймера 1
	TCNT1 = 0;								// Обнуление счетного регистра таймера 1
	OCR1A = 4000;							// Установка регистра сравнения таймера 1
	TIMSK |= (1<<OCIE1A);					// разрешение прерываний
}

ISR(TIMER1_COMPA_vect) {
	TCNT1 = 0;
	flags.gameCycle = 1;
}

void Game_Button_Handler() {
	if(!(BUTTON_PIN & 1<<BUTTON_RIGHT_PIN) && snakeData.turn != 2)
		snakeData.turn = 0;
		
	if(!(BUTTON_PIN & 1<<BUTTON_DOWN_PIN) && snakeData.turn != 3)
		snakeData.turn = 1;
		
	if(!(BUTTON_PIN & 1<<BUTTON_LEFT_PIN) && snakeData.turn != 0)
		snakeData.turn = 2;
		
	if(!(BUTTON_PIN & 1<<BUTTON_UP_PIN) && snakeData.turn != 1)
		snakeData.turn = 3;
}

int main(void)
{
	LCD5110_Init();
	GPIO_Init();
	Timer1_Init();
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


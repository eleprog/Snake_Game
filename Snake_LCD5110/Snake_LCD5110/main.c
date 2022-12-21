#define F_CPU 8000000

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "LCD5110.h"
#include "graphics/tiles.h"
#include "graphics/numbers.h"

#include "game_logic.h"

/////////////* button pinout connection */////////////
#define BUTTON_UP_DDR		DDRB
#define BUTTON_UP_PORT		PORTB
#define BUTTON_UP_PIN		1

#define BUTTON_DOWN_DDR		DDRD
#define BUTTON_DOWN_PORT	PORTD
#define BUTTON_DOWN_PIN		5

#define BUTTON_LEFT_DDR		DDRD
#define BUTTON_LEFT_PORT	PORTD
#define BUTTON_LEFT_PIN		6

#define BUTTON_RIGHT_DDR	DDRD
#define BUTTON_RIGHT_PORT	PORTD
#define BUTTON_RIGHT_PIN	7

#define BUTTON_ENTER_DDR	DDRB
#define BUTTON_ENTER_PORT	PORTB
#define BUTTON_ENTER_PIN	0


struct {
	uint8_t gameCycle	:1;
	uint8_t collision	:1;
}flags;

void GPIO_Init() {
	BUTTON_UP_DDR &= ~(0<<BUTTON_UP_PIN);
	BUTTON_DOWN_DDR &= ~(0<<BUTTON_DOWN_PIN);
	BUTTON_LEFT_DDR &= ~(0<<BUTTON_LEFT_PIN);
	BUTTON_RIGHT_DDR &= ~(0<<BUTTON_RIGHT_PIN);
	BUTTON_ENTER_DDR &= ~(0<<BUTTON_ENTER_PIN);
}

void Timer1_Init() {
	TCCR1B = (1<<CS22)|(0<<CS21)|(1<<CS20);	// Установка предделителя (1024) таймера 1
	TCNT1 = 0;								// Обнуление счетного регистра таймера 1
	OCR1A = 5000;							// Установка регистра сравнения таймера 1
	TIMSK |= (1<<OCIE1A);					// разрешение прерываний
}

ISR(TIMER1_COMPA_vect) {
	TCNT1 = 0;
	flags.gameCycle = 1;	
}

void Game_Button_Handler() {
	//if()
}



int main(void)
{	
	LCD5110_Init();
	GPIO_Init();
	Timer1_Init();
	Game_Init();
	Game_Map_Output();
	asm("sei");
	
	snakeData.turn = 2;
    while (1)
    {
		if(flags.gameCycle) {
			Game_Cycle();
			flags.gameCycle = 0;
		}
    }
}


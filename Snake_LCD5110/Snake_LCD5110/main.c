#define F_CPU 8000000

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
	uint8_t buttonCheck	:1;
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
	TCCR0 = (0<<CS22)|(1<<CS21)|(1<<CS20);	// setting the prescaler (64) timer 0
	TCNT0 = 0;								// zeroing the counting register of the timer 0
	TIMSK |= (1<<TOIE0);					// set interrupt for overflow
}

void Timer1_Init() {
	TCCR1B = (1<<CS22)|(0<<CS21)|(1<<CS20);	// setting the prescaler (1024) timer 1
	TCNT1 = 0;								// zeroing the counting register of the timer 1
	OCR1A = 2000;							// setting of comparison register timer 1
	TIMSK |= (1<<OCIE1A);					// set interrupt for match
}

ISR(TIMER0_OVF_vect) {
	TCNT0 = 0;
	
	static uint8_t counter = 0;
	static const uint8_t comp = F_CPU / 64 / 256 / BUTTON_POLL_FREQ;
	counter++;
	
	if(counter > comp) {
		flags.buttonCheck = 1;
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
			flags.gameCycle = 0;
		}
		if(flags.buttonCheck) {
			Game_Button_Handler();
			flags.buttonCheck = 0;
		}
    }
}


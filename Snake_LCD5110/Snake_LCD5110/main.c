#include <avr/io.h>
#include <avr/pgmspace.h>

#include "LCD5110.h"


//const uint8_t tileArr[][] PROGMEM = {1,1,1,1,1,1};


int main(void)
{
	LCD5110_Init();
    while (1) 
    {
		LCD5110_Send(0xFF);
    }
}


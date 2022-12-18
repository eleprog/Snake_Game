#include "LCD5110.h"

void LCD_Init()
{
	RESET_PORT |= 1<<RESET_PIN;
	LCD_Write(0x21, COMMAND);
	LCD_Write(0x13, COMMAND);
	LCD_Write(0x04, COMMAND);
	LCD_Write(0xBF, COMMAND);
	LCD_Write(0x20, COMMAND);
	LCD_Write(0x0C, COMMAND);
	LCD_Clear();
}

void LCD_Write(uint8_t data, uint8_t mode)
{
	if(mode) DC_PORT |= 1<<DC_PIN; // Выбор команда/данные
	else DC_PORT &= ~(1<<DC_PIN);
	
	CE_PORT &= ~(1<<CE_PIN);
	for(uint8_t i = 0; i < 8; i++)
	{
		CLK_PORT &= ~(1<<CLK_PIN);

		if(data & 0x80) DIN_PORT |= 1<<DIN_PIN;
		else DIN_PORT &= ~(1<<DIN_PIN);

		data <<= 1;

		CLK_PORT |= 1<<CLK_PIN;
	}
	CE_PORT |= 1<<CE_PIN;
}

void LCD_Clear()
{
	LCD_Setpos(0,0);
	for(uint16_t i = 0; i < LCD_X * LCD_Y; i++)
	LCD_Write(0, DATA);
	LCD_Setpos(0,0);
}

void LCD_Setpos(uint8_t X, uint8_t Y)
{
	LCD_Write(0x80 + X, COMMAND);
	LCD_Write(0x40 + Y, COMMAND);
}


#ifndef LCD5110_H
#define LCD5110_H

	#define RESET_PORT	PORTB
	#define RESET_PIN	1
	#define CE_PORT		PORTD
	#define CE_PIN		5
	#define DC_PORT		PORTD
	#define DC_PIN		6
	#define DIN_PORT	PORTD
	#define DIN_PIN		7
	#define CLK_PORT	PORTB
	#define CLK_PIN		0

	#define COMMAND 0
	#define DATA	1

	#define LCD_X	84  // Количество столбцов на дисплее
	#define LCD_Y	6   // Количество строк на дисплее


	//void LCD_Init();
	void LCD_Write(uint8_t data, uint8_t mode);
	void LCD_Clear();
	void LCD_Setpos(uint8_t X, uint8_t Y);

	#include "LCD5110.c"

#endif /* LCD5110_H */
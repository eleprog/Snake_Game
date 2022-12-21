void Game_Init() {
	snakeData.head = PLAYING_FIELD_X * 4 + 13;
	snakeData.tail = PLAYING_FIELD_X * 4 + 7;
	snakeData.length = 6;
	snakeData.turn = 0;
	Game_Map_Clear();
	
	map[snakeData.head].tileNumber = TILE_HEAD;
	map[snakeData.tail].tileNumber = TILE_TAIL;
	
	for (uint8_t i = snakeData.tail + 1; i < snakeData.head; i++)
		map[i].tileNumber = TILE_BODY;
		
	Game_Map_Output();	
}

//////////* v(ಠ_ಠ)v *//////////
void Game_Map_Output() {
	for(uint8_t bank = 1; bank < 6; bank++) {
		LCD5110_Setpos(2, bank);
		
		for(uint8_t i = 0; i < PLAYING_FIELD_X; i++) {
			
			uint8_t tmp[4] = {0,0,0,0};
			if(bank == 1) {
				tmp[0] = 0x0F;
				tmp[1] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 0].tileNumber] [0]));
				tmp[2] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 0].tileNumber] [1]));
				tmp[3] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 1].tileNumber] [0]));
			}
			else if(bank == 2) {
				tmp[0] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 1].tileNumber] [1]));
				tmp[1] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 2].tileNumber] [0]));
				tmp[2] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 2].tileNumber] [1]));
				tmp[3] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 3].tileNumber] [0]));
			}
			else if(bank == 3) {
				tmp[0] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 3].tileNumber] [1]));
				tmp[1] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 4].tileNumber] [0]));
				tmp[2] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 4].tileNumber] [1]));
				tmp[3] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 5].tileNumber] [0]));
			}
			else if(bank == 4) {
				tmp[0] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 5].tileNumber] [1]));
				tmp[1] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 6].tileNumber] [0]));
				tmp[2] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 6].tileNumber] [1]));
				tmp[3] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 7].tileNumber] [0]));
			}
			else {
				tmp[0] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 7].tileNumber] [1]));
				tmp[1] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 8].tileNumber] [0]));
				tmp[2] = pgm_read_byte(&(tiles[map[i + PLAYING_FIELD_X * 8].tileNumber] [1]));
				tmp[3] = 0xF0;
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

void Game_Cycle() {
	int8_t x = snakeData.head % 20;
	int8_t y = snakeData.head / 20;
	
	// вычисление новых координат головы змейки
	if(snakeData.turn == 0 && ++x == PLAYING_FIELD_X)		// right
	x = 0;
	else if(snakeData.turn == 1 && ++y == PLAYING_FIELD_Y)	// down
	y = 0;
	else if(snakeData.turn == 2 && --x < 0)					// left
	x = PLAYING_FIELD_X - 1;
	else if(snakeData.turn == 3 && --y < 0)					// right
	y = PLAYING_FIELD_Y - 1;
		
	// head draw
	map[x + y * PLAYING_FIELD_X].tileNumber = TILE_HEAD + snakeData.turn;
	
	// если угол поворота не изменился
	if(snakeData.turn == snakeData.turnOld)
		map[snakeData.head].tileNumber = TILE_BODY + snakeData.turn;
	
	// если угол поворота изменился
	else {
		if(snakeData.turnOld == 0) {	// right
			if(snakeData.turn == 1)			// down
				map[snakeData.head].tileNumber = TILE_TURN + snakeData.turn;		// 1 | 1
			else							// up
				map[snakeData.head].tileNumber = TILE_TURN + snakeData.turn - 1;	// 3 | 2
		}	
		if(snakeData.turnOld == 1) {	// down
			if(snakeData.turn == 2)			// left
				map[snakeData.head].tileNumber = TILE_TURN + snakeData.turn;		// 2 | 2
			else							// right
				map[snakeData.head].tileNumber = TILE_TURN + snakeData.turn - 1;	// 0 | 3
		}	
		if(snakeData.turnOld == 2) {	// left
			if(snakeData.turn == 3)			// up
				map[snakeData.head].tileNumber = TILE_TURN + snakeData.turn;		// 3 | 3
			else							// down
				map[snakeData.head].tileNumber = TILE_TURN + snakeData.turn - 1;	// 1 | 0
		}	
		if(snakeData.turnOld == 3) {	// up
			if(snakeData.turn == 0)			// right
				map[snakeData.head].tileNumber = TILE_TURN + snakeData.turn;		// 0 | 0
			else							// left
				map[snakeData.head].tileNumber = TILE_TURN + snakeData.turn - 1;	// 2 | 1
		}
		snakeData.turnOld = snakeData.turn;
	}
	snakeData.head = x + y * PLAYING_FIELD_X;
	Game_Map_Output();
}

uint8_t Game_Head_Collision(uint8_t head) {
	if(map[head].tileBody)
		return 1;
		
	return 0;
}

void Game_Map_Clear() {
	for (uint8_t i = 0; i < PLAYING_FIELD_X * PLAYING_FIELD_Y; i++) 
		map[i].tileNumber = TILE_NULL;
}
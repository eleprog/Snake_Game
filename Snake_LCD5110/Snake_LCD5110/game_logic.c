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

void Game_Map_Clear() {
	for (uint8_t i = 0; i < PLAYING_FIELD_X * PLAYING_FIELD_Y; i++) 
		map[i].tileNumber = TILE_NULL;
}
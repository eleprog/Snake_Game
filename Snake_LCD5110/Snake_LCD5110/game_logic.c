void Game_Init() {
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	snakeData.difficulty = 1;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	snakeData.turn = 0;
	snakeData.turnOld = 0;
	
	snakeData.tailPointer = 0;
	snakeData.headPointer = 5;
	
	snakeData.score = 0;
	
	// заполнение массива snakeBody[] адресами в которых находится тело змейки
	for (uint8_t i = snakeData.tailPointer; i < snakeData.headPointer + 1; i++) {
		snakeBody[i] = PLAYING_FIELD_X * 4 + 7 + i;
		map[snakeBody[i]].tileBody = 1;
	}
	
	LCD5110_Clear();
	Game_Map_Clear();
	
	// заполнение массива map[] адресами на тайлы
	for (uint8_t i = snakeData.tailPointer; i < snakeData.headPointer; i++)
		map[snakeBody[i]].tileNumber = TILE_BODY;
	
	map[snakeBody[snakeData.headPointer]].tileNumber = TILE_HEAD;
	map[snakeBody[snakeData.tailPointer]].tileNumber = TILE_TAIL;
	
	Game_Draw_Frame();
	Game_Score_Output();
	Game_Spawn_Food();
	Game_Map_Output();
}

void Game_Cycle() {
	uint8_t headPointerNext = Game_Calc_Pointer_Next_Step(snakeBody[snakeData.headPointer]);
	uint8_t collision = Game_Check_Collision(headPointerNext);
	
	// обработчик столкновения с телом змейки
	if(collision == COLLISION_BODY) {
		
	}
	// если столкновения с телом змейки не было
	else {
		
		
		
		// обработчик столкновения с едой
		if(collision == COLLISION_FOOD) {
			snakeData.score += snakeData.difficulty;
			Game_Spawn_Food();
			
			if(++snakeData.bonusCount == 5)
				Game_Bonus_Handler(BONUS_SPAWN);
		}
		// обработчик столкновения с бонусной едой
		else if(collision == COLLISION_BONUS)
			Game_Bonus_Handler(BONUS_DESPAWN);
		
		// если столкновений с едой не было
		else 
			Game_Draw_Tail();
			
		Game_Bonus_Handler(BONUS_DECREMENT);
		Game_Draw_Head(headPointerNext);
	}
	Game_Score_Output();
	Game_Map_Output();
}

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

void Game_Map_Clear() {
	for (uint8_t i = 0; i < PLAYING_FIELD_X * PLAYING_FIELD_Y; i++) {
		map[i].tileNumber = TILE_NULL;
		map[i].tileBody = 0;
		map[i].tileFood = 0;
	}
}

void Game_Score_Output() {
	LCD5110_Setpos(1, 0);
	
	uint16_t digit = 1000;
	while(digit) {
		uint8_t number = snakeData.score / digit % 10;
		digit /= 10;
		
		for(uint8_t j = 0; j < 3; j++)
			LCD5110_Send(pgm_read_byte(&(numbers[number][j])));
		
		LCD5110_Send(0x40);
	}
}

void Game_Bonus_Output(uint8_t bonusType) {
	LCD5110_Setpos(65, 0);
	
	if(bonusType != TILE_NULL) {
		for(uint8_t i = 0; i < 2; i++)
		{
			uint8_t tmp[2] = {0,0};
			tmp[0] = pgm_read_byte(&(tiles[TILE_BONUS + bonusType * 2 + i][0]));
			tmp[1] = pgm_read_byte(&(tiles[TILE_BONUS + bonusType * 2 + i][1]));
			
			uint8_t data[4] = {0,0,0,0};
			for(uint8_t j = 0; j < 4; j++) {
				if(tmp[0] & 1<<(0+j)) data[j] |= 1<<1;
				if(tmp[0] & 1<<(4+j)) data[j] |= 1<<2;
				if(tmp[1] & 1<<(0+j)) data[j] |= 1<<3;
				if(tmp[1] & 1<<(4+j)) data[j] |= 1<<4;
				data[j] |= 1<<6;
			}
			
			for(uint8_t j = 0; j < 4; j++)
			LCD5110_Send(data[j]);
		}
		LCD5110_Send(0x40);
		LCD5110_Send(0x40);

		uint16_t digit = 10;
		while(digit) {
			uint8_t number = snakeData.bonusTimer / digit % 10;
			digit /= 10;
			
			for(uint8_t j = 0; j < 3; j++)
			LCD5110_Send(pgm_read_byte(&(numbers[number][j])));
			
			LCD5110_Send(0x40);
		}
	}
	else
		for(uint8_t i = 65; i < LCD5510_X; i++)
			LCD5110_Send(0x40);
}

void Game_Spawn_Food() {
	uint8_t addr = 0;
	do {
		uint8_t foodX = rand() % PLAYING_FIELD_X;
		uint8_t foodY = rand() % PLAYING_FIELD_Y;	
		
		addr = foodY * PLAYING_FIELD_X + foodX;
	} while (map[addr].tileNumber != TILE_NULL);
	
	map[addr].tileNumber = TILE_FOOD;
	map[addr].tileFood = 1;
}

void Game_Bonus_Handler(uint8_t mode) {
	static uint8_t addr = 0;
	static uint8_t bonusType = 0;
	
	if(mode == BONUS_SPAWN) {
		do {
			uint8_t foodX = rand() % (PLAYING_FIELD_X - 1);
			uint8_t foodY = rand() % PLAYING_FIELD_Y;
			
			addr = foodY * PLAYING_FIELD_X + foodX;
		} while (map[addr].tileNumber != TILE_NULL || map[addr + 1].tileNumber != TILE_NULL);
		
		bonusType = rand() % 6;
		
		map[addr].tileNumber = TILE_BONUS + bonusType * 2;
		map[addr].tileFood = 1;
		map[addr + 1].tileNumber = TILE_BONUS + 1 + bonusType * 2;
		map[addr + 1].tileFood = 1;
		
		snakeData.bonusTimer = 21;
		snakeData.bonusFlag = 1;
		Game_Bonus_Output(bonusType);
	}
	if(mode == BONUS_DESPAWN) {
		uint8_t pointer = Game_Calc_Pointer_Next_Step(snakeBody[snakeData.headPointer]);
		if(addr != pointer) {
			map[addr].tileNumber = TILE_NULL;
			map[addr].tileFood = 0;
		}
		if(addr + 1 != pointer) {
			map[addr + 1].tileNumber = TILE_NULL;
			map[addr + 1].tileFood = 0;
		}

		snakeData.score += snakeData.bonusTimer * snakeData.difficulty;
		snakeData.bonusCount = 0;
		
		snakeData.bonusFlag = 0;
		Game_Bonus_Output(TILE_NULL);
	}
	if(mode == BONUS_DECREMENT) {
		if(snakeData.bonusFlag) {
			if(snakeData.bonusTimer > 1){
				snakeData.bonusTimer--;
				Game_Bonus_Output(bonusType);
			}
			else
				Game_Bonus_Handler(BONUS_DESPAWN);
		}
	}
}

void Game_Draw_Tail() {
	map[snakeBody[snakeData.tailPointer]].tileNumber = TILE_NULL;
	map[snakeBody[snakeData.tailPointer]].tileBody = 0;
	map[snakeBody[snakeData.tailPointer]].tileFood = 0;
	
	// инкремент tailPointer
	if(++snakeData.tailPointer >= PLAYING_FIELD_X * PLAYING_FIELD_Y)
		snakeData.tailPointer = 0;
	
	uint8_t tailPointerNext = snakeData.tailPointer;
	
	if(++tailPointerNext >= PLAYING_FIELD_X * PLAYING_FIELD_Y)
		tailPointerNext = 0;
	
	uint8_t tailX = snakeBody[snakeData.tailPointer] % PLAYING_FIELD_X;
	uint8_t tailY = snakeBody[snakeData.tailPointer] / PLAYING_FIELD_X;
	
	uint8_t tailNextX = snakeBody[tailPointerNext] % PLAYING_FIELD_X;
	uint8_t tailNextY = snakeBody[tailPointerNext] / PLAYING_FIELD_X;
	
	uint8_t tailTurn = 0;
	if(tailNextY == tailY) {							// left, right
		if(tailNextX > tailX && (tailNextX - tailX <= 1))	// right
			tailTurn = 0;
		else if(tailX - tailNextX <= 1)						// left
			tailTurn = 2;
		else
			tailTurn = 0;									// right
	}
	else {												// up, down
		if(tailNextY > tailY && (tailNextY - tailY <= 1))	// down
			tailTurn = 1;
		else if(tailY - tailNextY <= 1)						// up
			tailTurn = 3;
		else
			tailTurn = 1;									// down
	}
	
	map[snakeBody[snakeData.tailPointer]].tileNumber = TILE_TAIL + tailTurn;
}

void Game_Draw_Head(uint8_t pointer) {
	uint8_t tileFood = 0;
	
	uint8_t collision = Game_Check_Collision(Game_Calc_Pointer_Next_Step(pointer));
	if(collision == COLLISION_FOOD || collision == COLLISION_BONUS)
		tileFood = 4;
	
	map[pointer].tileNumber = TILE_HEAD + snakeData.turn + tileFood;
	map[pointer].tileBody = 1;
	
	tileFood = 0;
	if(map[snakeBody[snakeData.headPointer]].tileFood)
		tileFood = 4;
		
	map[snakeBody[snakeData.headPointer]].tileFood = 0;
	
	// если угол поворота не изменился
	if(snakeData.turn == snakeData.turnOld)
		map[snakeBody[snakeData.headPointer]].tileNumber = TILE_BODY + snakeData.turn + tileFood;
	
	// если угол поворота изменился
	else {
		int8_t turnDecrement = snakeData.turn - 1;
		
		// (0 - 1 = 3)
		if(turnDecrement < 0)
			turnDecrement = 3;
		
		// отрисовка предыдущего за головой тайла
		if(snakeData.turnOld == turnDecrement)
			map[snakeBody[snakeData.headPointer]].tileNumber = TILE_TURN + snakeData.turn + tileFood;
		else
			map[snakeBody[snakeData.headPointer]].tileNumber = TILE_TURN + turnDecrement + tileFood;
		
		snakeData.turnOld = snakeData.turn;
	}
	
	// инкремент headPointer
	if(++snakeData.headPointer >= PLAYING_FIELD_X * PLAYING_FIELD_Y)
		snakeData.headPointer = 0;
	
	snakeBody[snakeData.headPointer] = pointer;
}

void Game_Draw_Frame() {
	LCD5110_Setpos(0, 0);
	for(uint8_t i = 0; i < LCD5510_X; i++)
		LCD5110_Send(0x40);
		
	LCD5110_Setpos(0, 1);
	LCD5110_Send(0xFF);
	LCD5110_Send(0x01);
	
	LCD5110_Setpos(LCD5510_X - 2, 1);
	LCD5110_Send(0x01);
	
	for(uint8_t i = 1; i < 5; i++) {
		LCD5110_Setpos(LCD5510_X - 1, i);
		LCD5110_Send(0xFF);
		LCD5110_Send(0xFF);
	}
	
	LCD5110_Send(0x80);
	
	LCD5110_Setpos(LCD5510_X - 2, 5);
	LCD5110_Send(0x80);
	LCD5110_Send(0xFF);
}

uint8_t Game_Check_Collision(uint8_t pointer) {
	// check collision with body
	if(map[pointer].tileBody && pointer != snakeBody[snakeData.tailPointer])
		return COLLISION_BODY;
			
	// check collision with food
	if(map[pointer].tileFood) {
		if(map[pointer].tileNumber == TILE_FOOD)
			return COLLISION_FOOD;
		else
			return COLLISION_BONUS;
	}

	return COLLISION_NULL;
}

uint8_t Game_Calc_Pointer_Next_Step(uint8_t pointer) {
	int8_t headX = pointer % PLAYING_FIELD_X;
	int8_t headY = pointer / PLAYING_FIELD_X;
	
	// вычисление новых координат головы змейки
	if(snakeData.turn == 0 && ++headX == PLAYING_FIELD_X)		// right
		headX = 0;
	else if(snakeData.turn == 1 && ++headY == PLAYING_FIELD_Y)	// down
		headY = 0;
	else if(snakeData.turn == 2 && --headX < 0)					// left
		headX = PLAYING_FIELD_X - 1;
	else if(snakeData.turn == 3 && --headY < 0)					// right
		headY = PLAYING_FIELD_Y - 1;
	
	return headX + headY * PLAYING_FIELD_X;
}
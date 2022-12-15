#include "game.h"

Game game;
Snake snake;
VideoBuffer videoBuffer;
Fruit fruit;

bool redrawMenu = true;

void moveSnake()
{
	if ((snake.coords[0].x + snake.headVelocity.x >= DISPLAY_WIDTH - SNAKE_SCALE) || ((uint8_t)(snake.coords[0].y + snake.headVelocity.y) >= DISPLAY_HEIGHT - SNAKE_SCALE))
	{
		//game.gameState = Lose;
		stopGame(MainMenu);
		redrawMenu = true;
	}

	for (uint8_t i = game.score + SNAKE_START_LENGTH - 1; i > 0; i--)
	{
		snake.coords[i].x = snake.coords[i-1].x;
		snake.coords[i].y = snake.coords[i-1].y;	
	}

	const uint8_t snakeTailIndex = game.score + SNAKE_START_LENGTH - 1;
	if (snake.coords[snakeTailIndex].y - snake.coords[snakeTailIndex-1].y > 0)
	{
		snake.tailVelocity.y = -1;
		snake.tailVelocity.x = 0;
	}
	else if (snake.coords[snakeTailIndex].y - snake.coords[snakeTailIndex-1].y < 0)
	{
		snake.tailVelocity.y = 1;
		snake.tailVelocity.x = 0;
	}
	else if (snake.coords[snakeTailIndex].x - snake.coords[snakeTailIndex-1].x < 0)
	{
		snake.tailVelocity.y = 0;
		snake.tailVelocity.x = 1;
	}
	else if (snake.coords[snakeTailIndex].x - snake.coords[snakeTailIndex-1].x > 0)
	{
		snake.tailVelocity.y = 0;
		snake.tailVelocity.x = -1;
	}

	snake.coords[0].x += snake.headVelocity.x;
	snake.coords[0].y += snake.headVelocity.y;
}

void initSnake()
{
	snake.coords[0].x = 30;
	snake.coords[0].y = 50;
	snake.headVelocity.x = 1;
	snake.headVelocity.y = 0;
	snake.tailVelocity.x = snake.headVelocity.x;
	snake.tailVelocity.y = snake.headVelocity.y;
	snake.prevDirection.x = -1 * snake.headVelocity.x;
	snake.prevDirection.y = -1 * snake.headVelocity.y;

	for (uint8_t i = 1; i < game.score + SNAKE_START_LENGTH; i++)
	{
		snake.coords[i].x = snake.coords[i-1].x + (-1 * snake.headVelocity.x); 
		snake.coords[i].y = snake.coords[i-1].y;
	}
}

void initGame()
{
	game.score = 0;
	game.gameState = Alive;
}

void stopGame(GameState gamestate)
{
	game.gameState = gamestate;
	game.score = 0;
}

void drawSnake()
{
	if ((snake.coords[0].x + snake.headVelocity.x >= DISPLAY_WIDTH - SNAKE_SCALE) || ((uint8_t)(snake.coords[0].y + snake.headVelocity.y) >= DISPLAY_HEIGHT - SNAKE_SCALE))
	{
		//game.gameState = Lose;
		stopGame(MainMenu);
		redrawMenu = true;
		return;
	}

	for (uint8_t i = 0; i < game.score + SNAKE_START_LENGTH; i++)
	{
		if (snake.headVelocity.y)
		{
			for (uint8_t x = 0; x < SNAKE_SCALE; x++)
				drawToBuffer(&videoBuffer, snake.coords[i].x + x, snake.coords[i].y / 8, 0x1F << (snake.coords[i].y % 8), Draw);
		}
		else
			drawToBuffer(&videoBuffer, snake.coords[i].x, snake.coords[i].y / 8, 0x1F << (snake.coords[i].y % 8), Draw);

		if ((snake.coords[i].y % 8) > 8 - SNAKE_SCALE)
		{
			if (snake.headVelocity.y)
			{
				for (uint8_t x = 0; x < SNAKE_SCALE; x++)
					drawToBuffer(&videoBuffer, snake.coords[i].x + x, snake.coords[i].y / 8 + 1, 0x1F >> (8 - snake.coords[i].y % 8), Draw);
			}
			else
				drawToBuffer(&videoBuffer, snake.coords[i].x, snake.coords[i].y / 8 + 1, 0x1F >> (8 - snake.coords[i].y % 8), Draw);
		}
	}

	const uint8_t snakeTailIndex = game.score + SNAKE_START_LENGTH - 1;

	if (snake.tailVelocity.x > 0)
	{
		drawToBuffer(&videoBuffer, snake.coords[snakeTailIndex].x, snake.coords[snakeTailIndex].y / 8, 0x00, ApplyMask);
		if ((snake.coords[snakeTailIndex].y % 8) > 8 - SNAKE_SCALE)
		{
			drawToBuffer(&videoBuffer, snake.coords[snakeTailIndex].x, snake.coords[snakeTailIndex].y / 8 + 1, 0xFF << (SNAKE_SCALE - (8 - snake.coords[snakeTailIndex].y % 8)), ApplyMask);
		}
	}

	else if (snake.tailVelocity.x < 0)
	{
		drawToBuffer(&videoBuffer, snake.coords[snakeTailIndex].x + SNAKE_SCALE - 1, snake.coords[snakeTailIndex].y / 8, 0x00, ApplyMask);
		if ((snake.coords[snakeTailIndex].y % 8) > 8 - SNAKE_SCALE)
		{
			drawToBuffer(&videoBuffer, snake.coords[snakeTailIndex].x + SNAKE_SCALE - 1, snake.coords[snakeTailIndex].y / 8 + 1, 0xFF << (SNAKE_SCALE - (8 - snake.coords[snakeTailIndex].y % 8)), ApplyMask);
		}
	}

	else if (snake.tailVelocity.y > 0)
	{
		for (uint8_t x = 0; x < SNAKE_SCALE; x++)
			drawToBuffer(&videoBuffer, snake.coords[snakeTailIndex].x + x, snake.coords[snakeTailIndex].y / 8, 0xFF << (snake.coords[snakeTailIndex].y % 8+1), ApplyMask);
	}

	else if (snake.tailVelocity.y < 0)
	{
		for (uint8_t x = 0; x < SNAKE_SCALE; x++)
		{
			if ((snake.coords[snakeTailIndex].y % 8) > 8 - SNAKE_SCALE)
				drawToBuffer(&videoBuffer, snake.coords[snakeTailIndex].x + x, snake.coords[snakeTailIndex].y / 8 + 1, 0x1F >> (8 - snake.coords[snakeTailIndex].y % 8 + 1), ApplyMask);
			else
				drawToBuffer(&videoBuffer, snake.coords[snakeTailIndex].x + x, snake.coords[snakeTailIndex].y / 8, 0xFF >> (8 - (snake.coords[snakeTailIndex].y % 8 + SNAKE_SCALE - 1)), ApplyMask);
		}
	}

	drawFromBufferToDisplay(&videoBuffer);
}

void placeFruit(uint8_t x, uint8_t y)
{
    fruit.coords.x = x;
    fruit.coords.y = y;
}

void drawFruit(DrawMethod draw)
{
	uint8_t mask = 0x1F;
	if (draw == ApplyMask)
		mask = 0x00;

    for (uint8_t i = 0; i < FRUIT_SIZE; i++)
    {
        drawToBuffer(&videoBuffer, fruit.coords.x + i, fruit.coords.y / 8, mask << (fruit.coords.y % 8), draw);

        if ((fruit.coords.y % 8) > 8 - SNAKE_SCALE)
		{
			drawToBuffer(&videoBuffer, fruit.coords.x + i, fruit.coords.y / 8 + 1, mask >> (8 - fruit.coords.y % 8), draw);
		}
    }
}

void eatFruit()
{
	int16_t vX = snake.tailVelocity.x;
	int16_t vY = snake.tailVelocity.y;

	game.score += SNAKE_SCALE;
	
	for (int8_t i = game.score + SNAKE_START_LENGTH - 1; i > (int)(game.score + SNAKE_START_LENGTH - SNAKE_SCALE - 1); i--)
	{
		snake.coords[i].x = snake.coords[i-SNAKE_SCALE].x + ( -1 * vX * SNAKE_SCALE );
		snake.coords[i].y = snake.coords[i-SNAKE_SCALE].y + ( -1 * vY * SNAKE_SCALE );
	}

	snake.tailVelocity.x = vX;
	snake.tailVelocity.y = vY;

	drawFruit(ApplyMask);

	Coords newFruitCoords;
	newFruitCoords.x = TIM2->CNT % (DISPLAY_WIDTH - SNAKE_SCALE + 1);
	newFruitCoords.y = TIM2->CNT % (DISPLAY_HEIGHT - SNAKE_SCALE + 1);

	placeFruit(newFruitCoords.x, newFruitCoords.y);
}

void gameUpdate()
{
	for (uint8_t i = 1; i < game.score + SNAKE_START_LENGTH; i++)
	{
		if (snake.coords[0].x == snake.coords[i].x && snake.coords[0].y == snake.coords[i].y)
		{
			//stopGame(Lose);
			stopGame(MainMenu);
			redrawMenu = true;
		}
	}

	Coords offset;
	offset.x = snake.coords[0].x + SNAKE_SCALE;
	offset.y = snake.coords[0].y + SNAKE_SCALE;

	for (uint8_t i = snake.coords[0].y; i <= offset.y; i += SNAKE_SCALE)
	{
		for (uint8_t j = snake.coords[0].x; j <= offset.x; j += SNAKE_SCALE)
		{
			if ( 
					fruit.coords.x <= j && fruit.coords.x + FRUIT_SIZE >= j &&
					fruit.coords.y <= i && fruit.coords.y + FRUIT_SIZE >= i
			)
			{
				eatFruit();

				if (game.score >= 75)
				{
					//stopGame(Win);

					stopGame(MainMenu);
					redrawMenu = true;
					drawMenu(FunFace);
					delay_us(1000000);
				}
			}
		}
	}    
}

void drawMenu(uint8_t menu[64][128])
{
	clearScreen(0x00);

	for (uint8_t j = 0; j < DISPLAY_HEIGHT / 8; j++)
	{
		for (uint8_t i = 0; i < DISPLAY_WIDTH; i++)
		{
			paintRegion(i, j, menu[7 - j][127-i]);
		}
	}
}

void drawDigit(uint8_t digit, uint8_t x, uint8_t y)
{
	uint8_t page = y >> 3;
	uint8_t offset = y << 5;
	offset = offset >> 5;

	if (offset > 1)
		for (uint8_t i = 0; i < 5; i++)
		{
			paintRegion(x + i, page, digits[digit][i] << offset);
			paintRegion(x + i, page + 1, digits[digit][i] >> (8 - offset));
		}
	else 
		for (uint8_t i = 0; i < 5; i++)
			paintRegion(x + i, page, digits[digit][i]);
	
}

void drawScore()
{
	uint8_t offset = 62;
	uint8_t score = game.score / 5;

	if (!score) drawDigit(score, offset, 0);

	while (score > 0)
	{
		drawDigit(score % 10, offset, 0);
		score /= 10;
		offset -= 6;
	}
}

void drawBorder()
{
	for (uint8_t i = 0; i < DISPLAY_WIDTH; i++)
			paintRegion(i, 0, 0x80);
}
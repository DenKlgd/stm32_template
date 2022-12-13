#include "game.h"

Game game;
Snake snake;
VideoBuffer videoBuffer;
Fruit fruit;

void moveSnake()
{
	if (snake.coords[0].x >= DISPLAY_WIDTH - SNAKE_SCALE || snake.coords[0].y >= DISPLAY_HEIGHT - SNAKE_SCALE)
		game.isAlive = false;

	for (uint8_t i = game.score + SNAKE_START_LENGTH - 1; i > 0; i--)
	{
		snake.coords[i].x = snake.coords[i-1].x;
		snake.coords[i].y = snake.coords[i-1].y;	
	}

	const uint8_t snakeTailIndex = game.score + SNAKE_START_LENGTH - 1;
	if (snake.coords[snakeTailIndex].y - snake.coords[snakeTailIndex-1].y > 0)
	{
		snake.velocity[snakeTailIndex].y = -1;
		snake.velocity[snakeTailIndex].x = 0;
	}
	else if (snake.coords[snakeTailIndex].y - snake.coords[snakeTailIndex-1].y < 0)
	{
		snake.velocity[snakeTailIndex].y = 1;
		snake.velocity[snakeTailIndex].x = 0;
	}
	else if (snake.coords[snakeTailIndex].x - snake.coords[snakeTailIndex-1].x < 0)
	{
		snake.velocity[snakeTailIndex].y = 0;
		snake.velocity[snakeTailIndex].x = 1;
	}
	else if (snake.coords[snakeTailIndex].x - snake.coords[snakeTailIndex-1].x > 0)
	{
		snake.velocity[snakeTailIndex].y = 0;
		snake.velocity[snakeTailIndex].x = -1;
	}

	snake.coords[0].x += snake.velocity[0].x;
	snake.coords[0].y += snake.velocity[0].y;
}

void initSnake()
{
	snake.coords[0].x = 30;
	snake.coords[0].y = 50;
	snake.velocity[0].x = 1;
	snake.velocity[0].y = 0;
	snake.prevDirection.x = -1 * snake.velocity[0].x;
	snake.prevDirection.y = -1 * snake.velocity[0].y;

	for (uint8_t i = 1; i < game.score + SNAKE_START_LENGTH; i++)
	{
		snake.coords[i].x = snake.coords[i-1].x + (-1 * snake.velocity[0].x); 
		snake.coords[i].y = snake.coords[i-1].y;
		snake.velocity[i].x = snake.velocity[i-1].x;
		snake.velocity[i].y = snake.velocity[i-1].y;
	}
}

void initGame()
{
	game.isAlive = true;
	game.score = 0;
}

void stopGame()
{
	game.isAlive = false;
	game.score = 0;
}

void drawSnake()
{
	for (uint8_t i = 0; i < game.score + SNAKE_START_LENGTH; i++)
	{
		drawToBuffer(&videoBuffer, snake.coords[i].x, snake.coords[i].y / 8, 0x1F << (snake.coords[i].y % 8), Draw);
		if (snake.velocity[0].y < 0)
		{
			for (uint8_t x = 0; x < SNAKE_SCALE; x++)
				drawToBuffer(&videoBuffer, snake.coords[i].x + x, snake.coords[i].y / 8, 0x1F << (snake.coords[i].y % 8), Draw);
		}
		else if (snake.velocity[0].y > 0)
		{
			for (uint8_t x = 0; x < SNAKE_SCALE; x++)
				drawToBuffer(&videoBuffer, snake.coords[i].x + x, snake.coords[i].y / 8, 0x1F << (snake.coords[i].y % 8), Draw);
		}

		if ((snake.coords[i].y % 8) > 8 - SNAKE_SCALE)
		{
			drawToBuffer(&videoBuffer, snake.coords[i].x, snake.coords[i].y / 8 + 1, 0x1F >> (8 - snake.coords[i].y % 8), Draw);

			if (snake.velocity[0].y < 0)
			{
				for (uint8_t x = 0; x < SNAKE_SCALE; x++)
					drawToBuffer(&videoBuffer, snake.coords[i].x + x, snake.coords[i].y / 8 + 1, 0x1F >> (8 - snake.coords[i].y % 8), Draw);
			}
			else if (snake.velocity[0].y > 0)
			{
				for (uint8_t x = 0; x < SNAKE_SCALE; x++)
					drawToBuffer(&videoBuffer, snake.coords[i].x + x, snake.coords[i].y / 8 + 1, 0x1F >> (8 - snake.coords[i].y % 8), Draw);
			}
		}
	}

	const uint8_t snakeTailIndex = game.score + SNAKE_START_LENGTH - 1;

	if (snake.velocity[snakeTailIndex].x > 0)
	{
		drawToBuffer(&videoBuffer, snake.coords[snakeTailIndex].x, snake.coords[snakeTailIndex].y / 8, 0x00, ApplyMask);
		if ((snake.coords[snakeTailIndex].y % 8) > 8 - SNAKE_SCALE)
		{
			drawToBuffer(&videoBuffer, snake.coords[snakeTailIndex].x, snake.coords[snakeTailIndex].y / 8 + 1, 0xFF << (SNAKE_SCALE - (8 - snake.coords[snakeTailIndex].y % 8)), ApplyMask);
		}
	}

	else if (snake.velocity[snakeTailIndex].x < 0)
	{
		drawToBuffer(&videoBuffer, snake.coords[snakeTailIndex].x + SNAKE_SCALE - 1, snake.coords[snakeTailIndex].y / 8, 0x00, ApplyMask);
		if ((snake.coords[snakeTailIndex].y % 8) > 8 - SNAKE_SCALE)
		{
			drawToBuffer(&videoBuffer, snake.coords[snakeTailIndex].x + SNAKE_SCALE - 1, snake.coords[snakeTailIndex].y / 8 + 1, 0xFF << (SNAKE_SCALE - (8 - snake.coords[snakeTailIndex].y % 8)), ApplyMask);
		}
	}

	else if (snake.velocity[snakeTailIndex].y > 0)
	{
		for (uint8_t x = 0; x < SNAKE_SCALE; x++)
			drawToBuffer(&videoBuffer, snake.coords[snakeTailIndex].x + x, snake.coords[snakeTailIndex].y / 8, 0xFF << (snake.coords[snakeTailIndex].y % 8+1), ApplyMask);
	}

	else if (snake.velocity[snakeTailIndex].y < 0)
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
	int16_t vX = snake.velocity[game.score + SNAKE_START_LENGTH - 1].x;
	int16_t vY = snake.velocity[game.score + SNAKE_START_LENGTH - 1].y;

	game.score += SNAKE_SCALE;
	
	for (int8_t i = game.score + SNAKE_START_LENGTH - 1; i > (int)(game.score + SNAKE_START_LENGTH - SNAKE_SCALE - 1); i--)
	{
		snake.coords[i].x = snake.coords[i-SNAKE_SCALE].x + ( -1 * vX * SNAKE_SCALE );
		snake.coords[i].y = snake.coords[i-SNAKE_SCALE].y + ( -1 * vY * SNAKE_SCALE );
	}

	snake.velocity[game.score + SNAKE_START_LENGTH - 1].x = vX;
	snake.velocity[game.score + SNAKE_START_LENGTH - 1].y = vY;

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
			stopGame();
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

				if (game.score >= 50)
				{
					stopGame();
				}
			}
		}
	}    
}

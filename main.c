#include <stm32f10x.h>
#include "StaticLib/delay.h"
#include "StaticLib/spi.h"
#include "StaticLib/configuration.h"
#include "StaticLib/display.h"
#include "StaticLib/DoubleBuffering.h"
#include "StaticLib/game.h"
#include "StaticLib/Menu.h"

#define PLAY_BUTTON 0
#define DIFFICULTY_BUTTON 1

#define DIFFICULTY_SELECTOR 0
#define GO_BACK_BUTTON 1

void initButtons();
void userInputGame();
void userInputMenu();
void userInputDifficultyMenu();

Menu mainMenu;
Menu difficultyMenu;

uint8_t mainMenuButtonsFrameCoords[2][4] = { {47, 36, 79, 50}, {32, 51, 97, 63} };
uint8_t difficultyMenuButtonsFrameCoords[2][4] = { {34, 23, 92, 40}, {42, 48, 83, 59} };
MenuElementID button = 0;

int main(void)
{
	enabling_clocks();
	configuring_ports();
	configuring_timers();
	SPI1_Init();
	SPI1_Preset();	

	game.gameState = MainMenu;
	game.scoreToBeat = 15;

	initMenu(&mainMenu, 2);
	initMenu(&difficultyMenu, 2);
	initButtons();
	placeFruit(TIM2->CNT % (DISPLAY_WIDTH - SNAKE_SCALE + 1), TIM2->CNT % (DISPLAY_HEIGHT - SNAKE_SCALE + 1));

	clearScreen(0x00);
	initVideoBuffer(&videoBuffer);

	while (true)
	{
		switch (game.gameState)
		{
		case MainMenu:	
			if (redrawMenu)
			{
				redrawMenu = false;
				drawMenuToBuffer(SnakeStartMenu);
				drawButtonFrameToBuffer(mainMenuButtonsFrameCoords[button][0], mainMenuButtonsFrameCoords[button][1], mainMenuButtonsFrameCoords[button][2], mainMenuButtonsFrameCoords[button][3]);
				drawFromBufferToDisplay(&videoBuffer);
				delay_us(300000);
			}
			userInputMenu();		
			break;

		case Alive:
			userInputGame();
			moveSnake();
			gameUpdate();
			drawSnake();
			drawFruit();
			drawNumber(game.score / 5, (Coords){62, 0});
			drawFromBufferToDisplay(&videoBuffer);	
			delay_us(20000);
			break;

		case Lose:
			drawMenuToBuffer(DeathScreen);
			drawFromBufferToDisplay(&videoBuffer);
			delay_us(1000000);
			stopGame(MainMenu);
			//userInputMenu();
			break;

		case Win:
			userInputMenu();
			break;
		
		case DifficultyMenu:
			if (redrawMenu)
			{
				redrawMenu = false;
				drawMenuToBuffer(SnakeDifficultyMenu);
				drawButtonFrameToBuffer(difficultyMenuButtonsFrameCoords[button][0], difficultyMenuButtonsFrameCoords[button][1], difficultyMenuButtonsFrameCoords[button][2], difficultyMenuButtonsFrameCoords[button][3]);
				drawNumber(game.scoreToBeat, (Coords){64, 29});
				drawFromBufferToDisplay(&videoBuffer);
				delay_us(300000);
			}
			userInputDifficultyMenu();
			break;

		default:
			break;
		}
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM2->SR & TIM_SR_UIF) // True if Update Interrupt Flag is set
	{
		TIM2->SR &= ~TIM_SR_UIF; // Clearing UIF flag
	}
}

void userInputGame()
{
	if ( !(GPIOA->IDR & GPIO_IDR_IDR8) )
	{
		snake.prevDirection.x = snake.headVelocity.x;
		snake.prevDirection.y = snake.headVelocity.y;

		snake.headVelocity.x = 0;
		snake.headVelocity.y = -1;  //up

		if (snake.headVelocity.x == snake.prevDirection.x * -1 && snake.headVelocity.y == snake.prevDirection.y * -1)
		{
			snake.headVelocity.x *= -1;
			snake.headVelocity.y *= -1;
		}
	}

	else if ( !(GPIOB->IDR & GPIO_IDR_IDR15) )
	{
		snake.prevDirection.x = snake.headVelocity.x;
		snake.prevDirection.y = snake.headVelocity.y;
		
		snake.headVelocity.x = 0;
		snake.headVelocity.y = 1;  //down

		if (snake.headVelocity.x == snake.prevDirection.x * -1 && snake.headVelocity.y == snake.prevDirection.y * -1)
		{
			snake.headVelocity.x *= -1;
			snake.headVelocity.y *= -1;
		}
	}

	else if ( !(GPIOB->IDR & GPIO_IDR_IDR14) )
	{
		snake.prevDirection.x = snake.headVelocity.x;
		snake.prevDirection.y = snake.headVelocity.y;

		snake.headVelocity.x = -1;
		snake.headVelocity.y = 0;  //left

		if (snake.headVelocity.x == snake.prevDirection.x * -1 && snake.headVelocity.y == snake.prevDirection.y * -1)
		{
			snake.headVelocity.x *= -1;
			snake.headVelocity.y *= -1;
		}
	}

	else if ( !(GPIOB->IDR & GPIO_IDR_IDR13) )
	{
		snake.prevDirection.x = snake.headVelocity.x;
		snake.prevDirection.y = snake.headVelocity.y;

		snake.headVelocity.x = 1;
		snake.headVelocity.y = 0;  //right

		if (snake.headVelocity.x == snake.prevDirection.x * -1 && snake.headVelocity.y == snake.prevDirection.y * -1)
		{
			snake.headVelocity.x *= -1;
			snake.headVelocity.y *= -1;
		}
	}
}

void userInputMenu()
{
	if ( !(GPIOA->IDR & GPIO_IDR_IDR8) )
	{
		button = move(&mainMenu, Up);	//up
		redrawMenu = true;
	}

	else if ( !(GPIOB->IDR & GPIO_IDR_IDR15) )
	{
		button = move(&mainMenu, Down);	//down
		redrawMenu = true;
	}

	else if ( !(GPIOB->IDR & GPIO_IDR_IDR12) )
	{
		MenuElementID menuElemID = getPressedMenuElement(&mainMenu);	//mid
		
		switch (menuElemID)
		{
		case PLAY_BUTTON:
			initVideoBuffer(&videoBuffer);
			clearScreen(0x00);
			initGame();
			initSnake();
			break;
		
		case DIFFICULTY_BUTTON:
			game.gameState = DifficultyMenu;
			redrawMenu = true;
			break;

		default:
			break;
		}
	}

	// if ( !(GPIOB->IDR & GPIO_IDR_IDR12) )
	// {
	// 	initVideoBuffer(&videoBuffer);
	// 	clearScreen(0x00);
	// 	initGame();
	// 	initSnake();
	// }
}

void userInputDifficultyMenu()
{
	if ( !(GPIOA->IDR & GPIO_IDR_IDR8) )
	{
		button = move(&difficultyMenu, Up);	//up
		redrawMenu = true;
	}

	else if ( !(GPIOB->IDR & GPIO_IDR_IDR15) )
	{
		button = move(&difficultyMenu, Down);	//down
		redrawMenu = true;
	}

	else if (button == DIFFICULTY_SELECTOR)
	{
		if ( !(GPIOB->IDR & GPIO_IDR_IDR14) )
		{
			if (game.scoreToBeat * 5 > SNAKE_SCALE)
			{
				game.scoreToBeat--;					//left
				redrawMenu = true;
			}
		}

		else if ( !(GPIOB->IDR & GPIO_IDR_IDR13) )
		{
			if (game.scoreToBeat * 5 < SNAKE_LENGTH - SNAKE_START_LENGTH)
			{
				game.scoreToBeat++;					//right
				redrawMenu = true;
			}
		}
	}
	
	else if ( !(GPIOB->IDR & GPIO_IDR_IDR12) )
	{
		MenuElementID menuElemID = getPressedMenuElement(&difficultyMenu);	//mid
		
		switch (menuElemID)
		{
		case GO_BACK_BUTTON:
			game.gameState = MainMenu;
			redrawMenu = true;
			break;

		default:
			break;
		}
	}
}

void initButtons()
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

	GPIOA->CRH &= ~(GPIO_CRH_CNF8 | GPIO_CRH_MODE8) | GPIO_CRH_CNF8_1;
	GPIOA->ODR |= GPIO_ODR_ODR8;

	GPIOB->CRH &= ~GPIO_CRH_CNF12;
	GPIOB->CRH |= GPIO_CRH_CNF12_1;
	GPIOB->CRH &= ~GPIO_CRH_MODE12;
	GPIOB->ODR |= GPIO_ODR_ODR12;

	GPIOB->CRH &= ~GPIO_CRH_CNF13;
	GPIOB->CRH |= GPIO_CRH_CNF13_1;
	GPIOB->CRH &= ~GPIO_CRH_MODE13;
	GPIOB->ODR |= GPIO_ODR_ODR13;

	GPIOB->CRH &= ~GPIO_CRH_CNF14;
	GPIOB->CRH |= GPIO_CRH_CNF14_1;
	GPIOB->CRH &= ~GPIO_CRH_MODE14;
	GPIOB->ODR |= GPIO_ODR_ODR14;

	GPIOB->CRH &= ~GPIO_CRH_CNF15;
	GPIOB->CRH |= GPIO_CRH_CNF15_1;
	GPIOB->CRH &= ~GPIO_CRH_MODE15;
	GPIOB->ODR |= GPIO_ODR_ODR15;
}

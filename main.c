#include <stm32f10x.h>
#include "StaticLib/delay.h"
#include "StaticLib/spi.h"
#include "StaticLib/configuration.h"
#include "StaticLib/display.h"
#include "StaticLib/DoubleBuffering.h"
#include "StaticLib/game.h"

void initButtons();
void userInputGame();
void userInputMenu();

int main(void)
{
	enabling_clocks();
	configuring_ports();
	configuring_timers();
	SPI1_Init();
	SPI1_Preset();	

	game.gameState = MainMenu;

	initButtons();
	placeFruit(TIM2->CNT % (DISPLAY_WIDTH - SNAKE_SCALE + 1), TIM2->CNT % (DISPLAY_HEIGHT - SNAKE_SCALE + 1));

	while (true)
	{
		switch (game.gameState)
		{
		case MainMenu:	
			if (redrawMenu)
			{
				redrawMenu = false;
				drawMenu(SnakeStartMenu);
			}
			userInputMenu();		
			break;

		case Alive:
			userInputGame();
			moveSnake();
			gameUpdate();
			drawSnake();
			drawFruit();
			drawScore();
			drawFromBufferToDisplay(&videoBuffer);	
			delay_us(20000);
			break;

		case Lose:
			userInputMenu();
			break;

		case Win:
			userInputMenu();
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
	if ( !(GPIOB->IDR & GPIO_IDR_IDR12) )
	{
		initVideoBuffer(&videoBuffer);
		clearScreen(0x00);
		initGame();
		initSnake();
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

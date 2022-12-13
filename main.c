#include <stm32f10x.h>
#include "StaticLib/delay.h"
#include "StaticLib/spi.h"
#include "StaticLib/configuration.h"
#include "StaticLib/display.h"
#include "StaticLib/videobuffer.h"
#include "StaticLib/game.h"

void initButtons();
void userInput();

int main(void)
{
	enabling_clocks();
	configuring_ports();
	configuring_timers();
	SPI1_Init();
	SPI1_Preset();	

	clearScreen(0x00);

	initVideoBuffer(&videoBuffer);

	initGame();
	initSnake();
	initButtons();
	placeFruit(10, 10);

	while (true)
	{
		if (game.isAlive)
		{
			userInput();
			moveSnake();
			drawFruit(Draw);
			gameUpdate();
			drawSnake();
			delay_us(20000);
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

void userInput()
{
	if ( !(GPIOB->IDR & GPIO_IDR_IDR15) )
	{
		snake.prevDirection.x = snake.velocity[0].x;
		snake.prevDirection.y = snake.velocity[0].y;

		snake.velocity[0].x = 0;
		snake.velocity[0].y = -1;  //up

		if (snake.velocity[0].x == snake.prevDirection.x * -1 && snake.velocity[0].y == snake.prevDirection.y * -1)
		{
			snake.velocity[0].x *= -1;
			snake.velocity[0].y *= -1;
		}
	}

	else if ( !(GPIOB->IDR & GPIO_IDR_IDR14) )
	{
		snake.prevDirection.x = snake.velocity[0].x;
		snake.prevDirection.y = snake.velocity[0].y;
		
		snake.velocity[0].x = 0;
		snake.velocity[0].y = 1;  //down

		if (snake.velocity[0].x == snake.prevDirection.x * -1 && snake.velocity[0].y == snake.prevDirection.y * -1)
		{
			snake.velocity[0].x *= -1;
			snake.velocity[0].y *= -1;
		}
	}

	else if ( !(GPIOB->IDR & GPIO_IDR_IDR13) )
	{
		snake.prevDirection.x = snake.velocity[0].x;
		snake.prevDirection.y = snake.velocity[0].y;

		snake.velocity[0].x = -1;
		snake.velocity[0].y = 0;  //left

		if (snake.velocity[0].x == snake.prevDirection.x * -1 && snake.velocity[0].y == snake.prevDirection.y * -1)
		{
			snake.velocity[0].x *= -1;
			snake.velocity[0].y *= -1;
		}
	}

	else if ( !(GPIOB->IDR & GPIO_IDR_IDR12) )
	{
		snake.prevDirection.x = snake.velocity[0].x;
		snake.prevDirection.y = snake.velocity[0].y;

		snake.velocity[0].x = 1;
		snake.velocity[0].y = 0;  //right

		if (snake.velocity[0].x == snake.prevDirection.x * -1 && snake.velocity[0].y == snake.prevDirection.y * -1)
		{
			snake.velocity[0].x *= -1;
			snake.velocity[0].y *= -1;
		}
	}
}

void initButtons()
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

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

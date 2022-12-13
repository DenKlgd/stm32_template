#include "configuration.h"


void enabling_clocks(void) 
{
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // Enabling clock for AFIO
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // Enabling clock for GPIOA
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; // Enabling clock for GPIOB
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; // Enabling clock for GPIOC
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Enabling clock for TIM2
}

void configuring_ports (void)
{
	GPIOC->CRH &= ~GPIO_CRH_CNF13;	 // Setting up port C13 in push-pull output mode
	GPIOC->CRH |= GPIO_CRH_MODE13_0; //

	GPIOB->CRH &= ~GPIO_CRH_CNF12;	// Setting up port B12 in input pull-up mode
	GPIOB->CRH |= GPIO_CRH_CNF12_1; //
	GPIOB->CRH &= ~GPIO_CRH_MODE12; //
	GPIOB->ODR |= GPIO_ODR_ODR12;	//

	GPIOB->CRH &= ~GPIO_CRH_CNF13;  // Setting up port B13 in input pull-up mode
	GPIOB->CRH |= GPIO_CRH_CNF13_1; //	
	GPIOB->CRH &= ~GPIO_CRH_MODE13; //
	GPIOB->ODR |= GPIO_ODR_ODR13;   //

	GPIOB->CRH &= ~GPIO_CRH_CNF14;  // Setting up port B14 in input pull-up mode
	GPIOB->CRH |= GPIO_CRH_CNF14_1; //	
	GPIOB->CRH &= ~GPIO_CRH_MODE14; //
	GPIOB->ODR |= GPIO_ODR_ODR14;   //

	GPIOB->CRH &= ~GPIO_CRH_CNF15;  // Setting up port B15 in input pull-up mode
	GPIOB->CRH |= GPIO_CRH_CNF15_1; //	
	GPIOB->CRH &= ~GPIO_CRH_MODE15; //
	GPIOB->ODR |= GPIO_ODR_ODR15;   //

	GPIOA->CRH &= ~GPIO_CRH_CNF8;  // Setting up port A8 in input pull-up mode
	GPIOA->CRH |= GPIO_CRH_CNF8_1; //	
	GPIOA->CRH &= ~GPIO_CRH_MODE8; //
	GPIOA->ODR |= GPIO_ODR_ODR8;   //
}

void configuring_timers (void)
{
	RCC->APB1RSTR |= RCC_APB1RSTR_TIM2RST;	// Reseting TIM2
	RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM2RST; //						
	TIM2->PSC = 65534U;					// Editing prescaler value (1-65536) to configure timer frequency
	TIM2->ARR = 0b1001111111;			// Editing TIM2 reload value (count to X number)
	TIM2->DIER |= TIM_DIER_UIE ; 			// Enabling "Update" interrupt
	NVIC_ClearPendingIRQ(TIM2_IRQn);			 // Clearing pending interuptions for TIM2 ???
	NVIC_EnableIRQ(TIM2_IRQn);					 // Enabling interuptions from TIM2
	TIM2->CR1 |= TIM_CR1_ARPE;
	TIM2->CR1 |= TIM_CR1_CEN;					 // Enabling counter
}
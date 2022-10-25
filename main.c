#include <stdint.h>
#include <stm32f10x.h>

void delay(uint32_t ticks) {
	for (int i=0; i<ticks; i++) {
		__NOP();
	}
}

//Den4ik

int __attribute((noreturn)) main(void) {
	/*
	// Enable clock for AFIO
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	// Enable clock for GPIOC
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	// Enable PC13 push-pull mode
	GPIOC->CRH &= ~GPIO_CRH_CNF13; //clear cnf bits
	GPIOC->CRH |= GPIO_CRH_MODE13_0; //Max speed = 10Mhz

    while (1) {
	    GPIOC->ODR |= (1U<<13U); //U -- unsigned suffix (to avoid syntax warnings in IDE)
		//delay(1000000);
		delay(100000);
	    GPIOC->ODR &= ~(1U<<13U);
	    delay(10000);
    }
	*/


	

	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	GPIOC->CRH = GPIOC->CRH & ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13) | GPIO_CRH_MODE13_0;
	GPIOC->CRH = GPIOC->CRH & ~(GPIO_CRH_CNF14 | GPIO_CRH_MODE14) | GPIO_CRH_MODE14_1;
	GPIOC->ODR |= GPIO_ODR_ODR14;
	uint32_t i;
	uint8_t isPressed = 0xFF;

	while (1)
	{
		//while (!(GPIOC->IDR & GPIO_IDR_IDR14));
		delay(100);
		if (!(GPIOC->IDR & GPIO_IDR_IDR14))
		{
			isPressed = ~isPressed;
			if (isPressed)
				GPIOC->ODR &= ~GPIO_ODR_ODR13;
			else
				GPIOC->ODR |= GPIO_ODR_ODR13;
		}
		

		if (isPressed)
			GPIOC->ODR &= ~GPIO_ODR_ODR13;
		else
			GPIOC->ODR |= GPIO_ODR_ODR13;

		//delay(300000);

		//delay(300000);
		
	}

}


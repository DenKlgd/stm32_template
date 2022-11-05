#include <stdint.h>
#include <stm32f10x.h>
#include "amogus.h"

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

	toggleLED();
	//blinkLEDxODR('C', 13);

}


#pragma once
#include <stdint.h>
#include <stm32f10x.h>

void delay(uint32_t ticks) {
	for (int i=0; i<ticks; i++) {
		__NOP();
	}
}

void delay_us(uint32_t us)
{
	__asm volatile
	(
		"push {r0}\r\n"
		"mov R0, %0\r\n"
		"_loop:\r\n"
			"cmp R0, #0\r\n"
			"beq _exit\r\n"
			"sub R0, R0, #1\r\n"
			"nop\r\n"
			"b _loop\r\n"
		"_exit:\r\n"
		"pop {r0}\r\n"
		:: "r"(9 * us)
	);
}

void toggleLED()
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	GPIOC->CRH = GPIOC->CRH & ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13) | GPIO_CRH_MODE13_0;
	GPIOC->CRH = GPIOC->CRH & ~(GPIO_CRH_CNF14 | GPIO_CRH_MODE14) | GPIO_CRH_MODE14_1;
	GPIOC->ODR |= GPIO_ODR_ODR14;
	uint32_t i;
	uint8_t isPressed = 0xFF;

	while (1)
	{
        if (!(GPIOC->IDR & GPIO_IDR_IDR14))
            isPressed = ~isPressed;

		while (!(GPIOC->IDR & GPIO_IDR_IDR14))
        {

        }

        if (isPressed)
			GPIOC->ODR &= ~GPIO_ODR_ODR13;
		else
			GPIOC->ODR |= GPIO_ODR_ODR13;

        /*
		if (!(GPIOC->IDR & GPIO_IDR_IDR14))
		{
			isPressed = ~isPressed;
			delay_us(100);

            if (isPressed)
			    GPIOC->ODR &= ~GPIO_ODR_ODR13;
		    else
			    GPIOC->ODR |= GPIO_ODR_ODR13;
		}
        */
	}
}

void blinkLEDxBSRR()
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    GPIOC->CRH |= GPIO_CRH_MODE13_0;

    while (1)
    {
        GPIOC->BSRR |= GPIO_BSRR_BS13;
        delay(1000000);

        GPIOC->BSRR |= GPIO_BSRR_BR13;
        delay(1000000);
    }
}

void blinkLEDxODR(char gpioPort, unsigned char pin)
{
    if (pin > 15)
        return;

    GPIO_TypeDef* gpio = 0;

    switch (gpioPort)
    {
    case 'A':
        gpio = GPIOA;
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
        break;
    
    case 'B':
        gpio = GPIOB;
        RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
        break;

    case 'C':
        gpio = GPIOC;
        RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
        break;
    
    case 'D':
        gpio = GPIOD;
        RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
        break;

    default:
        return;
    }

    unsigned char offset = 0;

    if (pin > 7)
    {
        offset = 4 * (pin - 8);
        gpio->CRH &= ~(0x0000000F << offset);
        gpio->CRH |= (0x00000001 << offset);
    }
    else
    {
        offset = 4 * pin;
        gpio->CRL &= ~(0x0000000F << offset);
        gpio->CRL |= (0x00000001 << offset);
    }

    while (1)
    {
        gpio->ODR |= (0x0001 << pin);
        delay(1000000);

        gpio->ODR &= ~(0x0001 << pin);
        delay(1000000);
    }

    /*
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    GPIOC->CRH &= ~(GPIO_CRH_CNF13_0 | GPIO_CRH_CNF13_1);

    GPIOC->CRH &= ~GPIO_CRH_MODE13_1;

    GPIOC->CRH |= GPIO_CRH_MODE13_0;
    

    while (1)
    {
        GPIOC->ODR |= GPIO_ODR_ODR13;
        delay(1000000);

        GPIOC->ODR &= ~GPIO_ODR_ODR13;
        delay(1000000);
    }
    */
    
}

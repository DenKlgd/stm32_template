#ifndef delay_H
    #define delay_H

    #include <stdint.h>
    #include <stm32f10x.h>

    void delay(uint32_t ticks);
    void delay_us(uint32_t us);

#endif
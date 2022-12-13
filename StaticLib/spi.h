#ifndef spi_H
    #define spi_H
    #include <stm32f10x.h>

    void SPI1_Init(void);
    void SPI1_Preset(void);
    void SPI1_Write(uint8_t data);
    uint8_t SPI1_Read(void);
    void cmd(uint8_t data);
    void dat(uint8_t data);

#endif
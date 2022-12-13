#include "display.h"

void drawPixel(uint8_t x, uint8_t y)
{
    if (x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT)
        return;

    uint8_t x_high, x_low, page_number, fillMask, bitmaskOffset;

    page_number = y >> 3;    //y / 8;
    
    fillMask = 0x01;
    bitmaskOffset = y << 5;
    bitmaskOffset = bitmaskOffset >> 5;
    fillMask = fillMask << bitmaskOffset;

    x_high = x >> 4;          // Splitting column address between two address registers
    x_low = x & 0x0F; 		  // 

    GPIOA->BSRR = GPIO_ODR_ODR4 << 16U; // Selecting display (CS=0) 
    GPIOA->BSRR = GPIO_ODR_ODR3 << 16U; //Selecting sending of command (A0=0)

    SPI1_Write(0xB0|page_number);		// Seting page address ((0xb0) command code + (0b0000<=i<=0b0111) page adress)

    SPI1_Write(0x00|x_low);
    SPI1_Write(0x10|x_high);

    while (SPI1->SR & SPI_SR_BSY);

    GPIOA->BSRR = GPIO_ODR_ODR3; // Selecting sending of data
    SPI1_Write(fillMask);   //SPI1_Write(Bit_map[x][page_number]);	// Drawing page on display
    while (SPI1->SR & SPI_SR_BSY);

    GPIOA->BSRR = GPIO_ODR_ODR4; // Ending display selection
}

void paintRegion(uint8_t x, uint8_t page, uint8_t fillMask)
{
	if (x >= DISPLAY_WIDTH || page >= DISPLAY_HEIGHT / 8)
		return;

	uint8_t x_high, x_low;
	//X_UNION x_union;

	// x_union.high = x >> 4;
	// x_union.low = x & 0x0F;
	x_high = x >> 4;          // Splitting column address between two address registers
	x_low = x & 0x0F; 		  // 

	GPIOA->BSRR = GPIO_ODR_ODR4 << 16U; // Selecting display (CS=0) 
	GPIOA->BSRR = GPIO_ODR_ODR3 << 16U; //Selecting sending of command (A0=0)

	SPI1_Write(0xB0|page);		// Seting page address ((0xb0) command code + (0b0000<=i<=0b0111) page adress)

	SPI1_Write(0x00|x_low);
	SPI1_Write(0x10|x_high);
	// SPI1_Write(0x00|x_union.low );
	// SPI1_Write(0x10|x_union.high);

	while (SPI1->SR & SPI_SR_BSY);

	GPIOA->BSRR = GPIO_ODR_ODR3; // Selecting sending of data
	SPI1_Write(fillMask);   //SPI1_Write(Bit_map[x][page_number]);	// Drawing page on display
	while (SPI1->SR & SPI_SR_BSY);

	GPIOA->BSRR = GPIO_ODR_ODR4; // Ending display selection
}

void clearScreen(uint8_t fillMask)
{
	for (int page = -1; page < 8; page++)
	{		
		for (int x = 0; x < 128; x++)
		{
			paintRegion(x, page, fillMask);
		}
	}
}

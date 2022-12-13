#ifndef DISPLAY_H

    #define DISPLAY_H

    #include "spi.h"

    #define DISPLAY_WIDTH 128
    #define DISPLAY_HEIGHT 64

    void drawPixel(uint8_t x, uint8_t y);

    void paintRegion(uint8_t x, uint8_t page, uint8_t fillMask);

    void clearScreen(uint8_t fillMask);

#endif
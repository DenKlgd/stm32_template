#ifndef VIDEOBUFFER_H

    #define VIDEOBUFFER_H

    #include <stdint.h>
    #include <stdbool.h>
    #include "display.h"

    #define STACK_SIZE 255

    typedef struct DisplayCoords
    {
        uint8_t x;
        uint8_t page;
    } DisplayCoords;

    typedef struct Stack
    {
        uint8_t stack[1024][2];
        uint16_t stackPointer;
    } Stack;

    typedef enum DrawMethod
    {
        Draw,
        ApplyMask
    } DrawMethod;

    typedef struct VideoBuffer
    {
        uint8_t videobuffer [DISPLAY_HEIGHT / 8] [DISPLAY_WIDTH];
        Stack stack;
    } VideoBuffer;

    void initStack(Stack* stack);

    bool push(Stack* stack, uint8_t x, uint8_t page);

    bool pop(Stack* stack, DisplayCoords* coords);

    void initVideoBuffer(VideoBuffer* videobuffer);

    bool drawToBuffer(VideoBuffer* videobuffer, uint8_t x, uint8_t page, uint8_t value, DrawMethod drawMethod);

    void drawFromBufferToDisplay(VideoBuffer* videobuffer);

#endif
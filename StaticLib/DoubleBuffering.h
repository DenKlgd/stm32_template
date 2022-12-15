#ifndef DOUBLE_BUFFERING_H

    #define DOUBLE_BUFFERING_H

    #include <stdint.h>
    #include <stdbool.h>
    #include "display.h"

    typedef struct DisplayCoords
    {
        uint8_t x;
        uint8_t page;
    } DisplayCoords;

    typedef enum DrawMethod
    {
        Draw,
        ApplyMask
    } DrawMethod;

    typedef enum Frame
    {
        PreviousFrame, 
        NextFrame,
    } Frame;

    typedef struct VideoBuffer
    {
        uint8_t nextFrameBuffer [DISPLAY_HEIGHT / 8] [DISPLAY_WIDTH];
        uint8_t prevFrameBuffer [DISPLAY_HEIGHT / 8] [DISPLAY_WIDTH];
        uint8_t* videobuffers[2];
    } VideoBuffer;

    void initVideoBuffer(VideoBuffer* videobuffer);

    void drawToBuffer(VideoBuffer* videobuffer, uint8_t x, uint8_t page, uint8_t value);

    void drawFromBufferToDisplay(VideoBuffer* videobuffer);

#endif
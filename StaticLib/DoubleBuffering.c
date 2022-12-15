#include "DoubleBuffering.h"

void initVideoBuffer(VideoBuffer* videobuffer)
{
	videobuffer->videobuffers[0] = videobuffer->prevFrameBuffer;
	videobuffer->videobuffers[1] = videobuffer->nextFrameBuffer;
	uint8_t* firstBuf = videobuffer->videobuffers[0];
	uint8_t* secondBuf = videobuffer->videobuffers[1];

	for (uint8_t y = 0; y < 8; y++)
	{
		for (uint8_t x = 0; x < 128; x++)
		{
			*(firstBuf + x + y * 128) = 0;
			*(secondBuf + x + y * 128) = 0;
		}
	}
}

void drawToBuffer(VideoBuffer* videobuffer, uint8_t x, uint8_t page, uint8_t value)
{
	uint8_t* firstBuf = videobuffer->videobuffers[NextFrame];
	*(firstBuf + x + page * 128) = value;
}

void compareBuffers(VideoBuffer* videobuffer)
{
	uint8_t* prevBuf = videobuffer->videobuffers[PreviousFrame];
	uint8_t* nextBuf = videobuffer->videobuffers[NextFrame];

	for (uint8_t y = 0; y < 8; y++)
	{
		for (uint8_t x = 0; x < 128; x++)
		{
			if ( *(nextBuf + x + y * 128) != *(prevBuf + x + y * 128))
			{
				paintRegion(x, y / 8, *(nextBuf + x + y * 128));
			}
		}
	}

	uint8_t* tmpBufPtr = videobuffer->videobuffers[0];
	videobuffer->videobuffers[0] = videobuffer->videobuffers[1];
	videobuffer->videobuffers[1] = tmpBufPtr;
}
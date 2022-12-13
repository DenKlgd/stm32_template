#include "videobuffer.h"

void initStack(Stack* stack)
{
	stack->stackPointer = 1024 - 1;
	for (uint16_t i = 0; i < 1024; i++)
	{
		stack->stack[i][0] = 0;
		stack->stack[i][1] = 0;
	}
}

bool push(Stack* stack, uint8_t x, uint8_t page)
{
	if (stack->stackPointer > 1024)
		return false;

	stack->stack[stack->stackPointer][0] = x;
	stack->stack[stack->stackPointer][1] = page;
	stack->stackPointer--;

	return true;
}

bool pop(Stack* stack, DisplayCoords* coords)
{
	if (stack->stackPointer == 1024 - 1)
		return false;
		
	stack->stackPointer++;
	coords->x = stack->stack[stack->stackPointer][0];
	coords->page = stack->stack[stack->stackPointer][1];

	return true;
}

void initVideoBuffer(VideoBuffer* videobuffer)
{
	for (uint8_t y = 0; y < 8; y++)
	{
		for (uint8_t x = 0; x < 128; x++)
		{
			videobuffer->videobuffer[y][x] = 0;
		}
	}
	
	initStack(&(videobuffer->stack));
}

bool drawToBuffer(VideoBuffer* videobuffer, uint8_t x, uint8_t page, uint8_t value, DrawMethod drawMethod)
{
	if (drawMethod == ApplyMask)
		videobuffer->videobuffer[page][x] &= value;
	else
		videobuffer->videobuffer[page][x] |= value;

	return push(&(videobuffer->stack), x, page);
}

void drawFromBufferToDisplay(VideoBuffer* videobuffer)
{
	DisplayCoords coords;

	while (pop(&(videobuffer->stack), &coords))
	{
		paintRegion(coords.x, coords.page, videobuffer->videobuffer[coords.page][coords.x]);	
	}
}

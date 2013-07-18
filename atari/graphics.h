#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>

typedef struct
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} Rgb;

// RRRRRGGG GGGBBBBB
typedef uint16_t HighColor;

typedef uint8_t	 Chunky;

typedef uint16_t Bitplane;

typedef union
{
	Bitplane*	pPlanes;
	Chunky*		pChunky;
	HighColor*	pHc;
	void*		pUnknown;
} PixelPointer;

typedef enum
{
	BitmapTypeUnknown = 0,
	BitmapTypePlanar,	// 1/2/4/8-bit
	BitmapTypeChunky,	// 4/6/8-bit (always 1 pixel = 1 byte)
	BitmapTypeHighColor,	// 5-6-5
	BitmapTypeTrueColor	// 8-8-8
} BitmapType;

typedef struct
{
	int32_t		width;
	int32_t		height;
	int32_t		depth;
	BitmapType	type;
	Rgb*		pPalette;
	PixelPointer	pixels;
} Bitmap;

#endif // GRAPHICS_H

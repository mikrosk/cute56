#include "atari_internal.h"

#include <stdlib.h>
#ifndef HOST
#include <mint/osbind.h>
#endif

#include "dsp_internal.h"
#include "graphics.h"

static volatile int isVblSet;

static Bitmap screen = { 320, 240, 16, BitmapTypeHighColor, NULL, { 0 } };
static HighColor col = 0x0000;

#ifdef HOST
int32_t	Main( const DspWrapperInfo* pDspWrapperInfo )
{
	setDspWrapper( pDspWrapperInfo );
#else
int main( int argc, char* argv[] )
{
#endif
	// your code here
	size_t screenSize = ( screen.width * screen.height * screen.depth / 8 ) + 15;

#ifdef HOST
	screen.pixels.pHc = (HighColor*)malloc( screenSize );
#else
	screen.pixels.pHc = (HighColor*)Mxalloc( screenSize, MX_STRAM );
#endif
	// align on 16 bytes, beware you can't use (M)free anymore
	intptr_t ptr = (intptr_t)screen.pixels.pHc;
	ptr = ( ptr + 15 ) & ~15;
	screen.pixels.pHc = (void*)ptr;

	for( ; ; )
	{
		if( isVblSet )
		{
			// race condition as hell for HOST
			isVblSet = 0;

			HighColor* p = screen.pixels.pHc;
			// wait for vbl
			for( size_t i = 0; i < screenSize / 2; ++i )
			{
				*p++ = col;
			}

			dspSendUnsignedWord( col );
			// do the math :)
			col = dspReceiveWord();
		}
	}

	return 0;
}

void SysExit( int32_t code )
{
	exit( code );
}

void TimerDCallback( void )
{
}

void TimerVblCallback( void )
{
	isVblSet = 1;
}

Bitmap* ScreenGetPhysical( void )
{
	return &screen;
}

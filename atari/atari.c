#include "atari_internal.h"

#include <stdlib.h>

#include "dsp_internal.h"
#include "graphics.h"

#ifdef HOST
int32_t	Main( const DspWrapperInfo* pDspWrapperInfo )
{
	setDspWrapper( pDspWrapperInfo );
#else
int main( int argc, char* argv[] )
{
#endif
	// your code here
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

}

Bitmap* ScreenGetPhysical( void )
{
	return NULL;
}

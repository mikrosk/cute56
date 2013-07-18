#ifndef ATARI_INTERNAL_H
#define ATARI_INTERNAL_H

#include <stdint.h>

#include "atari.h"
#include "dsp.h"
#include "graphics.h"

#ifdef __cplusplus
extern "C"
{
#endif
// these signatures are hardcoded in atarithread.cpp, don't change!
#ifdef HOST
int32_t	Main( const DspWrapperInfo* pDspWrapperInfo );
#endif
void	SysExit( int32_t code );
void	TimerDCallback( void );
void	TimerVblCallback( void );
Bitmap* ScreenGetPhysical( void );
#ifdef __cplusplus
}
#endif

#endif // ATARI_INTERNAL_H

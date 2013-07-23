#ifndef ATARI_INTERNAL_H
#define ATARI_INTERNAL_H

#include <stdint.h>

#include "atari.h"
#include "dsp.h"
#include "graphics.h"
#include "atari_global.h"

#ifdef __cplusplus
extern "C"
{
#endif
// these signatures are hardcoded in atarithread.cpp, musn't change!
#ifdef HOST
ATARISHARED_EXPORT int32_t	Main( const DspWrapperInfo* pDspWrapperInfo );
#endif
ATARISHARED_EXPORT void     SysExit( int32_t code );
ATARISHARED_EXPORT void     TimerDCallback( void );
ATARISHARED_EXPORT void     TimerVblCallback( void );
ATARISHARED_EXPORT Bitmap*  ScreenGetPhysical( void );
#ifdef __cplusplus
}
#endif

#endif // ATARI_INTERNAL_H

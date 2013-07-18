#ifndef VIDEO_SCREEN_ASM_H
#define VIDEO_SCREEN_ASM_H

extern void asm_screen_save( void );
extern void asm_screen_restore( void );
extern void asm_screen_setvram( const void* pPixels );

#endif

#ifndef DSP_INTERNAL_H
#define DSP_INTERNAL_H

#include <stdint.h>

#include "dsp.h"

int8_t	dspReceiveByte( void );
int16_t	dspReceiveWord( void );
int32_t	dspReceiveSignedLong( void );
uint32_t dspReceiveUnsignedLong( void );
void	dspSendLong( int32_t value );

void	dspSendSignedByte( int8_t value );
void	dspSendUnsignedByte( uint8_t value );
void	dspSendSignedWord( int16_t value );
void	dspSendUnsignedWord( uint16_t value );

#endif // DSP_INTERNAL_H

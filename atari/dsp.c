#include "dsp_internal.h"

#ifndef HOST
#include <stdio.h>
#include "dsp_fix-asm.h"

#define DSP_HOST_LONG (volatile int32_t*)0xffffa204
#define DSP_HOST_WORD (volatile int16_t*)0xffffa206
#define DSP_HOST_BYTE (volatile int8_t*)0xffffa207
#define DSP_ISR	      (volatile uint8_t*)0xffffa202

#define WAIT_RX \
	while( ( *(DSP_ISR) & 0x01 ) == 0 );

#define WAIT_TX \
	while( ( *(DSP_ISR) & 0x02 ) == 0 );

int loadDspBinary( const char* sPath )
{
	static char buf[32768 * 3];
	FILE* f = fopen( sPath, "rb" );
	if( f == NULL )
	{
		return 0;
	}

	int len = fread( buf, 1, sizeof( buf ), f );
	fclose( f );

	return asm_dsp_load_program( buf, len / 3 );
}

#else
static DspWrapperInfo dspWrapperInfo;

void setDspWrapper( const DspWrapperInfo* pDspWrapperInfo )
{
	dspWrapperInfo = *pDspWrapperInfo;
}
#endif

int8_t dspReceiveByte( void )
{
#ifdef HOST
	return dspWrapperInfo.dspReceiveByte( dspWrapperInfo.pObject );
#else
	WAIT_RX;
	return *(DSP_HOST_BYTE);
#endif
}

int16_t dspReceiveWord( void )
{
#ifdef HOST
	return dspWrapperInfo.dspReceiveWord( dspWrapperInfo.pObject );
#else
	WAIT_RX;
	return *(DSP_HOST_WORD);
#endif
}

int32_t dspReceiveSignedLong( void )
{
#ifdef HOST
	return dspWrapperInfo.dspReceiveSignedLong( dspWrapperInfo.pObject );
#else
	WAIT_RX;
	return ( *(DSP_HOST_LONG) << 8 ) >> 8;
#endif
}

uint32_t dspReceiveUnsignedLong( void )
{
#ifdef HOST
	return dspWrapperInfo.dspReceiveUnsignedLong( dspWrapperInfo.pObject );
#else
	WAIT_RX;
	return *(DSP_HOST_LONG) & 0x00ffffff;
#endif
}

void dspSendLong( int32_t value )
{
#ifdef HOST
	dspWrapperInfo.dspSendLong( dspWrapperInfo.pObject, value );
#else
	WAIT_TX;
	*(DSP_HOST_LONG) = value;
#endif
}


void dspSendSignedByte( int8_t value )
{
	int32_t dspWord = value;

	dspSendLong( dspWord );
}

void dspSendUnsignedByte( uint8_t value )
{
	uint32_t dspWord = value;

	dspSendLong( dspWord );
}

void dspSendSignedWord( int16_t value )
{
	int32_t dspWord = value;

	dspSendLong( dspWord );
}

void dspSendUnsignedWord( uint16_t value )
{
	uint32_t dspWord = value;

	dspSendLong( dspWord );
}
